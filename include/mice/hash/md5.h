#ifndef   _MICE_HASH_MD5_H_
#define   _MICE_HASH_MD5_H_

#include <stddef.h>
#include <stdint.h>

typedef struct Mice_Md5_State {
  uint32_t a, b, c, d;
} Mice_Md5_State;

Mice_Md5_State mice_md5_hash(size_t inputLength, const char input[inputLength]);

#endif // _MICE_HASH_MD5_H_