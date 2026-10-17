#include <stdio.h>

// @check-fn-begin
static int inc_high_byte_of_wide_operand(int x) {
  __asm__("incb %h0" : "+Q"(x));
  return x;
}
// @check-fn-end

int main(void) {
  printf("%d\n", inc_high_byte_of_wide_operand(0x1234));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn inc_high_byte_of_wide_operand(mut x: i32) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "incb {0:h}", inlateout(reg_abcd) x as u32 => [[T_0]], options(pure, nomem,
// CHECK-NEXT:             nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         x = unsafe { std::mem::transmute::<u32, i32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     x
// CHECK: {{^}}}
// SLATE-FILECHECK-END
