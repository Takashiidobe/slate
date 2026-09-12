#include <stdio.h>

int main(void) {
  char buf[64];
  sprintf(buf, "%d-%d", 3, 4);
  printf("value: %s\n", buf);
  return 0;
}

// LOWERING: #![feature(c_variadic)]

// REWRITES: #![feature(c_variadic)]

// SLATE-FILECHECK-ISYSTEM ~/Projects/slate/libc-shim/include /usr/lib/clang/22/include

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// SLATE-FILECHECK-END DEFAULT
