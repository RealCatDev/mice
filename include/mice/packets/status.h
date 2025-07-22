#ifndef   _MICE_PACKETS_STATUS_H_
#define   _MICE_PACKETS_STATUS_H_

#include "mice/packets/packet.h"

#include "mice/data/varint.h"
#include "mice/data/string.h"

#include "mice/io/reader.h"
#include "mice/io/writer.h"

#include <stdint.h>

typedef struct Mice_Packet_Status_In_Ping_Request {
  Mice_Packet base;
  int64_t payload;
} Mice_Packet_Status_In_Ping_Request;

void mice_reader_read_packet_status_in_ping_request(Mice_Reader *reader, Mice_Packet_Status_In_Ping_Request *packet);
void mice_writer_write_packet_status_in_ping_request(Mice_Writer *writer, Mice_Packet_Status_In_Ping_Request *packet);



typedef struct Mice_Packet_Status_Out_Status_Response {
  Mice_Packet base;
  Mice_String jsonResponse;
} Mice_Packet_Status_Out_Status_Response;

void mice_reader_read_packet_status_out_status_response(Mice_Reader *reader, Mice_Packet_Status_Out_Status_Response *packet);
void mice_writer_write_packet_status_out_status_response(Mice_Writer *writer, Mice_Packet_Status_Out_Status_Response *packet);

typedef struct Mice_Packet_Status_Out_Ping_Response {
  Mice_Packet base;
  int64_t payload;
} Mice_Packet_Status_Out_Ping_Response;

void mice_reader_read_packet_status_out_ping_response(Mice_Reader *reader, Mice_Packet_Status_Out_Ping_Response *packet);
void mice_writer_write_packet_status_out_ping_response(Mice_Writer *writer, Mice_Packet_Status_Out_Ping_Response *packet);

#endif // _MICE_PACKETS_STATUS_H_