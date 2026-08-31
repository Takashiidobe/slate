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
// LOWERING-NEXT:     __value: {{anon_[0-9]+}},
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: union {{anon_[0-9]+}} {
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
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = 176;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = unsafe { stdc_leading_zeros_ui({{_v[0-9]+}} as u32) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = unsafe { stdc_leading_ones_ui({{_v[0-9]+}} as u32) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = unsafe { stdc_trailing_zeros_ui({{_v[0-9]+}} as u32) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = unsafe { stdc_trailing_ones_ui({{_v[0-9]+}} as u32) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = unsafe { stdc_first_leading_zero_ui({{_v[0-9]+}} as u32) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = unsafe { stdc_first_leading_one_ui({{_v[0-9]+}} as u32) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = unsafe { stdc_first_trailing_zero_ui({{_v[0-9]+}} as u32) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = unsafe { stdc_first_trailing_one_ui({{_v[0-9]+}} as u32) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = unsafe { stdc_count_zeros_ui({{_v[0-9]+}} as u32) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = unsafe { stdc_count_ones_ui({{_v[0-9]+}} as u32) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = true;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = {{_v[0-9]+}} as u32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = unsafe { stdc_bit_width_ui({{_v[0-9]+}} as u32) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = unsafe { stdc_bit_floor_ui({{_v[0-9]+}} as u32) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = unsafe { stdc_bit_ceil_ui({{_v[0-9]+}} as u32) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn c23_checked_arithmetic() -> i32 {
// LOWERING-NEXT:     let mut result: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 20;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 22;
// LOWERING-NEXT:     let {{_v[0-9]+}} = {{_v[0-9]+}}.overflowing_add({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}}.0 as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}}.1 || ({{_v[0-9]+}}.0 < -2147483648 || {{_v[0-9]+}}.0 > 2147483647);
// LOWERING-NEXT:     result = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = result;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 42;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 50;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 8;
// LOWERING-NEXT:     let {{_v[0-9]+}} = {{_v[0-9]+}}.overflowing_sub({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}}.0 as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}}.1 || ({{_v[0-9]+}}.0 < -2147483648 || {{_v[0-9]+}}.0 > 2147483647);
// LOWERING-NEXT:     result = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = result;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 42;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 6;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 7;
// LOWERING-NEXT:     let {{_v[0-9]+}} = {{_v[0-9]+}}.overflowing_mul({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}}.0 as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}}.1 || ({{_v[0-9]+}}.0 < -2147483648 || {{_v[0-9]+}}.0 > 2147483647);
// LOWERING-NEXT:     result = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = result;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 42;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 2147483647;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}} = {{_v[0-9]+}}.overflowing_add({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}}.0 as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}}.1 || ({{_v[0-9]+}}.0 < -2147483648 || {{_v[0-9]+}}.0 > 2147483647);
// LOWERING-NEXT:     result = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn c23_utf8() -> i32 {
// LOWERING-NEXT:     let mut input_state: __mbstate_t = __mbstate_t { __count: 0, __value: unsafe { std::mem::zeroed::<{{anon_[0-9]+}}>() } };
// LOWERING-NEXT:     let mut output_state: __mbstate_t = __mbstate_t { __count: 0, __value: unsafe { std::mem::zeroed::<{{anon_[0-9]+}}>() } };
// LOWERING-NEXT:     let mut character: u8 = 0;
// LOWERING-NEXT:     let mut output: [i8; 4] = [0; 4];
// LOWERING-NEXT:     let mut atomic_character: u8 = 0;
// LOWERING-NEXT:     input_state = __mbstate_t { __count: 0, __value: unsafe { std::mem::zeroed::<{{anon_[0-9]+}}>() } };
// LOWERING-NEXT:     output_state = __mbstate_t { __count: 0, __value: unsafe { std::mem::zeroed::<{{anon_[0-9]+}}>() } };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u8 = 0;
// LOWERING-NEXT:     character = {{_v[0-9]+}};
// LOWERING-NEXT:     output = [0, 0, 0, 0];
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"A\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = (unsafe { mbrtoc8(std::ptr::addr_of_mut!(character) as *mut u8, {{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as usize, std::ptr::addr_of_mut!(input_state) as *mut __mbstate_t) }) as u64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = output.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u8 = character;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = (unsafe { c8rtomb({{_v[0-9]+}} as *mut i8, {{_v[0-9]+}} as u8, std::ptr::addr_of_mut!(output_state) as *mut __mbstate_t) }) as u64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u8 = character;
// LOWERING-NEXT:     atomic_character = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u8 = 66;
// LOWERING-NEXT:     unsafe { std::sync::atomic::AtomicU8::from_ptr(std::ptr::addr_of_mut!(atomic_character)).store({{_v[0-9]+}}, std::sync::atomic::Ordering::SeqCst) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i8 = output[({{_v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 65;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u8 = unsafe { std::sync::atomic::AtomicU8::from_ptr(std::ptr::addr_of_mut!(atomic_character)).load(std::sync::atomic::Ordering::SeqCst) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 66;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} > {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn c23_memory() -> i32 {
// LOWERING-NEXT:     let mut source: [i8; 7] = [0; 7];
// LOWERING-NEXT:     let mut destination: [i8; 8] = [0; 8];
// LOWERING-NEXT:     let mut secret: [i8; 7] = [0; 7];
// LOWERING-NEXT:     let mut first_copy: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut second_copy: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut mutable_phrase: [i8; 12] = [0; 12];
// LOWERING-NEXT:     let mut mut_hit: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     source = [97, 98, 99, 100, 101, 102, 0];
// LOWERING-NEXT:     destination = [0, 0, 0, 0, 0, 0, 0, 0];
// LOWERING-NEXT:     secret = [115, 101, 99, 114, 101, 116, 0];
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"hello world\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     mutable_phrase = [104, 101, 108, 108, 111, 32, 119, 111, 114, 108, 100, 0];
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = destination.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = source.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 99;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 6;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe { memccpy({{_v[0-9]+}} as *mut core::ffi::c_void, {{_v[0-9]+}} as *const core::ffi::c_void, {{_v[0-9]+}} as i32, {{_v[0-9]+}} as usize) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = destination.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = unsafe { {{_v[0-9]+}}.add(3) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i8 = destination[({{_v[0-9]+}} as usize)];
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 99;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = secret.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 7;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe { memset_explicit({{_v[0-9]+}} as *mut core::ffi::c_void, {{_v[0-9]+}} as i32, {{_v[0-9]+}} as usize) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i8 = secret[({{_v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: i64 = 5;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i8 = secret[({{_v[0-9]+}} as usize)];
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"c23\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = unsafe { strdup({{_v[0-9]+}} as *const i8) };
// LOWERING-NEXT:     first_copy = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"library\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = unsafe { strndup({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as usize) };
// LOWERING-NEXT:     second_copy = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = first_copy;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut i8 = first_copy;
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut i8 = b"c23\0".as_ptr() as *mut i8;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = unsafe { strcmp({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as *const i8) };
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = second_copy;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut i8 = second_copy;
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut i8 = b"lib\0".as_ptr() as *mut i8;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = unsafe { strcmp({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as *const i8) };
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = first_copy;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     unsafe { free({{_v[0-9]+}} as *mut core::ffi::c_void) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = second_copy;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     unsafe { free({{_v[0-9]+}} as *mut core::ffi::c_void) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 119;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = unsafe { strchr({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as i32) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = mutable_phrase.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 119;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = unsafe { strchr({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as i32) };
// LOWERING-NEXT:     mut_hit = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut i8 = mut_hit;
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 111;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 11;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe { memchr({{_v[0-9]+}} as *const core::ffi::c_void, {{_v[0-9]+}} as i32, {{_v[0-9]+}} as usize) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = mutable_phrase.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 111;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 11;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe { memchr({{_v[0-9]+}} as *const core::ffi::c_void, {{_v[0-9]+}} as i32, {{_v[0-9]+}} as usize) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"world\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = unsafe { strstr({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as *const i8) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = mutable_phrase.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"world\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = unsafe { strstr({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as *const i8) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn c23_time() -> i32 {
// LOWERING-NEXT:     let mut timestamp: i64 = 0;
// LOWERING-NEXT:     let mut utc: tm = tm { tm_sec: 0, tm_min: 0, tm_hour: 0, tm_mday: 0, tm_mon: 0, tm_year: 0, tm_wday: 0, tm_yday: 0, tm_isdst: 0, __tm_gmtoff: 0, __tm_zone: std::ptr::null_mut() };
// LOWERING-NEXT:     let mut local: tm = tm { tm_sec: 0, tm_min: 0, tm_hour: 0, tm_mday: 0, tm_mon: 0, tm_year: 0, tm_wday: 0, tm_yday: 0, tm_isdst: 0, __tm_gmtoff: 0, __tm_zone: std::ptr::null_mut() };
// LOWERING-NEXT:     let mut resolution: libc::timespec = libc::timespec { tv_sec: 0, tv_nsec: 0 };
// LOWERING-NEXT:     let mut month: aligned::Aligned<aligned::A16, [i8; 32]> = aligned::Aligned([0; 32]);
// LOWERING-NEXT:     let mut wide_month: aligned::Aligned<aligned::A16, [i32; 32]> = aligned::Aligned([0; 32]);
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     timestamp = {{_v[0-9]+}};
// LOWERING-NEXT:     utc = tm { tm_sec: 0, tm_min: 0, tm_hour: 0, tm_mday: 0, tm_mon: 0, tm_year: 0, tm_wday: 0, tm_yday: 0, tm_isdst: 0, __tm_gmtoff: 0, __tm_zone: std::ptr::null_mut() };
// LOWERING-NEXT:     local = tm { tm_sec: 0, tm_min: 0, tm_hour: 0, tm_mday: 0, tm_mon: 0, tm_year: 0, tm_wday: 0, tm_yday: 0, tm_isdst: 0, __tm_gmtoff: 0, __tm_zone: std::ptr::null_mut() };
// LOWERING-NEXT:     resolution = libc::timespec { tv_sec: 0, tv_nsec: 0 };
// LOWERING-NEXT:     *month = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
// LOWERING-NEXT:     *wide_month = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut tm = unsafe { gmtime_r(std::ptr::addr_of_mut!(timestamp) as *const i64, std::ptr::addr_of_mut!(utc) as *mut tm) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == std::ptr::addr_of_mut!(utc);
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut tm = unsafe { localtime_r(std::ptr::addr_of_mut!(timestamp) as *const i64, std::ptr::addr_of_mut!(local) as *mut tm) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == std::ptr::addr_of_mut!(local);
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { timespec_getres(std::ptr::addr_of_mut!(resolution) as *mut libc::timespec, {{_v[0-9]+}} as i32) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = resolution.tv_sec;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} > {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = true;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: i64 = resolution.tv_nsec;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} > {{_v[0-9]+}};
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { timegm(std::ptr::addr_of_mut!(utc) as *mut tm) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = month.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%OB\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = (unsafe { strftime({{_v[0-9]+}} as *mut i8, {{_v[0-9]+}} as usize, {{_v[0-9]+}} as *const i8, std::ptr::addr_of_mut!(utc) as *const tm) }) as u64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 7;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = month.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"January\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { strcmp({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as *const i8) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = wide_month.as_mut_ptr() as *mut i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = std::ptr::addr_of_mut!(_str_9).cast::<i32>();
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = (unsafe { wcsftime({{_v[0-9]+}} as *mut i32, {{_v[0-9]+}} as usize, {{_v[0-9]+}} as *const i32, std::ptr::addr_of_mut!(utc) as *const tm) }) as u64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 7;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = wide_month.as_mut_ptr() as *mut i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = std::ptr::addr_of_mut!(_str_10).cast::<i32>();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { wcscmp({{_v[0-9]+}} as *const i32, {{_v[0-9]+}} as *const i32) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = wide_month.as_mut_ptr() as *mut i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 110;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = unsafe { wcschr({{_v[0-9]+}} as *const i32, {{_v[0-9]+}} as i32) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = wide_month.as_mut_ptr() as *mut i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 110;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = unsafe { wcschr({{_v[0-9]+}} as *const i32, {{_v[0-9]+}} as i32) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = std::ptr::addr_of_mut!(_str_11).cast::<i32>();
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = unsafe { wcsstr({{_v[0-9]+}} as *const i32, {{_v[0-9]+}} as *const i32) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = wide_month.as_mut_ptr() as *mut i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = std::ptr::addr_of_mut!(_str_11).cast::<i32>();
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = unsafe { wcsstr({{_v[0-9]+}} as *const i32, {{_v[0-9]+}} as *const i32) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn c23_io() -> i32 {
// LOWERING-NEXT:     let mut output: aligned::Aligned<aligned::A16, [i8; 64]> = aligned::Aligned([0; 64]);
// LOWERING-NEXT:     let mut float_output: aligned::Aligned<aligned::A16, [i8; 16]> = aligned::Aligned([0; 16]);
// LOWERING-NEXT:     let mut double_output: aligned::Aligned<aligned::A16, [i8; 16]> = aligned::Aligned([0; 16]);
// LOWERING-NEXT:     let mut long_double_output: aligned::Aligned<aligned::A16, [i8; 16]> = aligned::Aligned([0; 16]);
// LOWERING-NEXT:     let mut binary_value: u32 = 0;
// LOWERING-NEXT:     let mut exact_value: u16 = 0;
// LOWERING-NEXT:     let mut fast_value: u64 = 0;
// LOWERING-NEXT:     *output = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
// LOWERING-NEXT:     *float_output = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
// LOWERING-NEXT:     *double_output = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
// LOWERING-NEXT:     *long_double_output = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = 0;
// LOWERING-NEXT:     binary_value = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u16 = 0;
// LOWERING-NEXT:     exact_value = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 0;
// LOWERING-NEXT:     fast_value = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = output.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%b %w16u %wf16u\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = 13;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 21;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 34;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { snprintf({{_v[0-9]+}} as *mut i8, {{_v[0-9]+}} as usize, {{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"1011 55 89\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%b %w16u %wf16u\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { sscanf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as *const i8, std::ptr::addr_of_mut!(binary_value), std::ptr::addr_of_mut!(exact_value), std::ptr::addr_of_mut!(fast_value)) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = float_output.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 16;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%.1f\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = 1.5;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { strfromf({{_v[0-9]+}} as *mut i8, {{_v[0-9]+}} as usize, {{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as f32) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = double_output.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 16;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%.1f\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 2.5;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { strfromd({{_v[0-9]+}} as *mut i8, {{_v[0-9]+}} as usize, {{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as f64) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = long_double_output.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 16;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%.1f\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 224, 0, 64]);
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { __slate_strfroml__ri32_pi8_usize_pi8_f80({{_v[0-9]+}} as *mut i8, {{_v[0-9]+}} as usize, {{_v[0-9]+}} as *const i8, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = output.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"1101 21 34\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { strcmp({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as *const i8) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = binary_value;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = 11;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u16 = exact_value;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 55;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = fast_value;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 89;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = float_output.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"1.5\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { strcmp({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as *const i8) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = double_output.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"2.5\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { strcmp({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as *const i8) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = long_double_output.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"3.5\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { strcmp({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as *const i8) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn c23_limits() -> i32 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 16;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 16;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = 0.0000000000000000000000000000000000000000000014013;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = 0.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} > {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 0.000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000005;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 0.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} > {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([1, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} > {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = 340282346999999984391321947108527833088.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = 340282346999999984391321947108527833088.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} <= {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} <= {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([255, 255, 255, 255, 255, 255, 255, 255, 254, 127]);
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([255, 255, 255, 255, 255, 255, 255, 255, 254, 127]);
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} <= {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = -1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} >= {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = -1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} >= {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = -1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} >= {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 4;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 4;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 16;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 16;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 202311;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 202311;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 202311;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 202311;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 202311;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 202311;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 202311;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 202311;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 202311;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 202311;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 202311;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 202311;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 202311;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 202311;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %d %d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = c23_stdbit();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = c23_checked_arithmetic();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = c23_utf8();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = c23_memory();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = c23_time();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = c23_io();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = c23_limits();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
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
// REWRITES-NEXT:     __value: {{anon_[0-9]+}},
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: union {{anon_[0-9]+}} {
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
// REWRITES-NEXT: let {{_v[0-9]+}}: u32 = 176;
// REWRITES-NEXT: let {{_v[0-9]+}}: u32 = unsafe { stdc_leading_zeros_ui({{_v[0-9]+}} as u32) };
// REWRITES-NEXT: let {{_v[0-9]+}}: u32 = unsafe { stdc_leading_ones_ui({{_v[0-9]+}} as u32) };
// REWRITES-NEXT: let {{_v[0-9]+}}: u32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: u32 = unsafe { stdc_trailing_zeros_ui({{_v[0-9]+}} as u32) };
// REWRITES-NEXT: let {{_v[0-9]+}}: u32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: u32 = unsafe { stdc_trailing_ones_ui({{_v[0-9]+}} as u32) };
// REWRITES-NEXT: let {{_v[0-9]+}}: u32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: u32 = unsafe { stdc_first_leading_zero_ui({{_v[0-9]+}} as u32) };
// REWRITES-NEXT: let {{_v[0-9]+}}: u32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: u32 = unsafe { stdc_first_leading_one_ui({{_v[0-9]+}} as u32) };
// REWRITES-NEXT: let {{_v[0-9]+}}: u32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: u32 = unsafe { stdc_first_trailing_zero_ui({{_v[0-9]+}} as u32) };
// REWRITES-NEXT: let {{_v[0-9]+}}: u32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: u32 = unsafe { stdc_first_trailing_one_ui({{_v[0-9]+}} as u32) };
// REWRITES-NEXT: let {{_v[0-9]+}}: u32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: u32 = unsafe { stdc_count_zeros_ui({{_v[0-9]+}} as u32) };
// REWRITES-NEXT: let {{_v[0-9]+}}: u32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: u32 = unsafe { stdc_count_ones_ui({{_v[0-9]+}} as u32) };
// REWRITES-NEXT: let {{_v[0-9]+}}: u32 = {{_v[0-9]+}} + {{_v[0-9]+}} + (true as u32);
// REWRITES-NEXT: let {{_v[0-9]+}}: u32 = unsafe { stdc_bit_width_ui({{_v[0-9]+}} as u32) };
// REWRITES-NEXT: let {{_v[0-9]+}}: u32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: u32 = unsafe { stdc_bit_floor_ui({{_v[0-9]+}} as u32) };
// REWRITES-NEXT: let {{_v[0-9]+}}: u32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: u32 = unsafe { stdc_bit_ceil_ui({{_v[0-9]+}} as u32) };
// REWRITES-NEXT: return ({{_v[0-9]+}} + {{_v[0-9]+}}) as i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn c23_checked_arithmetic() -> i32 {
// REWRITES-NEXT: let mut result: i32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 20;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 22;
// REWRITES-NEXT: let {{_v[0-9]+}} = {{_v[0-9]+}}.overflowing_add({{_v[0-9]+}});
// REWRITES-NEXT: let {{_v[0-9]+}}: bool = {{_v[0-9]+}}.1 || ({{_v[0-9]+}}.0 < -2147483648 || {{_v[0-9]+}}.0 > 2147483647);
// REWRITES-NEXT: result = {{_v[0-9]+}}.0 as i32;
// REWRITES-NEXT: let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = 42;
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = result == {{_v[0-9]+}};
// REWRITES-NEXT:     {{_v[0-9]+}}
// REWRITES-NEXT: } else {
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = false;
// REWRITES-NEXT:     {{_v[0-9]+}}
// REWRITES-NEXT: };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + ({{_v[0-9]+}} as i32);
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 50;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 8;
// REWRITES-NEXT: let {{_v[0-9]+}} = {{_v[0-9]+}}.overflowing_sub({{_v[0-9]+}});
// REWRITES-NEXT: let {{_v[0-9]+}}: bool = {{_v[0-9]+}}.1 || ({{_v[0-9]+}}.0 < -2147483648 || {{_v[0-9]+}}.0 > 2147483647);
// REWRITES-NEXT: result = {{_v[0-9]+}}.0 as i32;
// REWRITES-NEXT: let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = 42;
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = result == {{_v[0-9]+}};
// REWRITES-NEXT:     {{_v[0-9]+}}
// REWRITES-NEXT: } else {
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = false;
// REWRITES-NEXT:     {{_v[0-9]+}}
// REWRITES-NEXT: };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + ({{_v[0-9]+}} as i32);
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 6;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 7;
// REWRITES-NEXT: let {{_v[0-9]+}} = {{_v[0-9]+}}.overflowing_mul({{_v[0-9]+}});
// REWRITES-NEXT: let {{_v[0-9]+}}: bool = {{_v[0-9]+}}.1 || ({{_v[0-9]+}}.0 < -2147483648 || {{_v[0-9]+}}.0 > 2147483647);
// REWRITES-NEXT: result = {{_v[0-9]+}}.0 as i32;
// REWRITES-NEXT: let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = 42;
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = result == {{_v[0-9]+}};
// REWRITES-NEXT:     {{_v[0-9]+}}
// REWRITES-NEXT: } else {
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = false;
// REWRITES-NEXT:     {{_v[0-9]+}}
// REWRITES-NEXT: };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + ({{_v[0-9]+}} as i32);
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 2147483647;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}} = {{_v[0-9]+}}.overflowing_add({{_v[0-9]+}});
// REWRITES-NEXT: let {{_v[0-9]+}}: bool = {{_v[0-9]+}}.1 || ({{_v[0-9]+}}.0 < -2147483648 || {{_v[0-9]+}}.0 > 2147483647);
// REWRITES-NEXT: result = {{_v[0-9]+}}.0 as i32;
// REWRITES-NEXT: return {{_v[0-9]+}} + ({{_v[0-9]+}} as i32);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn c23_utf8() -> i32 {
// REWRITES-NEXT: let mut input_state: __mbstate_t = __mbstate_t { __count: 0, __value: unsafe { std::mem::zeroed::<{{anon_[0-9]+}}>() } };
// REWRITES-NEXT: let mut output_state: __mbstate_t = __mbstate_t { __count: 0, __value: unsafe { std::mem::zeroed::<{{anon_[0-9]+}}>() } };
// REWRITES-NEXT: let mut character: u8 = 0;
// REWRITES-NEXT: let mut output: [i8; 4] = [0; 4];
// REWRITES-NEXT: let mut atomic_character: u8 = 0;
// REWRITES-NEXT: input_state = __mbstate_t { __count: 0, __value: unsafe { std::mem::zeroed::<{{anon_[0-9]+}}>() } };
// REWRITES-NEXT: output_state = __mbstate_t { __count: 0, __value: unsafe { std::mem::zeroed::<{{anon_[0-9]+}}>() } };
// REWRITES-NEXT: character = 0;
// REWRITES-NEXT: output = [0, 0, 0, 0];
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"A\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = (unsafe { mbrtoc8(std::ptr::addr_of_mut!(character) as *mut u8, {{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as usize, std::ptr::addr_of_mut!(input_state) as *mut __mbstate_t) }) as u64;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = output.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = (unsafe { c8rtomb({{_v[0-9]+}} as *mut i8, character as u8, std::ptr::addr_of_mut!(output_state) as *mut __mbstate_t) }) as u64;
// REWRITES-NEXT: atomic_character = character;
// REWRITES-NEXT: let {{_v[0-9]+}}: u8 = 66;
// REWRITES-NEXT: unsafe { std::sync::atomic::AtomicU8::from_ptr(std::ptr::addr_of_mut!(atomic_character)).store({{_v[0-9]+}}, std::sync::atomic::Ordering::SeqCst) };
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 65;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = (({{_v[0-9]+}} == {{_v[0-9]+}}) as i32) + (({{_v[0-9]+}} == {{_v[0-9]+}}) as i32) + (((output[({{_v[0-9]+}} as usize)] as i32) == {{_v[0-9]+}}) as i32);
// REWRITES-NEXT: let {{_v[0-9]+}}: u8 = unsafe { std::sync::atomic::AtomicU8::from_ptr(std::ptr::addr_of_mut!(atomic_character)).load(std::sync::atomic::Ordering::SeqCst) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 66;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 2;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: return {{_v[0-9]+}} + ((({{_v[0-9]+}} as i32) == {{_v[0-9]+}}) as i32) + (({{_v[0-9]+}} > {{_v[0-9]+}}) as i32);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn c23_memory() -> i32 {
// REWRITES-NEXT: let mut source: [i8; 7] = [97, 98, 99, 100, 101, 102, 0];
// REWRITES-NEXT: let mut destination: [i8; 8] = [0, 0, 0, 0, 0, 0, 0, 0];
// REWRITES-NEXT: let mut secret: [i8; 7] = [115, 101, 99, 114, 101, 116, 0];
// REWRITES-NEXT: let mut first_copy: *mut i8 = std::ptr::null_mut();
// REWRITES-NEXT: let mut second_copy: *mut i8 = std::ptr::null_mut();
// REWRITES-NEXT: let mut mutable_phrase: [i8; 12] = [104, 101, 108, 108, 111, 32, 119, 111, 114, 108, 100, 0];
// REWRITES-NEXT: let mut mut_hit: *mut i8 = std::ptr::null_mut();
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"hello world\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = destination.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = source.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 99;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 6;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe { memccpy({{_v[0-9]+}} as *mut core::ffi::c_void, ({{_v[0-9]+}} as *mut core::ffi::c_void) as *const core::ffi::c_void, {{_v[0-9]+}} as i32, {{_v[0-9]+}} as usize) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = destination.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 3;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = unsafe { {{_v[0-9]+}}.add(3) };
// REWRITES-NEXT: let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} == ({{_v[0-9]+}} as *mut core::ffi::c_void) {
// REWRITES-NEXT:         let {{_v[0-9]+}}: i64 = 2;
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = 99;
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = (destination[({{_v[0-9]+}} as usize)] as i32) == {{_v[0-9]+}};
// REWRITES-NEXT:     {{_v[0-9]+}}
// REWRITES-NEXT: } else {
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = false;
// REWRITES-NEXT:     {{_v[0-9]+}}
// REWRITES-NEXT: };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = secret.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 7;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe { memset_explicit(({{_v[0-9]+}} as *mut core::ffi::c_void) as *mut core::ffi::c_void, {{_v[0-9]+}} as i32, {{_v[0-9]+}} as usize) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: bool = if (secret[({{_v[0-9]+}} as usize)] as i32) == {{_v[0-9]+}} {
// REWRITES-NEXT:         let {{_v[0-9]+}}: i64 = 5;
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = (secret[({{_v[0-9]+}} as usize)] as i32) == {{_v[0-9]+}};
// REWRITES-NEXT:     {{_v[0-9]+}}
// REWRITES-NEXT: } else {
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = false;
// REWRITES-NEXT:     {{_v[0-9]+}}
// REWRITES-NEXT: };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + ({{_v[0-9]+}} as i32);
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"c23\0".as_ptr() as *mut i8;
// REWRITES-NEXT: first_copy = unsafe { strdup({{_v[0-9]+}} as *const i8) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"library\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 3;
// REWRITES-NEXT: second_copy = unsafe { strndup({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as usize) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// REWRITES-NEXT: let {{_v[0-9]+}}: bool = first_copy != {{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// REWRITES-NEXT:         let {{_v[0-9]+}}: *mut i8 = first_copy;
// REWRITES-NEXT:         let {{_v[0-9]+}}: *mut i8 = b"c23\0".as_ptr() as *mut i8;
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = unsafe { strcmp({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as *const i8) };
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// REWRITES-NEXT:     {{_v[0-9]+}}
// REWRITES-NEXT: } else {
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = false;
// REWRITES-NEXT:     {{_v[0-9]+}}
// REWRITES-NEXT: };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + ({{_v[0-9]+}} as i32);
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// REWRITES-NEXT: let {{_v[0-9]+}}: bool = second_copy != {{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// REWRITES-NEXT:         let {{_v[0-9]+}}: *mut i8 = second_copy;
// REWRITES-NEXT:         let {{_v[0-9]+}}: *mut i8 = b"lib\0".as_ptr() as *mut i8;
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = unsafe { strcmp({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as *const i8) };
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// REWRITES-NEXT:     {{_v[0-9]+}}
// REWRITES-NEXT: } else {
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = false;
// REWRITES-NEXT:     {{_v[0-9]+}}
// REWRITES-NEXT: };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + ({{_v[0-9]+}} as i32);
// REWRITES-NEXT: unsafe { free((first_copy as *mut core::ffi::c_void) as *mut core::ffi::c_void) };
// REWRITES-NEXT: unsafe { free((second_copy as *mut core::ffi::c_void) as *mut core::ffi::c_void) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 119;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = unsafe { strchr({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as i32) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = mutable_phrase.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 119;
// REWRITES-NEXT: mut_hit = unsafe { strchr({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as i32) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// REWRITES-NEXT: let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} != {{_v[0-9]+}} {
// REWRITES-NEXT:         let {{_v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = mut_hit != {{_v[0-9]+}};
// REWRITES-NEXT:     {{_v[0-9]+}}
// REWRITES-NEXT: } else {
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = false;
// REWRITES-NEXT:     {{_v[0-9]+}}
// REWRITES-NEXT: };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + ({{_v[0-9]+}} as i32);
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 111;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 11;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe { memchr(({{_v[0-9]+}} as *mut core::ffi::c_void) as *const core::ffi::c_void, {{_v[0-9]+}} as i32, {{_v[0-9]+}} as usize) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + (({{_v[0-9]+}} != {{_v[0-9]+}}) as i32);
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = mutable_phrase.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 111;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 11;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe { memchr(({{_v[0-9]+}} as *mut core::ffi::c_void) as *const core::ffi::c_void, {{_v[0-9]+}} as i32, {{_v[0-9]+}} as usize) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + (({{_v[0-9]+}} != {{_v[0-9]+}}) as i32);
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"world\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = unsafe { strstr({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as *const i8) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + (({{_v[0-9]+}} != {{_v[0-9]+}}) as i32);
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = mutable_phrase.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"world\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = unsafe { strstr({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as *const i8) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// REWRITES-NEXT: return {{_v[0-9]+}} + (({{_v[0-9]+}} != {{_v[0-9]+}}) as i32);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn c23_time() -> i32 {
// REWRITES-NEXT: let mut timestamp: i64 = 0;
// REWRITES-NEXT: let mut utc: tm = tm { tm_sec: 0, tm_min: 0, tm_hour: 0, tm_mday: 0, tm_mon: 0, tm_year: 0, tm_wday: 0, tm_yday: 0, tm_isdst: 0, __tm_gmtoff: 0, __tm_zone: std::ptr::null_mut() };
// REWRITES-NEXT: let mut local: tm = tm { tm_sec: 0, tm_min: 0, tm_hour: 0, tm_mday: 0, tm_mon: 0, tm_year: 0, tm_wday: 0, tm_yday: 0, tm_isdst: 0, __tm_gmtoff: 0, __tm_zone: std::ptr::null_mut() };
// REWRITES-NEXT: let mut resolution: libc::timespec = libc::timespec { tv_sec: 0, tv_nsec: 0 };
// REWRITES-NEXT: let mut month: aligned::Aligned<aligned::A16, [i8; 32]> = aligned::Aligned([0; 32]);
// REWRITES-NEXT: let mut wide_month: aligned::Aligned<aligned::A16, [i32; 32]> = aligned::Aligned([0; 32]);
// REWRITES-NEXT: timestamp = 0;
// REWRITES-NEXT: utc = tm { tm_sec: 0, tm_min: 0, tm_hour: 0, tm_mday: 0, tm_mon: 0, tm_year: 0, tm_wday: 0, tm_yday: 0, tm_isdst: 0, __tm_gmtoff: 0, __tm_zone: std::ptr::null_mut() };
// REWRITES-NEXT: local = tm { tm_sec: 0, tm_min: 0, tm_hour: 0, tm_mday: 0, tm_mon: 0, tm_year: 0, tm_wday: 0, tm_yday: 0, tm_isdst: 0, __tm_gmtoff: 0, __tm_zone: std::ptr::null_mut() };
// REWRITES-NEXT: resolution = libc::timespec { tv_sec: 0, tv_nsec: 0 };
// REWRITES-NEXT: *month = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
// REWRITES-NEXT: *wide_month = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut tm = unsafe { gmtime_r(std::ptr::addr_of_mut!(timestamp) as *const i64, std::ptr::addr_of_mut!(utc) as *mut tm) };
// REWRITES-NEXT: let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == std::ptr::addr_of_mut!(utc);
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut tm = unsafe { localtime_r(std::ptr::addr_of_mut!(timestamp) as *const i64, std::ptr::addr_of_mut!(local) as *mut tm) };
// REWRITES-NEXT: let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == std::ptr::addr_of_mut!(local);
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + ({{_v[0-9]+}} as i32);
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { timespec_getres(std::ptr::addr_of_mut!(resolution) as *mut libc::timespec, {{_v[0-9]+}} as i32) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + (({{_v[0-9]+}} == {{_v[0-9]+}}) as i32);
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: bool = if resolution.tv_sec > {{_v[0-9]+}} {
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = true;
// REWRITES-NEXT:     {{_v[0-9]+}}
// REWRITES-NEXT: } else {
// REWRITES-NEXT:         let {{_v[0-9]+}}: i64 = 0;
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = resolution.tv_nsec > {{_v[0-9]+}};
// REWRITES-NEXT:     {{_v[0-9]+}}
// REWRITES-NEXT: };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + ({{_v[0-9]+}} as i32);
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = unsafe { timegm(std::ptr::addr_of_mut!(utc) as *mut tm) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + (({{_v[0-9]+}} == {{_v[0-9]+}}) as i32);
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = month.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 32;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%OB\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = (unsafe { strftime({{_v[0-9]+}} as *mut i8, {{_v[0-9]+}} as usize, {{_v[0-9]+}} as *const i8, std::ptr::addr_of_mut!(utc) as *const tm) }) as u64;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 7;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + (({{_v[0-9]+}} == {{_v[0-9]+}}) as i32);
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = month.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"January\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { strcmp({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as *const i8) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + (({{_v[0-9]+}} == {{_v[0-9]+}}) as i32);
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i32 = wide_month.as_mut_ptr() as *mut i32;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 32;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i32 = std::ptr::addr_of_mut!(_str_9).cast::<i32>();
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = (unsafe { wcsftime({{_v[0-9]+}} as *mut i32, {{_v[0-9]+}} as usize, {{_v[0-9]+}} as *const i32, std::ptr::addr_of_mut!(utc) as *const tm) }) as u64;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 7;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + (({{_v[0-9]+}} == {{_v[0-9]+}}) as i32);
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i32 = wide_month.as_mut_ptr() as *mut i32;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i32 = std::ptr::addr_of_mut!(_str_10).cast::<i32>();
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { wcscmp({{_v[0-9]+}} as *const i32, {{_v[0-9]+}} as *const i32) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + (({{_v[0-9]+}} == {{_v[0-9]+}}) as i32);
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i32 = wide_month.as_mut_ptr() as *mut i32;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 110;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i32 = unsafe { wcschr({{_v[0-9]+}} as *const i32, {{_v[0-9]+}} as i32) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i32 = std::ptr::null_mut();
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + (({{_v[0-9]+}} != {{_v[0-9]+}}) as i32);
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i32 = wide_month.as_mut_ptr() as *mut i32;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 110;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i32 = unsafe { wcschr({{_v[0-9]+}} as *const i32, {{_v[0-9]+}} as i32) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i32 = std::ptr::null_mut();
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + (({{_v[0-9]+}} != {{_v[0-9]+}}) as i32);
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i32 = std::ptr::addr_of_mut!(_str_11).cast::<i32>();
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i32 = unsafe { wcsstr({{_v[0-9]+}} as *const i32, {{_v[0-9]+}} as *const i32) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i32 = std::ptr::null_mut();
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + (({{_v[0-9]+}} != {{_v[0-9]+}}) as i32);
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i32 = wide_month.as_mut_ptr() as *mut i32;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i32 = std::ptr::addr_of_mut!(_str_11).cast::<i32>();
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i32 = unsafe { wcsstr({{_v[0-9]+}} as *const i32, {{_v[0-9]+}} as *const i32) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i32 = std::ptr::null_mut();
// REWRITES-NEXT: return {{_v[0-9]+}} + (({{_v[0-9]+}} != {{_v[0-9]+}}) as i32);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn c23_io() -> i32 {
// REWRITES-NEXT: let mut output: aligned::Aligned<aligned::A16, [i8; 64]> = aligned::Aligned([0; 64]);
// REWRITES-NEXT: let mut float_output: aligned::Aligned<aligned::A16, [i8; 16]> = aligned::Aligned([0; 16]);
// REWRITES-NEXT: let mut double_output: aligned::Aligned<aligned::A16, [i8; 16]> = aligned::Aligned([0; 16]);
// REWRITES-NEXT: let mut long_double_output: aligned::Aligned<aligned::A16, [i8; 16]> = aligned::Aligned([0; 16]);
// REWRITES-NEXT: let mut binary_value: u32 = 0;
// REWRITES-NEXT: let mut exact_value: u16 = 0;
// REWRITES-NEXT: let mut fast_value: u64 = 0;
// REWRITES-NEXT: *output = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
// REWRITES-NEXT: *float_output = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
// REWRITES-NEXT: *double_output = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
// REWRITES-NEXT: *long_double_output = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
// REWRITES-NEXT: binary_value = 0;
// REWRITES-NEXT: exact_value = 0;
// REWRITES-NEXT: fast_value = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = output.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 64;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%b %w16u %wf16u\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: u32 = 13;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 21;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 34;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { snprintf({{_v[0-9]+}} as *mut i8, {{_v[0-9]+}} as usize, {{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"1011 55 89\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%b %w16u %wf16u\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { sscanf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as *const i8, std::ptr::addr_of_mut!(binary_value), std::ptr::addr_of_mut!(exact_value), std::ptr::addr_of_mut!(fast_value)) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = float_output.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 16;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%.1f\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: f32 = 1.5;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { strfromf({{_v[0-9]+}} as *mut i8, {{_v[0-9]+}} as usize, {{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as f32) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = double_output.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 16;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%.1f\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: f64 = 2.5;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { strfromd({{_v[0-9]+}} as *mut i8, {{_v[0-9]+}} as usize, {{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as f64) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = long_double_output.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 16;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%.1f\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 224, 0, 64]);
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { __slate_strfroml__ri32_pi8_usize_pi8_f80({{_v[0-9]+}} as *mut i8, {{_v[0-9]+}} as usize, {{_v[0-9]+}} as *const i8, {{_v[0-9]+}}) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = output.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"1101 21 34\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { strcmp({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as *const i8) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: u32 = 11;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 55;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 89;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + (({{_v[0-9]+}} == {{_v[0-9]+}}) as i32) + {{_v[0-9]+}} + ((binary_value == {{_v[0-9]+}}) as i32) + (((exact_value as i32) == {{_v[0-9]+}}) as i32) + ((fast_value == {{_v[0-9]+}}) as i32) + {{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = float_output.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"1.5\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { strcmp({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as *const i8) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + (({{_v[0-9]+}} == {{_v[0-9]+}}) as i32);
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = double_output.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"2.5\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { strcmp({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as *const i8) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + (({{_v[0-9]+}} == {{_v[0-9]+}}) as i32);
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = long_double_output.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"3.5\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { strcmp({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as *const i8) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: return {{_v[0-9]+}} + (({{_v[0-9]+}} == {{_v[0-9]+}}) as i32);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn c23_limits() -> i32 {
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 16;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 16;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 32;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 32;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 64;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 64;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 64;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 64;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 64;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 64;
// REWRITES-NEXT: let {{_v[0-9]+}}: f32 = 0.0000000000000000000000000000000000000000000014013;
// REWRITES-NEXT: let {{_v[0-9]+}}: f32 = 0.0;
// REWRITES-NEXT: let {{_v[0-9]+}}: f64 = 0.000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000005;
// REWRITES-NEXT: let {{_v[0-9]+}}: f64 = 0.0;
// REWRITES-NEXT: let {{_v[0-9]+}}: f32 = 340282346999999984391321947108527833088.0;
// REWRITES-NEXT: let {{_v[0-9]+}}: f32 = 340282346999999984391321947108527833088.0;
// REWRITES-NEXT: let {{_v[0-9]+}}: f64 = 179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368.0;
// REWRITES-NEXT: let {{_v[0-9]+}}: f64 = 179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368.0;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = -1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = -1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = -1;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 4;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 4;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 8;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 8;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 16;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 16;
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = 202311;
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = 202311;
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = 202311;
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = 202311;
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = 202311;
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = 202311;
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = 202311;
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = 202311;
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = 202311;
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = 202311;
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = 202311;
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = 202311;
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = 202311;
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = 202311;
// REWRITES-NEXT: return {{_v[0-9]+}} + {{_v[0-9]+}} + {{_v[0-9]+}} + {{_v[0-9]+}} + {{_v[0-9]+}} + {{_v[0-9]+}} + {{_v[0-9]+}} + {{_v[0-9]+}} + {{_v[0-9]+}} + {{_v[0-9]+}} + {{_v[0-9]+}} + {{_v[0-9]+}} + {{_v[0-9]+}} + {{_v[0-9]+}} + ((({{_v[0-9]+}} > {{_v[0-9]+}}) as i32) + (({{_v[0-9]+}} > {{_v[0-9]+}}) as i32) + ((LongDouble([1, 0, 0, 0, 0, 0, 0, 0, 0, 0]) > LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0])) as i32) + (({{_v[0-9]+}} <= {{_v[0-9]+}}) as i32) + (({{_v[0-9]+}} <= {{_v[0-9]+}}) as i32) + ((LongDouble([255, 255, 255, 255, 255, 255, 255, 255, 254, 127]) <= LongDouble([255, 255, 255, 255, 255, 255, 255, 255, 254, 127])) as i32) + (({{_v[0-9]+}} >= {{_v[0-9]+}}) as i32) + (({{_v[0-9]+}} >= {{_v[0-9]+}}) as i32) + (({{_v[0-9]+}} >= {{_v[0-9]+}}) as i32) + (({{_v[0-9]+}} == {{_v[0-9]+}}) as i32) + (({{_v[0-9]+}} == {{_v[0-9]+}}) as i32) + (({{_v[0-9]+}} == {{_v[0-9]+}}) as i32)) + ((({{_v[0-9]+}} == {{_v[0-9]+}}) as i32) + (({{_v[0-9]+}} == {{_v[0-9]+}}) as i32) + (({{_v[0-9]+}} == {{_v[0-9]+}}) as i32) + (({{_v[0-9]+}} == {{_v[0-9]+}}) as i32) + (({{_v[0-9]+}} == {{_v[0-9]+}}) as i32) + (({{_v[0-9]+}} == {{_v[0-9]+}}) as i32) + (({{_v[0-9]+}} == {{_v[0-9]+}}) as i32));
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%d %d %d %d %d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = c23_stdbit();
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = c23_checked_arithmetic();
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = c23_utf8();
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = c23_memory();
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = c23_time();
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = c23_io();
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = c23_limits();
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}} + {{_v[0-9]+}}) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: std::process::exit({{_v[0-9]+}} as i32);
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
