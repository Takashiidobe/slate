#include <stdio.h>
#include <stdlib.h>

void bail(int code) __attribute__((noreturn));

void bail(int code) {
  printf("bailing with %d\n", code);
  exit(code);
}

int main(void) {
  printf("main\n");
  bail(7);
}

// LOWERING: #![feature(c_variadic)]

// REWRITES: #![feature(c_variadic)]

// SLATE-FILECHECK-ISYSTEM ~/Projects/slate/libc-shim/include /usr/lib/clang/22/include

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// SLATE-FILECHECK-END DEFAULT
