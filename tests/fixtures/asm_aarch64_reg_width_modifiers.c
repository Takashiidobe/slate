#include <stdio.h>

static int inc32_via_r(int x) {
  // @lowering-begin
  // @rewrite-begin
  __asm__("add %0, %0, #1" : "+r"(x));
  // @rewrite-end
  // @lowering-end
  return x;
}

static long inc64_via_r(long x) {
  // @lowering-begin
  // @rewrite-begin
  __asm__("add %0, %0, #1" : "+r"(x));
  // @rewrite-end
  // @lowering-end
  return x;
}

static long inc_w_then_x(long x) {
  // @lowering-begin
  // @rewrite-begin
  __asm__("add %w0, %w0, #1\n\tadd %x0, %x0, #0" : "+r"(x));
  // @rewrite-end
  // @lowering-end
  return x;
}

int main(void) {
  printf("%d %ld %ld\n", inc32_via_r(41), inc64_via_r(41),
         inc_w_then_x(0xFFFFFFFF00000005L));
  return 0;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-AARCH64-GNU-DAG: let {{__v[0-9]+}}: i32;
// LOWERING-AARCH64-GNU-DAG: unsafe {
// LOWERING-AARCH64-GNU-DAG:     core::arch::asm!("add {0:w}, {0:w}, #1", inlateout(reg) {{arg[0-9]+}} => {{__v[0-9]+}});
// LOWERING-AARCH64-GNU-DAG: }
// LOWERING-AARCH64-GNU-DAG: let {{__v[0-9]+}}: i64;
// LOWERING-AARCH64-GNU-DAG: unsafe {
// LOWERING-AARCH64-GNU-DAG:     core::arch::asm!("add {0:x}, {0:x}, #1", inlateout(reg) {{arg[0-9]+}} => {{__v[0-9]+}});
// LOWERING-AARCH64-GNU-DAG: }
// LOWERING-AARCH64-GNU-DAG: let {{__v[0-9]+}}: i64;
// LOWERING-AARCH64-GNU-DAG: unsafe {
// LOWERING-AARCH64-GNU-DAG:     core::arch::asm!("add {0:w}, {0:w}, #1\n\tadd {0:x}, {0:x}, #0", inlateout(reg) {{arg[0-9]+}} => {{__v[0-9]+}});
// LOWERING-AARCH64-GNU-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-AARCH64-GNU-DAG: let {{__v[0-9]+}}: i32;
// REWRITES-AARCH64-GNU-DAG: unsafe {
// REWRITES-AARCH64-GNU-DAG:     core::arch::asm!("add {0:w}, {0:w}, #1", inlateout(reg) {{arg[0-9]+}} => {{__v[0-9]+}});
// REWRITES-AARCH64-GNU-DAG: }
// REWRITES-AARCH64-GNU-DAG: let {{__v[0-9]+}}: i64;
// REWRITES-AARCH64-GNU-DAG: unsafe {
// REWRITES-AARCH64-GNU-DAG:     core::arch::asm!("add {0:x}, {0:x}, #1", inlateout(reg) {{arg[0-9]+}} => {{__v[0-9]+}});
// REWRITES-AARCH64-GNU-DAG: }
// REWRITES-AARCH64-GNU-DAG: let {{__v[0-9]+}}: i64;
// REWRITES-AARCH64-GNU-DAG: unsafe {
// REWRITES-AARCH64-GNU-DAG:     core::arch::asm!("add {0:w}, {0:w}, #1\n\tadd {0:x}, {0:x}, #0", inlateout(reg) {{arg[0-9]+}} => {{__v[0-9]+}});
// REWRITES-AARCH64-GNU-DAG: }
// SLATE-FILECHECK-END rewrites
