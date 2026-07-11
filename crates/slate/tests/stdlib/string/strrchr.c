#include <string.h>
#include <stdio.h>
int main(void) {
  const char *volatile v = "hello";
  const char *s = (char*)v;
  printf("%ld %ld %d\n", strrchr(s, 'l') - s, strrchr(s, 0) - s, strrchr(s, 'z') == 0);
  return 0;
}
