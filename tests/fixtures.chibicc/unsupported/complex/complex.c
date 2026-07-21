#include "test.h"

int main() {
  // Size tests
  ASSERT(8, sizeof(float _Complex));
  ASSERT(16, sizeof(double _Complex));
  ASSERT(32, sizeof(long double _Complex));

  // Alignment tests
  ASSERT(4, _Alignof(float _Complex));
  ASSERT(8, _Alignof(double _Complex));
  ASSERT(16, _Alignof(long double _Complex));

  // Variable declaration and storage
  float _Complex fc = 0;
  double _Complex dc = 0;
  long double _Complex ldc = 0;

  // Basic addition - cast results to int for testing
  double _Complex z1 = 1.0 + 2.0 * 1.0i;
  double _Complex z2 = 3.0 + 4.0 * 1.0i;
  double _Complex sum = z1 + z2;
  ASSERT(4, (int)__real__ sum);
  ASSERT(6, (int)__imag__ sum);

  // Basic subtraction
  double _Complex diff = z2 - z1;
  ASSERT(2, (int)__real__ diff);
  ASSERT(2, (int)__imag__ diff);

  // Multiplication: (1+2i)*(3+4i) = 3+4i+6i+8i^2 = 3+10i-8 = -5+10i
  double _Complex prod = z1 * z2;
  ASSERT(-5, (int)__real__ prod);
  ASSERT(10, (int)__imag__ prod);

  // Division: (3+4i)/(1+2i)
  // = (3+4i)(1-2i)/((1+2i)(1-2i)) = (3-6i+4i-8i^2)/(1+4) = (11-2i)/5 = 2.2-0.4i
  double _Complex quot = z2 / z1;
  ASSERT(2, (int)__real__ quot);
  ASSERT(0, (int)__imag__ quot);

  // Float complex
  float _Complex fc1 = 1.0f + 2.0f * 1.0fi;
  float _Complex fc2 = 3.0f + 4.0f * 1.0fi;
  float _Complex fsum = fc1 + fc2;
  ASSERT(4, (int)__real__ fsum);
  ASSERT(6, (int)__imag__ fsum);

  // Float complex multiplication
  float _Complex fprod = fc1 * fc2;
  ASSERT(-5, (int)__real__ fprod);
  ASSERT(10, (int)__imag__ fprod);

  // Casting real to complex
  double _Complex from_real = (double _Complex)5.0;
  ASSERT(5, (int)__real__ from_real);
  ASSERT(0, (int)__imag__ from_real);

  // Casting complex to real
  double to_real = (double)z1;
  ASSERT(1, (int)to_real);

  // Cast between complex types
  float _Complex from_dc = (float _Complex)z1;
  ASSERT(1, (int)__real__ from_dc);
  ASSERT(2, (int)__imag__ from_dc);

  printf("OK\n");
  return 0;
}
