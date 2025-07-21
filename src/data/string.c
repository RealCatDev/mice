// TODO: Acknowledge UTF-8

#include "mice/data/string.h"

#include "mice/data/varint.h"

#include <stdlib.h>
#include <assert.h>

Mice_String mice_reader_read_string(Mice_Reader *reader) {
  Mice_Varint length = mice_reader_read_varint(reader);
  char *data = malloc(length);
  assert(data);

  for (Mice_Varint i = 0; i < length; ++i) {
    char c = mice_reader_read_byte(reader);
    assert(!(c & ~(1 << 7)));
    data[i] = c;
  }

  return (Mice_String){ .length = length, .data = data };
}

void mice_writer_write_string(Mice_Writer *writer, Mice_String a) {
  mice_writer_write_varint(writer, a.length);
  mice_writer_write_bytes(writer, a.data, a.length);
}