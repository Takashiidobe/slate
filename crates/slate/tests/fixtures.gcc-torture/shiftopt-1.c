/* Copyright (C) 2002  Free Software Foundation

   Check that constant folding of shift operations is working.

   Roger Sayle, 10th October 2002.  */

extern void abort(void);
extern void link_error(void);

void utest(unsigned int x) {
  if (x >> 0 != x)
    link_error();

  if (x << 0 != x)
    link_error();

  if (0 << x != 0)
    link_error();

  if (0 >> x != 0)
    link_error();

  if (-1 >> x != -1)
    link_error();

  if (~0 >> x != ~0)
    link_error();
}

void stest(int x) {
  if (x >> 0 != x)
    link_error();

  if (x << 0 != x)
    link_error();

  if (0 << x != 0)
    link_error();

  if (0 >> x != 0)
    link_error();
}

int main() {
  utest(9);
  utest(0);

  stest(9);
  stest(0);

  return 0;
}

#ifndef __OPTIMIZE__
void link_error() { abort(); }
#endif

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Copyright (C) 2002  Free Software Foundation
// CHECK-NEXT: //!
// CHECK-NEXT: //! Check that constant folding of shift operations is working.
// CHECK-NEXT: //!
// CHECK-NEXT: //! Roger Sayle, 10th October 2002.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn utest(mut x: u32) {
// CHECK-NEXT:     if x >> (0 as i32) != x {
// CHECK-NEXT:         link_error();
// CHECK-NEXT:     }
// CHECK-NEXT:     if x << (0 as i32) != x {
// CHECK-NEXT:         link_error();
// CHECK-NEXT:     }
// CHECK-NEXT:     if (0 as i32) << x != (0 as i32) {
// CHECK-NEXT:         link_error();
// CHECK-NEXT:     }
// CHECK-NEXT:     if (0 as i32) >> x != (0 as i32) {
// CHECK-NEXT:         link_error();
// CHECK-NEXT:     }
// CHECK-NEXT:     if -(1 as i32) >> x != -(1 as i32) {
// CHECK-NEXT:         link_error();
// CHECK-NEXT:     }
// CHECK-NEXT:     if !(0 as i32) >> x != !(0 as i32) {
// CHECK-NEXT:         link_error();
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn stest(mut x: i32) {
// CHECK-NEXT:     if x >> (0 as i32) != x {
// CHECK-NEXT:         link_error();
// CHECK-NEXT:     }
// CHECK-NEXT:     if x << (0 as i32) != x {
// CHECK-NEXT:         link_error();
// CHECK-NEXT:     }
// CHECK-NEXT:     if (0 as i32) << x != (0 as i32) {
// CHECK-NEXT:         link_error();
// CHECK-NEXT:     }
// CHECK-NEXT:     if (0 as i32) >> x != (0 as i32) {
// CHECK-NEXT:         link_error();
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     utest((9 as i32) as u32);
// CHECK-NEXT:     utest((0 as i32) as u32);
// CHECK-NEXT:     stest(9 as i32);
// CHECK-NEXT:     stest(0 as i32);
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
