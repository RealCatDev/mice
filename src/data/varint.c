#include "mice/data/varint.h"

#define _MICE_VARINT_CONTINUATION_BIT (1 << 7)

Mice_Varint mice_reader_read_varint(Mice_Reader *reader) {
  Mice_Varint varint = 0, shift = 0;

  char byte = 0;
  do {
    byte = mice_reader_read_byte(reader);
    varint += (byte & ~_MICE_VARINT_CONTINUATION_BIT) << shift;
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
  while (a) {
    char byte = (a & ~_MICE_VARINT_CONTINUATION_BIT);
    byte |= ((a >>= 7) ? _MICE_VARINT_CONTINUATION_BIT : 0);
    mice_writer_write_byte(writer, byte);
  }
}

void mice_writer_write_varlong(Mice_Writer *writer, Mice_Varlong a) {
  while (a) {
    char byte = (a & ~_MICE_VARINT_CONTINUATION_BIT);
    byte |= ((a >>= 7) ? _MICE_VARINT_CONTINUATION_BIT : 0);
    mice_writer_write_byte(writer, byte);
  }
}