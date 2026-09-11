#include <stdio.h>

static int inot(int a) { return ~a; }

int main(void) {
  printf("%d\n", inot(0));
  printf("%d\n", inot(12));
  printf("%d\n", inot(-1));
  unsigned int u = 0xF0u;
  printf("%u\n", ~u);
  return 0;
}

// LOWERING: #![feature(c_variadic)]

// REWRITES: #![feature(c_variadic)]
