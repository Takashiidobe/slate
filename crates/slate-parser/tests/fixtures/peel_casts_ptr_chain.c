#include <string.h>

int main(void) {
  char  buf[8];
  char *p = buf;
  memset((void *)p, 0, 8);
  return p[0];
}



// SLATE-FILECHECK-ISYSTEM ~/Projects/slate/libc-shim/include /usr/lib/clang/22/include

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// SLATE-FILECHECK-END DEFAULT
