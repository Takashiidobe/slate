#include <stdio.h>

int main(void) {
  char        buf[64];
  const char *fmt = "%d-%d";
  sprintf(buf, fmt, 3, 4);
  puts(buf);
  return 0;
}
