#include <assert.h>
#include <stdio.h>

int main(void) {
  printf("before\n");
  assert(5 == 5);
  printf("after\n");
  return 0;
}

// LOWERING: #![feature(c_variadic)]

// REWRITES: #![feature(c_variadic)]
