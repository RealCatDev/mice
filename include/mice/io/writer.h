#ifndef   _MICE_IO_WRITER_H_
#define   _MICE_IO_WRITER_H_

#include <stddef.h>
#include <stdint.h>

typedef struct Mice_Writer {
  char *items;
  size_t capacity;
  size_t count;
} Mice_Writer;

void mice_writer_write_byte(Mice_Writer *writer, int8_t a);
void mice_writer_write_short(Mice_Writer *writer, int16_t a);
void mice_writer_write_int(Mice_Writer *writer, int32_t a);
void mice_writer_write_long(Mice_Writer *writer, int64_t a);

void mice_writer_write_ubyte(Mice_Writer *writer, uint8_t a);
void mice_writer_write_ushort(Mice_Writer *writer, uint16_t a);
void mice_writer_write_uint(Mice_Writer *writer, uint32_t a);
void mice_writer_write_ulong(Mice_Writer *writer, uint64_t a);

void mice_writer_write_float(Mice_Writer *writer, float a);
void mice_writer_write_double(Mice_Writer *writer, double a);

void mice_writer_write_bytes(Mice_Writer *writer, const char *data, size_t length);

#endif // _MICE_IO_WRITER_H_