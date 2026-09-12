#include <stdio.h>

int main(void) {
  char  greeting[] = "hé";
  char  mutate[]   = "abc";
  char *p          = mutate;
  *p               = 'Z';
  printf("%s\n", greeting);
  printf("%c\n", mutate[0]);
  return 0;
}

// REWRITES: {{^}}}

// LOWERING: #![feature(c_variadic)]

// REWRITES: #![feature(c_variadic)]

// SLATE-FILECHECK-ISYSTEM ~/Projects/slate/libc-shim/include /usr/lib/clang/22/include

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// SLATE-FILECHECK-END DEFAULT
