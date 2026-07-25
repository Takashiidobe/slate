#include <math.h>
#include <stdio.h>
int main(void) {
  double volatile x = 2.0;
  printf("%f\n", sqrt(x));
  return 0;
}
