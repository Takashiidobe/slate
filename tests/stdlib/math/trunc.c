#include <math.h>
#include <stdio.h>
int main(void) {
  double volatile x = 2.9;
  printf("%f\n", trunc(x));
  return 0;
}
