#include <stdio.h>

int use_it(int *a, int *b) __attribute__((nonnull(1, 2)));

int main(void) {
  int x = 6;
  int y = 2;
  printf("%d\n", use_it(&x, &y));
  return 0;
}

int use_it(int *a, int *b) { return *a / *b; }

// LOWERING: #![feature(c_variadic)]

// REWRITES: #![feature(c_variadic)]

// SLATE-FILECHECK-ISYSTEM ~/Projects/slate/libc-shim/include /usr/lib/clang/22/include

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// SLATE-FILECHECK-END DEFAULT
