#include <stdarg.h>
#include <stdio.h>
static int sum(int n, ...) {
  va_list ap;
  va_start(ap, n);
  int total = 0;
  for (int i = 0; i < n; i++) total += va_arg(ap, int);
  va_end(ap);
  return total;
}
int main(void) { printf("%d\n", sum(4, 10, 20, 30, 40)); return 0; }
