#include "mice/hash/SHA256.h"

#include <assert.h>
#include <string.h>

static uint32_t _right_rotate(uint32_t v, uint32_t n);
static void _mice_sha256_transform(Mice_Sha256 *ctx);

void mice_sha256_init(Mice_Sha256 *ctx) {
  assert(ctx);
  memset(ctx, 0, sizeof(*ctx));

  ctx->state[0] = 0x6A09E667;
  ctx->state[1] = 0xBB67AE85;
  ctx->state[2] = 0x3C6EF372;
  ctx->state[3] = 0xA54FF53A;
  ctx->state[4] = 0x510E527F;
  ctx->state[5] = 0x9B05688C;
  ctx->state[6] = 0x1F83D9AB;
  ctx->state[7] = 0x5BE0CD19;
}

void mice_sha256_update(Mice_Sha256 *ctx, size_t length, const uint8_t data[length]) {
  const uint8_t *ptr = data;
  while (length) {
    size_t a = sizeof(ctx->buffer)-ctx->bufferLength;
    size_t n = length;
    if (n > a) n = a;

    memcpy(ctx->buffer+ctx->bufferLength, ptr, n);
    ctx->bufferLength += n;
    ptr += n;
    length -= n;
    ctx->bitCount += n*8;

    if (ctx->bufferLength == sizeof(ctx->buffer)) {
      _mice_sha256_transform(ctx);
      ctx->bufferLength = 0;
    }
  }
}

void mice_sha256_finish(Mice_Sha256 *ctx, Mice_Sha256_Digest digest) {
  uint64_t bitCount = ctx->bitCount;

  mice_sha256_update(ctx, 1, (uint8_t[1]){ 0x80 });

  size_t paddingLength = (ctx->bufferLength < 56) ? (56 - ctx->bufferLength) : (64 + 56 - ctx->bufferLength);
  uint8_t padding[paddingLength];
  memset(padding, 0, paddingLength);
  mice_sha256_update(ctx, paddingLength, padding);

  uint8_t length[8] = {
    (bitCount>>56)&0xFF,
    (bitCount>>48)&0xFF,
    (bitCount>>40)&0xFF,
    (bitCount>>32)&0xFF,
    (bitCount>>24)&0xFF,
    (bitCount>>16)&0xFF,
    (bitCount>> 8)&0xFF,
    (bitCount>> 0)&0xFF
  };

  mice_sha256_update(ctx, 8, length);

  for (size_t i = 0; i < 8; ++i) {
    digest[4*i+0] = (ctx->state[i] >> 24) & 0xFF;
    digest[4*i+1] = (ctx->state[i] >> 16) & 0xFF;
    digest[4*i+2] = (ctx->state[i] >>  8) & 0xFF;
    digest[4*i+3] = (ctx->state[i] >>  0) & 0xFF;
  }
}

static uint32_t _right_rotate(uint32_t v, uint32_t n) {
  const uint32_t mask = 31;
  n &= mask;
  return (v >> n) | (v << ((32 - n) & mask));
}

static void _mice_sha256_transform(Mice_Sha256 *ctx) {
  uint32_t w[64] = {0};

  for (size_t i = 0; i < 16; i++) {
    w[i] = (ctx->buffer[4*i+0] << 24)|
           (ctx->buffer[4*i+1] << 16)|
           (ctx->buffer[4*i+2] <<  8)|
           (ctx->buffer[4*i+3] <<  0);
  }

  for (size_t i = 16; i < 64; ++i) {
    uint32_t s0 = _right_rotate(w[i-15],  7) ^ _right_rotate(w[i-15], 18) ^ (w[i-15] >>  3);
    uint32_t s1 = _right_rotate(w[i- 2], 17) ^ _right_rotate(w[i- 2], 19) ^ (w[i- 2] >> 10);
    w[i] = w[i-16] + s0 + w[i-7] + s1;
  }

  static uint32_t k[64] = {
    0x428A2F98, 0x71374491, 0xB5C0FBCF, 0xE9B5DBA5, 0x3956C25B, 0x59F111F1, 0x923F82A4, 0xAB1C5ED5,
    0xD807AA98, 0x12835B01, 0x243185BE, 0x550C7DC3, 0x72BE5D74, 0x80DEB1FE, 0x9BDC06A7, 0xC19BF174,
    0xE49B69C1, 0xEFBE4786, 0x0FC19DC6, 0x240CA1CC, 0x2DE92C6F, 0x4A7484AA, 0x5CB0A9DC, 0x76F988DA,
    0x983E5152, 0xA831C66D, 0xB00327C8, 0xBF597FC7, 0xC6E00BF3, 0xD5A79147, 0x06CA6351, 0x14292967,
    0x27B70A85, 0x2E1B2138, 0x4D2C6DFC, 0x53380D13, 0x650A7354, 0x766A0ABB, 0x81C2C92E, 0x92722C85,
    0xA2BFE8A1, 0xA81A664B, 0xC24B8B70, 0xC76C51A3, 0xD192E819, 0xD6990624, 0xF40E3585, 0x106AA070,
    0x19A4C116, 0x1E376C08, 0x2748774C, 0x34B0BCB5, 0x391C0CB3, 0x4ED8AA4A, 0x5B9CCA4F, 0x682E6FF3,
    0x748F82EE, 0x78A5636F, 0x84C87814, 0x8CC70208, 0x90BEFFFA, 0xA4506CEB, 0xBEF9A3F7, 0xC67178F2
  };

  uint32_t a = ctx->state[0],
           b = ctx->state[1],
           c = ctx->state[2],
           d = ctx->state[3],
           e = ctx->state[4],
           f = ctx->state[5],
           g = ctx->state[6],
           h = ctx->state[7];

  for (size_t i = 0; i < 64; ++i) {
    uint32_t ch = (e & f) ^ ((~e) & g);
    uint32_t ma = (a & b) ^ (a & c) ^ (b & c);
    uint32_t e0 = (_right_rotate(a, 2) ^ _right_rotate(a, 13) ^ _right_rotate(a, 22));
    uint32_t e1 = (_right_rotate(e, 6) ^ _right_rotate(e, 11) ^ _right_rotate(e, 25));
    uint32_t t1 = h + e1 + ch + k[i] + w[i];
    uint32_t t2 = e0 + ma;

    h = g;
    g = f;
    f = e;
    e = d + t1;
    d = c;
    c = b;
    b = a;
    a = t1 + t2;
  }

  ctx->state[0] += a;
  ctx->state[1] += b;
  ctx->state[2] += c;
  ctx->state[3] += d;
  ctx->state[4] += e;
  ctx->state[5] += f;
  ctx->state[6] += g;
  ctx->state[7] += h;
}