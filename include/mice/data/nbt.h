#ifndef   _MICE_DATA_NBT_H_
#define   _MICE_DATA_NBT_H_

#include "mice/data/da.h"

#include "mice/io/reader.h"
#include "mice/io/writer.h"

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct Mice_Nbt_Tag Mice_Nbt_Tag;
typedef enum Mice_Nbt_Tag_Type {
  MICE_NBT_TAG_TYPE_BYTE = 1,
  MICE_NBT_TAG_TYPE_SHORT,
  MICE_NBT_TAG_TYPE_INT,
  MICE_NBT_TAG_TYPE_LONG,
  MICE_NBT_TAG_TYPE_FLOAT,
  MICE_NBT_TAG_TYPE_DOUBLE,
  MICE_NBT_TAG_TYPE_BYTE_ARRAY,
  MICE_NBT_TAG_TYPE_STRING,
  MICE_NBT_TAG_TYPE_LIST,
  MICE_NBT_TAG_TYPE_COMPOUND,
  MICE_NBT_TAG_TYPE_INT_ARRAY,
  MICE_NBT_TAG_TYPE_LONG_ARRAY,

  COUNT_MICE_NBT_TAG_TYPES
} Mice_Nbt_Tag_Type;

typedef        int8_t                  Mice_Nbt_Tag_Byte;
typedef        int16_t                 Mice_Nbt_Tag_Short;
typedef        int32_t                 Mice_Nbt_Tag_Int;
typedef        int64_t                 Mice_Nbt_Tag_Long;
typedef        float                   Mice_Nbt_Tag_Float;
typedef        double                  Mice_Nbt_Tag_Double;
typedef struct Mice_Nbt_Tag_Byte_Array Mice_Nbt_Tag_Byte_Array;
typedef struct Mice_Nbt_Tag_String     Mice_Nbt_Tag_String;
typedef struct Mice_Nbt_Tag_List       Mice_Nbt_Tag_List;
typedef struct Mice_Nbt_Tag_Compound   Mice_Nbt_Tag_Compound;
typedef struct Mice_Nbt_Tag_Int_Array  Mice_Nbt_Tag_Int_Array;
typedef struct Mice_Nbt_Tag_Long_Array Mice_Nbt_Tag_Long_Array;

struct Mice_Nbt_Tag_Byte_Array {
  int32_t size;
  Mice_Nbt_Tag_Byte *data;
};

struct Mice_Nbt_Tag_String {
  uint16_t length;
  char *data;
};

Mice_Nbt_Tag_String mice_reader_read_nbt_tag_string(Mice_Reader *reader);
void mice_writer_write_nbt_tag_string(Mice_Writer *writer, Mice_Nbt_Tag_String string);

struct Mice_Nbt_Tag_List {
  Mice_Nbt_Tag_Type type;
  int32_t size;
  Mice_Nbt_Tag *data;
};

struct Mice_Nbt_Tag_Compound {
  Mice_Nbt_Tag *items;
  size_t capacity;
  size_t count;
};

struct Mice_Nbt_Tag_Int_Array {
  int32_t size;
  Mice_Nbt_Tag_Int *data;
};

struct Mice_Nbt_Tag_Long_Array {
  int32_t size;
  Mice_Nbt_Tag_Long *data;
};



struct Mice_Nbt_Tag {
  Mice_Nbt_Tag_Type id;
  Mice_Nbt_Tag_String name;
  union {
    Mice_Nbt_Tag_Byte       Byte;
    Mice_Nbt_Tag_Short      Short;
    Mice_Nbt_Tag_Int        Int;
    Mice_Nbt_Tag_Long       Long;
    Mice_Nbt_Tag_Float      Float;
    Mice_Nbt_Tag_Double     Double;
    Mice_Nbt_Tag_Byte_Array byteArray;
    Mice_Nbt_Tag_String     string;
    Mice_Nbt_Tag_List       list;
    Mice_Nbt_Tag_Compound   compound;
    Mice_Nbt_Tag_Int_Array  intArray;
    Mice_Nbt_Tag_Long_Array longArray;
  } payload;
};

Mice_Nbt_Tag mice_reader_read_nbt_tag(Mice_Reader *reader, Mice_Nbt_Tag_Type id);
void mice_writer_write_nbt_tag(Mice_Writer *writer, Mice_Nbt_Tag tag, Mice_Nbt_Tag_Type id);

typedef struct Mice_Nbt_Tag_Compound Mice_Nbt;

Mice_Nbt mice_reader_read_nbt(Mice_Reader *reader);
void mice_writer_write_nbt(Mice_Writer *writer, Mice_Nbt nbt);

#endif // _MICE_DATA_NBT_H_