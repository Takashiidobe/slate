#include "shared.h"

int add_ignore_b(int a, int b) {
  (void)b;
  return a + 1;
}

int call_handler(struct Callback *cb, int a, int b) {
  return cb->handler(a, b);
}
