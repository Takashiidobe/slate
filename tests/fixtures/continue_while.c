#include <stdio.h>

static int sum_skipping_threes(int n) {
  int total = 0;
  int i = 0;
  while (i < n) {
    i++;
    if (i % 3 == 0) {
      continue;
    }
    total += i;
  }
  return total;
}

int main(void) {
  printf("%d\n", sum_skipping_threes(10));
  printf("%d\n", sum_skipping_threes(2));
  return 0;
}
