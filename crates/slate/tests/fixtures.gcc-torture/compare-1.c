/* Copyright (C) 2002 Free Software Foundation.

   Test for correctness of composite comparisons.

   Written by Roger Sayle, 3rd June 2002.  */

extern void abort(void);

int ieq(int x, int y, int ok) {
  if ((x <= y) && (x >= y)) {
    if (!ok)
      abort();
  } else if (ok)
    abort();

  if ((x <= y) && (x == y)) {
    if (!ok)
      abort();
  } else if (ok)
    abort();

  if ((x <= y) && (y <= x)) {
    if (!ok)
      abort();
  } else if (ok)
    abort();

  if ((y == x) && (x <= y)) {
    if (!ok)
      abort();
  } else if (ok)
    abort();
}

int ine(int x, int y, int ok) {
  if ((x < y) || (x > y)) {
    if (!ok)
      abort();
  } else if (ok)
    abort();
}

int ilt(int x, int y, int ok) {
  if ((x < y) && (x != y)) {
    if (!ok)
      abort();
  } else if (ok)
    abort();
}

int ile(int x, int y, int ok) {
  if ((x < y) || (x == y)) {
    if (!ok)
      abort();
  } else if (ok)
    abort();
}

int igt(int x, int y, int ok) {
  if ((x > y) && (x != y)) {
    if (!ok)
      abort();
  } else if (ok)
    abort();
}

int ige(int x, int y, int ok) {
  if ((x > y) || (x == y)) {
    if (!ok)
      abort();
  } else if (ok)
    abort();
}

int main() {
  ieq(1, 4, 0);
  ieq(3, 3, 1);
  ieq(5, 2, 0);

  ine(1, 4, 1);
  ine(3, 3, 0);
  ine(5, 2, 1);

  ilt(1, 4, 1);
  ilt(3, 3, 0);
  ilt(5, 2, 0);

  ile(1, 4, 1);
  ile(3, 3, 1);
  ile(5, 2, 0);

  igt(1, 4, 0);
  igt(3, 3, 0);
  igt(5, 2, 1);

  ige(1, 4, 0);
  ige(3, 3, 1);
  ige(5, 2, 1);

  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Copyright (C) 2002 Free Software Foundation.
// CHECK-NEXT: //!
// CHECK-NEXT: //! Test for correctness of composite comparisons.
// CHECK-NEXT: //!
// CHECK-NEXT: //! Written by Roger Sayle, 3rd June 2002.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ieq(mut x: i32, mut y: i32, mut ok: i32) -> i32 {
// CHECK-NEXT:     if x <= y && x >= y {
// CHECK-NEXT:         if !(ok != (0 as i32)) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         if ok != (0 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     if x <= y && x == y {
// CHECK-NEXT:         if !(ok != (0 as i32)) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         if ok != (0 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     if x <= y && y <= x {
// CHECK-NEXT:         if !(ok != (0 as i32)) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         if ok != (0 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     if y == x && x <= y {
// CHECK-NEXT:         if !(ok != (0 as i32)) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         if ok != (0 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ine(mut x: i32, mut y: i32, mut ok: i32) -> i32 {
// CHECK-NEXT:     if x < y || x > y {
// CHECK-NEXT:         if !(ok != (0 as i32)) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         if ok != (0 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ilt(mut x: i32, mut y: i32, mut ok: i32) -> i32 {
// CHECK-NEXT:     if x < y && x != y {
// CHECK-NEXT:         if !(ok != (0 as i32)) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         if ok != (0 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ile(mut x: i32, mut y: i32, mut ok: i32) -> i32 {
// CHECK-NEXT:     if x < y || x == y {
// CHECK-NEXT:         if !(ok != (0 as i32)) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         if ok != (0 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn igt(mut x: i32, mut y: i32, mut ok: i32) -> i32 {
// CHECK-NEXT:     if x > y && x != y {
// CHECK-NEXT:         if !(ok != (0 as i32)) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         if ok != (0 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn ige(mut x: i32, mut y: i32, mut ok: i32) -> i32 {
// CHECK-NEXT:     if x > y || x == y {
// CHECK-NEXT:         if !(ok != (0 as i32)) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         if ok != (0 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     ieq(1 as i32, 4 as i32, 0 as i32);
// CHECK-NEXT:     ieq(3 as i32, 3 as i32, 1 as i32);
// CHECK-NEXT:     ieq(5 as i32, 2 as i32, 0 as i32);
// CHECK-NEXT:     ine(1 as i32, 4 as i32, 1 as i32);
// CHECK-NEXT:     ine(3 as i32, 3 as i32, 0 as i32);
// CHECK-NEXT:     ine(5 as i32, 2 as i32, 1 as i32);
// CHECK-NEXT:     ilt(1 as i32, 4 as i32, 1 as i32);
// CHECK-NEXT:     ilt(3 as i32, 3 as i32, 0 as i32);
// CHECK-NEXT:     ilt(5 as i32, 2 as i32, 0 as i32);
// CHECK-NEXT:     ile(1 as i32, 4 as i32, 1 as i32);
// CHECK-NEXT:     ile(3 as i32, 3 as i32, 1 as i32);
// CHECK-NEXT:     ile(5 as i32, 2 as i32, 0 as i32);
// CHECK-NEXT:     igt(1 as i32, 4 as i32, 0 as i32);
// CHECK-NEXT:     igt(3 as i32, 3 as i32, 0 as i32);
// CHECK-NEXT:     igt(5 as i32, 2 as i32, 1 as i32);
// CHECK-NEXT:     ige(1 as i32, 4 as i32, 0 as i32);
// CHECK-NEXT:     ige(3 as i32, 3 as i32, 1 as i32);
// CHECK-NEXT:     ige(5 as i32, 2 as i32, 1 as i32);
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
