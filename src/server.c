#include "server.h"

#include "mice/data/da.h"

#include "mice/packets/handshake.h"
#include "mice/packets/status.h"
#include "mice/packets/login.h"

void _mice_server_run(_Mice_Server *server) {
  _mice_server_platform_init(server);

  while (!server->stopped) {
    _mice_server_platform_update(server);

    for (size_t i = 0; i < server->clients.count; ++i) {
      _Mice_Client *client = &server->clients.items[i];

      Mice_Packet *packet = NULL;
      while (_mice_client_get_packet(client, &packet)) {
        _mice_server_process_packet(server, client, packet);
        free(packet);
      }
    }

    _mice_server_tick(server);
  }

  _mice_server_platform_cleanup(server);
}

void _mice_server_tick(_Mice_Server *server) {
  (void)server;

  // TODO: Implement
  // TODO: Add TPS limit
}

void _mice_server_process_packet(_Mice_Server *server, _Mice_Client *sender, Mice_Packet *packet) {
  switch (sender->state) {
  case _MICE_CLIENT_STATE_HANDSHAKING: {
    assert(packet->packetId == 0);

    Mice_Packet_Handshake_In *p = (Mice_Packet_Handshake_In*)packet;
    assert(p->protocolVersion == 762);
    if (p->nextState < 1 || p->nextState > 2) sender->state = _COUNT_MICE_CLIENT_STATES;
    else sender->state = (_Mice_Client_State)p->nextState;
  } break;
  case _MICE_CLIENT_STATE_STATUS: {
    if (packet->packetId == 0) {
      Mice_Packet_Status_Out_Status_Response response = { .base.packetId = 0 };
      response.jsonResponse = MICE_STRING("{\"version\":{\"name\":\"1.19.4\",\"protocol\":762},\"players\":{\"max\":69,\"online\":0,\"sample\":[]},\"description\":{\"text\":\"Mice server\"}}");
      _mice_client_send_packet(sender, (Mice_Packet*)&response);
    } else if (packet->packetId == 1) {
      Mice_Packet_Status_Out_Ping_Response response = { .base.packetId = 1 };
      response.payload = ((Mice_Packet_Status_In_Ping_Request*)packet)->payload;
      _mice_client_send_packet(sender, (Mice_Packet*)&response);
    } else assert(0);
  } break;
  case _MICE_CLIENT_STATE_LOGIN: { // TODO: Implement encryption
    if (packet->packetId == 0) {
      Mice_Packet_Login_In_Login_Start *p = (Mice_Packet_Login_In_Login_Start*)packet;

      Mice_Packet_Login_Out_Login_Success response = { .base.packetId = 2 };
      if (!p->hasPlayerUuid) response.uuid = mice_uuid_offline(p->name.length, p->name.data);
      else response.uuid = p->uuid;

      response.username = p->name;
      response.propertyCount = 0; // TODO: Query from (if in online mode): https://sessionserver.mojang.com/session/minecraft/profile/<UUID>

      _mice_client_send_packet(sender, (Mice_Packet*)&response);

      sender->state = _MICE_CLIENT_STATE_PLAY;
    } else if (packet->packetId == 1) {
      assert(0); // TODO: Implement
    } else if (packet->packetId == 2) {
      assert(0); // TODO: Implement
    } else assert(0);
  } break;
  case _MICE_CLIENT_STATE_PLAY: {

  } break;
  case _COUNT_MICE_CLIENT_STATES: break;
  default: assert(0);
  }

  (void)server;
  (void)sender;
  (void)packet;
}