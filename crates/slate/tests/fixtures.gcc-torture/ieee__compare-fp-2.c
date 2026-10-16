/* { dg-do run } */
/* Copyright (C) 2004 Free Software Foundation.

   Ensure that the composite comparison optimization doesn't misfire
   and attempt to combine an integer comparison with a floating-point one.

   Written by Paolo Bonzini, 26th May 2004.  */

extern void abort(void);

int foo(double x, double y) {
  /* If miscompiled the following may become false.  */
  return (x > y) && ((int)x == (int)y);
}

int main() {
  if (!foo(1.3, 1.0))
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-do run }
// CHECK-NEXT: //!
// CHECK-NEXT: //! Copyright (C) 2004 Free Software Foundation.
// CHECK-NEXT: //!
// CHECK-NEXT: //! Ensure that the composite comparison optimization doesn't misfire
// CHECK-NEXT: //! and attempt to combine an integer comparison with a floating-point one.
// CHECK-NEXT: //!
// CHECK-NEXT: //! Written by Paolo Bonzini, 26th May 2004.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: f64, mut y: f64) -> i32 {
// CHECK-NEXT:     // If miscompiled the following may become false.
// CHECK-NEXT:     (x > y && (x as i32) == (y as i32)) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if !(foo(1.3f64, 1.0f64) != (0 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
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
// SLATE-FILECHECK-END
