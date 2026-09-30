#include <stdio.h>

static int freeze_probe(int seed) {
  int value = seed * 2 + 1;
#if __has_builtin(__builtin_nondeterministic_value)
  int frozen = __builtin_nondeterministic_value(value);
  return frozen == frozen;
#else
  return value == value;
#endif
}

int main(void) {
  volatile int input = 20;
  printf("%d\n", freeze_probe(input));
  return 0;
}
