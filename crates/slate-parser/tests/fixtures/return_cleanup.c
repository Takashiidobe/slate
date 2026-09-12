#include <stdio.h>

int choose(int value) {
  if (value < 0)
    return -1;
  return value + 2;
}

int main(void) {
  printf("%d %d\n", choose(-4), choose(5));
  return 0;
}



// SLATE-FILECHECK-ISYSTEM ~/Projects/slate/libc-shim/include /usr/lib/clang/22/include

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// SLATE-FILECHECK-END DEFAULT
