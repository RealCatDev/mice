#include "packets.h"

#include "mice/packets/handshake.h"
#include "mice/packets/status.h"
#include "mice/packets/login.h"

_Mice_State_Packets_Info _mice_packets[_COUNT_MICE_CLIENT_STATES] = {
  [_MICE_CLIENT_STATE_HANDSHAKING] = {
    .in = (_Mice_Packet_Info[]){
      [0] = {
        .typeSize = sizeof(Mice_Packet_Handshake_In),
        .read = (_Mice_Read_Packet)mice_reader_read_packet_handshake_in,
        .write = (_Mice_Write_Packet)mice_writer_write_packet_handshake_in
      }
    },
    .out = NULL
  },
  [_MICE_CLIENT_STATE_STATUS] = {
    .in = (_Mice_Packet_Info[]){
      [0] = {
        .typeSize = 0,
        .read = NULL,
        .write = NULL
      },
      [1] = {
        .typeSize = sizeof(Mice_Packet_Status_In_Ping_Request),
        .read = (_Mice_Read_Packet)mice_reader_read_packet_status_in_ping_request,
        .write = (_Mice_Write_Packet)mice_writer_write_packet_status_in_ping_request
      }
    },
    .out = (_Mice_Packet_Info[]){
      [0] = {
        .typeSize = sizeof(Mice_Packet_Status_Out_Status_Response),
        .read = (_Mice_Read_Packet)mice_reader_read_packet_status_out_status_response,
        .write = (_Mice_Write_Packet)mice_writer_write_packet_status_out_status_response
      },
      [1] = {
        .typeSize = sizeof(Mice_Packet_Status_Out_Ping_Response),
        .read = (_Mice_Read_Packet)mice_reader_read_packet_status_out_ping_response,
        .write = (_Mice_Write_Packet)mice_writer_write_packet_status_out_ping_response
      }
    }
  },
  [_MICE_CLIENT_STATE_LOGIN] = {
    .in = (_Mice_Packet_Info[]){
      [0] = {
        .typeSize = sizeof(Mice_Packet_Login_In_Login_Start),
        .read = (_Mice_Read_Packet)mice_reader_read_packet_login_in_login_start,
        .write = (_Mice_Write_Packet)mice_writer_write_packet_login_in_login_start
      }
    },
    .out = (_Mice_Packet_Info[]){
      [2] = {
        .typeSize = sizeof(Mice_Packet_Login_Out_Login_Success),
        .read = (_Mice_Read_Packet)mice_reader_read_packet_login_out_login_success,
        .write = (_Mice_Write_Packet)mice_writer_write_packet_login_out_login_success
      }
    }
  },
};