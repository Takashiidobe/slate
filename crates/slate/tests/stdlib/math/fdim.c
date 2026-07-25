#include <math.h>
#include <stdio.h>
int main(void) {
  double volatile a = 7.0, b = 3.0;
  printf("%f %f\n", fdim(a, b), fdim(b, a));
  return 0;
}
