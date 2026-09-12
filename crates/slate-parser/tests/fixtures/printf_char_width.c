#include <stdio.h>

int main(void) {
  printf("%3c|%-3c|%c\n", 'a', 'b', 'c');
  return 0;
}


// SLATE-FILECHECK-ISYSTEM ~/Projects/slate/libc-shim/include /usr/lib/clang/22/include

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// SLATE-FILECHECK-END DEFAULT
