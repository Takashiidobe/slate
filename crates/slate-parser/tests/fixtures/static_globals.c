#include <stdio.h>

static int counter = 2;

static int bump_counter(void) {
  counter = counter + 1;
  return counter;
}

int main(void) {
  printf("%d\n", bump_counter());
  printf("%d\n", bump_counter());
  return 0;
}




// SLATE-FILECHECK-ISYSTEM ~/Projects/slate/libc-shim/include /usr/lib/clang/22/include

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// SLATE-FILECHECK-END DEFAULT
