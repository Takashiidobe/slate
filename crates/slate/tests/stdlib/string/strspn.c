#include <stdio.h>
#include <string.h>
int main(void) {
  const char *volatile v = "abc:def";
  const char *s = (char *)v;
  printf("%zu %zu %zu\n", strspn(s, "abc"), strspn(s, "xyz"), strspn(s, ""));
  return 0;
}
