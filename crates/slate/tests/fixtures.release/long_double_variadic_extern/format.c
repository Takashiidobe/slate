#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>

int my_snprintf(char *buffer, size_t size, const char *format, ...) {
  va_list ap;
  va_start(ap, format);
  int written = vsnprintf(buffer, size, format, ap);
  va_end(ap);
  return written;
}
