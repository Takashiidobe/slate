#include <stdio.h>

int classify(int x) {
  switch (x) {
  case 1:;
  case 2:
    return 10;
  case 3: {
  }
  case 4: {
    ;
  }
  case 5:
    return 20;
  case 6:
    __attribute__((fallthrough));
  default:
    return 30;
  case -7:
    break;
  }
  return 40;
}

int stacked(unsigned long long x) {
  int out = 0;
  switch (x) {
  case 0:
  case 18446744073709551615ULL:
    out = 1;
    break;
  case 9223372036854775808ULL:
    out = 2;
  case 3:
    out += 3;
    break;
  }
  return out;
}

int main(void) {
  for (int i = -8; i <= 8; i++)
    printf("%d ", classify(i));
  printf("\n%d %d %d %d %d\n", stacked(0), stacked(18446744073709551615ULL),
         stacked(9223372036854775808ULL), stacked(3), stacked(4));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn classify(x: i32) -> i32 {
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: {
// CHECK-NEXT:         match x {
// CHECK-NEXT:             1 | 2 => {
// CHECK-NEXT:                 return 10;
// CHECK-NEXT:             }
// CHECK-NEXT:             3..=5 => {
// CHECK-NEXT:                 return 20;
// CHECK-NEXT:             }
// CHECK-NEXT:             -7 => {}
// CHECK-NEXT:             _ => {
// CHECK-NEXT:                 return 30;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     40
// CHECK-NEXT: }
// CHECK-NEXT: fn stacked(x: u64) -> i32 {
// CHECK-NEXT:     let mut out: i32 = 0;
// CHECK-NEXT:     match x {
// CHECK-NEXT:         0 | 18446744073709551615 => {
// CHECK-NEXT:             out = 1;
// CHECK-NEXT:         }
// CHECK-NEXT:         9223372036854775808 => {
// CHECK-NEXT:             out = 2;
// CHECK-NEXT:             let [[V_0:_v[0-9]+]]: i32 = out;
// CHECK-NEXT:             let [[V_1:_v[0-9]+]]: i32 = [[V_0]] + 3;
// CHECK-NEXT:             out = [[V_1]];
// CHECK-NEXT:         }
// CHECK-NEXT:         3 => {
// CHECK-NEXT:             out += 3;
// CHECK-NEXT:         }
// CHECK-NEXT:         _ => {}
// CHECK-NEXT:     }
// CHECK-NEXT:     out
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut i: i32 = -8;
// CHECK-NEXT:     while i <= 8 {
// CHECK-NEXT:         print!("{} ", classify(i));
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         i += 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "\n{} {} {} {} {}", stacked(0 as u64), stacked(18446744073709551615 as u64),
// CHECK-NEXT:         stacked(9223372036854775808 as u64), stacked(3 as u64), stacked(4 as u64)
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
