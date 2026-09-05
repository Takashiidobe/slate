// { dg-do run { target x86_64-*-* } }
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

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32;
// COMMON-LOWERING-DAG: unsafe {
// COMMON-LOWERING-DAG:     core::arch::asm!("add {0:e}, 7", inlateout(reg) {{arg[0-9]+}} => {{__v[0-9]+}});
// COMMON-LOWERING-DAG: }
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32;
// COMMON-LOWERING-DAG: unsafe {
// COMMON-LOWERING-DAG:     core::arch::asm!("subl $2, {0:e}", inlateout(reg) {{arg[0-9]+}} => {{__v[0-9]+}}, options(att_syntax));
// COMMON-LOWERING-DAG: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES-DAG: let {{__v[0-9]+}}: i32;
// COMMON-REWRITES-DAG: unsafe {
// COMMON-REWRITES-DAG:     core::arch::asm!("add {0:e}, 7", inlateout(reg) {{arg[0-9]+}} => {{__v[0-9]+}});
// COMMON-REWRITES-DAG: }
// COMMON-REWRITES-DAG: let {{__v[0-9]+}}: i32;
// COMMON-REWRITES-DAG: unsafe {
// COMMON-REWRITES-DAG:     core::arch::asm!("subl $2, {0:e}", inlateout(reg) {{arg[0-9]+}} => {{__v[0-9]+}}, options(att_syntax));
// COMMON-REWRITES-DAG: }
// SLATE-FILECHECK-END common-rewrites
