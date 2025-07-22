#include "mice/data/varint.h"

#define _MICE_VARINT_CONTINUATION_BIT (1 << 7)

const char *mice_get_varint(size_t length, const char source[length], Mice_Varint *result) {
  if (!length) return NULL;
  *result = 0;

  Mice_Varint shift = 0;
  size_t i = 0;
  for (; i < length; ++i) {
    *result += (source[i] & ~_MICE_VARINT_CONTINUATION_BIT) << shift;
    if (!(source[i] & _MICE_VARINT_CONTINUATION_BIT)) break;
  }

  if (i >= length) return NULL;
  return &source[i+1];
}

const char *mice_get_varlong(size_t length, const char source[length], Mice_Varlong *result) {
  if (!length) return NULL;
  *result = 0;

  Mice_Varlong shift = 0;
  size_t i = 0;
  for (; i < length; ++i) {
    *result += (source[i] & ~_MICE_VARINT_CONTINUATION_BIT) << shift;
    if (!(source[i] & _MICE_VARINT_CONTINUATION_BIT)) break;
  }

  if (i >= length) return NULL;
  return &source[i+1];
}

bool mice_varint_is_complete(size_t length, const char source[length]) {
  if (!length) return false;
  size_t i = 0;
  while (i < length && source[i] & _MICE_VARINT_CONTINUATION_BIT) ++i;
  return !(source[i] & _MICE_VARINT_CONTINUATION_BIT);
}

bool mice_varlong_is_complete(size_t length, const char source[length]) {
  if (!length) return false;
  size_t i = 0;
  while (i < length && source[i] & _MICE_VARINT_CONTINUATION_BIT) ++i;
  return !(source[i] & _MICE_VARINT_CONTINUATION_BIT);
}

Mice_Varint mice_reader_read_varint(Mice_Reader *reader) {
  Mice_Varint varint = 0, shift = 0;

  char byte = 0;
  do {
    byte = mice_reader_read_byte(reader);
    varint |= (byte & (~_MICE_VARINT_CONTINUATION_BIT & 0xFF)) << shift;
    shift += 7;
  } while (byte & _MICE_VARINT_CONTINUATION_BIT);

  return varint;
}

Mice_Varlong mice_reader_read_varlong(Mice_Reader *reader) {
  Mice_Varlong varlong = 0, shift = 0;

  char byte = 0;
  do {
    byte = mice_reader_read_byte(reader);
    varlong += (byte & ~_MICE_VARINT_CONTINUATION_BIT) << shift;
    shift += 7;
  } while (byte & _MICE_VARINT_CONTINUATION_BIT);

  return varlong;
}

void mice_writer_write_varint(Mice_Writer *writer, Mice_Varint a) {
  do {
    char byte = (a & ~_MICE_VARINT_CONTINUATION_BIT);
    byte |= ((a >>= 7) ? _MICE_VARINT_CONTINUATION_BIT : 0);
    mice_writer_write_byte(writer, byte);
  } while (a);
}

void mice_writer_write_varlong(Mice_Writer *writer, Mice_Varlong a) {
  do {
    char byte = (a & ~_MICE_VARINT_CONTINUATION_BIT);
    byte |= ((a >>= 7) ? _MICE_VARINT_CONTINUATION_BIT : 0);
    mice_writer_write_byte(writer, byte);
  } while (a);
}