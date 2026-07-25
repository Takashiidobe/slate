#include <stdio.h>

int main(void) {
  unsigned int u = 0x12345678u;
  unsigned int z = 0u;
  int s = -12345;
  unsigned int sh = 5u;

  unsigned int rev = __builtin_bitreverse32(u);
  unsigned int swapped = __builtin_bswap32(u);
  int leading = __builtin_clz(u);
  int trailing = __builtin_ctz(u);
  int first_set = __builtin_ffs((int)u);
  int zero_first = __builtin_ffs((int)z);
  int ones = __builtin_popcount(u);
  int odd = __builtin_parity(u);
  int redundant_sign = __builtin_clrsb(s);
  unsigned int left = __builtin_rotateleft32(u, sh);
  unsigned int right = __builtin_rotateright32(u, sh);

  printf("%u %u %d %d %d %d %d %d %d %u %u\n", rev, swapped, leading, trailing,
         first_set, zero_first, ones, odd, redundant_sign, left, right);
  return 0;
}
