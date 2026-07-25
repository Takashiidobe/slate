#include <stdio.h>
#include <string.h>
int main(void) {
  char b[16] = "foo";
  char *r = strncat(b, "barbaz", 3);
  strncat(b, "zzz", 0);
  printf("%s %d %zu\n", b, r == b, strlen(b));
  return 0;
}
