#include <stdio.h>

__attribute__((nonnull(1, 2))) int add(int *a, int *b) { return *a + *b; }

int main(void) {
  int x = 3;
  int y = 4;
  printf("%d\n", add(&x, &y));
  return 0;
}



// SLATE-FILECHECK-ISYSTEM ~/Projects/slate/libc-shim/include /usr/lib/clang/22/include

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// SLATE-FILECHECK-END DEFAULT
