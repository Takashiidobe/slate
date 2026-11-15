/* { dg-options "-ffreestanding" } */
#include <stdio.h>

static unsigned _BitInt(524288) wide;

int main(void) {
  wide = 3;
  wide = wide * wide;
  printf("%d\n", (int)wide);
  return 0;
}
