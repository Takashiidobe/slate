#include <fenv.h>
#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdatomic.h>
#include <stdbit.h>
#include <stdckdint.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <uchar.h>
#include <wchar.h>

static int c23_stdbit(void) {
  unsigned int value = 0b10110000u;
  return (int)(stdc_leading_zeros(value) + stdc_leading_ones(value) +
               stdc_trailing_zeros(value) + stdc_trailing_ones(value) +
               stdc_first_leading_zero(value) + stdc_first_leading_one(value) +
               stdc_first_trailing_zero(value) +
               stdc_first_trailing_one(value) + stdc_count_zeros(value) +
               stdc_count_ones(value) + stdc_has_single_bit(64u) +
               stdc_bit_width(value) + stdc_bit_floor(value) +
               stdc_bit_ceil(value));
}

static int c23_checked_arithmetic(void) {
  int result;
  int total  = 0;
  total     += !ckd_add(&result, 20, 22) && result == 42;
  total     += !ckd_sub(&result, 50, 8) && result == 42;
  total     += !ckd_mul(&result, 6, 7) && result == 42;
  total     += ckd_add(&result, INT_MAX, 1);
  return total;
}

static int c23_utf8(void) {
  mbstate_t      input_state        = {};
  mbstate_t      output_state       = {};
  char8_t        character          = 0;
  char           output[MB_LEN_MAX] = {};
  size_t         input_size         = mbrtoc8(&character, "A", 1, &input_state);
  size_t         output_size        = c8rtomb(output, character, &output_state);
  atomic_char8_t atomic_character   = character;
  atomic_store(&atomic_character, u8'B');
  return (input_size == 1) + (output_size == 1) + (output[0] == 'A') +
         (atomic_load(&atomic_character) == u8'B') +
         (ATOMIC_CHAR8_T_LOCK_FREE > 0);
}

static int c23_memory(void) {
  char        source[]       = "abcdef";
  char        destination[8] = {};
  char        secret[]       = "secret";
  char       *first_copy;
  char       *second_copy;
  const char *phrase = "hello world";
  char        mutable_phrase[] = "hello world";
  void       *stop  = memccpy(destination, source, 'c', 6);
  int         total = stop == destination + 3 && destination[2] == 'c';
  memset_explicit(secret, 0, sizeof(secret));
  total       += secret[0] == 0 && secret[5] == 0;
  first_copy   = strdup("c23");
  second_copy  = strndup("library", 3);
  total       += first_copy != nullptr && strcmp(first_copy, "c23") == 0;
  total       += second_copy != nullptr && strcmp(second_copy, "lib") == 0;
  free(first_copy);
  free(second_copy);

  const char *const_hit = strchr(phrase, 'w');
  char       *mut_hit    = strchr(mutable_phrase, 'w');
  total += const_hit != nullptr && mut_hit != nullptr;
  total += memchr(phrase, 'o', 11) != nullptr;
  total += memchr(mutable_phrase, 'o', 11) != nullptr;
  total += strstr(phrase, "world") != nullptr;
  total += strstr(mutable_phrase, "world") != nullptr;
  return total;
}

static int c23_time(void) {
  time_t          timestamp       = 0;
  struct tm       utc             = {};
  struct tm       local           = {};
  struct timespec resolution      = {};
  char            month[32]       = {};
  wchar_t         wide_month[32]  = {};
  int             total           = gmtime_r(&timestamp, &utc) == &utc;
  total                          += localtime_r(&timestamp, &local) == &local;
  total += timespec_getres(&resolution, TIME_UTC) == TIME_UTC;
  total += resolution.tv_sec > 0 || resolution.tv_nsec > 0;
  total += timegm(&utc) == 0;
  total += strftime(month, sizeof(month), "%OB", &utc) == 7;
  total += strcmp(month, "January") == 0;
  total += wcsftime(wide_month, 32, L"%OB", &utc) == 7;
  total += wcscmp(wide_month, L"January") == 0;

  const wchar_t *const_month = wide_month;
  total += wcschr(const_month, L'n') != nullptr;
  total += wcschr(wide_month, L'n') != nullptr;
  total += wcsstr(const_month, L"Jan") != nullptr;
  total += wcsstr(wide_month, L"Jan") != nullptr;
  return total;
}

static int c23_io(void) {
  char          output[64]             = {};
  char          float_output[16]       = {};
  char          double_output[16]      = {};
  char          long_double_output[16] = {};
  unsigned int  binary_value           = 0;
  uint16_t      exact_value            = 0;
  uint_fast16_t fast_value             = 0;
  int written = snprintf(output, sizeof(output), "%b %w16u %wf16u", 13u,
                         (uint16_t)21, (uint_fast16_t)34);
  int scanned = sscanf("1011 55 89", "%b %w16u %wf16u", &binary_value,
                       &exact_value, &fast_value);
  int floating_written =
      strfromf(float_output, sizeof(float_output), "%.1f", 1.5f) +
      strfromd(double_output, sizeof(double_output), "%.1f", 2.5) +
      strfroml(long_double_output, sizeof(long_double_output), "%.1f", 3.5L);
  return written + (strcmp(output, "1101 21 34") == 0) + scanned +
         (binary_value == 11) + (exact_value == 55) + (fast_value == 89) +
         floating_written + (strcmp(float_output, "1.5") == 0) +
         (strcmp(double_output, "2.5") == 0) +
         (strcmp(long_double_output, "3.5") == 0);
}

static int c23_limits(void) {
  int integer_widths  = BOOL_WIDTH + CHAR_WIDTH + SCHAR_WIDTH + UCHAR_WIDTH +
                        SHRT_WIDTH + USHRT_WIDTH + INT_WIDTH + UINT_WIDTH +
                        LONG_WIDTH + ULONG_WIDTH + LLONG_WIDTH + ULLONG_WIDTH +
                        SIZE_WIDTH + PTRDIFF_WIDTH;
  int floating_limits = (FLT_TRUE_MIN > 0.0f) + (DBL_TRUE_MIN > 0.0) +
                        (LDBL_TRUE_MIN > 0.0L) + (FLT_NORM_MAX <= FLT_MAX) +
                        (DBL_NORM_MAX <= DBL_MAX) +
                        (LDBL_NORM_MAX <= LDBL_MAX) + (FLT_HAS_SUBNORM >= -1) +
                        (DBL_HAS_SUBNORM >= -1) + (LDBL_HAS_SUBNORM >= -1) +
                        (sizeof(FLT_SNAN) == sizeof(float)) +
                        (sizeof(DBL_SNAN) == sizeof(double)) +
                        (sizeof(LDBL_SNAN) == sizeof(long double));
  int header_versions = (__STDC_VERSION_FENV_H__ == 202311L) +
                        (__STDC_VERSION_MATH_H__ == 202311L) +
                        (__STDC_VERSION_STDINT_H__ == 202311L) +
                        (__STDC_VERSION_STDLIB_H__ == 202311L) +
                        (__STDC_VERSION_TIME_H__ == 202311L) +
                        (__STDC_VERSION_STDCKDINT_H__ == 202311L) +
                        (__STDC_VERSION_STDBIT_H__ == 202311L);
  return integer_widths + floating_limits + header_versions;
}

