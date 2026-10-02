#include <stdarg.h>
#include <stdio.h>

typedef int (*Callback)(int, ...);
struct Dispatch { Callback callback; };

static int total(int count, ...) {
  va_list args;
  va_start(args, count);
  int result = va_arg(args, int);
  result += (int)va_arg(args, double);
  result += va_arg(args, int);
  va_end(args);
  return count + result;
}

static Callback select_callback(Callback callback) { return callback; }

int main(void) {
  struct Dispatch dispatch = {total};
  Callback callback = select_callback(dispatch.callback);
  signed char a = 7;
  float b = 2.5f;
  unsigned short c = 19;
  if (callback(3, a, b, c) != 31) return 1;
  callback = 0;
  if (callback != 0) return 2;
  int (*format)(char *, size_t, const char *, ...) = snprintf;
  char buffer[32];
  if (format(buffer, sizeof(buffer), "%d %.1f", a, b) != 5) return 3;
  puts(buffer);
  return 0;
}
