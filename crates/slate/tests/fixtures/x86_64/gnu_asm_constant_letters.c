#include <stdio.h>

// @check-fn-begin
static int add_const_n(int value) {
  __asm__("addl $%c[amount], %[value]"
          : [value] "+r"(value)
          : [amount] "n"(4)
          : "cc");
  return value;
}
// @check-fn-end

// @check-fn-begin
static int shift_const_i_letter(int value) {
  __asm__("sall $%c[amount], %[value]"
          : [value] "+r"(value)
          : [amount] "I"(3)
          : "cc");
  return value;
}
// @check-fn-end

int main(void) {
  printf("%d %d\n", add_const_n(10), shift_const_i_letter(2));
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}fn add_const_n(mut value: i32) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "addl ${1}, {0:e}", inlateout(reg) value as u32 => [[T_0]], const 4 as i32,
// CHECK-NEXT:             options(pure, nomem, nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         value = unsafe { std::mem::transmute::<u32, i32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     value
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn shift_const_i_letter(mut value: i32) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "sall ${1}, {0:e}", inlateout(reg) value as u32 => [[T_0]], const 3 as i32,
// CHECK-NEXT:             options(pure, nomem, nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         value = unsafe { std::mem::transmute::<u32, i32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     value
// CHECK: {{^}}}
// SLATE-FILECHECK-END
