/* Copyright (C) 1999 Free Software Foundation, Inc.
  Contributed by Nathan Sidwell 20 Jan 1999 <nathan@acm.org> */

/* check range combining boolean operations work */

extern void abort();

#define N 77

void func(int i) {
  /* fold-const does some clever things with range tests. Make sure
     we get (some of) them right */

  /* these must fail, regardless of the value of i */
  if ((i < 0) && (i >= 0))
    abort();
  if ((i > 0) && (i <= 0))
    abort();
  if ((i >= 0) && (i < 0))
    abort();
  if ((i <= 0) && (i > 0))
    abort();

  if ((i < N) && (i >= N))
    abort();
  if ((i > N) && (i <= N))
    abort();
  if ((i >= N) && (i < N))
    abort();
  if ((i <= N) && (i > N))
    abort();

  /* these must pass, regardless of the value of i */
  if (!((i < 0) || (i >= 0)))
    abort();
  if (!((i > 0) || (i <= 0)))
    abort();
  if (!((i >= 0) || (i < 0)))
    abort();
  if (!((i <= 0) || (i > 0)))
    abort();

  if (!((i < N) || (i >= N)))
    abort();
  if (!((i > N) || (i <= N)))
    abort();
  if (!((i >= N) || (i < N)))
    abort();
  if (!((i <= N) || (i > N)))
    abort();

  return;
}

int main() {
  func(0);
  func(1);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Copyright (C) 1999 Free Software Foundation, Inc.
// CHECK-NEXT: //! Contributed by Nathan Sidwell 20 Jan 1999 <nathan@acm.org>
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // check range combining boolean operations work
// CHECK-NEXT: fn func(mut i: i32) {
// CHECK-NEXT:     // fold-const does some clever things with range tests. Make sure
// CHECK-NEXT:     // we get (some of) them right
// CHECK-NEXT:     // these must fail, regardless of the value of i
// CHECK-NEXT:     if i < (0 as i32) && i >= (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if i > (0 as i32) && i <= (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if i >= (0 as i32) && i < (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if i <= (0 as i32) && i > (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if i < (77 as i32) && i >= (77 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if i > (77 as i32) && i <= (77 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if i >= (77 as i32) && i < (77 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if i <= (77 as i32) && i > (77 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     // these must pass, regardless of the value of i
// CHECK-NEXT:     if !(i < (0 as i32) || i >= (0 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if !(i > (0 as i32) || i <= (0 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if !(i >= (0 as i32) || i < (0 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if !(i <= (0 as i32) || i > (0 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if !(i < (77 as i32) || i >= (77 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if !(i > (77 as i32) || i <= (77 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if !(i >= (77 as i32) || i < (77 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if !(i <= (77 as i32) || i > (77 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     return;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     func(0 as i32);
// CHECK-NEXT:     func(1 as i32);
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
