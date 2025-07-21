#ifndef   _MICE_DATA_STRING_H_
#define   _MICE_DATA_STRING_H_

#include "mice/data/varint.h"

#include "mice/io/reader.h"
#include "mice/io/writer.h"

typedef struct Mice_String {
  Mice_Varint length;
  char *data;
} Mice_String;

Mice_String mice_reader_read_string(Mice_Reader *reader);

void mice_writer_write_string(Mice_Writer *writer, Mice_String a);

#endif // _MICE_DATA_STRING_H_