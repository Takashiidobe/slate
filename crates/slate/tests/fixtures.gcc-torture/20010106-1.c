/* Copyright 2001 Free Software Foundation
   Contributed by Alexandre Oliva <aoliva@redhat.com> */

void abort(void);
void exit(int);

int f(int i) {
  switch (i) {
  case -2:
    return 33;
  case -1:
    return 0;
  case 0:
    return 7;
  case 1:
    return 4;
  case 2:
    return 3;
  case 3:
    return 15;
  case 4:
    return 9;
  default:
    abort();
  }
}

int main() {
  if (f(-1))
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Copyright 2001 Free Software Foundation
// CHECK-NEXT: //! Contributed by Alexandre Oliva <aoliva@redhat.com>
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f(mut i: i32) -> i32 {
// CHECK-NEXT:     match i {
// CHECK-NEXT:         -2 => {
// CHECK-NEXT:             return 33 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         -1 => {
// CHECK-NEXT:             return 0 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         0 => {
// CHECK-NEXT:             return 7 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         1 => {
// CHECK-NEXT:             return 4 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         2 => {
// CHECK-NEXT:             return 3 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         3 => {
// CHECK-NEXT:             return 15 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         4 => {
// CHECK-NEXT:             return 9 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         _ => {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if f(-(1 as i32)) != (0 as i32) {
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
