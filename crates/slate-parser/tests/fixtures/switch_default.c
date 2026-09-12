#include <stdio.h>

int classify(int x) {
  int out = 0;
  switch (x) {
  case 1:
    out = 11;
    break;
  case 2:
    out = 22;
    break;
  default:
    out = 99;
    break;
  }
  return out;
}

int main(void) {
  printf("%d %d %d\n", classify(1), classify(2), classify(7));
  return 0;
}



// SLATE-FILECHECK-ISYSTEM ~/Projects/slate/libc-shim/include /usr/lib/clang/22/include

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// SLATE-FILECHECK-END DEFAULT
