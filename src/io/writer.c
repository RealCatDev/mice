#include "mice/io/writer.h"

#define MICE_DA_DEFAULT_SIZE (1<<5)
#include "mice/data/da.h"

#include <assert.h>

void mice_writer_write_byte(Mice_Writer *writer, int8_t a) {
  assert(writer);
  MICE_DA_APPEND(writer, a);
}

void mice_writer_write_short(Mice_Writer *writer, int16_t a) {
  assert(writer);
  MICE_DA_APPEND(writer, (a >> 8) & 0xFF);
  MICE_DA_APPEND(writer, (a >> 0) & 0xFF);
}

void mice_writer_write_int(Mice_Writer *writer, int32_t a) {
  assert(writer);
  MICE_DA_APPEND(writer, (a >> 24) & 0xFF);
  MICE_DA_APPEND(writer, (a >> 16) & 0xFF);
  MICE_DA_APPEND(writer, (a >>  8) & 0xFF);
  MICE_DA_APPEND(writer, (a >>  0) & 0xFF);
}

void mice_writer_write_long(Mice_Writer *writer, int64_t a) {
  assert(writer);
  MICE_DA_APPEND(writer, (a >> 56) & 0xFF);
  MICE_DA_APPEND(writer, (a >> 48) & 0xFF);
  MICE_DA_APPEND(writer, (a >> 40) & 0xFF);
  MICE_DA_APPEND(writer, (a >> 32) & 0xFF);
  MICE_DA_APPEND(writer, (a >> 24) & 0xFF);
  MICE_DA_APPEND(writer, (a >> 16) & 0xFF);
  MICE_DA_APPEND(writer, (a >>  8) & 0xFF);
  MICE_DA_APPEND(writer, (a >>  0) & 0xFF);
}

void mice_writer_write_ubyte(Mice_Writer *writer, uint8_t a) {
  assert(writer);
  MICE_DA_APPEND(writer, a);
}

void mice_writer_write_ushort(Mice_Writer *writer, uint16_t a) {
  assert(writer);
  MICE_DA_APPEND(writer, (a >> 8) & 0xFF);
  MICE_DA_APPEND(writer, (a >> 0) & 0xFF);
}

void mice_writer_write_uint(Mice_Writer *writer, uint32_t a) {
  assert(writer);
  MICE_DA_APPEND(writer, (a >> 24) & 0xFF);
  MICE_DA_APPEND(writer, (a >> 16) & 0xFF);
  MICE_DA_APPEND(writer, (a >>  8) & 0xFF);
  MICE_DA_APPEND(writer, (a >>  0) & 0xFF);
}

void mice_writer_write_ulong(Mice_Writer *writer, uint64_t a) {
  assert(writer);
  MICE_DA_APPEND(writer, (a >> 56) & 0xFF);
  MICE_DA_APPEND(writer, (a >> 48) & 0xFF);
  MICE_DA_APPEND(writer, (a >> 40) & 0xFF);
  MICE_DA_APPEND(writer, (a >> 32) & 0xFF);
  MICE_DA_APPEND(writer, (a >> 24) & 0xFF);
  MICE_DA_APPEND(writer, (a >> 16) & 0xFF);
  MICE_DA_APPEND(writer, (a >>  8) & 0xFF);
  MICE_DA_APPEND(writer, (a >>  0) & 0xFF);
}

void mice_writer_write_float(Mice_Writer *writer, float a) {
  mice_writer_write_int(writer, *(int32_t*)&a);
}

void mice_writer_write_double(Mice_Writer *writer, double a) {
  mice_writer_write_long(writer, *(int64_t*)&a);
}

void mice_writer_write_bytes(Mice_Writer *writer, const char *data, size_t length) {
  assert(writer);
  MICE_DA_APPEND_MANY(writer, data, length);
}