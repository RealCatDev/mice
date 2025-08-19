#ifndef   _MICE_DATA_SPARSE_SET_H_
#define   _MICE_DATA_SPARSE_SET_H_

#include "mice/data/da.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MICE_SPARSE_SET(name, sparseType, pageSize) \
  struct name {                                     \
    size_t size;                                    \
    struct {                                        \
      sparseType **items;                           \
      size_t capacity;                              \
      size_t count;                                 \
      char size[pageSize];                          \
    } sparse;                                       \
    void *dense;                                    \
    sparseType *denseToSparse;                      \
    size_t capacity;                                \
    size_t count;                                   \
  }

#define MICE_SPARSE_SET_SIZE(set) ((set)->count)

#define _MICE_SPARSE_SET_PAGE_SIZE(set) sizeof((set)->sparse.size)

#define _MICE_SPARSE_SET_NEW_PAGE(set) \
  memset(malloc(sizeof(**(set)->sparse.items) * sizeof((set)->sparse.size)) \
        , 0xFF, sizeof(**(set)->sparse.items) * sizeof((set)->sparse.size))

#define _MICE_SPARSE_SET_GET_DENSE_ITEM(set, idx) \
  (((char*)(set)->dense)+(idx*(set)->size))

#define _MICE_SPARSE_SET_SET_DENSE_INDEX(set, id, idx)                             \
  do {                                                                             \
    size_t page = id/_MICE_SPARSE_SET_PAGE_SIZE(set);                              \
    if (page >= (set)->sparse.count) {                                             \
      size_t oldCount = (set)->sparse.count;                                       \
      MICE_DA_RESIZE(&(set)->sparse, page+1);                                      \
      memset(&(set)->sparse.items[oldCount], 0x00,                                 \
             sizeof(*(set)->sparse.items) * ((set)->sparse.count-oldCount));       \
    }                                                                              \
    if (!(set)->sparse.items[page]) {                                              \
      (set)->sparse.items[page] = _MICE_SPARSE_SET_NEW_PAGE(set);                  \
    }                                                                              \
    (set)->sparse.items[page][id%_MICE_SPARSE_SET_PAGE_SIZE(set)] = idx;           \
  } while (0)

#define _MICE_SPARSE_SET_GET_DENSE_INDEX(set, sparseType, id) \
  (((id)/_MICE_SPARSE_SET_PAGE_SIZE(set) >= (set)->sparse.count) \
      ? ((sparseType)-1) \
      : ((set)->sparse.items[(id)/_MICE_SPARSE_SET_PAGE_SIZE(set)][(id)%_MICE_SPARSE_SET_PAGE_SIZE(set)]))

#define MICE_SPARSE_SET_PUT(set, sparseType, id, v)                         \
  do {                                                                      \
    void *value = &v;                                                       \
    sparseType idx = _MICE_SPARSE_SET_GET_DENSE_INDEX(set, sparseType, id); \
    if (idx != (sparseType)-1) {                                            \
      memcpy(_MICE_SPARSE_SET_GET_DENSE_ITEM(set, (set)->count),            \
             value, (set)->size);                                           \
      (set)->denseToSparse[idx] = id;                                       \
      break;                                                                \
    }                                                                       \
    _MICE_SPARSE_SET_SET_DENSE_INDEX(set, id, (set)->count);                \
    if ((set)->count >= (set)->capacity) {                                  \
      if (!(set)->capacity) (set)->capacity = 8;                            \
      while ((set)->count >= (set)->capacity) (set)->capacity <<= 1;        \
      (set)->dense = realloc((set)->dense, (set)->size * (set)->capacity);  \
      (set)->denseToSparse = realloc((set)->denseToSparse,                  \
        sizeof(*(set)->denseToSparse) * (set)->capacity);                   \
    }                                                                       \
    assert((set)->dense);                                                   \
    assert((set)->denseToSparse);                                           \
    memcpy(_MICE_SPARSE_SET_GET_DENSE_ITEM(set, (set)->count),              \
           value, (set)->size);                                             \
    (set)->denseToSparse[(set)->count] = id;                                \
    ++(set)->count;                                                         \
  } while (0)

#define MICE_SPARSE_SET_REMOVE(set, sparseType, id)                         \
  do {                                                                      \
    sparseType idx = _MICE_SPARSE_SET_GET_DENSE_INDEX(set, sparseType, id); \
    assert(idx != ((sparseType)-1));                                        \
    _MICE_SPARSE_SET_SET_DENSE_INDEX(set,                                   \
      (set)->denseToSparse[(set)->count-1], idx);                           \
    _MICE_SPARSE_SET_SET_DENSE_INDEX(set, id, (sparseType)-1);              \
    --(set)->count;                                                         \
    memcpy(_MICE_SPARSE_SET_GET_DENSE_ITEM(set, idx),                       \
           _MICE_SPARSE_SET_GET_DENSE_ITEM(set, (set)->count),              \
           (set)->size);                                                    \
    (set)->denseToSparse[idx] = (set)->denseToSparse[(set)->count];         \
  } while (0)

#define MICE_SPARSE_SET_GET(set, type, sparseType, id) \
  (*(type*)_MICE_SPARSE_SET_GET_DENSE_ITEM(set, \
    (assert(_MICE_SPARSE_SET_GET_DENSE_INDEX(set, sparseType, id) != ((sparseType)-1)), \
    _MICE_SPARSE_SET_GET_DENSE_INDEX(set, sparseType, id))))

#define MICE_SPARSE_SET_FREE(set, sparseType) \
  do {                                        \
    MICE_DA_FOREACH(&(set)->sparse, it) {     \
      free(*it);                              \
    }                                         \
    free((set)->sparse.items);                \
    free((set)->dense);                       \
    free((set)->denseToSparse);               \
  } while (0)

#define MICE_SPARSE_SET_FOREACH(set, type, it) \
  for (type *it = (type*)(set)->dense; it != (type*)_MICE_SPARSE_SET_GET_DENSE_ITEM(set, (set)->count); ++it)

#define MICE_SPARSE_SET_FOREACH_ID(set, sparseType, it) \
  for (sparseType *it = (set)->denseToSparse; it != &(set)->denseToSparse[(set)->count]; ++it)

// #define MICE_SPARSE_SET_ID(set, )

#endif // _MICE_DATA_SPARSE_SET_H_