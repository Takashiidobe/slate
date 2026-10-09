#include <stdarg.h>

double sum_args(int wide, int count, ...) {
  va_list arglist;
  double  total = 0;
  va_start(arglist, count);
  for (int i = 0; i < count; i++) {
    if (wide)
      total += (double)va_arg(arglist, long double);
    else
      total += va_arg(arglist, double);
  }
  va_end(arglist);
  return total;
}
