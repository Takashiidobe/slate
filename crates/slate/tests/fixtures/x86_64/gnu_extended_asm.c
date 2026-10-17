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

// SLATE-FILECHECK-BEGIN
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn gnu_extended_asm(mut input: i32) -> i32 {
// CHECK-NEXT:     let mut output: i32 = 0;
// CHECK-NEXT:     let mut tied: i32 = 0;
// CHECK-NEXT:     let mut early: i32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "leal 3({1:e}), {0:e}", lateout(reg) [[T_0]], in (reg) input as u32,
// CHECK-NEXT:             options(nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         output = unsafe { std::mem::transmute::<u32, i32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_1:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "addl $2, {0:e}", inlateout(reg) output as u32 => [[T_1]], options(nostack,
// CHECK-NEXT:             att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         output = unsafe { std::mem::transmute::<u32, i32>([[T_1]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_2:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "imull {1:e}, {0:e}", inlateout(reg) output as u32 => [[T_2]], in (reg) (2 as
// CHECK-NEXT:             i32) as u32, options(pure, nomem, nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         tied = unsafe { std::mem::transmute::<u32, i32>([[T_2]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_3:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "movl {1:e}, {0:e}\n\taddl {2:e}, {0:e}", out(reg) [[T_3]], in (reg) tied as
// CHECK-NEXT:             u32, in (reg) (1 as i32) as u32, options(pure, nomem, nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         early = unsafe { std::mem::transmute::<u32, i32>([[T_3]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_4:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "addl ${1}, {0:e}", inlateout(reg) early as u32 => [[T_4]], const 4 as i32,
// CHECK-NEXT:             options(pure, nomem, nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         early = unsafe { std::mem::transmute::<u32, i32>([[T_4]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     early
// CHECK-NEXT: }
// CHECK-NEXT: fn gnu_numeric_operands(mut left: i32, mut right: i32) -> i32 {
// CHECK-NEXT:     let mut result: i32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "subl {1:e}, {0:e}", inlateout(reg) left as u32 => [[T_0]], in (reg) right as
// CHECK-NEXT:             u32, options(pure, nomem, nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         result = unsafe { std::mem::transmute::<u32, i32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     result
// CHECK-NEXT: }
// CHECK-NEXT: fn gnu_multiple_outputs() -> i32 {
// CHECK-NEXT:     let mut left: i32 = 0;
// CHECK-NEXT:     let mut right: i32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u32;
// CHECK-NEXT:         let [[T_1]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "movl $3, {0:e}\n\tmovl $4, {1:e}", lateout(reg) [[T_0]], lateout(reg) [[T_1]],
// CHECK-NEXT:             options(pure, nomem, nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         left = unsafe { std::mem::transmute::<u32, i32>([[T_0]] as u32) };
// CHECK-NEXT:         right = unsafe { std::mem::transmute::<u32, i32>([[T_1]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     left * 10 + right
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {}", gnu_extended_asm(7 as i32), gnu_numeric_operands(19 as i32, 4 as
// CHECK-NEXT:         i32), gnu_multiple_outputs()
// CHECK-NEXT:     );
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
