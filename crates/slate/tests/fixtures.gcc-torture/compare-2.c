/* Copyright (C) 2002 Free Software Foundation.

   Ensure that the composite comparison optimization doesn't misfire
   and attempt to combine a signed comparison with an unsigned one.

   Written by Roger Sayle, 3rd June 2002.  */

extern void abort(void);

int foo(int x, int y) {
  /* If miscompiled the following may become "x == y".  */
  return (x <= y) && ((unsigned int)x >= (unsigned int)y);
}

int main() {
  if (!foo(-1, 0))
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Copyright (C) 2002 Free Software Foundation.
// CHECK-NEXT: //!
// CHECK-NEXT: //! Ensure that the composite comparison optimization doesn't misfire
// CHECK-NEXT: //! and attempt to combine a signed comparison with an unsigned one.
// CHECK-NEXT: //!
// CHECK-NEXT: //! Written by Roger Sayle, 3rd June 2002.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: i32, mut y: i32) -> i32 {
// CHECK-NEXT:     // If miscompiled the following may become "x == y".
// CHECK-NEXT:     (x <= y && (x as u32) >= (y as u32)) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if !(foo(-(1 as i32), 0 as i32) != (0 as i32)) {
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
