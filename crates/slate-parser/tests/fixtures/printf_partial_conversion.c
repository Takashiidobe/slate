#include <stdio.h>

int main(void) {
  printf("first %d\n", 1);
  printf("dynamic %*d\n", 5, 2);
  printf("last %d\n", 3);
  return 0;
}



// SLATE-FILECHECK-ISYSTEM ~/Projects/slate/libc-shim/include /usr/lib/clang/22/include

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// SLATE-FILECHECK-END DEFAULT
