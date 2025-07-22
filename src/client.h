#ifndef   __MICE_CLIENT_H_
#define   __MICE_CLIENT_H_

#include "mice/packets/packet.h"

#include <stddef.h>
#include <stdbool.h>

typedef enum _Mice_Client_State {
  _MICE_CLIENT_STATE_HANDSHAKING = 0,
  _MICE_CLIENT_STATE_STATUS,
  _MICE_CLIENT_STATE_LOGIN,
  _MICE_CLIENT_STATE_PLAY,

  _COUNT_MICE_CLIENT_STATES
} _Mice_Client_State;

typedef struct _Mice_Client {
  _Mice_Client_State state;

  struct {
    char *items;
    size_t capacity;
    size_t count;
    size_t cursor;
  } recvBuffer;

  struct {
    char *items;
    size_t capacity;
    size_t count;
    size_t cursor;
  } sendBuffer;
} _Mice_Client;

bool _mice_client_get_packet(_Mice_Client *client, Mice_Packet **packet);
void _mice_client_send_packet(_Mice_Client *client, Mice_Packet *packet);

#endif // __MICE_CLIENT_H_