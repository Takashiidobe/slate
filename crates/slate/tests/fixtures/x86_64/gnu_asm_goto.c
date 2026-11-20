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
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![feature(asm_goto_with_outputs)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: fn gnu_asm_goto(mut value: i32) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let mut __slate_asm_target: bool = false;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "testl {0:e}, {0:e}\n\tjz {1}",
// CHECK-NEXT:             in(reg) value as u32,
// CHECK-NEXT:             label {
// CHECK-NEXT:                 __slate_asm_target = true;
// CHECK-NEXT:             },
// CHECK-NEXT:             options(nostack, att_syntax),
// CHECK-NEXT:         );
// CHECK-NEXT:         if __slate_asm_target {
// CHECK-NEXT:             return 13;
// CHECK-NEXT:         } else {
// CHECK-NEXT:             return 11;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn gnu_asm_goto_output(value: i32) -> i32 {
// CHECK-NEXT:     let mut output: core::mem::MaybeUninit<i32> = core::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_0:__slate_slot_[0-9]+]]: *mut i32 = core::ptr::addr_of_mut!(output) as *mut i32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::ptr::write([[SLATE_SLOT_0]], value);
// CHECK-NEXT:         let mut __slate_asm_target: bool = false;
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "testl {0:e}, {0:e}\n\tjz {1}",
// CHECK-NEXT:             inlateout(reg) *[[SLATE_SLOT_0]] as u32 => [[T_0]],
// CHECK-NEXT:             label {
// CHECK-NEXT:                 __slate_asm_target = true;
// CHECK-NEXT:             },
// CHECK-NEXT:             options(nostack, att_syntax),
// CHECK-NEXT:         );
// CHECK-NEXT:         *[[SLATE_SLOT_0]] = unsafe { core::mem::transmute::<u32, i32>([[T_0]] as u32) };
// CHECK-NEXT:         if __slate_asm_target {
// CHECK-NEXT:             return *[[SLATE_SLOT_0]] + 2;
// CHECK-NEXT:         } else {
// CHECK-NEXT:             return *[[SLATE_SLOT_0]] + 1;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {}",
// CHECK-NEXT:         gnu_asm_goto(0i32),
// CHECK-NEXT:         gnu_asm_goto_output(5i32),
// CHECK-NEXT:         gnu_asm_goto_output(0i32)
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
