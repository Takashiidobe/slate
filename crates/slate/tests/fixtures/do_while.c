#include <stdio.h>

static int sum_do_while(int n) {
  int total = 0;
  int i     = 1;
  do {
    total += i;
    i++;
  } while (i <= n);
  return total;
}

static int runs_once_when_false(void) {
  int total = 0;
  do {
    total += 7;
  } while (0);
  return total;
}

static int continue_checks_condition(int n) {
  int total = 0;
  int i     = 0;
  do {
    i++;
    if (i % 2 == 0) {
      continue;
    }
    total += i;
  } while (i < n);
  return total;
}

int main(void) {
  printf("%d\n", sum_do_while(5));
  printf("%d\n", sum_do_while(0));
  printf("%d\n", runs_once_when_false());
  printf("%d\n", continue_checks_condition(6));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn sum_do_while(n: i32) -> i32 {
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     let mut i: i32 = 1;
// CHECK-NEXT:     loop {
// CHECK-NEXT:         total += i;
// CHECK-NEXT:         i += 1;
// CHECK-NEXT:         if i > n {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-NEXT: fn runs_once_when_false() -> i32 {
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     loop {
// CHECK-NEXT:         total += 7;
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-NEXT: fn continue_checks_condition(n: i32) -> i32 {
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     let mut i: i32 = 0;
// CHECK-NEXT:     loop {
// CHECK-NEXT:         i += 1;
// CHECK-NEXT:         if i % 2 != 0 {
// CHECK-NEXT:             total += i;
// CHECK-NEXT:         }
// CHECK-NEXT:         if i >= n {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!("{}", sum_do_while(5i32));
// CHECK-NEXT:     println!("{}", sum_do_while(0i32));
// CHECK-NEXT:     println!("{}", runs_once_when_false());
// CHECK-NEXT:     println!("{}", continue_checks_condition(6i32));
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
