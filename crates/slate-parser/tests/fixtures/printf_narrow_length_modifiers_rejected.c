#include <stdio.h>

int main(void) {
  int x    = 300;
  int prec = 2;
  printf("%.*hhd\n", prec, x);
  printf("%.*hd\n", prec, x);
  return 0;
}




// SLATE-FILECHECK-ISYSTEM ~/Projects/slate/libc-shim/include /usr/lib/clang/22/include

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// SLATE-FILECHECK-END DEFAULT
