#include <stdio.h>
#include <string.h>
int main(void) {
  char  b[6];
  void *r = memset(b, 'x', 5);
  memset(b + 1, 'z', 0);
  b[5] = 0;
  printf("%s %d %c\n", b, r == b, b[1]);
  return 0;
}
