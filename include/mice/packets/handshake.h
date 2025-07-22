#ifndef   _MICE_PACKETS_HANDSHAKE_H_
#define   _MICE_PACKETS_HANDSHAKE_H_

#include "mice/packets/packet.h"

#include "mice/data/varint.h"
#include "mice/data/string.h"

#include "mice/io/reader.h"
#include "mice/io/writer.h"

#include <stdint.h>

typedef struct Mice_Packet_Handshake_In {
  Mice_Packet base;
  Mice_Varint protocolVersion;
  Mice_String serverAddress;
  uint16_t serverPort;
  Mice_Varint nextState;
} Mice_Packet_Handshake_In;

void mice_reader_read_packet_handshake_in(Mice_Reader *reader, Mice_Packet_Handshake_In *packet);
void mice_writer_write_packet_handshake_in(Mice_Writer *writer, Mice_Packet_Handshake_In *packet);

#endif // _MICE_PACKETS_HANDSHAKE_H_