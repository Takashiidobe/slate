#include <stdio.h>
#include <stdlib.h>

int main(void) {
  int *p = malloc(sizeof(int));
  *p     = 41;
  *p     = *p + 1;
  printf("%d\n", *p);
  free(p);
  return 0;
}

// LOWERING: #![feature(c_variadic)]

// REWRITES: #![feature(c_variadic)]
