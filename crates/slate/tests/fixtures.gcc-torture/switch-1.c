/* Copyright (C) 2003  Free Software Foundation.

   Test that switch statements suitable using case bit tests are
   implemented correctly.

   Written by Roger Sayle, 01/25/2001.  */

extern void abort(void);

int foo(int x) {
  switch (x) {
  case 4:
  case 6:
  case 9:
  case 11:
    return 30;
  }
  return 31;
}

int main() {
  int i, r;

  for (i = -1; i < 66; i++) {
    r = foo(i);
    if (i == 4) {
      if (r != 30)
        abort();
    } else if (i == 6) {
      if (r != 30)
        abort();
    } else if (i == 9) {
      if (r != 30)
        abort();
    } else if (i == 11) {
      if (r != 30)
        abort();
    } else if (r != 31)
      abort();
  }
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Copyright (C) 2003  Free Software Foundation.
// CHECK-NEXT: //!
// CHECK-NEXT: //! Test that switch statements suitable using case bit tests are
// CHECK-NEXT: //! implemented correctly.
// CHECK-NEXT: //!
// CHECK-NEXT: //! Written by Roger Sayle, 01/25/2001.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: i32) -> i32 {
// CHECK-NEXT:     match x {
// CHECK-NEXT:         4 | 6 | 9 | 11 => {
// CHECK-NEXT:             return 30 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         _ => {}
// CHECK-NEXT:     }
// CHECK-NEXT:     31 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut i: i32 = -(1 as i32);
// CHECK-NEXT:     let mut r: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < (66 as i32) {
// CHECK-NEXT:         r = foo(i);
// CHECK-NEXT:         if i == (4 as i32) {
// CHECK-NEXT:             if r != (30 as i32) {
// CHECK-NEXT:                 unsafe { abort() };
// CHECK-NEXT:             }
// CHECK-NEXT:         } else {
// CHECK-NEXT:             if i == (6 as i32) {
// CHECK-NEXT:                 if r != (30 as i32) {
// CHECK-NEXT:                     unsafe { abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 if i == (9 as i32) {
// CHECK-NEXT:                     if r != (30 as i32) {
// CHECK-NEXT:                         unsafe { abort() };
// CHECK-NEXT:                     }
// CHECK-NEXT:                 } else {
// CHECK-NEXT:                     if i == (11 as i32) {
// CHECK-NEXT:                         if r != (30 as i32) {
// CHECK-NEXT:                             unsafe { abort() };
// CHECK-NEXT:                         }
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         if r != (31 as i32) {
// CHECK-NEXT:                             unsafe { abort() };
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:         i = [[V_1]];
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
