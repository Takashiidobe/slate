#include <math.h>
#include <stdio.h>
int main(void) {
  double volatile x = 2.5, y = -2.5;
  printf("%ld %lld %ld\n", lround(x), llround(y), lrint(x));
  return 0;
}
