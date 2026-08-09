#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  unsigned char byte = 7;
  (void)arc4random();
  arc4random_buf(&byte, 0);
  printf("%u %u\n", arc4random_uniform(1), byte);
  return 0;
}
