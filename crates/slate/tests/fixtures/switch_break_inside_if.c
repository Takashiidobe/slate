#include <stdio.h>

int classify(int x, int extra) {
  int result = -1;
  switch (x) {
  case 0:
    if (extra) {
      result = 10;
      break;
    }
    result = 11;
    break;
  case 1:
    result = 20;
    break;
  default:
    result = -2;
    break;
  }
  return result;
}

int main(void) {
  printf("%d\n", classify(0, 1));
  printf("%d\n", classify(0, 0));
  printf("%d\n", classify(1, 0));
  printf("%d\n", classify(5, 0));
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn classify(x: i32, extra: i32) -> i32 {
// CHECK-NEXT:     let mut result: i32 = -1;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: {
// CHECK-NEXT:         match x {
// CHECK-NEXT:             0 => {
// CHECK-NEXT:                 if extra != 0 {
// CHECK-NEXT:                     result = 10;
// CHECK-NEXT:                 } else {
// CHECK-NEXT:                     result = 11;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             1 => {
// CHECK-NEXT:                 result = 20;
// CHECK-NEXT:             }
// CHECK-NEXT:             _ => {
// CHECK-NEXT:                 result = -2;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     result
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!("{}", classify(0 as i32, 1 as i32));
// CHECK-NEXT:     println!("{}", classify(0 as i32, 0 as i32));
// CHECK-NEXT:     println!("{}", classify(1 as i32, 0 as i32));
// CHECK-NEXT:     println!("{}", classify(5 as i32, 0 as i32));
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
