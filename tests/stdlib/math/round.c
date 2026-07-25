#include <math.h>
#include <stdio.h>
int main(void) {
  double volatile x = 2.5;
  printf("%f\n", round(x));
  return 0;
}
