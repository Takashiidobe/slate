/* PR tree-optimization/61682 */

int         a, b;
static int *c = &b;

int main() {
  int *d = &a;
  for (a = 0; a < 12; a++)
    *c |= *d / 9;

  if (b != 1)
    __builtin_abort();

  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/61682
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut a: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut b: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut c: *mut i32 = unsafe { std::ptr::addr_of_mut!(b) };
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut d: *mut i32 = unsafe { std::ptr::addr_of_mut!(a) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         a = 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while (unsafe { a }) < (12 as i32) {
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: *mut i32 = unsafe { c };
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = unsafe { *[[V_0]] };
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = [[V_1]] | (unsafe { *d }) / (9 as i32);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *[[V_0]] = [[V_2]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = unsafe { a };
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: i32 = [[V_3]] + (1 as i32);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             a = [[V_4]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { b }) != (1 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
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
