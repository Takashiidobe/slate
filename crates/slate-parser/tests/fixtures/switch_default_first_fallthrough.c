#include <stdio.h>

int score(int x) {
  int out = 0;
  switch (x) {
  default:
    out += 1;
  case 5:
    out += 10;
  case 6:
    out += 20;
    break;
  case 7:
    out += 40;
  }
  return out;
}

int main(void) {
  printf("%d %d %d %d\n", score(5), score(6), score(7), score(9));
  return 0;
}



// SLATE-FILECHECK-ISYSTEM ~/Projects/slate/libc-shim/include /usr/lib/clang/22/include

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// SLATE-FILECHECK-END DEFAULT
