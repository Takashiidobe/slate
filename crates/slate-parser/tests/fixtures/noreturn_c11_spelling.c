#include <stdio.h>
#include <stdlib.h>

_Noreturn void die(int code) {
  printf("dying with %d\n", code);
  exit(code);
}

int main(void) {
  if (0) {
    die(1);
  }
  printf("main\n");
  return 0;
}

// LOWERING: #![feature(c_variadic)]

// REWRITES: #![feature(c_variadic)]

// SLATE-FILECHECK-ISYSTEM ~/Projects/slate/libc-shim/include /usr/lib/clang/22/include

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// SLATE-FILECHECK-END DEFAULT
