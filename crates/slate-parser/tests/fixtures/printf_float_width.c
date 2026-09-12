#include <stdio.h>

int main(void) {
  double a = 3.14159;
  double b = -3.14159;
  printf("%8.2f|%+8.2f|%08.2f|%-8.2f|%+.2f\n", a, a, b, a, b);
  return 0;
}



// SLATE-FILECHECK-ISYSTEM ~/Projects/slate/libc-shim/include /usr/lib/clang/22/include

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// SLATE-FILECHECK-END DEFAULT
