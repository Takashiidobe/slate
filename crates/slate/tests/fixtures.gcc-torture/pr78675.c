/* PR tree-optimization/78675 */

long int a;

__attribute__((noinline, noclone)) long int foo(long int x) {
  long int b;
  while (a < 1) {
    b = a && x;
    ++a;
  }
  return b;
}

int main() {
  if (foo(0) != 0)
    __builtin_abort();
  a = 0;
  if (foo(1) != 0)
    __builtin_abort();
  a = 0;
  if (foo(25) != 0)
    __builtin_abort();
  a = -64;
  if (foo(0) != 0)
    __builtin_abort();
  a = -64;
  if (foo(1) != 0)
    __builtin_abort();
  a = -64;
  if (foo(25) != 0)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/78675
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut a: i64 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: i64) -> i64 {
// CHECK-NEXT:     let mut b: i64 = 0 as i64;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while (unsafe { a }) < ((1 as i32) as i64) {
// CHECK-NEXT:         b = ((unsafe { a }) != (0 as i64) && x != (0 as i64)) as i64;
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i64 = unsafe { a };
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i64 = [[V_0]] + ((1 as i32) as i64);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             a = [[V_1]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     b
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if foo((0 as i32) as i64) != ((0 as i32) as i64) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         a = (0 as i32) as i64;
// CHECK-NEXT:     }
// CHECK-NEXT:     if foo((1 as i32) as i64) != ((0 as i32) as i64) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         a = (0 as i32) as i64;
// CHECK-NEXT:     }
// CHECK-NEXT:     if foo((25 as i32) as i64) != ((0 as i32) as i64) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         a = -(64 as i32) as i64;
// CHECK-NEXT:     }
// CHECK-NEXT:     if foo((0 as i32) as i64) != ((0 as i32) as i64) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         a = -(64 as i32) as i64;
// CHECK-NEXT:     }
// CHECK-NEXT:     if foo((1 as i32) as i64) != ((0 as i32) as i64) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         a = -(64 as i32) as i64;
// CHECK-NEXT:     }
// CHECK-NEXT:     if foo((25 as i32) as i64) != ((0 as i32) as i64) {
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
