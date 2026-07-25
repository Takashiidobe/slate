#include <stdio.h>
#include <string.h>
int main(void) {
  char b[16] = "foo";
  char *r = strcat(b, "bar");
  strcat(b, "");
  printf("%s %d %zu\n", b, r == b, strlen(b));
  return 0;
}
