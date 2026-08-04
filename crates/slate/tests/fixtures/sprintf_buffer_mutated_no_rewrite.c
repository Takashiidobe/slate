#include <stdio.h>

int main(void) {
  char buf[64];
  sprintf(buf, "%d-%d", 3, 4);
  buf[0] = 'X';
  puts(buf);
  return 0;
}
