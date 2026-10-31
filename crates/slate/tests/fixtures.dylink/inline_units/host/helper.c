#include <stdio.h>

int format_sum(char *out, size_t size, int sum) {
  int written = snprintf(out, size, "sum=%d", sum);
  printf("helper wrote %d bytes\n", written);
  return written;
}
