#ifndef   __MICE_PACKETS_H_
#define   __MICE_PACKETS_H_

#include "mice/packets/packet.h"

#include "mice/io/reader.h"
#include "mice/io/writer.h"

#include "client.h"

#include <stddef.h>

typedef void (*_Mice_Read_Packet)(Mice_Reader *, void *);
typedef void (*_Mice_Write_Packet)(Mice_Writer *, void *);

typedef struct _Mice_Packet_Info {
  size_t typeSize; // size of the packet's structure type
  _Mice_Read_Packet read;
  _Mice_Write_Packet write;
} _Mice_Packet_Info;

typedef struct _Mice_State_Packets_Info {
  _Mice_Packet_Info *in, *out;
} _Mice_State_Packets_Info;

extern _Mice_State_Packets_Info _mice_packets[_COUNT_MICE_CLIENT_STATES];

#endif // __MICE_PACKETS_H_