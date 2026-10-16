/* { dg-require-effective-target int32plus } */
#include <limits.h>

extern void exit(int);
extern void abort();

void test01(unsigned int a, unsigned int b) {
  if (a < 5)
    abort();
  if (b < 5)
    abort();
  if (a - b != 5)
    abort();
}

void test02(unsigned int a, unsigned int b) {
  if (a >= 12)
    if (b > 15)
      if (a - b < UINT_MAX - 15U)
        abort();
}

int main(int argc, char *argv[]) {
  unsigned x = 0x80000000;
  test01(x + 5, x);
  test02(14, 16);
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-require-effective-target int32plus }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test01(mut a: u32, mut b: u32) {
// CHECK-NEXT:     if a < ((5 as i32) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if b < ((5 as i32) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if a.wrapping_sub(b) != ((5 as i32) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test02(mut a: u32, mut b: u32) {
// CHECK-NEXT:     if a >= ((12 as i32) as u32) {
// CHECK-NEXT:         if b > ((15 as i32) as u32) {
// CHECK-NEXT:             if a.wrapping_sub(b)
// CHECK-NEXT:                 < ((2147483647 as i32) as u32)
// CHECK-NEXT:                     .wrapping_mul(2 as u32)
// CHECK-NEXT:                     .wrapping_add(1 as u32)
// CHECK-NEXT:                     .wrapping_sub(15 as u32)
// CHECK-NEXT:             {
// CHECK-NEXT:                 unsafe { abort() };
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main(mut argc: i32, mut argv: *mut *mut i8) -> i32 {
// CHECK-NEXT:     let mut x: u32 = 2147483648 as u32;
// CHECK-NEXT:     test01(x.wrapping_add((5 as i32) as u32), x);
// CHECK-NEXT:     test02((14 as i32) as u32, (16 as i32) as u32);
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main(argc as _, argv as _) }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
