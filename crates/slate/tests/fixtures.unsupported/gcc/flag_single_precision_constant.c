/* { dg-options "-fsingle-precision-constant" } */
#include <stdio.h>

int main(void) {
  double tenth = 0.1;
  double third = 1.0 / 3.0;
  long double exact = 0.1L;
  printf("%.17g %.17g %.17Lg\n", tenth, third, exact);
  printf("%d %d\n", tenth == 0.1f, sizeof(1.5) == sizeof(double));
  return 0;
}
