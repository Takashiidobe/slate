#include <stdio.h>

static unsigned add_via_rbx(unsigned a, unsigned b) {
  __asm__("addl %%ebx, %0" : "+r"(a) : "b"(b));
  return a;
}

static unsigned double_in_rbx(unsigned x) {
  __asm__("addl %0, %0" : "+b"(x));
  return x;
}

static long scaled_sum(long a, long b) {
  register long r10 __asm__("r10") = b;
  register long r8 __asm__("r8") = a;
  __asm__("leaq (%1,%2,4), %0\n\taddb $1, %b0" : "=r"(r8) : "r"(r8), "r"(r10));
  return r8;
}

static unsigned short low_word(unsigned x) {
  register unsigned short w __asm__("si");
  __asm__("movw %w1, %0" : "=r"(w) : "r"(x));
  return w;
}

int main(void) {
  printf("%u %u %ld %u\n", add_via_rbx(40, 2), double_in_rbx(21),
         scaled_sum(3, 10), low_word(0x12345678u));
  return 0;
}
