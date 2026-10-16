/* PR rtl-optimization/57281 */

int                a = 1, b, d, *e = &d;
long long          c, *g = &c;
volatile long long f;

int foo(int h) {
  int j = *g = b;
  return h == 0 ? j : 0;
}

int main() {
  int h = a;
  for (; b != -20; b--) {
    (int)f;
    *e = 0;
    *e = foo(h);
  }
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR rtl-optimization/57281
// CHECK-NEXT: static mut a: i32 = 1 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: static mut b: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut d: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut e: *mut i32 = unsafe { std::ptr::addr_of_mut!(d) };
// CHECK-EMPTY:
// CHECK-NEXT: static mut c: i64 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut g: *mut i64 = unsafe { std::ptr::addr_of_mut!(c) };
// CHECK-EMPTY:
// CHECK-NEXT: static mut f: i64 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut h: i32) -> i32 {
// CHECK-NEXT:     let mut j: i32 = 0 as i32;
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: i64 = (unsafe { b }) as i64;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { g } = [[V_0]];
// CHECK-NEXT:     }
// CHECK-NEXT:     j = [[V_0]] as i32;
// CHECK-NEXT:     if h == (0 as i32) { j } else { 0 as i32 }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut h: i32 = unsafe { a };
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while (unsafe { b }) != -(20 as i32) {
// CHECK-NEXT:         (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(f)) }) as i32;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { e } = 0 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { e } = foo(h);
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = unsafe { b };
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = [[V_1]] - (1 as i32);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             b = [[V_2]];
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
