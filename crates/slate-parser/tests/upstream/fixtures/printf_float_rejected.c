#include <stdio.h>

int main(void) {
  double x = 1.25;
  printf("%a\n", x);
  return 0;
}


// LOWERING: #![feature(c_variadic)]

// REWRITES: #![feature(c_variadic)]
