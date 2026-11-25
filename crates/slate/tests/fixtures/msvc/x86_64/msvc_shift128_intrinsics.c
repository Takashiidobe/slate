// { dg-options "/Zc:preprocessor" }
#include <intrin.h>

#pragma intrinsic(__shiftleft128, __shiftright128)

static unsigned __int64 shift_left(unsigned __int64 low, unsigned __int64 high,
                                   unsigned char count) {
  return __shiftleft128(low, high, count);
}

static unsigned __int64 shift_right(unsigned __int64 low, unsigned __int64 high,
                                    unsigned char count) {
  return __shiftright128(low, high, count);
}

int main(void) {
  unsigned __int64 low = 0x8000000000000002ULL;
  unsigned __int64 high = 0x4000000000000001ULL;
  return shift_left(low, high, 64) != high || shift_right(low, high, 64) != low ||
         shift_left(low, high, 65) != 0x8000000000000003ULL ||
         shift_right(low, high, 65) != 0xc000000000000001ULL;
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
// CHECK-NEXT: fn shift_left(low: u64, high: u64, count: u8) -> u64 {
// CHECK-NEXT:     (((low as u128) | (high as u128) << 64) << (count & 63) >> 64) as u64
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn shift_right(low: u64, high: u64, count: u8) -> u64 {
// CHECK-NEXT:     (((low as u128) | (high as u128) << 64) >> (count & 63)) as u64
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let low: u64 = 9223372036854775810;
// CHECK-NEXT:     let high: u64 = 4611686018427387905;
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: bool = shift_left(low, high, 64u8) != high
// CHECK-NEXT:         || shift_right(low, high, 64u8) != low;
// CHECK-NEXT:     let [[V_1:__v[0-9]+]]: bool = [[V_0]] || shift_left(low, high, 65u8) != 9223372036854775811;
// CHECK-NEXT:     ([[V_1]] || shift_right(low, high, 65u8) != 13835058055282163713) as i32
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
