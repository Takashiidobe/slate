// REWRITES-NOT: return _v
// REWRITES-DAG: std::ptr::read_volatile
// LOWERING-LABEL: {{^}}fn volatile_local(
// LOWERING-DAG: std::ptr::read_volatile
// LOWERING-DAG: std::ptr::write_volatile
// LOWERING: {{^}}}

#include <stdio.h>

static int volatile_local(int value) {
  volatile int slot = value;
  slot              = slot + 3;
  return slot;
}

int main(void) {
  printf("%d\n", volatile_local(4));
  return 0;
}
