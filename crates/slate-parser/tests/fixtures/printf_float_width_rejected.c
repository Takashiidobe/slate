#include <stdio.h>

int main(void) {
  double x    = 3.14159;
  int    prec = 2;
  printf("%.*f\n", prec, x);
  return 0;
}


// LOWERING: #![feature(c_variadic)]

// REWRITES: #![feature(c_variadic)]

// SLATE-FILECHECK-ISYSTEM ~/Projects/slate/libc-shim/include /usr/lib/clang/22/include

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// SLATE-FILECHECK-END DEFAULT
