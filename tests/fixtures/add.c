#include <stdio.h>

static int add(int a, int b) {
  int c = a + b;
  return c;
}

int main(void) {
  printf("%d\n", add(2, 3));
  printf("%d\n", add(-10, 4));
  printf("%d\n", add(0, 0));
  return 0;
}
