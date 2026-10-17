#include <stdio.h>

static int sum_odds(int n) {
  int total = 0;
  for (int i = 0; i <= n; i++) {
    if (i % 2 == 0) {
      continue;
    }
    total += i;
  }
  return total;
}

int main(void) {
  printf("%d\n", sum_odds(10));
  printf("%d\n", sum_odds(1));
  printf("%d\n", sum_odds(0));
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn sum_odds(n: i32) -> i32 {
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     let mut i: i32 = 0;
// CHECK-NEXT:     while i <= n {
// CHECK-NEXT:         if i % 2 == 0 {} else {
// CHECK-NEXT:             total += i;
// CHECK-NEXT:         }
// CHECK-NEXT:         i += 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!("{}", sum_odds(10 as i32));
// CHECK-NEXT:     println!("{}", sum_odds(1 as i32));
// CHECK-NEXT:     println!("{}", sum_odds(0 as i32));
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
