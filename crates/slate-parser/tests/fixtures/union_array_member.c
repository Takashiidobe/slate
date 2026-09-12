#include <stdio.h>

union Word {
  int           value;
  unsigned char bytes[4];
};

int main(void) {
  union Word word = {0};
  word.bytes[0]   = 65;
  word.bytes[1]   = 66;
  printf("%d %d\n", word.bytes[0], word.bytes[1]);
  return 0;
}

// LOWERING: #![feature(c_variadic)]

// REWRITES: #![feature(c_variadic)]

// SLATE-FILECHECK-ISYSTEM ~/Projects/slate/libc-shim/include /usr/lib/clang/22/include

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// SLATE-FILECHECK-END DEFAULT
