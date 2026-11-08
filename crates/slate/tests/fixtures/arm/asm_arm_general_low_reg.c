#include <stdio.h>

// @check-fn-begin
static int inc_via_r(int x) {
  __asm__("add %0, %0, #1" : "+r"(x));
  return x;
}
// @check-fn-end

// @check-fn-begin
static int inc_via_l(int x) {
  __asm__("add %0, %0, #1" : "+l"(x));
  return x;
}
// @check-fn-end

int main(void) {
  printf("%d %d\n", inc_via_r(41), inc_via_l(41));
  return 0;
}

// SLATE-FILECHECK-BEGIN armv7-unknown-linux-gnueabihf
// CHECK-LABEL: {{^}}fn inc_via_r(mut x: i32) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "add {0}, {0}, #1", inlateout(reg) x as u32 => [[T_0]], options(pure, nomem,
// CHECK-NEXT:             nostack, preserves_flags)
// CHECK-NEXT:         );
// CHECK-NEXT:         x = unsafe { std::mem::transmute::<u32, i32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     x
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn inc_via_l(mut x: i32) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "add {0}, {0}, #1", inlateout(reg) x as u32 => [[T_0]], options(pure, nomem,
// CHECK-NEXT:             nostack, preserves_flags)
// CHECK-NEXT:         );
// CHECK-NEXT:         x = unsafe { std::mem::transmute::<u32, i32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     x
// CHECK: {{^}}}
// SLATE-FILECHECK-END
