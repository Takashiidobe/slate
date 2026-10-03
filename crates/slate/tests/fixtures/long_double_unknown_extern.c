#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  long double value = 3.25L;
  int         decpt = 0;
  int         sign  = 0;
  char       *digits = qecvt(value, 6, &decpt, &sign);
  char        buffer[64];
  qgcvt(-value / 4, 8, buffer);
  printf("%s %d %d %s\n", digits, decpt, sign, buffer);
  return 0;
}
