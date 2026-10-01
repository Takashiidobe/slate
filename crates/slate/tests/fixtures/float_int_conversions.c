#include <limits.h>
#include <stdio.h>

static double half(void) { return 0.5; }

int main(void) {
  double values[] = {3.9, -3.9, 0.999, -0.999, 255.5, 2147483647.0, -2147483648.0};
  for (int i = 0; i < 7; i++)
    printf("%d ", (int)values[i]);
  printf("\n");

  printf("%u %u\n", (unsigned)3.9, (unsigned)4294967295.0);
  printf("%u %d\n", (unsigned)(unsigned char)200.7, (int)(signed char)-100.5);
  printf("%d %d\n", (short)-32768.9, (unsigned short)65535.9);
  printf("%lld %lld\n", (long long)1e18, (long long)-9.2e18);
  printf("%llu\n", (unsigned long long)1.8e19);

  printf("%.1f %.1f\n", (double)LLONG_MAX, (double)ULLONG_MAX);
  printf("%.1f %.1f\n", (float)INT_MAX, (float)16777217);
  printf("%.1f %.1f\n", (double)4294967295u, (double)INT_MIN);
  unsigned long long big = 0xfffffffffffff801ULL;
  printf("%.1f %.1f\n", (double)big, (float)big);
  signed char sc = -7;
  unsigned char uc = 250;
  printf("%g %g\n", (double)sc, (float)uc);

  double precise = 0.1;
  float narrowed = (float)precise;
  double widened = narrowed;
  printf("%.17g %.17g\n", (double)narrowed, widened);
  printf("%.9g\n", (float)1e-50);
  printf("%g\n", (double)(float)1e39);

  _Bool from_half = half();
  _Bool from_tiny = 1e-300;
  _Bool from_zero = -0.0;
  _Bool from_nan  = 0.0 / (half() - 0.5);
  printf("%d %d %d %d\n", from_half, from_tiny, from_zero, from_nan);

  int n   = 10;
  double r = n / 4;
  float s  = n / 4.0f;
  long l   = 3.99f;
  printf("%g %g %ld\n", r, s, l);
  n = 2.75;
  n += 1.5;
  printf("%d\n", n);
  unsigned u = 7;
  u *= 0.5;
  printf("%u\n", u);
  return 0;
}
