#include <stdio.h>
int main(void) {
  char b[8];
  snprintf(b, sizeof b, "%d", 123456);
  puts(b);
  return 0;
}
