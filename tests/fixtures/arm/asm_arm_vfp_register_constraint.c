#include <stdio.h>

static float copy_via_t(float value) {
  float result;
  // @lowering-begin
  // @rewrite-begin
  __asm__ volatile("vmov.f32 %0, %1" : "=t"(result) : "t"(value));
  // @rewrite-end
  // @lowering-end
  return result;
}

static double copy_via_w(double value) {
  double result;
  // @lowering-begin
  // @rewrite-begin
  __asm__ volatile("fcpyd %0, %1" : "=w"(result) : "w"(value));
  // @rewrite-end
  // @lowering-end
  return result;
}

static double copy_via_x(double value) {
  double result;
  // @lowering-begin
  // @rewrite-begin
  __asm__ volatile("fcpyd %0, %1" : "=x"(result) : "x"(value));
  // @rewrite-end
  // @lowering-end
  return result;
}

int main(void) {
  printf("%.1f %.1f %.1f\n", copy_via_t(1.5f), copy_via_w(2.5), copy_via_x(3.5));
  return 0;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-ARMV7-GNU-DAG: let {{__v[0-9]+}}: f32;
// LOWERING-ARMV7-GNU-DAG: unsafe {
// LOWERING-ARMV7-GNU-DAG:     core::arch::asm!("vmov.f32 {0}, {1}", lateout(sreg) {{__v[0-9]+}}, in(sreg) {{arg[0-9]+}});
// LOWERING-ARMV7-GNU-DAG: }
// LOWERING-ARMV7-GNU-DAG: let {{__v[0-9]+}}: f64;
// LOWERING-ARMV7-GNU-DAG: unsafe {
// LOWERING-ARMV7-GNU-DAG:     core::arch::asm!("fcpyd {0}, {1}", lateout(dreg) {{__v[0-9]+}}, in(dreg) {{arg[0-9]+}});
// LOWERING-ARMV7-GNU-DAG: }
// LOWERING-ARMV7-GNU-DAG: let {{__v[0-9]+}}: f64;
// LOWERING-ARMV7-GNU-DAG: unsafe {
// LOWERING-ARMV7-GNU-DAG:     core::arch::asm!("fcpyd {0}, {1}", lateout(dreg_low8) {{__v[0-9]+}}, in(dreg_low8) {{arg[0-9]+}});
// LOWERING-ARMV7-GNU-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-ARMV7-GNU-DAG: let {{__v[0-9]+}}: f32;
// REWRITES-ARMV7-GNU-DAG: unsafe {
// REWRITES-ARMV7-GNU-DAG:     core::arch::asm!("vmov.f32 {0}, {1}", lateout(sreg) {{__v[0-9]+}}, in(sreg) {{arg[0-9]+}});
// REWRITES-ARMV7-GNU-DAG: }
// REWRITES-ARMV7-GNU-DAG: let {{__v[0-9]+}}: f64;
// REWRITES-ARMV7-GNU-DAG: unsafe {
// REWRITES-ARMV7-GNU-DAG:     core::arch::asm!("fcpyd {0}, {1}", lateout(dreg) {{__v[0-9]+}}, in(dreg) {{arg[0-9]+}});
// REWRITES-ARMV7-GNU-DAG: }
// REWRITES-ARMV7-GNU-DAG: let {{__v[0-9]+}}: f64;
// REWRITES-ARMV7-GNU-DAG: unsafe {
// REWRITES-ARMV7-GNU-DAG:     core::arch::asm!("fcpyd {0}, {1}", lateout(dreg_low8) {{__v[0-9]+}}, in(dreg_low8) {{arg[0-9]+}});
// REWRITES-ARMV7-GNU-DAG: }
// SLATE-FILECHECK-END rewrites
