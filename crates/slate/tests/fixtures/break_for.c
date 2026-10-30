#include <stdio.h>

static int first_multiple(int n, int m) {
  for (int i = 1; i <= n; i++) {
    if (i % m == 0) {
      return i;
    }
    if (i > 100) {
      break;
    }
  }
  return -1;
}

int main(void) {
  printf("%d\n", first_multiple(50, 7));
  printf("%d\n", first_multiple(3, 7));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![allow(arithmetic_overflow, unconditional_panic)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn first_multiple(n: i32, m: i32) -> i32 {
// CHECK-NEXT:     let mut i: i32 = 1;
// CHECK-NEXT:     while i <= n {
// CHECK-NEXT:         if i % m == 0 {
// CHECK-NEXT:             return i;
// CHECK-NEXT:         }
// CHECK-NEXT:         if i > 100 {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:         i += 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     -1
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!("{}", first_multiple(50i32, 7i32));
// CHECK-NEXT:     println!("{}", first_multiple(3i32, 7i32));
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
