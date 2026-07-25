#include <stdio.h>

int main(void) {
  int a = 42;
  int b = 7;
  int c = -42;
  unsigned int u = 9u;
  long l = 123L;
  printf("%05d|%-4d|%+d|%5u|%+06ld\n", a, b, c, u, l);
  return 0;
}
