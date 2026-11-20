#include <ctype.h>
#include <stdio.h>

static int next_lower(void) {
  static int c = 'a';
  return c++;
}

int main(void) {
  int upper = 'Q';
  int lower = 'q';
  int digit = '5';
  int punct = '!';
  int eof   = EOF;

  printf("%d %d %d %d %d\n", toupper(lower), toupper(digit), toupper(punct),
         toupper(upper), toupper(eof));
  printf("%d %d %d %d %d\n", tolower(upper), tolower(digit), tolower(punct),
         tolower(lower), tolower(eof));
  printf("%d %d\n", toupper(next_lower()), tolower(next_lower()));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: fn next_lower() -> i32 {
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: i32 = unsafe { c };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         c = [[V_0]] + 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     [[V_0]]
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut c: i32 = 97;
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let upper: i32 = 81;
// CHECK-NEXT:     let lower: i32 = 113;
// CHECK-NEXT:     let digit: i32 = 53;
// CHECK-NEXT:     let punct: i32 = 33;
// CHECK-NEXT:     let eof: i32 = slate_support::EOF as i32;
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {} {} {}",
// CHECK-NEXT:         unsafe { slate_support::toupper(lower) },
// CHECK-NEXT:         unsafe { slate_support::toupper(digit) },
// CHECK-NEXT:         unsafe { slate_support::toupper(punct) },
// CHECK-NEXT:         unsafe { slate_support::toupper(upper) },
// CHECK-NEXT:         unsafe { slate_support::toupper(eof) }
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {} {} {}",
// CHECK-NEXT:         unsafe { slate_support::tolower(upper) },
// CHECK-NEXT:         unsafe { slate_support::tolower(digit) },
// CHECK-NEXT:         unsafe { slate_support::tolower(punct) },
// CHECK-NEXT:         unsafe { slate_support::tolower(lower) },
// CHECK-NEXT:         unsafe { slate_support::tolower(eof) }
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {}",
// CHECK-NEXT:         unsafe { slate_support::toupper(next_lower()) },
// CHECK-NEXT:         unsafe { slate_support::tolower(next_lower()) }
// CHECK-NEXT:     );
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut core::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut core::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
