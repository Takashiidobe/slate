#include <stdio.h>

static double zero(void) { return 0.0; }

static int compare_all(double a, double b) {
  return (a < b) | (a <= b) << 1 | (a > b) << 2 | (a >= b) << 3 | (a == b) << 4 |
         (a != b) << 5;
}

int main(void) {
  double nan = zero() / zero();
  double inf = 1.0 / zero();
  printf("%d %d %d\n", compare_all(nan, nan), compare_all(nan, 1.0), compare_all(1.0, 2.0));
  printf("%d %d\n", compare_all(-0.0, 0.0), compare_all(inf, -inf));
  printf("%d %d %d\n", !nan, nan ? 1 : 0, -0.0 ? 1 : 0);
  printf("%g %g %d\n", inf, -inf, inf - inf == inf - inf);

  float third = 1.0f / 3.0f;
  float sum   = 0.1f + 0.2f;
  printf("%.10f %.10f %d\n", third, sum, sum == 0.3f);
  double dsum = 0.1 + 0.2;
  printf("%.17g %d\n", dsum, dsum == 0.3);

  float f = 16777216.0f;
  f      += 1.0f;
  printf("%.1f\n", f);

  double d = 2.5;
  d       *= 3;
  d       -= 0.5;
  d       /= 4;
  printf("%g\n", d);

  int i = 7;
  printf("%g %g\n", i / 2.0, i / 2 * 1.0);
  printf("%g %g\n", -d, -(0.0));
  float neg = -1.5f;
  printf("%g %g\n", neg * neg, neg / 0.5f);

  double values[] = {3.5, -1.25, 0.0, 1e300};
  double max      = values[0];
  for (int k = 1; k < 4; k++)
    max = values[k] > max ? values[k] : max;
  printf("%g %g\n", max, values[3] * 10);
  printf("%d %d\n", 1.0f < 1.0, (float)0.1 == 0.1);
  return 0;
}
