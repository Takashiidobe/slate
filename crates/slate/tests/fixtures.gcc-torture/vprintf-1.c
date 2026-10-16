/* { dg-skip-if "requires io" { freestanding } }  */
/* { dg-xfail-run-if {unexpected PTX 'vprintf' return value} { nvptx-*-* } } */

#ifndef test
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void inner(int x, ...) {
  va_list ap, ap2;
  va_start(ap, x);
  va_start(ap2, x);

  switch (x) {
#define test(n, ret, fmt, args)                                                \
  case n:                                                                      \
    vprintf(fmt, ap);                                                          \
    if (vprintf(fmt, ap2) != ret)                                              \
      abort();                                                                 \
    break;
#include "vprintf-1.c"
#undef test
  default:
    abort();
  }

  va_end(ap);
  va_end(ap2);
}

int main(void) {
#define test(n, ret, fmt, args) inner args;
#include "vprintf-1.c"
#undef test
  return 0;
}

#else
test(0, 5, "hello", (0));
test(1, 6, "hello\n", (1));
test(2, 1, "a", (2));
test(3, 0, "", (3));
test(4, 5, "%s", (4, "hello"));
test(5, 6, "%s", (5, "hello\n"));
test(6, 1, "%s", (6, "a"));
test(7, 0, "%s", (7, ""));
test(8, 1, "%c", (8, 'x'));
test(9, 7, "%s\n", (9, "hello\n"));
test(10, 2, "%d\n", (10, 0));
#endif

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-skip-if "requires io" { freestanding } }
// CHECK-NEXT: //!
// CHECK-NEXT: //! { dg-xfail-run-if {unexpected PTX 'vprintf' return value} { nvptx-*-* } }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn vprintf(__format: *const i8, __arg: core::ffi::VaList<'_>) -> i32;
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C-unwind" fn inner(mut x: i32, mut __va_args: ...) {
// CHECK-NEXT:     let mut ap: core::ffi::VaList<'_> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut ap2: core::ffi::VaList<'_> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     ap = __va_args.clone();
// CHECK-NEXT:     ap2 = __va_args.clone();
// CHECK-NEXT:     let mut [[T_0:__t[0-9]+]]: i64 = match x {
// CHECK-NEXT:         0 => 0,
// CHECK-NEXT:         1 => 1,
// CHECK-NEXT:         2 => 2,
// CHECK-NEXT:         3 => 3,
// CHECK-NEXT:         4 => 4,
// CHECK-NEXT:         5 => 5,
// CHECK-NEXT:         6 => 6,
// CHECK-NEXT:         7 => 7,
// CHECK-NEXT:         8 => 8,
// CHECK-NEXT:         9 => 9,
// CHECK-NEXT:         10 => 10,
// CHECK-NEXT:         _ => 11,
// CHECK-NEXT:     };
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         match [[T_0]] {
// CHECK-NEXT:             0 => {
// CHECK-NEXT:                 // { dg-skip-if "requires io" { freestanding } }
// CHECK-NEXT:                 //
// CHECK-NEXT:                 // { dg-xfail-run-if {unexpected PTX 'vprintf' return value} { nvptx-*-* } }
// CHECK-NEXT:                 let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:                 unsafe { vprintf(c"hello".as_ptr() as *const i8, ap.clone()) };
// CHECK-NEXT:                 unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:                 if (unsafe { vprintf(c"hello".as_ptr() as *const i8, ap2.clone()) }) != (5 as i32) {
// CHECK-NEXT:                     unsafe { abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 break '[[SLATE_BREAK_0]];
// CHECK-NEXT:                 {}
// CHECK-NEXT:                 [[T_0]] = 1;
// CHECK-NEXT:                 continue '[[SLATE_BREAK_0]];
// CHECK-NEXT:             }
// CHECK-NEXT:             1 => {
// CHECK-NEXT:                 let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:                 unsafe { vprintf(c"hello\n".as_ptr() as *const i8, ap.clone()) };
// CHECK-NEXT:                 unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:                 if (unsafe { vprintf(c"hello\n".as_ptr() as *const i8, ap2.clone()) }) != (6 as i32)
// CHECK-NEXT:                 {
// CHECK-NEXT:                     unsafe { abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 break '[[SLATE_BREAK_0]];
// CHECK-NEXT:                 {}
// CHECK-NEXT:                 [[T_0]] = 2;
// CHECK-NEXT:                 continue '[[SLATE_BREAK_0]];
// CHECK-NEXT:             }
// CHECK-NEXT:             2 => {
// CHECK-NEXT:                 let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:                 unsafe { vprintf(c"a".as_ptr() as *const i8, ap.clone()) };
// CHECK-NEXT:                 unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:                 if (unsafe { vprintf(c"a".as_ptr() as *const i8, ap2.clone()) }) != (1 as i32) {
// CHECK-NEXT:                     unsafe { abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 break '[[SLATE_BREAK_0]];
// CHECK-NEXT:                 {}
// CHECK-NEXT:                 [[T_0]] = 3;
// CHECK-NEXT:                 continue '[[SLATE_BREAK_0]];
// CHECK-NEXT:             }
// CHECK-NEXT:             3 => {
// CHECK-NEXT:                 let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:                 unsafe { vprintf(c"".as_ptr() as *const i8, ap.clone()) };
// CHECK-NEXT:                 unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:                 if (unsafe { vprintf(c"".as_ptr() as *const i8, ap2.clone()) }) != (0 as i32) {
// CHECK-NEXT:                     unsafe { abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 break '[[SLATE_BREAK_0]];
// CHECK-NEXT:                 {}
// CHECK-NEXT:                 [[T_0]] = 4;
// CHECK-NEXT:                 continue '[[SLATE_BREAK_0]];
// CHECK-NEXT:             }
// CHECK-NEXT:             4 => {
// CHECK-NEXT:                 let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:                 unsafe { vprintf(c"%s".as_ptr() as *const i8, ap.clone()) };
// CHECK-NEXT:                 unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:                 if (unsafe { vprintf(c"%s".as_ptr() as *const i8, ap2.clone()) }) != (5 as i32) {
// CHECK-NEXT:                     unsafe { abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 break '[[SLATE_BREAK_0]];
// CHECK-NEXT:                 {}
// CHECK-NEXT:                 [[T_0]] = 5;
// CHECK-NEXT:                 continue '[[SLATE_BREAK_0]];
// CHECK-NEXT:             }
// CHECK-NEXT:             5 => {
// CHECK-NEXT:                 let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:                 unsafe { vprintf(c"%s".as_ptr() as *const i8, ap.clone()) };
// CHECK-NEXT:                 unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:                 if (unsafe { vprintf(c"%s".as_ptr() as *const i8, ap2.clone()) }) != (6 as i32) {
// CHECK-NEXT:                     unsafe { abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 break '[[SLATE_BREAK_0]];
// CHECK-NEXT:                 {}
// CHECK-NEXT:                 [[T_0]] = 6;
// CHECK-NEXT:                 continue '[[SLATE_BREAK_0]];
// CHECK-NEXT:             }
// CHECK-NEXT:             6 => {
// CHECK-NEXT:                 let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:                 unsafe { vprintf(c"%s".as_ptr() as *const i8, ap.clone()) };
// CHECK-NEXT:                 unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:                 if (unsafe { vprintf(c"%s".as_ptr() as *const i8, ap2.clone()) }) != (1 as i32) {
// CHECK-NEXT:                     unsafe { abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 break '[[SLATE_BREAK_0]];
// CHECK-NEXT:                 {}
// CHECK-NEXT:                 [[T_0]] = 7;
// CHECK-NEXT:                 continue '[[SLATE_BREAK_0]];
// CHECK-NEXT:             }
// CHECK-NEXT:             7 => {
// CHECK-NEXT:                 let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:                 unsafe { vprintf(c"%s".as_ptr() as *const i8, ap.clone()) };
// CHECK-NEXT:                 unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:                 if (unsafe { vprintf(c"%s".as_ptr() as *const i8, ap2.clone()) }) != (0 as i32) {
// CHECK-NEXT:                     unsafe { abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 break '[[SLATE_BREAK_0]];
// CHECK-NEXT:                 {}
// CHECK-NEXT:                 [[T_0]] = 8;
// CHECK-NEXT:                 continue '[[SLATE_BREAK_0]];
// CHECK-NEXT:             }
// CHECK-NEXT:             8 => {
// CHECK-NEXT:                 let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:                 unsafe { vprintf(c"%c".as_ptr() as *const i8, ap.clone()) };
// CHECK-NEXT:                 unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:                 if (unsafe { vprintf(c"%c".as_ptr() as *const i8, ap2.clone()) }) != (1 as i32) {
// CHECK-NEXT:                     unsafe { abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 break '[[SLATE_BREAK_0]];
// CHECK-NEXT:                 {}
// CHECK-NEXT:                 [[T_0]] = 9;
// CHECK-NEXT:                 continue '[[SLATE_BREAK_0]];
// CHECK-NEXT:             }
// CHECK-NEXT:             9 => {
// CHECK-NEXT:                 let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:                 unsafe { vprintf(c"%s\n".as_ptr() as *const i8, ap.clone()) };
// CHECK-NEXT:                 unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:                 if (unsafe { vprintf(c"%s\n".as_ptr() as *const i8, ap2.clone()) }) != (7 as i32) {
// CHECK-NEXT:                     unsafe { abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 break '[[SLATE_BREAK_0]];
// CHECK-NEXT:                 {}
// CHECK-NEXT:                 [[T_0]] = 10;
// CHECK-NEXT:                 continue '[[SLATE_BREAK_0]];
// CHECK-NEXT:             }
// CHECK-NEXT:             10 => {
// CHECK-NEXT:                 let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:                 unsafe { vprintf(c"%d\n".as_ptr() as *const i8, ap.clone()) };
// CHECK-NEXT:                 unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:                 if (unsafe { vprintf(c"%d\n".as_ptr() as *const i8, ap2.clone()) }) != (2 as i32) {
// CHECK-NEXT:                     unsafe { abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 break '[[SLATE_BREAK_0]];
// CHECK-NEXT:                 {}
// CHECK-NEXT:                 [[T_0]] = 11;
// CHECK-NEXT:                 continue '[[SLATE_BREAK_0]];
// CHECK-NEXT:             }
// CHECK-NEXT:             11 => {
// CHECK-NEXT:                 unsafe { abort() };
// CHECK-NEXT:                 break '[[SLATE_BREAK_0]];
// CHECK-NEXT:             }
// CHECK-NEXT:             _ => {
// CHECK-NEXT:                 break '[[SLATE_BREAK_0]];
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     {}
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     // { dg-skip-if "requires io" { freestanding } }
// CHECK-NEXT:     //
// CHECK-NEXT:     // { dg-xfail-run-if {unexpected PTX 'vprintf' return value} { nvptx-*-* } }
// CHECK-NEXT:     unsafe { inner(0 as i32) };
// CHECK-NEXT:     {}
// CHECK-NEXT:     unsafe { inner(1 as i32) };
// CHECK-NEXT:     {}
// CHECK-NEXT:     unsafe { inner(2 as i32) };
// CHECK-NEXT:     {}
// CHECK-NEXT:     unsafe { inner(3 as i32) };
// CHECK-NEXT:     {}
// CHECK-NEXT:     unsafe { inner(4 as i32, c"hello".as_ptr() as *mut i8) };
// CHECK-NEXT:     {}
// CHECK-NEXT:     unsafe { inner(5 as i32, c"hello\n".as_ptr() as *mut i8) };
// CHECK-NEXT:     {}
// CHECK-NEXT:     unsafe { inner(6 as i32, c"a".as_ptr() as *mut i8) };
// CHECK-NEXT:     {}
// CHECK-NEXT:     unsafe { inner(7 as i32, c"".as_ptr() as *mut i8) };
// CHECK-NEXT:     {}
// CHECK-NEXT:     unsafe { inner(8 as i32, 120 as i32) };
// CHECK-NEXT:     {}
// CHECK-NEXT:     unsafe { inner(9 as i32, c"hello\n".as_ptr() as *mut i8) };
// CHECK-NEXT:     {}
// CHECK-NEXT:     unsafe { inner(10 as i32, 0 as i32) };
// CHECK-NEXT:     {}
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
