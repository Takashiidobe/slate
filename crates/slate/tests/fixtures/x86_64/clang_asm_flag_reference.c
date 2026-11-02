#include <stdio.h>

static int same_as_place(int place, int other) {
  int flag = place;
  __asm__("cmpl %1, %0" : "+@ccz"(flag) : "r"(other));
  return flag;
}

static int wide_place(long place, long other) {
  long flag = place;
  __asm__("cmpl %k1, %0" : "+@ccg"(flag) : "r"(other));
  return flag != 0;
}

static int scratch_zero(void) {
  int flag;
  __asm__("xorl %0, %0" : "=@ccz"(flag));
  return flag;
}

int main(void) {
  printf("%d %d\n", same_as_place(4, 4), same_as_place(4, 5));
  printf("%d %d\n", wide_place(0x100000005L, 1L), wide_place(1L, 0x100000005L));
  printf("%d\n", scratch_zero());
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: fn same_as_place(mut flag: i32, mut other: i32) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u8;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "cmpl {2:e}, {1:e}\nsetz {0}", out(reg_byte) [[T_0]], in (reg) flag as u32, in
// CHECK-NEXT:             (reg) other as u32, options(pure, nomem, nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         flag = unsafe { std::mem::transmute::<u32, i32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     flag
// CHECK-NEXT: }
// CHECK-NEXT: fn wide_place(mut flag: i64, mut other: i64) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u8;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "cmpl {2:e}, {1:e}\nsetg {0}", out(reg_byte) [[T_0]], in (reg) (flag as u64) as
// CHECK-NEXT:             u32, in (reg) other as u64, options(pure, nomem, nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         flag = unsafe { std::mem::transmute::<u64, i64>([[T_0]] as u64) };
// CHECK-NEXT:     }
// CHECK-NEXT:     (flag != 0) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn scratch_zero() -> i32 {
// CHECK-NEXT:     let mut flag: i32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u8;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "xorl {1:e}, {1:e}\nsetz {0}", lateout(reg_byte) [[T_0]], out(reg) _,
// CHECK-NEXT:             options(pure, nomem, nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         flag = unsafe { std::mem::transmute::<u32, i32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     flag
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!("{} {}", same_as_place(4i32, 4i32), same_as_place(4i32, 5i32));
// CHECK-NEXT:     println!("{} {}", wide_place(4294967301i64, 1i64), wide_place(1i64, 4294967301i64));
// CHECK-NEXT:     println!("{}", scratch_zero());
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
