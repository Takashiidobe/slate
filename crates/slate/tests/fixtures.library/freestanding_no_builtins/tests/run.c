#include <stdio.h>

#include "../src/kstring.h"

int main(void) {
  static char buf[64];
  printf("%zu\n", strlen("freestanding"));
  printf("%zu %zu\n", padded_length(buf, 7), padded_length(buf, 40));
  printf("%s\n", buf);
  return 0;
}
