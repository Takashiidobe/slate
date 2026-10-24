#include <stdio.h>

static int countdown(int n) {
  int steps = 0;
  while (1) {
    if (n <= 0) {
      break;
    }
    n /= 2;
    steps++;
  }
  return steps;
}

int main(void) {
  printf("%d\n", countdown(100));
  printf("%d\n", countdown(0));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn countdown(mut n: i32) -> i32 {
// CHECK-NEXT:     let mut steps: i32 = 0;
// CHECK-NEXT:     while true {
// CHECK-NEXT:         if n <= 0 {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:         n /= 2;
// CHECK-NEXT:         steps += 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     steps
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!("{}", countdown(100i32));
// CHECK-NEXT:     println!("{}", countdown(0i32));
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
