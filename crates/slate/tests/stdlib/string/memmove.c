#include <stdio.h>
#include <string.h>
int main(void) {
  char  b[8] = "abcdef";
  char  c[8] = "abcdef";
  void *r    = memmove(b + 1, b, 3);
  memmove(c, c + 2, 3);
  memmove(c + 5, "Z", 0);
  b[7] = 0;
  c[5] = 0;
  printf("%s %s %d %c\n", b, c, r == b + 1, c[5]);
  return 0;
}
