#include <stdio.h>

/* noreturn via an infinite loop: correct C, but Slate's lowering does not yet
 * prove this diverges (the generated Rust loop keeps a conditional break
 * guard), so it must fall back to a conservative, non-`!` return type
 * instead of emitting Rust that rustc would reject. */
__attribute__((noreturn)) static void spin(void) {
  for (;;) {
  }
}

void (*unused_spin)(void) = spin;

int main(void) {
  printf("main\n");
  return 0;
}



// SLATE-FILECHECK-ISYSTEM ~/Projects/slate/libc-shim/include /usr/lib/clang/22/include

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// SLATE-FILECHECK-END DEFAULT
