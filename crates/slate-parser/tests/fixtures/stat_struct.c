#include <stdio.h>
#include <sys/stat.h>

int main(void) {
  struct stat info   = {0};
  int         result = stat("/dev/null", &info);
  printf("%d %lld\n", result, (long long)info.st_size);
  return 0;
}



// SLATE-FILECHECK-ISYSTEM ~/Projects/slate/libc-shim/include /usr/lib/clang/22/include

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// SLATE-FILECHECK-END DEFAULT
