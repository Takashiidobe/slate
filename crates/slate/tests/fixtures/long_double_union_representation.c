#include <stdint.h>
#include <stdio.h>

static int convert(long double value) {
  union {
    long double f80;
    uint64_t u64[2];
  } bits;
  bits.u64[0] = 0;
  bits.u64[1] = 0;
  bits.f80 = value;
  return (int)bits.f80;
}

int main(void) { printf("%d\n", convert(6.75L)); }
