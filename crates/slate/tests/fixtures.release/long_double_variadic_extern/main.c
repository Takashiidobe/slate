#include <stddef.h>
#include <stdio.h>

int my_snprintf(char *buffer, size_t size, const char *format, ...);

static char *format_long_double(char *buffer, size_t size, const char *format, long double value) {
  my_snprintf(buffer, size, format, value);
  return buffer;
}

int main(void) {
  char buffer[128];
  long double x = 0x1.0000000000000001p0L;
  printf("%s\n", format_long_double(buffer, sizeof buffer, "%.21Lf", x));
  int n = my_snprintf(buffer, sizeof buffer, "%d %.3f %La %d %.2Lf", 7, 2.5, x, -3, -x / 3);
  printf("%d %s\n", n, buffer);
  return 0;
}
