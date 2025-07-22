#include "mice/packets/status.h"

void mice_reader_read_packet_status_in_ping_request(Mice_Reader *reader, Mice_Packet_Status_In_Ping_Request *packet) {
  packet->payload = mice_reader_read_long(reader);
}

void mice_writer_write_packet_status_in_ping_request(Mice_Writer *writer, Mice_Packet_Status_In_Ping_Request *packet) {
  mice_writer_write_long(writer, packet->payload);
}

void mice_reader_read_packet_status_out_status_response(Mice_Reader *reader, Mice_Packet_Status_Out_Status_Response *packet) {
  packet->jsonResponse = mice_reader_read_string(reader);
}

void mice_writer_write_packet_status_out_status_response(Mice_Writer *writer, Mice_Packet_Status_Out_Status_Response *packet) {
  mice_writer_write_string(writer, packet->jsonResponse);
}

void mice_reader_read_packet_status_out_ping_response(Mice_Reader *reader, Mice_Packet_Status_Out_Ping_Response *packet) {
  packet->payload = mice_reader_read_long(reader);
}

void mice_writer_write_packet_status_out_ping_response(Mice_Writer *writer, Mice_Packet_Status_Out_Ping_Response *packet) {
  mice_writer_write_long(writer, packet->payload);
}