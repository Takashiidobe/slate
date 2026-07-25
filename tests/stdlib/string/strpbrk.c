#include <stdio.h>
#include <string.h>
int main(void) {
  const char *volatile v = "abcdef";
  const char *s = (char *)v;
  printf("%ld %d %d\n", strpbrk(s, "xdy") - s, strpbrk(s, "xyz") == 0,
         strpbrk(s, "") == 0);
  return 0;
}
