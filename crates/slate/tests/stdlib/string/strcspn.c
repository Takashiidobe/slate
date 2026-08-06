#include <stdio.h>
#include <string.h>
int main(void) {
  const char *volatile v = "abc:def";
  const char *s          = (char *)v;
  printf("%zu %zu %zu\n", strcspn(s, ":"), strcspn(s, "xyz"), strcspn(s, ""));
  return 0;
}
