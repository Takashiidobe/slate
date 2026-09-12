#include <setjmp.h>
#include <stdio.h>

static jmp_buf frame;

int main(void) {
  if (setjmp(frame) == 0) {
    longjmp(frame, 42);
  }
  printf("returned\n");
  return 0;
}



// SLATE-FILECHECK-ISYSTEM ~/Projects/slate/libc-shim/include /usr/lib/clang/22/include

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// SLATE-FILECHECK-END DEFAULT
