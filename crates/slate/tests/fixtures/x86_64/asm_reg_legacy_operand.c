#include <stdio.h>

static unsigned legacy_add(unsigned a, unsigned b) {
  __asm__("addl %1, %0" : "+R"(a) : "R"(b));
  return a;
}

static unsigned char legacy_low_byte(unsigned x) {
  unsigned char r;
  __asm__("movb %b1, %0" : "=R"(r) : "R"(x) : "eax", "ecx");
  return r;
}

int main(void) {
  printf("%u %u\n", legacy_add(40, 2), legacy_low_byte(0x1234));
  return 0;
}
