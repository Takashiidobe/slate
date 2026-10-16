/* The composite type of int and an enum compatible with int might be
   either of the two types, but it isn't an unsigned type.  */
/* Origin: Joseph Myers <jsm@polyomino.org.uk> */

#include <limits.h>

#include <stdio.h>

extern void abort(void);
extern void exit(int);

enum e { a = INT_MIN };

int    *p;
enum e *q;
int     main(void) {
  enum e x = a;
  q        = &x;
  if (*(1 ? q : p) > 0)
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! The composite type of int and an enum compatible with int might be
// CHECK-NEXT: //! either of the two types, but it isn't an unsigned type.
// CHECK-NEXT: //!
// CHECK-NEXT: //! Origin: Joseph Myers <jsm@polyomino.org.uk>
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut p: *mut i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut q: *mut i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut x: i32 = -2147483648 as i32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         q = std::ptr::addr_of_mut!(x);
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((unsafe {
// CHECK-NEXT:         *if (1 as i32) != (0 as i32) {
// CHECK-NEXT:             unsafe { q }
// CHECK-NEXT:         } else {
// CHECK-NEXT:             (unsafe { p }) as *mut i32
// CHECK-NEXT:         }
// CHECK-NEXT:     }) as i32)
// CHECK-NEXT:         > (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT:     0
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
