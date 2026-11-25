// { dg-options "/Zc:preprocessor" }
#include <intrin.h>
#include <stdint.h>

static int check_signed(__int64 a, __int64 b, __int64 expected_high,
                        __int64 expected_low) {
  __int64 high;
  __int64 low = _mul128(a, b, &high);
  if (low != expected_low || high != expected_high ||
      __mulh(a, b) != expected_high)
    return 1;

  high = ~expected_high;
  _mul128(a, b, &high);
  return high != expected_high;
}

static int check_unsigned(unsigned __int64 a, unsigned __int64 b,
                          unsigned __int64 expected_high) {
  return __umulh(a, b) != expected_high;
}

int main(void) {
  if (check_signed(INT64_MIN, 3, -2, INT64_MIN))
    return 1;
  if (check_signed(INT64_MIN, -1, 0, INT64_MIN))
    return 2;
  if (check_signed(INT64_MIN, INT64_MIN, 1LL << 62, 0))
    return 3;
  if (check_unsigned(UINT64_MAX, UINT64_MAX, UINT64_MAX - 1) ||
      check_unsigned(1ULL << 63, 2, 1))
    return 4;
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-pc-windows-msvc
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
// CHECK-NEXT: use slate_support::_mul128;
// CHECK-EMPTY:
// CHECK-NEXT: fn check_signed(a: i64, b: i64, expected_high: i64, expected_low: i64) -> i32 {
// CHECK-NEXT:     let mut high: i64 = 0;
// CHECK-NEXT:     let low: i64 = unsafe {
// CHECK-NEXT:         _mul128(a as i64, b as i64, core::ptr::addr_of_mut!(high) as *mut i64) as i64
// CHECK-NEXT:     };
// CHECK-NEXT:     if low != expected_low || high != expected_high
// CHECK-NEXT:         || (unsafe { __mulh(a as i64, b as i64) as i64 }) != expected_high {
// CHECK-NEXT:         return 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     high = !expected_high;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         _mul128(a as i64, b as i64, core::ptr::addr_of_mut!(high) as *mut i64) as i64
// CHECK-NEXT:     };
// CHECK-NEXT:     (high != expected_high) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn check_unsigned(a: u64, b: u64, expected_high: u64) -> i32 {
// CHECK-NEXT:     ((unsafe { __umulh(a as u64, b as u64) as u64 }) != expected_high) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if check_signed(
// CHECK-NEXT:         -9223372036854775807i64 - 1i64,
// CHECK-NEXT:         3i64,
// CHECK-NEXT:         -2i64,
// CHECK-NEXT:         -9223372036854775807i64 - 1i64,
// CHECK-NEXT:     ) != 0 {
// CHECK-NEXT:         return 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     if check_signed(
// CHECK-NEXT:         -9223372036854775807i64 - 1i64,
// CHECK-NEXT:         -1i64,
// CHECK-NEXT:         0i64,
// CHECK-NEXT:         -9223372036854775807i64 - 1i64,
// CHECK-NEXT:     ) != 0 {
// CHECK-NEXT:         return 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     if check_signed(
// CHECK-NEXT:         -9223372036854775807i64 - 1i64,
// CHECK-NEXT:         -9223372036854775807i64 - 1i64,
// CHECK-NEXT:         1i64 << 62i32,
// CHECK-NEXT:         0i64,
// CHECK-NEXT:     ) != 0 {
// CHECK-NEXT:         return 3;
// CHECK-NEXT:     }
// CHECK-NEXT:     if check_unsigned(
// CHECK-NEXT:         18446744073709551615u64,
// CHECK-NEXT:         18446744073709551615u64,
// CHECK-NEXT:         18446744073709551615u64.wrapping_sub(1u64),
// CHECK-NEXT:     ) != 0 || check_unsigned(1u64 << 63i32, 2u64, 1u64) != 0 {
// CHECK-NEXT:         return 4;
// CHECK-NEXT:     }
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
