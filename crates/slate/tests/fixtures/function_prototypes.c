#include <stdio.h>

int is_even(int n);

int is_odd(int n) {
  if (n == 0)
    return 0;
  return is_even(n - 1);
}

int is_even(int n) {
  if (n == 0)
    return 1;
  return is_odd(n - 1);
}

int square(int x);

int main(void) {
  printf("%d %d\n", is_odd(7), is_even(10));
  printf("%d\n", square(5));
  return 0;
}

int square(int x) { return x * x; }

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn is_odd(n: i32) -> i32 {
// CHECK-NEXT:     if n == 0 {
// CHECK-NEXT:         return 0;
// CHECK-NEXT:     }
// CHECK-NEXT:     is_even(n - 1)
// CHECK-NEXT: }
// CHECK-NEXT: fn is_even(n: i32) -> i32 {
// CHECK-NEXT:     if n == 0 {
// CHECK-NEXT:         return 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     is_odd(n - 1)
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!("{} {}", is_odd(7i32), is_even(10i32));
// CHECK-NEXT:     println!("{}", square(5i32));
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: fn square(x: i32) -> i32 {
// CHECK-NEXT:     x * x
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
