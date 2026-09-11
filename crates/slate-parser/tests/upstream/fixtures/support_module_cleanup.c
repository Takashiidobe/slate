#include <stdio.h>
#include <stdlib.h>

int main(void) {
  char digits[] = "123";
  printf("%d\n", atoi(digits));
  return 0;
}


// LOWERING: #![feature(c_variadic)]

// REWRITES: #![feature(c_variadic)]
