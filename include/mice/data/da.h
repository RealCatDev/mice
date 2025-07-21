#ifndef   _MICE_DATA_DA_H_
#define   _MICE_DATA_DA_H_

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#ifndef MICE_DA_DEFAULT_SIZE
#define MICE_DA_DEFAULT_SIZE (1<<4)
#endif

#define MICE_DA_APPEND(da, item)                                                 \
  do {                                                                           \
    if ((da)->count >= (da)->capacity) {                                         \
      if (!(da)->capacity) (da)->capacity = MICE_DA_DEFAULT_SIZE;                \
      while ((da)->count >= (da)->capacity) (da)->capacity <<= 1;                \
      (da)->items = realloc((da)->items, sizeof(*(da)->items) * (da)->capacity); \
    }                                                                            \
    assert((da)->items);                                                         \
    (da)->items[(da)->count++] = item;                                           \
  } while (0)

#define MICE_DA_APPEND_MANY(da, data, length)                                    \
  do {                                                                           \
    if ((da)->count+(length) > (da)->capacity) {                                 \
      if (!(da)->capacity) (da)->capacity = MICE_DA_DEFAULT_SIZE;                \
      while ((da)->count+(length) > (da)->capacity) (da)->capacity <<= 1;        \
      (da)->items = realloc((da)->items, sizeof(*(da)->items) * (da)->capacity); \
    }                                                                            \
    assert((da)->items);                                                         \
    memcpy(&(da)->items[(da)->count], (data), sizeof(*(da)->items) * (length));  \
    (da)->count += (length);                                                     \
  } while (0)

#endif // _MICE_DATA_DA_H_