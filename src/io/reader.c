#include "mice/io/reader.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

int8_t mice_reader_read_byte(Mice_Reader *reader) {
  assert(reader && reader->cursor+0 < reader->count);
  return reader->data[reader->cursor++];
}

int16_t mice_reader_read_short(Mice_Reader *reader) {
  assert(reader && reader->cursor+1 < reader->count);
  int16_t a  = (int16_t)reader->data[reader->cursor++] << 8;
  return  a |  (int16_t)reader->data[reader->cursor++] << 0;
}

int32_t mice_reader_read_int(Mice_Reader *reader) {
  assert(reader && reader->cursor+3 < reader->count);
  int32_t a  = (int32_t)reader->data[reader->cursor++] << 25;
          a |= (int32_t)reader->data[reader->cursor++] << 16;
          a |= (int32_t)reader->data[reader->cursor++] <<  8;
  return  a |  (int32_t)reader->data[reader->cursor++] <<  0;
}

int64_t mice_reader_read_long(Mice_Reader *reader) {
  assert(reader && reader->cursor+7 < reader->count);
  int64_t a  = (int64_t)reader->data[reader->cursor++] << 56;
          a |= (int64_t)reader->data[reader->cursor++] << 48;
          a |= (int64_t)reader->data[reader->cursor++] << 40;
          a |= (int64_t)reader->data[reader->cursor++] << 32;
          a |= (int64_t)reader->data[reader->cursor++] << 24;
          a |= (int64_t)reader->data[reader->cursor++] << 16;
          a |= (int64_t)reader->data[reader->cursor++] <<  8;
  return  a |  (int64_t)reader->data[reader->cursor++] <<  0;
}

uint8_t mice_reader_read_ubyte(Mice_Reader *reader) {
  assert(reader && reader->cursor+0 < reader->count);
  return reader->data[reader->cursor++];
}

uint16_t mice_reader_read_ushort(Mice_Reader *reader) {
  assert(reader && reader->cursor+1 < reader->count);
  uint16_t a  = (uint16_t)reader->data[reader->cursor++] << 8;
  return   a |  (uint16_t)reader->data[reader->cursor++] << 0;
}

uint32_t mice_reader_read_uint(Mice_Reader *reader) {
  assert(reader && reader->cursor+3 < reader->count);
  uint32_t a  = (uint32_t)reader->data[reader->cursor++] << 25;
           a |= (uint32_t)reader->data[reader->cursor++] << 16;
           a |= (uint32_t)reader->data[reader->cursor++] <<  8;
  return   a |  (uint32_t)reader->data[reader->cursor++] <<  0;
}

uint64_t mice_reader_read_ulong(Mice_Reader *reader) {
  assert(reader && reader->cursor+7 < reader->count);
  uint64_t a  = (uint64_t)reader->data[reader->cursor++] << 56;
           a |= (uint64_t)reader->data[reader->cursor++] << 48;
           a |= (uint64_t)reader->data[reader->cursor++] << 40;
           a |= (uint64_t)reader->data[reader->cursor++] << 32;
           a |= (uint64_t)reader->data[reader->cursor++] << 24;
           a |= (uint64_t)reader->data[reader->cursor++] << 16;
           a |= (uint64_t)reader->data[reader->cursor++] <<  8;
  return   a |  (uint64_t)reader->data[reader->cursor++] <<  0;
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