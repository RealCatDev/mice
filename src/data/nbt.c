#include "mice/data/nbt.h"

#include <stdlib.h>

Mice_Nbt_Tag_String mice_reader_read_nbt_tag_string(Mice_Reader *reader) {
  Mice_Nbt_Tag_String string = {0};
  string.length = mice_reader_read_ushort(reader);
  string.data = malloc(string.length);
  assert(string.data);
  mice_reader_read_bytes(reader, string.length, string.data);

  return string;
}

void mice_writer_write_nbt_tag_string(Mice_Writer *writer, Mice_Nbt_Tag_String string) {
  mice_writer_write_ushort(writer, string.length);
  mice_writer_write_bytes(writer, string.data, string.length);
}

Mice_Nbt_Tag mice_reader_read_nbt_tag(Mice_Reader *reader, Mice_Nbt_Tag_Type id) {
  Mice_Nbt_Tag tag = {0};
  if (id == 0) {
    if ((tag.id = mice_reader_read_byte(reader))) {
      tag.name = mice_reader_read_nbt_tag_string(reader);
    }
  } else tag.id = id;

  if (!tag.id) return tag;
  switch (tag.id) {
  case MICE_NBT_TAG_TYPE_BYTE: {
    tag.payload.Byte = mice_reader_read_byte(reader);
  } break;
  case MICE_NBT_TAG_TYPE_SHORT: {
    tag.payload.Short = mice_reader_read_short(reader);
  } break;
  case MICE_NBT_TAG_TYPE_INT: {
    tag.payload.Int = mice_reader_read_int(reader);
  } break;
  case MICE_NBT_TAG_TYPE_LONG: {
    tag.payload.Long = mice_reader_read_long(reader);
  } break;
  case MICE_NBT_TAG_TYPE_FLOAT: {
    tag.payload.Float = mice_reader_read_float(reader);
  } break;
  case MICE_NBT_TAG_TYPE_DOUBLE: {
    tag.payload.Double = mice_reader_read_double(reader);
  } break;
  case MICE_NBT_TAG_TYPE_BYTE_ARRAY: {
    tag.payload.byteArray.size = mice_reader_read_int(reader);
    tag.payload.byteArray.data = malloc(tag.payload.byteArray.size);
    assert(tag.payload.byteArray.data);

    mice_reader_read_bytes(reader, tag.payload.byteArray.size, (char*)tag.payload.byteArray.data);
  } break;
  case MICE_NBT_TAG_TYPE_STRING: {
    tag.payload.string = mice_reader_read_nbt_tag_string(reader);
  } break;
  case MICE_NBT_TAG_TYPE_LIST: {
    tag.payload.list.type = mice_reader_read_byte(reader);
    tag.payload.list.size = mice_reader_read_int(reader);
    tag.payload.list.data = malloc(sizeof(*tag.payload.list.data) * tag.payload.list.size);
    assert(tag.payload.list.data);

    for (int32_t i = 0; i < tag.payload.list.size; ++i) tag.payload.list.data[i] = mice_reader_read_nbt_tag(reader, tag.payload.list.type);
  } break;
  case MICE_NBT_TAG_TYPE_COMPOUND: {
    Mice_Nbt_Tag tag = {0};
    while ((tag = mice_reader_read_nbt_tag(reader, 0)).id) {
      MICE_DA_APPEND(&tag.payload.compound, tag);
    }
  } break;
  case MICE_NBT_TAG_TYPE_INT_ARRAY: {
    tag.payload.intArray.size = mice_reader_read_int(reader);
    tag.payload.intArray.data = malloc(sizeof(*tag.payload.intArray.data) * tag.payload.intArray.size);
    assert(tag.payload.intArray.data);

    for (int32_t i = 0; i < tag.payload.intArray.size; ++i) tag.payload.intArray.data[i] = mice_reader_read_int(reader);
  } break;
  case MICE_NBT_TAG_TYPE_LONG_ARRAY: {
    tag.payload.longArray.size = mice_reader_read_long(reader);
    tag.payload.longArray.data = malloc(sizeof(*tag.payload.longArray.data) * tag.payload.longArray.size);
    assert(tag.payload.longArray.data);

    for (int32_t i = 0; i < tag.payload.longArray.size; ++i) tag.payload.longArray.data[i] = mice_reader_read_long(reader);
  } break;
  default: assert(0);
  }

  return tag;
}

