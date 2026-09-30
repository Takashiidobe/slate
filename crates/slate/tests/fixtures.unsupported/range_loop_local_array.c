#include <stdio.h>

static int sum_fixed(void) {
  int a[5]  = {1, 2, 3, 4, 5};
  int total = 0;
  for (int i = 0; i < 5; i++) {
    total += a[i];
  }
  return total;
}

int main(void) {
  printf("%d\n", sum_fixed());
  return 0;
}
