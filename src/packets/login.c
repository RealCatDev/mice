#include "mice/packets/login.h"

#include <stdlib.h>
#include <assert.h>

void mice_reader_read_packet_login_in_login_start(Mice_Reader *reader, Mice_Packet_Login_In_Login_Start *packet) {
  packet->name = mice_reader_read_string(reader);
  packet->hasPlayerUuid = mice_reader_read_byte(reader);
  if (packet->hasPlayerUuid) packet->uuid = mice_reader_read_uuid(reader);
}

void mice_writer_write_packet_login_in_login_start(Mice_Writer *writer, Mice_Packet_Login_In_Login_Start *packet) {
  mice_writer_write_string(writer, packet->name);
  mice_writer_write_byte(writer, packet->hasPlayerUuid);
  if (packet->hasPlayerUuid) mice_writer_write_uuid(writer, packet->uuid);
}

void mice_reader_read_packet_login_out_login_success(Mice_Reader *reader, Mice_Packet_Login_Out_Login_Success *packet) {
  packet->uuid = mice_reader_read_uuid(reader);
  packet->username = mice_reader_read_string(reader);
  packet->propertyCount = mice_reader_read_varint(reader);
  if (!packet->propertyCount) return;

  packet->properties = malloc(sizeof(*packet->properties) * packet->propertyCount);
  assert(packet->properties);
  for (Mice_Varint i = 0; i < packet->propertyCount; ++i) {
    packet->properties[i].name = mice_reader_read_string(reader);
    packet->properties[i].value = mice_reader_read_string(reader);
    packet->properties[i].isSigned = mice_reader_read_byte(reader);
    packet->properties[i].signature = mice_reader_read_string(reader);
  }
}

void mice_writer_write_packet_login_out_login_success(Mice_Writer *writer, Mice_Packet_Login_Out_Login_Success *packet) {
  mice_writer_write_uuid(writer, packet->uuid);
  mice_writer_write_string(writer, packet->username);
  mice_writer_write_varint(writer, packet->propertyCount);

  for (Mice_Varint i = 0; i < packet->propertyCount; ++i) {
    mice_writer_write_string(writer, packet->properties[i].name);
    mice_writer_write_string(writer, packet->properties[i].value);
    mice_writer_write_byte(writer, packet->properties[i].isSigned);
    mice_writer_write_string(writer, packet->properties[i].signature);
  }
}