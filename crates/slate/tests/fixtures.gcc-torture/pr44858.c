/* PR rtl-optimization/44858 */

extern void abort(void);
int         a = 3;
int         b = 1;

__attribute__((noinline)) long long foo(int x, int y) { return x / y; }

__attribute__((noinline)) int bar(void) {
  int c  = 2;
  c     &= foo(1, b) > b;
  b      = (a != 0) | c;
  return c;
}

int main(void) {
  if (bar() != 0 || b != 1)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR rtl-optimization/44858
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut a: i32 = 3 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: static mut b: i32 = 1 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: i32, mut y: i32) -> i64 {
// CHECK-NEXT:     (x / y) as i64
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar() -> i32 {
// CHECK-NEXT:     let mut c: i32 = 2 as i32;
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: i32 = c;
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] & ((foo(1 as i32, unsafe { b }) > ((unsafe { b }) as i64)) as i32);
// CHECK-NEXT:     c = [[V_1]];
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         b = (((unsafe { a }) != (0 as i32)) as i32) | c;
// CHECK-NEXT:     }
// CHECK-NEXT:     c
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if bar() != (0 as i32) || (unsafe { b }) != (1 as i32) {
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
