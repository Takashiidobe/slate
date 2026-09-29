#include <fenv.h>
#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdatomic.h>
#include <stdckdint.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <uchar.h>
#include <wchar.h>

#if __has_include(<stdbit.h>)
#include <stdbit.h>
#define C23_HAS_STDBIT 1
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
#else
#define C23_HAS_STDBIT 0
static int c23_stdbit(void) { return 0; }
#endif

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
                        C23_HAS_STDBIT;
  return integer_widths + floating_limits + header_versions;
}

int main(void) {
  printf("%d %d %d %d %d %d\n", c23_stdbit(), c23_checked_arithmetic(),
         c23_utf8(), c23_memory(), c23_time(), c23_io() + c23_limits());
  return 0;
}


// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-unknown-linux-gnu" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f80;
// DEFAULT-NEXT:         storage bool [size=1, align=1];
// DEFAULT-NEXT:         storage i8, u8 [size=1, align=1];
// DEFAULT-NEXT:         storage i16, u16 [size=2, align=2];
// DEFAULT-NEXT:         storage i32, u32 [size=4, align=4];
// DEFAULT-NEXT:         storage i64, u64 [size=8, align=8];
// DEFAULT-NEXT:         storage i128, u128 [size=16, align=16];
// DEFAULT-NEXT:         storage bf16 [size=2, align=2];
// DEFAULT-NEXT:         storage f16 [size=2, align=2];
// DEFAULT-NEXT:         storage f32 [size=4, align=4];
// DEFAULT-NEXT:         storage f64 [size=8, align=8];
// DEFAULT-NEXT:         storage f80 [size=16, align=16];
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE_wchar_t:[0-9]+]] wchar_t = i32;
// DEFAULT-NEXT:     type @type[[TYPE___uint8_t:[0-9]+]] __uint8_t = u8;
// DEFAULT-NEXT:     type @type[[TYPE___uint16_t:[0-9]+]] __uint16_t = u16;
// DEFAULT-NEXT:     type @type[[TYPE___uint32_t:[0-9]+]] __uint32_t = u32;
// DEFAULT-NEXT:     type @type[[TYPE___uint64_t:[0-9]+]] __uint64_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE___time_t:[0-9]+]] __time_t = i64;
// DEFAULT-NEXT:     type @type[[TYPE___syscall_slong_t:[0-9]+]] __syscall_slong_t = i64;
// DEFAULT-NEXT:     type @type[[TYPE_uint8_t:[0-9]+]] uint8_t = u8;
// DEFAULT-NEXT:     type @type[[TYPE_uint16_t:[0-9]+]] uint16_t = u16;
// DEFAULT-NEXT:     type @type[[TYPE_uint32_t:[0-9]+]] uint32_t = u32;
// DEFAULT-NEXT:     type @type[[TYPE_uint64_t:[0-9]+]] uint64_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE_uint_fast16_t:[0-9]+]] uint_fast16_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE_atomic_char8_t:[0-9]+]] atomic_char8_t = u8;
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 __count: i32;
// DEFAULT-NEXT:         field1 __value: @type[[TYPE1:[0-9]+]];
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE1]] = union {
// DEFAULT-NEXT:         field0 __wch: u32;
// DEFAULT-NEXT:         field1 __wchb: array<i8, 4>;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE___mbstate_t:[0-9]+]] __mbstate_t = @type[[TYPE0]];
// DEFAULT-NEXT:     type @type[[TYPE_time_t:[0-9]+]] time_t = i64;
// DEFAULT-NEXT:     type @type[[TYPE_timespec:[0-9]+]] timespec = struct {
// DEFAULT-NEXT:         field0 tv_sec: i64;
// DEFAULT-NEXT:         field1 tv_nsec: i64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_tm:[0-9]+]] tm = struct {
// DEFAULT-NEXT:         field0 tm_sec: i32;
// DEFAULT-NEXT:         field1 tm_min: i32;
// DEFAULT-NEXT:         field2 tm_hour: i32;
// DEFAULT-NEXT:         field3 tm_mday: i32;
// DEFAULT-NEXT:         field4 tm_mon: i32;
// DEFAULT-NEXT:         field5 tm_year: i32;
// DEFAULT-NEXT:         field6 tm_wday: i32;
// DEFAULT-NEXT:         field7 tm_yday: i32;
// DEFAULT-NEXT:         field8 tm_isdst: i32;
// DEFAULT-NEXT:         field9 tm_gmtoff: i64;
// DEFAULT-NEXT:         field10 tm_zone: ptr<const i8>;
// DEFAULT-NEXT:     } [size=56, align=8, offsets=[0, 4, 8, 12, 16, 20, 24, 28, 32, 40, 48]];
// DEFAULT-NEXT:     type @type[[TYPE_mbstate_t:[0-9]+]] mbstate_t = @type[[TYPE0]];
// DEFAULT-NEXT:     type @type[[TYPE_char8_t:[0-9]+]] char8_t = u8;
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([65, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 12> [storage=static] = code_units<array<i8, 12>>([104, 101, 108, 108, 111, 32, 119, 111, 114, 108, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([99, 50, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_4:[0-9]+]] .str[[VALUE_str_4]]: array<i8, 8> [storage=static] = code_units<array<i8, 8>>([108, 105, 98, 114, 97, 114, 121, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_5:[0-9]+]] .str[[VALUE_str_5]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([99, 50, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_6:[0-9]+]] .str[[VALUE_str_6]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([108, 105, 98, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_7:[0-9]+]] .str[[VALUE_str_7]]: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([119, 111, 114, 108, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_8:[0-9]+]] .str[[VALUE_str_8]]: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([119, 111, 114, 108, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_9:[0-9]+]] .str[[VALUE_str_9]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 79, 66, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_10:[0-9]+]] .str[[VALUE_str_10]]: array<i8, 8> [storage=static] = code_units<array<i8, 8>>([74, 97, 110, 117, 97, 114, 121, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_11:[0-9]+]] .str[[VALUE_str_11]]: array<i32, 4> [storage=static] = code_units<array<i32, 4>>([37, 79, 66, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_12:[0-9]+]] .str[[VALUE_str_12]]: array<i32, 8> [storage=static] = code_units<array<i32, 8>>([74, 97, 110, 117, 97, 114, 121, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_13:[0-9]+]] .str[[VALUE_str_13]]: array<i32, 4> [storage=static] = code_units<array<i32, 4>>([74, 97, 110, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_14:[0-9]+]] .str[[VALUE_str_14]]: array<i32, 4> [storage=static] = code_units<array<i32, 4>>([74, 97, 110, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_15:[0-9]+]] .str[[VALUE_str_15]]: array<i8, 16> [storage=static] = code_units<array<i8, 16>>([37, 98, 32, 37, 119, 49, 54, 117, 32, 37, 119, 102, 49, 54, 117, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_16:[0-9]+]] .str[[VALUE_str_16]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([49, 48, 49, 49, 32, 53, 53, 32, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_17:[0-9]+]] .str[[VALUE_str_17]]: array<i8, 16> [storage=static] = code_units<array<i8, 16>>([37, 98, 32, 37, 119, 49, 54, 117, 32, 37, 119, 102, 49, 54, 117, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_18:[0-9]+]] .str[[VALUE_str_18]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([37, 46, 49, 102, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_19:[0-9]+]] .str[[VALUE_str_19]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([37, 46, 49, 102, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_20:[0-9]+]] .str[[VALUE_str_20]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([37, 46, 49, 102, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_21:[0-9]+]] .str[[VALUE_str_21]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([49, 49, 48, 49, 32, 50, 49, 32, 51, 52, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_22:[0-9]+]] .str[[VALUE_str_22]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([49, 46, 53, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_23:[0-9]+]] .str[[VALUE_str_23]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([50, 46, 53, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_24:[0-9]+]] .str[[VALUE_str_24]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([51, 46, 53, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_25:[0-9]+]] .str[[VALUE_str_25]]: array<i8, 19> [storage=static] = code_units<array<i8, 19>>([37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_snprintf:[0-9]+]] @snprintf(%[[VALUE___s:[0-9]+]] __s: ptr<i8> [restrict], %[[VALUE___maxlen:[0-9]+]] __maxlen: u64, %[[VALUE___format_2:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_sscanf:[0-9]+]] @sscanf(%[[VALUE___s_2:[0-9]+]] __s: ptr<const i8> [restrict], %[[VALUE___format_3:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external] [asm_name="__isoc23_sscanf"];
// DEFAULT-NEXT:     fn %[[VALUE_strfromd:[0-9]+]] @strfromd(%[[VALUE___dest:[0-9]+]] __dest: ptr<i8>, %[[VALUE___size:[0-9]+]] __size: u64, %[[VALUE___format_4:[0-9]+]] __format: ptr<const i8>, %[[VALUE___f:[0-9]+]] __f: f64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_strfromf:[0-9]+]] @strfromf(%[[VALUE___dest_2:[0-9]+]] __dest: ptr<i8>, %[[VALUE___size_2:[0-9]+]] __size: u64, %[[VALUE___format_5:[0-9]+]] __format: ptr<const i8>, %[[VALUE___f_2:[0-9]+]] __f: f32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_strfroml:[0-9]+]] @strfroml(%[[VALUE___dest_3:[0-9]+]] __dest: ptr<i8>, %[[VALUE___size_3:[0-9]+]] __size: u64, %[[VALUE___format_6:[0-9]+]] __format: ptr<const i8>, %[[VALUE___f_3:[0-9]+]] __f: f80) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_free:[0-9]+]] @free(%[[VALUE___ptr:[0-9]+]] __ptr: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_memccpy:[0-9]+]] @memccpy(%[[VALUE___dest_4:[0-9]+]] __dest: ptr<void> [restrict], %[[VALUE___src:[0-9]+]] __src: ptr<const void> [restrict], %[[VALUE___c:[0-9]+]] __c: i32, %[[VALUE___n:[0-9]+]] __n: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_memset_explicit:[0-9]+]] @memset_explicit(%[[VALUE___s_3:[0-9]+]] __s: ptr<void>, %[[VALUE___c_2:[0-9]+]] __c: i32, %[[VALUE___n_2:[0-9]+]] __n: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_memchr:[0-9]+]] @memchr(%[[VALUE___s_4:[0-9]+]] __s: ptr<const void>, %[[VALUE___c_3:[0-9]+]] __c: i32, %[[VALUE___n_3:[0-9]+]] __n: u64) -> ptr<void> [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE_strcmp:[0-9]+]] @strcmp(%[[VALUE___s1:[0-9]+]] __s1: ptr<const i8>, %[[VALUE___s2:[0-9]+]] __s2: ptr<const i8>) -> i32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE_strdup:[0-9]+]] @strdup(%[[VALUE___s_5:[0-9]+]] __s: ptr<const i8>) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_strndup:[0-9]+]] @strndup(%[[VALUE___string:[0-9]+]] __string: ptr<const i8>, %[[VALUE___n_4:[0-9]+]] __n: u64) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_strchr:[0-9]+]] @strchr(%[[VALUE___s_6:[0-9]+]] __s: ptr<const i8>, %[[VALUE___c_4:[0-9]+]] __c: i32) -> ptr<i8> [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE_strstr:[0-9]+]] @strstr(%[[VALUE___haystack:[0-9]+]] __haystack: ptr<const i8>, %[[VALUE___needle:[0-9]+]] __needle: ptr<const i8>) -> ptr<i8> [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE_strftime:[0-9]+]] @strftime(%[[VALUE___s_7:[0-9]+]] __s: ptr<i8> [restrict], %[[VALUE___maxsize:[0-9]+]] __maxsize: u64, %[[VALUE___format_7:[0-9]+]] __format: ptr<const i8> [restrict], %[[VALUE___tp:[0-9]+]] __tp: ptr<const @type[[TYPE_tm]]> [restrict]) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_gmtime_r:[0-9]+]] @gmtime_r(%[[VALUE___timer:[0-9]+]] __timer: ptr<const i64> [restrict], %[[VALUE___tp_2:[0-9]+]] __tp: ptr<@type[[TYPE_tm]]> [restrict]) -> ptr<@type[[TYPE_tm]]> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_localtime_r:[0-9]+]] @localtime_r(%[[VALUE___timer_2:[0-9]+]] __timer: ptr<const i64> [restrict], %[[VALUE___tp_3:[0-9]+]] __tp: ptr<@type[[TYPE_tm]]> [restrict]) -> ptr<@type[[TYPE_tm]]> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_timegm:[0-9]+]] @timegm(%[[VALUE___tp_4:[0-9]+]] __tp: ptr<@type[[TYPE_tm]]>) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_timespec_getres:[0-9]+]] @timespec_getres(%[[VALUE___ts:[0-9]+]] __ts: ptr<@type[[TYPE_timespec]]>, %[[VALUE___base:[0-9]+]] __base: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_mbrtoc8:[0-9]+]] @mbrtoc8(%[[VALUE___pc8:[0-9]+]] __pc8: ptr<u8> [restrict], %[[VALUE___s_8:[0-9]+]] __s: ptr<const i8> [restrict], %[[VALUE___n_5:[0-9]+]] __n: u64, %[[VALUE___p:[0-9]+]] __p: ptr<@type[[TYPE0]]> [restrict]) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_c8rtomb:[0-9]+]] @c8rtomb(%[[VALUE___s_9:[0-9]+]] __s: ptr<i8> [restrict], %[[VALUE___c8:[0-9]+]] __c8: u8, %[[VALUE___ps:[0-9]+]] __ps: ptr<@type[[TYPE0]]> [restrict]) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_wcscmp:[0-9]+]] @wcscmp(%[[VALUE___s1_2:[0-9]+]] __s1: ptr<const i32>, %[[VALUE___s2_2:[0-9]+]] __s2: ptr<const i32>) -> i32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE_wcschr:[0-9]+]] @wcschr(%[[VALUE___wcs:[0-9]+]] __wcs: ptr<const i32>, %[[VALUE___wc:[0-9]+]] __wc: i32) -> ptr<i32> [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE_wcsstr:[0-9]+]] @wcsstr(%[[VALUE___haystack_2:[0-9]+]] __haystack: ptr<const i32>, %[[VALUE___needle_2:[0-9]+]] __needle: ptr<const i32>) -> ptr<i32> [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE_wcsftime:[0-9]+]] @wcsftime(%[[VALUE___s_10:[0-9]+]] __s: ptr<i32> [restrict], %[[VALUE___maxsize_2:[0-9]+]] __maxsize: u64, %[[VALUE___format_8:[0-9]+]] __format: ptr<const i32> [restrict], %[[VALUE___tp_5:[0-9]+]] __tp: ptr<const @type[[TYPE_tm]]> [restrict]) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_clzll:[0-9]+]] @__builtin_clzll(%[[VALUE0:[0-9]+]] <unnamed>: u64) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___clz64_inline:[0-9]+]] @__clz64_inline(%[[VALUE___x:[0-9]+]] __x: u64) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<u32>(eq<u64>(read<u64>(%[[VALUE___x]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), const<u32>(64), reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn(u64) -> i32>(%[[VALUE___builtin_clzll]], read<u64>(%[[VALUE___x]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_clz:[0-9]+]] @__builtin_clz(%[[VALUE1:[0-9]+]] <unnamed>: u32) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___clz32_inline:[0-9]+]] @__clz32_inline(%[[VALUE___x_2:[0-9]+]] __x: u32) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<u32>(eq<u32>(read<u32>(%[[VALUE___x_2]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))), const<u32>(32), reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_clz]], read<u32>(%[[VALUE___x_2]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___clz16_inline:[0-9]+]] @__clz16_inline(%[[VALUE___x_3:[0-9]+]] __x: u16) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return sub<u32, overflow=wrap>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___clz32_inline]], widen<u32, reason=arg>(read<u16>(%[[VALUE___x_3]]))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(16)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___clz8_inline:[0-9]+]] @__clz8_inline(%[[VALUE___x_4:[0-9]+]] __x: u8) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return sub<u32, overflow=wrap>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___clz32_inline]], widen<u32, reason=arg>(read<u8>(%[[VALUE___x_4]]))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(24)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___clo64_inline:[0-9]+]] @__clo64_inline(%[[VALUE___x_5:[0-9]+]] __x: u64) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<u32, signature=fn(u64) -> u32>(%[[VALUE___clz64_inline]], not<u64>(read<u64>(%[[VALUE___x_5]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___clo32_inline:[0-9]+]] @__clo32_inline(%[[VALUE___x_6:[0-9]+]] __x: u32) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<u32, signature=fn(u32) -> u32>(%[[VALUE___clz32_inline]], not<u32>(read<u32>(%[[VALUE___x_6]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___clo16_inline:[0-9]+]] @__clo16_inline(%[[VALUE___x_7:[0-9]+]] __x: u16) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<u32, signature=fn(u16) -> u32>(%[[VALUE___clz16_inline]], reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(not<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE___x_7]])))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___clo8_inline:[0-9]+]] @__clo8_inline(%[[VALUE___x_8:[0-9]+]] __x: u8) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<u32, signature=fn(u8) -> u32>(%[[VALUE___clz8_inline]], reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(not<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE___x_8]])))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_ctzll:[0-9]+]] @__builtin_ctzll(%[[VALUE2:[0-9]+]] <unnamed>: u64) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___ctz64_inline:[0-9]+]] @__ctz64_inline(%[[VALUE___x_9:[0-9]+]] __x: u64) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<u32>(eq<u64>(read<u64>(%[[VALUE___x_9]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), const<u32>(64), reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn(u64) -> i32>(%[[VALUE___builtin_ctzll]], read<u64>(%[[VALUE___x_9]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_ctz:[0-9]+]] @__builtin_ctz(%[[VALUE3:[0-9]+]] <unnamed>: u32) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___ctz32_inline:[0-9]+]] @__ctz32_inline(%[[VALUE___x_10:[0-9]+]] __x: u32) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<u32>(eq<u32>(read<u32>(%[[VALUE___x_10]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))), const<u32>(32), reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_ctz]], read<u32>(%[[VALUE___x_10]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___ctz16_inline:[0-9]+]] @__ctz16_inline(%[[VALUE___x_11:[0-9]+]] __x: u16) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<u32>(eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE___x_11]]))), const<i32>(0)), const<u32>(16), reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_ctz]], widen<u32, reason=arg>(read<u16>(%[[VALUE___x_11]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___ctz8_inline:[0-9]+]] @__ctz8_inline(%[[VALUE___x_12:[0-9]+]] __x: u8) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<u32>(eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE___x_12]]))), const<i32>(0)), const<u32>(8), reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_ctz]], widen<u32, reason=arg>(read<u8>(%[[VALUE___x_12]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___cto64_inline:[0-9]+]] @__cto64_inline(%[[VALUE___x_13:[0-9]+]] __x: u64) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<u32, signature=fn(u64) -> u32>(%[[VALUE___ctz64_inline]], not<u64>(read<u64>(%[[VALUE___x_13]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___cto32_inline:[0-9]+]] @__cto32_inline(%[[VALUE___x_14:[0-9]+]] __x: u32) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<u32, signature=fn(u32) -> u32>(%[[VALUE___ctz32_inline]], not<u32>(read<u32>(%[[VALUE___x_14]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___cto16_inline:[0-9]+]] @__cto16_inline(%[[VALUE___x_15:[0-9]+]] __x: u16) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<u32, signature=fn(u16) -> u32>(%[[VALUE___ctz16_inline]], reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(not<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE___x_15]])))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___cto8_inline:[0-9]+]] @__cto8_inline(%[[VALUE___x_16:[0-9]+]] __x: u8) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<u32, signature=fn(u8) -> u32>(%[[VALUE___ctz8_inline]], reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(not<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE___x_16]])))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___flz64_inline:[0-9]+]] @__flz64_inline(%[[VALUE___x_17:[0-9]+]] __x: u64) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: u32 [synthetic];
// DEFAULT-NEXT:         if eq<u64>(read<u64>(%[[VALUE___x_17]]), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:             write<u32>(%[[VALUE4]], reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<u32>(%[[VALUE4]], add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)), call<u32, signature=fn(u64) -> u32>(%[[VALUE___clo64_inline]], read<u64>(%[[VALUE___x_17]]))));
// DEFAULT-NEXT:         return read<u32>(%[[VALUE4]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___flz32_inline:[0-9]+]] @__flz32_inline(%[[VALUE___x_18:[0-9]+]] __x: u32) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: u32 [synthetic];
// DEFAULT-NEXT:         if eq<u32>(read<u32>(%[[VALUE___x_18]]), reinterpret<u32, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:             write<u32>(%[[VALUE5]], reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<u32>(%[[VALUE5]], add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)), call<u32, signature=fn(u32) -> u32>(%[[VALUE___clo32_inline]], read<u32>(%[[VALUE___x_18]]))));
// DEFAULT-NEXT:         return read<u32>(%[[VALUE5]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___flz16_inline:[0-9]+]] @__flz16_inline(%[[VALUE___x_19:[0-9]+]] __x: u16) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: u32 [synthetic];
// DEFAULT-NEXT:         if eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE___x_19]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))))
// DEFAULT-NEXT:             write<u32>(%[[VALUE6]], reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<u32>(%[[VALUE6]], add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)), call<u32, signature=fn(u16) -> u32>(%[[VALUE___clo16_inline]], read<u16>(%[[VALUE___x_19]]))));
// DEFAULT-NEXT:         return read<u32>(%[[VALUE6]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___flz8_inline:[0-9]+]] @__flz8_inline(%[[VALUE___x_20:[0-9]+]] __x: u8) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE7:[0-9]+]]: u32 [synthetic];
// DEFAULT-NEXT:         if eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE___x_20]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))))
// DEFAULT-NEXT:             write<u32>(%[[VALUE7]], reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<u32>(%[[VALUE7]], add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)), call<u32, signature=fn(u8) -> u32>(%[[VALUE___clo8_inline]], read<u8>(%[[VALUE___x_20]]))));
// DEFAULT-NEXT:         return read<u32>(%[[VALUE7]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___flo64_inline:[0-9]+]] @__flo64_inline(%[[VALUE___x_21:[0-9]+]] __x: u64) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE8:[0-9]+]]: u32 [synthetic];
// DEFAULT-NEXT:         if eq<u64>(read<u64>(%[[VALUE___x_21]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<u32>(%[[VALUE8]], reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<u32>(%[[VALUE8]], add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)), call<u32, signature=fn(u64) -> u32>(%[[VALUE___clz64_inline]], read<u64>(%[[VALUE___x_21]]))));
// DEFAULT-NEXT:         return read<u32>(%[[VALUE8]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___flo32_inline:[0-9]+]] @__flo32_inline(%[[VALUE___x_22:[0-9]+]] __x: u32) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE9:[0-9]+]]: u32 [synthetic];
// DEFAULT-NEXT:         if eq<u32>(read<u32>(%[[VALUE___x_22]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:             write<u32>(%[[VALUE9]], reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<u32>(%[[VALUE9]], add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)), call<u32, signature=fn(u32) -> u32>(%[[VALUE___clz32_inline]], read<u32>(%[[VALUE___x_22]]))));
// DEFAULT-NEXT:         return read<u32>(%[[VALUE9]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___flo16_inline:[0-9]+]] @__flo16_inline(%[[VALUE___x_23:[0-9]+]] __x: u16) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE10:[0-9]+]]: u32 [synthetic];
// DEFAULT-NEXT:         if eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE___x_23]]))), const<i32>(0))
// DEFAULT-NEXT:             write<u32>(%[[VALUE10]], reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<u32>(%[[VALUE10]], add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)), call<u32, signature=fn(u16) -> u32>(%[[VALUE___clz16_inline]], read<u16>(%[[VALUE___x_23]]))));
// DEFAULT-NEXT:         return read<u32>(%[[VALUE10]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___flo8_inline:[0-9]+]] @__flo8_inline(%[[VALUE___x_24:[0-9]+]] __x: u8) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE11:[0-9]+]]: u32 [synthetic];
// DEFAULT-NEXT:         if eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE___x_24]]))), const<i32>(0))
// DEFAULT-NEXT:             write<u32>(%[[VALUE11]], reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<u32>(%[[VALUE11]], add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)), call<u32, signature=fn(u8) -> u32>(%[[VALUE___clz8_inline]], read<u8>(%[[VALUE___x_24]]))));
// DEFAULT-NEXT:         return read<u32>(%[[VALUE11]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___ftz64_inline:[0-9]+]] @__ftz64_inline(%[[VALUE___x_25:[0-9]+]] __x: u64) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE12:[0-9]+]]: u32 [synthetic];
// DEFAULT-NEXT:         if eq<u64>(read<u64>(%[[VALUE___x_25]]), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:             write<u32>(%[[VALUE12]], reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<u32>(%[[VALUE12]], add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)), call<u32, signature=fn(u64) -> u32>(%[[VALUE___cto64_inline]], read<u64>(%[[VALUE___x_25]]))));
// DEFAULT-NEXT:         return read<u32>(%[[VALUE12]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___ftz32_inline:[0-9]+]] @__ftz32_inline(%[[VALUE___x_26:[0-9]+]] __x: u32) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE13:[0-9]+]]: u32 [synthetic];
// DEFAULT-NEXT:         if eq<u32>(read<u32>(%[[VALUE___x_26]]), reinterpret<u32, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:             write<u32>(%[[VALUE13]], reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<u32>(%[[VALUE13]], add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)), call<u32, signature=fn(u32) -> u32>(%[[VALUE___cto32_inline]], read<u32>(%[[VALUE___x_26]]))));
// DEFAULT-NEXT:         return read<u32>(%[[VALUE13]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___ftz16_inline:[0-9]+]] @__ftz16_inline(%[[VALUE___x_27:[0-9]+]] __x: u16) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE14:[0-9]+]]: u32 [synthetic];
// DEFAULT-NEXT:         if eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE___x_27]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))))
// DEFAULT-NEXT:             write<u32>(%[[VALUE14]], reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<u32>(%[[VALUE14]], add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)), call<u32, signature=fn(u16) -> u32>(%[[VALUE___cto16_inline]], read<u16>(%[[VALUE___x_27]]))));
// DEFAULT-NEXT:         return read<u32>(%[[VALUE14]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___ftz8_inline:[0-9]+]] @__ftz8_inline(%[[VALUE___x_28:[0-9]+]] __x: u8) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE15:[0-9]+]]: u32 [synthetic];
// DEFAULT-NEXT:         if eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE___x_28]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))))
// DEFAULT-NEXT:             write<u32>(%[[VALUE15]], reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<u32>(%[[VALUE15]], add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)), call<u32, signature=fn(u8) -> u32>(%[[VALUE___cto8_inline]], read<u8>(%[[VALUE___x_28]]))));
// DEFAULT-NEXT:         return read<u32>(%[[VALUE15]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___fto64_inline:[0-9]+]] @__fto64_inline(%[[VALUE___x_29:[0-9]+]] __x: u64) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE16:[0-9]+]]: u32 [synthetic];
// DEFAULT-NEXT:         if eq<u64>(read<u64>(%[[VALUE___x_29]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<u32>(%[[VALUE16]], reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<u32>(%[[VALUE16]], add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)), call<u32, signature=fn(u64) -> u32>(%[[VALUE___ctz64_inline]], read<u64>(%[[VALUE___x_29]]))));
// DEFAULT-NEXT:         return read<u32>(%[[VALUE16]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___fto32_inline:[0-9]+]] @__fto32_inline(%[[VALUE___x_30:[0-9]+]] __x: u32) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE17:[0-9]+]]: u32 [synthetic];
// DEFAULT-NEXT:         if eq<u32>(read<u32>(%[[VALUE___x_30]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:             write<u32>(%[[VALUE17]], reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<u32>(%[[VALUE17]], add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)), call<u32, signature=fn(u32) -> u32>(%[[VALUE___ctz32_inline]], read<u32>(%[[VALUE___x_30]]))));
// DEFAULT-NEXT:         return read<u32>(%[[VALUE17]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___fto16_inline:[0-9]+]] @__fto16_inline(%[[VALUE___x_31:[0-9]+]] __x: u16) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE18:[0-9]+]]: u32 [synthetic];
// DEFAULT-NEXT:         if eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE___x_31]]))), const<i32>(0))
// DEFAULT-NEXT:             write<u32>(%[[VALUE18]], reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<u32>(%[[VALUE18]], add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)), call<u32, signature=fn(u16) -> u32>(%[[VALUE___ctz16_inline]], read<u16>(%[[VALUE___x_31]]))));
// DEFAULT-NEXT:         return read<u32>(%[[VALUE18]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___fto8_inline:[0-9]+]] @__fto8_inline(%[[VALUE___x_32:[0-9]+]] __x: u8) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE19:[0-9]+]]: u32 [synthetic];
// DEFAULT-NEXT:         if eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE___x_32]]))), const<i32>(0))
// DEFAULT-NEXT:             write<u32>(%[[VALUE19]], reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<u32>(%[[VALUE19]], add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)), call<u32, signature=fn(u8) -> u32>(%[[VALUE___ctz8_inline]], read<u8>(%[[VALUE___x_32]]))));
// DEFAULT-NEXT:         return read<u32>(%[[VALUE19]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_popcountll:[0-9]+]] @__builtin_popcountll(%[[VALUE20:[0-9]+]] <unnamed>: u64) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___cz64_inline:[0-9]+]] @__cz64_inline(%[[VALUE___x_33:[0-9]+]] __x: u64) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return sub<u32, overflow=wrap>(const<u32>(64), reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn(u64) -> i32>(%[[VALUE___builtin_popcountll]], read<u64>(%[[VALUE___x_33]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___co64_inline:[0-9]+]] @__co64_inline(%[[VALUE___x_34:[0-9]+]] __x: u64) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn(u64) -> i32>(%[[VALUE___builtin_popcountll]], read<u64>(%[[VALUE___x_34]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___hsb64_inline:[0-9]+]] @__hsb64_inline(%[[VALUE___x_35:[0-9]+]] __x: u64) -> bool [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return gt<u64>(xor<u64>(read<u64>(%[[VALUE___x_35]]), sub<u64, overflow=wrap>(read<u64>(%[[VALUE___x_35]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), sub<u64, overflow=wrap>(read<u64>(%[[VALUE___x_35]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___hsb32_inline:[0-9]+]] @__hsb32_inline(%[[VALUE___x_36:[0-9]+]] __x: u32) -> bool [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return gt<u32>(xor<u32>(read<u32>(%[[VALUE___x_36]]), sub<u32, overflow=wrap>(read<u32>(%[[VALUE___x_36]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))), sub<u32, overflow=wrap>(read<u32>(%[[VALUE___x_36]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___bw64_inline:[0-9]+]] @__bw64_inline(%[[VALUE___x_37:[0-9]+]] __x: u64) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return sub<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(64)), call<u32, signature=fn(u64) -> u32>(%[[VALUE___clz64_inline]], read<u64>(%[[VALUE___x_37]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___bf64_inline:[0-9]+]] @__bf64_inline(%[[VALUE___x_38:[0-9]+]] __x: u64) -> u64 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE21:[0-9]+]]: u64 [synthetic];
// DEFAULT-NEXT:         if eq<u64>(read<u64>(%[[VALUE___x_38]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<u64>(%[[VALUE21]], reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<u64>(%[[VALUE21]], shl<u64, overflow=wrap, amount_out_of_range=ub>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), sub<u32, overflow=wrap>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___bw64_inline]], read<u64>(%[[VALUE___x_38]])), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))));
// DEFAULT-NEXT:         return read<u64>(%[[VALUE21]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___bc64_inline:[0-9]+]] @__bc64_inline(%[[VALUE___x_39:[0-9]+]] __x: u64) -> u64 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE22:[0-9]+]]: u64 [synthetic];
// DEFAULT-NEXT:         if le<u64>(read<u64>(%[[VALUE___x_39]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             write<u64>(%[[VALUE22]], reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<u64>(%[[VALUE22]], shl<u64, overflow=wrap, amount_out_of_range=ub>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), sub<u32, overflow=wrap>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___bw64_inline]], sub<u64, overflow=wrap>(read<u64>(%[[VALUE___x_39]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))));
// DEFAULT-NEXT:         return read<u64>(%[[VALUE22]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_c23_stdbit:[0-9]+]] @c23_stdbit() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_value:[0-9]+]] value: u32 [storage=automatic] = const<u32>(176);
// DEFAULT-NEXT:         let %[[VALUE23:[0-9]+]]: u32 [synthetic];
// DEFAULT-NEXT:         if eq<u64>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))))
// DEFAULT-NEXT:             write<u32>(%[[VALUE23]], call<u32, signature=fn(u64) -> u32>(%[[VALUE___ctz64_inline]], widen<u64, reason=arg>(read<u32>(%[[VALUE_value]]))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             let %[[VALUE24:[0-9]+]]: u32 [synthetic];
// DEFAULT-NEXT:             if eq<u64>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))
// DEFAULT-NEXT:                 write<u32>(%[[VALUE24]], call<u32, signature=fn(u32) -> u32>(%[[VALUE___ctz32_inline]], read<u32>(%[[VALUE_value]])));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 let %[[VALUE25:[0-9]+]]: u32 [synthetic];
// DEFAULT-NEXT:                 if eq<u64>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:                     write<u32>(%[[VALUE25]], call<u32, signature=fn(u16) -> u32>(%[[VALUE___ctz16_inline]], truncate<u16, reason=explicit, fits=unknown>(read<u32>(%[[VALUE_value]]))));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<u32>(%[[VALUE25]], call<u32, signature=fn(u8) -> u32>(%[[VALUE___ctz8_inline]], truncate<u8, reason=explicit, fits=unknown>(read<u32>(%[[VALUE_value]]))));
// DEFAULT-NEXT:                 write<u32>(%[[VALUE24]], read<u32>(%[[VALUE25]]));
// DEFAULT-NEXT:             write<u32>(%[[VALUE23]], read<u32>(%[[VALUE24]]));
// DEFAULT-NEXT:         let %[[VALUE26:[0-9]+]]: u32 [synthetic];
// DEFAULT-NEXT:         if eq<u64>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))))
// DEFAULT-NEXT:             write<u32>(%[[VALUE26]], call<u32, signature=fn(u64) -> u32>(%[[VALUE___flz64_inline]], widen<u64, reason=arg>(read<u32>(%[[VALUE_value]]))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             let %[[VALUE27:[0-9]+]]: u32 [synthetic];
// DEFAULT-NEXT:             if eq<u64>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))
// DEFAULT-NEXT:                 write<u32>(%[[VALUE27]], call<u32, signature=fn(u32) -> u32>(%[[VALUE___flz32_inline]], read<u32>(%[[VALUE_value]])));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 let %[[VALUE28:[0-9]+]]: u32 [synthetic];
// DEFAULT-NEXT:                 if eq<u64>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:                     write<u32>(%[[VALUE28]], call<u32, signature=fn(u16) -> u32>(%[[VALUE___flz16_inline]], truncate<u16, reason=explicit, fits=unknown>(read<u32>(%[[VALUE_value]]))));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<u32>(%[[VALUE28]], call<u32, signature=fn(u8) -> u32>(%[[VALUE___flz8_inline]], truncate<u8, reason=explicit, fits=unknown>(read<u32>(%[[VALUE_value]]))));
// DEFAULT-NEXT:                 write<u32>(%[[VALUE27]], read<u32>(%[[VALUE28]]));
// DEFAULT-NEXT:             write<u32>(%[[VALUE26]], read<u32>(%[[VALUE27]]));
// DEFAULT-NEXT:         let %[[VALUE29:[0-9]+]]: u32 [synthetic];
// DEFAULT-NEXT:         if eq<u64>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))))
// DEFAULT-NEXT:             write<u32>(%[[VALUE29]], call<u32, signature=fn(u64) -> u32>(%[[VALUE___flo64_inline]], widen<u64, reason=arg>(read<u32>(%[[VALUE_value]]))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             let %[[VALUE30:[0-9]+]]: u32 [synthetic];
// DEFAULT-NEXT:             if eq<u64>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))
// DEFAULT-NEXT:                 write<u32>(%[[VALUE30]], call<u32, signature=fn(u32) -> u32>(%[[VALUE___flo32_inline]], read<u32>(%[[VALUE_value]])));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 let %[[VALUE31:[0-9]+]]: u32 [synthetic];
// DEFAULT-NEXT:                 if eq<u64>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:                     write<u32>(%[[VALUE31]], call<u32, signature=fn(u16) -> u32>(%[[VALUE___flo16_inline]], truncate<u16, reason=explicit, fits=unknown>(read<u32>(%[[VALUE_value]]))));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<u32>(%[[VALUE31]], call<u32, signature=fn(u8) -> u32>(%[[VALUE___flo8_inline]], truncate<u8, reason=explicit, fits=unknown>(read<u32>(%[[VALUE_value]]))));
// DEFAULT-NEXT:                 write<u32>(%[[VALUE30]], read<u32>(%[[VALUE31]]));
// DEFAULT-NEXT:             write<u32>(%[[VALUE29]], read<u32>(%[[VALUE30]]));
// DEFAULT-NEXT:         let %[[VALUE32:[0-9]+]]: u32 [synthetic];
// DEFAULT-NEXT:         if eq<u64>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))))
// DEFAULT-NEXT:             write<u32>(%[[VALUE32]], call<u32, signature=fn(u64) -> u32>(%[[VALUE___ftz64_inline]], widen<u64, reason=arg>(read<u32>(%[[VALUE_value]]))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             let %[[VALUE33:[0-9]+]]: u32 [synthetic];
// DEFAULT-NEXT:             if eq<u64>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))
// DEFAULT-NEXT:                 write<u32>(%[[VALUE33]], call<u32, signature=fn(u32) -> u32>(%[[VALUE___ftz32_inline]], read<u32>(%[[VALUE_value]])));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 let %[[VALUE34:[0-9]+]]: u32 [synthetic];
// DEFAULT-NEXT:                 if eq<u64>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:                     write<u32>(%[[VALUE34]], call<u32, signature=fn(u16) -> u32>(%[[VALUE___ftz16_inline]], truncate<u16, reason=explicit, fits=unknown>(read<u32>(%[[VALUE_value]]))));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<u32>(%[[VALUE34]], call<u32, signature=fn(u8) -> u32>(%[[VALUE___ftz8_inline]], truncate<u8, reason=explicit, fits=unknown>(read<u32>(%[[VALUE_value]]))));
// DEFAULT-NEXT:                 write<u32>(%[[VALUE33]], read<u32>(%[[VALUE34]]));
// DEFAULT-NEXT:             write<u32>(%[[VALUE32]], read<u32>(%[[VALUE33]]));
// DEFAULT-NEXT:         let %[[VALUE35:[0-9]+]]: u32 [synthetic];
// DEFAULT-NEXT:         if eq<u64>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))))
// DEFAULT-NEXT:             write<u32>(%[[VALUE35]], call<u32, signature=fn(u64) -> u32>(%[[VALUE___fto64_inline]], widen<u64, reason=arg>(read<u32>(%[[VALUE_value]]))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             let %[[VALUE36:[0-9]+]]: u32 [synthetic];
// DEFAULT-NEXT:             if eq<u64>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))
// DEFAULT-NEXT:                 write<u32>(%[[VALUE36]], call<u32, signature=fn(u32) -> u32>(%[[VALUE___fto32_inline]], read<u32>(%[[VALUE_value]])));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 let %[[VALUE37:[0-9]+]]: u32 [synthetic];
// DEFAULT-NEXT:                 if eq<u64>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:                     write<u32>(%[[VALUE37]], call<u32, signature=fn(u16) -> u32>(%[[VALUE___fto16_inline]], truncate<u16, reason=explicit, fits=unknown>(read<u32>(%[[VALUE_value]]))));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<u32>(%[[VALUE37]], call<u32, signature=fn(u8) -> u32>(%[[VALUE___fto8_inline]], truncate<u8, reason=explicit, fits=unknown>(read<u32>(%[[VALUE_value]]))));
// DEFAULT-NEXT:                 write<u32>(%[[VALUE36]], read<u32>(%[[VALUE37]]));
// DEFAULT-NEXT:             write<u32>(%[[VALUE35]], read<u32>(%[[VALUE36]]));
// DEFAULT-NEXT:         let %[[VALUE38:[0-9]+]]: i32 [synthetic];
// DEFAULT-NEXT:         if le<u64>(const<u64>(4), const<u64>(4))
// DEFAULT-NEXT:             write<i32>(%[[VALUE38]], from_bool<i32, reason=promotion>(call<bool, signature=fn(u32) -> bool>(%[[VALUE___hsb32_inline]], const<u32>(64))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<i32>(%[[VALUE38]], from_bool<i32, reason=promotion>(call<bool, signature=fn(u64) -> bool>(%[[VALUE___hsb64_inline]], widen<u64, reason=arg>(const<u32>(64)))));
// DEFAULT-NEXT:         return reinterpret<i32, reason=explicit, fits=unknown>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(sub<u32, overflow=wrap>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___clz64_inline]], widen<u64, reason=arg>(read<u32>(%[[VALUE_value]]))), truncate<u32, reason=explicit, fits=unknown>(mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), sub<u64, overflow=wrap>(const<u64>(8), const<u64>(4))))), call<u32, signature=fn(u64) -> u32>(%[[VALUE___clo64_inline]], shl<u64, overflow=wrap, amount_out_of_range=ub>(widen<u64, reason=explicit>(read<u32>(%[[VALUE_value]])), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), sub<u64, overflow=wrap>(const<u64>(8), const<u64>(4)))))), read<u32>(%[[VALUE23]])), call<u32, signature=fn(u64) -> u32>(%[[VALUE___cto64_inline]], widen<u64, reason=arg>(read<u32>(%[[VALUE_value]])))), read<u32>(%[[VALUE26]])), read<u32>(%[[VALUE29]])), read<u32>(%[[VALUE32]])), read<u32>(%[[VALUE35]])), sub<u32, overflow=wrap>(call<u32, signature=fn(u64) -> u32>(%[[VALUE___cz64_inline]], widen<u64, reason=arg>(read<u32>(%[[VALUE_value]]))), truncate<u32, reason=explicit, fits=unknown>(mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), sub<u64, overflow=wrap>(const<u64>(8), const<u64>(4)))))), call<u32, signature=fn(u64) -> u32>(%[[VALUE___co64_inline]], widen<u64, reason=arg>(read<u32>(%[[VALUE_value]])))), reinterpret<u32, reason=usual_arith, fits=always>(from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(read<i32>(%[[VALUE38]]), const<i32>(0))))), call<u32, signature=fn(u64) -> u32>(%[[VALUE___bw64_inline]], widen<u64, reason=arg>(read<u32>(%[[VALUE_value]])))), truncate<u32, reason=explicit, fits=unknown>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___bf64_inline]], widen<u64, reason=arg>(read<u32>(%[[VALUE_value]]))))), truncate<u32, reason=explicit, fits=unknown>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___bc64_inline]], widen<u64, reason=arg>(read<u32>(%[[VALUE_value]]))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_c23_checked_arithmetic:[0-9]+]] @c23_checked_arithmetic() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_result:[0-9]+]] result: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_total:[0-9]+]] total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE39:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:         let %[[VALUE40:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE39]]), from_bool<i32, reason=promotion>(logical_and<bool>(not<bool>(overflow_add<bool>(const<i32>(20), const<i32>(22), deref(addr_of<ptr<i32>>(%[[VALUE_result]])))), eq<i32>(read<i32>(%[[VALUE_result]]), const<i32>(42)))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE40]]));
// DEFAULT-NEXT:         let %[[VALUE41:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:         let %[[VALUE42:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE41]]), from_bool<i32, reason=promotion>(logical_and<bool>(not<bool>(overflow_sub<bool>(const<i32>(50), const<i32>(8), deref(addr_of<ptr<i32>>(%[[VALUE_result]])))), eq<i32>(read<i32>(%[[VALUE_result]]), const<i32>(42)))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE42]]));
// DEFAULT-NEXT:         let %[[VALUE43:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:         let %[[VALUE44:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE43]]), from_bool<i32, reason=promotion>(logical_and<bool>(not<bool>(overflow_mul<bool>(const<i32>(6), const<i32>(7), deref(addr_of<ptr<i32>>(%[[VALUE_result]])))), eq<i32>(read<i32>(%[[VALUE_result]]), const<i32>(42)))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE44]]));
// DEFAULT-NEXT:         let %[[VALUE45:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:         let %[[VALUE46:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE45]]), from_bool<i32, reason=promotion>(overflow_add<bool>(const<i32>(2147483647), const<i32>(1), deref(addr_of<ptr<i32>>(%[[VALUE_result]])))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE46]]));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_c23_utf8:[0-9]+]] @c23_utf8() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_input_state:[0-9]+]] input_state: @type[[TYPE0]] [storage=automatic] = aggregate<@type[[TYPE0]], zero_fill=true>();
// DEFAULT-NEXT:         let %[[VALUE_output_state:[0-9]+]] output_state: @type[[TYPE0]] [storage=automatic] = aggregate<@type[[TYPE0]], zero_fill=true>();
// DEFAULT-NEXT:         let %[[VALUE_character:[0-9]+]] character: u8 [storage=automatic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE_output:[0-9]+]] output: array<i8, 16> [storage=automatic] [align=16] = aggregate<array<i8, 16>, zero_fill=true>();
// DEFAULT-NEXT:         let %[[VALUE_input_size:[0-9]+]] input_size: u64 [storage=automatic] = call<u64, signature=fn(ptr<u8>, ptr<const i8>, u64, ptr<@type[[TYPE0]]>) -> u64>(%[[VALUE_mbrtoc8]], addr_of<ptr<u8>>(%[[VALUE_character]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), addr_of<ptr<@type[[TYPE0]]>>(%[[VALUE_input_state]]));
// DEFAULT-NEXT:         let %[[VALUE_output_size:[0-9]+]] output_size: u64 [storage=automatic] = call<u64, signature=fn(ptr<i8>, u8, ptr<@type[[TYPE0]]>) -> u64>(%[[VALUE_c8rtomb]], array_decay<ptr<i8>, length=Some(16)>(%[[VALUE_output]]), read<u8>(%[[VALUE_character]]), addr_of<ptr<@type[[TYPE0]]>>(%[[VALUE_output_state]]));
// DEFAULT-NEXT:         let %[[VALUE_atomic_character:[0-9]+]] atomic_character: atomic u8 [storage=automatic] = read<u8>(%[[VALUE_character]]);
// DEFAULT-NEXT:         write<u8, atomic=seq_cst>(deref(addr_of<ptr<atomic u8>>(%[[VALUE_atomic_character]])), const<u8>(66));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(eq<u64>(read<u64>(%[[VALUE_input_size]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), from_bool<i32, reason=promotion>(eq<u64>(read<u64>(%[[VALUE_output_size]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))), from_bool<i32, reason=promotion>(eq<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(16)>(%[[VALUE_output]]), const<i32>(0))))), const<i32>(65)))), from_bool<i32, reason=promotion>(eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, atomic=seq_cst>(deref(addr_of<ptr<atomic u8>>(%[[VALUE_atomic_character]]))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(const<u8>(66)))))), from_bool<i32, reason=promotion>(gt<i32>(const<i32>(2), const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_c23_memory:[0-9]+]] @c23_memory() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_source:[0-9]+]] source: array<i8, 7> [storage=automatic] = code_units<array<i8, 7>>([97, 98, 99, 100, 101, 102, 0]);
// DEFAULT-NEXT:         let %[[VALUE_destination:[0-9]+]] destination: array<i8, 8> [storage=automatic] = aggregate<array<i8, 8>, zero_fill=true>();
// DEFAULT-NEXT:         let %[[VALUE_secret:[0-9]+]] secret: array<i8, 7> [storage=automatic] = code_units<array<i8, 7>>([115, 101, 99, 114, 101, 116, 0]);
// DEFAULT-NEXT:         let %[[VALUE_first_copy:[0-9]+]] first_copy: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_second_copy:[0-9]+]] second_copy: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_phrase:[0-9]+]] phrase: ptr<const i8> [storage=automatic] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(12)>(%[[VALUE_str_2]]));
// DEFAULT-NEXT:         let %[[VALUE_mutable_phrase:[0-9]+]] mutable_phrase: array<i8, 12> [storage=automatic] = code_units<array<i8, 12>>([104, 101, 108, 108, 111, 32, 119, 111, 114, 108, 100, 0]);
// DEFAULT-NEXT:         let %[[VALUE_stop:[0-9]+]] stop: ptr<void> [storage=automatic] = call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memccpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_destination]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%[[VALUE_source]])), const<i32>(99), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6))));
// DEFAULT-NEXT:         let %[[VALUE_total_2:[0-9]+]] total: i32 [storage=automatic] = from_bool<i32, reason=assign>(logical_and<bool>(eq<ptr<void>>(read<ptr<void>>(%[[VALUE_stop]]), pointer_cast<ptr<void>, reason=usual_arith>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_destination]]), const<i32>(3)))), eq<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_destination]]), const<i32>(2))))), const<i32>(99))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset_explicit]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%[[VALUE_secret]])), const<i32>(0), const<u64>(7));
// DEFAULT-NEXT:         let %[[VALUE47:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_2]]);
// DEFAULT-NEXT:         let %[[VALUE48:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE47]]), from_bool<i32, reason=promotion>(logical_and<bool>(eq<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(7)>(%[[VALUE_secret]]), const<i32>(0))))), const<i32>(0)), eq<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(7)>(%[[VALUE_secret]]), const<i32>(5))))), const<i32>(0)))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_2]], read<i32>(%[[VALUE48]]));
// DEFAULT-NEXT:         write<ptr<i8>>(%[[VALUE_first_copy]], call<ptr<i8>, signature=fn(ptr<const i8>) -> ptr<i8>>(%[[VALUE_strdup]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_3]]))));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<const i8>) -> ptr<i8>>(%[[VALUE_strdup]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_3]])));
// DEFAULT-NEXT:         write<ptr<i8>>(%[[VALUE_second_copy]], call<ptr<i8>, signature=fn(ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strndup]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_str_4]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3)))));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strndup]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_str_4]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))));
// DEFAULT-NEXT:         let %[[VALUE49:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_2]]);
// DEFAULT-NEXT:         let %[[VALUE50:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE49]]), from_bool<i32, reason=promotion>(logical_and<bool>(ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_first_copy]]), null<ptr<i8>>), eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%[[VALUE_strcmp]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_first_copy]])), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_5]]))), const<i32>(0)))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_2]], read<i32>(%[[VALUE50]]));
// DEFAULT-NEXT:         let %[[VALUE51:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_2]]);
// DEFAULT-NEXT:         let %[[VALUE52:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE51]]), from_bool<i32, reason=promotion>(logical_and<bool>(ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_second_copy]]), null<ptr<i8>>), eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%[[VALUE_strcmp]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_second_copy]])), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_6]]))), const<i32>(0)))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_2]], read<i32>(%[[VALUE52]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_free]], pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%[[VALUE_first_copy]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_free]], pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%[[VALUE_second_copy]])));
// DEFAULT-NEXT:         let %[[VALUE_const_hit:[0-9]+]] const_hit: ptr<const i8> [storage=automatic] = pointer_cast<ptr<const i8>, reason=explicit>(call<ptr<i8>, signature=fn(ptr<const i8>, i32) -> ptr<i8>>(%[[VALUE_strchr]], read<ptr<const i8>>(%[[VALUE_phrase]]), const<i32>(119)));
// DEFAULT-NEXT:         let %[[VALUE_mut_hit:[0-9]+]] mut_hit: ptr<i8> [storage=automatic] = call<ptr<i8>, signature=fn(ptr<const i8>, i32) -> ptr<i8>>(%[[VALUE_strchr]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(12)>(%[[VALUE_mutable_phrase]])), const<i32>(119));
// DEFAULT-NEXT:         let %[[VALUE53:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_2]]);
// DEFAULT-NEXT:         let %[[VALUE54:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE53]]), from_bool<i32, reason=promotion>(logical_and<bool>(ne<ptr<const i8>>(read<ptr<const i8>>(%[[VALUE_const_hit]]), null<ptr<const i8>>), ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_mut_hit]]), null<ptr<i8>>))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_2]], read<i32>(%[[VALUE54]]));
// DEFAULT-NEXT:         let %[[VALUE55:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_2]]);
// DEFAULT-NEXT:         let %[[VALUE56:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE55]]), from_bool<i32, reason=promotion>(ne<ptr<const void>>(pointer_cast<ptr<const void>, reason=explicit>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i8>>(%[[VALUE_phrase]])), const<i32>(111), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(11))))), null<ptr<const void>>)));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_2]], read<i32>(%[[VALUE56]]));
// DEFAULT-NEXT:         let %[[VALUE57:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_2]]);
// DEFAULT-NEXT:         let %[[VALUE58:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE57]]), from_bool<i32, reason=promotion>(ne<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(12)>(%[[VALUE_mutable_phrase]])), const<i32>(111), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(11)))), null<ptr<void>>)));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_2]], read<i32>(%[[VALUE58]]));
// DEFAULT-NEXT:         let %[[VALUE59:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_2]]);
// DEFAULT-NEXT:         let %[[VALUE60:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE59]]), from_bool<i32, reason=promotion>(ne<ptr<const i8>>(pointer_cast<ptr<const i8>, reason=explicit>(call<ptr<i8>, signature=fn(ptr<const i8>, ptr<const i8>) -> ptr<i8>>(%[[VALUE_strstr]], read<ptr<const i8>>(%[[VALUE_phrase]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_str_7]])))), null<ptr<const i8>>)));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_2]], read<i32>(%[[VALUE60]]));
// DEFAULT-NEXT:         let %[[VALUE61:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_2]]);
// DEFAULT-NEXT:         let %[[VALUE62:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE61]]), from_bool<i32, reason=promotion>(ne<ptr<i8>>(call<ptr<i8>, signature=fn(ptr<const i8>, ptr<const i8>) -> ptr<i8>>(%[[VALUE_strstr]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(12)>(%[[VALUE_mutable_phrase]])), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_str_8]]))), null<ptr<i8>>)));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_2]], read<i32>(%[[VALUE62]]));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_total_2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_c23_time:[0-9]+]] @c23_time() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_timestamp:[0-9]+]] timestamp: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE_utc:[0-9]+]] utc: @type[[TYPE_tm]] [storage=automatic] = aggregate<@type[[TYPE_tm]], zero_fill=true>();
// DEFAULT-NEXT:         let %[[VALUE_local:[0-9]+]] local: @type[[TYPE_tm]] [storage=automatic] = aggregate<@type[[TYPE_tm]], zero_fill=true>();
// DEFAULT-NEXT:         let %[[VALUE_resolution:[0-9]+]] resolution: @type[[TYPE_timespec]] [storage=automatic] = aggregate<@type[[TYPE_timespec]], zero_fill=true>();
// DEFAULT-NEXT:         let %[[VALUE_month:[0-9]+]] month: array<i8, 32> [storage=automatic] [align=16] = aggregate<array<i8, 32>, zero_fill=true>();
// DEFAULT-NEXT:         let %[[VALUE_wide_month:[0-9]+]] wide_month: array<i32, 32> [storage=automatic] [align=16] = aggregate<array<i32, 32>, zero_fill=true>();
// DEFAULT-NEXT:         let %[[VALUE_total_3:[0-9]+]] total: i32 [storage=automatic] = from_bool<i32, reason=assign>(eq<ptr<@type[[TYPE_tm]]>>(call<ptr<@type[[TYPE_tm]]>, signature=fn(ptr<const i64>, ptr<@type[[TYPE_tm]]>) -> ptr<@type[[TYPE_tm]]>>(%[[VALUE_gmtime_r]], pointer_cast<ptr<const i64>, reason=arg>(addr_of<ptr<i64>>(%[[VALUE_timestamp]])), addr_of<ptr<@type[[TYPE_tm]]>>(%[[VALUE_utc]])), addr_of<ptr<@type[[TYPE_tm]]>>(%[[VALUE_utc]])));
// DEFAULT-NEXT:         let %[[VALUE63:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_3]]);
// DEFAULT-NEXT:         let %[[VALUE64:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE63]]), from_bool<i32, reason=promotion>(eq<ptr<@type[[TYPE_tm]]>>(call<ptr<@type[[TYPE_tm]]>, signature=fn(ptr<const i64>, ptr<@type[[TYPE_tm]]>) -> ptr<@type[[TYPE_tm]]>>(%[[VALUE_localtime_r]], pointer_cast<ptr<const i64>, reason=arg>(addr_of<ptr<i64>>(%[[VALUE_timestamp]])), addr_of<ptr<@type[[TYPE_tm]]>>(%[[VALUE_local]])), addr_of<ptr<@type[[TYPE_tm]]>>(%[[VALUE_local]]))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_3]], read<i32>(%[[VALUE64]]));
// DEFAULT-NEXT:         let %[[VALUE65:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_3]]);
// DEFAULT-NEXT:         let %[[VALUE66:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE65]]), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<@type[[TYPE_timespec]]>, i32) -> i32>(%[[VALUE_timespec_getres]], addr_of<ptr<@type[[TYPE_timespec]]>>(%[[VALUE_resolution]]), const<i32>(1)), const<i32>(1))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_3]], read<i32>(%[[VALUE66]]));
// DEFAULT-NEXT:         let %[[VALUE67:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_3]]);
// DEFAULT-NEXT:         let %[[VALUE68:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE67]]), from_bool<i32, reason=promotion>(logical_or<bool>(gt<i64>(read<i64>(field0(%[[VALUE_resolution]])), widen<i64, reason=usual_arith>(const<i32>(0))), gt<i64>(read<i64>(field1(%[[VALUE_resolution]])), widen<i64, reason=usual_arith>(const<i32>(0))))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_3]], read<i32>(%[[VALUE68]]));
// DEFAULT-NEXT:         let %[[VALUE69:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_3]]);
// DEFAULT-NEXT:         let %[[VALUE70:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE69]]), from_bool<i32, reason=promotion>(eq<i64>(call<i64, signature=fn(ptr<@type[[TYPE_tm]]>) -> i64>(%[[VALUE_timegm]], addr_of<ptr<@type[[TYPE_tm]]>>(%[[VALUE_utc]])), widen<i64, reason=usual_arith>(const<i32>(0)))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_3]], read<i32>(%[[VALUE70]]));
// DEFAULT-NEXT:         let %[[VALUE71:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_3]]);
// DEFAULT-NEXT:         let %[[VALUE72:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE71]]), from_bool<i32, reason=promotion>(eq<u64>(call<u64, signature=fn(ptr<i8>, u64, ptr<const i8>, ptr<const @type[[TYPE_tm]]>) -> u64>(%[[VALUE_strftime]], array_decay<ptr<i8>, length=Some(32)>(%[[VALUE_month]]), const<u64>(32), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_9]])), pointer_cast<ptr<const @type[[TYPE_tm]]>, reason=arg>(addr_of<ptr<@type[[TYPE_tm]]>>(%[[VALUE_utc]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(7))))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_3]], read<i32>(%[[VALUE72]]));
// DEFAULT-NEXT:         let %[[VALUE73:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_3]]);
// DEFAULT-NEXT:         let %[[VALUE74:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE73]]), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%[[VALUE_strcmp]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(32)>(%[[VALUE_month]])), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_str_10]]))), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_3]], read<i32>(%[[VALUE74]]));
// DEFAULT-NEXT:         let %[[VALUE75:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_3]]);
// DEFAULT-NEXT:         let %[[VALUE76:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE75]]), from_bool<i32, reason=promotion>(eq<u64>(call<u64, signature=fn(ptr<i32>, u64, ptr<const i32>, ptr<const @type[[TYPE_tm]]>) -> u64>(%[[VALUE_wcsftime]], array_decay<ptr<i32>, length=Some(32)>(%[[VALUE_wide_month]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(32))), pointer_cast<ptr<const i32>, reason=arg>(array_decay<ptr<i32>, length=Some(4)>(%[[VALUE_str_11]])), pointer_cast<ptr<const @type[[TYPE_tm]]>, reason=arg>(addr_of<ptr<@type[[TYPE_tm]]>>(%[[VALUE_utc]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(7))))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_3]], read<i32>(%[[VALUE76]]));
// DEFAULT-NEXT:         let %[[VALUE77:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_3]]);
// DEFAULT-NEXT:         let %[[VALUE78:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE77]]), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const i32>, ptr<const i32>) -> i32>(%[[VALUE_wcscmp]], pointer_cast<ptr<const i32>, reason=arg>(array_decay<ptr<i32>, length=Some(32)>(%[[VALUE_wide_month]])), pointer_cast<ptr<const i32>, reason=arg>(array_decay<ptr<i32>, length=Some(8)>(%[[VALUE_str_12]]))), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_3]], read<i32>(%[[VALUE78]]));
// DEFAULT-NEXT:         let %[[VALUE_const_month:[0-9]+]] const_month: ptr<const i32> [storage=automatic] = pointer_cast<ptr<const i32>, reason=assign>(array_decay<ptr<i32>, length=Some(32)>(%[[VALUE_wide_month]]));
// DEFAULT-NEXT:         let %[[VALUE79:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_3]]);
// DEFAULT-NEXT:         let %[[VALUE80:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE79]]), from_bool<i32, reason=promotion>(ne<ptr<const i32>>(pointer_cast<ptr<const i32>, reason=explicit>(call<ptr<i32>, signature=fn(ptr<const i32>, i32) -> ptr<i32>>(%[[VALUE_wcschr]], read<ptr<const i32>>(%[[VALUE_const_month]]), const<i32>(110))), null<ptr<const i32>>)));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_3]], read<i32>(%[[VALUE80]]));
// DEFAULT-NEXT:         let %[[VALUE81:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_3]]);
// DEFAULT-NEXT:         let %[[VALUE82:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE81]]), from_bool<i32, reason=promotion>(ne<ptr<i32>>(call<ptr<i32>, signature=fn(ptr<const i32>, i32) -> ptr<i32>>(%[[VALUE_wcschr]], pointer_cast<ptr<const i32>, reason=arg>(array_decay<ptr<i32>, length=Some(32)>(%[[VALUE_wide_month]])), const<i32>(110)), null<ptr<i32>>)));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_3]], read<i32>(%[[VALUE82]]));
// DEFAULT-NEXT:         let %[[VALUE83:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_3]]);
// DEFAULT-NEXT:         let %[[VALUE84:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE83]]), from_bool<i32, reason=promotion>(ne<ptr<const i32>>(pointer_cast<ptr<const i32>, reason=explicit>(call<ptr<i32>, signature=fn(ptr<const i32>, ptr<const i32>) -> ptr<i32>>(%[[VALUE_wcsstr]], read<ptr<const i32>>(%[[VALUE_const_month]]), pointer_cast<ptr<const i32>, reason=arg>(array_decay<ptr<i32>, length=Some(4)>(%[[VALUE_str_13]])))), null<ptr<const i32>>)));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_3]], read<i32>(%[[VALUE84]]));
// DEFAULT-NEXT:         let %[[VALUE85:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_3]]);
// DEFAULT-NEXT:         let %[[VALUE86:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE85]]), from_bool<i32, reason=promotion>(ne<ptr<i32>>(call<ptr<i32>, signature=fn(ptr<const i32>, ptr<const i32>) -> ptr<i32>>(%[[VALUE_wcsstr]], pointer_cast<ptr<const i32>, reason=arg>(array_decay<ptr<i32>, length=Some(32)>(%[[VALUE_wide_month]])), pointer_cast<ptr<const i32>, reason=arg>(array_decay<ptr<i32>, length=Some(4)>(%[[VALUE_str_14]]))), null<ptr<i32>>)));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_3]], read<i32>(%[[VALUE86]]));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_total_3]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_c23_io:[0-9]+]] @c23_io() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_output_2:[0-9]+]] output: array<i8, 64> [storage=automatic] [align=16] = aggregate<array<i8, 64>, zero_fill=true>();
// DEFAULT-NEXT:         let %[[VALUE_float_output:[0-9]+]] float_output: array<i8, 16> [storage=automatic] [align=16] = aggregate<array<i8, 16>, zero_fill=true>();
// DEFAULT-NEXT:         let %[[VALUE_double_output:[0-9]+]] double_output: array<i8, 16> [storage=automatic] [align=16] = aggregate<array<i8, 16>, zero_fill=true>();
// DEFAULT-NEXT:         let %[[VALUE_long_double_output:[0-9]+]] long_double_output: array<i8, 16> [storage=automatic] [align=16] = aggregate<array<i8, 16>, zero_fill=true>();
// DEFAULT-NEXT:         let %[[VALUE_binary_value:[0-9]+]] binary_value: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE_exact_value:[0-9]+]] exact_value: u16 [storage=automatic] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE_fast_value:[0-9]+]] fast_value: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE_written:[0-9]+]] written: i32 [storage=automatic] = call<i32, signature=fn(ptr<i8>, u64, ptr<const i8>, ...) -> i32>(%[[VALUE_snprintf]], array_decay<ptr<i8>, length=Some(64)>(%[[VALUE_output_2]]), const<u64>(64), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%[[VALUE_str_15]])), const<u32>(13), reinterpret<i32, reason=vararg, fits=unknown>(widen<u32, reason=vararg>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(21))))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(34))));
// DEFAULT-NEXT:         let %[[VALUE_scanned:[0-9]+]] scanned: i32 [storage=automatic] = call<i32, signature=fn(ptr<const i8>, ptr<const i8>, ...) -> i32>(%[[VALUE_sscanf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_16]])), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%[[VALUE_str_17]])), addr_of<ptr<u32>>(%[[VALUE_binary_value]]), addr_of<ptr<u16>>(%[[VALUE_exact_value]]), addr_of<ptr<u64>>(%[[VALUE_fast_value]]));
// DEFAULT-NEXT:         let %[[VALUE_floating_written:[0-9]+]] floating_written: i32 [storage=automatic] = add<i32, overflow=ub>(add<i32, overflow=ub>(call<i32, signature=fn(ptr<i8>, u64, ptr<const i8>, f32) -> i32>(%[[VALUE_strfromf]], array_decay<ptr<i8>, length=Some(16)>(%[[VALUE_float_output]]), const<u64>(16), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str_18]])), const<f32>(1.5)), call<i32, signature=fn(ptr<i8>, u64, ptr<const i8>, f64) -> i32>(%[[VALUE_strfromd]], array_decay<ptr<i8>, length=Some(16)>(%[[VALUE_double_output]]), const<u64>(16), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str_19]])), const<f64>(2.5))), call<i32, signature=fn(ptr<i8>, u64, ptr<const i8>, f80) -> i32>(%[[VALUE_strfroml]], array_decay<ptr<i8>, length=Some(16)>(%[[VALUE_long_double_output]]), const<u64>(16), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str_20]])), const<f80>(3.5)));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(%[[VALUE_written]]), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%[[VALUE_strcmp]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(64)>(%[[VALUE_output_2]])), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_21]]))), const<i32>(0)))), read<i32>(%[[VALUE_scanned]])), from_bool<i32, reason=promotion>(eq<u32>(read<u32>(%[[VALUE_binary_value]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(11))))), from_bool<i32, reason=promotion>(eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_exact_value]]))), const<i32>(55)))), from_bool<i32, reason=promotion>(eq<u64>(read<u64>(%[[VALUE_fast_value]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(89)))))), read<i32>(%[[VALUE_floating_written]])), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%[[VALUE_strcmp]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%[[VALUE_float_output]])), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_22]]))), const<i32>(0)))), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%[[VALUE_strcmp]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%[[VALUE_double_output]])), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_23]]))), const<i32>(0)))), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%[[VALUE_strcmp]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%[[VALUE_long_double_output]])), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_24]]))), const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_c23_limits:[0-9]+]] @c23_limits() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_integer_widths:[0-9]+]] integer_widths: i32 [storage=automatic] = add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(const<i32>(1), const<i32>(8)), const<i32>(8)), const<i32>(8)), const<i32>(16)), const<i32>(16)), const<i32>(32)), const<i32>(32)), const<i32>(64)), const<i32>(64)), const<i32>(64)), const<i32>(64)), const<i32>(64)), const<i32>(64));
// DEFAULT-NEXT:         let %[[VALUE_floating_limits:[0-9]+]] floating_limits: i32 [storage=automatic] = add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(gt<f32, exceptions=ignore>(const<f32>(1e-45), const<f32>(0.0))), from_bool<i32, reason=promotion>(gt<f64, exceptions=ignore>(const<f64>(5e-324), const<f64>(0.0)))), from_bool<i32, reason=promotion>(gt<f80, exceptions=ignore>(const<f80>(3.64519953188247460253E-4951), const<f80>(0)))), from_bool<i32, reason=promotion>(le<f32, exceptions=ignore>(const<f32>(3.4028235e38), const<f32>(3.4028235e38)))), from_bool<i32, reason=promotion>(le<f64, exceptions=ignore>(const<f64>(1.7976931348623157e308), const<f64>(1.7976931348623157e308)))), from_bool<i32, reason=promotion>(le<f80, exceptions=ignore>(const<f80>(1.18973149535723176502E+4932), const<f80>(1.18973149535723176502E+4932)))), from_bool<i32, reason=promotion>(ge<i32>(const<i32>(1), neg<i32, overflow=ub>(const<i32>(1))))), from_bool<i32, reason=promotion>(ge<i32>(const<i32>(1), neg<i32, overflow=ub>(const<i32>(1))))), from_bool<i32, reason=promotion>(ge<i32>(const<i32>(1), neg<i32, overflow=ub>(const<i32>(1))))), from_bool<i32, reason=promotion>(eq<u64>(const<u64>(4), const<u64>(4)))), from_bool<i32, reason=promotion>(eq<u64>(const<u64>(8), const<u64>(8)))), from_bool<i32, reason=promotion>(eq<u64>(const<u64>(16), const<u64>(16))));
// DEFAULT-NEXT:         let %[[VALUE_header_versions:[0-9]+]] header_versions: i32 [storage=automatic] = add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(eq<i64>(const<i64>(202311), const<i64>(202311))), from_bool<i32, reason=promotion>(eq<i64>(const<i64>(202311), const<i64>(202311)))), from_bool<i32, reason=promotion>(eq<i64>(const<i64>(202311), const<i64>(202311)))), from_bool<i32, reason=promotion>(eq<i64>(const<i64>(202311), const<i64>(202311)))), from_bool<i32, reason=promotion>(eq<i64>(const<i64>(202311), const<i64>(202311)))), from_bool<i32, reason=promotion>(eq<i64>(const<i64>(202311), const<i64>(202311)))), const<i32>(1));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(%[[VALUE_integer_widths]]), read<i32>(%[[VALUE_floating_limits]])), read<i32>(%[[VALUE_header_versions]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(19)>(%[[VALUE_str_25]])), call<i32, signature=fn() -> i32>(%[[VALUE_c23_stdbit]]), call<i32, signature=fn() -> i32>(%[[VALUE_c23_checked_arithmetic]]), call<i32, signature=fn() -> i32>(%[[VALUE_c23_utf8]]), call<i32, signature=fn() -> i32>(%[[VALUE_c23_memory]]), call<i32, signature=fn() -> i32>(%[[VALUE_c23_time]]), add<i32, overflow=ub>(call<i32, signature=fn() -> i32>(%[[VALUE_c23_io]]), call<i32, signature=fn() -> i32>(%[[VALUE_c23_limits]])));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
