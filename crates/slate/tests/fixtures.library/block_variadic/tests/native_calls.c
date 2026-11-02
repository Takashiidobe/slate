#include <stdarg.h>
#include <stdio.h>

extern void _Block_release(const void *);

typedef long (^summer)(int, ...);

summer make_summer(long base);
double apply(double (^f)(int, ...), double scale);

int main(void) {
  summer s = make_summer(1000);
  printf("%ld %ld\n", s(3, 1L, 20L, 300L), s(0));
  _Block_release(s);
  double offset = 0.25;
  double r = apply(^(int n, ...) {
    va_list ap;
    va_start(ap, n);
    double t = offset;
    for (int i = 0; i < n; i++) t += va_arg(ap, double);
    va_end(ap);
    return t;
  }, 2.0);
  printf("%.3f\n", r);
  return 0;
}
