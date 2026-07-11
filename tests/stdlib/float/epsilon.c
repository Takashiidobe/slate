#include <float.h>
#include <stdio.h>
int main(void) {
  double volatile one = 1.0;
  printf("%d\n", (one + DBL_EPSILON) > one ? 1 : 0);
  printf("%d\n", DBL_MAX > DBL_MIN ? 1 : 0);
  printf("%d\n", FLT_MAX > FLT_MIN ? 1 : 0);
  return 0;
}
