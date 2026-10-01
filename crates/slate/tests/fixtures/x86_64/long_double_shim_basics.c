#include <math.h>
#include <stdio.h>

long double scale = 2.5L;
long double zeroed_global;

static long double average(long double a, long double b) {
  return (a + b) / 2.0L;
}

static int compare(long double a, long double b) {
  return (a < b) + 2 * (a <= b) + 4 * (a > b) + 8 * (a >= b) + 16 * (a == b) +
         32 * (a != b);
}

int main(void) {
  long double x = 1.0L / 3.0L;
  long double y = x * scale - 0.25L;
  long double uninit;
  uninit = -y;
  printf("%.18Lf %.18Lf %.18Lf\n", x, y, uninit);
  printf("%.18Lf %.18Lf\n", average(x, y), zeroed_global);

  int i = (int)(y * 100.0L);
  unsigned long long big = (unsigned long long)18446744073709551615.0L;
  long long neg = (long long)-1234567.75L;
  double d = (double)x;
  float f = (float)y;
  long double from_int = (long double)i + (long double)big + (long double)neg;
  long double from_double = (long double)d + (long double)f;
  printf("%d %llu %lld %.17g %.9g\n", i, big, neg, d, (double)f);
  printf("%.6Lf %.18Lf\n", from_int, from_double);

  printf("%d %d %d\n", compare(x, y), compare(y, x), compare(x, x));
  printf("%d\n", x == (long double)d);

  long double root = sqrtl(2.0L);
  printf("%.18Lf %.18Lf\n", root, fabsl(-root));
  printf("%La\n", root * root);

  long double acc = 0.0L;
  for (int k = 1; k <= 10; k++)
    acc += 1.0L / (long double)k;
  printf("%.18Lf\n", acc);
  return y > x ? 0 : 1;
}
