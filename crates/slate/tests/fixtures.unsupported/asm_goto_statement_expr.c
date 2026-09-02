#include <stdio.h>

#define CLASSIFY_ZERO(value)                                                   \
  ({                                                                           \
    __label__ zero, done;                                                      \
    int result = 0;                                                            \
    __asm__ goto("testl %0, %0\n\tjz %l[zero]" : : "r"(value) : "cc" : zero);  \
    result = 11;                                                               \
    goto done;                                                                 \
  zero:                                                                        \
    result = 13;                                                               \
  done:                                                                        \
    result;                                                                    \
  })

int main(void) {
  printf("%d %d\n", CLASSIFY_ZERO(0), CLASSIFY_ZERO(9));
  return 0;
}
