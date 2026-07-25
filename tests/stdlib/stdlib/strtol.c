#include <stdio.h>
#include <stdlib.h>
int main(void) {
  printf("%ld\n", strtol("ff", NULL, 16));
  return 0;
}
