#include <stdio.h>

int main(void) {
  int x = 0;
  x     = 42;

  int y = 0;
  int z = getchar();
  y     = z + 1;

  printf("%d %d %d\n", x, y, z);
  return 0;
}



// SLATE-FILECHECK-ISYSTEM ~/Projects/slate/libc-shim/include /usr/lib/clang/22/include

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// SLATE-FILECHECK-END DEFAULT
