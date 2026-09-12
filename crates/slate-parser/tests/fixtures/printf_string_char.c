#include <stdio.h>

int main(void) {
  printf("%s %c %c %d\n", "tag", 'A', 10, 7);
  printf("literal=%s", "tail");
  return 0;
}



// SLATE-FILECHECK-ISYSTEM ~/Projects/slate/libc-shim/include /usr/lib/clang/22/include

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// SLATE-FILECHECK-END DEFAULT
