#include <stdio.h>

int main(void) {
  char buf[4];
  sprintf(buf, "%d", 7);
  puts(buf);
  return 0;
}
