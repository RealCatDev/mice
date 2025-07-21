#include "mice/data/uuid.h"

#include "mice/hash/md5.h"

#include <ctype.h>
#include <string.h>
#include <assert.h>

Mice_Uuid mice_reader_read_uuid(Mice_Reader *reader) {
  Mice_Uuid uuid = {0};
  uuid.bytes[ 0] = mice_reader_read_ubyte(reader);
  uuid.bytes[ 1] = mice_reader_read_ubyte(reader);
  uuid.bytes[ 2] = mice_reader_read_ubyte(reader);
  uuid.bytes[ 3] = mice_reader_read_ubyte(reader);
  uuid.bytes[ 4] = mice_reader_read_ubyte(reader);
  uuid.bytes[ 5] = mice_reader_read_ubyte(reader);
  uuid.bytes[ 6] = mice_reader_read_ubyte(reader);
  uuid.bytes[ 7] = mice_reader_read_ubyte(reader);
  uuid.bytes[ 8] = mice_reader_read_ubyte(reader);
  uuid.bytes[ 9] = mice_reader_read_ubyte(reader);
  uuid.bytes[10] = mice_reader_read_ubyte(reader);
  uuid.bytes[11] = mice_reader_read_ubyte(reader);
  uuid.bytes[12] = mice_reader_read_ubyte(reader);
  uuid.bytes[13] = mice_reader_read_ubyte(reader);
  uuid.bytes[14] = mice_reader_read_ubyte(reader);
  uuid.bytes[15] = mice_reader_read_ubyte(reader);

  return uuid;
}

void mice_writer_write_uuid(Mice_Writer *writer, Mice_Uuid uuid) {
  mice_writer_write_ubyte(writer, uuid.bytes[ 0]);
  mice_writer_write_ubyte(writer, uuid.bytes[ 1]);
  mice_writer_write_ubyte(writer, uuid.bytes[ 2]);
  mice_writer_write_ubyte(writer, uuid.bytes[ 3]);
  mice_writer_write_ubyte(writer, uuid.bytes[ 4]);
  mice_writer_write_ubyte(writer, uuid.bytes[ 5]);
  mice_writer_write_ubyte(writer, uuid.bytes[ 6]);
  mice_writer_write_ubyte(writer, uuid.bytes[ 7]);
  mice_writer_write_ubyte(writer, uuid.bytes[ 8]);
  mice_writer_write_ubyte(writer, uuid.bytes[ 9]);
  mice_writer_write_ubyte(writer, uuid.bytes[10]);
  mice_writer_write_ubyte(writer, uuid.bytes[11]);
  mice_writer_write_ubyte(writer, uuid.bytes[12]);
  mice_writer_write_ubyte(writer, uuid.bytes[13]);
  mice_writer_write_ubyte(writer, uuid.bytes[14]);
  mice_writer_write_ubyte(writer, uuid.bytes[15]);
}

static uint8_t hex_to_int(char c) {
  c = tolower(c);
  if (c >= '0' && c <= '9') return c - '0';
  if (c >= 'a' && c <= 'f') return c - 'a' + 10;
  assert(0);
  return 0;
}

static uint8_t read_byte(const char string[2]) {
  return (hex_to_int(string[0]) << 4)|
         (hex_to_int(string[1]) << 0);
}

Mice_Uuid mice_uuid_from_string(const char string[36]) {
  Mice_Uuid uuid = {0};
  uuid.bytes[ 0] = read_byte(&string[0]);
  uuid.bytes[ 1] = read_byte(&string[2]);
  uuid.bytes[ 2] = read_byte(&string[4]);
  uuid.bytes[ 3] = read_byte(&string[6]);
  assert(string[8] == '-');
  uuid.bytes[ 4] = read_byte(&string[9]);
  uuid.bytes[ 5] = read_byte(&string[11]);
  assert(string[13] == '-');
  uuid.bytes[ 6] = read_byte(&string[14]);
  uuid.bytes[ 7] = read_byte(&string[16]);
  assert(string[18] == '-');
  uuid.bytes[ 8] = read_byte(&string[19]);
  uuid.bytes[ 9] = read_byte(&string[21]);
  assert(string[23] == '-');
  uuid.bytes[10] = read_byte(&string[24]);
  uuid.bytes[11] = read_byte(&string[26]);
  uuid.bytes[12] = read_byte(&string[28]);
  uuid.bytes[13] = read_byte(&string[30]);
  uuid.bytes[14] = read_byte(&string[32]);
  uuid.bytes[15] = read_byte(&string[34]);

  return uuid;
}

static void write_byte(uint8_t byte, char string[2]) {
  uint8_t a = ((byte >> 4) & 0x0F);
  string[0] = '0' + a + ((a >= 10)?39:0);
  uint8_t b = ((byte >> 0) & 0x0F);
  string[1] = '0' + b + ((b >= 10)?39:0);
}

void mice_uuid_to_string(Mice_Uuid uuid, char string[36]) {
  write_byte(uuid.bytes[0],  &string[0]);
  write_byte(uuid.bytes[1],  &string[2]);
  write_byte(uuid.bytes[2],  &string[4]);
  write_byte(uuid.bytes[3],  &string[6]);
  string[8] = '-';
  write_byte(uuid.bytes[4],  &string[9]);
  write_byte(uuid.bytes[5],  &string[11]);
  string[13] = '-';
  write_byte(uuid.bytes[6],  &string[14]);
  write_byte(uuid.bytes[7],  &string[16]);
  string[18] = '-';
  write_byte(uuid.bytes[8],  &string[19]);
  write_byte(uuid.bytes[9],  &string[21]);
  string[23] = '-';
  write_byte(uuid.bytes[10], &string[24]);
  write_byte(uuid.bytes[11], &string[26]);
  write_byte(uuid.bytes[12], &string[28]);
  write_byte(uuid.bytes[13], &string[30]);
  write_byte(uuid.bytes[14], &string[32]);
  write_byte(uuid.bytes[15], &string[34]);
}

Mice_Uuid mice_uuid_offline(size_t usernameLength, const char username[usernameLength]) {
  const char prefix[14] = "OfflinePlayer:";

  size_t bufferLength = 14+usernameLength;
  char buffer[bufferLength];
  memcpy(&buffer[ 0], prefix, 14);
  memcpy(&buffer[14], username, usernameLength);

  Mice_Md5_State hash = mice_md5_hash(bufferLength, buffer);
  uint32_t parts[4] = { hash.a, hash.b, hash.c, hash.d };

  Mice_Uuid uuid = {0};
  memcpy(uuid.bytes, parts, sizeof(parts));

  uuid.bytes[6] = (uuid.bytes[6] & 0x0F) | 0x30;
  uuid.bytes[8] = (uuid.bytes[8] & 0x3F) | 0x80;
  return uuid;
}