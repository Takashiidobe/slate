#include <stdarg.h>
#include <stdio.h>

int consume(va_list arguments) { return va_arg(arguments, int); }

int relay(int count, ...) {
  va_list arguments;
  va_start(arguments, count);
  int value = consume(arguments);
  va_end(arguments);
  return value;
}

int main(void) {
  printf("%d\n", relay(1, 37));
  return 0;
}
