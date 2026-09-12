#include <stdio.h>

int main(void) {
  FILE *fp = fopen("slate_perror_fopen_missing.tmp", "r");
  if (fp == NULL) {
    perror("open failed");
    return 1;
  }
  fclose(fp);
  return 0;
}

// LOWERING: #![allow(

// REWRITES: #![allow(

// SLATE-FILECHECK-ISYSTEM ~/Projects/slate/libc-shim/include /usr/lib/clang/22/include

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// SLATE-FILECHECK-END DEFAULT
