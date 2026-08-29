#include <stdint.h>
#include <stdio.h>
#include <string.h>

static long double load80(const unsigned char *bytes) {
  long double value;
  memcpy(&value, bytes, 10);
  return value;
}

static void dump80(const char *name, long double value) {
  unsigned char bytes[10];
  memcpy(bytes, &value, 10);
  printf("%s", name);
  for (int i = 0; i < 10; ++i)
    printf("%02x", bytes[i]);
  printf("\n");
}

int main(void) {
  unsigned char one_bits[10]  = {0, 0, 0, 0, 0, 0, 0, 0x80, 0xff, 0x3f};
  unsigned char two_bits[10]  = {0, 0, 0, 0, 0, 0, 0, 0x80, 0x00, 0x40};
  unsigned char near_bits[10] = {1, 0, 0, 0, 0, 0, 0, 0x80, 0xff, 0x3f};
  long double   one           = load80(one_bits);
  long double   two           = load80(two_bits);
  long double   near          = load80(near_bits);

  dump80("add", one + two);
  dump80("sub", near - one);
  dump80("mul", one * two);
  dump80("div", two / one);
  dump80("neg", -near);

  printf("%d %d %d %d\n", one<two, near> one, one == one, near != one);
  printf("%d\n", (int)(one + two));
  dump80("from_i64", (long double)1234567890123LL);
  printf("%d\n", (int)(long double)123456789);
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C, align(16))]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct LongDouble([u8; 10]);
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::ops::Add for LongDouble {
// LOWERING-NEXT:     type Output = LongDouble;
// LOWERING-NEXT:     fn add(self, o: LongDouble) -> LongDouble { __slate_f80_add(self, o) }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::ops::Sub for LongDouble {
// LOWERING-NEXT:     type Output = LongDouble;
// LOWERING-NEXT:     fn sub(self, o: LongDouble) -> LongDouble { __slate_f80_sub(self, o) }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::ops::Mul for LongDouble {
// LOWERING-NEXT:     type Output = LongDouble;
// LOWERING-NEXT:     fn mul(self, o: LongDouble) -> LongDouble { __slate_f80_mul(self, o) }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::ops::Div for LongDouble {
// LOWERING-NEXT:     type Output = LongDouble;
// LOWERING-NEXT:     fn div(self, o: LongDouble) -> LongDouble { __slate_f80_div(self, o) }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::ops::AddAssign for LongDouble {
// LOWERING-NEXT:     fn add_assign(&mut self, o: LongDouble) { {
// LOWERING-NEXT:     *self = __slate_f80_add(*self, o);
// LOWERING-NEXT: } }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::ops::SubAssign for LongDouble {
// LOWERING-NEXT:     fn sub_assign(&mut self, o: LongDouble) { {
// LOWERING-NEXT:     *self = __slate_f80_sub(*self, o);
// LOWERING-NEXT: } }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::ops::MulAssign for LongDouble {
// LOWERING-NEXT:     fn mul_assign(&mut self, o: LongDouble) { {
// LOWERING-NEXT:     *self = __slate_f80_mul(*self, o);
// LOWERING-NEXT: } }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::ops::DivAssign for LongDouble {
// LOWERING-NEXT:     fn div_assign(&mut self, o: LongDouble) { {
// LOWERING-NEXT:     *self = __slate_f80_div(*self, o);
// LOWERING-NEXT: } }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::ops::Neg for LongDouble {
// LOWERING-NEXT:     type Output = LongDouble;
// LOWERING-NEXT:     fn neg(self) -> LongDouble { __slate_f80_neg(self) }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::cmp::PartialEq for LongDouble {
// LOWERING-NEXT:     fn eq(&self, other: &LongDouble) -> bool { __slate_f80_eq(*self, *other) }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::cmp::PartialOrd for LongDouble {
// LOWERING-NEXT:     fn partial_cmp(&self, other: &LongDouble) -> Option<std::cmp::Ordering> { if __slate_f80_lt(*self, *other) { Some(std::cmp::Ordering::Less) } else { if __slate_f80_gt(*self, *other) { Some(std::cmp::Ordering::Greater) } else { if __slate_f80_eq(*self, *other) { Some(std::cmp::Ordering::Equal) } else { None } } } }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn memcpy(_0: *mut core::ffi::c_void, _1: *const core::ffi::c_void, _2: usize) -> *mut core::ffi::c_void;
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn load80(arg2: *mut u8) -> LongDouble {
// LOWERING-NEXT:     let mut bytes: *mut u8 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut __retval: LongDouble = LongDouble([0; 10]);
// LOWERING-NEXT:     let mut value: LongDouble = LongDouble([0; 10]);
// LOWERING-NEXT:     bytes = arg2;
// LOWERING-NEXT:     let _v0: *mut core::ffi::c_void = std::ptr::addr_of_mut!(value) as *mut core::ffi::c_void;
// LOWERING-NEXT:     let _v1: *mut u8 = bytes;
// LOWERING-NEXT:     let _v2: *mut core::ffi::c_void = _v1 as *mut core::ffi::c_void;
// LOWERING-NEXT:     let _v3: u64 = 10;
// LOWERING-NEXT:     let _v4: *mut core::ffi::c_void = unsafe { memcpy(_v0 as *mut core::ffi::c_void, _v2 as *const core::ffi::c_void, _v3 as usize) };
// LOWERING-NEXT:     let _v5: LongDouble = value;
// LOWERING-NEXT:     __retval = _v5;
// LOWERING-NEXT:     let _v6: LongDouble = __retval;
// LOWERING-NEXT:     return _v6;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn dump80(arg0: *mut i8, arg1: LongDouble) {
// LOWERING-NEXT:     let mut name: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut value: LongDouble = LongDouble([0; 10]);
// LOWERING-NEXT:     let mut bytes: [u8; 10] = [0; 10];
// LOWERING-NEXT:     name = arg0;
// LOWERING-NEXT:     value = arg1;
// LOWERING-NEXT:     let _v0: *mut u8 = bytes.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     let _v1: *mut core::ffi::c_void = _v0 as *mut core::ffi::c_void;
// LOWERING-NEXT:     let _v2: *mut core::ffi::c_void = std::ptr::addr_of_mut!(value) as *mut core::ffi::c_void;
// LOWERING-NEXT:     let _v3: u64 = 10;
// LOWERING-NEXT:     let _v4: *mut core::ffi::c_void = unsafe { memcpy(_v1 as *mut core::ffi::c_void, _v2 as *const core::ffi::c_void, _v3 as usize) };
// LOWERING-NEXT:     let _v5: *mut i8 = b"%s\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v6: *mut i8 = name;
// LOWERING-NEXT:     let _v7: i32 = unsafe { printf(_v5 as *const i8, _v6) };
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut i: i32 = 0;
// LOWERING-NEXT:         let _v8: i32 = 0;
// LOWERING-NEXT:         i = _v8;
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let _v9: i32 = i;
// LOWERING-NEXT:             let _v10: i32 = 10;
// LOWERING-NEXT:             let _v11: bool = _v9 < _v10;
// LOWERING-NEXT:             if !_v11 {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let _v12: *mut i8 = b"%02x\0".as_ptr() as *mut i8;
// LOWERING-NEXT:             let _v13: i32 = i;
// LOWERING-NEXT:             let _v14: i64 = _v13 as i64;
// LOWERING-NEXT:             let _v15: u8 = bytes[(_v14 as usize)];
// LOWERING-NEXT:             let _v16: i32 = _v15 as i32;
// LOWERING-NEXT:             let _v17: i32 = unsafe { printf(_v12 as *const i8, _v16) };
// LOWERING-NEXT:             let _v18: i32 = i;
// LOWERING-NEXT:             let _v19: i32 = _v18 + 1;
// LOWERING-NEXT:             i = _v19;
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v20: *mut i8 = b"\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v21: i32 = unsafe { printf(_v20 as *const i8) };
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut one_bits: [u8; 10] = [0; 10];
// LOWERING-NEXT:     let mut two_bits: [u8; 10] = [0; 10];
// LOWERING-NEXT:     let mut near_bits: [u8; 10] = [0; 10];
// LOWERING-NEXT:     let mut one: LongDouble = LongDouble([0; 10]);
// LOWERING-NEXT:     let mut two: LongDouble = LongDouble([0; 10]);
// LOWERING-NEXT:     let mut near: LongDouble = LongDouble([0; 10]);
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     one_bits = [0, 0, 0, 0, 0, 0, 0, 128, 255, 63];
// LOWERING-NEXT:     two_bits = [0, 0, 0, 0, 0, 0, 0, 128, 0, 64];
// LOWERING-NEXT:     near_bits = [1, 0, 0, 0, 0, 0, 0, 128, 255, 63];
// LOWERING-NEXT:     let _v1: *mut u8 = one_bits.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     let _v2: LongDouble = load80(_v1);
// LOWERING-NEXT:     one = _v2;
// LOWERING-NEXT:     let _v3: *mut u8 = two_bits.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     let _v4: LongDouble = load80(_v3);
// LOWERING-NEXT:     two = _v4;
// LOWERING-NEXT:     let _v5: *mut u8 = near_bits.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     let _v6: LongDouble = load80(_v5);
// LOWERING-NEXT:     near = _v6;
// LOWERING-NEXT:     let _v7: *mut i8 = b"add\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v8: LongDouble = one;
// LOWERING-NEXT:     let _v9: LongDouble = two;
// LOWERING-NEXT:     let _v10: LongDouble = _v8 + _v9;
// LOWERING-NEXT:     dump80(_v7, _v10);
// LOWERING-NEXT:     let _v11: *mut i8 = b"sub\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v12: LongDouble = near;
// LOWERING-NEXT:     let _v13: LongDouble = one;
// LOWERING-NEXT:     let _v14: LongDouble = _v12 - _v13;
// LOWERING-NEXT:     dump80(_v11, _v14);
// LOWERING-NEXT:     let _v15: *mut i8 = b"mul\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v16: LongDouble = one;
// LOWERING-NEXT:     let _v17: LongDouble = two;
// LOWERING-NEXT:     let _v18: LongDouble = _v16 * _v17;
// LOWERING-NEXT:     dump80(_v15, _v18);
// LOWERING-NEXT:     let _v19: *mut i8 = b"div\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v20: LongDouble = two;
// LOWERING-NEXT:     let _v21: LongDouble = one;
// LOWERING-NEXT:     let _v22: LongDouble = _v20 / _v21;
// LOWERING-NEXT:     dump80(_v19, _v22);
// LOWERING-NEXT:     let _v23: *mut i8 = b"neg\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v24: LongDouble = near;
// LOWERING-NEXT:     let _v25: LongDouble = -_v24;
// LOWERING-NEXT:     dump80(_v23, _v25);
// LOWERING-NEXT:     let _v26: *mut i8 = b"%d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v27: LongDouble = one;
// LOWERING-NEXT:     let _v28: LongDouble = two;
// LOWERING-NEXT:     let _v29: bool = _v27 < _v28;
// LOWERING-NEXT:     let _v30: i32 = _v29 as i32;
// LOWERING-NEXT:     let _v31: LongDouble = near;
// LOWERING-NEXT:     let _v32: LongDouble = one;
// LOWERING-NEXT:     let _v33: bool = _v31 > _v32;
// LOWERING-NEXT:     let _v34: i32 = _v33 as i32;
// LOWERING-NEXT:     let _v35: LongDouble = one;
// LOWERING-NEXT:     let _v36: LongDouble = one;
// LOWERING-NEXT:     let _v37: bool = _v35 == _v36;
// LOWERING-NEXT:     let _v38: i32 = _v37 as i32;
// LOWERING-NEXT:     let _v39: LongDouble = near;
// LOWERING-NEXT:     let _v40: LongDouble = one;
// LOWERING-NEXT:     let _v41: bool = _v39 != _v40;
// LOWERING-NEXT:     let _v42: i32 = _v41 as i32;
// LOWERING-NEXT:     let _v43: i32 = unsafe { printf(_v26 as *const i8, _v30, _v34, _v38, _v42) };
// LOWERING-NEXT:     let _v44: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v45: LongDouble = one;
// LOWERING-NEXT:     let _v46: LongDouble = two;
// LOWERING-NEXT:     let _v47: LongDouble = _v45 + _v46;
// LOWERING-NEXT:     let _v48: i32 = __slate_f80_to_i32(_v47);
// LOWERING-NEXT:     let _v49: i32 = unsafe { printf(_v44 as *const i8, _v48) };
// LOWERING-NEXT:     let _v50: *mut i8 = b"from_i64\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v51: i64 = 1234567890123i64;
// LOWERING-NEXT:     let _v52: LongDouble = __slate_f80_from_i64(_v51);
// LOWERING-NEXT:     dump80(_v50, _v52);
// LOWERING-NEXT:     let _v53: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v54: i32 = 123456789;
// LOWERING-NEXT:     let _v55: LongDouble = __slate_f80_from_i32(_v54);
// LOWERING-NEXT:     let _v56: i32 = __slate_f80_to_i32(_v55);
// LOWERING-NEXT:     let _v57: i32 = unsafe { printf(_v53 as *const i8, _v56) };
// LOWERING-NEXT:     let _v58: i32 = 0;
// LOWERING-NEXT:     __retval = _v58;
// LOWERING-NEXT:     let _v59: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v59 as i32);
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     safe fn __slate_cf80_div(a: num_complex::Complex<LongDouble>, b: num_complex::Complex<LongDouble>) -> num_complex::Complex<LongDouble>;
// LOWERING-NEXT:     safe fn __slate_cf80_mul(a: num_complex::Complex<LongDouble>, b: num_complex::Complex<LongDouble>) -> num_complex::Complex<LongDouble>;
// LOWERING-NEXT:     safe fn __slate_f80_abs(a: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_add(a: LongDouble, b: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_ceil(a: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_copysign(a: LongDouble, b: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_div(a: LongDouble, b: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_eq(a: LongDouble, b: LongDouble) -> bool;
// LOWERING-NEXT:     safe fn __slate_f80_floor(a: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_fma(a: LongDouble, b: LongDouble, c: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_fmax(a: LongDouble, b: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_fmin(a: LongDouble, b: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_fract(a: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_from_bool(a: bool) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_from_f32(a: f32) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_from_f64(a: f64) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_from_i128(a: i128) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_from_i16(a: i16) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_from_i32(a: i32) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_from_i64(a: i64) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_from_i8(a: i8) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_from_u128(a: u128) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_from_u16(a: u16) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_from_u32(a: u32) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_from_u64(a: u64) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_from_u8(a: u8) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_ge(a: LongDouble, b: LongDouble) -> bool;
// LOWERING-NEXT:     safe fn __slate_f80_gt(a: LongDouble, b: LongDouble) -> bool;
// LOWERING-NEXT:     safe fn __slate_f80_is_fp_class(a: LongDouble, flags: i32) -> bool;
// LOWERING-NEXT:     safe fn __slate_f80_le(a: LongDouble, b: LongDouble) -> bool;
// LOWERING-NEXT:     safe fn __slate_f80_lt(a: LongDouble, b: LongDouble) -> bool;
// LOWERING-NEXT:     safe fn __slate_f80_mul(a: LongDouble, b: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_ne(a: LongDouble, b: LongDouble) -> bool;
// LOWERING-NEXT:     safe fn __slate_f80_neg(a: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_rint(a: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_round(a: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_signbit(a: LongDouble) -> bool;
// LOWERING-NEXT:     safe fn __slate_f80_sub(a: LongDouble, b: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_to_bool(a: LongDouble) -> bool;
// LOWERING-NEXT:     safe fn __slate_f80_to_f32(a: LongDouble) -> f32;
// LOWERING-NEXT:     safe fn __slate_f80_to_f64(a: LongDouble) -> f64;
// LOWERING-NEXT:     safe fn __slate_f80_to_i128(a: LongDouble) -> i128;
// LOWERING-NEXT:     safe fn __slate_f80_to_i16(a: LongDouble) -> i16;
// LOWERING-NEXT:     safe fn __slate_f80_to_i32(a: LongDouble) -> i32;
// LOWERING-NEXT:     safe fn __slate_f80_to_i64(a: LongDouble) -> i64;
// LOWERING-NEXT:     safe fn __slate_f80_to_i8(a: LongDouble) -> i8;
// LOWERING-NEXT:     safe fn __slate_f80_to_u128(a: LongDouble) -> u128;
// LOWERING-NEXT:     safe fn __slate_f80_to_u16(a: LongDouble) -> u16;
// LOWERING-NEXT:     safe fn __slate_f80_to_u32(a: LongDouble) -> u32;
// LOWERING-NEXT:     safe fn __slate_f80_to_u64(a: LongDouble) -> u64;
// LOWERING-NEXT:     safe fn __slate_f80_to_u8(a: LongDouble) -> u8;
// LOWERING-NEXT:     safe fn __slate_f80_trunc(a: LongDouble) -> LongDouble;
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C, align(16))]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct LongDouble([u8; 10]);
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::ops::Add for LongDouble {
// REWRITES-NEXT:     type Output = LongDouble;
// REWRITES-NEXT:     fn add(self, o: LongDouble) -> LongDouble { __slate_f80_add(self, o) }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::ops::Sub for LongDouble {
// REWRITES-NEXT:     type Output = LongDouble;
// REWRITES-NEXT:     fn sub(self, o: LongDouble) -> LongDouble { __slate_f80_sub(self, o) }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::ops::Mul for LongDouble {
// REWRITES-NEXT:     type Output = LongDouble;
// REWRITES-NEXT:     fn mul(self, o: LongDouble) -> LongDouble { __slate_f80_mul(self, o) }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::ops::Div for LongDouble {
// REWRITES-NEXT:     type Output = LongDouble;
// REWRITES-NEXT:     fn div(self, o: LongDouble) -> LongDouble { __slate_f80_div(self, o) }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::ops::AddAssign for LongDouble {
// REWRITES-NEXT:     fn add_assign(&mut self, o: LongDouble) { {
// REWRITES-NEXT:     *self = __slate_f80_add(*self, o);
// REWRITES-NEXT: } }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::ops::SubAssign for LongDouble {
// REWRITES-NEXT:     fn sub_assign(&mut self, o: LongDouble) { {
// REWRITES-NEXT:     *self = __slate_f80_sub(*self, o);
// REWRITES-NEXT: } }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::ops::MulAssign for LongDouble {
// REWRITES-NEXT:     fn mul_assign(&mut self, o: LongDouble) { {
// REWRITES-NEXT:     *self = __slate_f80_mul(*self, o);
// REWRITES-NEXT: } }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::ops::DivAssign for LongDouble {
// REWRITES-NEXT:     fn div_assign(&mut self, o: LongDouble) { {
// REWRITES-NEXT:     *self = __slate_f80_div(*self, o);
// REWRITES-NEXT: } }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::ops::Neg for LongDouble {
// REWRITES-NEXT:     type Output = LongDouble;
// REWRITES-NEXT:     fn neg(self) -> LongDouble { __slate_f80_neg(self) }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::cmp::PartialEq for LongDouble {
// REWRITES-NEXT:     fn eq(&self, other: &LongDouble) -> bool { __slate_f80_eq(*self, *other) }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::cmp::PartialOrd for LongDouble {
// REWRITES-NEXT:     fn partial_cmp(&self, other: &LongDouble) -> Option<std::cmp::Ordering> { if __slate_f80_lt(*self, *other) { Some(std::cmp::Ordering::Less) } else { if __slate_f80_gt(*self, *other) { Some(std::cmp::Ordering::Greater) } else { if __slate_f80_eq(*self, *other) { Some(std::cmp::Ordering::Equal) } else { None } } } }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn memcpy(_0: *mut core::ffi::c_void, _1: *const core::ffi::c_void, _2: usize) -> *mut core::ffi::c_void;
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn load80(arg2: *mut u8) -> LongDouble {
// REWRITES-NEXT: let mut bytes: *mut u8 = arg2;
// REWRITES-NEXT: let mut __retval: LongDouble = LongDouble([0; 10]);
// REWRITES-NEXT: let mut value: LongDouble = LongDouble([0; 10]);
// REWRITES-NEXT: let _v0: *mut core::ffi::c_void = std::ptr::addr_of_mut!(value) as *mut core::ffi::c_void;
// REWRITES-NEXT: let _v3: u64 = 10;
// REWRITES-NEXT: unsafe { std::ptr::copy_nonoverlapping(((bytes as *mut core::ffi::c_void) as *const core::ffi::c_void) as *const u8, (_v0 as *mut core::ffi::c_void) as *mut u8, (_v3 as usize) as usize) };
// REWRITES-NEXT: __retval = value;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn dump80(arg0: *mut i8, arg1: LongDouble) {
// REWRITES-NEXT: let mut name: *mut i8 = arg0;
// REWRITES-NEXT: let mut value: LongDouble = arg1;
// REWRITES-NEXT: let mut bytes: [u8; 10] = [0; 10];
// REWRITES-NEXT: let _v0: *mut u8 = bytes.as_mut_ptr() as *mut u8;
// REWRITES-NEXT: let _v1: *mut core::ffi::c_void = _v0 as *mut core::ffi::c_void;
// REWRITES-NEXT: let _v2: *mut core::ffi::c_void = std::ptr::addr_of_mut!(value) as *mut core::ffi::c_void;
// REWRITES-NEXT: let _v3: u64 = 10;
// REWRITES-NEXT: unsafe { std::ptr::copy_nonoverlapping((_v2 as *const core::ffi::c_void) as *const u8, (_v1 as *mut core::ffi::c_void) as *mut u8, (_v3 as usize) as usize) };
// REWRITES-NEXT: let _v5: *mut i8 = b"%s\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v7: i32 = unsafe { printf(_v5 as *const i8, name) };
// REWRITES-NEXT: {
// REWRITES-NEXT:         let mut i: i32 = 0;
// REWRITES-NEXT:         i = 0;
// REWRITES-NEXT:         loop {
// REWRITES-NEXT:                     let _v10: i32 = 10;
// REWRITES-NEXT:                     if !(i < _v10) {
// REWRITES-NEXT:                                     break;
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     let _v12: *mut i8 = b"%02x\0".as_ptr() as *mut i8;
// REWRITES-NEXT:                     let _v17: i32 = unsafe { printf(_v12 as *const i8, bytes[((i as i64) as usize)] as i32) };
// REWRITES-NEXT:                     i = i + 1;
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v20: *mut i8 = b"\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v21: i32 = unsafe { printf(_v20 as *const i8) };
// REWRITES-NEXT: return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut one_bits: [u8; 10] = [0; 10];
// REWRITES-NEXT: let mut two_bits: [u8; 10] = [0; 10];
// REWRITES-NEXT: let mut near_bits: [u8; 10] = [0; 10];
// REWRITES-NEXT: let mut one: LongDouble = LongDouble([0; 10]);
// REWRITES-NEXT: let mut two: LongDouble = LongDouble([0; 10]);
// REWRITES-NEXT: let mut near: LongDouble = LongDouble([0; 10]);
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: one_bits = [0, 0, 0, 0, 0, 0, 0, 128, 255, 63];
// REWRITES-NEXT: two_bits = [0, 0, 0, 0, 0, 0, 0, 128, 0, 64];
// REWRITES-NEXT: near_bits = [1, 0, 0, 0, 0, 0, 0, 128, 255, 63];
// REWRITES-NEXT: let _v1: *mut u8 = one_bits.as_mut_ptr() as *mut u8;
// REWRITES-NEXT: one = load80(_v1);
// REWRITES-NEXT: let _v3: *mut u8 = two_bits.as_mut_ptr() as *mut u8;
// REWRITES-NEXT: two = load80(_v3);
// REWRITES-NEXT: let _v5: *mut u8 = near_bits.as_mut_ptr() as *mut u8;
// REWRITES-NEXT: near = load80(_v5);
// REWRITES-NEXT: let _v7: *mut i8 = b"add\0".as_ptr() as *mut i8;
// REWRITES-NEXT: dump80(_v7, one + two);
// REWRITES-NEXT: let _v11: *mut i8 = b"sub\0".as_ptr() as *mut i8;
// REWRITES-NEXT: dump80(_v11, near - one);
// REWRITES-NEXT: let _v15: *mut i8 = b"mul\0".as_ptr() as *mut i8;
// REWRITES-NEXT: dump80(_v15, one * two);
// REWRITES-NEXT: let _v19: *mut i8 = b"div\0".as_ptr() as *mut i8;
// REWRITES-NEXT: dump80(_v19, two / one);
// REWRITES-NEXT: let _v23: *mut i8 = b"neg\0".as_ptr() as *mut i8;
// REWRITES-NEXT: dump80(_v23, -near);
// REWRITES-NEXT: let _v26: *mut i8 = b"%d %d %d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v43: i32 = unsafe { printf(_v26 as *const i8, (one < two) as i32, (near > one) as i32, (one == one) as i32, (near != one) as i32) };
// REWRITES-NEXT: let _v44: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v48: i32 = __slate_f80_to_i32(one + two);
// REWRITES-NEXT: let _v49: i32 = unsafe { printf(_v44 as *const i8, _v48) };
// REWRITES-NEXT: let _v50: *mut i8 = b"from_i64\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v51: i64 = 1234567890123i64;
// REWRITES-NEXT: let _v52: LongDouble = __slate_f80_from_i64(_v51);
// REWRITES-NEXT: dump80(_v50, _v52);
// REWRITES-NEXT: let _v53: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v54: i32 = 123456789;
// REWRITES-NEXT: let _v55: LongDouble = __slate_f80_from_i32(_v54);
// REWRITES-NEXT: let _v56: i32 = __slate_f80_to_i32(_v55);
// REWRITES-NEXT: let _v57: i32 = unsafe { printf(_v53 as *const i8, _v56) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     safe fn __slate_cf80_div(a: num_complex::Complex<LongDouble>, b: num_complex::Complex<LongDouble>) -> num_complex::Complex<LongDouble>;
// REWRITES-NEXT:     safe fn __slate_cf80_mul(a: num_complex::Complex<LongDouble>, b: num_complex::Complex<LongDouble>) -> num_complex::Complex<LongDouble>;
// REWRITES-NEXT:     safe fn __slate_f80_abs(a: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_add(a: LongDouble, b: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_ceil(a: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_copysign(a: LongDouble, b: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_div(a: LongDouble, b: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_eq(a: LongDouble, b: LongDouble) -> bool;
// REWRITES-NEXT:     safe fn __slate_f80_floor(a: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_fma(a: LongDouble, b: LongDouble, c: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_fmax(a: LongDouble, b: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_fmin(a: LongDouble, b: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_fract(a: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_from_bool(a: bool) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_from_f32(a: f32) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_from_f64(a: f64) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_from_i128(a: i128) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_from_i16(a: i16) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_from_i32(a: i32) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_from_i64(a: i64) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_from_i8(a: i8) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_from_u128(a: u128) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_from_u16(a: u16) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_from_u32(a: u32) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_from_u64(a: u64) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_from_u8(a: u8) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_ge(a: LongDouble, b: LongDouble) -> bool;
// REWRITES-NEXT:     safe fn __slate_f80_gt(a: LongDouble, b: LongDouble) -> bool;
// REWRITES-NEXT:     safe fn __slate_f80_is_fp_class(a: LongDouble, flags: i32) -> bool;
// REWRITES-NEXT:     safe fn __slate_f80_le(a: LongDouble, b: LongDouble) -> bool;
// REWRITES-NEXT:     safe fn __slate_f80_lt(a: LongDouble, b: LongDouble) -> bool;
// REWRITES-NEXT:     safe fn __slate_f80_mul(a: LongDouble, b: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_ne(a: LongDouble, b: LongDouble) -> bool;
// REWRITES-NEXT:     safe fn __slate_f80_neg(a: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_rint(a: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_round(a: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_signbit(a: LongDouble) -> bool;
// REWRITES-NEXT:     safe fn __slate_f80_sub(a: LongDouble, b: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_to_bool(a: LongDouble) -> bool;
// REWRITES-NEXT:     safe fn __slate_f80_to_f32(a: LongDouble) -> f32;
// REWRITES-NEXT:     safe fn __slate_f80_to_f64(a: LongDouble) -> f64;
// REWRITES-NEXT:     safe fn __slate_f80_to_i128(a: LongDouble) -> i128;
// REWRITES-NEXT:     safe fn __slate_f80_to_i16(a: LongDouble) -> i16;
// REWRITES-NEXT:     safe fn __slate_f80_to_i32(a: LongDouble) -> i32;
// REWRITES-NEXT:     safe fn __slate_f80_to_i64(a: LongDouble) -> i64;
// REWRITES-NEXT:     safe fn __slate_f80_to_i8(a: LongDouble) -> i8;
// REWRITES-NEXT:     safe fn __slate_f80_to_u128(a: LongDouble) -> u128;
// REWRITES-NEXT:     safe fn __slate_f80_to_u16(a: LongDouble) -> u16;
// REWRITES-NEXT:     safe fn __slate_f80_to_u32(a: LongDouble) -> u32;
// REWRITES-NEXT:     safe fn __slate_f80_to_u64(a: LongDouble) -> u64;
// REWRITES-NEXT:     safe fn __slate_f80_to_u8(a: LongDouble) -> u8;
// REWRITES-NEXT:     safe fn __slate_f80_trunc(a: LongDouble) -> LongDouble;
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
