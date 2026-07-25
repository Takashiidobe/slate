#include <math.h>
#include <stdio.h>
int main(void) {
  double volatile x = 2.0, y = 10.0;
  printf("%f\n", pow(x, y));
  return 0;
}
