#include <stdio.h>

int main(void) {
  int rc = remove("slate_perror_intervening_missing.tmp");
  fflush(stdout);
  if (rc < 0) {
    perror("remove failed");
    return 1;
  }
  return 0;
}



// SLATE-FILECHECK-ISYSTEM ~/Projects/slate/libc-shim/include /usr/lib/clang/22/include

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// SLATE-FILECHECK-END DEFAULT
