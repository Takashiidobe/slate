#include <stdio.h>
#include <stdlib.h>

int main(void) {
  char *end = 0;
  long long v = strtoll("-7xyz", &end, 10);
  printf("%lld %c\n", v, *end);
  return 0;
}
