#ifndef   _MICE_IO_READER_H_
#define   _MICE_IO_READER_H_

#include <stddef.h>
#include <stdint.h>

typedef struct Mice_Reader {
  const char *data;
  size_t cursor;
  size_t count;
} Mice_Reader;

int8_t mice_reader_read_byte(Mice_Reader *reader);
int16_t mice_reader_read_short(Mice_Reader *reader);
int32_t mice_reader_read_int(Mice_Reader *reader);
int64_t mice_reader_read_long(Mice_Reader *reader);

uint8_t mice_reader_read_ubyte(Mice_Reader *reader);
uint16_t mice_reader_read_ushort(Mice_Reader *reader);
uint32_t mice_reader_read_uint(Mice_Reader *reader);
uint64_t mice_reader_read_ulong(Mice_Reader *reader);

float mice_reader_read_float(Mice_Reader *reader);
double mice_reader_read_double(Mice_Reader *reader);

void mice_reader_read_bytes(Mice_Reader *reader, size_t count, char bytes[count]);

#endif // _MICE_IO_READER_H_