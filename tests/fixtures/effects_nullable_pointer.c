#include <stdio.h>

int main(void) {
  unsigned char buf[4] = {10, 20, 30, 40};
  unsigned char *hit = (unsigned char *)__builtin_memchr(buf, 30, 4);
  unsigned char *miss = (unsigned char *)__builtin_memchr(buf, 99, 4);
  printf("%ld %d\n", hit - buf, miss == 0);
  return 0;
}
