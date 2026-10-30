#include <stdio.h>

static int sum_grid(int n) {
  int total = 0;
  for (int i = 0; i < n; i++) {
    if (i == 2) {
      continue;
    }
    for (int j = 0; j < n; j++) {
      if (j == 3) {
        continue;
      }
      total += i * 10 + j;
    }
  }
  return total;
}

int main(void) {
  printf("%d\n", sum_grid(5));
  printf("%d\n", sum_grid(1));
  printf("%d\n", sum_grid(0));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn sum_grid(n: i32) -> i32 {
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     for i in 0..n {
// CHECK-NEXT:         if i == 2 {
// CHECK-NEXT:             continue;
// CHECK-NEXT:         }
// CHECK-NEXT:         for j in 0..n {
// CHECK-NEXT:             if j == 3 {
// CHECK-NEXT:                 continue;
// CHECK-NEXT:             }
// CHECK-NEXT:             total += i * 10 + j;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!("{}", sum_grid(5i32));
// CHECK-NEXT:     println!("{}", sum_grid(1i32));
// CHECK-NEXT:     println!("{}", sum_grid(0i32));
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
