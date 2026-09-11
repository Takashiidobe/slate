#include <stdio.h>
#include <stdlib.h>

int main(void) {
  int *x = malloc(sizeof(int));
  free(x);
  *x = 4;
  printf("%d %d\n", *x, *x);
  return 0;
}

// LOWERING: #![feature(c_variadic)]

// REWRITES: #![feature(c_variadic)]
