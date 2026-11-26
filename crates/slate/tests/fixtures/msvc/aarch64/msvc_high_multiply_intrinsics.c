// { dg-options "/Zc:preprocessor" }
#include <intrin.h>
#include <stdint.h>

static __int64 signed_high(__int64 a, __int64 b) {
  return __mulh(a, b);
}

static unsigned __int64 unsigned_high(unsigned __int64 a, unsigned __int64 b) {
  return __umulh(a, b);
}

int main(void) {
  return signed_high(INT64_MIN, 3) != -2 ||
         unsigned_high(UINT64_MAX, UINT64_MAX) != UINT64_MAX - 1;
}

// SLATE-FILECHECK-BEGIN aarch64-pc-windows-msvc
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-options "/Zc:preprocessor" }
// CHECK-NEXT: mod __slate_oldnames {
// CHECK-NEXT:     #[link(name = "oldnames")]
// CHECK-NEXT:     unsafe extern "C" {}
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: use slate_support::__umulh;
// CHECK-NEXT: use slate_support::__mulh;
// CHECK-EMPTY:
// CHECK-NEXT: fn signed_high(a: i64, b: i64) -> i64 {
// CHECK-NEXT:     unsafe { __mulh(a as i64, b as i64) as i64 }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn unsigned_high(a: u64, b: u64) -> u64 {
// CHECK-NEXT:     unsafe { __umulh(a as u64, b as u64) as u64 }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     (signed_high(-9223372036854775807i64 - 1i64, 3i64) != -2
// CHECK-NEXT:         || unsigned_high(18446744073709551615u64, 18446744073709551615u64)
// CHECK-NEXT:             != 18446744073709551615u64.wrapping_sub(1u64)) as i32
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
