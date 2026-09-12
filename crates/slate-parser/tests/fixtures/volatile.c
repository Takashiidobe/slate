// LOWERING: {{^}}}

#include <stdio.h>

static int volatile_local(int value) {
  volatile int slot = value;
  slot              = slot + 3;
  return slot;
}

int main(void) {
  printf("%d\n", volatile_local(4));
  return 0;
}

// LOWERING: #![feature(c_variadic)]

// REWRITES: #![feature(c_variadic)]

// SLATE-FILECHECK-ISYSTEM ~/Projects/slate/libc-shim/include /usr/lib/clang/22/include

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// SLATE-FILECHECK-END DEFAULT
