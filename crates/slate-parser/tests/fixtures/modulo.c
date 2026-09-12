#include <stdio.h>

static int imod(int a, int b) { return a % b; }

int main(void) {
  printf("%d\n", imod(-7, 3));
  printf("%d\n", imod(7, -3));
  unsigned int u = 100u;
  printf("%u\n", u % 7u);
  return 0;
}



// SLATE-FILECHECK-ISYSTEM ~/Projects/slate/libc-shim/include /usr/lib/clang/22/include

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// SLATE-FILECHECK-END DEFAULT
