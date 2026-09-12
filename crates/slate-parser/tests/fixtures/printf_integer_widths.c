#include <stdio.h>

int main(void) {
  int          a = 42;
  int          b = 7;
  int          c = -42;
  unsigned int u = 9u;
  long         l = 123L;
  printf("%05d|%-4d|%+d|%5u|%+06ld\n", a, b, c, u, l);
  return 0;
}



// SLATE-FILECHECK-ISYSTEM ~/Projects/slate/libc-shim/include /usr/lib/clang/22/include

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// SLATE-FILECHECK-END DEFAULT
