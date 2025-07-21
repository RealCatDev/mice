// Translated from: https://en.wikipedia.org/wiki/MD5#Pseudocode

#include "mice/hash/md5.h"

#include "mice/data/da.h"

static uint32_t sK[64] = {
  0xD76AA478, 0xE8C7B756, 0x242070DB, 0xC1BDCEEE,
  0xF57C0FAF, 0x4787C62A, 0xA8304613, 0xFD469501,
  0x698098D8, 0x8B44F7AF, 0xFFFF5BB1, 0x895CD7BE,
  0x6B901122, 0xFD987193, 0xA679438E, 0x49B40821,
  0xF61E2562, 0xC040B340, 0x265E5A51, 0xE9B6C7AA,
  0xD62F105D, 0x02441453, 0xD8A1E681, 0xE7D3FBC8,
  0x21E1CDE6, 0xC33707D6, 0xF4D50D87, 0x455A14ED,
  0xA9E3E905, 0xFCEFA3F8, 0x676F02D9, 0x8D2A4C8A,
  0xFFFA3942, 0x8771F681, 0x6D9D6122, 0xFDE5380C,
  0xA4BEEA44, 0x4BDECFA9, 0xF6BB4B60, 0xBEBFBC70,
  0x289B7EC6, 0xEAA127FA, 0xD4EF3085, 0x04881D05,
  0xD9D4D039, 0xE6DB99E5, 0x1FA27CF8, 0xC4AC5665,
  0xF4292244, 0x432AFF97, 0xAB9423A7, 0xFC93A039,
  0x655B59C3, 0x8F0CCC92, 0xFFEFF47D, 0x85845DD1,
  0x6FA87E4F, 0xFE2CE6E0, 0xA3014314, 0x4E0811A1,
  0xF7537E82, 0xBD3AF235, 0x2AD7D2BB, 0xEB86D391
};

static uint32_t sShift[64] = {
  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,
  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,
  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,
  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,
};

typedef uint32_t Mice_Md5_Chunk[16];

static uint32_t leftrotate(uint32_t x, uint32_t c);

Mice_Md5_State mice_md5_hash(size_t inputLength, const char input[inputLength]) {
  Mice_Md5_State state = {
    .a = 0x67452301,
    .b = 0xEFCDAB89,
    .c = 0x98BADCFE,
    .d = 0x10325476
  };

  struct {
    uint8_t *items;
    size_t capacity;
    size_t count;
  } message = {0};
  if (inputLength) MICE_DA_APPEND_MANY(&message, input, inputLength);
  MICE_DA_APPEND(&message, 0x80);

  while ((message.count%64) != 56) MICE_DA_APPEND(&message, 0);

  uint64_t bitLength = (uint64_t)inputLength * 8;
  for (uint32_t i = 0; i < 8; ++i) {
    MICE_DA_APPEND(&message, (bitLength >> (i*8)) & 0xFF);
  }

  for (size_t chunkOffset = 0; chunkOffset < message.count; chunkOffset += 64) {
    Mice_Md5_Chunk chunk = {0};
    for (uint32_t i = 0; i < 16; ++i) {
      chunk[i] = ((uint8_t)message.items[chunkOffset+i*4+0] <<  0)|
                 ((uint8_t)message.items[chunkOffset+i*4+1] <<  8)|
                 ((uint8_t)message.items[chunkOffset+i*4+2] << 16)|
                 ((uint8_t)message.items[chunkOffset+i*4+3] << 24);
    }

    uint32_t a = state.a,
             b = state.b,
             c = state.c,
             d = state.d;

    for (uint32_t i = 0; i < 64; ++i) {
      uint32_t F, g;
      if (i < 16) {
        F = (b & c) | (~b & d);
        g = i;
      } else if (i < 32) {
        F = (b & d) | (~d & c);
        g = (5*i+1) % 16;
      } else if (i < 48) {
        F = b ^ c ^ d;
        g = (3*i+5) % 16;
      } else if (i < 64) {
        F = c ^ (b | ~d);
        g = (7*i) % 16;
      }

      F += a + sK[i] + chunk[g];
      F  = leftrotate(F, sShift[i]);
      F += b;

      a = d;
      d = c;
      c = b;
      b = F;
    }

    state.a += a;
    state.b += b;
    state.c += c;
    state.d += d;
  }

  return state;
}

static uint32_t leftrotate(uint32_t x, uint32_t c) {
  return (x << c) | ((uint32_t)x >> (32 - c));
}