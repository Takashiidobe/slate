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
  const char *phrase           = "hello world";
  char        mutable_phrase[] = "hello world";
  void       *stop             = memccpy(destination, source, 'c', 6);
  int         total = stop == destination + 3 && destination[2] == 'c';
  memset_explicit(secret, 0, sizeof(secret));
  total       += secret[0] == 0 && secret[5] == 0;
  first_copy   = strdup("c23");
  second_copy  = strndup("library", 3);
  total       += first_copy != nullptr && strcmp(first_copy, "c23") == 0;
  total       += second_copy != nullptr && strcmp(second_copy, "lib") == 0;
  free(first_copy);
  free(second_copy);

  const char *const_hit  = strchr(phrase, 'w');
  char       *mut_hit    = strchr(mutable_phrase, 'w');
  total                 += const_hit != nullptr && mut_hit != nullptr;
  total                 += memchr(phrase, 'o', 11) != nullptr;
  total                 += memchr(mutable_phrase, 'o', 11) != nullptr;
  total                 += strstr(phrase, "world") != nullptr;
  total                 += strstr(mutable_phrase, "world") != nullptr;
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

  const wchar_t *const_month  = wide_month;
  total                      += wcschr(const_month, L'n') != nullptr;
  total                      += wcschr(wide_month, L'n') != nullptr;
  total                      += wcsstr(const_month, L"Jan") != nullptr;
  total                      += wcsstr(wide_month, L"Jan") != nullptr;
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

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT:     fn snprintf(__s: *mut i8, __maxlen: u64, __format: *const i8, ...) -> i32;
// CHECK-NEXT:     fn sscanf(__s: *const i8, __format: *const i8, ...) -> i32;
// CHECK-NEXT:     fn strfromd(__dest: *mut i8, __size: u64, __format: *const i8, __f: f64) -> i32;
// CHECK-NEXT:     fn strfromf(__dest: *mut i8, __size: u64, __format: *const i8, __f: f32) -> i32;
// CHECK-NEXT:     fn free(__ptr: *mut ());
// CHECK-NEXT:     fn memccpy(__dest: *mut (), __src: *const (), __c: i32, __n: u64) -> *mut ();
// CHECK-NEXT:     fn memset_explicit(__s: *mut (), __c: i32, __n: u64) -> *mut ();
// CHECK-NEXT:     fn memchr(__s: *const (), __c: i32, __n: u64) -> *mut ();
// CHECK-NEXT:     fn strcmp(__s1: *const i8, __s2: *const i8) -> i32;
// CHECK-NEXT:     fn strdup(__s: *const i8) -> *mut i8;
// CHECK-NEXT:     fn strndup(__string: *const i8, __n: u64) -> *mut i8;
// CHECK-NEXT:     fn strchr(__s: *const i8, __c: i32) -> *mut i8;
// CHECK-NEXT:     fn strstr(__haystack: *const i8, __needle: *const i8) -> *mut i8;
// CHECK-NEXT:     fn strftime(
// CHECK-NEXT:         __s: *mut i8,
// CHECK-NEXT:         __maxsize: u64,
// CHECK-NEXT:         __format: *const i8,
// CHECK-NEXT:         __tp: *const tm,
// CHECK-NEXT:     ) -> u64;
// CHECK-NEXT:     fn gmtime_r(__timer: *const i64, __tp: *mut tm) -> *mut tm;
// CHECK-NEXT:     fn localtime_r(__timer: *const i64, __tp: *mut tm) -> *mut tm;
// CHECK-NEXT:     fn timegm(__tp: *mut tm) -> i64;
// CHECK-NEXT:     fn timespec_getres(__ts: *mut timespec, __base: i32) -> i32;
// CHECK-NEXT:     fn mbrtoc8(
// CHECK-NEXT:         __pc8: *mut u8,
// CHECK-NEXT:         __s: *const i8,
// CHECK-NEXT:         __n: u64,
// CHECK-NEXT:         __p: *mut [[SLATERECORD_0:__SlateRecord[0-9]+]],
// CHECK-NEXT:     ) -> u64;
// CHECK-NEXT:     fn c8rtomb(__s: *mut i8, __c8: u8, __ps: *mut [[SLATERECORD_0]]) -> u64;
// CHECK-NEXT:     fn wcscmp(__s1: *const i32, __s2: *const i32) -> i32;
// CHECK-NEXT:     fn wcschr(__wcs: *const i32, __wc: i32) -> *mut i32;
// CHECK-NEXT:     fn wcsstr(__haystack: *const i32, __needle: *const i32) -> *mut i32;
// CHECK-NEXT:     fn wcsftime(
// CHECK-NEXT:         __s: *mut i32,
// CHECK-NEXT:         __maxsize: u64,
// CHECK-NEXT:         __format: *const i32,
// CHECK-NEXT:         __tp: *const tm,
// CHECK-NEXT:     ) -> u64;
// CHECK-NEXT:     safe fn __slate_f80_add(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_sub(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_mul(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_div(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_copysign(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_fmax(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_fmin(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_powi(__a: LongDouble, __n: i32) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_neg(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     fn __slate_f80_va_arg(__ap: *mut core::ffi::VaList<'static>) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_abs(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_ceil(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_floor(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_sqrt(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_cbrt(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_exp(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_exp2(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_expm1(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_log(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_log2(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_log10(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_log1p(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_sin(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_cos(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_tan(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_asin(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_acos(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_atan(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_sinh(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_cosh(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_tanh(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_asinh(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_acosh(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_atanh(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_nearbyint(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_fract(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_round(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_trunc(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_rint(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_fmod(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_remainder(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_pow(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_fdim(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_hypot(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_signbit(__a: LongDouble) -> bool;
// CHECK-NEXT:     safe fn __slate_f80_is_fp_class(__a: LongDouble, __flags: i32) -> bool;
// CHECK-NEXT:     safe fn __slate_f80_fma(
// CHECK-NEXT:         __a: LongDouble,
// CHECK-NEXT:         __b: LongDouble,
// CHECK-NEXT:         __c: LongDouble,
// CHECK-NEXT:     ) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_lt(__a: LongDouble, __b: LongDouble) -> bool;
// CHECK-NEXT:     safe fn __slate_f80_le(__a: LongDouble, __b: LongDouble) -> bool;
// CHECK-NEXT:     safe fn __slate_f80_gt(__a: LongDouble, __b: LongDouble) -> bool;
// CHECK-NEXT:     safe fn __slate_f80_ge(__a: LongDouble, __b: LongDouble) -> bool;
// CHECK-NEXT:     safe fn __slate_f80_eq(__a: LongDouble, __b: LongDouble) -> bool;
// CHECK-NEXT:     safe fn __slate_f80_ne(__a: LongDouble, __b: LongDouble) -> bool;
// CHECK-NEXT:     safe fn __slate_f80_from_i8(__a: i8) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_u8(__a: u8) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_i16(__a: i16) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_u16(__a: u16) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_i32(__a: i32) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_u32(__a: u32) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_i64(__a: i64) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_u64(__a: u64) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_i128(__a: i128) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_u128(__a: u128) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_f32(__a: f32) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_f64(__a: f64) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_bool(__a: bool) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_to_i8(__a: LongDouble) -> i8;
// CHECK-NEXT:     safe fn __slate_f80_to_u8(__a: LongDouble) -> u8;
// CHECK-NEXT:     safe fn __slate_f80_to_i16(__a: LongDouble) -> i16;
// CHECK-NEXT:     safe fn __slate_f80_to_u16(__a: LongDouble) -> u16;
// CHECK-NEXT:     safe fn __slate_f80_to_i32(__a: LongDouble) -> i32;
// CHECK-NEXT:     safe fn __slate_f80_to_u32(__a: LongDouble) -> u32;
// CHECK-NEXT:     safe fn __slate_f80_to_i64(__a: LongDouble) -> i64;
// CHECK-NEXT:     safe fn __slate_f80_to_u64(__a: LongDouble) -> u64;
// CHECK-NEXT:     safe fn __slate_f80_to_i128(__a: LongDouble) -> i128;
// CHECK-NEXT:     safe fn __slate_f80_to_u128(__a: LongDouble) -> u128;
// CHECK-NEXT:     safe fn __slate_f80_to_f32(__a: LongDouble) -> f32;
// CHECK-NEXT:     safe fn __slate_f80_to_f64(__a: LongDouble) -> f64;
// CHECK-NEXT:     safe fn __slate_f80_to_bool(__a: LongDouble) -> bool;
// CHECK-NEXT:     fn __slate_strfroml__ri32_pi8_u64_pi8_f80(
// CHECK-NEXT:         _0: *mut i8,
// CHECK-NEXT:         _1: u64,
// CHECK-NEXT:         _2: *const i8,
// CHECK-NEXT:         _3: LongDouble,
// CHECK-NEXT:     ) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct LongDouble([u8; 10]);
// CHECK-NEXT: impl core::ops::Add for LongDouble {
// CHECK-NEXT:     type Output = LongDouble;
// CHECK-NEXT:     fn add(self, __o: LongDouble) -> LongDouble {
// CHECK-NEXT:         __slate_f80_add(self, __o)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: impl core::ops::Sub for LongDouble {
// CHECK-NEXT:     type Output = LongDouble;
// CHECK-NEXT:     fn sub(self, __o: LongDouble) -> LongDouble {
// CHECK-NEXT:         __slate_f80_sub(self, __o)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: impl core::ops::Mul for LongDouble {
// CHECK-NEXT:     type Output = LongDouble;
// CHECK-NEXT:     fn mul(self, __o: LongDouble) -> LongDouble {
// CHECK-NEXT:         __slate_f80_mul(self, __o)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: impl core::ops::Div for LongDouble {
// CHECK-NEXT:     type Output = LongDouble;
// CHECK-NEXT:     fn div(self, __o: LongDouble) -> LongDouble {
// CHECK-NEXT:         __slate_f80_div(self, __o)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: impl core::ops::AddAssign for LongDouble {
// CHECK-NEXT:     fn add_assign(&mut self, __o: LongDouble) {
// CHECK-NEXT:         {
// CHECK-NEXT:             *self = __slate_f80_add(*self, __o);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: impl core::ops::SubAssign for LongDouble {
// CHECK-NEXT:     fn sub_assign(&mut self, __o: LongDouble) {
// CHECK-NEXT:         {
// CHECK-NEXT:             *self = __slate_f80_sub(*self, __o);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: impl core::ops::MulAssign for LongDouble {
// CHECK-NEXT:     fn mul_assign(&mut self, __o: LongDouble) {
// CHECK-NEXT:         {
// CHECK-NEXT:             *self = __slate_f80_mul(*self, __o);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: impl core::ops::DivAssign for LongDouble {
// CHECK-NEXT:     fn div_assign(&mut self, __o: LongDouble) {
// CHECK-NEXT:         {
// CHECK-NEXT:             *self = __slate_f80_div(*self, __o);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: impl core::ops::Neg for LongDouble {
// CHECK-NEXT:     type Output = LongDouble;
// CHECK-NEXT:     fn neg(self) -> LongDouble {
// CHECK-NEXT:         __slate_f80_neg(self)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: impl core::cmp::PartialEq for LongDouble {
// CHECK-NEXT:     fn eq(&self, __other: &LongDouble) -> bool {
// CHECK-NEXT:         __slate_f80_eq(*self, *__other)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: impl core::cmp::PartialOrd for LongDouble {
// CHECK-NEXT:     fn partial_cmp(&self, __other: &LongDouble) -> Option<std::cmp::Ordering> {
// CHECK-NEXT:         if __slate_f80_lt(*self, *__other) {
// CHECK-NEXT:             Some(std::cmp::Ordering::Less)
// CHECK-NEXT:         } else {
// CHECK-NEXT:             if __slate_f80_gt(*self, *__other) {
// CHECK-NEXT:                 Some(std::cmp::Ordering::Greater)
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 if __slate_f80_eq(*self, *__other) {
// CHECK-NEXT:                     Some(std::cmp::Ordering::Equal)
// CHECK-NEXT:                 } else {
// CHECK-NEXT:                     None
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_0]] {
// CHECK-NEXT:     __count: i32,
// CHECK-NEXT:     __value: [[SLATERECORD_1:__SlateRecord[0-9]+]],
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union [[SLATERECORD_1]] {
// CHECK-NEXT:     __wch: u32,
// CHECK-NEXT:     __wchb: [i8; 4],
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct tm {
// CHECK-NEXT:     tm_sec: i32,
// CHECK-NEXT:     tm_min: i32,
// CHECK-NEXT:     tm_hour: i32,
// CHECK-NEXT:     tm_mday: i32,
// CHECK-NEXT:     tm_mon: i32,
// CHECK-NEXT:     tm_year: i32,
// CHECK-NEXT:     tm_wday: i32,
// CHECK-NEXT:     tm_yday: i32,
// CHECK-NEXT:     tm_isdst: i32,
// CHECK-NEXT:     __tm_gmtoff: i64,
// CHECK-NEXT:     __tm_zone: *const i8,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct timespec {
// CHECK-NEXT:     tv_sec: i64,
// CHECK-NEXT:     tv_nsec: i64,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: fn c23_stdbit() -> i32 {
// CHECK-NEXT:     let value: u32 = 176;
// CHECK-NEXT:     {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]] = value;
// CHECK-NEXT:         [[T_0]].leading_zeros() as u32
// CHECK-NEXT:     }
// CHECK-NEXT:         .wrapping_add({
// CHECK-NEXT:             let [[T_1:__t[0-9]+]] = value;
// CHECK-NEXT:             (![[T_1]] as u32).leading_zeros() as u32
// CHECK-NEXT:         })
// CHECK-NEXT:         .wrapping_add({
// CHECK-NEXT:             let [[T_2:__t[0-9]+]] = value;
// CHECK-NEXT:             [[T_2]].trailing_zeros() as u32
// CHECK-NEXT:         })
// CHECK-NEXT:         .wrapping_add({
// CHECK-NEXT:             let [[T_3:__t[0-9]+]] = value;
// CHECK-NEXT:             (![[T_3]] as u32).trailing_zeros() as u32
// CHECK-NEXT:         })
// CHECK-NEXT:         .wrapping_add({
// CHECK-NEXT:             let [[T_4:__t[0-9]+]] = value;
// CHECK-NEXT:             ({
// CHECK-NEXT:                 let [[T_5:__t[0-9]+]] = (![[T_4]] as u32).leading_zeros();
// CHECK-NEXT:                 if [[T_5]] == 32 { 0u32 } else { [[T_5]] + 1 }
// CHECK-NEXT:             }) as u32
// CHECK-NEXT:         })
// CHECK-NEXT:         .wrapping_add({
// CHECK-NEXT:             let [[T_6:__t[0-9]+]] = value;
// CHECK-NEXT:             ({
// CHECK-NEXT:                 let [[T_7:__t[0-9]+]] = [[T_6]].leading_zeros();
// CHECK-NEXT:                 if [[T_7]] == 32 { 0u32 } else { [[T_7]] + 1 }
// CHECK-NEXT:             }) as u32
// CHECK-NEXT:         })
// CHECK-NEXT:         .wrapping_add({
// CHECK-NEXT:             let [[T_8:__t[0-9]+]] = value;
// CHECK-NEXT:             ({
// CHECK-NEXT:                 let [[T_9:__t[0-9]+]] = (![[T_8]] as u32).trailing_zeros();
// CHECK-NEXT:                 if [[T_9]] == 32 { 0u32 } else { [[T_9]] + 1 }
// CHECK-NEXT:             }) as u32
// CHECK-NEXT:         })
// CHECK-NEXT:         .wrapping_add({
// CHECK-NEXT:             let [[T_10:__t[0-9]+]] = value;
// CHECK-NEXT:             ({
// CHECK-NEXT:                 let [[T_11:__t[0-9]+]] = [[T_10]].trailing_zeros();
// CHECK-NEXT:                 if [[T_11]] == 32 { 0u32 } else { [[T_11]] + 1 }
// CHECK-NEXT:             }) as u32
// CHECK-NEXT:         })
// CHECK-NEXT:         .wrapping_add({
// CHECK-NEXT:             let [[T_12:__t[0-9]+]] = value;
// CHECK-NEXT:             (32 - [[T_12]].count_ones()) as u32
// CHECK-NEXT:         })
// CHECK-NEXT:         .wrapping_add({
// CHECK-NEXT:             let [[T_13:__t[0-9]+]] = value;
// CHECK-NEXT:             [[T_13]].count_ones() as u32
// CHECK-NEXT:         })
// CHECK-NEXT:         .wrapping_add(
// CHECK-NEXT:             (({
// CHECK-NEXT:                 let [[T_14:__t[0-9]+]] = 64u32;
// CHECK-NEXT:                 [[T_14]].count_ones() == 1
// CHECK-NEXT:             }) as i32) as u32,
// CHECK-NEXT:         )
// CHECK-NEXT:         .wrapping_add({
// CHECK-NEXT:             let [[T_15:__t[0-9]+]] = value;
// CHECK-NEXT:             (32 - [[T_15]].leading_zeros()) as u32
// CHECK-NEXT:         })
// CHECK-NEXT:         .wrapping_add({
// CHECK-NEXT:             let [[T_16:__t[0-9]+]] = value;
// CHECK-NEXT:             if [[T_16]].leading_zeros() == 32 {
// CHECK-NEXT:                 0u32
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 1u32.checked_shl(31 - [[T_16]].leading_zeros()).unwrap_or(0u32) as u32
// CHECK-NEXT:             }
// CHECK-NEXT:         })
// CHECK-NEXT:         .wrapping_add({
// CHECK-NEXT:             let [[T_17:__t[0-9]+]] = value;
// CHECK-NEXT:             if [[T_17]].count_ones() == 0 {
// CHECK-NEXT:                 1u32
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 if [[T_17]].count_ones() == 1 {
// CHECK-NEXT:                     [[T_17]]
// CHECK-NEXT:                 } else {
// CHECK-NEXT:                     1u32.checked_shl(32 - [[T_17]].leading_zeros()).unwrap_or(0u32) as u32
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn c23_checked_arithmetic() -> i32 {
// CHECK-NEXT:     let mut result: i32 = 0;
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: i32 = (!{
// CHECK-NEXT:         let [[T_0]]: i128 = 20 as i128;
// CHECK-NEXT:         let [[T_1]]: i128 = 22 as i128;
// CHECK-NEXT:         let [[T_2]]: i128 = [[T_0]].wrapping_add([[T_1]]);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *std::ptr::addr_of_mut!(result) = [[T_2]] as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         [[T_0]].checked_add([[T_1]]).is_none() || (([[T_2]] as i32) as i128) != [[T_2]]
// CHECK-NEXT:     } && result == 42) as i32;
// CHECK-NEXT:     total += [[V_0]];
// CHECK-NEXT:     let [[V_1:__v[0-9]+]]: i32 = (!{
// CHECK-NEXT:         let [[T_3]]: i128 = 50 as i128;
// CHECK-NEXT:         let [[T_4]]: i128 = 8 as i128;
// CHECK-NEXT:         let [[T_5]]: i128 = [[T_3]].wrapping_sub([[T_4]]);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *std::ptr::addr_of_mut!(result) = [[T_5]] as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         [[T_3]].checked_sub([[T_4]]).is_none() || (([[T_5]] as i32) as i128) != [[T_5]]
// CHECK-NEXT:     } && result == 42) as i32;
// CHECK-NEXT:     total += [[V_1]];
// CHECK-NEXT:     let [[V_2:__v[0-9]+]]: i32 = (!{
// CHECK-NEXT:         let [[T_6]]: i128 = 6 as i128;
// CHECK-NEXT:         let [[T_7]]: i128 = 7 as i128;
// CHECK-NEXT:         let [[T_8]]: i128 = [[T_6]].wrapping_mul([[T_7]]);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *std::ptr::addr_of_mut!(result) = [[T_8]] as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         [[T_6]].checked_mul([[T_7]]).is_none() || (([[T_8]] as i32) as i128) != [[T_8]]
// CHECK-NEXT:     } && result == 42) as i32;
// CHECK-NEXT:     total += [[V_2]];
// CHECK-NEXT:     let [[V_3:__v[0-9]+]]: bool = {
// CHECK-NEXT:         let [[T_9]]: i128 = 2147483647 as i128;
// CHECK-NEXT:         let [[T_10]]: i128 = 1 as i128;
// CHECK-NEXT:         let [[T_11]]: i128 = [[T_9]].wrapping_add([[T_10]]);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *std::ptr::addr_of_mut!(result) = [[T_11]] as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         [[T_9]].checked_add([[T_10]]).is_none() || (([[T_11]] as i32) as i128) != [[T_11]]
// CHECK-NEXT:     };
// CHECK-NEXT:     total += [[V_3]] as i32;
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-NEXT: fn c23_utf8() -> i32 {
// CHECK-NEXT:     let mut input_state: [[SLATERECORD_0]] = {
// CHECK-NEXT:         let mut [[T_0]]: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut output_state: [[SLATERECORD_0]] = {
// CHECK-NEXT:         let mut [[T_1]]: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_1]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut character: u8 = 0;
// CHECK-NEXT:     let mut output: __SlateAlign16<[i8; 16]> = __SlateAlign16({
// CHECK-NEXT:         let mut [[T_2]]: [i8; 16] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_2]]
// CHECK-NEXT:     });
// CHECK-NEXT:     let input_size: u64 = unsafe {
// CHECK-NEXT:         mbrtoc8(
// CHECK-NEXT:             std::ptr::addr_of_mut!(character),
// CHECK-NEXT:             c"A".as_ptr(),
// CHECK-NEXT:             1u64,
// CHECK-NEXT:             std::ptr::addr_of_mut!(input_state),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let output_size: u64 = unsafe {
// CHECK-NEXT:         c8rtomb(
// CHECK-NEXT:             output.0.as_mut_ptr() as *mut i8,
// CHECK-NEXT:             character,
// CHECK-NEXT:             std::ptr::addr_of_mut!(output_state),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut atomic_character: u8 = character;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::sync::atomic::AtomicU8::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(atomic_character) as *mut u8,
// CHECK-NEXT:             )
// CHECK-NEXT:             .store(66u8, std::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:     };
// CHECK-NEXT:     ((input_size == 1) as i32) + ((output_size == 1) as i32)
// CHECK-NEXT:         + (((output.0[0] as i32) == 65) as i32)
// CHECK-NEXT:         + (((((unsafe {
// CHECK-NEXT:             std::sync::atomic::AtomicU8::from_ptr(
// CHECK-NEXT:                     std::ptr::addr_of_mut!(atomic_character) as *mut u8,
// CHECK-NEXT:                 )
// CHECK-NEXT:                 .load(std::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:         }) as u32) as i32) == 66) as i32) + ((2i32 > 0i32) as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn c23_memory() -> i32 {
// CHECK-NEXT:     let mut source: [i8; 7] = [97, 98, 99, 100, 101, 102, 0];
// CHECK-NEXT:     let mut destination: [i8; 8] = {
// CHECK-NEXT:         let mut [[T_0]]: [i8; 8] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut secret: [i8; 7] = [115, 101, 99, 114, 101, 116, 0];
// CHECK-NEXT:     let phrase: *const i8 = c"hello world".as_ptr();
// CHECK-NEXT:     let mut mutable_phrase: [i8; 12] = [
// CHECK-NEXT:         104, 101, 108, 108, 111, 32, 119, 111, 114, 108, 100, 0,
// CHECK-NEXT:     ];
// CHECK-NEXT:     let stop: *mut () = unsafe {
// CHECK-NEXT:         memccpy(
// CHECK-NEXT:             destination.as_mut_ptr() as *mut (),
// CHECK-NEXT:             source.as_mut_ptr() as *const (),
// CHECK-NEXT:             99i32,
// CHECK-NEXT:             6u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut total: i32 = (stop
// CHECK-NEXT:         == ((unsafe { (destination.as_mut_ptr() as *mut i8).offset(3 as isize) })
// CHECK-NEXT:             as *mut ()) && (destination[2] as i32) == 99) as i32;
// CHECK-NEXT:     unsafe { memset_explicit(secret.as_mut_ptr() as *mut (), 0i32, 7u64) };
// CHECK-NEXT:     total += ((secret[0] as i32) == 0 && (secret[5] as i32) == 0) as i32;
// CHECK-NEXT:     let first_copy: *mut i8 = unsafe { strdup(c"c23".as_ptr()) };
// CHECK-NEXT:     let second_copy: *mut i8 = unsafe { strndup(c"library".as_ptr(), 3u64) };
// CHECK-NEXT:     let [[V_4:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     total = [[V_4]]
// CHECK-NEXT:         + ((!first_copy.is_null()
// CHECK-NEXT:             && (unsafe { strcmp(first_copy as *const i8, c"c23".as_ptr()) }) == 0)
// CHECK-NEXT:             as i32);
// CHECK-NEXT:     let [[V_5:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     total = [[V_5]]
// CHECK-NEXT:         + ((!second_copy.is_null()
// CHECK-NEXT:             && (unsafe { strcmp(second_copy as *const i8, c"lib".as_ptr()) }) == 0)
// CHECK-NEXT:             as i32);
// CHECK-NEXT:     unsafe { free(first_copy as *mut ()) };
// CHECK-NEXT:     unsafe { free(second_copy as *mut ()) };
// CHECK-NEXT:     let const_hit: *const i8 = (unsafe { strchr(phrase, 119i32) }) as *const i8;
// CHECK-NEXT:     let mut_hit: *mut i8 = unsafe {
// CHECK-NEXT:         strchr(mutable_phrase.as_mut_ptr() as *const i8, 119i32)
// CHECK-NEXT:     };
// CHECK-NEXT:     let [[V_6:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     total = [[V_6]] + ((!const_hit.is_null() && !mut_hit.is_null()) as i32);
// CHECK-NEXT:     let [[V_7:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     total = [[V_7]]
// CHECK-NEXT:         + (!((unsafe { memchr(phrase as *const (), 111i32, 11u64) }) as *const ())
// CHECK-NEXT:             .is_null() as i32);
// CHECK-NEXT:     let [[V_8:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     total = [[V_8]]
// CHECK-NEXT:         + (!unsafe { memchr(mutable_phrase.as_mut_ptr() as *const (), 111i32, 11u64) }
// CHECK-NEXT:             .is_null() as i32);
// CHECK-NEXT:     let [[V_9:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     total = [[V_9]]
// CHECK-NEXT:         + (!((unsafe { strstr(phrase, c"world".as_ptr()) }) as *const i8).is_null()
// CHECK-NEXT:             as i32);
// CHECK-NEXT:     let [[V_10:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     total = [[V_10]]
// CHECK-NEXT:         + (!unsafe {
// CHECK-NEXT:             strstr(mutable_phrase.as_mut_ptr() as *const i8, c"world".as_ptr())
// CHECK-NEXT:         }
// CHECK-NEXT:             .is_null() as i32);
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-NEXT: fn c23_time() -> i32 {
// CHECK-NEXT:     let mut timestamp: i64 = 0;
// CHECK-NEXT:     let mut utc: tm = {
// CHECK-NEXT:         let mut [[T_0]]: tm = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut local: tm = {
// CHECK-NEXT:         let mut [[T_1]]: tm = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_1]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut resolution: timespec = {
// CHECK-NEXT:         let mut [[T_2]]: timespec = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_2]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut month: __SlateAlign16<[i8; 32]> = __SlateAlign16({
// CHECK-NEXT:         let mut [[T_3]]: [i8; 32] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_3]]
// CHECK-NEXT:     });
// CHECK-NEXT:     let mut wide_month: __SlateAlign16<[i32; 32]> = __SlateAlign16({
// CHECK-NEXT:         let mut [[T_4]]: [i32; 32] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_4]]
// CHECK-NEXT:     });
// CHECK-NEXT:     let mut total: i32 = ((unsafe {
// CHECK-NEXT:         gmtime_r(
// CHECK-NEXT:             std::ptr::addr_of_mut!(timestamp) as *const i64,
// CHECK-NEXT:             std::ptr::addr_of_mut!(utc),
// CHECK-NEXT:         )
// CHECK-NEXT:     }) == std::ptr::addr_of_mut!(utc)) as i32;
// CHECK-NEXT:     let [[V_11:__v[0-9]+]]: i32 = ((unsafe {
// CHECK-NEXT:         localtime_r(
// CHECK-NEXT:             std::ptr::addr_of_mut!(timestamp) as *const i64,
// CHECK-NEXT:             std::ptr::addr_of_mut!(local),
// CHECK-NEXT:         )
// CHECK-NEXT:     }) == std::ptr::addr_of_mut!(local)) as i32;
// CHECK-NEXT:     total += [[V_11]];
// CHECK-NEXT:     let [[V_12:__v[0-9]+]]: i32 = ((unsafe {
// CHECK-NEXT:         timespec_getres(std::ptr::addr_of_mut!(resolution), 1i32)
// CHECK-NEXT:     }) == 1) as i32;
// CHECK-NEXT:     total += [[V_12]];
// CHECK-NEXT:     total += (resolution.tv_sec > 0 || resolution.tv_nsec > 0) as i32;
// CHECK-NEXT:     let [[V_13:__v[0-9]+]]: i32 = ((unsafe { timegm(std::ptr::addr_of_mut!(utc)) }) == 0) as i32;
// CHECK-NEXT:     total += [[V_13]];
// CHECK-NEXT:     let [[V_14:__v[0-9]+]]: i32 = ((unsafe {
// CHECK-NEXT:         strftime(
// CHECK-NEXT:             month.0.as_mut_ptr() as *mut i8,
// CHECK-NEXT:             32u64,
// CHECK-NEXT:             c"%OB".as_ptr(),
// CHECK-NEXT:             std::ptr::addr_of_mut!(utc) as *const tm,
// CHECK-NEXT:         )
// CHECK-NEXT:     }) == 7) as i32;
// CHECK-NEXT:     total += [[V_14]];
// CHECK-NEXT:     let [[V_15:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     total = [[V_15]]
// CHECK-NEXT:         + (((unsafe { strcmp(month.0.as_mut_ptr() as *const i8, c"January".as_ptr()) })
// CHECK-NEXT:             == 0) as i32);
// CHECK-NEXT:     let [[V_16:__v[0-9]+]]: i32 = ((unsafe {
// CHECK-NEXT:         wcsftime(
// CHECK-NEXT:             wide_month.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:             32u64,
// CHECK-NEXT:             (std::ptr::addr_of_mut!(__slate_str281) as *mut i32) as *const i32,
// CHECK-NEXT:             std::ptr::addr_of_mut!(utc) as *const tm,
// CHECK-NEXT:         )
// CHECK-NEXT:     }) == 7) as i32;
// CHECK-NEXT:     total += [[V_16]];
// CHECK-NEXT:     let [[V_17:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     total = [[V_17]]
// CHECK-NEXT:         + (((unsafe {
// CHECK-NEXT:             wcscmp(
// CHECK-NEXT:                 wide_month.0.as_mut_ptr() as *const i32,
// CHECK-NEXT:                 (std::ptr::addr_of_mut!(__slate_str283) as *mut i32) as *const i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) == 0) as i32);
// CHECK-NEXT:     let const_month: *const i32 = wide_month.0.as_mut_ptr() as *const i32;
// CHECK-NEXT:     let [[V_18:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     total = [[V_18]]
// CHECK-NEXT:         + (!((unsafe { wcschr(const_month, 110i32) }) as *const i32).is_null() as i32);
// CHECK-NEXT:     let [[V_19:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     total = [[V_19]]
// CHECK-NEXT:         + (!unsafe { wcschr(wide_month.0.as_mut_ptr() as *const i32, 110i32) }.is_null()
// CHECK-NEXT:             as i32);
// CHECK-NEXT:     let [[V_20:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     total = [[V_20]]
// CHECK-NEXT:         + (!((unsafe {
// CHECK-NEXT:             wcsstr(
// CHECK-NEXT:                 const_month,
// CHECK-NEXT:                 (std::ptr::addr_of_mut!(__slate_str287) as *mut i32) as *const i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) as *const i32)
// CHECK-NEXT:             .is_null() as i32);
// CHECK-NEXT:     let [[V_21:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     total = [[V_21]]
// CHECK-NEXT:         + (!unsafe {
// CHECK-NEXT:             wcsstr(
// CHECK-NEXT:                 wide_month.0.as_mut_ptr() as *const i32,
// CHECK-NEXT:                 (std::ptr::addr_of_mut!(__slate_str289) as *mut i32) as *const i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         }
// CHECK-NEXT:             .is_null() as i32);
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-NEXT: static mut __slate_str281: [i32; 4] = [37, 79, 66, 0];
// CHECK-NEXT: static mut __slate_str283: [i32; 8] = [74, 97, 110, 117, 97, 114, 121, 0];
// CHECK-NEXT: static mut __slate_str287: [i32; 4] = [74, 97, 110, 0];
// CHECK-NEXT: static mut __slate_str289: [i32; 4] = [74, 97, 110, 0];
// CHECK-NEXT: fn c23_io() -> i32 {
// CHECK-NEXT:     let mut output: __SlateAlign16<[i8; 64]> = __SlateAlign16({
// CHECK-NEXT:         let mut [[T_0]]: [i8; 64] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     });
// CHECK-NEXT:     let mut float_output: __SlateAlign16<[i8; 16]> = __SlateAlign16({
// CHECK-NEXT:         let mut [[T_1]]: [i8; 16] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_1]]
// CHECK-NEXT:     });
// CHECK-NEXT:     let mut double_output: __SlateAlign16<[i8; 16]> = __SlateAlign16({
// CHECK-NEXT:         let mut [[T_2]]: [i8; 16] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_2]]
// CHECK-NEXT:     });
// CHECK-NEXT:     let mut long_double_output: __SlateAlign16<[i8; 16]> = __SlateAlign16({
// CHECK-NEXT:         let mut [[T_3]]: [i8; 16] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_3]]
// CHECK-NEXT:     });
// CHECK-NEXT:     let mut binary_value: u32 = 0;
// CHECK-NEXT:     let mut exact_value: u16 = 0;
// CHECK-NEXT:     let mut fast_value: u64 = 0;
// CHECK-NEXT:     let written: i32 = unsafe {
// CHECK-NEXT:         snprintf(
// CHECK-NEXT:             output.0.as_mut_ptr() as *mut i8,
// CHECK-NEXT:             64u64,
// CHECK-NEXT:             c"%b %w16u %wf16u".as_ptr(),
// CHECK-NEXT:             13u32,
// CHECK-NEXT:             21i32,
// CHECK-NEXT:             34u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let scanned: i32 = unsafe {
// CHECK-NEXT:         sscanf(
// CHECK-NEXT:             c"1011 55 89".as_ptr(),
// CHECK-NEXT:             c"%b %w16u %wf16u".as_ptr(),
// CHECK-NEXT:             std::ptr::addr_of_mut!(binary_value),
// CHECK-NEXT:             std::ptr::addr_of_mut!(exact_value),
// CHECK-NEXT:             std::ptr::addr_of_mut!(fast_value),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let floating_written: i32 = (unsafe {
// CHECK-NEXT:         strfromf(float_output.0.as_mut_ptr() as *mut i8, 16u64, c"%.1f".as_ptr(), 1.5f32)
// CHECK-NEXT:     })
// CHECK-NEXT:         + unsafe {
// CHECK-NEXT:             strfromd(
// CHECK-NEXT:                 double_output.0.as_mut_ptr() as *mut i8,
// CHECK-NEXT:                 16u64,
// CHECK-NEXT:                 c"%.1f".as_ptr(),
// CHECK-NEXT:                 2.5f64,
// CHECK-NEXT:             )
// CHECK-NEXT:         }
// CHECK-NEXT:         + unsafe {
// CHECK-NEXT:             __slate_strfroml__ri32_pi8_u64_pi8_f80(
// CHECK-NEXT:                 long_double_output.0.as_mut_ptr() as *mut i8,
// CHECK-NEXT:                 16u64,
// CHECK-NEXT:                 c"%.1f".as_ptr(),
// CHECK-NEXT:                 LongDouble([0, 0, 0, 0, 0, 0, 0, 224, 0, 64]),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:     written
// CHECK-NEXT:         + (((unsafe {
// CHECK-NEXT:             strcmp(output.0.as_mut_ptr() as *const i8, c"1101 21 34".as_ptr())
// CHECK-NEXT:         }) == 0) as i32) + scanned + ((binary_value == 11) as i32)
// CHECK-NEXT:         + ((((exact_value as u32) as i32) == 55) as i32) + ((fast_value == 89) as i32)
// CHECK-NEXT:         + floating_written
// CHECK-NEXT:         + (((unsafe {
// CHECK-NEXT:             strcmp(float_output.0.as_mut_ptr() as *const i8, c"1.5".as_ptr())
// CHECK-NEXT:         }) == 0) as i32)
// CHECK-NEXT:         + (((unsafe {
// CHECK-NEXT:             strcmp(double_output.0.as_mut_ptr() as *const i8, c"2.5".as_ptr())
// CHECK-NEXT:         }) == 0) as i32)
// CHECK-NEXT:         + (((unsafe {
// CHECK-NEXT:             strcmp(long_double_output.0.as_mut_ptr() as *const i8, c"3.5".as_ptr())
// CHECK-NEXT:         }) == 0) as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn c23_limits() -> i32 {
// CHECK-NEXT:     let integer_widths: i32 = 1 + 8 + 8 + 8 + 16 + 16 + 32 + 32 + 64 + 64 + 64 + 64 + 64
// CHECK-NEXT:         + 64;
// CHECK-NEXT:     let floating_limits: i32 = ((1e-45f32 > 0.0f32) as i32)
// CHECK-NEXT:         + ((5e-324f64 > 0.0f64) as i32)
// CHECK-NEXT:         + ((LongDouble([1, 0, 0, 0, 0, 0, 0, 0, 0, 0])
// CHECK-NEXT:             > LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0])) as i32)
// CHECK-NEXT:         + ((3.4028235e38f32 <= 3.4028235e38f32) as i32)
// CHECK-NEXT:         + ((1.7976931348623157e308f64 <= 1.7976931348623157e308f64) as i32)
// CHECK-NEXT:         + ((LongDouble([255, 255, 255, 255, 255, 255, 255, 255, 254, 127])
// CHECK-NEXT:             <= LongDouble([255, 255, 255, 255, 255, 255, 255, 255, 254, 127])) as i32)
// CHECK-NEXT:         + ((1i32 >= -1i32) as i32) + ((1i32 >= -1i32) as i32) + ((1i32 >= -1i32) as i32)
// CHECK-NEXT:         + ((4u64 == 4u64) as i32) + ((8u64 == 8u64) as i32) + ((16u64 == 16u64) as i32);
// CHECK-NEXT:     let header_versions: i32 = ((202311i64 == 202311i64) as i32)
// CHECK-NEXT:         + ((202311i64 == 202311i64) as i32) + ((202311i64 == 202311i64) as i32)
// CHECK-NEXT:         + ((202311i64 == 202311i64) as i32) + ((202311i64 == 202311i64) as i32)
// CHECK-NEXT:         + ((202311i64 == 202311i64) as i32) + ((202311i64 == 202311i64) as i32);
// CHECK-NEXT:     integer_widths + floating_limits + header_versions
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {} {} {} {}", c23_stdbit(), c23_checked_arithmetic(), c23_utf8(),
// CHECK-NEXT:         c23_memory(), c23_time(), c23_io() + c23_limits()
// CHECK-NEXT:     );
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
