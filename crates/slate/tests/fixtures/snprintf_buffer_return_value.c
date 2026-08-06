#include <stdio.h>

int main(void) {
  char buf[64];
  int  n = snprintf(buf, sizeof(buf), "%d-%d", 3, 4);
  printf("n=%d buf=%s\n", n, buf);
  return 0;
}
