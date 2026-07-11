#include <string.h>
#include <stdio.h>
int main(void) {
  char b[8] = {0};
  char c[6];
  char d[5];
  memset(c, 'x', sizeof c);
  memset(d, 'x', sizeof d);
  char *r = strncpy(b, "abcdef", 3);
  strncpy(c, "hi", 5);
  strncpy(d, "abcde", 5);
  printf("%s %d %c %c %d %d %d %c\n", b, r == b, c[0], c[1], c[2] == 0, c[3] == 0, c[4] == 0, d[4]);
  return 0;
}
