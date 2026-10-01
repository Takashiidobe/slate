#include <stdio.h>

static int negative          = -1;
static long long wide_neg    = -9000000000LL;
static unsigned wrapped      = -1u;
static int mixed             = (3 + 4) * -2 - 10 / 3 % 2;
static int bits              = (0xf0 | 0x0f) & ~0x3 ^ 1 << 4;
static unsigned shifted      = 0x80000000u >> 31;
static double scaled         = -2.5 * 4.0 + 1.0;
static float halved          = -(1.0f / 2.0f);
static char sign             = -'a';
static int table[3]          = {-1, -2 * 3, 7 - 10};
static struct { int lo, hi; } range = {-5, -(-5)};
int exported                 = -42;

int main(void) {
  printf("%d %lld %u %d %d %u\n", negative, wide_neg, wrapped, mixed, bits, shifted);
  printf("%g %g %d\n", scaled, halved, sign);
  printf("%d %d %d %d %d %d\n", table[0], table[1], table[2], range.lo, range.hi, exported);
  negative--;
  printf("%d\n", negative);
  return 0;
}
