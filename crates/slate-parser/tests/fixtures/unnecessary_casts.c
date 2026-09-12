#include <stdio.h>

int main(void) {
  unsigned char a = 200;
  unsigned char b = 100;
  unsigned char c = 0;
  for (int i = 0; i < 1; i++) {
    c = a + b;
  }
  printf("%d\n", c);
  return 0;
}



// SLATE-FILECHECK-ISYSTEM ~/Projects/slate/libc-shim/include /usr/lib/clang/22/include

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// SLATE-FILECHECK-END DEFAULT
