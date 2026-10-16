/* { dg-do run }
   { dg-additional-options "-fno-trapping-math" } */

/* Copyright (C) 2004 Free Software Foundation.

   Test for composite comparison always true/false optimization.

   Written by Paolo Bonzini, 26th May 2004.  */

extern void link_error0();
extern void link_error1();

void test1(float x, float y) {
  if ((x == y) && (x != y))
    link_error0();
}

void test2(float x, float y) {
  if ((x < y) && (x > y))
    link_error0();
}

void test3(float x, float y) {
  if ((x < y) && (y < x))
    link_error0();
}

void test4(float x, float y) {
  if ((x == y) || (x != y)) {
  } else
    link_error1();
}

void test5(float x, float y) {
  if (__builtin_isunordered(x, y) || (x >= y) || (x < y)) {
  } else
    link_error1();
}

void test6(float x, float y) {
  if (__builtin_isunordered(y, x) || (x <= y) || (y < x)) {
  } else
    link_error1();
}

void test7(float x, float y) {
  if (__builtin_isunordered(x, y) || !__builtin_isunordered(x, y)) {
  } else
    link_error1();
}

void all_tests(float x, float y) {
  test1(x, y);
  test2(x, y);
  test3(x, y);
  test4(x, y);
  test5(x, y);
  test6(x, y);
  test7(x, y);
}

int main() {
  all_tests(0, 0);
  all_tests(1, 2);
  all_tests(4, 3);

  return 0;
}

#ifndef __OPTIMIZE__
void link_error0() {}
void link_error1() {}
#endif /* ! __OPTIMIZE__ */

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-do run }
// CHECK-NEXT: //! { dg-additional-options "-fno-trapping-math" }
// CHECK-NEXT: // Copyright (C) 2004 Free Software Foundation.
// CHECK-NEXT: //
// CHECK-NEXT: // Test for composite comparison always true/false optimization.
// CHECK-NEXT: //
// CHECK-NEXT: // Written by Paolo Bonzini, 26th May 2004.
// CHECK-NEXT: fn test1(mut x: f32, mut y: f32) {
// CHECK-NEXT:     if x == y && x != y {
// CHECK-NEXT:         link_error0();
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test2(mut x: f32, mut y: f32) {
// CHECK-NEXT:     if x < y && x > y {
// CHECK-NEXT:         link_error0();
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test3(mut x: f32, mut y: f32) {
// CHECK-NEXT:     if x < y && y < x {
// CHECK-NEXT:         link_error0();
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test4(mut x: f32, mut y: f32) {
// CHECK-NEXT:     if x == y || x != y {
// CHECK-NEXT:     } else {
// CHECK-NEXT:         link_error1();
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test5(mut x: f32, mut y: f32) {
// CHECK-NEXT:     if (x.is_nan() as i32) | (y.is_nan() as i32) != (0 as i32) || x >= y || x < y {
// CHECK-NEXT:     } else {
// CHECK-NEXT:         link_error1();
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test6(mut x: f32, mut y: f32) {
// CHECK-NEXT:     if (y.is_nan() as i32) | (x.is_nan() as i32) != (0 as i32) || x <= y || y < x {
// CHECK-NEXT:     } else {
// CHECK-NEXT:         link_error1();
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test7(mut x: f32, mut y: f32) {
// CHECK-NEXT:     if (x.is_nan() as i32) | (y.is_nan() as i32) != (0 as i32)
// CHECK-NEXT:         || !((x.is_nan() as i32) | (y.is_nan() as i32) != (0 as i32))
// CHECK-NEXT:     {
// CHECK-NEXT:     } else {
// CHECK-NEXT:         link_error1();
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn all_tests(mut x: f32, mut y: f32) {
// CHECK-NEXT:     test1(x, y);
// CHECK-NEXT:     test2(x, y);
// CHECK-NEXT:     test3(x, y);
// CHECK-NEXT:     test4(x, y);
// CHECK-NEXT:     test5(x, y);
// CHECK-NEXT:     test6(x, y);
// CHECK-NEXT:     test7(x, y);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     all_tests((0 as i32) as f32, (0 as i32) as f32);
// CHECK-NEXT:     all_tests((1 as i32) as f32, (2 as i32) as f32);
// CHECK-NEXT:     all_tests((4 as i32) as f32, (3 as i32) as f32);
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn link_error0() {}
// CHECK-EMPTY:
// CHECK-NEXT: fn link_error1() {}
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
