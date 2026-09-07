#include <stdio.h>

static double add_via_x(double a, double b) {
  double result;
  // @lowering-begin
  // @rewrite-begin
  __asm__("addsd %2, %0" : "=x"(result) : "0"(a), "x"(b));
  // @rewrite-end
  // @lowering-end
  return result;
}

static int roundtrip_via_x(int a) {
  int mid, result;
  // @lowering-begin
  // @rewrite-begin
  __asm__("movd %1, %0" : "=x"(mid) : "r"(a));
  __asm__("movd %1, %0" : "=r"(result) : "x"(mid));
  // @rewrite-end
  // @lowering-end
  return result;
}

int main(void) {
  printf("%.2f %d\n", add_via_x(1.5, 2.25), roundtrip_via_x(42));
  return 0;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: let {{__v[0-9]+}}: f64;
// LOWERING-DAG: unsafe {
// LOWERING-DAG:     core::arch::asm!("addsd {1}, {0}", inlateout(xmm_reg) {{arg[0-9]+}} => {{__v[0-9]+}}, in(xmm_reg) {{arg[0-9]+}}, options(att_syntax));
// LOWERING-DAG: }
// LOWERING-DAG: let {{__v[0-9]+}}: i32;
// LOWERING-DAG: unsafe {
// LOWERING-DAG:     core::arch::asm!("movd {1:e}, {0}", lateout(xmm_reg) {{__v[0-9]+}}, in(reg) {{arg[0-9]+}}, options(att_syntax));
// LOWERING-DAG: }
// LOWERING-DAG: let {{__v[0-9]+}}: i32;
// LOWERING-DAG: unsafe {
// LOWERING-DAG:     core::arch::asm!("movd {1}, {0:e}", lateout(reg) {{__v[0-9]+}}, in(xmm_reg) {{__v[0-9]+}}, options(att_syntax));
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: let {{__v[0-9]+}}: f64;
// REWRITES-DAG: unsafe {
// REWRITES-DAG:     core::arch::asm!("addsd {1}, {0}", inlateout(xmm_reg) {{arg[0-9]+}} => {{__v[0-9]+}}, in(xmm_reg) {{arg[0-9]+}}, options(att_syntax));
// REWRITES-DAG: }
// REWRITES-DAG: let {{__v[0-9]+}}: i32;
// REWRITES-DAG: unsafe {
// REWRITES-DAG:     core::arch::asm!("movd {1:e}, {0}", lateout(xmm_reg) {{__v[0-9]+}}, in(reg) {{arg[0-9]+}}, options(att_syntax));
// REWRITES-DAG: }
// REWRITES-DAG: let {{__v[0-9]+}}: i32;
// REWRITES-DAG: unsafe {
// REWRITES-DAG:     core::arch::asm!("movd {1}, {0:e}", lateout(reg) {{__v[0-9]+}}, in(xmm_reg) {{__v[0-9]+}}, options(att_syntax));
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
