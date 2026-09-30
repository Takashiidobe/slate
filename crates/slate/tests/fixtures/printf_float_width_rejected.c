#include <stdio.h>

int main(void) {
  double x    = 3.14159;
  int    prec = 2;
  printf("%.*f\n", prec, x);
  return 0;
}
