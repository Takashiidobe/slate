#include <stdio.h>

static int global_value = 42;

static int read_via_address(void) {
  int result;
  __asm__("movl %a1, %0" : "=r"(result) : "p"(&global_value));
  return result;
}

int main(void) {
  printf("%d\n", read_via_address());
  return 0;
}


