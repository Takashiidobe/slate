#include <float.h>
#include <stdio.h>
int main(void) {
  printf("%d %d %d\n", FLT_RADIX, DBL_MANT_DIG, FLT_MANT_DIG);
  printf("%d %d\n", DBL_DIG, FLT_DIG);
  printf("%d %d\n", DBL_MAX_EXP, DBL_MIN_EXP);
  return 0;
}
