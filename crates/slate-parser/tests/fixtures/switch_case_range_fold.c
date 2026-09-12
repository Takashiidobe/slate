#include <stdio.h>

int classify(int c) {
  switch (c) {
  case 48:
  case 49:
  case 50:
  case 51:
  case 52:
  case 53:
  case 54:
  case 55:
  case 56:
  case 57:
    return 1;
  case 'a':
  case 'b':
  case 'c':
    return 2;
  case 100:
  case 200:
    return 3;
  case -3:
  case -2:
  case -1:
  case 7:
    return 4;
  default:
    return 0;
  }
}

int main(void) {
  printf("%d %d %d %d %d %d %d\n", classify('5'), classify('b'), classify(100),
         classify(200), classify(-2), classify(7), classify(9));
  return 0;
}



// SLATE-FILECHECK-ISYSTEM ~/Projects/slate/libc-shim/include /usr/lib/clang/22/include

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// SLATE-FILECHECK-END DEFAULT
