#ifndef   _MICE_IO_STRING_BUILDER_H_
#define   _MICE_IO_STRING_BUILDER_H_

#include "mice/data/da.h"

#include <stddef.h>

typedef struct Mice_String_Builder {
  char *items;
  size_t capacity;
  size_t count;
} Mice_String_Builder;

#endif // _MICE_IO_STRING_BUILDER_H_