#include <string.h>
#include <stdio.h>
int main(void) {
  char d[8] = "xxxx";
  void *r = memcpy(d, "abcd", 5);
  memcpy(d + 1, "Z", 0);
  printf("%s %d %c\n", d, r == d, d[1]);
  return 0;
}
