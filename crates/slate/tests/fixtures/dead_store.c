#include <stdio.h>

static int noisy(int x) {
  printf("noisy %d\n", x);
  return x;
}

int main(void) {
  int dead = 1 + 2;
  int kept = noisy(5);
  noisy(9);
  printf("%d\n", kept);
  return 0;
}
