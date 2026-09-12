#include <stdio.h>

int main(void) {
  int      d = 5;
  unsigned h = 0xAB;
  printf("{%d} %% \"quoted\" back\\slash %s|%c|%x\n", d, "hi", 'X', h);
  printf("}}%%{{%d}}\n", d);
  printf("%%%%%d%%%%\n", d);
  printf("{{}}%s{{}}\n", "mid");
  return 0;
}




// SLATE-FILECHECK-ISYSTEM ~/Projects/slate/libc-shim/include /usr/lib/clang/22/include

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// SLATE-FILECHECK-END DEFAULT
