/* { dg-options "-ffreestanding" } */
#include <stdio.h>

static int scratch(int count) {
  int *values = __builtin_alloca(count * sizeof *values);
  int total = 0;
  for (int i = 0; i < count; i++) {
    values[i] = i;
    total += values[i];
  }
  return total;
}

int main(void) {
  printf("%d\n", scratch(5));
  return 0;
}
