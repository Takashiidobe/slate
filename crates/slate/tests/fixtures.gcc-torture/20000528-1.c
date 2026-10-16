/* Copyright (C) 2000  Free Software Foundation  */
/* Contributed by Alexandre Oliva <aoliva@cygnus.com> */

void abort(void);
void exit(int);

unsigned long  l = (unsigned long)-2;
unsigned short s;

int main() {
  long t = l;
  s      = t;
  if (s != (unsigned short)-2)
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Copyright (C) 2000  Free Software Foundation
// CHECK-NEXT: //!
// CHECK-NEXT: //! Contributed by Alexandre Oliva <aoliva@cygnus.com>
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut l: u64 = (-(2 as i32) as i64) as u64;
// CHECK-EMPTY:
// CHECK-NEXT: static mut s: u16 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut t: i64 = (unsafe { l }) as i64;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         s = (t as i16) as u16;
// CHECK-NEXT:     }
// CHECK-NEXT:     if (((unsafe { s }) as u32) as i32) != ((((-(2 as i32) as i16) as u16) as u32) as i32) {
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
