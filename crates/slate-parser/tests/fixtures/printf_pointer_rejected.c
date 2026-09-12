#include <stdint.h>
#include <stdio.h>

int main(void) {
  void *stable = (void *)(uintptr_t)0x1234;
  printf("%20p\n", stable);
  return 0;
}


// LOWERING: #![feature(c_variadic)]

// REWRITES: #![feature(c_variadic)]

// SLATE-FILECHECK-ISYSTEM ~/Projects/slate/libc-shim/include /usr/lib/clang/22/include

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// SLATE-FILECHECK-END DEFAULT
