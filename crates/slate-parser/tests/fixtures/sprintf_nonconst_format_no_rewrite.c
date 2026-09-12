#include <stdio.h>

int main(void) {
  char        buf[64];
  const char *fmt = "%d-%d";
  sprintf(buf, fmt, 3, 4);
  puts(buf);
  return 0;
}



// SLATE-FILECHECK-ISYSTEM ~/Projects/slate/libc-shim/include /usr/lib/clang/22/include

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// SLATE-FILECHECK-END DEFAULT
