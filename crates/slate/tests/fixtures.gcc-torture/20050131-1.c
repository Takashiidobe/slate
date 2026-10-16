/* Verify that we do not lose side effects on a MOD expression.  */

#include <stdio.h>
#include <stdlib.h>

int foo(int a) {
  int x = 0 % a++;
  return a;
}

int main(void) {
  if (foo(9) != 10)
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Verify that we do not lose side effects on a MOD expression.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit(__status: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut a: i32) -> i32 {
// CHECK-NEXT:     let mut x: i32 = 0 as i32;
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: i32 = a;
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:     a = [[V_1]];
// CHECK-NEXT:     x = (0 as i32) % [[V_0]];
// CHECK-NEXT:     a
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if foo(9 as i32) != (10 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
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
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
