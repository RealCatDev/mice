#ifndef   _MICE_PACKETS_PACKET_H_
#define   _MICE_PACKETS_PACKET_H_

#include "mice/data/varint.h"

typedef struct Mice_Packet {
  Mice_Varint length;
  Mice_Varint packetId;
} Mice_Packet;

#endif // _MICE_PACKETS_PACKET_H_