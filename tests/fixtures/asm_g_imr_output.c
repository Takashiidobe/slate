#include <stdio.h>

static int store_via_g(void) {
  int x;
  // @lowering-begin
  // @rewrite-begin
  __asm__("movl $42, %0" : "=g"(x));
  // @rewrite-end
  // @lowering-end
  return x;
}

static int store_via_imr(void) {
  int x;
  // @lowering-begin
  // @rewrite-begin
  __asm__("movl $7, %0" : "=imr"(x));
  // @rewrite-end
  // @lowering-end
  return x;
}

static int add_tied_g(int y) {
  int x = 1;
  // @lowering-begin
  // @rewrite-begin
  __asm__("addl %1, %0" : "+g"(x) : "r"(y));
  // @rewrite-end
  // @lowering-end
  return x;
}

int main(void) {
  printf("%d %d %d\n", store_via_g(), store_via_imr(), add_tied_g(4));
  return 0;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: unsafe {
// LOWERING-DAG:     core::arch::asm!("movl $42, {0:e}", lateout(reg) x, options(att_syntax));
// LOWERING-DAG: }
// LOWERING-DAG: unsafe {
// LOWERING-DAG:     core::arch::asm!("movl $7, {0:e}", lateout(reg) x, options(att_syntax));
// LOWERING-DAG: }
// LOWERING-DAG: let {{__v[0-9]+}}: i32 = x;
// LOWERING-DAG: unsafe {
// LOWERING-DAG:     core::arch::asm!("addl {1:e}, {0:e}", inlateout(reg) {{__v[0-9]+}} => x, in(reg) {{arg[0-9]+}}, options(att_syntax));
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: unsafe {
// REWRITES-DAG:     core::arch::asm!("movl $42, {0:e}", lateout(reg) x, options(att_syntax));
// REWRITES-DAG: }
// REWRITES-DAG: unsafe {
// REWRITES-DAG:     core::arch::asm!("movl $7, {0:e}", lateout(reg) x, options(att_syntax));
// REWRITES-DAG: }
// REWRITES-DAG: let {{__v[0-9]+}}: i32 = x;
// REWRITES-DAG: unsafe {
// REWRITES-DAG:     core::arch::asm!("addl {1:e}, {0:e}", inlateout(reg) {{__v[0-9]+}} => x, in(reg) {{arg[0-9]+}}, options(att_syntax));
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
