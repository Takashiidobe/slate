#include <stdio.h>

static void source_probe(void) {
  printf("%s|%s|%d|%d\n", __builtin_FUNCTION(), __builtin_FILE(),
         __builtin_LINE(), __builtin_COLUMN());
}

int main(void) {
  source_probe();
  return 0;
}

// LOWERING: #![feature(c_variadic)]

// REWRITES: #![feature(c_variadic)]
