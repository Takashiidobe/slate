#include <stdio.h>

__attribute__((nonnull)) int use_all(int *a, int *b) { return *a - *b; }

int main(void) {
  int x = 10;
  int y = 4;
  printf("%d\n", use_all(&x, &y));
  return 0;
}

// LOWERING: #![feature(c_variadic)]

// REWRITES: #![feature(c_variadic)]

// SLATE-FILECHECK-ISYSTEM ~/Projects/slate/libc-shim/include /usr/lib/clang/22/include

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// SLATE-FILECHECK-END DEFAULT
