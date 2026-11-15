/* { dg-options "-ffreestanding" } */
#include <stdio.h>

static int fill(int count) {
  int values[count];
  int total = 0;
  for (int i = 0; i < count; i++) {
    values[i] = i;
    total += values[i];
  }
  return total;
}

int main(void) {
  printf("%d\n", fill(5));
  return 0;
}
