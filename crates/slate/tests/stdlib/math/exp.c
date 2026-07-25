#include <math.h>
#include <stdio.h>
int main(void) {
  double volatile x = 0.0;
  printf("%f\n", exp(x));
  return 0;
}
