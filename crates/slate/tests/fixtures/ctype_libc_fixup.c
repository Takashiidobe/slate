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
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn tolower(__c: i32) -> i32;
// CHECK-NEXT:     fn toupper(__c: i32) -> i32;
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn next_lower() -> i32 {
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: i32 = unsafe { c };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         c = [[V_0]] + 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     [[V_0]]
// CHECK-NEXT: }
// CHECK-NEXT: static mut c: i32 = 97 as i32;
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let upper: i32 = 81;
// CHECK-NEXT:     let lower: i32 = 113;
// CHECK-NEXT:     let digit: i32 = 53;
// CHECK-NEXT:     let punct: i32 = 33;
// CHECK-NEXT:     let eof: i32 = -1;
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {} {} {}", unsafe { toupper(lower) }, unsafe { toupper(digit) }, unsafe {
// CHECK-NEXT:         toupper(punct) }, unsafe { toupper(upper) }, unsafe { toupper(eof) }
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {} {} {}", unsafe { tolower(upper) }, unsafe { tolower(digit) }, unsafe {
// CHECK-NEXT:         tolower(punct) }, unsafe { tolower(lower) }, unsafe { tolower(eof) }
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {}", unsafe { toupper(next_lower()) }, unsafe { tolower(next_lower()) }
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
