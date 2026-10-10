#include <stdio.h>

static int compare_flags(int left, int right) {
  int equal;
  int above;
  int less;

  __asm__("cmpl %4, %3"
          : "=@cce"(equal), "=@cca"(above), "=@ccl"(less)
          : "r"(left), "r"(right));
  return equal * 100 + above * 10 + less;
}

static int compare_flag_widths(int value) {
  unsigned short sign;
  unsigned long  nonzero;

  __asm__("testl %2, %2" : "=@ccs"(sign), "=@ccne"(nonzero) : "r"(value));
  return (int)(sign * 10 + nonzero);
}

static int carry_bool(unsigned a, unsigned b) {
  _Bool carry;
  unsigned char zero;

  __asm__("addl %3, %2" : "=@ccc"(carry), "=@ccz"(zero), "+r"(a) : "r"(b));
  return carry * 10 + zero;
}

int main(void) {
  printf("%d %d %d %d\n", compare_flags(5, 5), compare_flags(9, 4),
         compare_flags(-1, 1), compare_flag_widths(-1));
  printf("%d %d %d\n", carry_bool(0xffffffffu, 2), carry_bool(0xffffffffu, 1),
         carry_bool(1, 2));
  return 0;
}
