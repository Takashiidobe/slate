// { dg-options "/Zc:preprocessor" }
#include <intrin.h>

#pragma intrinsic(__shiftleft128, __shiftright128)

static unsigned __int64 shift_left(unsigned __int64 low, unsigned char count) {
  return __shiftleft128(low, 0, count);
}

static unsigned __int64 shift_right(unsigned __int64 high, unsigned char count) {
  return __shiftright128(0, high, count);
}

int main(void) {
  unsigned __int64 high_bit = 0x8000000000000000ULL;
  return shift_left(1, 64) != 0 || shift_right(1, 64) != 0 ||
         shift_left(high_bit, 65) != 1 || shift_right(1, 65) != high_bit;
}
