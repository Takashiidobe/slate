#include <stdio.h>

int classify(int n) {
  if (n < 0)
    goto neg;
  if (n == 0)
    goto zero;
  return 1;
neg:
  return -1;
zero:
  return 0;
}

int main() {
  printf("%d %d %d\n", classify(-5), classify(0), classify(42));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn classify(n: i32) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         if n < 0 {
// CHECK-NEXT:             return -1;
// CHECK-NEXT:         } else {
// CHECK-NEXT:             if n == 0 {
// CHECK-NEXT:                 return 0;
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 return 1;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {}", classify(- (5 as i32)), classify(0 as i32), classify(42 as i32)
// CHECK-NEXT:     );
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
