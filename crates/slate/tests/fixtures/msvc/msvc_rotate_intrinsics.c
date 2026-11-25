// { dg-options "/Zc:preprocessor" }
#include <intrin.h>
#include <limits.h>
#include <stdlib.h>

static int check8(unsigned char value, unsigned char count,
                  unsigned char left, unsigned char right) {
  return _rotl8(value, count) != left || _rotr8(value, count) != right;
}

static int check16(unsigned short value, unsigned char count,
                   unsigned short left, unsigned short right) {
  return _rotl16(value, count) != left || _rotr16(value, count) != right;
}

static int check32(unsigned value, int count, unsigned left, unsigned right) {
  return _rotl(value, count) != left || _rotr(value, count) != right;
}

static int check64(unsigned __int64 value, int count,
                   unsigned __int64 left, unsigned __int64 right) {
  return _rotl64(value, count) != left || _rotr64(value, count) != right;
}

static int check_indirect(void) {
  unsigned (*volatile left)(unsigned, int) = &_rotl;
  unsigned __int64 (*volatile right)(unsigned __int64, int) = &_rotr64;
  return left(0x80000003U, 33) != 7 ||
         right(0x8000000000000003ULL, 65) != 0xc000000000000001ULL;
}

int main(void) {
  unsigned char v8 = 0x83;
  unsigned short v16 = 0x8003;
  unsigned v32 = 0x80000003U;
  unsigned __int64 v64 = 0x8000000000000003ULL;
  if (check8(v8, 0, v8, v8) || check8(v8, 8, v8, v8) ||
      check8(v8, 7, 0xc1, 7) || check8(v8, 9, 7, 0xc1) ||
      check8(v8, UCHAR_MAX, 0xc1, 7))
    return 1;
  if (check16(v16, 0, v16, v16) || check16(v16, 16, v16, v16) ||
      check16(v16, 15, 0xc001, 7) || check16(v16, 17, 7, 0xc001) ||
      check16(v16, UCHAR_MAX, 0xc001, 7))
    return 2;
  if (check32(v32, 0, v32, v32) || check32(v32, 32, v32, v32) ||
      check32(v32, 31, 0xc0000001U, 7) || check32(v32, 33, 7, 0xc0000001U) ||
      check32(v32, -1, 0xc0000001U, 7) || check32(v32, INT_MIN, v32, v32) ||
      check32(v32, INT_MAX, 0xc0000001U, 7))
    return 3;
  if (check64(v64, 0, v64, v64) || check64(v64, 64, v64, v64) ||
      check64(v64, 63, 0xc000000000000001ULL, 7) ||
      check64(v64, 65, 7, 0xc000000000000001ULL) ||
      check64(v64, -1, 0xc000000000000001ULL, 7) ||
      check64(v64, INT_MIN, v64, v64) ||
      check64(v64, INT_MAX, 0xc000000000000001ULL, 7))
    return 4;
  return check_indirect() ? 5 : 0;
}

