/* PR tree-optimization/120677 */
/* { dg-do run { target int32plus } } */

unsigned a;
int      b, e;

int foo(int d) {
  switch (d) {
  case 0:
  case 2:
    return 0;
  default:
    return 1;
  }
}

int main() {
  for (b = 8; b; b--)
    if (a & 1)
      a = a >> 1 ^ 20000000;
    else
      a >>= 1;
  e = foo(0);
  if (e || a)
    __builtin_abort();
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/120677
// CHECK-NEXT: //!
// CHECK-NEXT: //! { dg-do run { target int32plus } }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut a: u32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut b: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut e: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut d: i32) -> i32 {
// CHECK-NEXT:     match d {
// CHECK-NEXT:         0 | 2 => {
// CHECK-NEXT:             return 0 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         _ => {
// CHECK-NEXT:             return 1 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         b = 8 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while (unsafe { b }) != (0 as i32) {
// CHECK-NEXT:         if (unsafe { a }) & ((1 as i32) as u32) != (0 as u32) {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 a = (unsafe { a }) >> (1 as i32) ^ ((20000000 as i32) as u32);
// CHECK-NEXT:             }
// CHECK-NEXT:         } else {
// CHECK-NEXT:             let mut [[V_0:__v[0-9]+]]: u32 = unsafe { a };
// CHECK-NEXT:             let mut [[V_1:__v[0-9]+]]: u32 = [[V_0]] >> (1 as i32);
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 a = [[V_1]];
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = unsafe { b };
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] - (1 as i32);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             b = [[V_3]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         e = foo(0 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { e }) != (0 as i32) || (unsafe { a }) != (0 as u32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
