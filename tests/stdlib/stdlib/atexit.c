#include <stdio.h>
#include <stdlib.h>

static void first(void) { puts("first"); }
static void second(void) { puts("second"); }

int main(void) {
  atexit(first);
  atexit(second);
  puts("main");
  return 0;
}
