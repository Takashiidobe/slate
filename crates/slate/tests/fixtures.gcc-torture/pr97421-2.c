/* PR rtl-optimization/97421 */
/* { dg-additional-options "-fmodulo-sched -fno-dce -fno-strict-aliasing" } */

static int a, b, c;
int       *d = &c;
int      **e = &d;
int     ***f = &e;
int        main() {
  int h;
  for (a = 2; a; a--)
    for (h = 0; h <= 2; h++)
      for (b = 0; b <= 2; b++)
        ***f = 6;

  if (b != 3)
    __builtin_abort();
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR rtl-optimization/97421
// CHECK-NEXT: //!
// CHECK-NEXT: //! { dg-additional-options "-fmodulo-sched -fno-dce -fno-strict-aliasing" }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut a: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut b: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut c: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut d: *mut i32 = unsafe { std::ptr::addr_of_mut!(c) };
// CHECK-EMPTY:
// CHECK-NEXT: static mut e: *mut *mut i32 = unsafe { std::ptr::addr_of_mut!(d) };
// CHECK-EMPTY:
// CHECK-NEXT: static mut f: *mut *mut *mut i32 = unsafe { std::ptr::addr_of_mut!(e) };
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut h: i32 = 0 as i32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         a = 2 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while (unsafe { a }) != (0 as i32) {
// CHECK-NEXT:         h = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while h <= (2 as i32) {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 b = 0 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: while (unsafe { b }) <= (2 as i32) {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     *unsafe { *unsafe { *unsafe { f } } } = 6 as i32;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 let mut [[V_0:__v[0-9]+]]: i32 = unsafe { b };
// CHECK-NEXT:                 let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     b = [[V_1]];
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             let mut [[V_2:__v[0-9]+]]: i32 = h;
// CHECK-NEXT:             let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:             h = [[V_3]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: i32 = unsafe { a };
// CHECK-NEXT:         let mut [[V_5:__v[0-9]+]]: i32 = [[V_4]] - (1 as i32);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             a = [[V_5]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { b }) != (3 as i32) {
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
