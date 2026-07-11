#include <complex.h>
#include <stdio.h>
int main(void) {
  double complex z = 3.0 + 4.0 * I;
  printf("%f %f %f\n", creal(z), cimag(z), cabs(z));
  return 0;
}
