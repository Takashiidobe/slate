#include "shared.h"
#include <stdio.h>

void init_flags(struct Flags *flags, void *owner, int count);

int main(void) {
  int          sentinel = 0;
  struct Flags flags;
  init_flags(&flags, &sentinel, 42);
  printf("%d %u %u %d\n", flags.count, flags.ready, flags.mode,
         flags.owner == &sentinel);
  return 0;
}
