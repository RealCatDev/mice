#ifdef _WIN32

// TODO: Add error handling
// TODO: Add configuration

#include "win32.h"

#define MICE_DA_DEFAULT_SIZE (1<<3)
#include "mice/data/da.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void _mice_server_platform_init(_Mice_Server *server) {
  _Mice_Server_Win32 *platform = server->platform = malloc(sizeof(_Mice_Server_Win32));
  assert(platform);
  memset(platform, 0, sizeof(*platform));

  assert(WSAStartup(MAKEWORD(2, 2), &platform->wsaData) == 0);

  platform->listenSocket = socket(AF_INET, SOCK_STREAM, 0);
  assert(platform->listenSocket != INVALID_SOCKET);

  u_long mode = 1;
  ioctlsocket(platform->listenSocket, FIONBIO, &mode);

  SOCKADDR_IN addr = {
    .sin_family = AF_INET,
    .sin_port = htons(25565),
    .sin_addr.s_addr = INADDR_ANY
  };

  assert(bind(platform->listenSocket, (SOCKADDR*)&addr, sizeof(addr)) == 0);

  assert(listen(platform->listenSocket, SOMAXCONN) == 0);

  MICE_DA_APPEND(&platform->fds, ((WSAPOLLFD){
    .fd = platform->listenSocket,
    .events = POLLIN,
    .revents = 0
  }));
}

void _mice_server_platform_cleanup(_Mice_Server *server) {
  _Mice_Server_Win32 *platform = server->platform;

  assert(closesocket(platform->listenSocket) == 0);

  MICE_DA_FREE(&platform->fds);
  free(platform);

  WSACleanup();
}

void _mice_server_platform_update(_Mice_Server *server) {
  _Mice_Server_Win32 *platform = server->platform;

  for (size_t i = 0; i < server->clients.count; ++i) {
    _Mice_Client client = MICE_DA_AT(&server->clients, i);
    bool send = (client.sendBuffer.count-client.sendBuffer.cursor) > 0;
    if (send) MICE_DA_AT(&platform->fds, i+1).events |= POLLOUT;
    else MICE_DA_AT(&platform->fds, i+1).events &= ~POLLOUT;
  }

  int n = WSAPoll(platform->fds.items, platform->fds.count, 0);
  assert(n != SOCKET_ERROR);
  if (!n) return;

  for (size_t i = 0; i < platform->fds.count;) {
    WSAPOLLFD *pollFd = &MICE_DA_AT(&platform->fds, i);

    if (pollFd->revents & (POLLERR | POLLHUP | POLLNVAL)) {
      assert(i);
      goto clientDisconnect;
    }

    if (i == 0 && pollFd->revents & POLLIN) {
      SOCKET clientSocket  = accept(platform->listenSocket, NULL, NULL);
      assert(clientSocket != INVALID_SOCKET);

      MICE_DA_APPEND(&server->clients, ((_Mice_Client){0}));
      MICE_DA_APPEND(&platform->fds, ((WSAPOLLFD){
        .fd = clientSocket,
        .events = POLLIN,
        .revents = 0
      }));
    } else if (i != 0) {
      _Mice_Client *client = &MICE_DA_AT(&server->clients, i-1);
      if (client->state == _COUNT_MICE_CLIENT_STATES) goto clientDisconnect;

      if (pollFd->revents & POLLIN) {
        enum { BUF_CAPACITY = 2048 }; // TODO: Customize in config
        char buf[BUF_CAPACITY];

        int n = recv(pollFd->fd, buf, BUF_CAPACITY, 0);
        assert(n >= 0);
        if (n == 0) goto clientDisconnect;

        if (client->recvBuffer.cursor) {
          client->recvBuffer.count -= client->recvBuffer.cursor;
          memmove(client->recvBuffer.items, &client->recvBuffer.items[client->recvBuffer.cursor], client->recvBuffer.count);
          client->recvBuffer.cursor = 0;
        }

        MICE_DA_APPEND_MANY(&client->recvBuffer, buf, n);
      }

      if (pollFd->revents & POLLOUT) {
        int n = send(pollFd->fd, &client->sendBuffer.items[client->sendBuffer.cursor], client->sendBuffer.count-client->sendBuffer.cursor, 0);
        assert(n >= 0);
        if (n == 0) goto clientDisconnect;

        client->sendBuffer.cursor += n;
      }
    }

    ++i;
    continue;
  clientDisconnect:
    closesocket(pollFd->fd);

    _Mice_Client *client = &MICE_DA_AT(&server->clients, i-1);
    MICE_DA_FREE(&client->recvBuffer);
    MICE_DA_FREE(&client->sendBuffer);

    MICE_DA_FAST_REMOVE(&server->clients, i-1);
    MICE_DA_FAST_REMOVE(&platform->fds, i);
  }
}

#endif // _WIN32