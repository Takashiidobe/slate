#include <stdio.h>

static int intel_add(int value) {
  __asm__ volatile(".intel_syntax noprefix\n\t"
                   "add %V0, 7\n\t"
                   ".att_syntax prefix"
                   : "+r"(value));
  return value;
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

static long intel_shift_wide(long value, long amount) {
  __asm__(".intel_syntax noprefix\n\t"
          "lea %V0, [%V0 + %V1 * 4]\n\t"
          ".att_syntax prefix"
          : "+r"(value)
          : "r"(amount));
  return value;
}

static int att_subtract(int value) {
  __asm__ volatile("subl $2, %0" : "+r"(value));
  return value;
}

int main(void) {
  printf("%d %d %d %d %ld\n", intel_add(5), att_subtract(5),
         compare_intel_equal(7, 7), compare_intel_equal(7, 8),
         intel_shift_wide(0x100000000L, 3));
  return 0;
}
