#include <stdio.h>

static int inc_wide_via_Q(int x) {
  __asm__("incl %0" : "+Q"(x));
  return x;
}

static unsigned char inc_byte_via_q(unsigned char x) {
  __asm__("incb %0" : "+q"(x));
  return x;
}

int main(void) {
  printf("%d %d\n", inc_wide_via_Q(41), inc_byte_via_q(9));
  return 0;
}


