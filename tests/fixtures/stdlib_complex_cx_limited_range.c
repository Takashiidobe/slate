#include <complex.h>
#include <stdio.h>

int main(void) {
  volatile double real = 0.5;
  volatile double imaginary = -0.25;
  double complex a = __builtin_complex(real, imaginary);
  double complex b = __builtin_complex(real, -imaginary);
  double complex product;
  double complex quotient;

  {
#pragma STDC CX_LIMITED_RANGE ON
    product = a * b;
  }

  {
#pragma STDC CX_LIMITED_RANGE OFF
    quotient = a / b;
  }

  printf("%.4f %.4f %.4f %.4f\n", creal(product), cimag(product),
         creal(quotient), cimag(quotient));
  return creal(product) == 0.3125 && cimag(product) == 0.0 &&
                 creal(quotient) == 0.6 && cimag(quotient) == -0.8
             ? 0
             : 1;
}
