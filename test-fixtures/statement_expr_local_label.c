#include <stdio.h>

#define CHECK_VALUE(expression)                                               \
  ({                                                                          \
    __label__ failed, done;                                                    \
    int result;                                                               \
    if (!(expression))                                                        \
      goto failed;                                                            \
    result = 17;                                                              \
    goto done;                                                                \
  failed:                                                                     \
    result = -5;                                                              \
  done:                                                                       \
    result;                                                                   \
  })

int main(void) {
  int value = 0;
  int first = CHECK_VALUE(++value == 1);
  int second = CHECK_VALUE(++value == 9);
  printf("%d %d %d\n", first, second, value);
  return 0;
}
