#include <stdio.h>

// @check-fn-begin
static int add_via_i(int value) {
  __asm__("add %0, %0, %1" : "+r"(value) : "I"(1));
  return value;
}
// @check-fn-end

// @check-fn-begin
static int add_via_m(int value) {
  __asm__("add %0, %0, %1" : "+r"(value) : "M"(32));
  return value;
}
// @check-fn-end

int main(void) {
  printf("%d %d\n", add_via_i(40), add_via_m(8));
  return 0;
}

// SLATE-FILECHECK-BEGIN armv7-unknown-linux-gnueabihf
// CHECK-LABEL: {{^}}fn add_via_i(mut value: i32) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "add {0}, {0}, #{1}", inlateout(reg) value as u32 => [[T_0]], const 1 as i32,
// CHECK-NEXT:             options(pure, nomem, nostack, preserves_flags)
// CHECK-NEXT:         );
// CHECK-NEXT:         value = unsafe { std::mem::transmute::<u32, i32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     value
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn add_via_m(mut value: i32) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "add {0}, {0}, #{1}", inlateout(reg) value as u32 => [[T_0]], const 32 as i32,
// CHECK-NEXT:             options(pure, nomem, nostack, preserves_flags)
// CHECK-NEXT:         );
// CHECK-NEXT:         value = unsafe { std::mem::transmute::<u32, i32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     value
// CHECK: {{^}}}
// SLATE-FILECHECK-END
