/* Copyright (C) 2004 Free Software Foundation.

   Check that constant folding and RTL simplification of -(x >> y) doesn't
   break anything and produces the expected results.

   Written by Roger Sayle, 11th March 2004.  */

extern void abort(void);

#define INT_BITS (sizeof(int) * 8)

int test1(int x) { return -(x >> (INT_BITS - 1)); }

int test2(unsigned int x) { return -((int)(x >> (INT_BITS - 1))); }

int test3(int x) {
  int y;
  y = INT_BITS - 1;
  return -(x >> y);
}

int test4(unsigned int x) {
  int y;
  y = INT_BITS - 1;
  return -((int)(x >> y));
}

int main() {
  if (test1(0) != 0)
    abort();
  if (test1(1) != 0)
    abort();
  if (test1(-1) != 1)
    abort();

  if (test2(0) != 0)
    abort();
  if (test2(1) != 0)
    abort();
  if (test2((unsigned int)-1) != -1)
    abort();

  if (test3(0) != 0)
    abort();
  if (test3(1) != 0)
    abort();
  if (test3(-1) != 1)
    abort();

  if (test4(0) != 0)
    abort();
  if (test4(1) != 0)
    abort();
  if (test4((unsigned int)-1) != -1)
    abort();

  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Copyright (C) 2004 Free Software Foundation.
// CHECK-NEXT: //!
// CHECK-NEXT: //! Check that constant folding and RTL simplification of -(x >> y) doesn't
// CHECK-NEXT: //! break anything and produces the expected results.
// CHECK-NEXT: //!
// CHECK-NEXT: //! Written by Roger Sayle, 11th March 2004.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test1(mut x: i32) -> i32 {
// CHECK-NEXT:     -(x >> (4 as u64)
// CHECK-NEXT:         .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:         .wrapping_sub(((1 as i32) as i64) as u64))
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test2(mut x: u32) -> i32 {
// CHECK-NEXT:     -((x >> (4 as u64)
// CHECK-NEXT:         .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:         .wrapping_sub(((1 as i32) as i64) as u64)) as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test3(mut x: i32) -> i32 {
// CHECK-NEXT:     let mut y: i32 = ((4 as u64)
// CHECK-NEXT:         .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:         .wrapping_sub(((1 as i32) as i64) as u64) as u32) as i32;
// CHECK-NEXT:     -(x >> y)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test4(mut x: u32) -> i32 {
// CHECK-NEXT:     let mut y: i32 = ((4 as u64)
// CHECK-NEXT:         .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:         .wrapping_sub(((1 as i32) as i64) as u64) as u32) as i32;
// CHECK-NEXT:     -((x >> y) as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if test1(0 as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test1(1 as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test1(-(1 as i32)) != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test2((0 as i32) as u32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test2((1 as i32) as u32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test2(-(1 as i32) as u32) != -(1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test3(0 as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test3(1 as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test3(-(1 as i32)) != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test4((0 as i32) as u32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test4((1 as i32) as u32) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if test4(-(1 as i32) as u32) != -(1 as i32) {
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
