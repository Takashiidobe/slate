#include <stdio.h>
#include <string.h>
int main(void) {
  const char *volatile v = "hello";
  const char *s          = (char *)v;
  printf("%ld %ld %d\n", strchr(s, 'l') - s, strchr(s, 0) - s,
         strchr(s, 'z') == 0);
  return 0;
}
