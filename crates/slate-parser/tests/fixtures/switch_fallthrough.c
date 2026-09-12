#include <stdio.h>

int score(int x) {
  int out = 0;
  switch (x) {
  case 1:
    out += 10;
  case 2:
    out += 20;
    break;
  case 3:
  case 4:
    out += 40;
    break;
  default:
    out += 90;
  }
  return out;
}

int main(void) {
  printf("%d %d %d %d %d\n", score(1), score(2), score(3), score(4), score(8));
  return 0;
}



// SLATE-FILECHECK-ISYSTEM ~/Projects/slate/libc-shim/include /usr/lib/clang/22/include

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// SLATE-FILECHECK-END DEFAULT
