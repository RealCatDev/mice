#include "mice/io/reader.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

uint8_t mice_reader_read_ubyte(Mice_Reader *reader) {
  assert(reader && reader->cursor+1 <= reader->count);
  return reader->data[reader->cursor++] & 0xFF;
}

uint16_t mice_reader_read_ushort(Mice_Reader *reader) {
  assert(reader && reader->cursor+2 <= reader->count);
  uint16_t a  = (uint16_t)(reader->data[reader->cursor++]&0xFF) << 8;
  return   a |  (uint16_t)(reader->data[reader->cursor++]&0xFF) << 0;
}

uint32_t mice_reader_read_uint(Mice_Reader *reader) {
  assert(reader && reader->cursor+4 <= reader->count);
  uint32_t a  = (uint32_t)(reader->data[reader->cursor++]&0xFF) << 24;
           a |= (uint32_t)(reader->data[reader->cursor++]&0xFF) << 16;
           a |= (uint32_t)(reader->data[reader->cursor++]&0xFF) <<  8;
  return   a |  (uint32_t)(reader->data[reader->cursor++]&0xFF) <<  0;
}

uint64_t mice_reader_read_ulong(Mice_Reader *reader) {
  assert(reader && reader->cursor+8 <= reader->count);
  uint64_t a  = (uint64_t)(reader->data[reader->cursor++]&0xFF) << 56;
           a |= (uint64_t)(reader->data[reader->cursor++]&0xFF) << 48;
           a |= (uint64_t)(reader->data[reader->cursor++]&0xFF) << 40;
           a |= (uint64_t)(reader->data[reader->cursor++]&0xFF) << 32;
           a |= (uint64_t)(reader->data[reader->cursor++]&0xFF) << 24;
           a |= (uint64_t)(reader->data[reader->cursor++]&0xFF) << 16;
           a |= (uint64_t)(reader->data[reader->cursor++]&0xFF) <<  8;
  return   a |  (uint64_t)(reader->data[reader->cursor++]&0xFF) <<  0;
}

int8_t mice_reader_read_byte(Mice_Reader *reader) {
  return (int8_t)mice_reader_read_ubyte(reader);
}

int16_t mice_reader_read_short(Mice_Reader *reader) {
  return (int16_t)mice_reader_read_ushort(reader);
}

int32_t mice_reader_read_int(Mice_Reader *reader) {
  return (int32_t)mice_reader_read_uint(reader);
}

int64_t mice_reader_read_long(Mice_Reader *reader) {
  return (int64_t)mice_reader_read_ulong(reader);
}

float mice_reader_read_float(Mice_Reader *reader) {
  int32_t a = mice_reader_read_int(reader);
  return *(float*)&a;
}

double mice_reader_read_double(Mice_Reader *reader) {
  int64_t a = mice_reader_read_long(reader);
  return *(double*)&a;
}

void mice_reader_read_bytes(Mice_Reader *reader, size_t count, char bytes[count]) {
  assert(reader && reader->cursor+count < reader->count);
  memcpy(bytes, &reader->data[reader->cursor], count);
  reader->cursor += count;
}