#include <stdio.h>

static int equal_rw(int left, int right) {
  int flag = 7;
  __asm__("cmpl %2, %1" : "+@ccz"(flag) : "r"(left), "r"(right));
  return flag;
}

static unsigned char carry_rw(unsigned a, unsigned b) {
  unsigned char flag = 9;
  __asm__("addl %2, %1" : "+@ccc"(flag), "+r"(a) : "r"(b));
  return flag * 10 + (a == 0);
}

int main(void) {
  printf("%d %d %d\n", equal_rw(3, 3), equal_rw(3, 4), carry_rw(0xffffffffu, 1));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: fn equal_rw(mut left: i32, mut right: i32) -> i32 {
// CHECK-NEXT:     let mut flag: i32 = 7;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u8;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "cmpl {2:e}, {1:e}\nsetz {0}", out(reg_byte) [[T_0]], in (reg) left as u32, in
// CHECK-NEXT:             (reg) right as u32, options(pure, nomem, nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         flag = unsafe { std::mem::transmute::<u32, i32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     flag
// CHECK-NEXT: }
// CHECK-NEXT: fn carry_rw(mut a: u32, mut b: u32) -> u8 {
// CHECK-NEXT:     let mut flag: u8 = 9;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u8;
// CHECK-NEXT:         let [[T_1:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "addl {2:e}, {1:e}\nsetc {0}", out(reg_byte) [[T_0]], inlateout(reg) a as u32 =>
// CHECK-NEXT:             [[T_1]], in (reg) b as u32, options(pure, nomem, nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         flag = unsafe { std::mem::transmute::<u8, u8>([[T_0]] as u8) };
// CHECK-NEXT:         a = unsafe { std::mem::transmute::<u32, u32>([[T_1]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     ((flag as i32) * 10 + ((a == 0) as i32)) as u8
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {}", equal_rw(3i32, 3i32), equal_rw(3i32, 4i32), carry_rw(4294967295u32,
// CHECK-NEXT:         1u32) as i32
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
