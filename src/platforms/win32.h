#ifndef   __MICE_PLATFORMS_WIN32_H_
#define   __MICE_PLATFORMS_WIN32_H_

#include "server.h"

#include <winsock2.h>
#include <windows.h>

typedef struct _Mice_Server_Win32 {
  WSADATA wsaData;
  SOCKET listenSocket;

  struct {
    WSAPOLLFD *items;
    size_t capacity;
    size_t count;
  } fds;
} _Mice_Server_Win32;

#endif // __MICE_PLATFORMS_WIN32_H_