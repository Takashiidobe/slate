#include "shared.h"

int use_it(struct Parser *p) {
  set_handler(p, add_ignore_b);
  return p->handler(5, 99);
}
// REWRITES-DAG: use crate::handler::add_ignore_b;
// REWRITES-NOT: unsafe extern "C" {
