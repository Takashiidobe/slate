#include <stdio.h>

int main(void) {
  printf("%s %c %c %d\n", "tag", 'A', 10, 7);
  printf("literal=%s", "tail");
  return 0;
}

// LOWERING: #![feature(c_variadic)]

// REWRITES: #![feature(c_variadic)]
