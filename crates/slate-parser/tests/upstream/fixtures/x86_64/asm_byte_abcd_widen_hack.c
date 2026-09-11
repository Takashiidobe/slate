#include <stdio.h>

static unsigned char inc_byte_tied_via_Q(unsigned char x) {
  __asm__("incb %b0" : "+Q"(x));
  return x;
}

static unsigned char add_byte_input_via_Q(unsigned char x, unsigned char y) {
  __asm__("addb %b1, %b0" : "+Q"(x) : "Q"(y));
  return x;
}

int main(void) {
  printf("%d %d\n", inc_byte_tied_via_Q(0xFF), add_byte_input_via_Q(5, 9));
  return 0;
}


