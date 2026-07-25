#include <stdio.h>
#include <string.h>
int main(void) {
  const char *volatile v = "hello world";
  const char *s = (char *)v;
  printf("%ld %ld %d\n", strstr(s, "world") - s, strstr(s, "") - s,
         strstr(s, "mars") == 0);
  return 0;
}
