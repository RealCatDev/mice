#ifndef   _MICE_DATA_POSITION_H_
#define   _MICE_DATA_POSITION_H_

#include <stdint.h>

typedef uint64_t Mice_Position;

#define MICE_POSITION(x, y, z) \
  (((Mice_Position)x & 0x3FFFFFF) << 38) | \
  (((Mice_Position)y & 0xFFF    ) <<  0) | \
  (((Mice_Position)z & 0x3FFFFFF) << 12)

#define MICE_POSITION_X(pos) (((int64_t)pos <<  0) >> 38)
#define MICE_POSITION_Y(pos) (((int64_t)pos << 52) >> 52)
#define MICE_POSITION_Z(pos) (((int64_t)pos << 26) >> 38)

#endif // _MICE_DATA_POSITION_H_