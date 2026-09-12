#include <stdio.h>

int main(void) {
  int a = 2;
  int b = 3;
  printf("sum: %d and %d\n", a, b);
  printf("hex: %x\n", a);
  printf("no newline: %d", a + b);
}



// SLATE-FILECHECK-ISYSTEM ~/Projects/slate/libc-shim/include /usr/lib/clang/22/include

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// SLATE-FILECHECK-END DEFAULT
