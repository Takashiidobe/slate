#include <stdio.h>

static int add(int a, int unused) {
  return a + 1;
}

static int get_used(int a, int b) {
  return a + b;
}

int main(void) {
  printf("%d %d\n", add(5, 10), get_used(1, 2));
  return 0;
}
