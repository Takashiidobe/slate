#include <stdio.h>

struct __attribute__((packed)) MixedBits {
  unsigned char tag;
  unsigned int low : 3;
  signed int delta : 6;
  unsigned int : 0;
  unsigned long long wide : 35;
  unsigned int tail : 17;
};

int main(void) {
  struct MixedBits bits = {0};
  bits.tag = 0xa5;
  bits.low = 7;
  bits.delta = -17;
  bits.wide = 0x712345678ULL;
  bits.tail = 0x1abcd;
  printf("%u %u %d %llu %u %zu\n", bits.tag, bits.low, bits.delta,
         bits.wide, bits.tail, sizeof(bits));
  return 0;
}
