/* PR tree-optimization/59387 */

int  a, *d, **e = &d, f;
char c;
struct S {
  int f1;
} b;

int main() {
  for (a = -19; a; a++) {
    for (b.f1 = 0; b.f1 < 24; b.f1++)
      c--;
    *e = &f;
    if (!d)
      return 0;
  }
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/59387
// CHECK-NEXT: static mut a: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut d: *mut i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut e: *mut *mut i32 = unsafe { std::ptr::addr_of_mut!(d) };
// CHECK-EMPTY:
// CHECK-NEXT: static mut f: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut c: i8 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct S {
// CHECK-NEXT:     f1: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut b: S = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         a = -(19 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while (unsafe { a }) != (0 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             b.f1 = 0 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while (unsafe { b.f1 }) < (24 as i32) {
// CHECK-NEXT:             let mut [[V_0:__v[0-9]+]]: i8 = unsafe { c };
// CHECK-NEXT:             let mut [[V_1:__v[0-9]+]]: i8 = (([[V_0]] as i32) - (1 as i32)) as i8;
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 c = [[V_1]];
// CHECK-NEXT:             }
// CHECK-NEXT:             let mut [[V_2:__v[0-9]+]]: i32 = unsafe { b.f1 };
// CHECK-NEXT:             let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 b.f1 = [[V_3]];
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { e } = unsafe { std::ptr::addr_of_mut!(f) };
// CHECK-NEXT:         }
// CHECK-NEXT:         if !((unsafe { d }) != std::ptr::null_mut::<i32>()) {
// CHECK-NEXT:             return 0 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: i32 = unsafe { a };
// CHECK-NEXT:         let mut [[V_5:__v[0-9]+]]: i32 = [[V_4]] + (1 as i32);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             a = [[V_5]];
// CHECK-NEXT:         }
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
