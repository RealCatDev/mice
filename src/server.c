#include "server.h"

#include "mice/data/da.h"

#include "mice/packets/handshake.h"
#include "mice/packets/status.h"
#include "mice/packets/login.h"

#include "mice/data/stringBuilder.h"
#include "mice/data/json.h"

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
      Mice_Json *json = mice_json_create_object();

      Mice_Json *version = mice_json_create_object();
      mice_json_add_child(version, mice_json_create_string("1.19.4"), "name");
      mice_json_add_child(version, mice_json_create_number(762), "protocol");
      mice_json_add_child(json, version, "version");

      Mice_Json *players = mice_json_create_object();
      mice_json_add_child(players, mice_json_create_number(20), "max");
      mice_json_add_child(players, mice_json_create_number(0),  "online");
      mice_json_add_child(players, mice_json_create_array(),    "sample");
      mice_json_add_child(json, players, "players");

      Mice_Json *description = mice_json_create_object();
      mice_json_add_child(description, mice_json_create_string("Mice server"), "text");
      mice_json_add_child(description, mice_json_create_string("red"), "color");
      mice_json_add_child(json, description, "description");

      Mice_String_Builder jsonSb = {0};
      mice_json_write_to_string_builder(json, &jsonSb);

      Mice_Packet_Status_Out_Status_Response response = { .base.packetId = 0 };
      response.jsonResponse = (Mice_String){ .data = jsonSb.items, .length = jsonSb.count };
      _mice_client_send_packet(sender, (Mice_Packet*)&response);

      MICE_DA_FREE(&jsonSb);
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