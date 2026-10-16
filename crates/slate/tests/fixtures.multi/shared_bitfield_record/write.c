#include "shared.h"

// @check-fn-begin
void init_flags(struct Flags *flags, void *owner, int count) {
  flags->owner = owner;
  flags->count = count;
  flags->ready = 1;
  flags->mode  = 5;
}
// @check-fn-end
