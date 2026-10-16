/* { dg-skip-if "requires io" { freestanding } }  */
/* { dg-xfail-run-if {unexpected PTX 'vprintf' return value} { nvptx-*-* } } */

#include <stdio.h>
#include <stdlib.h>

int main(void) {
#define test(ret, args...)                                                     \
  printf(args);                                                                \
  if (printf(args) != ret)                                                     \
    abort();
  test(5, "hello");
  test(6, "hello\n");
  test(1, "a");
  test(0, "");
  test(5, "%s", "hello");
  test(6, "%s", "hello\n");
  test(1, "%s", "a");
  test(0, "%s", "");
  test(1, "%c", 'x');
  test(7, "%s\n", "hello\n");
  test(2, "%d\n", 0);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-skip-if "requires io" { freestanding } }
// CHECK-NEXT: //!
// CHECK-NEXT: //! { dg-xfail-run-if {unexpected PTX 'vprintf' return value} { nvptx-*-* } }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe { printf(c"hello".as_ptr() as *const i8) };
// CHECK-NEXT:     if (unsafe { printf(c"hello".as_ptr() as *const i8) }) != (5 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     unsafe { printf(c"hello\n".as_ptr() as *const i8) };
// CHECK-NEXT:     if (unsafe { printf(c"hello\n".as_ptr() as *const i8) }) != (6 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     unsafe { printf(c"a".as_ptr() as *const i8) };
// CHECK-NEXT:     if (unsafe { printf(c"a".as_ptr() as *const i8) }) != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     unsafe { printf(c"".as_ptr() as *const i8) };
// CHECK-NEXT:     if (unsafe { printf(c"".as_ptr() as *const i8) }) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     unsafe { printf(c"%s".as_ptr() as *const i8, c"hello".as_ptr() as *mut i8) };
// CHECK-NEXT:     if (unsafe { printf(c"%s".as_ptr() as *const i8, c"hello".as_ptr() as *mut i8) }) != (5 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     unsafe { printf(c"%s".as_ptr() as *const i8, c"hello\n".as_ptr() as *mut i8) };
// CHECK-NEXT:     if (unsafe { printf(c"%s".as_ptr() as *const i8, c"hello\n".as_ptr() as *mut i8) })
// CHECK-NEXT:         != (6 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     unsafe { printf(c"%s".as_ptr() as *const i8, c"a".as_ptr() as *mut i8) };
// CHECK-NEXT:     if (unsafe { printf(c"%s".as_ptr() as *const i8, c"a".as_ptr() as *mut i8) }) != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     unsafe { printf(c"%s".as_ptr() as *const i8, c"".as_ptr() as *mut i8) };
// CHECK-NEXT:     if (unsafe { printf(c"%s".as_ptr() as *const i8, c"".as_ptr() as *mut i8) }) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     unsafe { printf(c"%c".as_ptr() as *const i8, 120 as i32) };
// CHECK-NEXT:     if (unsafe { printf(c"%c".as_ptr() as *const i8, 120 as i32) }) != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%s\n".as_ptr() as *const i8,
// CHECK-NEXT:             c"hello\n".as_ptr() as *mut i8,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%s\n".as_ptr() as *const i8,
// CHECK-NEXT:             c"hello\n".as_ptr() as *mut i8,
// CHECK-NEXT:         )
// CHECK-NEXT:     }) != (7 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     unsafe { printf(c"%d\n".as_ptr() as *const i8, 0 as i32) };
// CHECK-NEXT:     if (unsafe { printf(c"%d\n".as_ptr() as *const i8, 0 as i32) }) != (2 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
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
