#include <stdio.h>

// @check-fn-begin
static unsigned char inc_byte_tied_via_Q(unsigned char x) {
  __asm__("incb %b0" : "+Q"(x));
  return x;
}
// @check-fn-end

// @check-fn-begin
static unsigned char add_byte_input_via_Q(unsigned char x, unsigned char y) {
  __asm__("addb %b1, %b0" : "+Q"(x) : "Q"(y));
  return x;
}
// @check-fn-end

int main(void) {
  printf("%d %d\n", inc_byte_tied_via_Q(0xFF), add_byte_input_via_Q(5, 9));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn inc_byte_tied_via_Q(mut x: u8) -> u8 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "incb {0:l}", inlateout(reg_abcd) (x as u8) as u32 => [[T_0]], options(pure,
// CHECK-NEXT:             nomem, nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         x = unsafe { std::mem::transmute::<u8, u8>([[T_0]] as u8) };
// CHECK-NEXT:     }
// CHECK-NEXT:     x
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn add_byte_input_via_Q(mut x: u8, mut y: u8) -> u8 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "addb {1:l}, {0:l}", inlateout(reg_abcd) (x as u8) as u32 => [[T_0]], in
// CHECK-NEXT:             (reg_abcd) (y as u8) as u32, options(pure, nomem, nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         x = unsafe { std::mem::transmute::<u8, u8>([[T_0]] as u8) };
// CHECK-NEXT:     }
// CHECK-NEXT:     x
// CHECK: {{^}}}
// SLATE-FILECHECK-END
