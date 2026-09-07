#include <stdio.h>

static int add_via_i(int value) {
  // @lowering-begin
  // @rewrite-begin
  __asm__("add %0, %0, %1" : "+r"(value) : "I"(1));
  // @rewrite-end
  // @lowering-end
  return value;
}

static int add_via_m(int value) {
  // @lowering-begin
  // @rewrite-begin
  __asm__("add %0, %0, %1" : "+r"(value) : "M"(32));
  // @rewrite-end
  // @lowering-end
  return value;
}

int main(void) {
  printf("%d %d\n", add_via_i(40), add_via_m(8));
  return 0;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-ARMV7-GNU-DAG: let {{__v[0-9]+}}: i32 = 1;
// LOWERING-ARMV7-GNU-DAG: let {{__v[0-9]+}}: i32;
// LOWERING-ARMV7-GNU-DAG: unsafe {
// LOWERING-ARMV7-GNU-DAG:     core::arch::asm!("add {0}, {0}, {1}", inlateout(reg) {{arg[0-9]+}} => {{__v[0-9]+}}, const 1);
// LOWERING-ARMV7-GNU-DAG: }
// LOWERING-ARMV7-GNU-DAG: let {{__v[0-9]+}}: i32 = 32;
// LOWERING-ARMV7-GNU-DAG: let {{__v[0-9]+}}: i32;
// LOWERING-ARMV7-GNU-DAG: unsafe {
// LOWERING-ARMV7-GNU-DAG:     core::arch::asm!("add {0}, {0}, {1}", inlateout(reg) {{arg[0-9]+}} => {{__v[0-9]+}}, const 32);
// LOWERING-ARMV7-GNU-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-ARMV7-GNU-DAG: let {{__v[0-9]+}}: i32;
// REWRITES-ARMV7-GNU-DAG: unsafe {
// REWRITES-ARMV7-GNU-DAG:     core::arch::asm!("add {0}, {0}, {1}", inlateout(reg) {{arg[0-9]+}} => {{__v[0-9]+}}, const 1);
// REWRITES-ARMV7-GNU-DAG: }
// REWRITES-ARMV7-GNU-DAG: let {{__v[0-9]+}}: i32;
// REWRITES-ARMV7-GNU-DAG: unsafe {
// REWRITES-ARMV7-GNU-DAG:     core::arch::asm!("add {0}, {0}, {1}", inlateout(reg) {{arg[0-9]+}} => {{__v[0-9]+}}, const 32);
// REWRITES-ARMV7-GNU-DAG: }
// SLATE-FILECHECK-END rewrites
