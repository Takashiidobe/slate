#include <stdio.h>

static int gnu_extended_asm(int input) {
  int output;
  int tied;
  int early;

  __asm__ volatile("leal 3(%1), %0" : "=r"(output) : "r"(input) : "cc");
  __asm__ volatile("addl $2, %0" : "+r"(output) : : "cc");
  __asm__("imull %[right], %[result]"
          : [result] "=r"(tied)
          : "0"(output), [right] "r"(2)
          : "cc");
  __asm__("movl %[left], %[result]\n\taddl %[right], %[result]"
          : [result] "=&r"(early)
          : [left] "r"(tied), [right] "r"(1)
          : "cc");
  __asm__("addl $%c[amount], %[value]"
          : [value] "+r"(early)
          : [amount] "i"(4)
          : "cc");
  return early;
}

static int gnu_numeric_operands(int left, int right) {
  int result;
  __asm__("subl %2, %0" : "=r"(result) : "0"(left), "r"(right) : "cc");
  return result;
}

static int gnu_multiple_outputs(void) {
  int left;
  int right;
  __asm__("movl $3, %0\n\tmovl $4, %1" : "=r"(left), "=r"(right));
  return left * 10 + right;
}

int main(void) {
  printf("%d %d %d\n", gnu_extended_asm(7), gnu_numeric_operands(19, 4),
         gnu_multiple_outputs());
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: fn gnu_extended_asm(input: i32) -> i32 {
// CHECK-NEXT:     let mut output: i32 = 0;
// CHECK-NEXT:     let mut tied: i32 = 0;
// CHECK-NEXT:     let mut early: i32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "leal 3({1:e}), {0:e}",
// CHECK-NEXT:             lateout(reg) output,
// CHECK-NEXT:             in(reg) input as u32,
// CHECK-NEXT:             options(nostack, att_syntax),
// CHECK-NEXT:         );
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "addl $2, {0:e}",
// CHECK-NEXT:             inlateout(reg) output => output,
// CHECK-NEXT:             options(nostack, att_syntax),
// CHECK-NEXT:         );
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "imull {1:e}, {0:e}",
// CHECK-NEXT:             inlateout(reg) output as u32 => tied,
// CHECK-NEXT:             in(reg) 2i32 as u32,
// CHECK-NEXT:             options(pure, nomem, nostack, att_syntax),
// CHECK-NEXT:         );
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "movl {1:e}, {0:e}\n\taddl {2:e}, {0:e}",
// CHECK-NEXT:             out(reg) early,
// CHECK-NEXT:             in(reg) tied as u32,
// CHECK-NEXT:             in(reg) 1i32 as u32,
// CHECK-NEXT:             options(pure, nomem, nostack, att_syntax),
// CHECK-NEXT:         );
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "addl ${1}, {0:e}",
// CHECK-NEXT:             inlateout(reg) early => early,
// CHECK-NEXT:             const 4i32,
// CHECK-NEXT:             options(pure, nomem, nostack, att_syntax),
// CHECK-NEXT:         );
// CHECK-NEXT:     }
// CHECK-NEXT:     early
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn gnu_numeric_operands(left: i32, right: i32) -> i32 {
// CHECK-NEXT:     let mut result: i32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "subl {1:e}, {0:e}",
// CHECK-NEXT:             inlateout(reg) left as u32 => result,
// CHECK-NEXT:             in(reg) right as u32,
// CHECK-NEXT:             options(pure, nomem, nostack, att_syntax),
// CHECK-NEXT:         );
// CHECK-NEXT:     }
// CHECK-NEXT:     result
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn gnu_multiple_outputs() -> i32 {
// CHECK-NEXT:     let mut left: i32 = 0;
// CHECK-NEXT:     let mut right: i32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "movl $3, {0:e}\n\tmovl $4, {1:e}",
// CHECK-NEXT:             lateout(reg) left,
// CHECK-NEXT:             lateout(reg) right,
// CHECK-NEXT:             options(pure, nomem, nostack, att_syntax),
// CHECK-NEXT:         );
// CHECK-NEXT:     }
// CHECK-NEXT:     left * 10 + right
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {}",
// CHECK-NEXT:         gnu_extended_asm(7i32),
// CHECK-NEXT:         gnu_numeric_operands(19i32, 4i32),
// CHECK-NEXT:         gnu_multiple_outputs()
// CHECK-NEXT:     );
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut core::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut core::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
