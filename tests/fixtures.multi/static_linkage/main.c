#include <stdio.h>

static int base = 10;
static int local(int x) { return x + base; }

int compute(int x);

int main(void) {
  printf("%d %d\n", local(5), compute(3));
  return 0;
}
// LOWERING-DAG: use crate::other::compute;
// LOWERING-DAG: {{^}}static mut base: i32 = 10;
// LOWERING-NOT: pub static mut base
// LOWERING-LABEL: {{^}}fn local(
// LOWERING: {{^}}}
