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

static int compare_intel_equal(int left, int right) {
  int equal;

  __asm__(".intel_syntax noprefix\n\t"
          "cmp %V1, %V2\n\t"
          ".att_syntax prefix"
          : "=@ccz"(equal)
          : "r"(left), "r"(right));
  return equal;
}

static int compare_flag_widths(int value) {
  unsigned short sign;
  unsigned long  nonzero;

  __asm__("testl %2, %2" : "=@ccs"(sign), "=@ccne"(nonzero) : "r"(value));
  return (int)(sign * 10 + nonzero);
}

int main(void) {
  printf("%d %d %d %d %d\n", compare_flags(5, 5), compare_flags(9, 4),
         compare_flags(-1, 1), compare_intel_equal(7, 7),
         compare_flag_widths(-1));
  return 0;
}
