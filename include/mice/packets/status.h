#ifndef   _MICE_PACKETS_STATUS_H_
#define   _MICE_PACKETS_STATUS_H_

#include "mice/packets/packet.h"

#include "mice/data/varint.h"
#include "mice/data/string.h"

#include <stdint.h>

typedef struct Mice_Packet_Status_In_Ping_Request {
  int64_t payload;
} Mice_Packet_Status_In_Ping_Request;



typedef struct Mice_Packet_Status_Out_Status_Response {
  Mice_String jsonResponse;
} Mice_Packet_Status_Out_Status_Response;

typedef struct Mice_Packet_Status_Out_Ping_Response {
  int64_t payload;
} Mice_Packet_Status_Out_Ping_Response;

#endif // _MICE_PACKETS_STATUS_H_