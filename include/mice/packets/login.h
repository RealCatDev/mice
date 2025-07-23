#ifndef   _MICE_PACKETS_LOGIN_H_
#define   _MICE_PACKETS_LOGIN_H_

#include "mice/packets/packet.h"

#include "mice/data/varint.h"
#include "mice/data/string.h"
#include "mice/data/uuid.h"

#include "mice/io/reader.h"
#include "mice/io/writer.h"

#include <stdint.h>

typedef struct Mice_Packet_Login_In_Login_Start {
  Mice_Packet base;
  Mice_String name;
  bool hasPlayerUuid;
  Mice_Uuid uuid;
} Mice_Packet_Login_In_Login_Start;

void mice_reader_read_packet_login_in_login_start(Mice_Reader *reader, Mice_Packet_Login_In_Login_Start *packet);
void mice_writer_write_packet_login_in_login_start(Mice_Writer *writer, Mice_Packet_Login_In_Login_Start *packet);

typedef struct Mice_Packet_Login_In_Encryption_Response {
  Mice_Packet base;
  Mice_Varint sharedSecretLength;
  char *sharedSecret;
  Mice_Varint verifyTokenLength;
  char *verifyToken;
} Mice_Packet_Login_In_Encryption_Response;

typedef struct Mice_Packet_Login_In_Login_Plugin_Response {
  Mice_Packet base;
  Mice_Varint messageId;
  bool successful;
  size_t dataLength;
  char *data;
} Mice_Packet_Login_In_Login_Plugin_Response;



typedef struct Mice_String Mice_Chat; // Chat is just a JSON string

typedef struct Mice_Packet_Login_Out_Disconnect {
  Mice_Packet base;
  Mice_Chat reason;
} Mice_Packet_Login_Out_Disconnect;

typedef struct Mice_Packet_Login_Out_Encryption_Request {
  Mice_Packet base;
  Mice_String serverId;
  Mice_Varint publicKeyLength;
  char *publicKey;
  Mice_Varint verifyTokenLength;
  char *verifyToken;
} Mice_Packet_Login_Out_Encryption_Request;

typedef struct Mice_Packet_Login_Out_Login_Success {
  Mice_Packet base;
  Mice_Uuid uuid;
  Mice_String username;
  Mice_Varint propertyCount;
  struct {
    Mice_String name;
    Mice_String value;
    bool isSigned;
    Mice_String signature;
  } *properties;
} Mice_Packet_Login_Out_Login_Success;

void mice_reader_read_packet_login_out_login_success(Mice_Reader *reader, Mice_Packet_Login_Out_Login_Success *packet);
void mice_writer_write_packet_login_out_login_success(Mice_Writer *writer, Mice_Packet_Login_Out_Login_Success *packet);

#endif // _MICE_PACKETS_LOGIN_H_