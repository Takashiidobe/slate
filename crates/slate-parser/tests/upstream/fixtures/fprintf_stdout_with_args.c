#include <stdio.h>

int main(void) {
  fprintf(stdout, "value: %d\n", 7);
  fputs("done\n", stdout);
  return 0;
}

// LOWERING: #![feature(c_variadic)]

// REWRITES: #![feature(c_variadic)]
