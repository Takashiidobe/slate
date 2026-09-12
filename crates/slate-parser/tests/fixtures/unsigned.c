#include <stdio.h>

static unsigned int uadd(unsigned int a, unsigned int b) {
  unsigned int c = a + b;
  return c;
}


int main(void) {
  unsigned int a = 4000000000u;
  unsigned int b = 500000000u;
  printf("%u\n", uadd(a, b));
  printf("%u\n", a);
  printf("%u\n", b);
  return 0;
}

// LOWERING: #![feature(c_variadic)]

// REWRITES: #![feature(c_variadic)]

// SLATE-FILECHECK-ISYSTEM ~/Projects/slate/libc-shim/include /usr/lib/clang/22/include

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// SLATE-FILECHECK-END DEFAULT
