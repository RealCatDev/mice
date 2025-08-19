#include "mice/data/hashMap.h"

Mice_Hash mice_u64_hash(uint64_t v) {
  return (Mice_Hash)(v ^ (v >> 32));
}

bool mice_u64_equals(uint64_t a, uint64_t b) {
  return a == b;
}