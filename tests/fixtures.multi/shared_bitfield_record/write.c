#include "shared.h"

void init_flags(struct Flags *flags, void *owner, int count) {
  flags->owner = owner;
  flags->count = count;
  flags->ready = 1;
  flags->mode = 5;
}
