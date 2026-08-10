#include "shared.h"

int use_it(struct Parser *p) {
  set_handler(p, add_ignore_b);
  return p->handler(5, 99);
}
