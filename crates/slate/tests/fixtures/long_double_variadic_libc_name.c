#include <stdarg.h>
#include <stdio.h>

static long double exit(int count, ...) {
  va_list ap;
  va_start(ap, count);
  long double total = 0.0L;
  for (int i = 0; i < count; i++) {
    total += va_arg(ap, long double);
  }
  va_end(ap);
  return total;
}

int main(void) {
  printf("%.2Lf\n", exit(2, 1.25L, 2.5L));
  return 7;
}
