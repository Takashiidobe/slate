#include <stdio.h>

static int add_via_i(int value) {
  __asm__("add %0, %0, %1" : "+r"(value) : "I"(1));
  return value;
}

static int add_via_m(int value) {
  __asm__("add %0, %0, %1" : "+r"(value) : "M"(32));
  return value;
}

int main(void) {
  printf("%d %d\n", add_via_i(40), add_via_m(8));
  return 0;
}
