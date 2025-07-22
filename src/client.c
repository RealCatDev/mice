#include "client.h"

#include "packets/packets.h"

#include "mice/packets/packet.h"
#include "mice/data/varint.h"

#include "mice/io/reader.h"

#include "mice/data/da.h"

#include <stdlib.h>
#include <assert.h>

bool _mice_client_get_packet(_Mice_Client *client, Mice_Packet **packet) {
  if (client->state == _COUNT_MICE_CLIENT_STATES) return false;
  size_t length = client->recvBuffer.count-client->recvBuffer.cursor;

  Mice_Varint packetLength = 0;
  const char *begin = &client->recvBuffer.items[client->recvBuffer.cursor];
  const char *ptr = mice_get_varint(length, begin, &packetLength);
  if (!ptr) return false;
  if (length < (size_t)packetLength) return false;

  Mice_Reader reader = {
    .data = begin,
    .count = packetLength+(ptr-begin)
  };

  Mice_Packet base = {0};
  base.length = mice_reader_read_varint(&reader);
  base.packetId = mice_reader_read_varint(&reader);

  assert(_mice_packets[client->state].in);
  _Mice_Packet_Info packetInfo = _mice_packets[client->state].in[base.packetId];
  if (packetInfo.typeSize < sizeof(Mice_Packet)) packetInfo.typeSize = sizeof(Mice_Packet);
  *packet = malloc(packetInfo.typeSize);
  assert(*packet);
  **packet = base;

  if (packetInfo.read) packetInfo.read(&reader, *packet);
  assert(reader.cursor == reader.count);
  client->recvBuffer.cursor += reader.cursor;

  return true;
}

void _mice_client_send_packet(_Mice_Client *client, Mice_Packet *packet) {
  assert(client->state < _COUNT_MICE_CLIENT_STATES);

  Mice_Writer writer = {0};
  mice_writer_write_varint(&writer, packet->packetId);

  assert(_mice_packets[client->state].out);
  _Mice_Packet_Info packetInfo = _mice_packets[client->state].out[packet->packetId];
  if (packetInfo.write) packetInfo.write(&writer, packet);

  Mice_Varint packetLength = writer.count;
  mice_writer_write_varint(&writer, packetLength);

  if (client->sendBuffer.cursor) {
    client->sendBuffer.count -= client->sendBuffer.cursor;
    memmove(client->sendBuffer.items, &client->sendBuffer.items[client->sendBuffer.cursor], client->sendBuffer.count);
    client->sendBuffer.cursor = 0;
  }

  MICE_DA_APPEND_MANY(&client->sendBuffer, &writer.items[packetLength], writer.count-packetLength);
  MICE_DA_APPEND_MANY(&client->sendBuffer,  writer.items,packetLength);
  MICE_DA_FREE(&writer);
}