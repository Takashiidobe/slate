#include <stdio.h>

int map(int x) {
  int out = 0;
  switch (x) {
  case -3:
    out = 13;
    break;
  case 100:
    out = 1000;
    break;
  default:
    out = -1;
    break;
  }
  return out;
}

int main(void) {
  printf("%d %d %d\n", map(-3), map(100), map(0));
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn map(x: i32) -> i32 {
// CHECK-NEXT:     let mut out: i32 = 0;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: {
// CHECK-NEXT:         match x {
// CHECK-NEXT:             -3 => {
// CHECK-NEXT:                 out = 13;
// CHECK-NEXT:             }
// CHECK-NEXT:             100 => {
// CHECK-NEXT:                 out = 1000;
// CHECK-NEXT:             }
// CHECK-NEXT:             _ => {
// CHECK-NEXT:                 out = -1;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     out
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!("{} {} {}", map(- (3 as i32)), map(100 as i32), map(0 as i32));
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
