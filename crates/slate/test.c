#include <stdio.h>

int main(void) {
  long double ld = 3.1415926535897932384626433832795L;

  printf("long double: %.10Lf\n", ld);

  printf("via double: %.10f\n", (double)ld);
}
