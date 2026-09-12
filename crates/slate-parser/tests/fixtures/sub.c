#include <stdio.h>

static int isub(int a, int b) { return a - b; }

int main(void) {
  printf("%d\n", isub(10, 25));
  unsigned int u = 3u;
  unsigned int w = 10u;
  printf("%u\n", u - w);
  return 0;
}

// LOWERING: #![feature(c_variadic)]

// REWRITES: #![feature(c_variadic)]

// SLATE-FILECHECK-ISYSTEM ~/Projects/slate/libc-shim/include /usr/lib/clang/22/include

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// SLATE-FILECHECK-END DEFAULT
