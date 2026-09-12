#include <stdio.h>

int main(void) {
  double x    = 1234.5678;
  int    prec = 3;
  printf("%g %.3g %-10.3g\n", x, x, x);
  printf("%G %.3G\n", x, x);
  printf("%.*g\n", prec, x);
  printf("% g\n", x);
  return 0;
}




// SLATE-FILECHECK-ISYSTEM ~/Projects/slate/libc-shim/include /usr/lib/clang/22/include

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// SLATE-FILECHECK-END DEFAULT
