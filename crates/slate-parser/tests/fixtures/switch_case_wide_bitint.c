#include <stdio.h>

typedef _BitInt(256) big;

int classify(int seed) {
  big v = 170141183460469231731687303715884105727wb + (big)seed;
  switch (v) {
  case 170141183460469231731687303715884105727wb:
  case 170141183460469231731687303715884105728wb:
  case 170141183460469231731687303715884105729wb:
    return 1;
  case 170141183460469231731687303715884105736wb:
    return 2;
  case -170141183460469231731687303715884105727wb:
    return 3;
  default:
    return 0;
  }
}

int main(void) {
  printf("%d %d %d %d\n", classify(0), classify(2), classify(9), classify(5));
  return 0;
}



// SLATE-FILECHECK-ISYSTEM ~/Projects/slate/libc-shim/include /usr/lib/clang/22/include

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// SLATE-FILECHECK-END DEFAULT