int main(void) {
  printf("%d %d %d %d %d %d\n", c23_stdbit(), c23_checked_arithmetic(),
         c23_utf8(), c23_memory(), c23_time(), c23_io() + c23_limits());
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
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[allow(non_camel_case_types)]
// LOWERING-NEXT: #[derive(Clone, Copy, PartialEq, Eq, Debug, Hash)]
// LOWERING-NEXT: enum memory_order {
// LOWERING-NEXT:     memory_order_relaxed = 0,
// LOWERING-NEXT:     memory_order_consume = 1,
// LOWERING-NEXT:     memory_order_acquire = 2,
// LOWERING-NEXT:     memory_order_release = 3,
// LOWERING-NEXT:     memory_order_acq_rel = 4,
// LOWERING-NEXT:     memory_order_seq_cst = 5,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct __mbstate_t {
// LOWERING-NEXT:     __count: i32,
// LOWERING-NEXT:     __value: anon_0,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: union anon_0 {
// LOWERING-NEXT:     __wch: i32,
// LOWERING-NEXT:     __wchb: [i8; 4],
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct tm {
// LOWERING-NEXT:     tm_sec: i32,
// LOWERING-NEXT:     tm_min: i32,
// LOWERING-NEXT:     tm_hour: i32,
// LOWERING-NEXT:     tm_mday: i32,
// LOWERING-NEXT:     tm_mon: i32,
// LOWERING-NEXT:     tm_year: i32,
// LOWERING-NEXT:     tm_wday: i32,
// LOWERING-NEXT:     tm_yday: i32,
// LOWERING-NEXT:     tm_isdst: i32,
// LOWERING-NEXT:     __tm_gmtoff: i64,
// LOWERING-NEXT:     __tm_zone: *mut i8,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut _str_10: [i32; 8] = [74, 97, 110, 117, 97, 114, 121, 0];
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut _str_11: [i32; 4] = [74, 97, 110, 0];
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut _str_9: [i32; 4] = [37, 79, 66, 0];
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT:     fn stdc_leading_zeros_ui(_0: u32) -> u32;
// LOWERING-NEXT:     fn stdc_leading_ones_ui(_0: u32) -> u32;
// LOWERING-NEXT:     fn stdc_trailing_zeros_ui(_0: u32) -> u32;
// LOWERING-NEXT:     fn stdc_trailing_ones_ui(_0: u32) -> u32;
// LOWERING-NEXT:     fn stdc_first_leading_zero_ui(_0: u32) -> u32;
// LOWERING-NEXT:     fn stdc_first_leading_one_ui(_0: u32) -> u32;
// LOWERING-NEXT:     fn stdc_first_trailing_zero_ui(_0: u32) -> u32;
// LOWERING-NEXT:     fn stdc_first_trailing_one_ui(_0: u32) -> u32;
// LOWERING-NEXT:     fn stdc_count_zeros_ui(_0: u32) -> u32;
// LOWERING-NEXT:     fn stdc_count_ones_ui(_0: u32) -> u32;
// LOWERING-NEXT:     fn stdc_bit_width_ui(_0: u32) -> u32;
// LOWERING-NEXT:     fn stdc_bit_floor_ui(_0: u32) -> u32;
// LOWERING-NEXT:     fn stdc_bit_ceil_ui(_0: u32) -> u32;
// LOWERING-NEXT:     fn mbrtoc8(_0: *mut u8, _1: *const i8, _2: usize, _3: *mut __mbstate_t) -> usize;
// LOWERING-NEXT:     fn c8rtomb(_0: *mut i8, _1: u8, _2: *mut __mbstate_t) -> usize;
// LOWERING-NEXT:     fn memccpy(_0: *mut core::ffi::c_void, _1: *const core::ffi::c_void, _2: i32, _3: usize) -> *mut core::ffi::c_void;
// LOWERING-NEXT:     fn memset_explicit(_0: *mut core::ffi::c_void, _1: i32, _2: usize) -> *mut core::ffi::c_void;
// LOWERING-NEXT:     fn strdup(_0: *const i8) -> *mut i8;
// LOWERING-NEXT:     fn strndup(_0: *const i8, _1: usize) -> *mut i8;
// LOWERING-NEXT:     fn strcmp(_0: *const i8, _1: *const i8) -> i32;
// LOWERING-NEXT:     fn free(_0: *mut core::ffi::c_void);
// LOWERING-NEXT:     fn strchr(_0: *const i8, _1: i32) -> *mut i8;
// LOWERING-NEXT:     fn memchr(_0: *const core::ffi::c_void, _1: i32, _2: usize) -> *mut core::ffi::c_void;
// LOWERING-NEXT:     fn strstr(_0: *const i8, _1: *const i8) -> *mut i8;
// LOWERING-NEXT:     fn gmtime_r(_0: *const i64, _1: *mut tm) -> *mut tm;
// LOWERING-NEXT:     fn localtime_r(_0: *const i64, _1: *mut tm) -> *mut tm;
// LOWERING-NEXT:     fn timespec_getres(_0: *mut libc::timespec, _1: i32) -> i32;
// LOWERING-NEXT:     fn timegm(_0: *mut tm) -> i64;
// LOWERING-NEXT:     fn strftime(_0: *mut i8, _1: usize, _2: *const i8, _3: *const tm) -> usize;
// LOWERING-NEXT:     fn wcsftime(_0: *mut i32, _1: usize, _2: *const i32, _3: *const tm) -> usize;
// LOWERING-NEXT:     fn wcscmp(_0: *const i32, _1: *const i32) -> i32;
// LOWERING-NEXT:     fn wcschr(_0: *const i32, _1: i32) -> *mut i32;
// LOWERING-NEXT:     fn wcsstr(_0: *const i32, _1: *const i32) -> *mut i32;
// LOWERING-NEXT:     fn snprintf(_0: *mut i8, _1: usize, _2: *const i8, ...) -> i32;
// LOWERING-NEXT:     fn sscanf(_0: *const i8, _1: *const i8, ...) -> i32;
// LOWERING-NEXT:     fn strfromf(_0: *mut i8, _1: usize, _2: *const i8, _3: f32) -> i32;
// LOWERING-NEXT:     fn strfromd(_0: *mut i8, _1: usize, _2: *const i8, _3: f64) -> i32;
// LOWERING-NEXT:     fn strfroml(_0: *mut i8, _1: usize, _2: *const i8, _3: LongDouble) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn c23_stdbit() -> i32 {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut value: u32 = 0;
// LOWERING-NEXT:     let _v0: u32 = 176;
// LOWERING-NEXT:     value = _v0;
// LOWERING-NEXT:     let _v1: u32 = value;
// LOWERING-NEXT:     let _v2: u32 = unsafe { stdc_leading_zeros_ui(_v1 as u32) };
// LOWERING-NEXT:     let _v3: u32 = value;
// LOWERING-NEXT:     let _v4: u32 = unsafe { stdc_leading_ones_ui(_v3 as u32) };
// LOWERING-NEXT:     let _v5: u32 = _v2 + _v4;
// LOWERING-NEXT:     let _v6: u32 = value;
// LOWERING-NEXT:     let _v7: u32 = unsafe { stdc_trailing_zeros_ui(_v6 as u32) };
// LOWERING-NEXT:     let _v8: u32 = _v5 + _v7;
// LOWERING-NEXT:     let _v9: u32 = value;
// LOWERING-NEXT:     let _v10: u32 = unsafe { stdc_trailing_ones_ui(_v9 as u32) };
// LOWERING-NEXT:     let _v11: u32 = _v8 + _v10;
// LOWERING-NEXT:     let _v12: u32 = value;
// LOWERING-NEXT:     let _v13: u32 = unsafe { stdc_first_leading_zero_ui(_v12 as u32) };
// LOWERING-NEXT:     let _v14: u32 = _v11 + _v13;
// LOWERING-NEXT:     let _v15: u32 = value;
// LOWERING-NEXT:     let _v16: u32 = unsafe { stdc_first_leading_one_ui(_v15 as u32) };
// LOWERING-NEXT:     let _v17: u32 = _v14 + _v16;
// LOWERING-NEXT:     let _v18: u32 = value;
// LOWERING-NEXT:     let _v19: u32 = unsafe { stdc_first_trailing_zero_ui(_v18 as u32) };
// LOWERING-NEXT:     let _v20: u32 = _v17 + _v19;
// LOWERING-NEXT:     let _v21: u32 = value;
// LOWERING-NEXT:     let _v22: u32 = unsafe { stdc_first_trailing_one_ui(_v21 as u32) };
// LOWERING-NEXT:     let _v23: u32 = _v20 + _v22;
// LOWERING-NEXT:     let _v24: u32 = value;
// LOWERING-NEXT:     let _v25: u32 = unsafe { stdc_count_zeros_ui(_v24 as u32) };
// LOWERING-NEXT:     let _v26: u32 = _v23 + _v25;
// LOWERING-NEXT:     let _v27: u32 = value;
// LOWERING-NEXT:     let _v28: u32 = unsafe { stdc_count_ones_ui(_v27 as u32) };
// LOWERING-NEXT:     let _v29: u32 = _v26 + _v28;
// LOWERING-NEXT:     let _v30: bool = true;
// LOWERING-NEXT:     let _v31: u32 = _v30 as u32;
// LOWERING-NEXT:     let _v32: u32 = _v29 + _v31;
// LOWERING-NEXT:     let _v33: u32 = value;
// LOWERING-NEXT:     let _v34: u32 = unsafe { stdc_bit_width_ui(_v33 as u32) };
// LOWERING-NEXT:     let _v35: u32 = _v32 + _v34;
// LOWERING-NEXT:     let _v36: u32 = value;
// LOWERING-NEXT:     let _v37: u32 = unsafe { stdc_bit_floor_ui(_v36 as u32) };
// LOWERING-NEXT:     let _v38: u32 = _v35 + _v37;
// LOWERING-NEXT:     let _v39: u32 = value;
// LOWERING-NEXT:     let _v40: u32 = unsafe { stdc_bit_ceil_ui(_v39 as u32) };
// LOWERING-NEXT:     let _v41: u32 = _v38 + _v40;
// LOWERING-NEXT:     let _v42: i32 = _v41 as i32;
// LOWERING-NEXT:     __retval = _v42;
// LOWERING-NEXT:     let _v43: i32 = __retval;
// LOWERING-NEXT:     return _v43;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn c23_checked_arithmetic() -> i32 {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut result: i32 = 0;
// LOWERING-NEXT:     let mut total: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     total = _v0;
// LOWERING-NEXT:     let _v1: i32 = 20;
// LOWERING-NEXT:     let _v2: i32 = 22;
// LOWERING-NEXT:     let _v3 = _v1.overflowing_add(_v2);
// LOWERING-NEXT:     let _v4: i32 = _v3.0 as i32;
// LOWERING-NEXT:     let _v5: bool = _v3.1 || (_v3.0 < -2147483648 || _v3.0 > 2147483647);
// LOWERING-NEXT:     result = _v4;
// LOWERING-NEXT:     let _v6: bool = !_v5;
// LOWERING-NEXT:     let _v7: bool = if _v6 {
// LOWERING-NEXT:         let _v8: i32 = result;
// LOWERING-NEXT:         let _v9: i32 = 42;
// LOWERING-NEXT:         let _v10: bool = _v8 == _v9;
// LOWERING-NEXT:         _v10
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let _v11: bool = false;
// LOWERING-NEXT:         _v11
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let _v12: i32 = _v7 as i32;
// LOWERING-NEXT:     let _v13: i32 = total;
// LOWERING-NEXT:     let _v14: i32 = _v13 + _v12;
// LOWERING-NEXT:     total = _v14;
// LOWERING-NEXT:     let _v15: i32 = 50;
// LOWERING-NEXT:     let _v16: i32 = 8;
// LOWERING-NEXT:     let _v17 = _v15.overflowing_sub(_v16);
// LOWERING-NEXT:     let _v18: i32 = _v17.0 as i32;
// LOWERING-NEXT:     let _v19: bool = _v17.1 || (_v17.0 < -2147483648 || _v17.0 > 2147483647);
// LOWERING-NEXT:     result = _v18;
// LOWERING-NEXT:     let _v20: bool = !_v19;
// LOWERING-NEXT:     let _v21: bool = if _v20 {
// LOWERING-NEXT:         let _v22: i32 = result;
// LOWERING-NEXT:         let _v23: i32 = 42;
// LOWERING-NEXT:         let _v24: bool = _v22 == _v23;
// LOWERING-NEXT:         _v24
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let _v25: bool = false;
// LOWERING-NEXT:         _v25
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let _v26: i32 = _v21 as i32;
// LOWERING-NEXT:     let _v27: i32 = total;
// LOWERING-NEXT:     let _v28: i32 = _v27 + _v26;
// LOWERING-NEXT:     total = _v28;
// LOWERING-NEXT:     let _v29: i32 = 6;
// LOWERING-NEXT:     let _v30: i32 = 7;
// LOWERING-NEXT:     let _v31 = _v29.overflowing_mul(_v30);
// LOWERING-NEXT:     let _v32: i32 = _v31.0 as i32;
// LOWERING-NEXT:     let _v33: bool = _v31.1 || (_v31.0 < -2147483648 || _v31.0 > 2147483647);
// LOWERING-NEXT:     result = _v32;
// LOWERING-NEXT:     let _v34: bool = !_v33;
// LOWERING-NEXT:     let _v35: bool = if _v34 {
// LOWERING-NEXT:         let _v36: i32 = result;
// LOWERING-NEXT:         let _v37: i32 = 42;
// LOWERING-NEXT:         let _v38: bool = _v36 == _v37;
// LOWERING-NEXT:         _v38
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let _v39: bool = false;
// LOWERING-NEXT:         _v39
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let _v40: i32 = _v35 as i32;
// LOWERING-NEXT:     let _v41: i32 = total;
// LOWERING-NEXT:     let _v42: i32 = _v41 + _v40;
// LOWERING-NEXT:     total = _v42;
// LOWERING-NEXT:     let _v43: i32 = 2147483647;
// LOWERING-NEXT:     let _v44: i32 = 1;
// LOWERING-NEXT:     let _v45 = _v43.overflowing_add(_v44);
// LOWERING-NEXT:     let _v46: i32 = _v45.0 as i32;
// LOWERING-NEXT:     let _v47: bool = _v45.1 || (_v45.0 < -2147483648 || _v45.0 > 2147483647);
// LOWERING-NEXT:     result = _v46;
// LOWERING-NEXT:     let _v48: i32 = _v47 as i32;
// LOWERING-NEXT:     let _v49: i32 = total;
// LOWERING-NEXT:     let _v50: i32 = _v49 + _v48;
// LOWERING-NEXT:     total = _v50;
// LOWERING-NEXT:     let _v51: i32 = total;
// LOWERING-NEXT:     __retval = _v51;
// LOWERING-NEXT:     let _v52: i32 = __retval;
// LOWERING-NEXT:     return _v52;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn c23_utf8() -> i32 {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut input_state: __mbstate_t = __mbstate_t { __count: 0, __value: anon_0 { __wch: 0 } };
// LOWERING-NEXT:     let mut output_state: __mbstate_t = __mbstate_t { __count: 0, __value: anon_0 { __wch: 0 } };
// LOWERING-NEXT:     let mut character: u8 = 0;
// LOWERING-NEXT:     let mut output: [i8; 4] = [0; 4];
// LOWERING-NEXT:     let mut input_size: u64 = 0;
// LOWERING-NEXT:     let mut output_size: u64 = 0;
// LOWERING-NEXT:     let mut atomic_character: u8 = 0;
// LOWERING-NEXT:     input_state = __mbstate_t { __count: 0, __value: anon_0 { __wch: 0 } };
// LOWERING-NEXT:     output_state = __mbstate_t { __count: 0, __value: anon_0 { __wch: 0 } };
// LOWERING-NEXT:     let _v0: u8 = 0;
// LOWERING-NEXT:     character = _v0;
// LOWERING-NEXT:     output = [0, 0, 0, 0];
// LOWERING-NEXT:     let _v1: *mut i8 = b"A\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v2: u64 = 1;
// LOWERING-NEXT:     let _v3: u64 = (unsafe { mbrtoc8(std::ptr::addr_of_mut!(character) as *mut u8, _v1 as *const i8, _v2 as usize, std::ptr::addr_of_mut!(input_state) as *mut __mbstate_t) }) as u64;
// LOWERING-NEXT:     input_size = _v3;
// LOWERING-NEXT:     let _v4: *mut i8 = output.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let _v5: u8 = character;
// LOWERING-NEXT:     let _v6: u64 = (unsafe { c8rtomb(_v4 as *mut i8, _v5 as u8, std::ptr::addr_of_mut!(output_state) as *mut __mbstate_t) }) as u64;
// LOWERING-NEXT:     output_size = _v6;
// LOWERING-NEXT:     let _v7: u8 = character;
// LOWERING-NEXT:     atomic_character = _v7;
// LOWERING-NEXT:     let _v8: u8 = 66;
// LOWERING-NEXT:     unsafe { std::sync::atomic::AtomicU8::from_ptr(std::ptr::addr_of_mut!(atomic_character)).store(_v8, std::sync::atomic::Ordering::SeqCst) };
// LOWERING-NEXT:     let _v9: u64 = input_size;
// LOWERING-NEXT:     let _v10: u64 = 1;
// LOWERING-NEXT:     let _v11: bool = _v9 == _v10;
// LOWERING-NEXT:     let _v12: i32 = _v11 as i32;
// LOWERING-NEXT:     let _v13: u64 = output_size;
// LOWERING-NEXT:     let _v14: u64 = 1;
// LOWERING-NEXT:     let _v15: bool = _v13 == _v14;
// LOWERING-NEXT:     let _v16: i32 = _v15 as i32;
// LOWERING-NEXT:     let _v17: i32 = _v12 + _v16;
// LOWERING-NEXT:     let _v18: i64 = 0;
// LOWERING-NEXT:     let _v19: i8 = output[(_v18 as usize)];
// LOWERING-NEXT:     let _v20: i32 = _v19 as i32;
// LOWERING-NEXT:     let _v21: i32 = 65;
// LOWERING-NEXT:     let _v22: bool = _v20 == _v21;
// LOWERING-NEXT:     let _v23: i32 = _v22 as i32;
// LOWERING-NEXT:     let _v24: i32 = _v17 + _v23;
// LOWERING-NEXT:     let _v25: u8 = unsafe { std::sync::atomic::AtomicU8::from_ptr(std::ptr::addr_of_mut!(atomic_character)).load(std::sync::atomic::Ordering::SeqCst) };
// LOWERING-NEXT:     let _v26: i32 = _v25 as i32;
// LOWERING-NEXT:     let _v27: i32 = 66;
// LOWERING-NEXT:     let _v28: bool = _v26 == _v27;
// LOWERING-NEXT:     let _v29: i32 = _v28 as i32;
// LOWERING-NEXT:     let _v30: i32 = _v24 + _v29;
// LOWERING-NEXT:     let _v31: i32 = 2;
// LOWERING-NEXT:     let _v32: i32 = 0;
// LOWERING-NEXT:     let _v33: bool = _v31 > _v32;
// LOWERING-NEXT:     let _v34: i32 = _v33 as i32;
// LOWERING-NEXT:     let _v35: i32 = _v30 + _v34;
// LOWERING-NEXT:     __retval = _v35;
// LOWERING-NEXT:     let _v36: i32 = __retval;
// LOWERING-NEXT:     return _v36;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn c23_memory() -> i32 {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut source: [i8; 7] = [0; 7];
// LOWERING-NEXT:     let mut destination: [i8; 8] = [0; 8];
// LOWERING-NEXT:     let mut secret: [i8; 7] = [0; 7];
// LOWERING-NEXT:     let mut first_copy: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut second_copy: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut phrase: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut mutable_phrase: [i8; 12] = [0; 12];
// LOWERING-NEXT:     let mut stop: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let mut total: i32 = 0;
// LOWERING-NEXT:     let mut const_hit: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut mut_hit: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     source = [97, 98, 99, 100, 101, 102, 0];
// LOWERING-NEXT:     destination = [0, 0, 0, 0, 0, 0, 0, 0];
// LOWERING-NEXT:     secret = [115, 101, 99, 114, 101, 116, 0];
// LOWERING-NEXT:     let _v0: *mut i8 = b"hello world\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     phrase = _v0;
// LOWERING-NEXT:     mutable_phrase = [104, 101, 108, 108, 111, 32, 119, 111, 114, 108, 100, 0];
// LOWERING-NEXT:     let _v1: *mut i8 = destination.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let _v2: *mut core::ffi::c_void = _v1 as *mut core::ffi::c_void;
// LOWERING-NEXT:     let _v3: *mut i8 = source.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let _v4: *mut core::ffi::c_void = _v3 as *mut core::ffi::c_void;
// LOWERING-NEXT:     let _v5: i32 = 99;
// LOWERING-NEXT:     let _v6: u64 = 6;
// LOWERING-NEXT:     let _v7: *mut core::ffi::c_void = unsafe { memccpy(_v2 as *mut core::ffi::c_void, _v4 as *const core::ffi::c_void, _v5 as i32, _v6 as usize) };
// LOWERING-NEXT:     stop = _v7;
// LOWERING-NEXT:     let _v8: *mut core::ffi::c_void = stop;
// LOWERING-NEXT:     let _v9: *mut i8 = destination.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let _v10: i32 = 3;
// LOWERING-NEXT:     let _v11: *mut i8 = unsafe { _v9.add(3) };
// LOWERING-NEXT:     let _v12: *mut core::ffi::c_void = _v11 as *mut core::ffi::c_void;
// LOWERING-NEXT:     let _v13: bool = _v8 == _v12;
// LOWERING-NEXT:     let _v14: bool = if _v13 {
// LOWERING-NEXT:         let _v15: i64 = 2;
// LOWERING-NEXT:         let _v16: i8 = destination[(_v15 as usize)];
// LOWERING-NEXT:         let _v17: i32 = _v16 as i32;
// LOWERING-NEXT:         let _v18: i32 = 99;
// LOWERING-NEXT:         let _v19: bool = _v17 == _v18;
// LOWERING-NEXT:         _v19
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let _v20: bool = false;
// LOWERING-NEXT:         _v20
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let _v21: i32 = _v14 as i32;
// LOWERING-NEXT:     total = _v21;
// LOWERING-NEXT:     let _v22: *mut i8 = secret.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let _v23: *mut core::ffi::c_void = _v22 as *mut core::ffi::c_void;
// LOWERING-NEXT:     let _v24: i32 = 0;
// LOWERING-NEXT:     let _v25: u64 = 7;
// LOWERING-NEXT:     let _v26: *mut core::ffi::c_void = unsafe { memset_explicit(_v23 as *mut core::ffi::c_void, _v24 as i32, _v25 as usize) };
// LOWERING-NEXT:     let _v27: i64 = 0;
// LOWERING-NEXT:     let _v28: i8 = secret[(_v27 as usize)];
// LOWERING-NEXT:     let _v29: i32 = _v28 as i32;
// LOWERING-NEXT:     let _v30: i32 = 0;
// LOWERING-NEXT:     let _v31: bool = _v29 == _v30;
// LOWERING-NEXT:     let _v32: bool = if _v31 {
// LOWERING-NEXT:         let _v33: i64 = 5;
// LOWERING-NEXT:         let _v34: i8 = secret[(_v33 as usize)];
// LOWERING-NEXT:         let _v35: i32 = _v34 as i32;
// LOWERING-NEXT:         let _v36: i32 = 0;
// LOWERING-NEXT:         let _v37: bool = _v35 == _v36;
// LOWERING-NEXT:         _v37
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let _v38: bool = false;
// LOWERING-NEXT:         _v38
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let _v39: i32 = _v32 as i32;
// LOWERING-NEXT:     let _v40: i32 = total;
// LOWERING-NEXT:     let _v41: i32 = _v40 + _v39;
// LOWERING-NEXT:     total = _v41;
// LOWERING-NEXT:     let _v42: *mut i8 = b"c23\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v43: *mut i8 = unsafe { strdup(_v42 as *const i8) };
// LOWERING-NEXT:     first_copy = _v43;
// LOWERING-NEXT:     let _v44: *mut i8 = b"library\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v45: u64 = 3;
// LOWERING-NEXT:     let _v46: *mut i8 = unsafe { strndup(_v44 as *const i8, _v45 as usize) };
// LOWERING-NEXT:     second_copy = _v46;
// LOWERING-NEXT:     let _v47: *mut i8 = first_copy;
// LOWERING-NEXT:     let _v48: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let _v49: bool = _v47 != _v48;
// LOWERING-NEXT:     let _v50: bool = if _v49 {
// LOWERING-NEXT:         let _v51: *mut i8 = first_copy;
// LOWERING-NEXT:         let _v52: *mut i8 = b"c23\0".as_ptr() as *mut i8;
// LOWERING-NEXT:         let _v53: i32 = unsafe { strcmp(_v51 as *const i8, _v52 as *const i8) };
// LOWERING-NEXT:         let _v54: i32 = 0;
// LOWERING-NEXT:         let _v55: bool = _v53 == _v54;
// LOWERING-NEXT:         _v55
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let _v56: bool = false;
// LOWERING-NEXT:         _v56
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let _v57: i32 = _v50 as i32;
// LOWERING-NEXT:     let _v58: i32 = total;
// LOWERING-NEXT:     let _v59: i32 = _v58 + _v57;
// LOWERING-NEXT:     total = _v59;
// LOWERING-NEXT:     let _v60: *mut i8 = second_copy;
// LOWERING-NEXT:     let _v61: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let _v62: bool = _v60 != _v61;
// LOWERING-NEXT:     let _v63: bool = if _v62 {
// LOWERING-NEXT:         let _v64: *mut i8 = second_copy;
// LOWERING-NEXT:         let _v65: *mut i8 = b"lib\0".as_ptr() as *mut i8;
// LOWERING-NEXT:         let _v66: i32 = unsafe { strcmp(_v64 as *const i8, _v65 as *const i8) };
// LOWERING-NEXT:         let _v67: i32 = 0;
// LOWERING-NEXT:         let _v68: bool = _v66 == _v67;
// LOWERING-NEXT:         _v68
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let _v69: bool = false;
// LOWERING-NEXT:         _v69
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let _v70: i32 = _v63 as i32;
// LOWERING-NEXT:     let _v71: i32 = total;
// LOWERING-NEXT:     let _v72: i32 = _v71 + _v70;
// LOWERING-NEXT:     total = _v72;
// LOWERING-NEXT:     let _v73: *mut i8 = first_copy;
// LOWERING-NEXT:     let _v74: *mut core::ffi::c_void = _v73 as *mut core::ffi::c_void;
// LOWERING-NEXT:     unsafe { free(_v74 as *mut core::ffi::c_void) };
// LOWERING-NEXT:     let _v75: *mut i8 = second_copy;
// LOWERING-NEXT:     let _v76: *mut core::ffi::c_void = _v75 as *mut core::ffi::c_void;
// LOWERING-NEXT:     unsafe { free(_v76 as *mut core::ffi::c_void) };
// LOWERING-NEXT:     let _v77: *mut i8 = phrase;
// LOWERING-NEXT:     let _v78: i32 = 119;
// LOWERING-NEXT:     let _v79: *mut i8 = unsafe { strchr(_v77 as *const i8, _v78 as i32) };
// LOWERING-NEXT:     const_hit = _v79;
// LOWERING-NEXT:     let _v80: *mut i8 = mutable_phrase.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let _v81: i32 = 119;
// LOWERING-NEXT:     let _v82: *mut i8 = unsafe { strchr(_v80 as *const i8, _v81 as i32) };
// LOWERING-NEXT:     mut_hit = _v82;
// LOWERING-NEXT:     let _v83: *mut i8 = const_hit;
// LOWERING-NEXT:     let _v84: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let _v85: bool = _v83 != _v84;
// LOWERING-NEXT:     let _v86: bool = if _v85 {
// LOWERING-NEXT:         let _v87: *mut i8 = mut_hit;
// LOWERING-NEXT:         let _v88: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:         let _v89: bool = _v87 != _v88;
// LOWERING-NEXT:         _v89
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let _v90: bool = false;
// LOWERING-NEXT:         _v90
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let _v91: i32 = _v86 as i32;
// LOWERING-NEXT:     let _v92: i32 = total;
// LOWERING-NEXT:     let _v93: i32 = _v92 + _v91;
// LOWERING-NEXT:     total = _v93;
// LOWERING-NEXT:     let _v94: *mut i8 = phrase;
// LOWERING-NEXT:     let _v95: *mut core::ffi::c_void = _v94 as *mut core::ffi::c_void;
// LOWERING-NEXT:     let _v96: i32 = 111;
// LOWERING-NEXT:     let _v97: u64 = 11;
// LOWERING-NEXT:     let _v98: *mut core::ffi::c_void = unsafe { memchr(_v95 as *const core::ffi::c_void, _v96 as i32, _v97 as usize) };
// LOWERING-NEXT:     let _v99: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let _v100: bool = _v98 != _v99;
// LOWERING-NEXT:     let _v101: i32 = _v100 as i32;
// LOWERING-NEXT:     let _v102: i32 = total;
// LOWERING-NEXT:     let _v103: i32 = _v102 + _v101;
// LOWERING-NEXT:     total = _v103;
// LOWERING-NEXT:     let _v104: *mut i8 = mutable_phrase.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let _v105: *mut core::ffi::c_void = _v104 as *mut core::ffi::c_void;
// LOWERING-NEXT:     let _v106: i32 = 111;
// LOWERING-NEXT:     let _v107: u64 = 11;
// LOWERING-NEXT:     let _v108: *mut core::ffi::c_void = unsafe { memchr(_v105 as *const core::ffi::c_void, _v106 as i32, _v107 as usize) };
// LOWERING-NEXT:     let _v109: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let _v110: bool = _v108 != _v109;
// LOWERING-NEXT:     let _v111: i32 = _v110 as i32;
// LOWERING-NEXT:     let _v112: i32 = total;
// LOWERING-NEXT:     let _v113: i32 = _v112 + _v111;
// LOWERING-NEXT:     total = _v113;
// LOWERING-NEXT:     let _v114: *mut i8 = phrase;
// LOWERING-NEXT:     let _v115: *mut i8 = b"world\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v116: *mut i8 = unsafe { strstr(_v114 as *const i8, _v115 as *const i8) };
// LOWERING-NEXT:     let _v117: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let _v118: bool = _v116 != _v117;
// LOWERING-NEXT:     let _v119: i32 = _v118 as i32;
// LOWERING-NEXT:     let _v120: i32 = total;
// LOWERING-NEXT:     let _v121: i32 = _v120 + _v119;
// LOWERING-NEXT:     total = _v121;
// LOWERING-NEXT:     let _v122: *mut i8 = mutable_phrase.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let _v123: *mut i8 = b"world\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v124: *mut i8 = unsafe { strstr(_v122 as *const i8, _v123 as *const i8) };
// LOWERING-NEXT:     let _v125: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let _v126: bool = _v124 != _v125;
// LOWERING-NEXT:     let _v127: i32 = _v126 as i32;
// LOWERING-NEXT:     let _v128: i32 = total;
// LOWERING-NEXT:     let _v129: i32 = _v128 + _v127;
// LOWERING-NEXT:     total = _v129;
// LOWERING-NEXT:     let _v130: i32 = total;
// LOWERING-NEXT:     __retval = _v130;
// LOWERING-NEXT:     let _v131: i32 = __retval;
// LOWERING-NEXT:     return _v131;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn c23_time() -> i32 {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut timestamp: i64 = 0;
// LOWERING-NEXT:     let mut utc: tm = tm { tm_sec: 0, tm_min: 0, tm_hour: 0, tm_mday: 0, tm_mon: 0, tm_year: 0, tm_wday: 0, tm_yday: 0, tm_isdst: 0, __tm_gmtoff: 0, __tm_zone: std::ptr::null_mut() };
// LOWERING-NEXT:     let mut local: tm = tm { tm_sec: 0, tm_min: 0, tm_hour: 0, tm_mday: 0, tm_mon: 0, tm_year: 0, tm_wday: 0, tm_yday: 0, tm_isdst: 0, __tm_gmtoff: 0, __tm_zone: std::ptr::null_mut() };
// LOWERING-NEXT:     let mut resolution: libc::timespec = libc::timespec { tv_sec: 0, tv_nsec: 0 };
// LOWERING-NEXT:     let mut month: aligned::Aligned<aligned::A16, [i8; 32]> = aligned::Aligned([0; 32]);
// LOWERING-NEXT:     let mut wide_month: aligned::Aligned<aligned::A16, [i32; 32]> = aligned::Aligned([0; 32]);
// LOWERING-NEXT:     let mut total: i32 = 0;
// LOWERING-NEXT:     let mut const_month: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let _v0: i64 = 0;
// LOWERING-NEXT:     timestamp = _v0;
// LOWERING-NEXT:     utc = tm { tm_sec: 0, tm_min: 0, tm_hour: 0, tm_mday: 0, tm_mon: 0, tm_year: 0, tm_wday: 0, tm_yday: 0, tm_isdst: 0, __tm_gmtoff: 0, __tm_zone: std::ptr::null_mut() };
// LOWERING-NEXT:     local = tm { tm_sec: 0, tm_min: 0, tm_hour: 0, tm_mday: 0, tm_mon: 0, tm_year: 0, tm_wday: 0, tm_yday: 0, tm_isdst: 0, __tm_gmtoff: 0, __tm_zone: std::ptr::null_mut() };
// LOWERING-NEXT:     resolution = libc::timespec { tv_sec: 0, tv_nsec: 0 };
// LOWERING-NEXT:     *month = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
// LOWERING-NEXT:     *wide_month = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
// LOWERING-NEXT:     let _v1: *mut tm = unsafe { gmtime_r(std::ptr::addr_of_mut!(timestamp) as *const i64, std::ptr::addr_of_mut!(utc) as *mut tm) };
// LOWERING-NEXT:     let _v2: bool = _v1 == std::ptr::addr_of_mut!(utc);
// LOWERING-NEXT:     let _v3: i32 = _v2 as i32;
// LOWERING-NEXT:     total = _v3;
// LOWERING-NEXT:     let _v4: *mut tm = unsafe { localtime_r(std::ptr::addr_of_mut!(timestamp) as *const i64, std::ptr::addr_of_mut!(local) as *mut tm) };
// LOWERING-NEXT:     let _v5: bool = _v4 == std::ptr::addr_of_mut!(local);
// LOWERING-NEXT:     let _v6: i32 = _v5 as i32;
// LOWERING-NEXT:     let _v7: i32 = total;
// LOWERING-NEXT:     let _v8: i32 = _v7 + _v6;
// LOWERING-NEXT:     total = _v8;
// LOWERING-NEXT:     let _v9: i32 = 1;
// LOWERING-NEXT:     let _v10: i32 = unsafe { timespec_getres(std::ptr::addr_of_mut!(resolution) as *mut libc::timespec, _v9 as i32) };
// LOWERING-NEXT:     let _v11: i32 = 1;
// LOWERING-NEXT:     let _v12: bool = _v10 == _v11;
// LOWERING-NEXT:     let _v13: i32 = _v12 as i32;
// LOWERING-NEXT:     let _v14: i32 = total;
// LOWERING-NEXT:     let _v15: i32 = _v14 + _v13;
// LOWERING-NEXT:     total = _v15;
// LOWERING-NEXT:     let _v16: i64 = resolution.tv_sec;
// LOWERING-NEXT:     let _v17: i64 = 0;
// LOWERING-NEXT:     let _v18: bool = _v16 > _v17;
// LOWERING-NEXT:     let _v19: bool = if _v18 {
// LOWERING-NEXT:         let _v20: bool = true;
// LOWERING-NEXT:         _v20
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let _v21: i64 = resolution.tv_nsec;
// LOWERING-NEXT:         let _v22: i64 = 0;
// LOWERING-NEXT:         let _v23: bool = _v21 > _v22;
// LOWERING-NEXT:         _v23
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let _v24: i32 = _v19 as i32;
// LOWERING-NEXT:     let _v25: i32 = total;
// LOWERING-NEXT:     let _v26: i32 = _v25 + _v24;
// LOWERING-NEXT:     total = _v26;
// LOWERING-NEXT:     let _v27: i64 = unsafe { timegm(std::ptr::addr_of_mut!(utc) as *mut tm) };
// LOWERING-NEXT:     let _v28: i64 = 0;
// LOWERING-NEXT:     let _v29: bool = _v27 == _v28;
// LOWERING-NEXT:     let _v30: i32 = _v29 as i32;
// LOWERING-NEXT:     let _v31: i32 = total;
// LOWERING-NEXT:     let _v32: i32 = _v31 + _v30;
// LOWERING-NEXT:     total = _v32;
// LOWERING-NEXT:     let _v33: *mut i8 = month.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let _v34: u64 = 32;
// LOWERING-NEXT:     let _v35: *mut i8 = b"%OB\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v36: u64 = (unsafe { strftime(_v33 as *mut i8, _v34 as usize, _v35 as *const i8, std::ptr::addr_of_mut!(utc) as *const tm) }) as u64;
// LOWERING-NEXT:     let _v37: u64 = 7;
// LOWERING-NEXT:     let _v38: bool = _v36 == _v37;
// LOWERING-NEXT:     let _v39: i32 = _v38 as i32;
// LOWERING-NEXT:     let _v40: i32 = total;
// LOWERING-NEXT:     let _v41: i32 = _v40 + _v39;
// LOWERING-NEXT:     total = _v41;
// LOWERING-NEXT:     let _v42: *mut i8 = month.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let _v43: *mut i8 = b"January\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v44: i32 = unsafe { strcmp(_v42 as *const i8, _v43 as *const i8) };
// LOWERING-NEXT:     let _v45: i32 = 0;
// LOWERING-NEXT:     let _v46: bool = _v44 == _v45;
// LOWERING-NEXT:     let _v47: i32 = _v46 as i32;
// LOWERING-NEXT:     let _v48: i32 = total;
// LOWERING-NEXT:     let _v49: i32 = _v48 + _v47;
// LOWERING-NEXT:     total = _v49;
// LOWERING-NEXT:     let _v50: *mut i32 = wide_month.as_mut_ptr() as *mut i32;
// LOWERING-NEXT:     let _v51: u64 = 32;
// LOWERING-NEXT:     let _v52: *mut i32 = std::ptr::addr_of_mut!(_str_9).cast::<i32>();
// LOWERING-NEXT:     let _v53: u64 = (unsafe { wcsftime(_v50 as *mut i32, _v51 as usize, _v52 as *const i32, std::ptr::addr_of_mut!(utc) as *const tm) }) as u64;
// LOWERING-NEXT:     let _v54: u64 = 7;
// LOWERING-NEXT:     let _v55: bool = _v53 == _v54;
// LOWERING-NEXT:     let _v56: i32 = _v55 as i32;
// LOWERING-NEXT:     let _v57: i32 = total;
// LOWERING-NEXT:     let _v58: i32 = _v57 + _v56;
// LOWERING-NEXT:     total = _v58;
// LOWERING-NEXT:     let _v59: *mut i32 = wide_month.as_mut_ptr() as *mut i32;
// LOWERING-NEXT:     let _v60: *mut i32 = std::ptr::addr_of_mut!(_str_10).cast::<i32>();
// LOWERING-NEXT:     let _v61: i32 = unsafe { wcscmp(_v59 as *const i32, _v60 as *const i32) };
// LOWERING-NEXT:     let _v62: i32 = 0;
// LOWERING-NEXT:     let _v63: bool = _v61 == _v62;
// LOWERING-NEXT:     let _v64: i32 = _v63 as i32;
// LOWERING-NEXT:     let _v65: i32 = total;
// LOWERING-NEXT:     let _v66: i32 = _v65 + _v64;
// LOWERING-NEXT:     total = _v66;
// LOWERING-NEXT:     let _v67: *mut i32 = wide_month.as_mut_ptr() as *mut i32;
// LOWERING-NEXT:     const_month = _v67;
// LOWERING-NEXT:     let _v68: *mut i32 = const_month;
// LOWERING-NEXT:     let _v69: i32 = 110;
// LOWERING-NEXT:     let _v70: *mut i32 = unsafe { wcschr(_v68 as *const i32, _v69 as i32) };
// LOWERING-NEXT:     let _v71: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let _v72: bool = _v70 != _v71;
// LOWERING-NEXT:     let _v73: i32 = _v72 as i32;
// LOWERING-NEXT:     let _v74: i32 = total;
// LOWERING-NEXT:     let _v75: i32 = _v74 + _v73;
// LOWERING-NEXT:     total = _v75;
// LOWERING-NEXT:     let _v76: *mut i32 = wide_month.as_mut_ptr() as *mut i32;
// LOWERING-NEXT:     let _v77: i32 = 110;
// LOWERING-NEXT:     let _v78: *mut i32 = unsafe { wcschr(_v76 as *const i32, _v77 as i32) };
// LOWERING-NEXT:     let _v79: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let _v80: bool = _v78 != _v79;
// LOWERING-NEXT:     let _v81: i32 = _v80 as i32;
// LOWERING-NEXT:     let _v82: i32 = total;
// LOWERING-NEXT:     let _v83: i32 = _v82 + _v81;
// LOWERING-NEXT:     total = _v83;
// LOWERING-NEXT:     let _v84: *mut i32 = const_month;
// LOWERING-NEXT:     let _v85: *mut i32 = std::ptr::addr_of_mut!(_str_11).cast::<i32>();
// LOWERING-NEXT:     let _v86: *mut i32 = unsafe { wcsstr(_v84 as *const i32, _v85 as *const i32) };
// LOWERING-NEXT:     let _v87: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let _v88: bool = _v86 != _v87;
// LOWERING-NEXT:     let _v89: i32 = _v88 as i32;
// LOWERING-NEXT:     let _v90: i32 = total;
// LOWERING-NEXT:     let _v91: i32 = _v90 + _v89;
// LOWERING-NEXT:     total = _v91;
// LOWERING-NEXT:     let _v92: *mut i32 = wide_month.as_mut_ptr() as *mut i32;
// LOWERING-NEXT:     let _v93: *mut i32 = std::ptr::addr_of_mut!(_str_11).cast::<i32>();
// LOWERING-NEXT:     let _v94: *mut i32 = unsafe { wcsstr(_v92 as *const i32, _v93 as *const i32) };
// LOWERING-NEXT:     let _v95: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let _v96: bool = _v94 != _v95;
// LOWERING-NEXT:     let _v97: i32 = _v96 as i32;
// LOWERING-NEXT:     let _v98: i32 = total;
// LOWERING-NEXT:     let _v99: i32 = _v98 + _v97;
// LOWERING-NEXT:     total = _v99;
// LOWERING-NEXT:     let _v100: i32 = total;
// LOWERING-NEXT:     __retval = _v100;
// LOWERING-NEXT:     let _v101: i32 = __retval;
// LOWERING-NEXT:     return _v101;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn c23_io() -> i32 {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut output: aligned::Aligned<aligned::A16, [i8; 64]> = aligned::Aligned([0; 64]);
// LOWERING-NEXT:     let mut float_output: aligned::Aligned<aligned::A16, [i8; 16]> = aligned::Aligned([0; 16]);
// LOWERING-NEXT:     let mut double_output: aligned::Aligned<aligned::A16, [i8; 16]> = aligned::Aligned([0; 16]);
// LOWERING-NEXT:     let mut long_double_output: aligned::Aligned<aligned::A16, [i8; 16]> = aligned::Aligned([0; 16]);
// LOWERING-NEXT:     let mut binary_value: u32 = 0;
// LOWERING-NEXT:     let mut exact_value: u16 = 0;
// LOWERING-NEXT:     let mut fast_value: u64 = 0;
// LOWERING-NEXT:     let mut written: i32 = 0;
// LOWERING-NEXT:     let mut scanned: i32 = 0;
// LOWERING-NEXT:     let mut floating_written: i32 = 0;
// LOWERING-NEXT:     *output = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
// LOWERING-NEXT:     *float_output = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
// LOWERING-NEXT:     *double_output = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
// LOWERING-NEXT:     *long_double_output = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
// LOWERING-NEXT:     let _v0: u32 = 0;
// LOWERING-NEXT:     binary_value = _v0;
// LOWERING-NEXT:     let _v1: u16 = 0;
// LOWERING-NEXT:     exact_value = _v1;
// LOWERING-NEXT:     let _v2: u64 = 0;
// LOWERING-NEXT:     fast_value = _v2;
// LOWERING-NEXT:     let _v3: *mut i8 = output.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let _v4: u64 = 64;
// LOWERING-NEXT:     let _v5: *mut i8 = b"%b %w16u %wf16u\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v6: u32 = 13;
// LOWERING-NEXT:     let _v7: i32 = 21;
// LOWERING-NEXT:     let _v8: u64 = 34;
// LOWERING-NEXT:     let _v9: i32 = unsafe { snprintf(_v3 as *mut i8, _v4 as usize, _v5 as *const i8, _v6, _v7, _v8) };
// LOWERING-NEXT:     written = _v9;
// LOWERING-NEXT:     let _v10: *mut i8 = b"1011 55 89\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v11: *mut i8 = b"%b %w16u %wf16u\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v12: i32 = unsafe { sscanf(_v10 as *const i8, _v11 as *const i8, std::ptr::addr_of_mut!(binary_value), std::ptr::addr_of_mut!(exact_value), std::ptr::addr_of_mut!(fast_value)) };
// LOWERING-NEXT:     scanned = _v12;
// LOWERING-NEXT:     let _v13: *mut i8 = float_output.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let _v14: u64 = 16;
// LOWERING-NEXT:     let _v15: *mut i8 = b"%.1f\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v16: f32 = 1.5;
// LOWERING-NEXT:     let _v17: i32 = unsafe { strfromf(_v13 as *mut i8, _v14 as usize, _v15 as *const i8, _v16 as f32) };
// LOWERING-NEXT:     let _v18: *mut i8 = double_output.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let _v19: u64 = 16;
// LOWERING-NEXT:     let _v20: *mut i8 = b"%.1f\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v21: f64 = 2.5;
// LOWERING-NEXT:     let _v22: i32 = unsafe { strfromd(_v18 as *mut i8, _v19 as usize, _v20 as *const i8, _v21 as f64) };
// LOWERING-NEXT:     let _v23: i32 = _v17 + _v22;
// LOWERING-NEXT:     let _v24: *mut i8 = long_double_output.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let _v25: u64 = 16;
// LOWERING-NEXT:     let _v26: *mut i8 = b"%.1f\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v27: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 224, 0, 64]);
// LOWERING-NEXT:     let _v28: i32 = unsafe { __slate_strfroml__ri32_pi8_usize_pi8_f80(_v24 as *mut i8, _v25 as usize, _v26 as *const i8, _v27) };
// LOWERING-NEXT:     let _v29: i32 = _v23 + _v28;
// LOWERING-NEXT:     floating_written = _v29;
// LOWERING-NEXT:     let _v30: i32 = written;
// LOWERING-NEXT:     let _v31: *mut i8 = output.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let _v32: *mut i8 = b"1101 21 34\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v33: i32 = unsafe { strcmp(_v31 as *const i8, _v32 as *const i8) };
// LOWERING-NEXT:     let _v34: i32 = 0;
// LOWERING-NEXT:     let _v35: bool = _v33 == _v34;
// LOWERING-NEXT:     let _v36: i32 = _v35 as i32;
// LOWERING-NEXT:     let _v37: i32 = _v30 + _v36;
// LOWERING-NEXT:     let _v38: i32 = scanned;
// LOWERING-NEXT:     let _v39: i32 = _v37 + _v38;
// LOWERING-NEXT:     let _v40: u32 = binary_value;
// LOWERING-NEXT:     let _v41: u32 = 11;
// LOWERING-NEXT:     let _v42: bool = _v40 == _v41;
// LOWERING-NEXT:     let _v43: i32 = _v42 as i32;
// LOWERING-NEXT:     let _v44: i32 = _v39 + _v43;
// LOWERING-NEXT:     let _v45: u16 = exact_value;
// LOWERING-NEXT:     let _v46: i32 = _v45 as i32;
// LOWERING-NEXT:     let _v47: i32 = 55;
// LOWERING-NEXT:     let _v48: bool = _v46 == _v47;
// LOWERING-NEXT:     let _v49: i32 = _v48 as i32;
// LOWERING-NEXT:     let _v50: i32 = _v44 + _v49;
// LOWERING-NEXT:     let _v51: u64 = fast_value;
// LOWERING-NEXT:     let _v52: u64 = 89;
// LOWERING-NEXT:     let _v53: bool = _v51 == _v52;
// LOWERING-NEXT:     let _v54: i32 = _v53 as i32;
// LOWERING-NEXT:     let _v55: i32 = _v50 + _v54;
// LOWERING-NEXT:     let _v56: i32 = floating_written;
// LOWERING-NEXT:     let _v57: i32 = _v55 + _v56;
// LOWERING-NEXT:     let _v58: *mut i8 = float_output.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let _v59: *mut i8 = b"1.5\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v60: i32 = unsafe { strcmp(_v58 as *const i8, _v59 as *const i8) };
// LOWERING-NEXT:     let _v61: i32 = 0;
// LOWERING-NEXT:     let _v62: bool = _v60 == _v61;
// LOWERING-NEXT:     let _v63: i32 = _v62 as i32;
// LOWERING-NEXT:     let _v64: i32 = _v57 + _v63;
// LOWERING-NEXT:     let _v65: *mut i8 = double_output.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let _v66: *mut i8 = b"2.5\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v67: i32 = unsafe { strcmp(_v65 as *const i8, _v66 as *const i8) };
// LOWERING-NEXT:     let _v68: i32 = 0;
// LOWERING-NEXT:     let _v69: bool = _v67 == _v68;
// LOWERING-NEXT:     let _v70: i32 = _v69 as i32;
// LOWERING-NEXT:     let _v71: i32 = _v64 + _v70;
// LOWERING-NEXT:     let _v72: *mut i8 = long_double_output.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let _v73: *mut i8 = b"3.5\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v74: i32 = unsafe { strcmp(_v72 as *const i8, _v73 as *const i8) };
// LOWERING-NEXT:     let _v75: i32 = 0;
// LOWERING-NEXT:     let _v76: bool = _v74 == _v75;
// LOWERING-NEXT:     let _v77: i32 = _v76 as i32;
// LOWERING-NEXT:     let _v78: i32 = _v71 + _v77;
// LOWERING-NEXT:     __retval = _v78;
// LOWERING-NEXT:     let _v79: i32 = __retval;
// LOWERING-NEXT:     return _v79;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn c23_limits() -> i32 {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut integer_widths: i32 = 0;
// LOWERING-NEXT:     let mut floating_limits: i32 = 0;
// LOWERING-NEXT:     let mut header_versions: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 1;
// LOWERING-NEXT:     let _v1: i32 = 8;
// LOWERING-NEXT:     let _v2: i32 = _v0 + _v1;
// LOWERING-NEXT:     let _v3: i32 = 8;
// LOWERING-NEXT:     let _v4: i32 = _v2 + _v3;
// LOWERING-NEXT:     let _v5: i32 = 8;
// LOWERING-NEXT:     let _v6: i32 = _v4 + _v5;
// LOWERING-NEXT:     let _v7: i32 = 16;
// LOWERING-NEXT:     let _v8: i32 = _v6 + _v7;
// LOWERING-NEXT:     let _v9: i32 = 16;
// LOWERING-NEXT:     let _v10: i32 = _v8 + _v9;
// LOWERING-NEXT:     let _v11: i32 = 32;
// LOWERING-NEXT:     let _v12: i32 = _v10 + _v11;
// LOWERING-NEXT:     let _v13: i32 = 32;
// LOWERING-NEXT:     let _v14: i32 = _v12 + _v13;
// LOWERING-NEXT:     let _v15: i32 = 64;
// LOWERING-NEXT:     let _v16: i32 = _v14 + _v15;
// LOWERING-NEXT:     let _v17: i32 = 64;
// LOWERING-NEXT:     let _v18: i32 = _v16 + _v17;
// LOWERING-NEXT:     let _v19: i32 = 64;
// LOWERING-NEXT:     let _v20: i32 = _v18 + _v19;
// LOWERING-NEXT:     let _v21: i32 = 64;
// LOWERING-NEXT:     let _v22: i32 = _v20 + _v21;
// LOWERING-NEXT:     let _v23: i32 = 64;
// LOWERING-NEXT:     let _v24: i32 = _v22 + _v23;
// LOWERING-NEXT:     let _v25: i32 = 64;
// LOWERING-NEXT:     let _v26: i32 = _v24 + _v25;
// LOWERING-NEXT:     integer_widths = _v26;
// LOWERING-NEXT:     let _v27: f32 = 0.0000000000000000000000000000000000000000000014013;
// LOWERING-NEXT:     let _v28: f32 = 0.0;
// LOWERING-NEXT:     let _v29: bool = _v27 > _v28;
// LOWERING-NEXT:     let _v30: i32 = _v29 as i32;
// LOWERING-NEXT:     let _v31: f64 = 0.000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000005;
// LOWERING-NEXT:     let _v32: f64 = 0.0;
// LOWERING-NEXT:     let _v33: bool = _v31 > _v32;
// LOWERING-NEXT:     let _v34: i32 = _v33 as i32;
// LOWERING-NEXT:     let _v35: i32 = _v30 + _v34;
// LOWERING-NEXT:     let _v36: LongDouble = LongDouble([1, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// LOWERING-NEXT:     let _v37: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// LOWERING-NEXT:     let _v38: bool = _v36 > _v37;
// LOWERING-NEXT:     let _v39: i32 = _v38 as i32;
// LOWERING-NEXT:     let _v40: i32 = _v35 + _v39;
// LOWERING-NEXT:     let _v41: f32 = 340282346999999984391321947108527833088.0;
// LOWERING-NEXT:     let _v42: f32 = 340282346999999984391321947108527833088.0;
// LOWERING-NEXT:     let _v43: bool = _v41 <= _v42;
// LOWERING-NEXT:     let _v44: i32 = _v43 as i32;
// LOWERING-NEXT:     let _v45: i32 = _v40 + _v44;
// LOWERING-NEXT:     let _v46: f64 = 179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368.0;
// LOWERING-NEXT:     let _v47: f64 = 179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368.0;
// LOWERING-NEXT:     let _v48: bool = _v46 <= _v47;
// LOWERING-NEXT:     let _v49: i32 = _v48 as i32;
// LOWERING-NEXT:     let _v50: i32 = _v45 + _v49;
// LOWERING-NEXT:     let _v51: LongDouble = LongDouble([255, 255, 255, 255, 255, 255, 255, 255, 254, 127]);
// LOWERING-NEXT:     let _v52: LongDouble = LongDouble([255, 255, 255, 255, 255, 255, 255, 255, 254, 127]);
// LOWERING-NEXT:     let _v53: bool = _v51 <= _v52;
// LOWERING-NEXT:     let _v54: i32 = _v53 as i32;
// LOWERING-NEXT:     let _v55: i32 = _v50 + _v54;
// LOWERING-NEXT:     let _v56: i32 = 1;
// LOWERING-NEXT:     let _v57: i32 = -1;
// LOWERING-NEXT:     let _v58: bool = _v56 >= _v57;
// LOWERING-NEXT:     let _v59: i32 = _v58 as i32;
// LOWERING-NEXT:     let _v60: i32 = _v55 + _v59;
// LOWERING-NEXT:     let _v61: i32 = 1;
// LOWERING-NEXT:     let _v62: i32 = -1;
// LOWERING-NEXT:     let _v63: bool = _v61 >= _v62;
// LOWERING-NEXT:     let _v64: i32 = _v63 as i32;
// LOWERING-NEXT:     let _v65: i32 = _v60 + _v64;
// LOWERING-NEXT:     let _v66: i32 = 1;
// LOWERING-NEXT:     let _v67: i32 = -1;
// LOWERING-NEXT:     let _v68: bool = _v66 >= _v67;
// LOWERING-NEXT:     let _v69: i32 = _v68 as i32;
// LOWERING-NEXT:     let _v70: i32 = _v65 + _v69;
// LOWERING-NEXT:     let _v71: u64 = 4;
// LOWERING-NEXT:     let _v72: u64 = 4;
// LOWERING-NEXT:     let _v73: bool = _v71 == _v72;
// LOWERING-NEXT:     let _v74: i32 = _v73 as i32;
// LOWERING-NEXT:     let _v75: i32 = _v70 + _v74;
// LOWERING-NEXT:     let _v76: u64 = 8;
// LOWERING-NEXT:     let _v77: u64 = 8;
// LOWERING-NEXT:     let _v78: bool = _v76 == _v77;
// LOWERING-NEXT:     let _v79: i32 = _v78 as i32;
// LOWERING-NEXT:     let _v80: i32 = _v75 + _v79;
// LOWERING-NEXT:     let _v81: u64 = 16;
// LOWERING-NEXT:     let _v82: u64 = 16;
// LOWERING-NEXT:     let _v83: bool = _v81 == _v82;
// LOWERING-NEXT:     let _v84: i32 = _v83 as i32;
// LOWERING-NEXT:     let _v85: i32 = _v80 + _v84;
// LOWERING-NEXT:     floating_limits = _v85;
// LOWERING-NEXT:     let _v86: i64 = 202311;
// LOWERING-NEXT:     let _v87: i64 = 202311;
// LOWERING-NEXT:     let _v88: bool = _v86 == _v87;
// LOWERING-NEXT:     let _v89: i32 = _v88 as i32;
// LOWERING-NEXT:     let _v90: i64 = 202311;
// LOWERING-NEXT:     let _v91: i64 = 202311;
// LOWERING-NEXT:     let _v92: bool = _v90 == _v91;
// LOWERING-NEXT:     let _v93: i32 = _v92 as i32;
// LOWERING-NEXT:     let _v94: i32 = _v89 + _v93;
// LOWERING-NEXT:     let _v95: i64 = 202311;
// LOWERING-NEXT:     let _v96: i64 = 202311;
// LOWERING-NEXT:     let _v97: bool = _v95 == _v96;
// LOWERING-NEXT:     let _v98: i32 = _v97 as i32;
// LOWERING-NEXT:     let _v99: i32 = _v94 + _v98;
// LOWERING-NEXT:     let _v100: i64 = 202311;
// LOWERING-NEXT:     let _v101: i64 = 202311;
// LOWERING-NEXT:     let _v102: bool = _v100 == _v101;
// LOWERING-NEXT:     let _v103: i32 = _v102 as i32;
// LOWERING-NEXT:     let _v104: i32 = _v99 + _v103;
// LOWERING-NEXT:     let _v105: i64 = 202311;
// LOWERING-NEXT:     let _v106: i64 = 202311;
// LOWERING-NEXT:     let _v107: bool = _v105 == _v106;
// LOWERING-NEXT:     let _v108: i32 = _v107 as i32;
// LOWERING-NEXT:     let _v109: i32 = _v104 + _v108;
// LOWERING-NEXT:     let _v110: i64 = 202311;
// LOWERING-NEXT:     let _v111: i64 = 202311;
// LOWERING-NEXT:     let _v112: bool = _v110 == _v111;
// LOWERING-NEXT:     let _v113: i32 = _v112 as i32;
// LOWERING-NEXT:     let _v114: i32 = _v109 + _v113;
// LOWERING-NEXT:     let _v115: i64 = 202311;
// LOWERING-NEXT:     let _v116: i64 = 202311;
// LOWERING-NEXT:     let _v117: bool = _v115 == _v116;
// LOWERING-NEXT:     let _v118: i32 = _v117 as i32;
// LOWERING-NEXT:     let _v119: i32 = _v114 + _v118;
// LOWERING-NEXT:     header_versions = _v119;
// LOWERING-NEXT:     let _v120: i32 = integer_widths;
// LOWERING-NEXT:     let _v121: i32 = floating_limits;
// LOWERING-NEXT:     let _v122: i32 = _v120 + _v121;
// LOWERING-NEXT:     let _v123: i32 = header_versions;
// LOWERING-NEXT:     let _v124: i32 = _v122 + _v123;
// LOWERING-NEXT:     __retval = _v124;
// LOWERING-NEXT:     let _v125: i32 = __retval;
// LOWERING-NEXT:     return _v125;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: *mut i8 = b"%d %d %d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v2: i32 = c23_stdbit();
// LOWERING-NEXT:     let _v3: i32 = c23_checked_arithmetic();
// LOWERING-NEXT:     let _v4: i32 = c23_utf8();
// LOWERING-NEXT:     let _v5: i32 = c23_memory();
// LOWERING-NEXT:     let _v6: i32 = c23_time();
// LOWERING-NEXT:     let _v7: i32 = c23_io();
// LOWERING-NEXT:     let _v8: i32 = c23_limits();
// LOWERING-NEXT:     let _v9: i32 = _v7 + _v8;
// LOWERING-NEXT:     let _v10: i32 = unsafe { printf(_v1 as *const i8, _v2, _v3, _v4, _v5, _v6, _v9) };
// LOWERING-NEXT:     let _v11: i32 = 0;
// LOWERING-NEXT:     __retval = _v11;
// LOWERING-NEXT:     let _v12: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v12 as i32);
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
// LOWERING-NEXT:     fn __slate_strfroml__ri32_pi8_usize_pi8_f80(_0: *mut i8, _1: usize, _2: *const i8, _3: LongDouble) -> i32;
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
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[allow(non_camel_case_types)]
// REWRITES-NEXT: #[derive(Clone, Copy, PartialEq, Eq, Debug, Hash)]
// REWRITES-NEXT: enum memory_order {
// REWRITES-NEXT:     memory_order_relaxed = 0,
// REWRITES-NEXT:     memory_order_consume = 1,
// REWRITES-NEXT:     memory_order_acquire = 2,
// REWRITES-NEXT:     memory_order_release = 3,
// REWRITES-NEXT:     memory_order_acq_rel = 4,
// REWRITES-NEXT:     memory_order_seq_cst = 5,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct __mbstate_t {
// REWRITES-NEXT:     __count: i32,
// REWRITES-NEXT:     __value: anon_0,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: union anon_0 {
// REWRITES-NEXT:     __wch: i32,
// REWRITES-NEXT:     __wchb: [i8; 4],
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct tm {
// REWRITES-NEXT:     tm_sec: i32,
// REWRITES-NEXT:     tm_min: i32,
// REWRITES-NEXT:     tm_hour: i32,
// REWRITES-NEXT:     tm_mday: i32,
// REWRITES-NEXT:     tm_mon: i32,
// REWRITES-NEXT:     tm_year: i32,
// REWRITES-NEXT:     tm_wday: i32,
// REWRITES-NEXT:     tm_yday: i32,
// REWRITES-NEXT:     tm_isdst: i32,
// REWRITES-NEXT:     __tm_gmtoff: i64,
// REWRITES-NEXT:     __tm_zone: *mut i8,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut _str_10: [i32; 8] = [74, 97, 110, 117, 97, 114, 121, 0];
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut _str_11: [i32; 4] = [74, 97, 110, 0];
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut _str_9: [i32; 4] = [37, 79, 66, 0];
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT:     fn stdc_leading_zeros_ui(_0: u32) -> u32;
// REWRITES-NEXT:     fn stdc_leading_ones_ui(_0: u32) -> u32;
// REWRITES-NEXT:     fn stdc_trailing_zeros_ui(_0: u32) -> u32;
// REWRITES-NEXT:     fn stdc_trailing_ones_ui(_0: u32) -> u32;
// REWRITES-NEXT:     fn stdc_first_leading_zero_ui(_0: u32) -> u32;
// REWRITES-NEXT:     fn stdc_first_leading_one_ui(_0: u32) -> u32;
// REWRITES-NEXT:     fn stdc_first_trailing_zero_ui(_0: u32) -> u32;
// REWRITES-NEXT:     fn stdc_first_trailing_one_ui(_0: u32) -> u32;
// REWRITES-NEXT:     fn stdc_count_zeros_ui(_0: u32) -> u32;
// REWRITES-NEXT:     fn stdc_count_ones_ui(_0: u32) -> u32;
// REWRITES-NEXT:     fn stdc_bit_width_ui(_0: u32) -> u32;
// REWRITES-NEXT:     fn stdc_bit_floor_ui(_0: u32) -> u32;
// REWRITES-NEXT:     fn stdc_bit_ceil_ui(_0: u32) -> u32;
// REWRITES-NEXT:     fn mbrtoc8(_0: *mut u8, _1: *const i8, _2: usize, _3: *mut __mbstate_t) -> usize;
// REWRITES-NEXT:     fn c8rtomb(_0: *mut i8, _1: u8, _2: *mut __mbstate_t) -> usize;
// REWRITES-NEXT:     fn memccpy(_0: *mut core::ffi::c_void, _1: *const core::ffi::c_void, _2: i32, _3: usize) -> *mut core::ffi::c_void;
// REWRITES-NEXT:     fn memset_explicit(_0: *mut core::ffi::c_void, _1: i32, _2: usize) -> *mut core::ffi::c_void;
// REWRITES-NEXT:     fn strdup(_0: *const i8) -> *mut i8;
// REWRITES-NEXT:     fn strndup(_0: *const i8, _1: usize) -> *mut i8;
// REWRITES-NEXT:     fn strcmp(_0: *const i8, _1: *const i8) -> i32;
// REWRITES-NEXT:     fn free(_0: *mut core::ffi::c_void);
// REWRITES-NEXT:     fn strchr(_0: *const i8, _1: i32) -> *mut i8;
// REWRITES-NEXT:     fn memchr(_0: *const core::ffi::c_void, _1: i32, _2: usize) -> *mut core::ffi::c_void;
// REWRITES-NEXT:     fn strstr(_0: *const i8, _1: *const i8) -> *mut i8;
// REWRITES-NEXT:     fn gmtime_r(_0: *const i64, _1: *mut tm) -> *mut tm;
// REWRITES-NEXT:     fn localtime_r(_0: *const i64, _1: *mut tm) -> *mut tm;
// REWRITES-NEXT:     fn timespec_getres(_0: *mut libc::timespec, _1: i32) -> i32;
// REWRITES-NEXT:     fn timegm(_0: *mut tm) -> i64;
// REWRITES-NEXT:     fn strftime(_0: *mut i8, _1: usize, _2: *const i8, _3: *const tm) -> usize;
// REWRITES-NEXT:     fn wcsftime(_0: *mut i32, _1: usize, _2: *const i32, _3: *const tm) -> usize;
// REWRITES-NEXT:     fn wcscmp(_0: *const i32, _1: *const i32) -> i32;
// REWRITES-NEXT:     fn wcschr(_0: *const i32, _1: i32) -> *mut i32;
// REWRITES-NEXT:     fn wcsstr(_0: *const i32, _1: *const i32) -> *mut i32;
// REWRITES-NEXT:     fn snprintf(_0: *mut i8, _1: usize, _2: *const i8, ...) -> i32;
// REWRITES-NEXT:     fn sscanf(_0: *const i8, _1: *const i8, ...) -> i32;
// REWRITES-NEXT:     fn strfromf(_0: *mut i8, _1: usize, _2: *const i8, _3: f32) -> i32;
// REWRITES-NEXT:     fn strfromd(_0: *mut i8, _1: usize, _2: *const i8, _3: f64) -> i32;
// REWRITES-NEXT:     fn strfroml(_0: *mut i8, _1: usize, _2: *const i8, _3: LongDouble) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn c23_stdbit() -> i32 {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut value: u32 = 0;
// REWRITES-NEXT: value = 176;
// REWRITES-NEXT: let _v2: u32 = unsafe { stdc_leading_zeros_ui(value as u32) };
// REWRITES-NEXT: let _v4: u32 = unsafe { stdc_leading_ones_ui(value as u32) };
// REWRITES-NEXT: let _v5: u32 = _v2 + _v4;
// REWRITES-NEXT: let _v7: u32 = unsafe { stdc_trailing_zeros_ui(value as u32) };
// REWRITES-NEXT: let _v8: u32 = _v5 + _v7;
// REWRITES-NEXT: let _v10: u32 = unsafe { stdc_trailing_ones_ui(value as u32) };
// REWRITES-NEXT: let _v11: u32 = _v8 + _v10;
// REWRITES-NEXT: let _v13: u32 = unsafe { stdc_first_leading_zero_ui(value as u32) };
// REWRITES-NEXT: let _v14: u32 = _v11 + _v13;
// REWRITES-NEXT: let _v16: u32 = unsafe { stdc_first_leading_one_ui(value as u32) };
// REWRITES-NEXT: let _v17: u32 = _v14 + _v16;
// REWRITES-NEXT: let _v19: u32 = unsafe { stdc_first_trailing_zero_ui(value as u32) };
// REWRITES-NEXT: let _v20: u32 = _v17 + _v19;
// REWRITES-NEXT: let _v22: u32 = unsafe { stdc_first_trailing_one_ui(value as u32) };
// REWRITES-NEXT: let _v23: u32 = _v20 + _v22;
// REWRITES-NEXT: let _v25: u32 = unsafe { stdc_count_zeros_ui(value as u32) };
// REWRITES-NEXT: let _v26: u32 = _v23 + _v25;
// REWRITES-NEXT: let _v28: u32 = unsafe { stdc_count_ones_ui(value as u32) };
// REWRITES-NEXT: let _v32: u32 = _v26 + _v28 + (true as u32);
// REWRITES-NEXT: let _v34: u32 = unsafe { stdc_bit_width_ui(value as u32) };
// REWRITES-NEXT: let _v35: u32 = _v32 + _v34;
// REWRITES-NEXT: let _v37: u32 = unsafe { stdc_bit_floor_ui(value as u32) };
// REWRITES-NEXT: let _v38: u32 = _v35 + _v37;
// REWRITES-NEXT: let _v40: u32 = unsafe { stdc_bit_ceil_ui(value as u32) };
// REWRITES-NEXT: __retval = (_v38 + _v40) as i32;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn c23_checked_arithmetic() -> i32 {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut result: i32 = 0;
// REWRITES-NEXT: let mut total: i32 = 0;
// REWRITES-NEXT: total = 0;
// REWRITES-NEXT: let _v1: i32 = 20;
// REWRITES-NEXT: let _v2: i32 = 22;
// REWRITES-NEXT: let _v3 = _v1.overflowing_add(_v2);
// REWRITES-NEXT: let _v5: bool = _v3.1 || (_v3.0 < -2147483648 || _v3.0 > 2147483647);
// REWRITES-NEXT: result = _v3.0 as i32;
// REWRITES-NEXT: let _v6: bool = !_v5;
// REWRITES-NEXT: let _v7: bool = if _v6 {
// REWRITES-NEXT:         let _v9: i32 = 42;
// REWRITES-NEXT:         let _v10: bool = result == _v9;
// REWRITES-NEXT:     _v10
// REWRITES-NEXT: } else {
// REWRITES-NEXT:         let _v11: bool = false;
// REWRITES-NEXT:     _v11
// REWRITES-NEXT: };
// REWRITES-NEXT: total = total + (_v7 as i32);
// REWRITES-NEXT: let _v15: i32 = 50;
// REWRITES-NEXT: let _v16: i32 = 8;
// REWRITES-NEXT: let _v17 = _v15.overflowing_sub(_v16);
// REWRITES-NEXT: let _v19: bool = _v17.1 || (_v17.0 < -2147483648 || _v17.0 > 2147483647);
// REWRITES-NEXT: result = _v17.0 as i32;
// REWRITES-NEXT: let _v20: bool = !_v19;
// REWRITES-NEXT: let _v21: bool = if _v20 {
// REWRITES-NEXT:         let _v23: i32 = 42;
// REWRITES-NEXT:         let _v24: bool = result == _v23;
// REWRITES-NEXT:     _v24
// REWRITES-NEXT: } else {
// REWRITES-NEXT:         let _v25: bool = false;
// REWRITES-NEXT:     _v25
// REWRITES-NEXT: };
// REWRITES-NEXT: total = total + (_v21 as i32);
// REWRITES-NEXT: let _v29: i32 = 6;
// REWRITES-NEXT: let _v30: i32 = 7;
// REWRITES-NEXT: let _v31 = _v29.overflowing_mul(_v30);
// REWRITES-NEXT: let _v33: bool = _v31.1 || (_v31.0 < -2147483648 || _v31.0 > 2147483647);
// REWRITES-NEXT: result = _v31.0 as i32;
// REWRITES-NEXT: let _v34: bool = !_v33;
// REWRITES-NEXT: let _v35: bool = if _v34 {
// REWRITES-NEXT:         let _v37: i32 = 42;
// REWRITES-NEXT:         let _v38: bool = result == _v37;
// REWRITES-NEXT:     _v38
// REWRITES-NEXT: } else {
// REWRITES-NEXT:         let _v39: bool = false;
// REWRITES-NEXT:     _v39
// REWRITES-NEXT: };
// REWRITES-NEXT: total = total + (_v35 as i32);
// REWRITES-NEXT: let _v43: i32 = 2147483647;
// REWRITES-NEXT: let _v44: i32 = 1;
// REWRITES-NEXT: let _v45 = _v43.overflowing_add(_v44);
// REWRITES-NEXT: let _v47: bool = _v45.1 || (_v45.0 < -2147483648 || _v45.0 > 2147483647);
// REWRITES-NEXT: result = _v45.0 as i32;
// REWRITES-NEXT: total = total + (_v47 as i32);
// REWRITES-NEXT: __retval = total;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn c23_utf8() -> i32 {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut input_state: __mbstate_t = __mbstate_t { __count: 0, __value: anon_0 { __wch: 0 } };
// REWRITES-NEXT: let mut output_state: __mbstate_t = __mbstate_t { __count: 0, __value: anon_0 { __wch: 0 } };
// REWRITES-NEXT: let mut character: u8 = 0;
// REWRITES-NEXT: let mut output: [i8; 4] = [0; 4];
// REWRITES-NEXT: let mut input_size: u64 = 0;
// REWRITES-NEXT: let mut output_size: u64 = 0;
// REWRITES-NEXT: let mut atomic_character: u8 = 0;
// REWRITES-NEXT: character = 0;
// REWRITES-NEXT: output = [0, 0, 0, 0];
// REWRITES-NEXT: let _v1: *mut i8 = b"A\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v2: u64 = 1;
// REWRITES-NEXT: input_size = (unsafe { mbrtoc8(std::ptr::addr_of_mut!(character) as *mut u8, _v1 as *const i8, _v2 as usize, std::ptr::addr_of_mut!(input_state) as *mut __mbstate_t) }) as u64;
// REWRITES-NEXT: let _v4: *mut i8 = output.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: output_size = (unsafe { c8rtomb(_v4 as *mut i8, character as u8, std::ptr::addr_of_mut!(output_state) as *mut __mbstate_t) }) as u64;
// REWRITES-NEXT: atomic_character = character;
// REWRITES-NEXT: let _v8: u8 = 66;
// REWRITES-NEXT: unsafe { std::sync::atomic::AtomicU8::from_ptr(std::ptr::addr_of_mut!(atomic_character)).store(_v8, std::sync::atomic::Ordering::SeqCst) };
// REWRITES-NEXT: let _v10: u64 = 1;
// REWRITES-NEXT: let _v14: u64 = 1;
// REWRITES-NEXT: let _v18: i64 = 0;
// REWRITES-NEXT: let _v21: i32 = 65;
// REWRITES-NEXT: let _v24: i32 = ((input_size == _v10) as i32) + ((output_size == _v14) as i32) + (((output[(_v18 as usize)] as i32) == _v21) as i32);
// REWRITES-NEXT: let _v25: u8 = unsafe { std::sync::atomic::AtomicU8::from_ptr(std::ptr::addr_of_mut!(atomic_character)).load(std::sync::atomic::Ordering::SeqCst) };
// REWRITES-NEXT: let _v27: i32 = 66;
// REWRITES-NEXT: let _v31: i32 = 2;
// REWRITES-NEXT: let _v32: i32 = 0;
// REWRITES-NEXT: __retval = _v24 + (((_v25 as i32) == _v27) as i32) + ((_v31 > _v32) as i32);
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn c23_memory() -> i32 {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut source: [i8; 7] = [97, 98, 99, 100, 101, 102, 0];
// REWRITES-NEXT: let mut destination: [i8; 8] = [0, 0, 0, 0, 0, 0, 0, 0];
// REWRITES-NEXT: let mut secret: [i8; 7] = [115, 101, 99, 114, 101, 116, 0];
// REWRITES-NEXT: let mut first_copy: *mut i8 = std::ptr::null_mut();
// REWRITES-NEXT: let mut second_copy: *mut i8 = std::ptr::null_mut();
// REWRITES-NEXT: let mut phrase: *mut i8 = std::ptr::null_mut();
// REWRITES-NEXT: let mut mutable_phrase: [i8; 12] = [0; 12];
// REWRITES-NEXT: let mut stop: *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-NEXT: let mut total: i32 = 0;
// REWRITES-NEXT: let mut const_hit: *mut i8 = std::ptr::null_mut();
// REWRITES-NEXT: let mut mut_hit: *mut i8 = std::ptr::null_mut();
// REWRITES-NEXT: phrase = b"hello world\0".as_ptr() as *mut i8;
// REWRITES-NEXT: mutable_phrase = [104, 101, 108, 108, 111, 32, 119, 111, 114, 108, 100, 0];
// REWRITES-NEXT: let _v1: *mut i8 = destination.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let _v2: *mut core::ffi::c_void = _v1 as *mut core::ffi::c_void;
// REWRITES-NEXT: let _v3: *mut i8 = source.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let _v5: i32 = 99;
// REWRITES-NEXT: let _v6: u64 = 6;
// REWRITES-NEXT: stop = unsafe { memccpy(_v2 as *mut core::ffi::c_void, (_v3 as *mut core::ffi::c_void) as *const core::ffi::c_void, _v5 as i32, _v6 as usize) };
// REWRITES-NEXT: let _v8: *mut core::ffi::c_void = stop;
// REWRITES-NEXT: let _v9: *mut i8 = destination.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let _v10: i32 = 3;
// REWRITES-NEXT: let _v11: *mut i8 = unsafe { _v9.add(3) };
// REWRITES-NEXT: let _v14: bool = if _v8 == (_v11 as *mut core::ffi::c_void) {
// REWRITES-NEXT:         let _v15: i64 = 2;
// REWRITES-NEXT:         let _v18: i32 = 99;
// REWRITES-NEXT:         let _v19: bool = (destination[(_v15 as usize)] as i32) == _v18;
// REWRITES-NEXT:     _v19
// REWRITES-NEXT: } else {
// REWRITES-NEXT:         let _v20: bool = false;
// REWRITES-NEXT:     _v20
// REWRITES-NEXT: };
// REWRITES-NEXT: total = _v14 as i32;
// REWRITES-NEXT: let _v22: *mut i8 = secret.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let _v24: i32 = 0;
// REWRITES-NEXT: let _v25: u64 = 7;
// REWRITES-NEXT: let _v26: *mut core::ffi::c_void = unsafe { memset_explicit((_v22 as *mut core::ffi::c_void) as *mut core::ffi::c_void, _v24 as i32, _v25 as usize) };
// REWRITES-NEXT: let _v27: i64 = 0;
// REWRITES-NEXT: let _v30: i32 = 0;
// REWRITES-NEXT: let _v32: bool = if (secret[(_v27 as usize)] as i32) == _v30 {
// REWRITES-NEXT:         let _v33: i64 = 5;
// REWRITES-NEXT:         let _v36: i32 = 0;
// REWRITES-NEXT:         let _v37: bool = (secret[(_v33 as usize)] as i32) == _v36;
// REWRITES-NEXT:     _v37
// REWRITES-NEXT: } else {
// REWRITES-NEXT:         let _v38: bool = false;
// REWRITES-NEXT:     _v38
// REWRITES-NEXT: };
// REWRITES-NEXT: total = total + (_v32 as i32);
// REWRITES-NEXT: let _v42: *mut i8 = b"c23\0".as_ptr() as *mut i8;
// REWRITES-NEXT: first_copy = unsafe { strdup(_v42 as *const i8) };
// REWRITES-NEXT: let _v44: *mut i8 = b"library\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v45: u64 = 3;
// REWRITES-NEXT: second_copy = unsafe { strndup(_v44 as *const i8, _v45 as usize) };
// REWRITES-NEXT: let _v48: *mut i8 = std::ptr::null_mut();
// REWRITES-NEXT: let _v49: bool = first_copy != _v48;
// REWRITES-NEXT: let _v50: bool = if _v49 {
// REWRITES-NEXT:         let _v51: *mut i8 = first_copy;
// REWRITES-NEXT:         let _v52: *mut i8 = b"c23\0".as_ptr() as *mut i8;
// REWRITES-NEXT:         let _v53: i32 = unsafe { strcmp(_v51 as *const i8, _v52 as *const i8) };
// REWRITES-NEXT:         let _v54: i32 = 0;
// REWRITES-NEXT:         let _v55: bool = _v53 == _v54;
// REWRITES-NEXT:     _v55
// REWRITES-NEXT: } else {
// REWRITES-NEXT:         let _v56: bool = false;
// REWRITES-NEXT:     _v56
// REWRITES-NEXT: };
// REWRITES-NEXT: total = total + (_v50 as i32);
// REWRITES-NEXT: let _v61: *mut i8 = std::ptr::null_mut();
// REWRITES-NEXT: let _v62: bool = second_copy != _v61;
// REWRITES-NEXT: let _v63: bool = if _v62 {
// REWRITES-NEXT:         let _v64: *mut i8 = second_copy;
// REWRITES-NEXT:         let _v65: *mut i8 = b"lib\0".as_ptr() as *mut i8;
// REWRITES-NEXT:         let _v66: i32 = unsafe { strcmp(_v64 as *const i8, _v65 as *const i8) };
// REWRITES-NEXT:         let _v67: i32 = 0;
// REWRITES-NEXT:         let _v68: bool = _v66 == _v67;
// REWRITES-NEXT:     _v68
// REWRITES-NEXT: } else {
// REWRITES-NEXT:         let _v69: bool = false;
// REWRITES-NEXT:     _v69
// REWRITES-NEXT: };
// REWRITES-NEXT: total = total + (_v63 as i32);
// REWRITES-NEXT: unsafe { free((first_copy as *mut core::ffi::c_void) as *mut core::ffi::c_void) };
// REWRITES-NEXT: unsafe { free((second_copy as *mut core::ffi::c_void) as *mut core::ffi::c_void) };
// REWRITES-NEXT: let _v78: i32 = 119;
// REWRITES-NEXT: const_hit = unsafe { strchr(phrase as *const i8, _v78 as i32) };
// REWRITES-NEXT: let _v80: *mut i8 = mutable_phrase.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let _v81: i32 = 119;
// REWRITES-NEXT: mut_hit = unsafe { strchr(_v80 as *const i8, _v81 as i32) };
// REWRITES-NEXT: let _v84: *mut i8 = std::ptr::null_mut();
// REWRITES-NEXT: let _v86: bool = if const_hit != _v84 {
// REWRITES-NEXT:         let _v88: *mut i8 = std::ptr::null_mut();
// REWRITES-NEXT:         let _v89: bool = mut_hit != _v88;
// REWRITES-NEXT:     _v89
// REWRITES-NEXT: } else {
// REWRITES-NEXT:         let _v90: bool = false;
// REWRITES-NEXT:     _v90
// REWRITES-NEXT: };
// REWRITES-NEXT: total = total + (_v86 as i32);
// REWRITES-NEXT: let _v96: i32 = 111;
// REWRITES-NEXT: let _v97: u64 = 11;
// REWRITES-NEXT: let _v98: *mut core::ffi::c_void = unsafe { memchr((phrase as *mut core::ffi::c_void) as *const core::ffi::c_void, _v96 as i32, _v97 as usize) };
// REWRITES-NEXT: let _v99: *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-NEXT: total = total + ((_v98 != _v99) as i32);
// REWRITES-NEXT: let _v104: *mut i8 = mutable_phrase.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let _v106: i32 = 111;
// REWRITES-NEXT: let _v107: u64 = 11;
// REWRITES-NEXT: let _v108: *mut core::ffi::c_void = unsafe { memchr((_v104 as *mut core::ffi::c_void) as *const core::ffi::c_void, _v106 as i32, _v107 as usize) };
// REWRITES-NEXT: let _v109: *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-NEXT: total = total + ((_v108 != _v109) as i32);
// REWRITES-NEXT: let _v114: *mut i8 = phrase;
// REWRITES-NEXT: let _v115: *mut i8 = b"world\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v116: *mut i8 = unsafe { strstr(_v114 as *const i8, _v115 as *const i8) };
// REWRITES-NEXT: let _v117: *mut i8 = std::ptr::null_mut();
// REWRITES-NEXT: total = total + ((_v116 != _v117) as i32);
// REWRITES-NEXT: let _v122: *mut i8 = mutable_phrase.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let _v123: *mut i8 = b"world\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v124: *mut i8 = unsafe { strstr(_v122 as *const i8, _v123 as *const i8) };
// REWRITES-NEXT: let _v125: *mut i8 = std::ptr::null_mut();
// REWRITES-NEXT: total = total + ((_v124 != _v125) as i32);
// REWRITES-NEXT: __retval = total;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn c23_time() -> i32 {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut timestamp: i64 = 0;
// REWRITES-NEXT: let mut utc: tm = tm { tm_sec: 0, tm_min: 0, tm_hour: 0, tm_mday: 0, tm_mon: 0, tm_year: 0, tm_wday: 0, tm_yday: 0, tm_isdst: 0, __tm_gmtoff: 0, __tm_zone: std::ptr::null_mut() };
// REWRITES-NEXT: let mut local: tm = tm { tm_sec: 0, tm_min: 0, tm_hour: 0, tm_mday: 0, tm_mon: 0, tm_year: 0, tm_wday: 0, tm_yday: 0, tm_isdst: 0, __tm_gmtoff: 0, __tm_zone: std::ptr::null_mut() };
// REWRITES-NEXT: let mut resolution: libc::timespec = libc::timespec { tv_sec: 0, tv_nsec: 0 };
// REWRITES-NEXT: let mut month: aligned::Aligned<aligned::A16, [i8; 32]> = aligned::Aligned([0; 32]);
// REWRITES-NEXT: let mut wide_month: aligned::Aligned<aligned::A16, [i32; 32]> = aligned::Aligned([0; 32]);
// REWRITES-NEXT: let mut total: i32 = 0;
// REWRITES-NEXT: let mut const_month: *mut i32 = std::ptr::null_mut();
// REWRITES-NEXT: timestamp = 0;
// REWRITES-NEXT: utc = tm { tm_sec: 0, tm_min: 0, tm_hour: 0, tm_mday: 0, tm_mon: 0, tm_year: 0, tm_wday: 0, tm_yday: 0, tm_isdst: 0, __tm_gmtoff: 0, __tm_zone: std::ptr::null_mut() };
// REWRITES-NEXT: local = tm { tm_sec: 0, tm_min: 0, tm_hour: 0, tm_mday: 0, tm_mon: 0, tm_year: 0, tm_wday: 0, tm_yday: 0, tm_isdst: 0, __tm_gmtoff: 0, __tm_zone: std::ptr::null_mut() };
// REWRITES-NEXT: resolution = libc::timespec { tv_sec: 0, tv_nsec: 0 };
// REWRITES-NEXT: *month = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
// REWRITES-NEXT: *wide_month = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
// REWRITES-NEXT: let _v1: *mut tm = unsafe { gmtime_r(std::ptr::addr_of_mut!(timestamp) as *const i64, std::ptr::addr_of_mut!(utc) as *mut tm) };
// REWRITES-NEXT: let _v2: bool = _v1 == std::ptr::addr_of_mut!(utc);
// REWRITES-NEXT: total = _v2 as i32;
// REWRITES-NEXT: let _v4: *mut tm = unsafe { localtime_r(std::ptr::addr_of_mut!(timestamp) as *const i64, std::ptr::addr_of_mut!(local) as *mut tm) };
// REWRITES-NEXT: let _v5: bool = _v4 == std::ptr::addr_of_mut!(local);
// REWRITES-NEXT: total = total + (_v5 as i32);
// REWRITES-NEXT: let _v9: i32 = 1;
// REWRITES-NEXT: let _v10: i32 = unsafe { timespec_getres(std::ptr::addr_of_mut!(resolution) as *mut libc::timespec, _v9 as i32) };
// REWRITES-NEXT: let _v11: i32 = 1;
// REWRITES-NEXT: total = total + ((_v10 == _v11) as i32);
// REWRITES-NEXT: let _v17: i64 = 0;
// REWRITES-NEXT: let _v19: bool = if resolution.tv_sec > _v17 {
// REWRITES-NEXT:         let _v20: bool = true;
// REWRITES-NEXT:     _v20
// REWRITES-NEXT: } else {
// REWRITES-NEXT:         let _v22: i64 = 0;
// REWRITES-NEXT:         let _v23: bool = resolution.tv_nsec > _v22;
// REWRITES-NEXT:     _v23
// REWRITES-NEXT: };
// REWRITES-NEXT: total = total + (_v19 as i32);
// REWRITES-NEXT: let _v27: i64 = unsafe { timegm(std::ptr::addr_of_mut!(utc) as *mut tm) };
// REWRITES-NEXT: let _v28: i64 = 0;
// REWRITES-NEXT: total = total + ((_v27 == _v28) as i32);
// REWRITES-NEXT: let _v33: *mut i8 = month.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let _v34: u64 = 32;
// REWRITES-NEXT: let _v35: *mut i8 = b"%OB\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v36: u64 = (unsafe { strftime(_v33 as *mut i8, _v34 as usize, _v35 as *const i8, std::ptr::addr_of_mut!(utc) as *const tm) }) as u64;
// REWRITES-NEXT: let _v37: u64 = 7;
// REWRITES-NEXT: total = total + ((_v36 == _v37) as i32);
// REWRITES-NEXT: let _v42: *mut i8 = month.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let _v43: *mut i8 = b"January\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v44: i32 = unsafe { strcmp(_v42 as *const i8, _v43 as *const i8) };
// REWRITES-NEXT: let _v45: i32 = 0;
// REWRITES-NEXT: total = total + ((_v44 == _v45) as i32);
// REWRITES-NEXT: let _v50: *mut i32 = wide_month.as_mut_ptr() as *mut i32;
// REWRITES-NEXT: let _v51: u64 = 32;
// REWRITES-NEXT: let _v52: *mut i32 = std::ptr::addr_of_mut!(_str_9).cast::<i32>();
// REWRITES-NEXT: let _v53: u64 = (unsafe { wcsftime(_v50 as *mut i32, _v51 as usize, _v52 as *const i32, std::ptr::addr_of_mut!(utc) as *const tm) }) as u64;
// REWRITES-NEXT: let _v54: u64 = 7;
// REWRITES-NEXT: total = total + ((_v53 == _v54) as i32);
// REWRITES-NEXT: let _v59: *mut i32 = wide_month.as_mut_ptr() as *mut i32;
// REWRITES-NEXT: let _v60: *mut i32 = std::ptr::addr_of_mut!(_str_10).cast::<i32>();
// REWRITES-NEXT: let _v61: i32 = unsafe { wcscmp(_v59 as *const i32, _v60 as *const i32) };
// REWRITES-NEXT: let _v62: i32 = 0;
// REWRITES-NEXT: total = total + ((_v61 == _v62) as i32);
// REWRITES-NEXT: const_month = wide_month.as_mut_ptr() as *mut i32;
// REWRITES-NEXT: let _v69: i32 = 110;
// REWRITES-NEXT: let _v70: *mut i32 = unsafe { wcschr(const_month as *const i32, _v69 as i32) };
// REWRITES-NEXT: let _v71: *mut i32 = std::ptr::null_mut();
// REWRITES-NEXT: total = total + ((_v70 != _v71) as i32);
// REWRITES-NEXT: let _v76: *mut i32 = wide_month.as_mut_ptr() as *mut i32;
// REWRITES-NEXT: let _v77: i32 = 110;
// REWRITES-NEXT: let _v78: *mut i32 = unsafe { wcschr(_v76 as *const i32, _v77 as i32) };
// REWRITES-NEXT: let _v79: *mut i32 = std::ptr::null_mut();
// REWRITES-NEXT: total = total + ((_v78 != _v79) as i32);
// REWRITES-NEXT: let _v84: *mut i32 = const_month;
// REWRITES-NEXT: let _v85: *mut i32 = std::ptr::addr_of_mut!(_str_11).cast::<i32>();
// REWRITES-NEXT: let _v86: *mut i32 = unsafe { wcsstr(_v84 as *const i32, _v85 as *const i32) };
// REWRITES-NEXT: let _v87: *mut i32 = std::ptr::null_mut();
// REWRITES-NEXT: total = total + ((_v86 != _v87) as i32);
// REWRITES-NEXT: let _v92: *mut i32 = wide_month.as_mut_ptr() as *mut i32;
// REWRITES-NEXT: let _v93: *mut i32 = std::ptr::addr_of_mut!(_str_11).cast::<i32>();
// REWRITES-NEXT: let _v94: *mut i32 = unsafe { wcsstr(_v92 as *const i32, _v93 as *const i32) };
// REWRITES-NEXT: let _v95: *mut i32 = std::ptr::null_mut();
// REWRITES-NEXT: total = total + ((_v94 != _v95) as i32);
// REWRITES-NEXT: __retval = total;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn c23_io() -> i32 {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut output: aligned::Aligned<aligned::A16, [i8; 64]> = aligned::Aligned([0; 64]);
// REWRITES-NEXT: let mut float_output: aligned::Aligned<aligned::A16, [i8; 16]> = aligned::Aligned([0; 16]);
// REWRITES-NEXT: let mut double_output: aligned::Aligned<aligned::A16, [i8; 16]> = aligned::Aligned([0; 16]);
// REWRITES-NEXT: let mut long_double_output: aligned::Aligned<aligned::A16, [i8; 16]> = aligned::Aligned([0; 16]);
// REWRITES-NEXT: let mut binary_value: u32 = 0;
// REWRITES-NEXT: let mut exact_value: u16 = 0;
// REWRITES-NEXT: let mut fast_value: u64 = 0;
// REWRITES-NEXT: let mut written: i32 = 0;
// REWRITES-NEXT: let mut scanned: i32 = 0;
// REWRITES-NEXT: let mut floating_written: i32 = 0;
// REWRITES-NEXT: *output = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
// REWRITES-NEXT: *float_output = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
// REWRITES-NEXT: *double_output = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
// REWRITES-NEXT: *long_double_output = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
// REWRITES-NEXT: binary_value = 0;
// REWRITES-NEXT: exact_value = 0;
// REWRITES-NEXT: fast_value = 0;
// REWRITES-NEXT: let _v3: *mut i8 = output.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let _v4: u64 = 64;
// REWRITES-NEXT: let _v5: *mut i8 = b"%b %w16u %wf16u\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v6: u32 = 13;
// REWRITES-NEXT: let _v7: i32 = 21;
// REWRITES-NEXT: let _v8: u64 = 34;
// REWRITES-NEXT: written = unsafe { snprintf(_v3 as *mut i8, _v4 as usize, _v5 as *const i8, _v6, _v7, _v8) };
// REWRITES-NEXT: let _v10: *mut i8 = b"1011 55 89\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v11: *mut i8 = b"%b %w16u %wf16u\0".as_ptr() as *mut i8;
// REWRITES-NEXT: scanned = unsafe { sscanf(_v10 as *const i8, _v11 as *const i8, std::ptr::addr_of_mut!(binary_value), std::ptr::addr_of_mut!(exact_value), std::ptr::addr_of_mut!(fast_value)) };
// REWRITES-NEXT: let _v13: *mut i8 = float_output.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let _v14: u64 = 16;
// REWRITES-NEXT: let _v15: *mut i8 = b"%.1f\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v16: f32 = 1.5;
// REWRITES-NEXT: let _v17: i32 = unsafe { strfromf(_v13 as *mut i8, _v14 as usize, _v15 as *const i8, _v16 as f32) };
// REWRITES-NEXT: let _v18: *mut i8 = double_output.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let _v19: u64 = 16;
// REWRITES-NEXT: let _v20: *mut i8 = b"%.1f\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v21: f64 = 2.5;
// REWRITES-NEXT: let _v22: i32 = unsafe { strfromd(_v18 as *mut i8, _v19 as usize, _v20 as *const i8, _v21 as f64) };
// REWRITES-NEXT: let _v23: i32 = _v17 + _v22;
// REWRITES-NEXT: let _v24: *mut i8 = long_double_output.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let _v25: u64 = 16;
// REWRITES-NEXT: let _v26: *mut i8 = b"%.1f\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v27: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 224, 0, 64]);
// REWRITES-NEXT: let _v28: i32 = unsafe { __slate_strfroml__ri32_pi8_usize_pi8_f80(_v24 as *mut i8, _v25 as usize, _v26 as *const i8, _v27) };
// REWRITES-NEXT: floating_written = _v23 + _v28;
// REWRITES-NEXT: let _v30: i32 = written;
// REWRITES-NEXT: let _v31: *mut i8 = output.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let _v32: *mut i8 = b"1101 21 34\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v33: i32 = unsafe { strcmp(_v31 as *const i8, _v32 as *const i8) };
// REWRITES-NEXT: let _v34: i32 = 0;
// REWRITES-NEXT: let _v41: u32 = 11;
// REWRITES-NEXT: let _v47: i32 = 55;
// REWRITES-NEXT: let _v52: u64 = 89;
// REWRITES-NEXT: let _v57: i32 = _v30 + ((_v33 == _v34) as i32) + scanned + ((binary_value == _v41) as i32) + (((exact_value as i32) == _v47) as i32) + ((fast_value == _v52) as i32) + floating_written;
// REWRITES-NEXT: let _v58: *mut i8 = float_output.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let _v59: *mut i8 = b"1.5\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v60: i32 = unsafe { strcmp(_v58 as *const i8, _v59 as *const i8) };
// REWRITES-NEXT: let _v61: i32 = 0;
// REWRITES-NEXT: let _v64: i32 = _v57 + ((_v60 == _v61) as i32);
// REWRITES-NEXT: let _v65: *mut i8 = double_output.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let _v66: *mut i8 = b"2.5\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v67: i32 = unsafe { strcmp(_v65 as *const i8, _v66 as *const i8) };
// REWRITES-NEXT: let _v68: i32 = 0;
// REWRITES-NEXT: let _v71: i32 = _v64 + ((_v67 == _v68) as i32);
// REWRITES-NEXT: let _v72: *mut i8 = long_double_output.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let _v73: *mut i8 = b"3.5\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v74: i32 = unsafe { strcmp(_v72 as *const i8, _v73 as *const i8) };
// REWRITES-NEXT: let _v75: i32 = 0;
// REWRITES-NEXT: __retval = _v71 + ((_v74 == _v75) as i32);
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn c23_limits() -> i32 {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut integer_widths: i32 = 0;
// REWRITES-NEXT: let mut floating_limits: i32 = 0;
// REWRITES-NEXT: let mut header_versions: i32 = 0;
// REWRITES-NEXT: let _v0: i32 = 1;
// REWRITES-NEXT: let _v1: i32 = 8;
// REWRITES-NEXT: let _v3: i32 = 8;
// REWRITES-NEXT: let _v5: i32 = 8;
// REWRITES-NEXT: let _v7: i32 = 16;
// REWRITES-NEXT: let _v9: i32 = 16;
// REWRITES-NEXT: let _v11: i32 = 32;
// REWRITES-NEXT: let _v13: i32 = 32;
// REWRITES-NEXT: let _v15: i32 = 64;
// REWRITES-NEXT: let _v17: i32 = 64;
// REWRITES-NEXT: let _v19: i32 = 64;
// REWRITES-NEXT: let _v21: i32 = 64;
// REWRITES-NEXT: let _v23: i32 = 64;
// REWRITES-NEXT: let _v25: i32 = 64;
// REWRITES-NEXT: integer_widths = _v0 + _v1 + _v3 + _v5 + _v7 + _v9 + _v11 + _v13 + _v15 + _v17 + _v19 + _v21 + _v23 + _v25;
// REWRITES-NEXT: let _v27: f32 = 0.0000000000000000000000000000000000000000000014013;
// REWRITES-NEXT: let _v28: f32 = 0.0;
// REWRITES-NEXT: let _v31: f64 = 0.000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000005;
// REWRITES-NEXT: let _v32: f64 = 0.0;
// REWRITES-NEXT: let _v41: f32 = 340282346999999984391321947108527833088.0;
// REWRITES-NEXT: let _v42: f32 = 340282346999999984391321947108527833088.0;
// REWRITES-NEXT: let _v46: f64 = 179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368.0;
// REWRITES-NEXT: let _v47: f64 = 179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368.0;
// REWRITES-NEXT: let _v56: i32 = 1;
// REWRITES-NEXT: let _v57: i32 = -1;
// REWRITES-NEXT: let _v61: i32 = 1;
// REWRITES-NEXT: let _v62: i32 = -1;
// REWRITES-NEXT: let _v66: i32 = 1;
// REWRITES-NEXT: let _v67: i32 = -1;
// REWRITES-NEXT: let _v71: u64 = 4;
// REWRITES-NEXT: let _v72: u64 = 4;
// REWRITES-NEXT: let _v76: u64 = 8;
// REWRITES-NEXT: let _v77: u64 = 8;
// REWRITES-NEXT: let _v81: u64 = 16;
// REWRITES-NEXT: let _v82: u64 = 16;
// REWRITES-NEXT: floating_limits = ((_v27 > _v28) as i32) + ((_v31 > _v32) as i32) + ((LongDouble([1, 0, 0, 0, 0, 0, 0, 0, 0, 0]) > LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0])) as i32) + ((_v41 <= _v42) as i32) + ((_v46 <= _v47) as i32) + ((LongDouble([255, 255, 255, 255, 255, 255, 255, 255, 254, 127]) <= LongDouble([255, 255, 255, 255, 255, 255, 255, 255, 254, 127])) as i32) + ((_v56 >= _v57) as i32) + ((_v61 >= _v62) as i32) + ((_v66 >= _v67) as i32) + ((_v71 == _v72) as i32) + ((_v76 == _v77) as i32) + ((_v81 == _v82) as i32);
// REWRITES-NEXT: let _v86: i64 = 202311;
// REWRITES-NEXT: let _v87: i64 = 202311;
// REWRITES-NEXT: let _v90: i64 = 202311;
// REWRITES-NEXT: let _v91: i64 = 202311;
// REWRITES-NEXT: let _v95: i64 = 202311;
// REWRITES-NEXT: let _v96: i64 = 202311;
// REWRITES-NEXT: let _v100: i64 = 202311;
// REWRITES-NEXT: let _v101: i64 = 202311;
// REWRITES-NEXT: let _v105: i64 = 202311;
// REWRITES-NEXT: let _v106: i64 = 202311;
// REWRITES-NEXT: let _v110: i64 = 202311;
// REWRITES-NEXT: let _v111: i64 = 202311;
// REWRITES-NEXT: let _v115: i64 = 202311;
// REWRITES-NEXT: let _v116: i64 = 202311;
// REWRITES-NEXT: header_versions = ((_v86 == _v87) as i32) + ((_v90 == _v91) as i32) + ((_v95 == _v96) as i32) + ((_v100 == _v101) as i32) + ((_v105 == _v106) as i32) + ((_v110 == _v111) as i32) + ((_v115 == _v116) as i32);
// REWRITES-NEXT: __retval = integer_widths + floating_limits + header_versions;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: let _v1: *mut i8 = b"%d %d %d %d %d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v2: i32 = c23_stdbit();
// REWRITES-NEXT: let _v3: i32 = c23_checked_arithmetic();
// REWRITES-NEXT: let _v4: i32 = c23_utf8();
// REWRITES-NEXT: let _v5: i32 = c23_memory();
// REWRITES-NEXT: let _v6: i32 = c23_time();
// REWRITES-NEXT: let _v7: i32 = c23_io();
// REWRITES-NEXT: let _v8: i32 = c23_limits();
// REWRITES-NEXT: let _v10: i32 = unsafe { printf(_v1 as *const i8, _v2, _v3, _v4, _v5, _v6, _v7 + _v8) };
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
// REWRITES-NEXT:     fn __slate_strfroml__ri32_pi8_usize_pi8_f80(_0: *mut i8, _1: usize, _2: *const i8, _3: LongDouble) -> i32;
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
