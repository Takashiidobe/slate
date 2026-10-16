/* Copyright (C) 2002 Free Software Foundation.

   Test for composite comparison always true/false optimization.

   Written by Roger Sayle, 7th June 2002.  */

extern void link_error0();
extern void link_error1();

void test1(int x, int y) {
  if ((x == y) && (x != y))
    link_error0();
}

void test2(int x, int y) {
  if ((x < y) && (x > y))
    link_error0();
}

void test3(int x, int y) {
  if ((x < y) && (y < x))
    link_error0();
}

void test4(int x, int y) {
  if ((x == y) || (x != y)) {
  } else
    link_error1();
}

void test5(int x, int y) {
  if ((x >= y) || (x < y)) {
  } else
    link_error1();
}

void test6(int x, int y) {
  if ((x <= y) || (y < x)) {
  } else
    link_error1();
}

void all_tests(int x, int y) {
  test1(x, y);
  test2(x, y);
  test3(x, y);
  test4(x, y);
  test5(x, y);
  test6(x, y);
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
// CHECK-NEXT: //! Copyright (C) 2002 Free Software Foundation.
// CHECK-NEXT: //!
// CHECK-NEXT: //! Test for composite comparison always true/false optimization.
// CHECK-NEXT: //!
// CHECK-NEXT: //! Written by Roger Sayle, 7th June 2002.
// CHECK-NEXT: fn test1(mut x: i32, mut y: i32) {
// CHECK-NEXT:     if x == y && x != y {
// CHECK-NEXT:         link_error0();
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test2(mut x: i32, mut y: i32) {
// CHECK-NEXT:     if x < y && x > y {
// CHECK-NEXT:         link_error0();
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test3(mut x: i32, mut y: i32) {
// CHECK-NEXT:     if x < y && y < x {
// CHECK-NEXT:         link_error0();
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test4(mut x: i32, mut y: i32) {
// CHECK-NEXT:     if x == y || x != y {
// CHECK-NEXT:     } else {
// CHECK-NEXT:         link_error1();
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test5(mut x: i32, mut y: i32) {
// CHECK-NEXT:     if x >= y || x < y {
// CHECK-NEXT:     } else {
// CHECK-NEXT:         link_error1();
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test6(mut x: i32, mut y: i32) {
// CHECK-NEXT:     if x <= y || y < x {
// CHECK-NEXT:     } else {
// CHECK-NEXT:         link_error1();
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn all_tests(mut x: i32, mut y: i32) {
// CHECK-NEXT:     test1(x, y);
// CHECK-NEXT:     test2(x, y);
// CHECK-NEXT:     test3(x, y);
// CHECK-NEXT:     test4(x, y);
// CHECK-NEXT:     test5(x, y);
// CHECK-NEXT:     test6(x, y);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     all_tests(0 as i32, 0 as i32);
// CHECK-NEXT:     all_tests(1 as i32, 2 as i32);
// CHECK-NEXT:     all_tests(4 as i32, 3 as i32);
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
