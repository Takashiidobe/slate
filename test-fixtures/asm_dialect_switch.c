#include <stdio.h>

static int intel_add(int value) {
  __asm__ volatile(".intel_syntax noprefix\n\t"
                   "add %V0, 7\n\t"
                   ".att_syntax prefix"
                   : "+r"(value));
  return value;
}

static int att_subtract(int value) {
  __asm__ volatile("subl $2, %0" : "+r"(value));
  return value;
}

int main(void) {
  printf("%d %d\n", intel_add(5), att_subtract(5));
  return 0;
}
