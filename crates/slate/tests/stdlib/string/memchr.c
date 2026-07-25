#include <stdio.h>
#include <string.h>
int main(void) {
  const char *volatile s = "abcdef";
  char *base = (char *)s;
  char *p = memchr(base, 'd', 6);
  char *z = memchr(base, 'a', 0);
  char *n = memchr(base, 'x', 6);
  printf("%ld %d %d\n", p - base, z == 0, n == 0);
  return 0;
}
