#include "shared.h"

int add_ignore_b(int a, int b) {
  (void)b;
  return a + 1;
}

void set_handler(struct Parser *p, callback_fn h) { p->handler = h; }
// REWRITES-LABEL: {{^}}pub extern "C" fn add_ignore_b(a: i32, b: i32) -> i32 {
// REWRITES: {{^}}}