void mice_writer_write_nbt_tag(Mice_Writer *writer, Mice_Nbt_Tag tag, Mice_Nbt_Tag_Type id) {
  if (!id) {
    mice_writer_write_byte(writer, tag.id);
    mice_writer_write_nbt_tag_string(writer, tag.name);
  } else tag.id = id;

  switch (tag.id) {
  case MICE_NBT_TAG_TYPE_BYTE: {
    mice_writer_write_byte(writer, tag.payload.Byte);
  } break;
  case MICE_NBT_TAG_TYPE_SHORT: {
    mice_writer_write_short(writer, tag.payload.Short);
  } break;
  case MICE_NBT_TAG_TYPE_INT: {
    mice_writer_write_int(writer, tag.payload.Int);
  } break;
  case MICE_NBT_TAG_TYPE_LONG: {
    mice_writer_write_long(writer, tag.payload.Long);
  } break;
  case MICE_NBT_TAG_TYPE_FLOAT: {
    mice_writer_write_float(writer, tag.payload.Float);
  } break;
  case MICE_NBT_TAG_TYPE_DOUBLE: {
    mice_writer_write_double(writer, tag.payload.Double);
  } break;
  case MICE_NBT_TAG_TYPE_BYTE_ARRAY: {
    mice_writer_write_int(writer, tag.payload.byteArray.size);
    mice_writer_write_bytes(writer, (char*)tag.payload.byteArray.data, tag.payload.byteArray.size);
  } break;
  case MICE_NBT_TAG_TYPE_STRING: {
    mice_writer_write_nbt_tag_string(writer, tag.payload.string);
  } break;
  case MICE_NBT_TAG_TYPE_LIST: {
    mice_writer_write_byte(writer, tag.payload.list.type);
    mice_writer_write_int(writer, tag.payload.list.size);
    for (int32_t i = 0; i < tag.payload.list.size; ++i) {
      mice_writer_write_nbt_tag(writer, tag.payload.list.data[i], tag.payload.list.type);
    }
  } break;
  case MICE_NBT_TAG_TYPE_COMPOUND: {
    for (size_t i = 0; i < tag.payload.compound.count; ++i) {
      mice_writer_write_nbt_tag(writer, tag.payload.compound.items[i], 0);
    }
    mice_writer_write_byte(writer, 0);
  } break;
  case MICE_NBT_TAG_TYPE_INT_ARRAY: {
    mice_writer_write_int(writer, tag.payload.intArray.size);
    for (int32_t i = 0; i < tag.payload.intArray.size; ++i) {
      mice_writer_write_int(writer, tag.payload.intArray.data[i]);
    }
  } break;
  case MICE_NBT_TAG_TYPE_LONG_ARRAY: {
    mice_writer_write_long(writer, tag.payload.longArray.size);
    for (int32_t i = 0; i < tag.payload.longArray.size; ++i) {
      mice_writer_write_long(writer, tag.payload.longArray.data[i]);
    }
  } break;
  default: assert(0);
  }
}

Mice_Nbt mice_reader_read_nbt(Mice_Reader *reader) {
  Mice_Nbt nbt = {0};

  Mice_Nbt_Tag tag = {0};
  while ((tag = mice_reader_read_nbt_tag(reader, 0)).id) {
    MICE_DA_APPEND(&nbt, tag);
  }
  return nbt;
}

void mice_writer_write_nbt(Mice_Writer *writer, Mice_Nbt nbt) {
  for (size_t i = 0; i < nbt.count; ++i) {
    mice_writer_write_nbt_tag(writer, nbt.items[i], 0);
  }
  mice_writer_write_byte(writer, 0);
}