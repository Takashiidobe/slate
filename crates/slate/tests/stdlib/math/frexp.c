#include <math.h>
#include <stdio.h>
int main(void) {
  int e             = 0;
  double volatile x = 8.0;
  double m          = frexp(x, &e);
  printf("%f %d\n", m, e);
  return 0;
}
