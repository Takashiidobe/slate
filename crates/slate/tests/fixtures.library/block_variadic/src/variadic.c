// { dg-options "-fblocks" }
#include <stdarg.h>

extern void *_Block_copy(const void *);

typedef long (^summer)(int, ...);

summer make_summer(long base) {
  return _Block_copy(^(int n, ...) {
    va_list ap;
    va_start(ap, n);
    long s = base;
    for (int i = 0; i < n; i++) s += va_arg(ap, long);
    va_end(ap);
    return s;
  });
}

double apply(double (^f)(int, ...), double scale) {
  return f(3, 1.5 * scale, 2.0, (double)'a') + f(1, 0.5);
}
