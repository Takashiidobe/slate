#include <stdarg.h>
#include <stdio.h>

static long double sum_long_doubles(int count, ...) {
  va_list ap;
  va_start(ap, count);
  long double total = 0.0L;
  for (int i = 0; i < count; i++) {
    total += va_arg(ap, long double);
  }
  va_end(ap);
  return total;
}

static long double mixed(int count, ...) {
  va_list ap;
  va_start(ap, count);
  long double total = 0.0L;
  for (int i = 0; i < count; i++) {
    int kind = va_arg(ap, int);
    if (kind == 0) {
      total += va_arg(ap, int);
    } else if (kind == 1) {
      total += va_arg(ap, double);
    } else {
      total += va_arg(ap, long double);
    }
  }
  va_end(ap);
  return total;
}

static long double first_twice(int unused, ...) {
  va_list ap, copy;
  va_start(ap, unused);
  va_copy(copy, ap);
  long double first  = va_arg(ap, long double);
  long double second = va_arg(ap, long double);
  long double again  = va_arg(copy, long double);
  va_end(copy);
  va_end(ap);
  return first * 100.0L + second * 10.0L + again;
}

int main(void) {
  long double precise = 1.0L + 0x1p-63L;
  printf("%.3Lf\n", sum_long_doubles(3, 1.25L, 2.5L, -0.75L));
  printf("%La\n", sum_long_doubles(2, precise, 0.0L));
  printf("%.3Lf\n", mixed(12, 0, 1, 1, 0.5, 2, 1.5L, 1, 2.0, 1, 3.0, 1, 4.0, 1, 5.0, 1, 6.0, 1, 7.0,
                          1, 8.0, 2, 0.25L, 1, 9.0));
  printf("%.1Lf\n", first_twice(0, 3.0L, 4.0L));
  return (int)sum_long_doubles(2, 20.5L, 21.5L);
}
