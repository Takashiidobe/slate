#include <stdio.h>

// @check-fn-begin
static int inc32_via_r(int x) {
  __asm__("add %0, %0, #1" : "+r"(x));
  return x;
}
// @check-fn-end

// @check-fn-begin
static long inc64_via_r(long x) {
  __asm__("add %0, %0, #1" : "+r"(x));
  return x;
}
// @check-fn-end

// @check-fn-begin
static long inc_w_then_x(long x) {
  __asm__("add %w0, %w0, #1\n\tadd %x0, %x0, #0" : "+r"(x));
  return x;
}
// @check-fn-end

int main(void) {
  printf("%d %ld %ld\n", inc32_via_r(41), inc64_via_r(41),
         inc_w_then_x(0xFFFFFFFF00000005L));
  return 0;
}

// SLATE-FILECHECK-BEGIN aarch64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn inc32_via_r(mut x: i32) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "add {0:x}, {0:x}, #1", inlateout(reg) x as u32 => [[T_0]], options(pure, nomem,
// CHECK-NEXT:             nostack, preserves_flags)
// CHECK-NEXT:         );
// CHECK-NEXT:         x = unsafe { std::mem::transmute::<u32, i32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     x
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn inc64_via_r(mut x: i64) -> i64 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u64;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "add {0:x}, {0:x}, #1", inlateout(reg) x as u64 => [[T_0]], options(pure, nomem,
// CHECK-NEXT:             nostack, preserves_flags)
// CHECK-NEXT:         );
// CHECK-NEXT:         x = unsafe { std::mem::transmute::<u64, i64>([[T_0]] as u64) };
// CHECK-NEXT:     }
// CHECK-NEXT:     x
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn inc_w_then_x(mut x: i64) -> i64 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u64;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "add {0:w}, {0:w}, #1\n\tadd {0:x}, {0:x}, #0", inlateout(reg) x as u64 =>
// CHECK-NEXT:             [[T_0]], options(pure, nomem, nostack, preserves_flags)
// CHECK-NEXT:         );
// CHECK-NEXT:         x = unsafe { std::mem::transmute::<u64, i64>([[T_0]] as u64) };
// CHECK-NEXT:     }
// CHECK-NEXT:     x
// CHECK: {{^}}}
// SLATE-FILECHECK-END
