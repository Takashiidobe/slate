#include <stddef.h>
#include <stdio.h>

static int target(void) { return 42; }

int main(void) {
  int (*base)(void) = target;
  ptrdiff_t forward = (base + 3) - base;
  ptrdiff_t backward = (base - 2) - base;
  ptrdiff_t difference = base - (base + 3);
  int unchanged = base + 0 == base;
  printf("%td %td %td %d\n", forward, backward, difference, unchanged);
  return 0;
}
