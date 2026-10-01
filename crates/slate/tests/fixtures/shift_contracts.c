#include <stdio.h>

static long long shl_mixed(long long value, int amount) { return value << amount; }
static int shl_by_wide(int value, unsigned long long amount) { return value << amount; }
static int sar(int value, unsigned char amount) { return value >> amount; }
static unsigned shr(unsigned value, long amount) { return value >> amount; }

int main(void) {
  printf("%lld\n", shl_mixed(1, 40));
  printf("%d\n", shl_by_wide(3, 4));
  printf("%d %d %d\n", sar(-1, 31), sar(-7, 1), sar(-256, 4));
  printf("%u %u\n", shr(0xffffffffu, 31), shr(0x80000000u, 0));

  unsigned long long top = 1ULL << 63;
  long long neg          = -5;
  printf("%llu %lld %lld\n", top, neg >> 1, (-1LL) >> 63);

  unsigned char uc = 0x80;
  signed char   sc = -128;
  printf("%d %d %d\n", uc << 1, sc >> 3, uc >> 7);

  short s = 0x1234;
  s <<= 4;
  signed char c = -64;
  c >>= 2;
  unsigned char b = 0xff;
  b <<= 4;
  printf("%d %d %u\n", s, c, (unsigned)b);

  unsigned long long amount = 5;
  int x                     = 1;
  x <<= amount;
  long wide = 1;
  wide <<= (char)33;
  printf("%d %ld\n", x, wide);

  int shifts[] = {0, 1, 15, 30};
  for (int i = 0; i < 4; i++)
    printf("%d:%u ", 1 << shifts[i], 0xdeadbeefu >> shifts[i]);
  printf("\n");
  return 0;
}
