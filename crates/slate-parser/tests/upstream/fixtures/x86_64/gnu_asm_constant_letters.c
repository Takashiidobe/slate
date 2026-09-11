#include <stdio.h>

static int add_const_n(int value) {
  __asm__("addl $%c[amount], %[value]"
          : [value] "+r"(value)
          : [amount] "n"(4)
          : "cc");
  return value;
}

static int shift_const_i_letter(int value) {
  __asm__("sall $%c[amount], %[value]"
          : [value] "+r"(value)
          : [amount] "I"(3)
          : "cc");
  return value;
}

int main(void) {
  printf("%d %d\n", add_const_n(10), shift_const_i_letter(2));
  return 0;
}


