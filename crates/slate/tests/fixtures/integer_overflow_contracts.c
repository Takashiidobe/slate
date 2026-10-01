#include <limits.h>
#include <stdint.h>
#include <stdio.h>

static unsigned scale(unsigned value, unsigned factor) { return value * factor; }

static uint64_t mix(uint64_t state) {
  state ^= state >> 33;
  state *= 0xff51afd7ed558ccdULL;
  state ^= state >> 33;
  state *= 0xc4ceb9fe1a85ec53ULL;
  return state ^ (state >> 33);
}

int main(void) {
  unsigned top = UINT_MAX;
  unsigned zero = 0;
  unsigned long long big = ULLONG_MAX;
  top += 5;
  zero -= 3;
  big *= 3;
  unsigned counter = UINT_MAX;
  counter++;
  unsigned negated = -top;
  printf("%u %u %llu %u %u\n", top, zero, big, counter, negated);
  printf("%u %llu\n", scale(0x80000001u, 6u), (unsigned long long)mix(42));

  int narrow = 0;
  long wide = 0;
  unsigned unsigned_out = 0;
  long long ll_out = 0;
  int a = __builtin_add_overflow(INT_MAX, 1, &wide);
  int b = __builtin_add_overflow(INT_MAX, 1, &narrow);
  int c = __builtin_sub_overflow(0u, 1u, &narrow);
  int d = __builtin_sub_overflow(-1, 0, &unsigned_out);
  int e = __builtin_mul_overflow(UINT_MAX, UINT_MAX, &ll_out);
  int f = __builtin_mul_overflow(LLONG_MIN, -1LL, &ll_out);
  int g = __builtin_mul_overflow(-7, 6, &narrow);
  printf("%d %ld %d %d %d %u\n", a, wide, b, c, d, unsigned_out);
  printf("%d %d %lld %d %d\n", e, f, ll_out, g, narrow);
  if (__builtin_add_overflow(ULLONG_MAX, 2ULL, &unsigned_out)) {
    printf("wrapped %u\n", unsigned_out);
  }
  return (int)(top + zero) & 0x3f;
}
