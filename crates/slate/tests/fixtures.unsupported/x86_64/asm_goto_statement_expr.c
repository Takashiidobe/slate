#include <stdio.h>

#define CLASSIFY_ZERO(value)                                                   \
  ({                                                                           \
    __label__ zero;                                                            \
    int result = 11;                                                           \
    __asm__ goto("testl %0, %0\n\tjz %l[zero]" : : "r"(value) : "cc" : zero);  \
    if (0) {                                                                   \
    zero:                                                                      \
      result = 13;                                                             \
    }                                                                          \
    result;                                                                    \
  })

static int classify_zero(int value) {
  int result;
  result = CLASSIFY_ZERO(value);
  return result;
}

int main(void) {
  printf("%d %d\n", classify_zero(0), classify_zero(9));
  return 0;
}
