#include <stdio.h>

static short sadd(short a, short b) {
  short c = a + b;
  return c;
}

int main(void) {
  short          s   = 30000;
  unsigned short us  = 60000;
  short          neg = -12345;
  printf("%d\n", sadd(s, -30000));
  printf("%d\n", us);
  printf("%d\n", neg);
  return 0;
}
