#include <math.h>
#include <stdio.h>
int main(void) {
  double volatile a = 2.0, b = 3.0, c = 4.0;
  printf("%f\n", fma(a, b, c));
  return 0;
}
