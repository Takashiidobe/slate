#include <stdio.h>

int main(void) {
  char buf[64];
  sprintf(buf, "%d-%d", 3, 4);
  printf("value: %s\n", buf);
  return 0;
}
