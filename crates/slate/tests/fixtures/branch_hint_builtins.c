// { dg-additional-options "-O1" }
#include <stdio.h>

static int side_effect_calls = 0;

static int bump(int v) {
  side_effect_calls++;
  return v;
}

static int use_expect(int x) {
  if (__builtin_expect(bump(x), 1)) {
    return 1;
  }
  return 0;
}

static int use_expect_with_probability(int x) {
  if (__builtin_expect_with_probability(bump(x), 1, 0.9)) {
    return 1;
  }
  return 0;
}

static int use_unpredictable(int x) {
  if (__builtin_unpredictable(bump(x) > 0)) {
    return 1;
  }
  return 0;
}

int main(void) {
  int a = use_expect(1);
  int b = use_expect_with_probability(1);
  int c = use_unpredictable(1);
  printf("%d %d %d %d\n", a, b, c, side_effect_calls);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-additional-options "-O1" }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT:     fn __builtin_expect([[V_0:__v[0-9]+]]: i64, [[V_1:__v[0-9]+]]: i64) -> i64;
// CHECK-NEXT:     fn __builtin_expect_with_probability([[V_2:__v[0-9]+]]: i64, [[V_3:__v[0-9]+]]: i64, [[V_4:__v[0-9]+]]: f64) -> i64;
// CHECK-NEXT:     fn __builtin_unpredictable([[V_5:__v[0-9]+]]: i64) -> i64;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut side_effect_calls: i32 = 0;
// CHECK-EMPTY:
// CHECK-NEXT: fn bump(v: i32) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         side_effect_calls = side_effect_calls + 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     v
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn use_expect(x: i32) -> i32 {
// CHECK-NEXT:     if (bump(x) as i64) != 0 {
// CHECK-NEXT:         return 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn use_expect_with_probability(x: i32) -> i32 {
// CHECK-NEXT:     if (bump(x) as i64) != 0 {
// CHECK-NEXT:         return 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn use_unpredictable(x: i32) -> i32 {
// CHECK-NEXT:     if bump(x) > 0 {
// CHECK-NEXT:         return 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let a: i32 = use_expect(1i32);
// CHECK-NEXT:     let b: i32 = use_expect_with_probability(1i32);
// CHECK-NEXT:     let c: i32 = use_unpredictable(1i32);
// CHECK-NEXT:     println!("{} {} {} {}", a, b, c, unsafe { side_effect_calls });
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
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
