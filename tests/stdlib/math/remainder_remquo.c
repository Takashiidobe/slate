#include <math.h>
#include <stdio.h>
int main(void) {
  int q1 = 0;
  int q2 = 0;
  double a = 17.5;
  double b = 4.0;
  double c = -17.5;
  printf("%.6f %.6f\n", remainder(a, b), remainder(c, b));
  printf("%.6f %d %.6f %d\n", remquo(a, b, &q1), q1, remquo(c, b, &q2), q2);
  return 0;
}
