#ifndef   __MICE_SERVER_H_
#define   __MICE_SERVER_H_

#include "client.h"

#include "mice/packets/packet.h"

#include <stddef.h>
#include <stdbool.h>

typedef struct _Mice_Server {
  struct {
    _Mice_Client *items;
    size_t capacity;
    size_t count;
  } clients;

  void *platform;
  bool stopped;
} _Mice_Server;

void _mice_server_run(_Mice_Server *server);
void _mice_server_tick(_Mice_Server *server);

void _mice_server_process_packet(_Mice_Server *server, _Mice_Client *sender, Mice_Packet *packet);

void _mice_server_platform_init(_Mice_Server *server);
void _mice_server_platform_cleanup(_Mice_Server *server);
void _mice_server_platform_update(_Mice_Server *server);

#endif // __MICE_SERVER_H_