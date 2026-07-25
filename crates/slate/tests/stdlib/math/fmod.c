#include <math.h>
#include <stdio.h>
int main(void) {
  double volatile x = 10.0, y = 3.0;
  printf("%f\n", fmod(x, y));
  return 0;
}
