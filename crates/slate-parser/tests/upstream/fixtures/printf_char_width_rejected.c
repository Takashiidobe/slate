#include <stdio.h>

int main(void) {
  int width = 3;
  printf("%*c\n", width, 'a');
  return 0;
}


// LOWERING: #![feature(c_variadic)]

// REWRITES: #![feature(c_variadic)]
