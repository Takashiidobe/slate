/* PR middle-end/59138 */
/* Testcase by John Regehr <regehr@cs.utah.edu> */

extern void abort(void);

#pragma pack(1)

struct S0 {
  int   f0;
  int   f1;
  int   f2;
  short f3;
};

short a = 1;

struct S0 b = {1}, c, d, e;

struct S0 fn1() { return c; }

void fn2(void) {
  b = fn1();
  a = 0;
  d = e;
}

int main(void) {
  fn2();
  if (a != 0)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR middle-end/59138
// CHECK-NEXT: //!
// CHECK-NEXT: //! Testcase by John Regehr <regehr@cs.utah.edu>
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, packed)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct S0 {
// CHECK-NEXT:     f0: i32,
// CHECK-NEXT:     f1: i32,
// CHECK-NEXT:     f2: i32,
// CHECK-NEXT:     f3: i16,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut a: i16 = (1 as i32) as i16;
// CHECK-EMPTY:
// CHECK-NEXT: static mut b: S0 = {
// CHECK-NEXT:     let mut [[T_0:__t[0-9]+]]: S0 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].f0 = 1 as i32;
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-EMPTY:
// CHECK-NEXT: static mut c: S0 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut d: S0 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut e: S0 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn fn1() -> S0 {
// CHECK-NEXT:     return unsafe { c };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fn2() {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         b = fn1();
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         a = (0 as i32) as i16;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         d = unsafe { e };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     fn2();
// CHECK-NEXT:     if ((unsafe { a }) as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
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
