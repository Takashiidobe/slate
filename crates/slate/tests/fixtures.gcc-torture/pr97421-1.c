/* PR rtl-optimization/97421 */
/* { dg-additional-options "-fmodulo-sched" } */

int a, b, d, e;
int *volatile c = &a;

__attribute__((noinline)) void f(void) {
  for (int g = 2; g >= 0; g--) {
    d = 0;
    for (b = 0; b <= 2; b++)
      ;
    e = *c;
  }
}

int main(void) {
  f();
  if (b != 3)
    __builtin_abort();
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR rtl-optimization/97421
// CHECK-NEXT: //!
// CHECK-NEXT: //! { dg-additional-options "-fmodulo-sched" }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut a: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut b: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut d: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut e: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut c: *mut i32 = unsafe { std::ptr::addr_of_mut!(a) };
// CHECK-EMPTY:
// CHECK-NEXT: fn f() {
// CHECK-NEXT:     let mut g: i32 = 2 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while g >= (0 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             d = 0 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             b = 0 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while (unsafe { b }) <= (2 as i32) {
// CHECK-NEXT:             {}
// CHECK-NEXT:             let mut [[V_0:__v[0-9]+]]: i32 = unsafe { b };
// CHECK-NEXT:             let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 b = [[V_1]];
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             e = unsafe { *unsafe { std::ptr::read_volatile(std::ptr::addr_of!(c)) } };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = g;
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] - (1 as i32);
// CHECK-NEXT:         g = [[V_3]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     f();
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
