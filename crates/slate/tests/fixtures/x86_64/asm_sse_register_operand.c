#include <stdio.h>

// @check-fn-begin
static double add_via_x(double a, double b) {
  double result;
  __asm__("addsd %2, %0" : "=x"(result) : "0"(a), "x"(b));
  return result;
}
// @check-fn-end

// @check-fn-begin
static int roundtrip_via_x(int a) {
  int mid, result;
  __asm__("movd %1, %0" : "=x"(mid) : "r"(a));
  __asm__("movd %1, %0" : "=r"(result) : "x"(mid));
  return result;
}
// @check-fn-end

int main(void) {
  printf("%.2f %d\n", add_via_x(1.5, 2.25), roundtrip_via_x(42));
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}fn add_via_x(mut a: f64, mut b: f64) -> f64 {
// CHECK-NEXT:     let mut result: f64 = 0 as f64;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u64;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "addsd {1}, {0}", inlateout(xmm_reg) unsafe { std::mem::transmute::< f64, u64
// CHECK-NEXT:             > (a) } => [[T_0]], in (xmm_reg) unsafe { std::mem::transmute::< f64, u64 > (b)
// CHECK-NEXT:             }, options(pure, nomem, nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         result = unsafe { std::mem::transmute::<u64, f64>([[T_0]] as u64) };
// CHECK-NEXT:     }
// CHECK-NEXT:     result
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn roundtrip_via_x(mut a: i32) -> i32 {
// CHECK-NEXT:     let mut mid: i32 = 0 as i32;
// CHECK-NEXT:     let mut result: i32 = 0 as i32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "movd {1:e}, {0}", lateout(xmm_reg) [[T_0]], in (reg) a as u32, options(pure,
// CHECK-NEXT:             nomem, nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         mid = unsafe { std::mem::transmute::<u32, i32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_1:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "movd {1}, {0:e}", lateout(reg) [[T_1]], in (xmm_reg) mid as u32, options(pure,
// CHECK-NEXT:             nomem, nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         result = unsafe { std::mem::transmute::<u32, i32>([[T_1]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     result
// CHECK: {{^}}}
// SLATE-FILECHECK-END
