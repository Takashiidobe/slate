/* PR target/12654
   The Alpha backend tried to do a >= 1024 as (a - 1024) >= 0, which fails
   for very large negative values.  */
/* Origin: tg@swox.com  */

#include <limits.h>

extern void abort(void);

void __attribute__((noinline)) foo(long x) {
  if (x >= 1024)
    abort();
}

int main() {
  foo(LONG_MIN);
  foo(LONG_MIN + 10000);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR target/12654
// CHECK-NEXT: //! The Alpha backend tried to do a >= 1024 as (a - 1024) >= 0, which fails
// CHECK-NEXT: //! for very large negative values.
// CHECK-NEXT: //!
// CHECK-NEXT: //! Origin: tg@swox.com
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: i64) {
// CHECK-NEXT:     if x >= ((1024 as i32) as i64) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     foo(-(9223372036854775807 as i64) - (1 as i64));
// CHECK-NEXT:     foo(-(9223372036854775807 as i64) - (1 as i64) + ((10000 as i32) as i64));
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
