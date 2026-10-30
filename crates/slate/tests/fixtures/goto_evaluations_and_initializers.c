#include <stdio.h>

static int initializers;
static int initialize(void) { ++initializers; return 19; }

static int run(int mode) {
  int sum = 0;
  int checks = 0;
  int steps = 0;
  int i = 0;
  if (mode) goto entered;
  {
    int value = initialize();
  entered:
    value = 3;
    sum += value;
  }
  for (i = 0; ++checks && i < 4; ++steps, ++i) {
  retry:
    if (i == 0) { i = 1; goto retry; }
    if (i == 2) continue;
    sum += i;
  }
  do {
    ++sum;
    if (sum < 10) continue;
    break;
  } while (++checks < 20);
  switch (mode) {
    int skipped = initialize();
    case 1:
      sum += 2;
    case 2 ... 4:
      sum += 3;
      break;
    default:
      sum += 4;
  }
  printf("%d %d %d %d\n", sum, checks, steps, initializers);
  return sum;
}

int main(void) {
  run(1);
  run(0);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: static mut initializers: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn initialize() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         initializers = initializers + 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     19
// CHECK-NEXT: }
// CHECK-NEXT: fn run(mode: i32) -> i32 {
// CHECK-NEXT:     let mut value: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut i: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut steps: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut checks: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut sum: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         sum = 0;
// CHECK-NEXT:         checks = 0;
// CHECK-NEXT:         steps = 0;
// CHECK-NEXT:         i = 0;
// CHECK-NEXT:         if mode == 0 {
// CHECK-NEXT:             value = initialize();
// CHECK-NEXT:         }
// CHECK-NEXT:         value = 3;
// CHECK-NEXT:         sum += value;
// CHECK-NEXT:         i = 0;
// CHECK-NEXT:         loop {
// CHECK-NEXT:             let [[V_0:__v[0-9]+]]: i32 = checks + 1;
// CHECK-NEXT:             checks = [[V_0]];
// CHECK-NEXT:             if [[V_0]] != 0 && i < 4 {
// CHECK-NEXT:                 while i == 0 {
// CHECK-NEXT:                     i = 1;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if i != 2 {
// CHECK-NEXT:                     sum += i;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 steps += 1;
// CHECK-NEXT:                 i += 1;
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         loop {
// CHECK-NEXT:             sum += 1;
// CHECK-NEXT:             if sum < 10 {
// CHECK-NEXT:                 let [[V_1:__v[0-9]+]]: i32 = checks + 1;
// CHECK-NEXT:                 checks = [[V_1]];
// CHECK-NEXT:                 if [[V_1]] >= 20 {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         '[[JOIN_0:__join_[0-9]+]]: {
// CHECK-NEXT:             let [[T_0:__t[0-9]+]]: i32 = mode;
// CHECK-NEXT:             if [[T_0]] == 1 {
// CHECK-NEXT:                 sum += 2;
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 if !([[T_0]] >= 2 && [[T_0]] <= 4) {
// CHECK-NEXT:                     sum += 4;
// CHECK-NEXT:                     break '[[JOIN_0]];
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             sum += 3;
// CHECK-NEXT:         }
// CHECK-NEXT:         println!("{} {} {} {}", sum, checks, steps, unsafe { initializers });
// CHECK-NEXT:         return sum;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     run(1i32);
// CHECK-NEXT:     run(0i32);
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
