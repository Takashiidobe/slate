#include <stdio.h>

static int classify(int value) {
  __asm__ goto("cmp %0, #0\n\t"
               "blt %l[negative]\n\t"
               "beq %l[zero]"
               :
               : "r"(value)
               : "cc"
               : negative, zero);
  return 1;
negative:
  return -1;
zero:
  return 0;
}

static int count_down(int value) {
  int steps = 0;
again:
  steps++;
  __asm__ goto("subs %0, %0, #1\n\t"
               "bne %l[again]"
               : "+r"(value)
               :
               : "cc"
               : again);
  return steps * 100 + value;
}

int main(void) {
  printf("%d %d %d\n", classify(-5), classify(0), classify(7));
  printf("%d\n", count_down(3));
  return 0;
}

// SLATE-FILECHECK-BEGIN armv7-unknown-linux-gnueabihf
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![feature(asm_goto_with_outputs)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: fn classify(mut value: i32) -> i32 {
// CHECK-NEXT:     let mut __slate_state: usize = 3usize;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         loop {
// CHECK-NEXT:             match __slate_state {
// CHECK-NEXT:                 0 => {
// CHECK-NEXT:                     return 0;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 1 => {
// CHECK-NEXT:                     return -1;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 2 => {
// CHECK-NEXT:                     return 1;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 3 => {
// CHECK-NEXT:                     __slate_state = 2usize;
// CHECK-NEXT:                     core::arch::asm!(
// CHECK-NEXT:                         "cmp {0}, #0\n\tblt {1}\n\tbeq {2}", in (reg) value as u32, label
// CHECK-NEXT:                         { __slate_state = 1usize; }, label { __slate_state = 0usize; },
// CHECK-NEXT:                         options(nostack)
// CHECK-NEXT:                     );
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
// CHECK-NEXT: fn count_down(mut value: i32) -> i32 {
// CHECK-NEXT:     let mut steps: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut __slate_state: usize = 2usize;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         loop {
// CHECK-NEXT:             match __slate_state {
// CHECK-NEXT:                 0 => {
// CHECK-NEXT:                     return steps * 100 + value;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 1 => {
// CHECK-NEXT:                     steps += 1;
// CHECK-NEXT:                     __slate_state = 0usize;
// CHECK-NEXT:                     let [[T_0:__t[0-9]+]]: u32;
// CHECK-NEXT:                     core::arch::asm!(
// CHECK-NEXT:                         "subs {0}, {0}, #1\n\tbne {1}", inlateout(reg) value as u32 =>
// CHECK-NEXT:                         [[T_0]], label { __slate_state = 1usize; }, options(nostack)
// CHECK-NEXT:                     );
// CHECK-NEXT:                     value = unsafe { std::mem::transmute::<u32, i32>([[T_0]] as u32) };
// CHECK-NEXT:                     continue;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 2 => {
// CHECK-NEXT:                     steps = 0;
// CHECK-NEXT:                     __slate_state = 1usize;
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
// CHECK-NEXT:     println!("{} {} {}", classify(- 5i32), classify(0i32), classify(7i32));
// CHECK-NEXT:     println!("{}", count_down(3i32));
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
