#include <stdio.h>
#include <stdlib.h>

int main(void) {
  int *p = calloc(4, sizeof(int));
  printf("%d\n", p[0] + p[3]);
  free(p);
  return 0;
}

// LOWERING: #![feature(c_variadic)]

// REWRITES: #![feature(c_variadic)]
