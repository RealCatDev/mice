#include "mice/packets/handshake.h"

void mice_reader_read_packet_handshake_in(Mice_Reader *reader, Mice_Packet_Handshake_In *packet) {
  packet->protocolVersion = mice_reader_read_varint(reader);
  packet->serverAddress = mice_reader_read_string(reader);
  packet->serverPort = mice_reader_read_ushort(reader);
  packet->nextState = mice_reader_read_varint(reader);
}

void mice_writer_write_packet_handshake_in(Mice_Writer *writer, Mice_Packet_Handshake_In *packet) {
  mice_writer_write_varint(writer, packet->protocolVersion);
  mice_writer_write_string(writer, packet->serverAddress);
  mice_writer_write_ushort(writer, packet->serverPort);
  mice_writer_write_varint(writer, packet->nextState);
}