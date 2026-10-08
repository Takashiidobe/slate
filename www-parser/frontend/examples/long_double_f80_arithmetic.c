#include <stdio.h>

int main(void) {
  long double one = 1.0L;
  long double two = 2.0L;
  long double near = 1.0000000000000000001L;

  printf("%Lf\n", one + two);
  printf("%Lf\n", near - one);
  printf("%Lf\n", one * two);
  printf("%Lf\n", two / one);
  printf("%Lf\n", -near);

  printf("%d %d %d %d\n", one < two, near > one, one == one, near != one);
  printf("%d\n", (int)(one + two));
  printf("%Lf\n", (long double)1234567890123LL);
  printf("%d\n", (int)(long double)123456789);
  return 0;
}
