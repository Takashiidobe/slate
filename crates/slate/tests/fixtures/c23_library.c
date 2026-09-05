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

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING: #![feature(f128)]
// COMMON-LOWERING-NEXT: #![feature(c_variadic)]
// COMMON-LOWERING-NEXT: #![allow(
// COMMON-LOWERING-NEXT:     dead_code,
// COMMON-LOWERING-NEXT:     unused,
// COMMON-LOWERING-NEXT:     non_camel_case_types,
// COMMON-LOWERING-NEXT:     non_snake_case,
// COMMON-LOWERING-NEXT:     non_upper_case_globals,
// COMMON-LOWERING-NEXT:     arithmetic_overflow,
// COMMON-LOWERING-NEXT:     unconditional_panic,
// COMMON-LOWERING-NEXT:     suspicious_runtime_symbol_definitions,
// COMMON-LOWERING-NEXT:     unpredictable_function_pointer_comparisons,
// COMMON-LOWERING-NEXT:     unused_comparisons
// COMMON-LOWERING-NEXT: )]
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[allow(non_camel_case_types)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy, PartialEq, Eq, Debug, Hash)]
// COMMON-LOWERING-NEXT: enum memory_order {
// COMMON-LOWERING-NEXT:     memory_order_relaxed = 0,
// COMMON-LOWERING-NEXT:     memory_order_consume = 1,
// COMMON-LOWERING-NEXT:     memory_order_acquire = 2,
// COMMON-LOWERING-NEXT:     memory_order_release = 3,
// COMMON-LOWERING-NEXT:     memory_order_acq_rel = 4,
// COMMON-LOWERING-NEXT:     memory_order_seq_cst = 5,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct __mbstate_t {
// COMMON-LOWERING-NEXT:     __count: i32,
// COMMON-LOWERING-NEXT:     __value: {{anon_[0-9]+}},
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: union {{anon_[0-9]+}} {
// COMMON-LOWERING-NEXT:     __wch: i32,
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: struct tm {
// COMMON-LOWERING-NEXT:     tm_sec: i32,
// COMMON-LOWERING-NEXT:     tm_min: i32,
// COMMON-LOWERING-NEXT:     tm_hour: i32,
// COMMON-LOWERING-NEXT:     tm_mday: i32,
// COMMON-LOWERING-NEXT:     tm_mon: i32,
// COMMON-LOWERING-NEXT:     tm_year: i32,
// COMMON-LOWERING-NEXT:     tm_wday: i32,
// COMMON-LOWERING-NEXT:     tm_yday: i32,
// COMMON-LOWERING-NEXT:     tm_isdst: i32,
// COMMON-LOWERING-NEXT:     __tm_gmtoff: i64,
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT:     fn mbrtoc8(_0: *mut u8, _1: *const core::ffi::c_char, _2: usize, _3: *mut __mbstate_t)
// COMMON-LOWERING-NEXT:     -> usize;
// COMMON-LOWERING-NEXT:     fn c8rtomb(_0: *mut core::ffi::c_char, _1: u8, _2: *mut __mbstate_t) -> usize;
// COMMON-LOWERING-NEXT:     fn memccpy(
// COMMON-LOWERING-NEXT:         _0: *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:         _1: *const core::ffi::c_void,
// COMMON-LOWERING-NEXT:         _2: i32,
// COMMON-LOWERING-NEXT:         _3: usize,
// COMMON-LOWERING-NEXT:     ) -> *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     fn memset_explicit(_0: *mut core::ffi::c_void, _1: i32, _2: usize) -> *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     fn strdup(_0: *const core::ffi::c_char) -> *mut core::ffi::c_char;
// COMMON-LOWERING-NEXT:     fn strndup(_0: *const core::ffi::c_char, _1: usize) -> *mut core::ffi::c_char;
// COMMON-LOWERING-NEXT:     fn strcmp(_0: *const core::ffi::c_char, _1: *const core::ffi::c_char) -> i32;
// COMMON-LOWERING-NEXT:     fn free(_0: *mut core::ffi::c_void);
// COMMON-LOWERING-NEXT:     fn strchr(_0: *const core::ffi::c_char, _1: i32) -> *mut core::ffi::c_char;
// COMMON-LOWERING-NEXT:     fn memchr(_0: *const core::ffi::c_void, _1: i32, _2: usize) -> *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     fn strstr(_0: *const core::ffi::c_char, _1: *const core::ffi::c_char)
// COMMON-LOWERING-NEXT:     -> *mut core::ffi::c_char;
// COMMON-LOWERING-NEXT:     fn gmtime_r(_0: *const i64, _1: *mut tm) -> *mut tm;
// COMMON-LOWERING-NEXT:     fn localtime_r(_0: *const i64, _1: *mut tm) -> *mut tm;
// COMMON-LOWERING-NEXT:     fn timespec_getres(_0: *mut libc::timespec, _1: i32) -> i32;
// COMMON-LOWERING-NEXT:     fn timegm(_0: *mut tm) -> i64;
// COMMON-LOWERING-NEXT:     fn strftime(
// COMMON-LOWERING-NEXT:         _0: *mut core::ffi::c_char,
// COMMON-LOWERING-NEXT:         _1: usize,
// COMMON-LOWERING-NEXT:         _2: *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:         _3: *const tm,
// COMMON-LOWERING-NEXT:     ) -> usize;
// COMMON-LOWERING-NEXT:     fn snprintf(_0: *mut core::ffi::c_char, _1: usize, _2: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT:     fn sscanf(_0: *const core::ffi::c_char, _1: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT:     fn strfromf(
// COMMON-LOWERING-NEXT:         _0: *mut core::ffi::c_char,
// COMMON-LOWERING-NEXT:         _1: usize,
// COMMON-LOWERING-NEXT:         _2: *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:         _3: f32,
// COMMON-LOWERING-NEXT:     ) -> i32;
// COMMON-LOWERING-NEXT:     fn strfromd(
// COMMON-LOWERING-NEXT:         _0: *mut core::ffi::c_char,
// COMMON-LOWERING-NEXT:         _1: usize,
// COMMON-LOWERING-NEXT:         _2: *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:         _3: f64,
// COMMON-LOWERING-NEXT:     ) -> i32;
// COMMON-LOWERING-NEXT:     fn strfroml(
// COMMON-LOWERING-NEXT:         _0: *mut core::ffi::c_char,
// COMMON-LOWERING-NEXT:         _1: usize,
// COMMON-LOWERING-NEXT:         _2: *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:     ) -> i32;
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = c23_stdbit();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = c23_checked_arithmetic();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = c23_utf8();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = c23_memory();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = c23_time();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = c23_io();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = c23_limits();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         printf(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: fn c23_stdbit() -> i32 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = 176;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}}.leading_zeros() as u32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = !{{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}}.leading_zeros() as u32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}}.trailing_zeros() as u32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = !{{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}}.trailing_zeros() as u32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = !{{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}}.leading_zeros() as u32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}}.leading_zeros() as u32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = !{{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}}.trailing_zeros() as u32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}}.trailing_zeros() as u32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}}.count_ones() as u32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = 32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}} - {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}}.count_ones() as u32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = true;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}} as u32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}}.leading_zeros() as u32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = 32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}} - {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = 31;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}}.leading_zeros() as u32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}} - {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}} - {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}}.leading_zeros() as u32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = 31;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}} - {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} <= {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: fn c23_checked_arithmetic() -> i32 {
// COMMON-LOWERING-NEXT:     let mut result: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 20;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 22;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}} = {{__v[0-9]+}}.overflowing_add({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}}.0 as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.1 || ({{__v[0-9]+}}.0 < -2147483648 || {{__v[0-9]+}}.0 > 2147483647);
// COMMON-LOWERING-NEXT:     result = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = result;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 42;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 50;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 8;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}} = {{__v[0-9]+}}.overflowing_sub({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}}.0 as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.1 || ({{__v[0-9]+}}.0 < -2147483648 || {{__v[0-9]+}}.0 > 2147483647);
// COMMON-LOWERING-NEXT:     result = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = result;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 42;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 6;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 7;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}} = {{__v[0-9]+}}.overflowing_mul({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}}.0 as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.1 || ({{__v[0-9]+}}.0 < -2147483648 || {{__v[0-9]+}}.0 > 2147483647);
// COMMON-LOWERING-NEXT:     result = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = result;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 42;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 2147483647;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}} = {{__v[0-9]+}}.overflowing_add({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}}.0 as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.1 || ({{__v[0-9]+}}.0 < -2147483648 || {{__v[0-9]+}}.0 > 2147483647);
// COMMON-LOWERING-NEXT:     result = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: fn c23_utf8() -> i32 {
// COMMON-LOWERING-NEXT:     let mut input_state: __mbstate_t = __mbstate_t {
// COMMON-LOWERING-NEXT:         __count: 0,
// COMMON-LOWERING-NEXT:         __value: unsafe { std::mem::zeroed::<{{anon_[0-9]+}}>() },
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let mut output_state: __mbstate_t = __mbstate_t {
// COMMON-LOWERING-NEXT:         __count: 0,
// COMMON-LOWERING-NEXT:         __value: unsafe { std::mem::zeroed::<{{anon_[0-9]+}}>() },
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let mut character: u8 = 0;
// COMMON-LOWERING-NEXT:     let mut atomic_character: u8 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: __mbstate_t = __mbstate_t {
// COMMON-LOWERING-NEXT:         __count: 0,
// COMMON-LOWERING-NEXT:         __value: unsafe { std::mem::zeroed::<{{anon_[0-9]+}}>() },
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     input_state = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: __mbstate_t = __mbstate_t {
// COMMON-LOWERING-NEXT:         __count: 0,
// COMMON-LOWERING-NEXT:         __value: unsafe { std::mem::zeroed::<{{anon_[0-9]+}}>() },
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     output_state = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = 0;
// COMMON-LOWERING-NEXT:     character = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     output = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = (unsafe {
// COMMON-LOWERING-NEXT:         mbrtoc8(
// COMMON-LOWERING-NEXT:             std::ptr::addr_of_mut!(character) as *mut u8,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as usize,
// COMMON-LOWERING-NEXT:             std::ptr::addr_of_mut!(input_state) as *mut __mbstate_t,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     }) as u64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = character;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = (unsafe {
// COMMON-LOWERING-NEXT:         c8rtomb(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *mut core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as u8,
// COMMON-LOWERING-NEXT:             std::ptr::addr_of_mut!(output_state) as *mut __mbstate_t,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     }) as u64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = character;
// COMMON-LOWERING-NEXT:     atomic_character = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = 66;
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         std::sync::atomic::AtomicU8::from_ptr(std::ptr::addr_of_mut!(atomic_character))
// COMMON-LOWERING-NEXT:             .store({{__v[0-9]+}}, std::sync::atomic::Ordering::SeqCst)
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 65;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = unsafe {
// COMMON-LOWERING-NEXT:         std::sync::atomic::AtomicU8::from_ptr(std::ptr::addr_of_mut!(atomic_character))
// COMMON-LOWERING-NEXT:             .load(std::sync::atomic::Ordering::SeqCst)
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 66;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 2;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} > {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: fn c23_memory() -> i32 {
// COMMON-LOWERING-NEXT:     source = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     destination = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     secret = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     mutable_phrase = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 99;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 6;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe {
// COMMON-LOWERING-NEXT:         memccpy(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_void,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as i32,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as usize,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 3;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i64 = 2;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 99;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 7;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe {
// COMMON-LOWERING-NEXT:         memset_explicit(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as i32,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as usize,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i64 = 5;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     first_copy = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 3;
// COMMON-LOWERING-NEXT:     second_copy = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:             strcmp(
// COMMON-LOWERING-NEXT:                 {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:                 {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             )
// COMMON-LOWERING-NEXT:         };
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:             strcmp(
// COMMON-LOWERING-NEXT:                 {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:                 {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             )
// COMMON-LOWERING-NEXT:         };
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     unsafe { free({{__v[0-9]+}} as *mut core::ffi::c_void) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     unsafe { free({{__v[0-9]+}} as *mut core::ffi::c_void) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 119;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 119;
// COMMON-LOWERING-NEXT:     mut_hit = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 111;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 11;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe {
// COMMON-LOWERING-NEXT:         memchr(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_void,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as i32,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as usize,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 111;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 11;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe {
// COMMON-LOWERING-NEXT:         memchr(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_void,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as i32,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as usize,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         strstr(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         strstr(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: fn c23_time() -> i32 {
// COMMON-LOWERING-NEXT:     let mut timestamp: i64 = 0;
// COMMON-LOWERING-NEXT:     let mut utc: tm = tm {
// COMMON-LOWERING-NEXT:         tm_sec: 0,
// COMMON-LOWERING-NEXT:         tm_min: 0,
// COMMON-LOWERING-NEXT:         tm_hour: 0,
// COMMON-LOWERING-NEXT:         tm_mday: 0,
// COMMON-LOWERING-NEXT:         tm_mon: 0,
// COMMON-LOWERING-NEXT:         tm_year: 0,
// COMMON-LOWERING-NEXT:         tm_wday: 0,
// COMMON-LOWERING-NEXT:         tm_yday: 0,
// COMMON-LOWERING-NEXT:         tm_isdst: 0,
// COMMON-LOWERING-NEXT:         __tm_gmtoff: 0,
// COMMON-LOWERING-NEXT:         __tm_zone: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let mut local: tm = tm {
// COMMON-LOWERING-NEXT:         tm_sec: 0,
// COMMON-LOWERING-NEXT:         tm_min: 0,
// COMMON-LOWERING-NEXT:         tm_hour: 0,
// COMMON-LOWERING-NEXT:         tm_mday: 0,
// COMMON-LOWERING-NEXT:         tm_mon: 0,
// COMMON-LOWERING-NEXT:         tm_year: 0,
// COMMON-LOWERING-NEXT:         tm_wday: 0,
// COMMON-LOWERING-NEXT:         tm_yday: 0,
// COMMON-LOWERING-NEXT:         tm_isdst: 0,
// COMMON-LOWERING-NEXT:         __tm_gmtoff: 0,
// COMMON-LOWERING-NEXT:         __tm_zone: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let mut resolution: libc::timespec = libc::timespec {
// COMMON-LOWERING-NEXT:         tv_sec: 0,
// COMMON-LOWERING-NEXT:         tv_nsec: 0,
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// COMMON-LOWERING-NEXT:     timestamp = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: tm = tm {
// COMMON-LOWERING-NEXT:         tm_sec: 0,
// COMMON-LOWERING-NEXT:         tm_min: 0,
// COMMON-LOWERING-NEXT:         tm_hour: 0,
// COMMON-LOWERING-NEXT:         tm_mday: 0,
// COMMON-LOWERING-NEXT:         tm_mon: 0,
// COMMON-LOWERING-NEXT:         tm_year: 0,
// COMMON-LOWERING-NEXT:         tm_wday: 0,
// COMMON-LOWERING-NEXT:         tm_yday: 0,
// COMMON-LOWERING-NEXT:         tm_isdst: 0,
// COMMON-LOWERING-NEXT:         __tm_gmtoff: 0,
// COMMON-LOWERING-NEXT:         __tm_zone: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     utc = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: tm = tm {
// COMMON-LOWERING-NEXT:         tm_sec: 0,
// COMMON-LOWERING-NEXT:         tm_min: 0,
// COMMON-LOWERING-NEXT:         tm_hour: 0,
// COMMON-LOWERING-NEXT:         tm_mday: 0,
// COMMON-LOWERING-NEXT:         tm_mon: 0,
// COMMON-LOWERING-NEXT:         tm_year: 0,
// COMMON-LOWERING-NEXT:         tm_wday: 0,
// COMMON-LOWERING-NEXT:         tm_yday: 0,
// COMMON-LOWERING-NEXT:         tm_isdst: 0,
// COMMON-LOWERING-NEXT:         __tm_gmtoff: 0,
// COMMON-LOWERING-NEXT:         __tm_zone: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     local = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: libc::timespec = libc::timespec {
// COMMON-LOWERING-NEXT:         tv_sec: 0,
// COMMON-LOWERING-NEXT:         tv_nsec: 0,
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     resolution = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut tm = unsafe {
// COMMON-LOWERING-NEXT:         gmtime_r(
// COMMON-LOWERING-NEXT:             std::ptr::addr_of_mut!(timestamp) as *const i64,
// COMMON-LOWERING-NEXT:             std::ptr::addr_of_mut!(utc) as *mut tm,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == std::ptr::addr_of_mut!(utc);
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut tm = unsafe {
// COMMON-LOWERING-NEXT:         localtime_r(
// COMMON-LOWERING-NEXT:             std::ptr::addr_of_mut!(timestamp) as *const i64,
// COMMON-LOWERING-NEXT:             std::ptr::addr_of_mut!(local) as *mut tm,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == std::ptr::addr_of_mut!(local);
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         timespec_getres(
// COMMON-LOWERING-NEXT:             std::ptr::addr_of_mut!(resolution) as *mut libc::timespec,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as i32,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = resolution.tv_sec;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} > {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = true;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i64 = resolution.tv_nsec;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i64 = 0;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} > {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { timegm(std::ptr::addr_of_mut!(utc) as *mut tm) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = (unsafe {
// COMMON-LOWERING-NEXT:         strftime(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *mut core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as usize,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             std::ptr::addr_of_mut!(utc) as *const tm,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     }) as u64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 7;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         strcmp(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = (unsafe {
// COMMON-LOWERING-NEXT:         wcsftime(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as usize,
// COMMON-LOWERING-NEXT:             std::ptr::addr_of_mut!(utc) as *const tm,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     }) as u64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 7;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: fn c23_io() -> i32 {
// COMMON-LOWERING-NEXT:     let mut binary_value: u32 = 0;
// COMMON-LOWERING-NEXT:     let mut exact_value: u16 = 0;
// COMMON-LOWERING-NEXT:     let mut fast_value: u64 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = 0;
// COMMON-LOWERING-NEXT:     binary_value = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u16 = 0;
// COMMON-LOWERING-NEXT:     exact_value = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 0;
// COMMON-LOWERING-NEXT:     fast_value = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = 13;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 21;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 34;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         snprintf(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *mut core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as usize,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         sscanf(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             std::ptr::addr_of_mut!(binary_value),
// COMMON-LOWERING-NEXT:             std::ptr::addr_of_mut!(exact_value),
// COMMON-LOWERING-NEXT:             std::ptr::addr_of_mut!(fast_value),
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 16;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = 1.5;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         strfromf(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *mut core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as usize,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as f32,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 16;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 2.5;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         strfromd(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *mut core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as usize,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as f64,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 16;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *mut core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as usize,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         strcmp(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = binary_value;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = 11;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u16 = exact_value;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 55;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = fast_value;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 89;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         strcmp(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         strcmp(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         strcmp(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: fn c23_limits() -> i32 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 8;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 8;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 8;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 16;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 16;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = 0.0000000000000000000000000000000000000000000014013;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = 0.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} > {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 0.000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000005;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 0.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} > {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} > {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = 340282346999999984391321947108527833088.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = 340282346999999984391321947108527833088.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} <= {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} <= {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} <= {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = -1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} >= {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = -1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} >= {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = -1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} >= {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 4;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 4;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 8;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 8;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 16;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 16;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 202311;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 202311;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 202311;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 202311;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 202311;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 202311;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 202311;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 202311;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 202311;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 202311;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 202311;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 202311;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 202311;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 202311;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT: #[repr(C, align(16))]
// LOWERING-X86_64-GNU-NEXT: struct LongDouble([u8; 10]);
// LOWERING-X86_64-GNU-NEXT: impl core::ops::Add for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     type Output = LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn add(self, __o: LongDouble) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:         __slate_f80_add(self, __o)
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT: impl core::ops::Sub for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     type Output = LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn sub(self, __o: LongDouble) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:         __slate_f80_sub(self, __o)
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT: impl core::ops::Mul for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     type Output = LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn mul(self, __o: LongDouble) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:         __slate_f80_mul(self, __o)
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT: impl core::ops::Div for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     type Output = LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn div(self, __o: LongDouble) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:         __slate_f80_div(self, __o)
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT: impl core::ops::AddAssign for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     fn add_assign(&mut self, __o: LongDouble) {
// LOWERING-X86_64-GNU-NEXT:         {
// LOWERING-X86_64-GNU-NEXT:             *self = __slate_f80_add(*self, __o);
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT: impl core::ops::SubAssign for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     fn sub_assign(&mut self, __o: LongDouble) {
// LOWERING-X86_64-GNU-NEXT:         {
// LOWERING-X86_64-GNU-NEXT:             *self = __slate_f80_sub(*self, __o);
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT: impl core::ops::MulAssign for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     fn mul_assign(&mut self, __o: LongDouble) {
// LOWERING-X86_64-GNU-NEXT:         {
// LOWERING-X86_64-GNU-NEXT:             *self = __slate_f80_mul(*self, __o);
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT: impl core::ops::DivAssign for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     fn div_assign(&mut self, __o: LongDouble) {
// LOWERING-X86_64-GNU-NEXT:         {
// LOWERING-X86_64-GNU-NEXT:             *self = __slate_f80_div(*self, __o);
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT: impl core::ops::Neg for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     type Output = LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn neg(self) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:         __slate_f80_neg(self)
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT: impl core::cmp::PartialEq for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     fn eq(&self, __other: &LongDouble) -> bool {
// LOWERING-X86_64-GNU-NEXT:         __slate_f80_eq(*self, *__other)
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT: impl core::cmp::PartialOrd for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     fn partial_cmp(&self, __other: &LongDouble) -> Option<std::cmp::Ordering> {
// LOWERING-X86_64-GNU-NEXT:         if __slate_f80_lt(*self, *__other) {
// LOWERING-X86_64-GNU-NEXT:             Some(std::cmp::Ordering::Less)
// LOWERING-X86_64-GNU-NEXT:         } else {
// LOWERING-X86_64-GNU-NEXT:             if __slate_f80_gt(*self, *__other) {
// LOWERING-X86_64-GNU-NEXT:                 Some(std::cmp::Ordering::Greater)
// LOWERING-X86_64-GNU-NEXT:             } else {
// LOWERING-X86_64-GNU-NEXT:                 if __slate_f80_eq(*self, *__other) {
// LOWERING-X86_64-GNU-NEXT:                     Some(std::cmp::Ordering::Equal)
// LOWERING-X86_64-GNU-NEXT:                 } else {
// LOWERING-X86_64-GNU-NEXT:                     None
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     __wchb: [i8; 4],
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-NEXT: #[derive(Clone, Copy)]
// LOWERING-X86_64-GNU-NEXT:     __tm_zone: *mut i8,
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: static mut _str_10: [i32; 8] = [74, 97, 110, 117, 97, 114, 121, 0];
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: static mut _str_11: [i32; 4] = [74, 97, 110, 0];
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: static mut _str_9: [i32; 4] = [37, 79, 66, 0];
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT:     fn wcsftime(_0: *mut i32, _1: usize, _2: *const i32, _3: *const tm) -> usize;
// LOWERING-X86_64-GNU-NEXT:     fn wcscmp(_0: *const i32, _1: *const i32) -> i32;
// LOWERING-X86_64-GNU-NEXT:     fn wcschr(_0: *const i32, _1: i32) -> *mut i32;
// LOWERING-X86_64-GNU-NEXT:     fn wcsstr(_0: *const i32, _1: *const i32) -> *mut i32;
// LOWERING-X86_64-GNU-NEXT:         _3: LongDouble,
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d %d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT:     let mut output: [i8; 4] = [0; 4];
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i8; 4] = [0; 4];
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"A\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = output.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = output[({{__v[0-9]+}} as usize)];
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT:     let mut source: [i8; 7] = [0; 7];
// LOWERING-X86_64-GNU-NEXT:     let mut destination: [i8; 8] = [0; 8];
// LOWERING-X86_64-GNU-NEXT:     let mut secret: [i8; 7] = [0; 7];
// LOWERING-X86_64-GNU-NEXT:     let mut first_copy: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT:     let mut second_copy: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT:     let mut mutable_phrase: [i8; 12] = [0; 12];
// LOWERING-X86_64-GNU-NEXT:     let mut mut_hit: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i8; 7] = [97, 98, 99, 100, 101, 102, 0];
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i8; 8] = [0; 8];
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i8; 7] = [115, 101, 99, 114, 101, 116, 0];
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"hello world\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i8; 12] = [104, 101, 108, 108, 111, 32, 119, 111, 114, 108, 100, 0];
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = destination.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = source.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = destination.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.add(3) };
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: i8 = destination[({{__v[0-9]+}} as usize)];
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = secret.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = secret[({{__v[0-9]+}} as usize)];
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: i8 = secret[({{__v[0-9]+}} as usize)];
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"c23\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = (unsafe { strdup({{__v[0-9]+}} as *const core::ffi::c_char) }) as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"library\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 =
// LOWERING-X86_64-GNU-NEXT:         (unsafe { strndup({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}} as usize) }) as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = first_copy;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = first_copy;
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = b"c23\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = second_copy;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = second_copy;
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = b"lib\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = first_copy;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = second_copy;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 =
// LOWERING-X86_64-GNU-NEXT:         (unsafe { strchr({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}} as i32) }) as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = mutable_phrase.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 =
// LOWERING-X86_64-GNU-NEXT:         (unsafe { strchr({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}} as i32) }) as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = mut_hit;
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = mutable_phrase.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"world\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = (unsafe {
// LOWERING-X86_64-GNU-NEXT:     }) as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = mutable_phrase.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"world\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = (unsafe {
// LOWERING-X86_64-GNU-NEXT:     }) as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT:     let mut month: aligned::Aligned<aligned::A16, [i8; 32]> = aligned::Aligned([0; 32]);
// LOWERING-X86_64-GNU-NEXT:     let mut wide_month: aligned::Aligned<aligned::A16, [i32; 32]> = aligned::Aligned([0; 32]);
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i8; 32] = [0; 32];
// LOWERING-X86_64-GNU-NEXT:     *month = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i32; 32] = [0; 32];
// LOWERING-X86_64-GNU-NEXT:     *wide_month = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = month.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%OB\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = month.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"January\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i32 = wide_month.as_mut_ptr() as *mut i32;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i32 = std::ptr::addr_of_mut!(_str_9).cast::<i32>();
// LOWERING-X86_64-GNU-NEXT:             {{__v[0-9]+}} as *mut i32,
// LOWERING-X86_64-GNU-NEXT:             {{__v[0-9]+}} as *const i32,
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i32 = wide_month.as_mut_ptr() as *mut i32;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i32 = std::ptr::addr_of_mut!(_str_10).cast::<i32>();
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { wcscmp({{__v[0-9]+}} as *const i32, {{__v[0-9]+}} as *const i32) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i32 = wide_month.as_mut_ptr() as *mut i32;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = 110;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i32 = unsafe { wcschr({{__v[0-9]+}} as *const i32, {{__v[0-9]+}} as i32) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i32 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i32 = wide_month.as_mut_ptr() as *mut i32;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = 110;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i32 = unsafe { wcschr({{__v[0-9]+}} as *const i32, {{__v[0-9]+}} as i32) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i32 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i32 = std::ptr::addr_of_mut!(_str_11).cast::<i32>();
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i32 = unsafe { wcsstr({{__v[0-9]+}} as *const i32, {{__v[0-9]+}} as *const i32) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i32 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i32 = wide_month.as_mut_ptr() as *mut i32;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i32 = std::ptr::addr_of_mut!(_str_11).cast::<i32>();
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i32 = unsafe { wcsstr({{__v[0-9]+}} as *const i32, {{__v[0-9]+}} as *const i32) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i32 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT:     let mut output: aligned::Aligned<aligned::A16, [i8; 64]> = aligned::Aligned([0; 64]);
// LOWERING-X86_64-GNU-NEXT:     let mut float_output: aligned::Aligned<aligned::A16, [i8; 16]> = aligned::Aligned([0; 16]);
// LOWERING-X86_64-GNU-NEXT:     let mut double_output: aligned::Aligned<aligned::A16, [i8; 16]> = aligned::Aligned([0; 16]);
// LOWERING-X86_64-GNU-NEXT:     let mut long_double_output: aligned::Aligned<aligned::A16, [i8; 16]> =
// LOWERING-X86_64-GNU-NEXT:         aligned::Aligned([0; 16]);
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i8; 64] = [0; 64];
// LOWERING-X86_64-GNU-NEXT:     *output = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i8; 16] = [0; 16];
// LOWERING-X86_64-GNU-NEXT:     *float_output = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i8; 16] = [0; 16];
// LOWERING-X86_64-GNU-NEXT:     *double_output = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i8; 16] = [0; 16];
// LOWERING-X86_64-GNU-NEXT:     *long_double_output = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = output.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%b %w16u %wf16u\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"1011 55 89\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%b %w16u %wf16u\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = float_output.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%.1f\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = double_output.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%.1f\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = long_double_output.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%.1f\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 224, 0, 64]);
// LOWERING-X86_64-GNU-NEXT:         __slate_strfroml__ri32_pc_usize_pc_f80(
// LOWERING-X86_64-GNU-NEXT:             {{__v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = output.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"1101 21 34\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = float_output.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"1.5\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = double_output.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"2.5\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = long_double_output.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"3.5\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([1, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([255, 255, 255, 255, 255, 255, 255, 255, 254, 127]);
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([255, 255, 255, 255, 255, 255, 255, 255, 254, 127]);
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: unsafe extern "C" {
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_cf80_div(
// LOWERING-X86_64-GNU-NEXT:         __a: num_complex::Complex<LongDouble>,
// LOWERING-X86_64-GNU-NEXT:         __b: num_complex::Complex<LongDouble>,
// LOWERING-X86_64-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_cf80_mul(
// LOWERING-X86_64-GNU-NEXT:         __a: num_complex::Complex<LongDouble>,
// LOWERING-X86_64-GNU-NEXT:         __b: num_complex::Complex<LongDouble>,
// LOWERING-X86_64-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f128_nexttoward(__from: f128, __toward: f128) -> f128;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_abs(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_acos(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_acosh(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_add(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_asin(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_asinh(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_atan(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_atanh(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_cbrt(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_ceil(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_copysign(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_cos(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_cosh(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_div(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_eq(__a: LongDouble, __b: LongDouble) -> bool;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_exp(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_exp2(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_expm1(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_fdim(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_floor(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_fma(__a: LongDouble, __b: LongDouble, __c: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_fmax(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_fmin(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_fmod(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_fract(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_bool(__a: bool) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_f32(__a: f32) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_f64(__a: f64) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_i128(__a: i128) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_i16(__a: i16) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_i32(__a: i32) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_i64(__a: i64) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_i8(__a: i8) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_u128(__a: u128) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_u16(__a: u16) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_u32(__a: u32) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_u64(__a: u64) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_u8(__a: u8) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_ge(__a: LongDouble, __b: LongDouble) -> bool;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_gt(__a: LongDouble, __b: LongDouble) -> bool;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_hypot(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_is_fp_class(__a: LongDouble, __flags: i32) -> bool;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_le(__a: LongDouble, __b: LongDouble) -> bool;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_log(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_log10(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_log1p(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_log2(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_lt(__a: LongDouble, __b: LongDouble) -> bool;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_mul(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_ne(__a: LongDouble, __b: LongDouble) -> bool;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_nearbyint(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_neg(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_pow(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_powi(__a: LongDouble, __n: i32) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_remainder(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_rint(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_round(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_signbit(__a: LongDouble) -> bool;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_sin(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_sinh(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_sqrt(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_sub(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_tan(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_tanh(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_bool(__a: LongDouble) -> bool;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_f32(__a: LongDouble) -> f32;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_f64(__a: LongDouble) -> f64;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_i128(__a: LongDouble) -> i128;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_i16(__a: LongDouble) -> i16;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_i32(__a: LongDouble) -> i32;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_i64(__a: LongDouble) -> i64;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_i8(__a: LongDouble) -> i8;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_u128(__a: LongDouble) -> u128;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_u16(__a: LongDouble) -> u16;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_u32(__a: LongDouble) -> u32;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_u64(__a: LongDouble) -> u64;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_u8(__a: LongDouble) -> u8;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_trunc(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn __slate_strfroml__ri32_pc_usize_pc_f80(
// LOWERING-X86_64-GNU-NEXT:         _0: *mut core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:         _1: usize,
// LOWERING-X86_64-GNU-NEXT:         _2: *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:         _3: LongDouble,
// LOWERING-X86_64-GNU-NEXT:     ) -> i32;
// LOWERING-X86_64-GNU-NEXT: }
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT:     __wchb: [u8; 4],
// LOWERING-AARCH64-GNU-NEXT:     __tm_zone: *mut u8,
// LOWERING-AARCH64-GNU-NEXT: static mut _str_10: [u32; 8] = [74, 97, 110, 117, 97, 114, 121, 0];
// LOWERING-AARCH64-GNU-NEXT: static mut _str_11: [u32; 4] = [74, 97, 110, 0];
// LOWERING-AARCH64-GNU-NEXT: static mut _str_9: [u32; 4] = [37, 79, 66, 0];
// LOWERING-AARCH64-GNU-NEXT:     fn wcsftime(_0: *mut u32, _1: usize, _2: *const u32, _3: *const tm) -> usize;
// LOWERING-AARCH64-GNU-NEXT:     fn wcscmp(_0: *const u32, _1: *const u32) -> i32;
// LOWERING-AARCH64-GNU-NEXT:     fn wcschr(_0: *const u32, _1: u32) -> *mut u32;
// LOWERING-AARCH64-GNU-NEXT:     fn wcsstr(_0: *const u32, _1: *const u32) -> *mut u32;
// LOWERING-AARCH64-GNU-NEXT:         _3: f128,
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %d %d %d %d %d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let mut output: [u8; 4] = [0; 4];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: [u8; 4] = [0; 4];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"A\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = output.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = output[({{__v[0-9]+}} as usize)];
// LOWERING-AARCH64-GNU-NEXT:     let mut source: [u8; 7] = [0; 7];
// LOWERING-AARCH64-GNU-NEXT:     let mut destination: [u8; 8] = [0; 8];
// LOWERING-AARCH64-GNU-NEXT:     let mut secret: [u8; 7] = [0; 7];
// LOWERING-AARCH64-GNU-NEXT:     let mut first_copy: *mut u8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:     let mut second_copy: *mut u8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:     let mut mutable_phrase: [u8; 12] = [0; 12];
// LOWERING-AARCH64-GNU-NEXT:     let mut mut_hit: *mut u8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: [u8; 7] = [97, 98, 99, 100, 101, 102, 0];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: [u8; 8] = [0; 8];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: [u8; 7] = [115, 101, 99, 114, 101, 116, 0];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"hello world\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: [u8; 12] = [104, 101, 108, 108, 111, 32, 119, 111, 114, 108, 100, 0];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = destination.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = source.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = destination.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(3) };
// LOWERING-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: u8 = destination[({{__v[0-9]+}} as usize)];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = secret.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = secret[({{__v[0-9]+}} as usize)];
// LOWERING-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: u8 = secret[({{__v[0-9]+}} as usize)];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"c23\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = (unsafe { strdup({{__v[0-9]+}} as *const core::ffi::c_char) }) as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"library\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 =
// LOWERING-AARCH64-GNU-NEXT:         (unsafe { strndup({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}} as usize) }) as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = first_copy;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = first_copy;
// LOWERING-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = b"c23\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = second_copy;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = second_copy;
// LOWERING-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = b"lib\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = first_copy;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = second_copy;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 =
// LOWERING-AARCH64-GNU-NEXT:         (unsafe { strchr({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}} as i32) }) as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = mutable_phrase.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 =
// LOWERING-AARCH64-GNU-NEXT:         (unsafe { strchr({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}} as i32) }) as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = mut_hit;
// LOWERING-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = mutable_phrase.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"world\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = (unsafe {
// LOWERING-AARCH64-GNU-NEXT:     }) as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = mutable_phrase.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"world\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = (unsafe {
// LOWERING-AARCH64-GNU-NEXT:     }) as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:     let mut month: [u8; 32] = [0; 32];
// LOWERING-AARCH64-GNU-NEXT:     let mut wide_month: [u32; 32] = [0; 32];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: [u8; 32] = [0; 32];
// LOWERING-AARCH64-GNU-NEXT:     month = {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: [u32; 32] = [0; 32];
// LOWERING-AARCH64-GNU-NEXT:     wide_month = {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = month.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%OB\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = month.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"January\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u32 = wide_month.as_mut_ptr() as *mut u32;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u32 = std::ptr::addr_of_mut!(_str_9).cast::<u32>();
// LOWERING-AARCH64-GNU-NEXT:             {{__v[0-9]+}} as *mut u32,
// LOWERING-AARCH64-GNU-NEXT:             {{__v[0-9]+}} as *const u32,
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u32 = wide_month.as_mut_ptr() as *mut u32;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u32 = std::ptr::addr_of_mut!(_str_10).cast::<u32>();
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { wcscmp({{__v[0-9]+}} as *const u32, {{__v[0-9]+}} as *const u32) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u32 = wide_month.as_mut_ptr() as *mut u32;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u32 = 110;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u32 = unsafe { wcschr({{__v[0-9]+}} as *const u32, {{__v[0-9]+}} as u32) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u32 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u32 = wide_month.as_mut_ptr() as *mut u32;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u32 = 110;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u32 = unsafe { wcschr({{__v[0-9]+}} as *const u32, {{__v[0-9]+}} as u32) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u32 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u32 = std::ptr::addr_of_mut!(_str_11).cast::<u32>();
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u32 = unsafe { wcsstr({{__v[0-9]+}} as *const u32, {{__v[0-9]+}} as *const u32) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u32 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u32 = wide_month.as_mut_ptr() as *mut u32;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u32 = std::ptr::addr_of_mut!(_str_11).cast::<u32>();
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u32 = unsafe { wcsstr({{__v[0-9]+}} as *const u32, {{__v[0-9]+}} as *const u32) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u32 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:     let mut output: [u8; 64] = [0; 64];
// LOWERING-AARCH64-GNU-NEXT:     let mut float_output: [u8; 16] = [0; 16];
// LOWERING-AARCH64-GNU-NEXT:     let mut double_output: [u8; 16] = [0; 16];
// LOWERING-AARCH64-GNU-NEXT:     let mut long_double_output: [u8; 16] = [0; 16];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: [u8; 64] = [0; 64];
// LOWERING-AARCH64-GNU-NEXT:     output = {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: [u8; 16] = [0; 16];
// LOWERING-AARCH64-GNU-NEXT:     float_output = {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: [u8; 16] = [0; 16];
// LOWERING-AARCH64-GNU-NEXT:     double_output = {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: [u8; 16] = [0; 16];
// LOWERING-AARCH64-GNU-NEXT:     long_double_output = {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = output.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%b %w16u %wf16u\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"1011 55 89\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%b %w16u %wf16u\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = float_output.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%.1f\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = double_output.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%.1f\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = long_double_output.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%.1f\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 3.500000e+00f128;
// LOWERING-AARCH64-GNU-NEXT:         strfroml(
// LOWERING-AARCH64-GNU-NEXT:             {{__v[0-9]+}} as f128,
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = output.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"1101 21 34\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = float_output.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"1.5\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = double_output.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"2.5\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = long_double_output.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"3.5\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 6.475180e-4966f128;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 1.18973149535723176508575932662800702E+4932f128;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 1.18973149535723176508575932662800702E+4932f128;
// SLATE-FILECHECK-END lowering-aarch64-gnu

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES: #![feature(f128)]
// COMMON-REWRITES-NEXT: #![feature(c_variadic)]
// COMMON-REWRITES-NEXT: #![allow(
// COMMON-REWRITES-NEXT:     dead_code,
// COMMON-REWRITES-NEXT:     unused,
// COMMON-REWRITES-NEXT:     non_camel_case_types,
// COMMON-REWRITES-NEXT:     non_snake_case,
// COMMON-REWRITES-NEXT:     non_upper_case_globals,
// COMMON-REWRITES-NEXT:     arithmetic_overflow,
// COMMON-REWRITES-NEXT:     unconditional_panic,
// COMMON-REWRITES-NEXT:     suspicious_runtime_symbol_definitions,
// COMMON-REWRITES-NEXT:     unpredictable_function_pointer_comparisons,
// COMMON-REWRITES-NEXT:     unused_comparisons
// COMMON-REWRITES-NEXT: )]
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT:         } else {
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[allow(non_camel_case_types)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy, PartialEq, Eq, Debug, Hash)]
// COMMON-REWRITES-NEXT: enum memory_order {
// COMMON-REWRITES-NEXT:     memory_order_relaxed = 0,
// COMMON-REWRITES-NEXT:     memory_order_consume = 1,
// COMMON-REWRITES-NEXT:     memory_order_acquire = 2,
// COMMON-REWRITES-NEXT:     memory_order_release = 3,
// COMMON-REWRITES-NEXT:     memory_order_acq_rel = 4,
// COMMON-REWRITES-NEXT:     memory_order_seq_cst = 5,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct __mbstate_t {
// COMMON-REWRITES-NEXT:     __count: i32,
// COMMON-REWRITES-NEXT:     __value: {{anon_[0-9]+}},
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: union {{anon_[0-9]+}} {
// COMMON-REWRITES-NEXT:     __wch: i32,
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: struct tm {
// COMMON-REWRITES-NEXT:     tm_sec: i32,
// COMMON-REWRITES-NEXT:     tm_min: i32,
// COMMON-REWRITES-NEXT:     tm_hour: i32,
// COMMON-REWRITES-NEXT:     tm_mday: i32,
// COMMON-REWRITES-NEXT:     tm_mon: i32,
// COMMON-REWRITES-NEXT:     tm_year: i32,
// COMMON-REWRITES-NEXT:     tm_wday: i32,
// COMMON-REWRITES-NEXT:     tm_yday: i32,
// COMMON-REWRITES-NEXT:     tm_isdst: i32,
// COMMON-REWRITES-NEXT:     __tm_gmtoff: i64,
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT:     fn mbrtoc8(_0: *mut u8, _1: *const core::ffi::c_char, _2: usize, _3: *mut __mbstate_t)
// COMMON-REWRITES-NEXT:     -> usize;
// COMMON-REWRITES-NEXT:     fn c8rtomb(_0: *mut core::ffi::c_char, _1: u8, _2: *mut __mbstate_t) -> usize;
// COMMON-REWRITES-NEXT:     fn memccpy(
// COMMON-REWRITES-NEXT:         _0: *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:         _1: *const core::ffi::c_void,
// COMMON-REWRITES-NEXT:         _2: i32,
// COMMON-REWRITES-NEXT:         _3: usize,
// COMMON-REWRITES-NEXT:     ) -> *mut core::ffi::c_void;
// COMMON-REWRITES-NEXT:     fn memset_explicit(_0: *mut core::ffi::c_void, _1: i32, _2: usize) -> *mut core::ffi::c_void;
// COMMON-REWRITES-NEXT:     fn strdup(_0: *const core::ffi::c_char) -> *mut core::ffi::c_char;
// COMMON-REWRITES-NEXT:     fn strndup(_0: *const core::ffi::c_char, _1: usize) -> *mut core::ffi::c_char;
// COMMON-REWRITES-NEXT:     fn strcmp(_0: *const core::ffi::c_char, _1: *const core::ffi::c_char) -> i32;
// COMMON-REWRITES-NEXT:     fn free(_0: *mut core::ffi::c_void);
// COMMON-REWRITES-NEXT:     fn strchr(_0: *const core::ffi::c_char, _1: i32) -> *mut core::ffi::c_char;
// COMMON-REWRITES-NEXT:     fn memchr(_0: *const core::ffi::c_void, _1: i32, _2: usize) -> *mut core::ffi::c_void;
// COMMON-REWRITES-NEXT:     fn strstr(_0: *const core::ffi::c_char, _1: *const core::ffi::c_char)
// COMMON-REWRITES-NEXT:     -> *mut core::ffi::c_char;
// COMMON-REWRITES-NEXT:     fn gmtime_r(_0: *const i64, _1: *mut tm) -> *mut tm;
// COMMON-REWRITES-NEXT:     fn localtime_r(_0: *const i64, _1: *mut tm) -> *mut tm;
// COMMON-REWRITES-NEXT:     fn timespec_getres(_0: *mut libc::timespec, _1: i32) -> i32;
// COMMON-REWRITES-NEXT:     fn timegm(_0: *mut tm) -> i64;
// COMMON-REWRITES-NEXT:     fn strftime(
// COMMON-REWRITES-NEXT:         _0: *mut core::ffi::c_char,
// COMMON-REWRITES-NEXT:         _1: usize,
// COMMON-REWRITES-NEXT:         _2: *const core::ffi::c_char,
// COMMON-REWRITES-NEXT:         _3: *const tm,
// COMMON-REWRITES-NEXT:     ) -> usize;
// COMMON-REWRITES-NEXT:     fn snprintf(_0: *mut core::ffi::c_char, _1: usize, _2: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT:     fn sscanf(_0: *const core::ffi::c_char, _1: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT:     fn strfromf(
// COMMON-REWRITES-NEXT:         _0: *mut core::ffi::c_char,
// COMMON-REWRITES-NEXT:         _1: usize,
// COMMON-REWRITES-NEXT:         _2: *const core::ffi::c_char,
// COMMON-REWRITES-NEXT:         _3: f32,
// COMMON-REWRITES-NEXT:     ) -> i32;
// COMMON-REWRITES-NEXT:     fn strfromd(
// COMMON-REWRITES-NEXT:         _0: *mut core::ffi::c_char,
// COMMON-REWRITES-NEXT:         _1: usize,
// COMMON-REWRITES-NEXT:         _2: *const core::ffi::c_char,
// COMMON-REWRITES-NEXT:         _3: f64,
// COMMON-REWRITES-NEXT:     ) -> i32;
// COMMON-REWRITES-NEXT:     fn strfroml(
// COMMON-REWRITES-NEXT:         _0: *mut core::ffi::c_char,
// COMMON-REWRITES-NEXT:         _1: usize,
// COMMON-REWRITES-NEXT:         _2: *const core::ffi::c_char,
// COMMON-REWRITES-NEXT:     ) -> i32;
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = c23_stdbit();
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = c23_checked_arithmetic();
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = c23_utf8();
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = c23_memory();
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = c23_time();
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = c23_io();
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = c23_limits();
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         printf(
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}} + {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: fn c23_stdbit() -> i32 {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u32 = 176;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}}.leading_zeros() as u32;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u32 = !{{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}} + ({{__v[0-9]+}}.leading_zeros() as u32) + ({{__v[0-9]+}}.trailing_zeros() as u32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u32 = !{{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}} + ({{__v[0-9]+}}.trailing_zeros() as u32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u32 = !{{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u32 = 0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u32 = 0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u32 =
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}} + if {{__v[0-9]+}} == {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:         } else {
// COMMON-REWRITES-NEXT:             ({{__v[0-9]+}}.leading_zeros() as u32) + 1
// COMMON-REWRITES-NEXT:         } + if {{__v[0-9]+}} == {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:         } else {
// COMMON-REWRITES-NEXT:             ({{__v[0-9]+}}.leading_zeros() as u32) + 1
// COMMON-REWRITES-NEXT:         };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u32 = !{{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u32 = 0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u32 = 0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u32 = 1;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u32 = 0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u32 = 1;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}} - {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     ({{__v[0-9]+}}
// COMMON-REWRITES-NEXT:         + if {{__v[0-9]+}} == {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:         } else {
// COMMON-REWRITES-NEXT:             ({{__v[0-9]+}}.trailing_zeros() as u32) + 1
// COMMON-REWRITES-NEXT:         + if {{__v[0-9]+}} == {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:         } else {
// COMMON-REWRITES-NEXT:             ({{__v[0-9]+}}.trailing_zeros() as u32) + 1
// COMMON-REWRITES-NEXT:         + (32 - ({{__v[0-9]+}}.count_ones() as u32))
// COMMON-REWRITES-NEXT:         + ({{__v[0-9]+}}.count_ones() as u32)
// COMMON-REWRITES-NEXT:         + (true as u32)
// COMMON-REWRITES-NEXT:         + (32 - ({{__v[0-9]+}}.leading_zeros() as u32))
// COMMON-REWRITES-NEXT:         + if {{__v[0-9]+}} == {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:         } else {
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}} << 31 - ({{__v[0-9]+}}.leading_zeros() as u32)
// COMMON-REWRITES-NEXT:         + if {{__v[0-9]+}} <= {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}} << {{__v[0-9]+}} << 31 - ({{__v[0-9]+}}.leading_zeros() as u32)
// COMMON-REWRITES-NEXT:         }) as i32
// COMMON-REWRITES-NEXT: fn c23_checked_arithmetic() -> i32 {
// COMMON-REWRITES-NEXT:     let mut result: i32 = 0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 20;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}} = {{__v[0-9]+}}.overflowing_add(22 as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.1 || ({{__v[0-9]+}}.0 < -2147483648 || {{__v[0-9]+}}.0 > 2147483647);
// COMMON-REWRITES-NEXT:     result = {{__v[0-9]+}}.0 as i32;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = result == 42;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + ({{__v[0-9]+}} as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 50;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}} = {{__v[0-9]+}}.overflowing_sub(8 as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.1 || ({{__v[0-9]+}}.0 < -2147483648 || {{__v[0-9]+}}.0 > 2147483647);
// COMMON-REWRITES-NEXT:     result = {{__v[0-9]+}}.0 as i32;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = result == 42;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + ({{__v[0-9]+}} as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 6;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}} = {{__v[0-9]+}}.overflowing_mul(7 as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.1 || ({{__v[0-9]+}}.0 < -2147483648 || {{__v[0-9]+}}.0 > 2147483647);
// COMMON-REWRITES-NEXT:     result = {{__v[0-9]+}}.0 as i32;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = result == 42;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + ({{__v[0-9]+}} as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 2147483647;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}} = {{__v[0-9]+}}.overflowing_add(1 as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.1 || ({{__v[0-9]+}}.0 < -2147483648 || {{__v[0-9]+}}.0 > 2147483647);
// COMMON-REWRITES-NEXT:     result = {{__v[0-9]+}}.0 as i32;
// COMMON-REWRITES-NEXT:     {{__v[0-9]+}} + ({{__v[0-9]+}} as i32)
// COMMON-REWRITES-NEXT: fn c23_utf8() -> i32 {
// COMMON-REWRITES-NEXT:     let mut input_state: __mbstate_t = __mbstate_t {
// COMMON-REWRITES-NEXT:         __count: 0,
// COMMON-REWRITES-NEXT:         __value: unsafe { std::mem::zeroed::<{{anon_[0-9]+}}>() },
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let mut output_state: __mbstate_t = __mbstate_t {
// COMMON-REWRITES-NEXT:         __count: 0,
// COMMON-REWRITES-NEXT:         __value: unsafe { std::mem::zeroed::<{{anon_[0-9]+}}>() },
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let mut character: u8 = 0;
// COMMON-REWRITES-NEXT:     let mut atomic_character: u8 = 0;
// COMMON-REWRITES-NEXT:     input_state = __mbstate_t {
// COMMON-REWRITES-NEXT:         __count: 0,
// COMMON-REWRITES-NEXT:         __value: unsafe { std::mem::zeroed::<{{anon_[0-9]+}}>() },
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     output_state = __mbstate_t {
// COMMON-REWRITES-NEXT:         __count: 0,
// COMMON-REWRITES-NEXT:         __value: unsafe { std::mem::zeroed::<{{anon_[0-9]+}}>() },
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     output = [0; 4];
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = (unsafe {
// COMMON-REWRITES-NEXT:         mbrtoc8(
// COMMON-REWRITES-NEXT:             std::ptr::addr_of_mut!(character) as *mut u8,
// COMMON-REWRITES-NEXT:             c"A".as_ptr(),
// COMMON-REWRITES-NEXT:             (1 as u64) as usize,
// COMMON-REWRITES-NEXT:             std::ptr::addr_of_mut!(input_state) as *mut __mbstate_t,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     }) as u64;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = (unsafe {
// COMMON-REWRITES-NEXT:         c8rtomb(
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}} as *mut core::ffi::c_char,
// COMMON-REWRITES-NEXT:             character as u8,
// COMMON-REWRITES-NEXT:             std::ptr::addr_of_mut!(output_state) as *mut __mbstate_t,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     }) as u64;
// COMMON-REWRITES-NEXT:     atomic_character = character;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u8 = 66;
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         std::sync::atomic::AtomicU8::from_ptr(std::ptr::addr_of_mut!(atomic_character))
// COMMON-REWRITES-NEXT:             .store({{__v[0-9]+}}, std::sync::atomic::Ordering::SeqCst)
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 =
// COMMON-REWRITES-NEXT:         (({{__v[0-9]+}} == 1) as i32) + (({{__v[0-9]+}} == 1) as i32) + (((output[0] as i32) == 65) as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u8 = unsafe {
// COMMON-REWRITES-NEXT:         std::sync::atomic::AtomicU8::from_ptr(std::ptr::addr_of_mut!(atomic_character))
// COMMON-REWRITES-NEXT:             .load(std::sync::atomic::Ordering::SeqCst)
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:     {{__v[0-9]+}} + ((({{__v[0-9]+}} as i32) == 66) as i32) + ((2 > {{__v[0-9]+}}) as i32)
// COMMON-REWRITES-NEXT: fn c23_memory() -> i32 {
// COMMON-REWRITES-NEXT:     source = [97, 98, 99, 100, 101, 102, 0];
// COMMON-REWRITES-NEXT:     destination = [0; 8];
// COMMON-REWRITES-NEXT:     secret = [115, 101, 99, 114, 101, 116, 0];
// COMMON-REWRITES-NEXT:     mutable_phrase = [104, 101, 108, 108, 111, 32, 119, 111, 114, 108, 100, 0];
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe {
// COMMON-REWRITES-NEXT:         memccpy(
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}} as *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_void,
// COMMON-REWRITES-NEXT:             99 as i32,
// COMMON-REWRITES-NEXT:             (6 as u64) as usize,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} == ({{__v[0-9]+}} as *mut core::ffi::c_void) {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = (destination[2] as i32) == 99;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         memset_explicit(
// COMMON-REWRITES-NEXT:             secret.as_mut_ptr() as *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:             0 as i32,
// COMMON-REWRITES-NEXT:             (7 as u64) as usize,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if (secret[0] as i32) == 0 {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = (secret[5] as i32) == 0;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + ({{__v[0-9]+}} as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = first_copy != std::ptr::null_mut();
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = unsafe { strcmp(first_copy as *const core::ffi::c_char, c"c23".as_ptr()) };
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == 0;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + ({{__v[0-9]+}} as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = second_copy != std::ptr::null_mut();
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 =
// COMMON-REWRITES-NEXT:             unsafe { strcmp(second_copy as *const core::ffi::c_char, c"lib".as_ptr()) };
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == 0;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + ({{__v[0-9]+}} as i32);
// COMMON-REWRITES-NEXT:     unsafe { free(first_copy as *mut core::ffi::c_void) };
// COMMON-REWRITES-NEXT:     unsafe { free(second_copy as *mut core::ffi::c_void) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} != std::ptr::null_mut() {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = mut_hit != std::ptr::null_mut();
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + ({{__v[0-9]+}} as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe {
// COMMON-REWRITES-NEXT:         memchr(
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_void,
// COMMON-REWRITES-NEXT:             111 as i32,
// COMMON-REWRITES-NEXT:             (11 as u64) as usize,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} != std::ptr::null_mut()) as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe {
// COMMON-REWRITES-NEXT:         memchr(
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_void,
// COMMON-REWRITES-NEXT:             111 as i32,
// COMMON-REWRITES-NEXT:             (11 as u64) as usize,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} != std::ptr::null_mut()) as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} != std::ptr::null_mut()) as i32);
// COMMON-REWRITES-NEXT:     {{__v[0-9]+}} + (({{__v[0-9]+}} != std::ptr::null_mut()) as i32)
// COMMON-REWRITES-NEXT: fn c23_time() -> i32 {
// COMMON-REWRITES-NEXT:     let mut timestamp: i64 = 0;
// COMMON-REWRITES-NEXT:     let mut utc: tm = tm {
// COMMON-REWRITES-NEXT:         tm_sec: 0,
// COMMON-REWRITES-NEXT:         tm_min: 0,
// COMMON-REWRITES-NEXT:         tm_hour: 0,
// COMMON-REWRITES-NEXT:         tm_mday: 0,
// COMMON-REWRITES-NEXT:         tm_mon: 0,
// COMMON-REWRITES-NEXT:         tm_year: 0,
// COMMON-REWRITES-NEXT:         tm_wday: 0,
// COMMON-REWRITES-NEXT:         tm_yday: 0,
// COMMON-REWRITES-NEXT:         tm_isdst: 0,
// COMMON-REWRITES-NEXT:         __tm_gmtoff: 0,
// COMMON-REWRITES-NEXT:         __tm_zone: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let mut local: tm = tm {
// COMMON-REWRITES-NEXT:         tm_sec: 0,
// COMMON-REWRITES-NEXT:         tm_min: 0,
// COMMON-REWRITES-NEXT:         tm_hour: 0,
// COMMON-REWRITES-NEXT:         tm_mday: 0,
// COMMON-REWRITES-NEXT:         tm_mon: 0,
// COMMON-REWRITES-NEXT:         tm_year: 0,
// COMMON-REWRITES-NEXT:         tm_wday: 0,
// COMMON-REWRITES-NEXT:         tm_yday: 0,
// COMMON-REWRITES-NEXT:         tm_isdst: 0,
// COMMON-REWRITES-NEXT:         __tm_gmtoff: 0,
// COMMON-REWRITES-NEXT:         __tm_zone: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let mut resolution: libc::timespec = libc::timespec {
// COMMON-REWRITES-NEXT:         tv_sec: 0,
// COMMON-REWRITES-NEXT:         tv_nsec: 0,
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     timestamp = 0;
// COMMON-REWRITES-NEXT:     utc = tm {
// COMMON-REWRITES-NEXT:         tm_sec: 0,
// COMMON-REWRITES-NEXT:         tm_min: 0,
// COMMON-REWRITES-NEXT:         tm_hour: 0,
// COMMON-REWRITES-NEXT:         tm_mday: 0,
// COMMON-REWRITES-NEXT:         tm_mon: 0,
// COMMON-REWRITES-NEXT:         tm_year: 0,
// COMMON-REWRITES-NEXT:         tm_wday: 0,
// COMMON-REWRITES-NEXT:         tm_yday: 0,
// COMMON-REWRITES-NEXT:         tm_isdst: 0,
// COMMON-REWRITES-NEXT:         __tm_gmtoff: 0,
// COMMON-REWRITES-NEXT:         __tm_zone: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     local = tm {
// COMMON-REWRITES-NEXT:         tm_sec: 0,
// COMMON-REWRITES-NEXT:         tm_min: 0,
// COMMON-REWRITES-NEXT:         tm_hour: 0,
// COMMON-REWRITES-NEXT:         tm_mday: 0,
// COMMON-REWRITES-NEXT:         tm_mon: 0,
// COMMON-REWRITES-NEXT:         tm_year: 0,
// COMMON-REWRITES-NEXT:         tm_wday: 0,
// COMMON-REWRITES-NEXT:         tm_yday: 0,
// COMMON-REWRITES-NEXT:         tm_isdst: 0,
// COMMON-REWRITES-NEXT:         __tm_gmtoff: 0,
// COMMON-REWRITES-NEXT:         __tm_zone: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     resolution = libc::timespec {
// COMMON-REWRITES-NEXT:         tv_sec: 0,
// COMMON-REWRITES-NEXT:         tv_nsec: 0,
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut tm = unsafe {
// COMMON-REWRITES-NEXT:         gmtime_r(
// COMMON-REWRITES-NEXT:             std::ptr::addr_of_mut!(timestamp) as *const i64,
// COMMON-REWRITES-NEXT:             std::ptr::addr_of_mut!(utc) as *mut tm,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == std::ptr::addr_of_mut!(utc);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut tm = unsafe {
// COMMON-REWRITES-NEXT:         localtime_r(
// COMMON-REWRITES-NEXT:             std::ptr::addr_of_mut!(timestamp) as *const i64,
// COMMON-REWRITES-NEXT:             std::ptr::addr_of_mut!(local) as *mut tm,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == std::ptr::addr_of_mut!(local);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + ({{__v[0-9]+}} as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-REWRITES-NEXT:         timespec_getres(
// COMMON-REWRITES-NEXT:             std::ptr::addr_of_mut!(resolution) as *mut libc::timespec,
// COMMON-REWRITES-NEXT:             1 as i32,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} == 1) as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if resolution.tv_sec > 0 {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = true;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = resolution.tv_nsec > 0;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + ({{__v[0-9]+}} as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { timegm(std::ptr::addr_of_mut!(utc) as *mut tm) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} == 0) as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = (unsafe {
// COMMON-REWRITES-NEXT:         strftime(
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}} as *mut core::ffi::c_char,
// COMMON-REWRITES-NEXT:             (32 as u64) as usize,
// COMMON-REWRITES-NEXT:             c"%OB".as_ptr(),
// COMMON-REWRITES-NEXT:             std::ptr::addr_of_mut!(utc) as *const tm,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     }) as u64;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} == 7) as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { strcmp({{__v[0-9]+}} as *const core::ffi::c_char, c"January".as_ptr()) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} == 0) as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = (unsafe {
// COMMON-REWRITES-NEXT:         wcsftime(
// COMMON-REWRITES-NEXT:             (32 as u64) as usize,
// COMMON-REWRITES-NEXT:             std::ptr::addr_of_mut!(utc) as *const tm,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     }) as u64;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} == 7) as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} == 0) as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} != std::ptr::null_mut()) as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} != std::ptr::null_mut()) as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} != std::ptr::null_mut()) as i32);
// COMMON-REWRITES-NEXT:     {{__v[0-9]+}} + (({{__v[0-9]+}} != std::ptr::null_mut()) as i32)
// COMMON-REWRITES-NEXT: fn c23_io() -> i32 {
// COMMON-REWRITES-NEXT:     let mut binary_value: u32 = 0;
// COMMON-REWRITES-NEXT:     let mut exact_value: u16 = 0;
// COMMON-REWRITES-NEXT:     let mut fast_value: u64 = 0;
// COMMON-REWRITES-NEXT:     binary_value = 0;
// COMMON-REWRITES-NEXT:     exact_value = 0;
// COMMON-REWRITES-NEXT:     fast_value = 0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-REWRITES-NEXT:         snprintf(
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}} as *mut core::ffi::c_char,
// COMMON-REWRITES-NEXT:             (64 as u64) as usize,
// COMMON-REWRITES-NEXT:             c"%b %w16u %wf16u".as_ptr(),
// COMMON-REWRITES-NEXT:             13 as u32,
// COMMON-REWRITES-NEXT:             21 as i32,
// COMMON-REWRITES-NEXT:             34 as u64,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-REWRITES-NEXT:         sscanf(
// COMMON-REWRITES-NEXT:             c"1011 55 89".as_ptr(),
// COMMON-REWRITES-NEXT:             c"%b %w16u %wf16u".as_ptr(),
// COMMON-REWRITES-NEXT:             std::ptr::addr_of_mut!(binary_value),
// COMMON-REWRITES-NEXT:             std::ptr::addr_of_mut!(exact_value),
// COMMON-REWRITES-NEXT:             std::ptr::addr_of_mut!(fast_value),
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-REWRITES-NEXT:         strfromf(
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}} as *mut core::ffi::c_char,
// COMMON-REWRITES-NEXT:             (16 as u64) as usize,
// COMMON-REWRITES-NEXT:             c"%.1f".as_ptr(),
// COMMON-REWRITES-NEXT:             1.5 as f32,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-REWRITES-NEXT:         strfromd(
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}} as *mut core::ffi::c_char,
// COMMON-REWRITES-NEXT:             (16 as u64) as usize,
// COMMON-REWRITES-NEXT:             c"%.1f".as_ptr(),
// COMMON-REWRITES-NEXT:             2.5 as f64,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}} as *mut core::ffi::c_char,
// COMMON-REWRITES-NEXT:             (16 as u64) as usize,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { strcmp({{__v[0-9]+}} as *const core::ffi::c_char, c"1101 21 34".as_ptr()) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:         + (({{__v[0-9]+}} == 0) as i32)
// COMMON-REWRITES-NEXT:         + {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:         + ((binary_value == 11) as i32)
// COMMON-REWRITES-NEXT:         + (((exact_value as i32) == 55) as i32)
// COMMON-REWRITES-NEXT:         + ((fast_value == 89) as i32)
// COMMON-REWRITES-NEXT:         + {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { strcmp({{__v[0-9]+}} as *const core::ffi::c_char, c"1.5".as_ptr()) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} == 0) as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { strcmp({{__v[0-9]+}} as *const core::ffi::c_char, c"2.5".as_ptr()) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} == 0) as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { strcmp({{__v[0-9]+}} as *const core::ffi::c_char, c"3.5".as_ptr()) };
// COMMON-REWRITES-NEXT:     {{__v[0-9]+}} + (({{__v[0-9]+}} == 0) as i32)
// COMMON-REWRITES-NEXT: fn c23_limits() -> i32 {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 8;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f32 = 0.0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = 0.0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f32 = 340282346999999984391321947108527833088.0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = 179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368.0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = -1;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = -1;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = -1;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = 4;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = 8;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = 16;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = 202311;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = 202311;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = 202311;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = 202311;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = 202311;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = 202311;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = 202311;
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-NEXT: #[repr(C, align(16))]
// REWRITES-X86_64-GNU-NEXT: struct LongDouble([u8; 10]);
// REWRITES-X86_64-GNU-NEXT: impl core::ops::Add for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     type Output = LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn add(self, __o: LongDouble) -> LongDouble {
// REWRITES-X86_64-GNU-NEXT:         __slate_f80_add(self, __o)
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT: impl core::ops::Sub for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     type Output = LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn sub(self, __o: LongDouble) -> LongDouble {
// REWRITES-X86_64-GNU-NEXT:         __slate_f80_sub(self, __o)
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT: impl core::ops::Mul for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     type Output = LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn mul(self, __o: LongDouble) -> LongDouble {
// REWRITES-X86_64-GNU-NEXT:         __slate_f80_mul(self, __o)
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT: impl core::ops::Div for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     type Output = LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn div(self, __o: LongDouble) -> LongDouble {
// REWRITES-X86_64-GNU-NEXT:         __slate_f80_div(self, __o)
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT: impl core::ops::AddAssign for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     fn add_assign(&mut self, __o: LongDouble) {
// REWRITES-X86_64-GNU-NEXT:         {
// REWRITES-X86_64-GNU-NEXT:             *self = __slate_f80_add(*self, __o);
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT: impl core::ops::SubAssign for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     fn sub_assign(&mut self, __o: LongDouble) {
// REWRITES-X86_64-GNU-NEXT:         {
// REWRITES-X86_64-GNU-NEXT:             *self = __slate_f80_sub(*self, __o);
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT: impl core::ops::MulAssign for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     fn mul_assign(&mut self, __o: LongDouble) {
// REWRITES-X86_64-GNU-NEXT:         {
// REWRITES-X86_64-GNU-NEXT:             *self = __slate_f80_mul(*self, __o);
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT: impl core::ops::DivAssign for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     fn div_assign(&mut self, __o: LongDouble) {
// REWRITES-X86_64-GNU-NEXT:         {
// REWRITES-X86_64-GNU-NEXT:             *self = __slate_f80_div(*self, __o);
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT: impl core::ops::Neg for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     type Output = LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn neg(self) -> LongDouble {
// REWRITES-X86_64-GNU-NEXT:         __slate_f80_neg(self)
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT: impl core::cmp::PartialEq for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     fn eq(&self, __other: &LongDouble) -> bool {
// REWRITES-X86_64-GNU-NEXT:         __slate_f80_eq(*self, *__other)
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT: impl core::cmp::PartialOrd for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     fn partial_cmp(&self, __other: &LongDouble) -> Option<std::cmp::Ordering> {
// REWRITES-X86_64-GNU-NEXT:         if __slate_f80_lt(*self, *__other) {
// REWRITES-X86_64-GNU-NEXT:             Some(std::cmp::Ordering::Less)
// REWRITES-X86_64-GNU-NEXT:             if __slate_f80_gt(*self, *__other) {
// REWRITES-X86_64-GNU-NEXT:                 Some(std::cmp::Ordering::Greater)
// REWRITES-X86_64-GNU-NEXT:             } else {
// REWRITES-X86_64-GNU-NEXT:                 if __slate_f80_eq(*self, *__other) {
// REWRITES-X86_64-GNU-NEXT:                     Some(std::cmp::Ordering::Equal)
// REWRITES-X86_64-GNU-NEXT:                 } else {
// REWRITES-X86_64-GNU-NEXT:                     None
// REWRITES-X86_64-GNU-NEXT:                 }
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     __wchb: [i8; 4],
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-NEXT: #[derive(Clone, Copy)]
// REWRITES-X86_64-GNU-NEXT:     __tm_zone: *mut i8,
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: static mut _str_10: [i32; 8] = [74, 97, 110, 117, 97, 114, 121, 0];
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: static mut _str_11: [i32; 4] = [74, 97, 110, 0];
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: static mut _str_9: [i32; 4] = [37, 79, 66, 0];
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT:     fn wcsftime(_0: *mut i32, _1: usize, _2: *const i32, _3: *const tm) -> usize;
// REWRITES-X86_64-GNU-NEXT:     fn wcscmp(_0: *const i32, _1: *const i32) -> i32;
// REWRITES-X86_64-GNU-NEXT:     fn wcschr(_0: *const i32, _1: i32) -> *mut i32;
// REWRITES-X86_64-GNU-NEXT:     fn wcsstr(_0: *const i32, _1: *const i32) -> *mut i32;
// REWRITES-X86_64-GNU-NEXT:         _3: LongDouble,
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = c"%d %d %d %d %d %d\n".as_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         } else {
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT:     let mut output: [i8; 4] = [0; 4];
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = output.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT:     let mut source: [i8; 7] = [0; 7];
// REWRITES-X86_64-GNU-NEXT:     let mut destination: [i8; 8] = [0; 8];
// REWRITES-X86_64-GNU-NEXT:     let mut secret: [i8; 7] = [0; 7];
// REWRITES-X86_64-GNU-NEXT:     let mut first_copy: *mut i8 = std::ptr::null_mut();
// REWRITES-X86_64-GNU-NEXT:     let mut second_copy: *mut i8 = std::ptr::null_mut();
// REWRITES-X86_64-GNU-NEXT:     let mut mutable_phrase: [i8; 12] = [0; 12];
// REWRITES-X86_64-GNU-NEXT:     let mut mut_hit: *mut i8 = std::ptr::null_mut();
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = c"hello world".as_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = destination.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = source.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = destination.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.add(3) };
// REWRITES-X86_64-GNU-NEXT:     first_copy = (unsafe { strdup(c"c23".as_ptr()) }) as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     second_copy = (unsafe { strndup(c"library".as_ptr(), (3 as u64) as usize) }) as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 =
// REWRITES-X86_64-GNU-NEXT:         (unsafe { strchr({{__v[0-9]+}} as *const core::ffi::c_char, 119 as i32) }) as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = mutable_phrase.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     mut_hit = (unsafe { strchr({{__v[0-9]+}} as *const core::ffi::c_char, 119 as i32) }) as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = mutable_phrase.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 =
// REWRITES-X86_64-GNU-NEXT:         (unsafe { strstr({{__v[0-9]+}} as *const core::ffi::c_char, c"world".as_ptr()) }) as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = mutable_phrase.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 =
// REWRITES-X86_64-GNU-NEXT:         (unsafe { strstr({{__v[0-9]+}} as *const core::ffi::c_char, c"world".as_ptr()) }) as *mut i8;
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT:     let mut month: aligned::Aligned<aligned::A16, [i8; 32]> = aligned::Aligned([0; 32]);
// REWRITES-X86_64-GNU-NEXT:     let mut wide_month: aligned::Aligned<aligned::A16, [i32; 32]> = aligned::Aligned([0; 32]);
// REWRITES-X86_64-GNU-NEXT:     *month = [0; 32];
// REWRITES-X86_64-GNU-NEXT:     *wide_month = [0; 32];
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = month.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = month.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i32 = wide_month.as_mut_ptr() as *mut i32;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i32 = std::ptr::addr_of_mut!(_str_9).cast::<i32>();
// REWRITES-X86_64-GNU-NEXT:             {{__v[0-9]+}} as *mut i32,
// REWRITES-X86_64-GNU-NEXT:             {{__v[0-9]+}} as *const i32,
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i32 = wide_month.as_mut_ptr() as *mut i32;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i32 = std::ptr::addr_of_mut!(_str_10).cast::<i32>();
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { wcscmp({{__v[0-9]+}} as *const i32, {{__v[0-9]+}} as *const i32) };
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i32 = wide_month.as_mut_ptr() as *mut i32;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i32 = unsafe { wcschr({{__v[0-9]+}} as *const i32, 110 as i32) };
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i32 = wide_month.as_mut_ptr() as *mut i32;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i32 = unsafe { wcschr({{__v[0-9]+}} as *const i32, 110 as i32) };
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i32 = std::ptr::addr_of_mut!(_str_11).cast::<i32>();
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i32 = unsafe { wcsstr({{__v[0-9]+}} as *const i32, {{__v[0-9]+}} as *const i32) };
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i32 = wide_month.as_mut_ptr() as *mut i32;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i32 = std::ptr::addr_of_mut!(_str_11).cast::<i32>();
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i32 = unsafe { wcsstr({{__v[0-9]+}} as *const i32, {{__v[0-9]+}} as *const i32) };
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT:     let mut output: aligned::Aligned<aligned::A16, [i8; 64]> = aligned::Aligned([0; 64]);
// REWRITES-X86_64-GNU-NEXT:     let mut float_output: aligned::Aligned<aligned::A16, [i8; 16]> = aligned::Aligned([0; 16]);
// REWRITES-X86_64-GNU-NEXT:     let mut double_output: aligned::Aligned<aligned::A16, [i8; 16]> = aligned::Aligned([0; 16]);
// REWRITES-X86_64-GNU-NEXT:     let mut long_double_output: aligned::Aligned<aligned::A16, [i8; 16]> =
// REWRITES-X86_64-GNU-NEXT:         aligned::Aligned([0; 16]);
// REWRITES-X86_64-GNU-NEXT:     *output = [0; 64];
// REWRITES-X86_64-GNU-NEXT:     *float_output = [0; 16];
// REWRITES-X86_64-GNU-NEXT:     *double_output = [0; 16];
// REWRITES-X86_64-GNU-NEXT:     *long_double_output = [0; 16];
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = output.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = float_output.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = double_output.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = long_double_output.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 224, 0, 64]);
// REWRITES-X86_64-GNU-NEXT:         __slate_strfroml__ri32_pc_usize_pc_f80(
// REWRITES-X86_64-GNU-NEXT:             c"%.1f".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:             {{__v[0-9]+}},
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = output.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = float_output.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = double_output.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = long_double_output.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT:     1 + {{__v[0-9]+}} + 8 + 8 + 16 + 16 + 32 + 32 + 64 + 64 + 64 + 64 + 64 + 64 + (((0.0000000000000000000000000000000000000000000014013 > {{__v[0-9]+}}) as i32) + ((0.000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000005 > {{__v[0-9]+}}) as i32) + ((LongDouble([1, 0, 0, 0, 0, 0, 0, 0, 0, 0]) > LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0])) as i32) + ((340282346999999984391321947108527833088.0 <= {{__v[0-9]+}}) as i32) + ((179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368.0 <= {{__v[0-9]+}}) as i32) + ((LongDouble([255, 255, 255, 255, 255, 255, 255, 255, 254, 127]) <= LongDouble([255, 255, 255, 255, 255, 255, 255, 255, 254, 127])) as i32) + ((1 >= {{__v[0-9]+}}) as i32) + ((1 >= {{__v[0-9]+}}) as i32) + ((1 >= {{__v[0-9]+}}) as i32) + ((4 == {{__v[0-9]+}}) as i32) + ((8 == {{__v[0-9]+}}) as i32) + ((16 == {{__v[0-9]+}}) as i32)) + (((202311 == {{__v[0-9]+}}) as i32) + ((202311 == {{__v[0-9]+}}) as i32) + ((202311 == {{__v[0-9]+}}) as i32) + ((202311 == {{__v[0-9]+}}) as i32) + ((202311 == {{__v[0-9]+}}) as i32) + ((202311 == {{__v[0-9]+}}) as i32) + ((202311 == {{__v[0-9]+}}) as i32))
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: unsafe extern "C" {
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_cf80_div(
// REWRITES-X86_64-GNU-NEXT:         __a: num_complex::Complex<LongDouble>,
// REWRITES-X86_64-GNU-NEXT:         __b: num_complex::Complex<LongDouble>,
// REWRITES-X86_64-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_cf80_mul(
// REWRITES-X86_64-GNU-NEXT:         __a: num_complex::Complex<LongDouble>,
// REWRITES-X86_64-GNU-NEXT:         __b: num_complex::Complex<LongDouble>,
// REWRITES-X86_64-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f128_nexttoward(__from: f128, __toward: f128) -> f128;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_abs(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_acos(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_acosh(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_add(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_asin(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_asinh(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_atan(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_atanh(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_cbrt(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_ceil(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_copysign(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_cos(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_cosh(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_div(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_eq(__a: LongDouble, __b: LongDouble) -> bool;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_exp(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_exp2(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_expm1(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_fdim(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_floor(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_fma(__a: LongDouble, __b: LongDouble, __c: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_fmax(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_fmin(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_fmod(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_fract(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_bool(__a: bool) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_f32(__a: f32) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_f64(__a: f64) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_i128(__a: i128) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_i16(__a: i16) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_i32(__a: i32) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_i64(__a: i64) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_i8(__a: i8) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_u128(__a: u128) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_u16(__a: u16) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_u32(__a: u32) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_u64(__a: u64) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_u8(__a: u8) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_ge(__a: LongDouble, __b: LongDouble) -> bool;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_gt(__a: LongDouble, __b: LongDouble) -> bool;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_hypot(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_is_fp_class(__a: LongDouble, __flags: i32) -> bool;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_le(__a: LongDouble, __b: LongDouble) -> bool;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_log(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_log10(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_log1p(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_log2(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_lt(__a: LongDouble, __b: LongDouble) -> bool;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_mul(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_ne(__a: LongDouble, __b: LongDouble) -> bool;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_nearbyint(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_neg(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_pow(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_powi(__a: LongDouble, __n: i32) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_remainder(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_rint(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_round(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_signbit(__a: LongDouble) -> bool;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_sin(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_sinh(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_sqrt(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_sub(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_tan(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_tanh(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_bool(__a: LongDouble) -> bool;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_f32(__a: LongDouble) -> f32;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_f64(__a: LongDouble) -> f64;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_i128(__a: LongDouble) -> i128;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_i16(__a: LongDouble) -> i16;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_i32(__a: LongDouble) -> i32;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_i64(__a: LongDouble) -> i64;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_i8(__a: LongDouble) -> i8;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_u128(__a: LongDouble) -> u128;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_u16(__a: LongDouble) -> u16;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_u32(__a: LongDouble) -> u32;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_u64(__a: LongDouble) -> u64;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_u8(__a: LongDouble) -> u8;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_trunc(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn __slate_strfroml__ri32_pc_usize_pc_f80(
// REWRITES-X86_64-GNU-NEXT:         _0: *mut core::ffi::c_char,
// REWRITES-X86_64-GNU-NEXT:         _1: usize,
// REWRITES-X86_64-GNU-NEXT:         _2: *const core::ffi::c_char,
// REWRITES-X86_64-GNU-NEXT:         _3: LongDouble,
// REWRITES-X86_64-GNU-NEXT:     ) -> i32;
// REWRITES-X86_64-GNU-NEXT: }
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-NEXT:     __wchb: [u8; 4],
// REWRITES-AARCH64-GNU-NEXT:     __tm_zone: *mut u8,
// REWRITES-AARCH64-GNU-NEXT: static mut _str_10: [u32; 8] = [74, 97, 110, 117, 97, 114, 121, 0];
// REWRITES-AARCH64-GNU-NEXT: static mut _str_11: [u32; 4] = [74, 97, 110, 0];
// REWRITES-AARCH64-GNU-NEXT: static mut _str_9: [u32; 4] = [37, 79, 66, 0];
// REWRITES-AARCH64-GNU-NEXT:     fn wcsftime(_0: *mut u32, _1: usize, _2: *const u32, _3: *const tm) -> usize;
// REWRITES-AARCH64-GNU-NEXT:     fn wcscmp(_0: *const u32, _1: *const u32) -> i32;
// REWRITES-AARCH64-GNU-NEXT:     fn wcschr(_0: *const u32, _1: u32) -> *mut u32;
// REWRITES-AARCH64-GNU-NEXT:     fn wcsstr(_0: *const u32, _1: *const u32) -> *mut u32;
// REWRITES-AARCH64-GNU-NEXT:         _3: f128,
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = c"%d %d %d %d %d %d\n".as_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let mut output: [u8; 4] = [0; 4];
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = output.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let mut source: [u8; 7] = [0; 7];
// REWRITES-AARCH64-GNU-NEXT:     let mut destination: [u8; 8] = [0; 8];
// REWRITES-AARCH64-GNU-NEXT:     let mut secret: [u8; 7] = [0; 7];
// REWRITES-AARCH64-GNU-NEXT:     let mut first_copy: *mut u8 = std::ptr::null_mut();
// REWRITES-AARCH64-GNU-NEXT:     let mut second_copy: *mut u8 = std::ptr::null_mut();
// REWRITES-AARCH64-GNU-NEXT:     let mut mutable_phrase: [u8; 12] = [0; 12];
// REWRITES-AARCH64-GNU-NEXT:     let mut mut_hit: *mut u8 = std::ptr::null_mut();
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = c"hello world".as_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = destination.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = source.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = destination.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(3) };
// REWRITES-AARCH64-GNU-NEXT:     first_copy = (unsafe { strdup(c"c23".as_ptr()) }) as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     second_copy = (unsafe { strndup(c"library".as_ptr(), (3 as u64) as usize) }) as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 =
// REWRITES-AARCH64-GNU-NEXT:         (unsafe { strchr({{__v[0-9]+}} as *const core::ffi::c_char, 119 as i32) }) as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = mutable_phrase.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     mut_hit = (unsafe { strchr({{__v[0-9]+}} as *const core::ffi::c_char, 119 as i32) }) as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = mutable_phrase.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 =
// REWRITES-AARCH64-GNU-NEXT:         (unsafe { strstr({{__v[0-9]+}} as *const core::ffi::c_char, c"world".as_ptr()) }) as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = mutable_phrase.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 =
// REWRITES-AARCH64-GNU-NEXT:         (unsafe { strstr({{__v[0-9]+}} as *const core::ffi::c_char, c"world".as_ptr()) }) as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let mut month: [u8; 32] = [0; 32];
// REWRITES-AARCH64-GNU-NEXT:     let mut wide_month: [u32; 32] = [0; 32];
// REWRITES-AARCH64-GNU-NEXT:     month = [0; 32];
// REWRITES-AARCH64-GNU-NEXT:     wide_month = [0; 32];
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = month.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = month.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u32 = wide_month.as_mut_ptr() as *mut u32;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u32 = std::ptr::addr_of_mut!(_str_9).cast::<u32>();
// REWRITES-AARCH64-GNU-NEXT:             {{__v[0-9]+}} as *mut u32,
// REWRITES-AARCH64-GNU-NEXT:             {{__v[0-9]+}} as *const u32,
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u32 = wide_month.as_mut_ptr() as *mut u32;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u32 = std::ptr::addr_of_mut!(_str_10).cast::<u32>();
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { wcscmp({{__v[0-9]+}} as *const u32, {{__v[0-9]+}} as *const u32) };
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u32 = wide_month.as_mut_ptr() as *mut u32;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u32 = unsafe { wcschr({{__v[0-9]+}} as *const u32, 110 as u32) };
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u32 = wide_month.as_mut_ptr() as *mut u32;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u32 = unsafe { wcschr({{__v[0-9]+}} as *const u32, 110 as u32) };
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u32 = std::ptr::addr_of_mut!(_str_11).cast::<u32>();
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u32 = unsafe { wcsstr({{__v[0-9]+}} as *const u32, {{__v[0-9]+}} as *const u32) };
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u32 = wide_month.as_mut_ptr() as *mut u32;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u32 = std::ptr::addr_of_mut!(_str_11).cast::<u32>();
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u32 = unsafe { wcsstr({{__v[0-9]+}} as *const u32, {{__v[0-9]+}} as *const u32) };
// REWRITES-AARCH64-GNU-NEXT:     let mut output: [u8; 64] = [0; 64];
// REWRITES-AARCH64-GNU-NEXT:     let mut float_output: [u8; 16] = [0; 16];
// REWRITES-AARCH64-GNU-NEXT:     let mut double_output: [u8; 16] = [0; 16];
// REWRITES-AARCH64-GNU-NEXT:     let mut long_double_output: [u8; 16] = [0; 16];
// REWRITES-AARCH64-GNU-NEXT:     output = [0; 64];
// REWRITES-AARCH64-GNU-NEXT:     float_output = [0; 16];
// REWRITES-AARCH64-GNU-NEXT:     double_output = [0; 16];
// REWRITES-AARCH64-GNU-NEXT:     long_double_output = [0; 16];
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = output.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = float_output.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = double_output.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = long_double_output.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = c"%.1f".as_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 3.500000e+00f128;
// REWRITES-AARCH64-GNU-NEXT:         strfroml(
// REWRITES-AARCH64-GNU-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// REWRITES-AARCH64-GNU-NEXT:             {{__v[0-9]+}} as f128,
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = output.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = float_output.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = double_output.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = long_double_output.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = 1 + {{__v[0-9]+}} + 8 + 8 + 16 + 16 + 32 + 32 + 64 + 64 + 64 + 64 + 64 + 64;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = ((0.0000000000000000000000000000000000000000000014013 > {{__v[0-9]+}}) as i32) + ((0.000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000005 > {{__v[0-9]+}}) as i32);
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 6.475180e-4966f128;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 0.000000e+00f128;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} > {{__v[0-9]+}}) as i32) + ((340282346999999984391321947108527833088.0 <= {{__v[0-9]+}}) as i32) + ((179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368.0 <= {{__v[0-9]+}}) as i32);
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 1.18973149535723176508575932662800702E+4932f128;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 1.18973149535723176508575932662800702E+4932f128;
// REWRITES-AARCH64-GNU-NEXT:     {{__v[0-9]+}}
// REWRITES-AARCH64-GNU-NEXT:         + ({{__v[0-9]+}}
// REWRITES-AARCH64-GNU-NEXT:             + (({{__v[0-9]+}} <= {{__v[0-9]+}}) as i32)
// REWRITES-AARCH64-GNU-NEXT:             + ((1 >= {{__v[0-9]+}}) as i32)
// REWRITES-AARCH64-GNU-NEXT:             + ((1 >= {{__v[0-9]+}}) as i32)
// REWRITES-AARCH64-GNU-NEXT:             + ((1 >= {{__v[0-9]+}}) as i32)
// REWRITES-AARCH64-GNU-NEXT:             + ((4 == {{__v[0-9]+}}) as i32)
// REWRITES-AARCH64-GNU-NEXT:             + ((8 == {{__v[0-9]+}}) as i32)
// REWRITES-AARCH64-GNU-NEXT:             + ((16 == {{__v[0-9]+}}) as i32))
// REWRITES-AARCH64-GNU-NEXT:         + (((202311 == {{__v[0-9]+}}) as i32)
// REWRITES-AARCH64-GNU-NEXT:             + ((202311 == {{__v[0-9]+}}) as i32)
// REWRITES-AARCH64-GNU-NEXT:             + ((202311 == {{__v[0-9]+}}) as i32)
// REWRITES-AARCH64-GNU-NEXT:             + ((202311 == {{__v[0-9]+}}) as i32)
// REWRITES-AARCH64-GNU-NEXT:             + ((202311 == {{__v[0-9]+}}) as i32)
// REWRITES-AARCH64-GNU-NEXT:             + ((202311 == {{__v[0-9]+}}) as i32)
// REWRITES-AARCH64-GNU-NEXT:             + ((202311 == {{__v[0-9]+}}) as i32))
// SLATE-FILECHECK-END rewrites-aarch64-gnu
