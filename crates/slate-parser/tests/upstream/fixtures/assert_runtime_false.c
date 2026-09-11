#include <assert.h>
#include <stdio.h>

int main(int argc, char **argv) {
  assert(argc == 2);
  printf("after\n");
  return 0;
}

// LOWERING: #![feature(c_variadic)]

// REWRITES: #![feature(c_variadic)]
