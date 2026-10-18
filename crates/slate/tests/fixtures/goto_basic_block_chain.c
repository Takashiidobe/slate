#include <stdio.h>

static int trace;
static int *saved;

static int mark(int digit) {
  trace = trace * 10 + digit;
  return digit;
}

static int run(int skip) {
  int result = mark(1);
  saved = &result;
  if (skip) goto entered;
  {
    int value = mark(2);
  entered:
    value = mark(3);
    result += value;
    *saved += mark(4);
  }
  goto alias;
  result = mark(9);
alias:
  goto finish;
finish:
  result += mark(5);
  printf("%d %d\n", result, trace);
  return result;
}

static int straight_line(void) {
  int result = mark(6);
  goto finish;
  result = mark(9);
finish:
  result += mark(7);
  result += mark(8);
  return result;
}

int main(void) {
  run(0);
  trace = 0;
  run(1);
  trace = 0;
  printf("%d\n", straight_line());
  printf("%d\n", trace);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: static mut trace: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut saved: *mut i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn mark(digit: i32) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         trace = (unsafe { trace }) * 10 + digit;
// CHECK-NEXT:     }
// CHECK-NEXT:     digit
// CHECK-NEXT: }
// CHECK-NEXT: fn run(skip: i32) -> i32 {
// CHECK-NEXT:     let mut value: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut result: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         '[[JOIN_0:__join_[0-9]+]]: {
// CHECK-NEXT:             result = mark(1 as i32);
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 saved = std::ptr::addr_of_mut!(result);
// CHECK-NEXT:             }
// CHECK-NEXT:             if skip != 0 {} else {
// CHECK-NEXT:                 value = mark(2 as i32);
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         value = mark(3 as i32);
// CHECK-NEXT:         result += value;
// CHECK-NEXT:         let [[V_0:__v[0-9]+]]: i32 = mark(4 as i32);
// CHECK-NEXT:         let [[V_1:__v[0-9]+]]: *mut i32 = unsafe { saved };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *[[V_1]] = (unsafe { *[[V_1]] }) + [[V_0]];
// CHECK-NEXT:         }
// CHECK-NEXT:         result += mark(5 as i32);
// CHECK-NEXT:         println!("{} {}", result, unsafe { trace });
// CHECK-NEXT:         return result;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn straight_line() -> i32 {
// CHECK-NEXT:     let mut result: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         result = mark(6 as i32);
// CHECK-NEXT:         result += mark(7 as i32);
// CHECK-NEXT:         result += mark(8 as i32);
// CHECK-NEXT:         return result;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     run(0 as i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         trace = 0;
// CHECK-NEXT:     }
// CHECK-NEXT:     run(1 as i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         trace = 0;
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("{}", straight_line());
// CHECK-NEXT:     println!("{}", unsafe { trace });
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
