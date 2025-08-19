#ifndef   _MICE_DATA_HASH_MAP_H_
#define   _MICE_DATA_HASH_MAP_H_

#include "mice/data/da.h"
#include "mice/data/sparseSet.h"

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

typedef uint32_t Mice_Hash;

#ifndef MICE_HASH_MAP_INITIAL_CAP
#define MICE_HASH_MAP_INITIAL_CAP 16
#endif

#ifndef MICE_HASH_MAP_LOAD_FACTOR
#define MICE_HASH_MAP_LOAD_FACTOR 0.75
#endif

#define MICE_HASH_MAP(name, K, V) \
  struct name {                   \
    struct { /* bucket */         \
      struct { /* entry */        \
        K key;                    \
        V value;                  \
        Mice_Hash hash;           \
      } *items;                   \
      size_t capacity;            \
      size_t count;               \
    } *buckets;                   \
    size_t capacity;              \
    size_t size;                  \
    Mice_Hash (*hash)(K);         \
    bool (*equals)(K, K);         \
  }

#define MICE_HASH_MAP_COUNT(hm) ((hm)->count)

#define _MICE_HASH_MAP_RESIZE(hm)                                                   \
  do {                                                                              \
    size_t newCapacity = (hm)->capacity << 1;                                       \
    if (!newCapacity) newCapacity = MICE_HASH_MAP_INITIAL_CAP;                      \
    typeof((hm)->buckets) newBuckets = calloc(newCapacity, sizeof(*(hm)->buckets)); \
    assert(newBuckets);                                                             \
    for (size_t b = 0; b < (hm)->capacity; ++b) {                                   \
      typeof(*(hm)->buckets) bucket = (hm)->buckets[b];                             \
      for (size_t e = 0; e < bucket.count; ++e) {                                   \
        typeof(*bucket.items) entry = bucket.items[e];                              \
        typeof((hm)->buckets) newBucket = &newBuckets[entry.hash % newCapacity];    \
        MICE_DA_APPEND(newBucket, entry);                                           \
      }                                                                             \
    }                                                                               \
    free((hm)->buckets);                                                            \
    (hm)->buckets = newBuckets;                                                     \
    (hm)->capacity = newCapacity;                                                   \
  } while (0)

#define MICE_HASH_MAP_PUT(hm, k, v)                                       \
  do {                                                                    \
    if (!(hm)->capacity) {                                                \
      _MICE_HASH_MAP_RESIZE(hm);                                          \
    } else {                                                              \
      float sizeRatio = (hm)->size / (float)(hm)->capacity;               \
      if (sizeRatio >= MICE_HASH_MAP_LOAD_FACTOR) {                       \
        _MICE_HASH_MAP_RESIZE(hm);                                        \
      }                                                                   \
    }                                                                     \
    typeof((hm)->buckets->items->key) key = k;                            \
    Mice_Hash hash = (hm)->hash(key);                                     \
    typeof((hm)->buckets) bucket = &(hm)->buckets[hash % (hm)->capacity]; \
    bool done = false;                                                    \
    for (size_t e = 0; e < bucket->count; ++e) {                          \
      typeof(bucket->items) entry = &bucket->items[e];                    \
      if ((hm)->equals(entry->key, key)) {                                \
        done = true;                                                      \
        break;                                                            \
      }                                                                   \
    }                                                                     \
    if (done) break;                                                      \
    MICE_DA_APPEND(bucket, ((typeof(*bucket->items)){                     \
      .key = key,                                                         \
      .value = v,                                                         \
      .hash = hash                                                        \
    }));                                                                  \
  } while (0)

#define MICE_HASH_MAP_GET(hm, k, v)                                       \
  do {                                                                    \
    typeof((hm)->buckets->items->key) key = k;                            \
    Mice_Hash hash = (hm)->hash(key);                                     \
    typeof((hm)->buckets) bucket = &(hm)->buckets[hash % (hm)->capacity]; \
    bool found = false;                                                   \
    for (size_t e = 0; e < bucket->count; ++e) {                          \
      typeof(bucket->items) entry = &bucket->items[e];                    \
      if ((hm)->equals(entry->key, key)) {                                \
        found = true;                                                     \
        *v = entry->value;                                                \
        break;                                                            \
      }                                                                   \
    }                                                                     \
    assert(found);                                                        \
  } while (0)

#define MICE_HASH_MAP_REMOVE(hm, k)                                       \
  do {                                                                    \
    typeof((hm)->buckets->items->key) key = k;                            \
    Mice_Hash hash = (hm)->hash(key);                                     \
    typeof((hm)->buckets) bucket = &(hm)->buckets[hash % (hm)->capacity]; \
    bool found = false;                                                   \
    for (size_t e = 0; e < bucket->count; ++e) {                          \
      if ((hm)->equals(bucket->items[e].key, key)) {                      \
        MICE_DA_FAST_REMOVE(bucket, e);                                   \
        found = true;                                                     \
        break;                                                            \
      }                                                                   \
    }                                                                     \
    assert(found);                                                        \
  } while (0)

#define MICE_HASH_MAP_FOREACH(hm, it)                               \
  for (size_t b = 0; b < (hm)->capacity; ++b)                       \
    for (typeof((hm)->buckets->items) it = (hm)->buckets[b].items;  \
         it != (hm)->buckets[b].items+(hm)->buckets[b].count; ++it)

#define MICE_HASH_MAP_FREE(hm)                    \
  do {                                            \
    for (size_t b = 0; b < (hm)->capacity; ++b) { \
      MICE_DA_FREE(&(hm)->buckets[b]);            \
    }                                             \
    free((hm)->buckets);                          \
  } while (0)
         
#endif // _MICE_DATA_HASH_MAP_H_