#include <stdio.h>

static int load_via_uv(const int *address) {
  int value;
  __asm__("ldr %0, %1" : "=r"(value) : "Uv"(*address));
  return value;
}

int main(void) {
  int value = 42;
  printf("%d\n", load_via_uv(&value));
  return 0;
}


