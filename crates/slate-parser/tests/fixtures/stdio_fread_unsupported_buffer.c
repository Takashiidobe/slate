#include <stdio.h>
#include <stdlib.h>

int main(void) {
  remove("slate_stdio_fread_unsupported_buffer.tmp");
  FILE *f = fopen("slate_stdio_fread_unsupported_buffer.tmp", "w");
  if (!f) {
    puts("open-fail");
    return 0;
  }
  fputs("heap-owned", f);
  fclose(f);

  FILE *g = fopen("slate_stdio_fread_unsupported_buffer.tmp", "r");
  if (!g) {
    puts("reopen-fail");
    return 0;
  }
  char *buf = malloc(16);
  buf[0]    = 0;
  size_t n  = fread(buf, 1, 10, g);
  buf[n]    = 0;
  printf("%zu %s\n", n, buf);
  free(buf);
  fclose(g);
  remove("slate_stdio_fread_unsupported_buffer.tmp");
  return 0;
}

// LOWERING: #![feature(c_variadic)]

// REWRITES: #![feature(c_variadic)]

// SLATE-FILECHECK-ISYSTEM ~/Projects/slate/libc-shim/include /usr/lib/clang/22/include

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// SLATE-FILECHECK-END DEFAULT
