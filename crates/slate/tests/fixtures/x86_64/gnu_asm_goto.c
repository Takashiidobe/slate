#include <stdio.h>

static int gnu_asm_goto(int value) {
  __asm__ goto("testl %[value], %[value]\n\tjz %l[zero]"
               :
               : [value] "r"(value)
               : "cc"
               : zero);
  return 11;
zero:
  return 13;
}

static int gnu_asm_goto_output(int value) {
  int output = value;
  __asm__ goto("testl %[output], %[output]\n\tjz %l[zero]"
               : [output] "+r"(output)
               :
               : "cc"
               : zero);
  return output + 1;
zero:
  return output + 2;
}

int main(void) {
  printf("%d %d %d\n", gnu_asm_goto(0), gnu_asm_goto_output(5),
         gnu_asm_goto_output(0));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![feature(asm_goto_with_outputs)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn gnu_asm_goto(mut value: i32) -> i32 {
// CHECK-NEXT:     let mut __slate_state: usize = 2usize;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         loop {
// CHECK-NEXT:             match __slate_state {
// CHECK-NEXT:                 0 => {
// CHECK-NEXT:                     return 13;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 1 => {
// CHECK-NEXT:                     return 11;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 2 => {
// CHECK-NEXT:                     __slate_state = 1usize;
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         core::arch::asm!(
// CHECK-NEXT:                             "testl {0:e}, {0:e}\n\tjz {1}", in (reg) value as u32, label
// CHECK-NEXT:                             { __slate_state = 0usize; }, options(nostack, att_syntax)
// CHECK-NEXT:                         );
// CHECK-NEXT:                     }
// CHECK-NEXT:                     continue;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 _ => {
// CHECK-NEXT:                     unreachable!();
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-NEXT: fn gnu_asm_goto_output(value: i32) -> i32 {
// CHECK-NEXT:     let mut output: std::mem::MaybeUninit<i32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_0:__slate_slot_[0-9]+]]: *mut i32 = std::ptr::addr_of_mut!(output) as *mut i32;
// CHECK-NEXT:     let mut __slate_state: usize = 2usize;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         loop {
// CHECK-NEXT:             match __slate_state {
// CHECK-NEXT:                 0 => {
// CHECK-NEXT:                     return *[[SLATE_SLOT_0]] + 2;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 1 => {
// CHECK-NEXT:                     return *[[SLATE_SLOT_0]] + 1;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 2 => {
// CHECK-NEXT:                     std::ptr::write([[SLATE_SLOT_0]], value);
// CHECK-NEXT:                     __slate_state = 1usize;
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         let [[T_0:__t[0-9]+]]: u32;
// CHECK-NEXT:                         core::arch::asm!(
// CHECK-NEXT:                             "testl {0:e}, {0:e}\n\tjz {1}", inlateout(reg) *
// CHECK-NEXT:                             [[SLATE_SLOT_0]] as u32 => [[T_0]], label { __slate_state =
// CHECK-NEXT:                             0usize; }, options(nostack, att_syntax)
// CHECK-NEXT:                         );
// CHECK-NEXT:                         *[[SLATE_SLOT_0]] = unsafe {
// CHECK-NEXT:                             std::mem::transmute::<u32, i32>([[T_0]] as u32)
// CHECK-NEXT:                         };
// CHECK-NEXT:                     }
// CHECK-NEXT:                     continue;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 _ => {
// CHECK-NEXT:                     unreachable!();
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {}", gnu_asm_goto(0 as i32), gnu_asm_goto_output(5 as i32),
// CHECK-NEXT:         gnu_asm_goto_output(0 as i32)
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
