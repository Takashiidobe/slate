#include <stdio.h>

int main(void) {
  int x;
#if defined(__linux__)
  x = 1;
#else
  x = 2;
#endif
  printf("%d\n", x);
  return 0;
}