// SLATE-FILECHECK-BEGIN x86_64-pc-windows-msvc i686-pc-windows-msvc aarch64-pc-windows-msvc
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-options "/Zc:preprocessor" }
// CHECK-NEXT: mod __slate_oldnames {
// CHECK-NEXT:     #[link(name = "oldnames")]
// CHECK-NEXT:     unsafe extern "C" {}
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn _rotl([[ARG_0:arg[0-9]+]]: u32, [[ARG_1:arg[0-9]+]]: i32) -> u32;
// CHECK-NEXT:     fn _rotl16([[ARG_0]]: u16, [[ARG_1]]: u8) -> u16;
// CHECK-NEXT:     fn _rotl64([[ARG_0]]: u64, [[ARG_1]]: i32) -> u64;
// CHECK-NEXT:     fn _rotl8([[ARG_0]]: u8, [[ARG_1]]: u8) -> u8;
// CHECK-NEXT:     fn _rotr([[ARG_0]]: u32, [[ARG_1]]: i32) -> u32;
// CHECK-NEXT:     fn _rotr16([[ARG_0]]: u16, [[ARG_1]]: u8) -> u16;
// CHECK-NEXT:     fn _rotr64([[ARG_0]]: u64, [[ARG_1]]: i32) -> u64;
// CHECK-NEXT:     fn _rotr8([[ARG_0]]: u8, [[ARG_1]]: u8) -> u8;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn check8(value: u8, count: u8, left: u8, right: u8) -> i32 {
// CHECK-NEXT:     ((value.rotate_left(count as u32) as i32) != (left as i32)
// CHECK-NEXT:         || (value.rotate_right(count as u32) as i32) != (right as i32)) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn check16(value: u16, count: u8, left: u16, right: u16) -> i32 {
// CHECK-NEXT:     ((value.rotate_left(count as u32) as i32) != (left as i32)
// CHECK-NEXT:         || (value.rotate_right(count as u32) as i32) != (right as i32)) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn check32(value: u32, count: i32, left: u32, right: u32) -> i32 {
// CHECK-NEXT:     (value.rotate_left(count as u32) != left
// CHECK-NEXT:         || value.rotate_right(count as u32) != right) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn check64(value: u64, count: i32, left: u64, right: u64) -> i32 {
// CHECK-NEXT:     (value.rotate_left(count as u32) != left
// CHECK-NEXT:         || value.rotate_right(count as u32) != right) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn check_indirect() -> i32 {
// CHECK-NEXT:     let left: Option<unsafe extern "C-unwind" fn(u32, i32) -> u32> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<*const (), Option<unsafe extern "C-unwind" fn(u32, i32) -> u32>>(
// CHECK-NEXT:             _rotl as *const (),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let right: Option<unsafe extern "C-unwind" fn(u64, i32) -> u64> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<*const (), Option<unsafe extern "C-unwind" fn(u64, i32) -> u64>>(
// CHECK-NEXT:             _rotr64 as *const (),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     ((unsafe {
// CHECK-NEXT:         core::ptr::read_volatile(core::ptr::addr_of!(left))
// CHECK-NEXT:             .unwrap()(2147483651u32, 33i32)
// CHECK-NEXT:     }) != 7
// CHECK-NEXT:         || (unsafe {
// CHECK-NEXT:             core::ptr::read_volatile(core::ptr::addr_of!(right))
// CHECK-NEXT:                 .unwrap()(9223372036854775811u64, 65i32)
// CHECK-NEXT:         }) != 13835058055282163713) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let v8: u8 = 131;
// CHECK-NEXT:     let v16: u16 = 32771;
// CHECK-NEXT:     let v32: u32 = 2147483651;
// CHECK-NEXT:     let v64: u64 = 9223372036854775811;
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: bool = check8(v8, 0u8, v8, v8) != 0 || check8(v8, 8u8, v8, v8) != 0;
// CHECK-NEXT:     let [[V_1:__v[0-9]+]]: bool = [[V_0]] || check8(v8, 7u8, 193u8, 7u8) != 0;
// CHECK-NEXT:     let [[V_2:__v[0-9]+]]: bool = [[V_1]] || check8(v8, 9u8, 7u8, 193u8) != 0;
// CHECK-NEXT:     if [[V_2]] || check8(v8, 255u8, 193u8, 7u8) != 0 {
// CHECK-NEXT:         return 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     let [[V_3:__v[0-9]+]]: bool = check16(v16, 0u8, v16, v16) != 0
// CHECK-NEXT:         || check16(v16, 16u8, v16, v16) != 0;
// CHECK-NEXT:     let [[V_4:__v[0-9]+]]: bool = [[V_3]] || check16(v16, 15u8, 49153u16, 7u16) != 0;
// CHECK-NEXT:     let [[V_5:__v[0-9]+]]: bool = [[V_4]] || check16(v16, 17u8, 7u16, 49153u16) != 0;
// CHECK-NEXT:     if [[V_5]] || check16(v16, 255u8, 49153u16, 7u16) != 0 {
// CHECK-NEXT:         return 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     let [[V_6:__v[0-9]+]]: bool = check32(v32, 0i32, v32, v32) != 0
// CHECK-NEXT:         || check32(v32, 32i32, v32, v32) != 0;
// CHECK-NEXT:     let [[V_7:__v[0-9]+]]: bool = [[V_6]] || check32(v32, 31i32, 3221225473u32, 7u32) != 0;
// CHECK-NEXT:     let [[V_8:__v[0-9]+]]: bool = [[V_7]] || check32(v32, 33i32, 7u32, 3221225473u32) != 0;
// CHECK-NEXT:     let [[V_9:__v[0-9]+]]: bool = [[V_8]] || check32(v32, -1i32, 3221225473u32, 7u32) != 0;
// CHECK-NEXT:     let [[V_10:__v[0-9]+]]: bool = [[V_9]] || check32(v32, -2147483647i32 - 1i32, v32, v32) != 0;
// CHECK-NEXT:     if [[V_10]] || check32(v32, 2147483647i32, 3221225473u32, 7u32) != 0 {
// CHECK-NEXT:         return 3;
// CHECK-NEXT:     }
// CHECK-NEXT:     let [[V_11:__v[0-9]+]]: bool = check64(v64, 0i32, v64, v64) != 0
// CHECK-NEXT:         || check64(v64, 64i32, v64, v64) != 0;
// CHECK-NEXT:     let [[V_12:__v[0-9]+]]: bool = [[V_11]] || check64(v64, 63i32, 13835058055282163713u64, 7u64) != 0;
// CHECK-NEXT:     let [[V_13:__v[0-9]+]]: bool = [[V_12]] || check64(v64, 65i32, 7u64, 13835058055282163713u64) != 0;
// CHECK-NEXT:     let [[V_14:__v[0-9]+]]: bool = [[V_13]] || check64(v64, -1i32, 13835058055282163713u64, 7u64) != 0;
// CHECK-NEXT:     let [[V_15:__v[0-9]+]]: bool = [[V_14]] || check64(v64, -2147483647i32 - 1i32, v64, v64) != 0;
// CHECK-NEXT:     if [[V_15]] || check64(v64, 2147483647i32, 13835058055282163713u64, 7u64) != 0 {
// CHECK-NEXT:         return 4;
// CHECK-NEXT:     }
// CHECK-NEXT:     if check_indirect() != 0 { 5 } else { 0 }
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
