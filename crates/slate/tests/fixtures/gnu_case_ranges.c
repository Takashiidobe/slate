#include <stdio.h>

static int classify(int value) {
  int result = 0;
  switch (value) {
  case 1 ... 4:
    result += 10;
  case 5 ... 8:
    result += 20;
    break;
  case 10 ... 12:
    result += 40;
    break;
  default:
    result = 90;
  }
  return result;
}

static int classify_direct(int value) {
  switch (value) {
  case -2 ... 2:
    return 7;
  default:
    return 9;
  }
}

static int classify_empty(int value) {
  switch (value) {
  case 5 ... 3:
    return 1;
  case 4:
    return 2;
  }
  return 3;
}

int main(void) {
  printf("%d %d %d %d %d %d %d %d %d %d %d %d\n", classify(1), classify(2),
         classify(4), classify(5), classify(7), classify(8), classify(9),
         classify(10), classify(11), classify(12), classify_direct(-1),
         classify_direct(3));
  printf("%d %d %d\n", classify_empty(3), classify_empty(4), classify_empty(5));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn classify(value: i32) -> i32 {
// CHECK-NEXT:     let mut result: i32 = 0;
// CHECK-NEXT:     match value {
// CHECK-NEXT:         1..=4 => {
// CHECK-NEXT:             result += 10;
// CHECK-NEXT:             let [[V_0:_v[0-9]+]]: i32 = result;
// CHECK-NEXT:             let [[V_1:_v[0-9]+]]: i32 = [[V_0]] + 20;
// CHECK-NEXT:             result = [[V_1]];
// CHECK-NEXT:         }
// CHECK-NEXT:         5..=8 => {
// CHECK-NEXT:             result += 20;
// CHECK-NEXT:         }
// CHECK-NEXT:         10..=12 => {
// CHECK-NEXT:             result += 40;
// CHECK-NEXT:         }
// CHECK-NEXT:         _ => {
// CHECK-NEXT:             result = 90;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     result
// CHECK-NEXT: }
// CHECK-NEXT: fn classify_direct(value: i32) -> i32 {
// CHECK-NEXT:     match value {
// CHECK-NEXT:         -2..=2 => {
// CHECK-NEXT:             return 7;
// CHECK-NEXT:         }
// CHECK-NEXT:         _ => {
// CHECK-NEXT:             return 9;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-NEXT: fn classify_empty(value: i32) -> i32 {
// CHECK-NEXT:     match value {
// CHECK-NEXT:         4 => {
// CHECK-NEXT:             return 2;
// CHECK-NEXT:         }
// CHECK-NEXT:         _ => {}
// CHECK-NEXT:     }
// CHECK-NEXT:     3
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {} {} {} {} {} {} {} {} {} {}", classify(1 as i32), classify(2 as i32),
// CHECK-NEXT:         classify(4 as i32), classify(5 as i32), classify(7 as i32), classify(8 as i32),
// CHECK-NEXT:         classify(9 as i32), classify(10 as i32), classify(11 as i32), classify(12 as
// CHECK-NEXT:         i32), classify_direct(- (1 as i32)), classify_direct(3 as i32)
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {}", classify_empty(3 as i32), classify_empty(4 as i32), classify_empty(5
// CHECK-NEXT:         as i32)
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
