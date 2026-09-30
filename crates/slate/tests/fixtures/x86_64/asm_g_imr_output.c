#include <stdio.h>

static int store_via_g(void) {
  int x;
  __asm__("movl $42, %0" : "=g"(x));
  return x;
}

static int store_via_imr(void) {
  int x;
  __asm__("movl $7, %0" : "=imr"(x));
  return x;
}

static int add_tied_g(int y) {
  int x = 1;
  __asm__("addl %1, %0" : "+g"(x) : "r"(y));
  return x;
}

int main(void) {
  printf("%d %d %d\n", store_via_g(), store_via_imr(), add_tied_g(4));
  return 0;
}
