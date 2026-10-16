#include <stdio.h>

// @check-fn-begin
static int inc_wide_via_Q(int x) {
  __asm__("incl %0" : "+Q"(x));
  return x;
}
// @check-fn-end

// @check-fn-begin
static unsigned char inc_byte_via_q(unsigned char x) {
  __asm__("incb %0" : "+q"(x));
  return x;
}
// @check-fn-end

int main(void) {
  printf("%d %d\n", inc_wide_via_Q(41), inc_byte_via_q(9));
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}fn inc_wide_via_Q(mut x: i32) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "incl {0:e}", inlateout(reg_abcd) x as u32 => [[T_0]], options(pure, nomem,
// CHECK-NEXT:             nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         x = unsafe { std::mem::transmute::<u32, i32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     x
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn inc_byte_via_q(mut x: u8) -> u8 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "incb {0:l}", inlateout(reg) (x as u8) as u32 => [[T_0]], options(pure, nomem,
// CHECK-NEXT:             nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         x = unsafe { std::mem::transmute::<u8, u8>([[T_0]] as u8) };
// CHECK-NEXT:     }
// CHECK-NEXT:     x
// CHECK: {{^}}}
// SLATE-FILECHECK-END
