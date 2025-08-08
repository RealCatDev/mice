#include "mice/data/position.h"

Mice_Position mice_position_encode(int32_t x, int16_t y, int32_t z) {
  return (((uint64_t)x & 0x3FFFFFF) << 38) | (((uint64_t)y & 0xFFF)) | (((uint64_t)z & 0x3FFFFFF) << 12);
}

void mice_position_decode(Mice_Position pos, int32_t *x, int16_t *y, int32_t *z) {
  if (x) *x = ((int64_t)pos <<  0) >> 38;
  if (y) *y = ((int64_t)pos << 52) >> 52;
  if (z) *z = ((int64_t)pos << 26) >> 38;
}