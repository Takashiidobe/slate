#include <stdio.h>
#include <stdbool.h>

static _Bool to_bool(int x) { return x; }
static _Bool to_bool_u8(unsigned char x) { return x; }
static _Bool to_bool_ll(long long x) { return (_Bool)x; }
static int from_bool(_Bool b) { return b + 1; }
static signed char trunc_sc(int x) { return (signed char)x; }
static unsigned char trunc_uc(int x) { return x; }
static short trunc_s(long long x) { return (short)x; }
static unsigned widen_u(signed char c) { return c; }
static long long widen_ll(unsigned short s) { return s; }
static unsigned long long widen_ull(int x) { return x; }
static int reinterp(unsigned x) { return (int)x; }
static unsigned reinterp_u(int x) { return x; }
static int promote(unsigned char a, unsigned char b) { return a - b; }
static int promote_s(short a, short b) { return a * b; }

int main(void) {
  printf("%d %d %d\n", to_bool(256), to_bool(0), to_bool(-7));
  printf("%d %d\n", to_bool_u8(0), to_bool_u8(200));
  printf("%d %d\n", to_bool_ll(1LL << 40), to_bool_ll(0));
  _Bool b = 42;
  printf("%d %d\n", b, from_bool(b));
  b = to_bool(0);
  int bi = b;
  printf("%d\n", bi);
  printf("%d %d %d\n", trunc_sc(200), trunc_uc(-1), trunc_s(0x12345678LL));
  printf("%u %lld %llu\n", widen_u(-3), widen_ll(65535), widen_ull(-1));
  printf("%d %u\n", reinterp(0xFFFFFFFFu), reinterp_u(-2));
  printf("%d %d\n", promote(1, 2), promote_s(300, 300));
  unsigned char uc = 255;
  uc = uc + 1;
  printf("%d\n", uc);
  _Bool c = !b;
  printf("%d %d\n", c, !bi);
  return to_bool(3) + to_bool(0);
}
