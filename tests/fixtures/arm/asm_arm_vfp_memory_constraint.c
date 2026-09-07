#include <stdio.h>

static int load_via_uv(const int *address) {
  int value;
  // @lowering-begin
  // @rewrite-begin
  __asm__("ldr %0, %1" : "=r"(value) : "Uv"(*address));
  // @rewrite-end
  // @lowering-end
  return value;
}

int main(void) {
  int value = 42;
  printf("%d\n", load_via_uv(&value));
  return 0;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-ARMV7-GNU-DAG: let {{__v[0-9]+}}: i32;
// LOWERING-ARMV7-GNU-DAG: unsafe {
// LOWERING-ARMV7-GNU-DAG:     core::arch::asm!("ldr {0}, [{1}]", lateout(reg) {{__v[0-9]+}}, in(reg) {{arg[0-9]+}});
// LOWERING-ARMV7-GNU-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-ARMV7-GNU-DAG: let {{__v[0-9]+}}: i32;
// REWRITES-ARMV7-GNU-DAG: unsafe {
// REWRITES-ARMV7-GNU-DAG:     core::arch::asm!("ldr {0}, [{1}]", lateout(reg) {{__v[0-9]+}}, in(reg) {{arg[0-9]+}} as *const i32);
// REWRITES-ARMV7-GNU-DAG: }
// SLATE-FILECHECK-END rewrites
