#include <stdio.h>

static int ishl(int a, int b) { return a << b; }

int main(void) {
  printf("%d\n", ishl(1, 4));
  printf("%d\n", ishl(3, 5));
  unsigned int u = 1u;
  printf("%u\n", u << 31);
  return 0;
}

// LOWERING: #![feature(c_variadic)]

// REWRITES: #![feature(c_variadic)]

// SLATE-FILECHECK-ISYSTEM ~/Projects/slate/libc-shim/include /usr/lib/clang/22/include

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// SLATE-FILECHECK-END DEFAULT
