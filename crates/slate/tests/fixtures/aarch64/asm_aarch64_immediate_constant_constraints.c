#include <stdio.h>

// @check-fn-begin
static int add_via_i(int value) {
  __asm__("add %w0, %w0, %1" : "+r"(value) : "I"(100));
  return value;
}
// @check-fn-end

// @check-fn-begin
static int and_via_k(int value) {
  __asm__("and %w0, %w0, %1" : "+r"(value) : "K"(0xff));
  return value;
}
// @check-fn-end

// @check-fn-begin
static long and_via_l(long value) {
  __asm__("and %0, %0, %1" : "+r"(value) : "L"(0xffL));
  return value;
}
// @check-fn-end

int main(void) {
  printf("%d %d %ld\n", add_via_i(5), and_via_k(0x1ff), and_via_l(0x1ffL));
  return 0;
}

// SLATE-FILECHECK-BEGIN aarch64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn add_via_i(mut value: i32) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "add {0:w}, {0:w}, {1}", inlateout(reg) value as u32 => [[T_0]], const 100 as
// CHECK-NEXT:             i32, options(pure, nomem, nostack, preserves_flags)
// CHECK-NEXT:         );
// CHECK-NEXT:         value = unsafe { std::mem::transmute::<u32, i32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     value
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn and_via_k(mut value: i32) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "and {0:w}, {0:w}, {1}", inlateout(reg) value as u32 => [[T_0]], const 255 as
// CHECK-NEXT:             i32, options(pure, nomem, nostack, preserves_flags)
// CHECK-NEXT:         );
// CHECK-NEXT:         value = unsafe { std::mem::transmute::<u32, i32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     value
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn and_via_l(mut value: i64) -> i64 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u64;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "and {0:x}, {0:x}, {1}", inlateout(reg) value as u64 => [[T_0]], const 255 as
// CHECK-NEXT:             i64, options(pure, nomem, nostack, preserves_flags)
// CHECK-NEXT:         );
// CHECK-NEXT:         value = unsafe { std::mem::transmute::<u64, i64>([[T_0]] as u64) };
// CHECK-NEXT:     }
// CHECK-NEXT:     value
// CHECK: {{^}}}
// SLATE-FILECHECK-END
