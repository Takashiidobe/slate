/* { dg-options "-msseregparm" } */
#include <math.h>
#include <stdio.h>

static long long mix(int a, short b, long long c) { return a * 3 + b * 5 + c; }

int main(void) {
  long long total = 0;
  for (int i = 0; i < 10; i++)
    total += mix(i, (short)-i, 1000000000LL * i);
  printf("%lld\n", total);
  return 0;
}

// SLATE-FILECHECK-BEGIN i686-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-options "-msseregparm" }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn mix(a: i32, b: i16, c: i64) -> i64 {
// CHECK-NEXT:     ((a * 3 + (b as i32) * 5) as i64) + c
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut total: i64 = 0;
// CHECK-NEXT:     for i in 0i32..10i32 {
// CHECK-NEXT:         let [[V_0:__v[0-9]+]]: i64 = mix(i, -i as i16, 1000000000 * (i as i64));
// CHECK-NEXT:         total += [[V_0]];
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"%lld\n".as_ptr(), total) };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut core::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut core::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
