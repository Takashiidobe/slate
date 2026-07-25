#include <stdio.h>

int main(void) {
  double pos = 1234.5678;
  double neg = -1234.5678;
  double zero = 0.0;
  double big = 1e300;
  double small = 1e-300;
  printf("%e %.2e %10.2e %+e\n", pos, pos, pos, pos);
  printf("%E %.2E\n", pos, pos);
  printf("%-10.2e|\n", pos);
  printf("%e %+e\n", neg, neg);
  printf("%.0e\n", pos);
  printf("%e\n", zero);
  printf("%e %E\n", big, small);
  return 0;
}
