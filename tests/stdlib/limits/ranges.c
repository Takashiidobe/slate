#include <limits.h>
#include <stdio.h>
int main(void) {
  printf("%d %d %d\n", SCHAR_MIN, SCHAR_MAX, UCHAR_MAX);
  printf("%d %d\n", SHRT_MIN, SHRT_MAX);
  printf("%d %d\n", INT_MIN, INT_MAX);
  printf("%ld %ld\n", LONG_MIN, LONG_MAX);
  printf("%d\n", CHAR_BIT);
  return 0;
}
