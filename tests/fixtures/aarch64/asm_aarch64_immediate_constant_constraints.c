#include <stdio.h>

static int add_via_i(int value) {
  // @lowering-begin
  // @rewrite-begin
  __asm__("add %w0, %w0, %1" : "+r"(value) : "I"(100));
  // @rewrite-end
  // @lowering-end
  return value;
}

static int and_via_k(int value) {
  // @lowering-begin
  // @rewrite-begin
  __asm__("and %w0, %w0, %1" : "+r"(value) : "K"(0xff));
  // @rewrite-end
  // @lowering-end
  return value;
}

static long and_via_l(long value) {
  // @lowering-begin
  // @rewrite-begin
  __asm__("and %0, %0, %1" : "+r"(value) : "L"(0xffL));
  // @rewrite-end
  // @lowering-end
  return value;
}

int main(void) {
  printf("%d %d %ld\n", add_via_i(5), and_via_k(0x1ff), and_via_l(0x1ffL));
  return 0;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-AARCH64-GNU-DAG: let {{__v[0-9]+}}: i32 = 100;
// LOWERING-AARCH64-GNU-DAG: let {{__v[0-9]+}}: i32;
// LOWERING-AARCH64-GNU-DAG: unsafe {
// LOWERING-AARCH64-GNU-DAG:     core::arch::asm!("add {0:w}, {0:w}, {1}", inlateout(reg) {{arg[0-9]+}} => {{__v[0-9]+}}, const 100);
// LOWERING-AARCH64-GNU-DAG: }
// LOWERING-AARCH64-GNU-DAG: let {{__v[0-9]+}}: i32 = 255;
// LOWERING-AARCH64-GNU-DAG: let {{__v[0-9]+}}: i32;
// LOWERING-AARCH64-GNU-DAG: unsafe {
// LOWERING-AARCH64-GNU-DAG:     core::arch::asm!("and {0:w}, {0:w}, {1}", inlateout(reg) {{arg[0-9]+}} => {{__v[0-9]+}}, const 255);
// LOWERING-AARCH64-GNU-DAG: }
// LOWERING-AARCH64-GNU-DAG: let {{__v[0-9]+}}: i64 = 255;
// LOWERING-AARCH64-GNU-DAG: let {{__v[0-9]+}}: i64;
// LOWERING-AARCH64-GNU-DAG: unsafe {
// LOWERING-AARCH64-GNU-DAG:     core::arch::asm!("and {0:x}, {0:x}, {1}", inlateout(reg) {{arg[0-9]+}} => {{__v[0-9]+}}, const 255);
// LOWERING-AARCH64-GNU-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-AARCH64-GNU-DAG: let {{__v[0-9]+}}: i32;
// REWRITES-AARCH64-GNU-DAG: unsafe {
// REWRITES-AARCH64-GNU-DAG:     core::arch::asm!("add {0:w}, {0:w}, {1}", inlateout(reg) {{arg[0-9]+}} => {{__v[0-9]+}}, const 100);
// REWRITES-AARCH64-GNU-DAG: }
// REWRITES-AARCH64-GNU-DAG: let {{__v[0-9]+}}: i32;
// REWRITES-AARCH64-GNU-DAG: unsafe {
// REWRITES-AARCH64-GNU-DAG:     core::arch::asm!("and {0:w}, {0:w}, {1}", inlateout(reg) {{arg[0-9]+}} => {{__v[0-9]+}}, const 255);
// REWRITES-AARCH64-GNU-DAG: }
// REWRITES-AARCH64-GNU-DAG: let {{__v[0-9]+}}: i64;
// REWRITES-AARCH64-GNU-DAG: unsafe {
// REWRITES-AARCH64-GNU-DAG:     core::arch::asm!("and {0:x}, {0:x}, {1}", inlateout(reg) {{arg[0-9]+}} => {{__v[0-9]+}}, const 255);
// REWRITES-AARCH64-GNU-DAG: }
// SLATE-FILECHECK-END rewrites
