/* Copyright (C) 2000  Free Software Foundation  */
/* by Alexandre Oliva <aoliva@redhat.com> */

#include <stdlib.h>
#include <string.h>

char *list[] = {"*", "e"};

static int bar(const char *fmt) { return (strchr(fmt, '*') != 0); }

static void foo() {
  int i;
  for (i = 0; i < sizeof(list) / sizeof(*list); i++) {
    const char *fmt = list[i];
    if (bar(fmt))
      continue;
    if (i == 0)
      abort();
    else
      exit(0);
  }
}

int main() { foo(); }

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Copyright (C) 2000  Free Software Foundation
// CHECK-NEXT: //!
// CHECK-NEXT: //! by Alexandre Oliva <aoliva@redhat.com>
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit(__status: i32);
// CHECK-NEXT:     fn strchr(__s: *const i8, __c: i32) -> *mut i8;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: static mut list: __SlateAlign16<[*mut i8; 2]> =
// CHECK-NEXT:     __SlateAlign16([b"*\0".as_ptr() as *mut i8, b"e\0".as_ptr() as *mut i8]);
// CHECK-EMPTY:
// CHECK-NEXT: fn bar(mut fmt: *const i8) -> i32 {
// CHECK-NEXT:     (((unsafe { strchr(fmt, 42 as i32) }) as *const i8) != std::ptr::null::<i8>()) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo() {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while ((i as i64) as u64) < (16 as u64) / (8 as u64) {
// CHECK-NEXT:         let mut fmt: *const i8 = (unsafe {
// CHECK-NEXT:             *unsafe { unsafe { std::ptr::addr_of_mut!(list.0) as *mut *mut i8 }.offset(i as isize) }
// CHECK-NEXT:         }) as *const i8;
// CHECK-NEXT:         if bar(fmt) != (0 as i32) {
// CHECK-NEXT:         } else {
// CHECK-NEXT:             if i == (0 as i32) {
// CHECK-NEXT:                 unsafe { abort() };
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 unsafe { exit(0 as i32) };
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:         i = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     foo();
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
