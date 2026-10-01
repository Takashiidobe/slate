#include <limits.h>
#include <stdint.h>
#include <stdio.h>

static int quotient(int a, int b) { return a / b; }
static int remainder_of(int a, int b) { return a % b; }

int main(void) {
  int pairs[][2] = {{7, 2}, {-7, 2}, {7, -2}, {-7, -2}, {0, 5}, {INT_MIN, 2}, {INT_MAX, -1}};
  for (unsigned i = 0; i < sizeof pairs / sizeof pairs[0]; i++) {
    int a = pairs[i][0];
    int b = pairs[i][1];
    printf("%d %d %d\n", quotient(a, b), remainder_of(a, b), quotient(a, b) * b + remainder_of(a, b) == a);
  }
  unsigned mixed = -7 / 2u;
  unsigned mixed_rem = -7 % 3u;
  long long big = LLONG_MIN / 3;
  long long big_rem = LLONG_MIN % 7;
  uint64_t ubig = UINT64_MAX / 10;
  uint64_t ubig_rem = UINT64_MAX % 10;
  printf("%u %u %lld %lld %llu %llu\n", mixed, mixed_rem, big, big_rem,
         (unsigned long long)ubig, (unsigned long long)ubig_rem);

  signed char sc = -100;
  unsigned char uc = 250;
  short sh = -30000;
  unsigned short us = 65535;
  sc /= 3;
  uc %= 7;
  sh /= -7;
  us /= 2;
  int8_t narrow = -128;
  narrow /= -1;
  printf("%d %d %d %d %d\n", sc, uc, sh, us, narrow);

  unsigned accumulator = 1000000007u;
  int steps = 0;
  while (accumulator > 1) {
    accumulator = accumulator % 2 ? accumulator / 3 : accumulator / 2;
    steps++;
  }
  printf("%d %d\n", steps, (int)(-17 % 5 + 17 % -5));
  return quotient(-45, 4) & 0xff;
}
