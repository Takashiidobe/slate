#include "shared.h"

int add_ignore_b(int a, int b) {
  (void)b;
  return a + 1;
}

void set_handler(struct Parser *p, callback_fn h) { p->handler = h; }
