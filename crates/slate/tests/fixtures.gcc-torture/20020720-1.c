/* Copyright (C) 2002  Free Software Foundation.

   Ensure that fabs(x) < 0.0 optimization is working.

   Written by Roger Sayle, 20th July 2002.  */

extern void   abort(void);
extern double fabs(double);
extern void   link_error(void);

void foo(double x) {
  double p, q;

  p = fabs(x);
  q = 0.0;
  if (p < q)
    link_error();
}

int main() {
  foo(1.0);
  return 0;
}

#ifndef __OPTIMIZE__
void link_error() { abort(); }
#endif

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Copyright (C) 2002  Free Software Foundation.
// CHECK-NEXT: //!
// CHECK-NEXT: //! Ensure that fabs(x) < 0.0 optimization is working.
// CHECK-NEXT: //!
// CHECK-NEXT: //! Written by Roger Sayle, 20th July 2002.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn fabs([[V_0:__v[0-9]+]]: f64) -> f64;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: f64) {
// CHECK-NEXT:     let mut p: f64 = unsafe { fabs(x) };
// CHECK-NEXT:     let mut q: f64 = 0.0f64;
// CHECK-NEXT:     if p < q {
// CHECK-NEXT:         link_error();
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     foo(1.0f64);
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn link_error() {
// CHECK-NEXT:     unsafe { abort() };
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
