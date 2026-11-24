// { dg-options "/Zc:preprocessor" }
#include <intrin.h>
#include <stdlib.h>

static int check8(unsigned char value, unsigned char count) {
  return _rotl8(value, count) != 3 || _rotr8(value, count) != 0xc0;
}

static int check16(unsigned short value, unsigned char count) {
  return _rotl16(value, count) != 3 || _rotr16(value, count) != 0xc000;
}

static int check32(unsigned value, int count) {
  return _rotl(value, count) != 3 || _rotr(value, count) != 0xc0000000U;
}

static int check64(unsigned __int64 value, int count) {
  return _rotl64(value, count) != 3 || _rotr64(value, count) != 0xc000000000000000ULL;
}

int main(void) {
  return check8(0x81, 9) + 2 * check16(0x8001, 17) +
         4 * check32(0x80000001U, 33) + 8 * check64(0x8000000000000001ULL, 65);
}
