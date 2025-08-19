#ifndef   _MICE_DATA_UUID_H_
#define   _MICE_DATA_UUID_H_

#include "mice/data/hashMap.h"

#include "mice/io/reader.h"
#include "mice/io/writer.h"

#include <stdint.h>
#include <stdbool.h>

typedef struct Mice_Uuid {
  uint8_t bytes[16];
} Mice_Uuid;

Mice_Uuid mice_reader_read_uuid(Mice_Reader *reader);
void mice_writer_write_uuid(Mice_Writer *writer, Mice_Uuid uuid);

Mice_Uuid mice_uuid_from_string(const char string[36]);
void mice_uuid_to_string(Mice_Uuid uuid, char string[36]);

Mice_Uuid mice_uuid_offline(size_t usernameLength, const char username[usernameLength]);

Mice_Hash mice_uuid_hash(Mice_Uuid uuid);
bool mice_uuid_equals(Mice_Uuid a, Mice_Uuid b);

#endif // _MICE_DATA_UUID_H_