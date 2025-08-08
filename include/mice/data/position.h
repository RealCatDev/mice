#ifndef   _MICE_DATA_POSITION_H_
#define   _MICE_DATA_POSITION_H_

#include <stdint.h>

typedef uint64_t Mice_Position;

Mice_Position mice_position_encode(int32_t x, int16_t y, int32_t z);
void mice_position_decode(Mice_Position pos, int32_t *x, int16_t *y, int32_t *z);

#endif // _MICE_DATA_POSITION_H_