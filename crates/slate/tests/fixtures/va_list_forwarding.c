#include <stdarg.h>
#include <stdio.h>

static int format_into(char *buffer, size_t size, const char *fmt, va_list ap) {
  return vsnprintf(buffer, size, fmt, ap);
}

static int report(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  int written = vprintf(fmt, ap);
  va_end(ap);
  return written;
}

static int render(char *buffer, size_t size, const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  int written = format_into(buffer, size, fmt, ap);
  va_end(ap);
  return written;
}

static long sum_ints(int count, ...) {
  va_list ap;
  va_start(ap, count);
  long total = 0;
  for (int i = 0; i < count; i++) {
    total += va_arg(ap, int);
  }
  va_end(ap);
  return total;
}

static double mixed(int count, ...) {
  va_list ap;
  va_start(ap, count);
  double total = 0;
  for (int i = 0; i < count; i++) {
    int kind = va_arg(ap, int);
    if (kind == 0) {
      total += va_arg(ap, int);
    } else if (kind == 1) {
      total += va_arg(ap, double);
    } else if (kind == 2) {
      const char *text = va_arg(ap, const char *);
      total += text[0];
    } else {
      total += (double)va_arg(ap, long long);
    }
  }
  va_end(ap);
  return total;
}

static int sum_twice(int count, ...) {
  va_list ap, copy;
  va_start(ap, count);
  va_copy(copy, ap);
  int first = 0;
  for (int i = 0; i < count; i++) {
    first += va_arg(ap, int);
  }
  int second = 0;
  for (int i = 0; i < count; i++) {
    second += va_arg(copy, int) * 2;
  }
  va_end(copy);
  va_end(ap);
  va_start(ap, count);
  int restarted = va_arg(ap, int);
  va_end(ap);
  return first + second + restarted;
}

static unsigned take_unsigned(va_list ap) { return va_arg(ap, unsigned); }

static unsigned unsigned_pair(int unused, ...) {
  va_list ap;
  va_start(ap, unused);
  unsigned high = take_unsigned(ap);
  va_end(ap);
  return high;
}

int main(void) {
  char buffer[64];
  int written = report("%s=%d %.1f\n", "value", 42, 2.5);
  int rendered = render(buffer, sizeof buffer, "[%d|%s|%c]", 7, "seven", 'x');
  printf("%d %d %s\n", written, rendered, buffer);
  printf("%ld\n", sum_ints(5, 1, 2, 3, 4, 5));
  printf("%.2f\n", mixed(4, 0, 10, 1, 0.5, 2, "A", 3, 1000000000000LL));
  printf("%d\n", sum_twice(3, 4, 5, 6));
  printf("%u\n", unsigned_pair(0, 4000000000u));
  return (int)(sum_ints(3, 10, 20, 30) % 13);
}
