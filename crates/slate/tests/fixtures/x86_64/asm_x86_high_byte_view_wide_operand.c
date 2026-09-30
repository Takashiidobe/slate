#include <stdio.h>

static int inc_high_byte_of_wide_operand(int x) {
  __asm__("incb %h0" : "+Q"(x));
  return x;
}

int main(void) {
  printf("%d\n", inc_high_byte_of_wide_operand(0x1234));
  return 0;
}
