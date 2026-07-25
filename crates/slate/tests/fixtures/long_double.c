#include <stdio.h>

static long double mix_long_double(long double a, long double b) {
  long double c = (a + b) / 2.0L;
  return c * 3.0L;
}

static int truncate_long_double(long double value) { return (int)value; }

int main(void) {
  long double x = 1.5L;
  long double y = 4.5L;
  printf("%d\n", truncate_long_double(x + y));
  printf("%d\n", truncate_long_double(mix_long_double(3.0L, 5.0L)));
  printf("%d\n", truncate_long_double((long double)7 / 2.0L));
  return 0;
}
