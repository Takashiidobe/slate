#include <stdarg.h>
#include <stdio.h>

#include "../src/ring.h"

void panic(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vprintf(fmt, args);
  va_end(args);
}

int main(void) {
  for (int i = 1; i <= 5; i++)
    printf("push %d -> %d\n", i, ring_push(i));
  printf("sum %d\n", ring_sum());
  return 0;
}
