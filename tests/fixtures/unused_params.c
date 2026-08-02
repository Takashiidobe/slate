#include <stdio.h>

static int add(int a, int unused) {
  return a + 1;
}

static int get_used(int a, int b) {
  return a + b;
}

static int remove_two(int a, int unused_a, int unused_b) {
  return a + 2;
}

static int address_taken(int a, int unused) {
  return a + 4;
}

int main(void) {
  int (*indirect)(int, int) = address_taken;
  printf("%d %d %d %d\n", add(5, 10), get_used(1, 2), remove_two(3, 4, 5), indirect(8, 9));
  return 0;
}
