#include <complex.h>

int main(void) {
  volatile double real = 0.5;
  volatile double imaginary = -0.25;
#pragma STDC CX_LIMITED_RANGE ON
  double complex value = (real + imaginary * I) * (real - imaginary * I);
#pragma STDC CX_LIMITED_RANGE OFF
  return creal(value) == 0.3125 ? 0 : 1;
}
