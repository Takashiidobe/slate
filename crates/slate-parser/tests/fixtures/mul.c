#include <stdio.h>

static int imul(int a, int b) { return a * b; }

int main(void) {
  printf("%d\n", imul(-4, 7));
  unsigned int u = 100000u;
  printf("%u\n", u * u);
  return 0;
}



// SLATE-FILECHECK-ISYSTEM ~/Projects/slate/libc-shim/include /usr/lib/clang/22/include

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// SLATE-FILECHECK-END DEFAULT
