#include <stdio.h>

static int intel_add(int value) {
  // @lowering-begin
  // @rewrite-begin
  __asm__ volatile(".intel_syntax noprefix\n\t"
                   "add %V0, 7\n\t"
                   ".att_syntax prefix"
                   : "+r"(value));
  // @rewrite-end
  // @lowering-end
  return value;
}

static int att_subtract(int value) {
  // @lowering-begin
  // @rewrite-begin
  __asm__ volatile("subl $2, %0" : "+r"(value));
  // @rewrite-end
  // @lowering-end
  return value;
}

int main(void) {
  printf("%d %d\n", intel_add(5), att_subtract(5));
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = value;
// LOWERING-DAG: let {{_v[0-9]+}}: i32;
// LOWERING-DAG: unsafe {
// LOWERING-DAG: core::arch::asm!("add {0:e}, 7", inlateout(reg) {{_v[0-9]+}} => {{_v[0-9]+}});
// LOWERING-DAG: }
// LOWERING-DAG: value = {{_v[0-9]+}};
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = value;
// LOWERING-DAG: let {{_v[0-9]+}}: i32;
// LOWERING-DAG: unsafe {
// LOWERING-DAG: core::arch::asm!("subl $2, {0:e}", inlateout(reg) {{_v[0-9]+}} => {{_v[0-9]+}}, options(att_syntax));
// LOWERING-DAG: }
// LOWERING-DAG: value = {{_v[0-9]+}};
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = value;
// REWRITES-DAG: let {{_v[0-9]+}}: i32;
// REWRITES-DAG: unsafe {
// REWRITES-DAG: core::arch::asm!("add {0:e}, 7", inlateout(reg) {{_v[0-9]+}} => {{_v[0-9]+}});
// REWRITES-DAG: }
// REWRITES-DAG: value = {{_v[0-9]+}};
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = value;
// REWRITES-DAG: let {{_v[0-9]+}}: i32;
// REWRITES-DAG: unsafe {
// REWRITES-DAG: core::arch::asm!("subl $2, {0:e}", inlateout(reg) {{_v[0-9]+}} => {{_v[0-9]+}}, options(att_syntax));
// REWRITES-DAG: }
// REWRITES-DAG: value = {{_v[0-9]+}};
// SLATE-FILECHECK-END rewrites
