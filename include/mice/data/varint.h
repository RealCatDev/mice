#ifndef   _MICE_DATA_VARINT_H_
#define   _MICE_DATA_VARINT_H_

#include "mice/io/reader.h"
#include "mice/io/writer.h"

#include <stdint.h>

typedef int32_t Mice_Varint;
typedef int64_t Mice_Varlong;

Mice_Varint mice_reader_read_varint(Mice_Reader *reader);
Mice_Varlong mice_reader_read_varlong(Mice_Reader *reader);

void mice_writer_write_varint(Mice_Writer *writer, Mice_Varint a);
void mice_writer_write_varlong(Mice_Writer *writer, Mice_Varlong a);

#endif // _MICE_DATA_VARINT_H_