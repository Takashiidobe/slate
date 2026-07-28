#include <float.h>
#include <stdio.h>

int main(void) {
  long double zero = 0.0L;
  long double tiny = LDBL_TRUE_MIN;
  long double one = 1.0L;

  printf("%d %d %d %d\n", tiny > zero, zero < tiny, one >= tiny,
         tiny <= one);
  printf("%d %d %d %d\n", tiny < zero, zero > tiny, one <= tiny,
         tiny >= one);
  return 0;
}
