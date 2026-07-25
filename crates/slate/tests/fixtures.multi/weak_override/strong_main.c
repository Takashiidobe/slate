#include <stdio.h>

int weak_global = 91;

int fallback_value(void) { return 42; }

int main(void) {
  printf("%d %d\n", fallback_value(), weak_global);
  return 0;
}
