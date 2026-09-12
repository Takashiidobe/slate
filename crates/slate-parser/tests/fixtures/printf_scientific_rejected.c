#include <stdio.h>

int main(void) {
  double x    = 1234.5678;
  int    prec = 2;
  printf("%015.2e\n", x);
  printf("%.*e\n", prec, x);
  printf("% e\n", x);
  printf("%#e\n", x);
  return 0;
}

// LOWERING: #![feature(c_variadic)]

// REWRITES: #![feature(c_variadic)]

// SLATE-FILECHECK-ISYSTEM ~/Projects/slate/libc-shim/include /usr/lib/clang/22/include

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// SLATE-FILECHECK-END DEFAULT
