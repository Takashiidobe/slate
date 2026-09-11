#include <stdio.h>

static int read_via_o(int x) {
  int result;
  __asm__("movl %1, %0" : "=r"(result) : "o"(x));
  return result;
}

static int incr_via_V(int x) {
  __asm__("incl %0" : "+V"(x));
  return x;
}

static int store_via_o(void) {
  int x;
  __asm__("movl $42, %0" : "=o"(x));
  return x;
}

int main(void) {
  printf("%d %d %d\n", read_via_o(7), incr_via_V(9), store_via_o());
  return 0;
}


