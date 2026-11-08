#include <stdio.h>

// @check-fn-begin
static float copy_via_t(float value) {
  float result;
  __asm__ volatile("vmov.f32 %0, %1" : "=t"(result) : "t"(value));
  return result;
}
// @check-fn-end

// @check-fn-begin
static double copy_via_w(double value) {
  double result;
  __asm__ volatile("fcpyd %0, %1" : "=w"(result) : "w"(value));
  return result;
}
// @check-fn-end

// @check-fn-begin
static double copy_via_x(double value) {
  double result;
  __asm__ volatile("fcpyd %0, %1" : "=x"(result) : "x"(value));
  return result;
}
// @check-fn-end

int main(void) {
  printf("%.1f %.1f %.1f\n", copy_via_t(1.5f), copy_via_w(2.5), copy_via_x(3.5));
  return 0;
}

// SLATE-FILECHECK-BEGIN armv7-unknown-linux-gnueabihf
// CHECK-LABEL: {{^}}fn copy_via_t(mut value: f32) -> f32 {
// CHECK-NEXT:     let mut result: f32 = 0 as f32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "vmov.f32 {0}, {1}", lateout(sreg) [[T_0]], in (sreg) unsafe {
// CHECK-NEXT:             std::mem::transmute::< f32, u32 > (value) }, options(nostack,
// CHECK-NEXT:             preserves_flags)
// CHECK-NEXT:         );
// CHECK-NEXT:         result = unsafe { std::mem::transmute::<u32, f32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     result
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn copy_via_w(mut value: f64) -> f64 {
// CHECK-NEXT:     let mut result: f64 = 0 as f64;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u64;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "fcpyd {0}, {1}", lateout(dreg_low16) [[T_0]], in (dreg_low16) unsafe {
// CHECK-NEXT:             std::mem::transmute::< f64, u64 > (value) }, options(nostack,
// CHECK-NEXT:             preserves_flags)
// CHECK-NEXT:         );
// CHECK-NEXT:         result = unsafe { std::mem::transmute::<u64, f64>([[T_0]] as u64) };
// CHECK-NEXT:     }
// CHECK-NEXT:     result
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn copy_via_x(mut value: f64) -> f64 {
// CHECK-NEXT:     let mut result: f64 = 0 as f64;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u64;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "fcpyd {0}, {1}", lateout(dreg_low8) [[T_0]], in (dreg_low8) unsafe {
// CHECK-NEXT:             std::mem::transmute::< f64, u64 > (value) }, options(nostack,
// CHECK-NEXT:             preserves_flags)
// CHECK-NEXT:         );
// CHECK-NEXT:         result = unsafe { std::mem::transmute::<u64, f64>([[T_0]] as u64) };
// CHECK-NEXT:     }
// CHECK-NEXT:     result
// CHECK: {{^}}}
// SLATE-FILECHECK-END
