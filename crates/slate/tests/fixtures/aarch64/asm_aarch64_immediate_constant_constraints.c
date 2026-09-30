#include <stdio.h>

static int add_via_i(int value) {
  __asm__("add %w0, %w0, %1" : "+r"(value) : "I"(100));
  return value;
}

static int and_via_k(int value) {
  __asm__("and %w0, %w0, %1" : "+r"(value) : "K"(0xff));
  return value;
}

static long and_via_l(long value) {
  __asm__("and %0, %0, %1" : "+r"(value) : "L"(0xffL));
  return value;
}

int main(void) {
  printf("%d %d %ld\n", add_via_i(5), and_via_k(0x1ff), and_via_l(0x1ffL));
  return 0;
}
