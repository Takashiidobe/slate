#include <stdio.h>
#include <stdlib.h>
int main(void) {
  printf("%lu\n", strtoul("12345", NULL, 10));
  return 0;
}
