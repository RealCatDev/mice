#ifndef   _MICE_HASH_SHA256_H_
#define   _MICE_HASH_SHA256_H_

#include <stdint.h>

typedef struct Mice_Sha256 {
  uint32_t state[8];
  uint64_t bitCount;
  size_t bufferLength;
  uint8_t buffer[64];
} Mice_Sha256;

typedef uint8_t Mice_Sha256_Digest[32];

void mice_sha256_init(Mice_Sha256 *ctx);
void mice_sha256_update(Mice_Sha256 *ctx, size_t length, const uint8_t data[length]);
void mice_sha256_finish(Mice_Sha256 *ctx, Mice_Sha256_Digest digest);

#endif // _MICE_HASH_SHA256_H_