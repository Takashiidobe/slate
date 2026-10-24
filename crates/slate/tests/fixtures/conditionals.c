#include <stdio.h>

static int classify(int n) {
  if (n < 0) {
    return -1;
  } else if (n == 0) {
    return 0;
  } else {
    return 1;
  }
}

static int clamp_low(int n) {
  int r = n;
  if (n < 10) {
    r = 10;
  }
  return r;
}

int main(void) {
  printf("%d\n", classify(-5));
  printf("%d\n", classify(0));
  printf("%d\n", classify(42));
  printf("%d\n", clamp_low(3));
  printf("%d\n", clamp_low(20));

  int x = 7;
  int label;
  if (x % 2 == 0) {
    label = 100;
  } else {
    label = 200;
  }
  printf("%d\n", label);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn classify(n: i32) -> i32 {
// CHECK-NEXT:     if n < 0 {
// CHECK-NEXT:         return -1;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         if n == 0 {
// CHECK-NEXT:             return 0;
// CHECK-NEXT:         } else {
// CHECK-NEXT:             return 1;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn clamp_low(n: i32) -> i32 {
// CHECK-NEXT:     let mut r: i32 = n;
// CHECK-NEXT:     if n < 10 {
// CHECK-NEXT:         r = 10;
// CHECK-NEXT:     }
// CHECK-NEXT:     r
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!("{}", classify(- 5i32));
// CHECK-NEXT:     println!("{}", classify(0i32));
// CHECK-NEXT:     println!("{}", classify(42i32));
// CHECK-NEXT:     println!("{}", clamp_low(3i32));
// CHECK-NEXT:     println!("{}", clamp_low(20i32));
// CHECK-NEXT:     let x: i32 = 7;
// CHECK-NEXT:     let label: i32 = if x % 2 == 0 { 100 } else { 200 };
// CHECK-NEXT:     println!("{}", label);
// CHECK-NEXT:     0
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
