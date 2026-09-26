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
// DEFAULT-NEXT:     type @type0 size_t = u64;
// DEFAULT-NEXT:     type @type1 wchar_t = i32;
// DEFAULT-NEXT:     type @type2 __uint8_t = u8;
// DEFAULT-NEXT:     type @type3 __uint16_t = u16;
// DEFAULT-NEXT:     type @type4 __uint32_t = u32;
// DEFAULT-NEXT:     type @type5 __uint64_t = u64;
// DEFAULT-NEXT:     type @type6 __time_t = i64;
// DEFAULT-NEXT:     type @type7 __syscall_slong_t = i64;
// DEFAULT-NEXT:     type @type8 uint8_t = u8;
// DEFAULT-NEXT:     type @type9 uint16_t = u16;
// DEFAULT-NEXT:     type @type10 uint32_t = u32;
// DEFAULT-NEXT:     type @type11 uint64_t = u64;
// DEFAULT-NEXT:     type @type12 uint_fast16_t = u64;
// DEFAULT-NEXT:     type @type13 atomic_char8_t = u8;
// DEFAULT-NEXT:     type @type14 = struct {
// DEFAULT-NEXT:         field0 __count: i32;
// DEFAULT-NEXT:         field1 __value: @type15;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type15 = union {
// DEFAULT-NEXT:         field0 __wch: u32;
// DEFAULT-NEXT:         field1 __wchb: array<i8, 4>;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type16 __mbstate_t = @type14;
// DEFAULT-NEXT:     type @type17 time_t = i64;
// DEFAULT-NEXT:     type @type18 timespec = struct {
// DEFAULT-NEXT:         field0 tv_sec: i64;
// DEFAULT-NEXT:         field1 tv_nsec: i64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type19 tm = struct {
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
// DEFAULT-NEXT:     type @type20 mbstate_t = @type14;
// DEFAULT-NEXT:     type @type21 char8_t = u8;
// DEFAULT-NEXT:     global %244 .str244: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([65, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %245 .str245: array<i8, 12> [storage=static] = code_units<array<i8, 12>>([104, 101, 108, 108, 111, 32, 119, 111, 114, 108, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %246 .str246: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([99, 50, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %247 .str247: array<i8, 8> [storage=static] = code_units<array<i8, 8>>([108, 105, 98, 114, 97, 114, 121, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %248 .str248: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([99, 50, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %249 .str249: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([108, 105, 98, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %250 .str250: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([119, 111, 114, 108, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %251 .str251: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([119, 111, 114, 108, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %252 .str252: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 79, 66, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %253 .str253: array<i8, 8> [storage=static] = code_units<array<i8, 8>>([74, 97, 110, 117, 97, 114, 121, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %254 .str254: array<i32, 4> [storage=static] = code_units<array<i32, 4>>([37, 79, 66, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %255 .str255: array<i32, 8> [storage=static] = code_units<array<i32, 8>>([74, 97, 110, 117, 97, 114, 121, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %256 .str256: array<i32, 4> [storage=static] = code_units<array<i32, 4>>([74, 97, 110, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %257 .str257: array<i32, 4> [storage=static] = code_units<array<i32, 4>>([74, 97, 110, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %258 .str258: array<i8, 16> [storage=static] = code_units<array<i8, 16>>([37, 98, 32, 37, 119, 49, 54, 117, 32, 37, 119, 102, 49, 54, 117, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %259 .str259: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([49, 48, 49, 49, 32, 53, 53, 32, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %260 .str260: array<i8, 16> [storage=static] = code_units<array<i8, 16>>([37, 98, 32, 37, 119, 49, 54, 117, 32, 37, 119, 102, 49, 54, 117, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %261 .str261: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([37, 46, 49, 102, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %262 .str262: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([37, 46, 49, 102, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %263 .str263: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([37, 46, 49, 102, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %264 .str264: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([49, 49, 48, 49, 32, 50, 49, 32, 51, 52, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %265 .str265: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([49, 46, 53, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %266 .str266: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([50, 46, 53, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %267 .str267: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([51, 46, 53, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %268 .str268: array<i8, 19> [storage=static] = code_units<array<i8, 19>>([37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %17 @printf(%176 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %18 @snprintf(%177 __s: ptr<i8> [restrict], %178 __maxlen: u64, %179 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %19 @sscanf(%180 __s: ptr<const i8> [restrict], %181 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external] [asm_name="__isoc23_sscanf"];
// DEFAULT-NEXT:     fn %20 @strfromd(%184 __dest: ptr<i8>, %185 __size: u64, %186 __format: ptr<const i8>, %187 __f: f64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %21 @strfromf(%188 __dest: ptr<i8>, %189 __size: u64, %190 __format: ptr<const i8>, %191 __f: f32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %22 @strfroml(%192 __dest: ptr<i8>, %193 __size: u64, %194 __format: ptr<const i8>, %195 __f: f80) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %25 @free(%196 __ptr: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %26 @memccpy(%197 __dest: ptr<void> [restrict], %198 __src: ptr<const void> [restrict], %199 __c: i32, %200 __n: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %27 @memset_explicit(%201 __s: ptr<void>, %202 __c: i32, %203 __n: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %28 @memchr(%204 __s: ptr<const void>, %205 __c: i32, %206 __n: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %29 @strcmp(%207 __s1: ptr<const i8>, %208 __s2: ptr<const i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %30 @strdup(%209 __s: ptr<const i8>) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %31 @strndup(%210 __string: ptr<const i8>, %211 __n: u64) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %32 @strchr(%212 __s: ptr<const i8>, %213 __c: i32) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %33 @strstr(%214 __haystack: ptr<const i8>, %215 __needle: ptr<const i8>) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %35 @strftime(%216 __s: ptr<i8> [restrict], %217 __maxsize: u64, %218 __format: ptr<const i8> [restrict], %219 __tp: ptr<const @type19> [restrict]) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %36 @gmtime_r(%220 __timer: ptr<const i64> [restrict], %221 __tp: ptr<@type19> [restrict]) -> ptr<@type19> [linkage=external];
// DEFAULT-NEXT:     fn %37 @localtime_r(%222 __timer: ptr<const i64> [restrict], %223 __tp: ptr<@type19> [restrict]) -> ptr<@type19> [linkage=external];
// DEFAULT-NEXT:     fn %38 @timegm(%224 __tp: ptr<@type19>) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %39 @timespec_getres(%225 __ts: ptr<@type18>, %226 __base: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %42 @mbrtoc8(%227 __pc8: ptr<u8> [restrict], %228 __s: ptr<const i8> [restrict], %229 __n: u64, %230 __p: ptr<@type14> [restrict]) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %43 @c8rtomb(%231 __s: ptr<i8> [restrict], %232 __c8: u8, %233 __ps: ptr<@type14> [restrict]) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %44 @wcscmp(%234 __s1: ptr<const i32>, %235 __s2: ptr<const i32>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %45 @wcschr(%236 __wcs: ptr<const i32>, %237 __wc: i32) -> ptr<i32> [linkage=external];
// DEFAULT-NEXT:     fn %46 @wcsstr(%238 __haystack: ptr<const i32>, %239 __needle: ptr<const i32>) -> ptr<i32> [linkage=external];
// DEFAULT-NEXT:     fn %47 @wcsftime(%240 __s: ptr<i32> [restrict], %241 __maxsize: u64, %242 __format: ptr<const i32> [restrict], %243 __tp: ptr<const @type19> [restrict]) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %48 @__clz64_inline(%49 __x: u64) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %269: u32 [synthetic];
// DEFAULT-NEXT:         if eq<u64>(read<u64>(%49), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<u32>(%269, const<u32>(64));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<u32>(%269, reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn(u64) -> i32>(__builtin_clzll, read<u64>(%49))));
// DEFAULT-NEXT:         return read<u32>(%269);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %50 @__clz32_inline(%51 __x: u32) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %270: u32 [synthetic];
// DEFAULT-NEXT:         if eq<u32>(read<u32>(%51), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:             write<u32>(%270, const<u32>(32));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<u32>(%270, reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn(u32) -> i32>(__builtin_clz, read<u32>(%51))));
// DEFAULT-NEXT:         return read<u32>(%270);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %52 @__clz16_inline(%53 __x: u16) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return sub<u32, overflow=wrap>(call<u32, signature=fn(u32) -> u32>(%50, widen<u32, reason=arg>(read<u16>(%53))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(16)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %54 @__clz8_inline(%55 __x: u8) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return sub<u32, overflow=wrap>(call<u32, signature=fn(u32) -> u32>(%50, widen<u32, reason=arg>(read<u8>(%55))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(24)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %56 @__clo64_inline(%57 __x: u64) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<u32, signature=fn(u64) -> u32>(%48, not<u64>(read<u64>(%57)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %58 @__clo32_inline(%59 __x: u32) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<u32, signature=fn(u32) -> u32>(%50, not<u32>(read<u32>(%59)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %60 @__clo16_inline(%61 __x: u16) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<u32, signature=fn(u16) -> u32>(%52, reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(not<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%61)))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %62 @__clo8_inline(%63 __x: u8) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<u32, signature=fn(u8) -> u32>(%54, reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(not<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%63)))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %64 @__ctz64_inline(%65 __x: u64) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %271: u32 [synthetic];
// DEFAULT-NEXT:         if eq<u64>(read<u64>(%65), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<u32>(%271, const<u32>(64));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<u32>(%271, reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn(u64) -> i32>(__builtin_ctzll, read<u64>(%65))));
// DEFAULT-NEXT:         return read<u32>(%271);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %66 @__ctz32_inline(%67 __x: u32) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %272: u32 [synthetic];
// DEFAULT-NEXT:         if eq<u32>(read<u32>(%67), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:             write<u32>(%272, const<u32>(32));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<u32>(%272, reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn(u32) -> i32>(__builtin_ctz, read<u32>(%67))));
// DEFAULT-NEXT:         return read<u32>(%272);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %68 @__ctz16_inline(%69 __x: u16) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %273: u32 [synthetic];
// DEFAULT-NEXT:         if eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%69))), const<i32>(0))
// DEFAULT-NEXT:             write<u32>(%273, const<u32>(16));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<u32>(%273, reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn(u32) -> i32>(__builtin_ctz, widen<u32, reason=arg>(read<u16>(%69)))));
// DEFAULT-NEXT:         return read<u32>(%273);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %70 @__ctz8_inline(%71 __x: u8) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %274: u32 [synthetic];
// DEFAULT-NEXT:         if eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%71))), const<i32>(0))
// DEFAULT-NEXT:             write<u32>(%274, const<u32>(8));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<u32>(%274, reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn(u32) -> i32>(__builtin_ctz, widen<u32, reason=arg>(read<u8>(%71)))));
// DEFAULT-NEXT:         return read<u32>(%274);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %72 @__cto64_inline(%73 __x: u64) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<u32, signature=fn(u64) -> u32>(%64, not<u64>(read<u64>(%73)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %74 @__cto32_inline(%75 __x: u32) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<u32, signature=fn(u32) -> u32>(%66, not<u32>(read<u32>(%75)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %76 @__cto16_inline(%77 __x: u16) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<u32, signature=fn(u16) -> u32>(%68, reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(not<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%77)))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %78 @__cto8_inline(%79 __x: u8) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<u32, signature=fn(u8) -> u32>(%70, reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(not<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%79)))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %80 @__flz64_inline(%81 __x: u64) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %275: u32 [synthetic];
// DEFAULT-NEXT:         if eq<u64>(read<u64>(%81), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:             write<u32>(%275, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<u32>(%275, add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)), call<u32, signature=fn(u64) -> u32>(%56, read<u64>(%81))));
// DEFAULT-NEXT:         return read<u32>(%275);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %82 @__flz32_inline(%83 __x: u32) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %276: u32 [synthetic];
// DEFAULT-NEXT:         if eq<u32>(read<u32>(%83), reinterpret<u32, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:             write<u32>(%276, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<u32>(%276, add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)), call<u32, signature=fn(u32) -> u32>(%58, read<u32>(%83))));
// DEFAULT-NEXT:         return read<u32>(%276);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %84 @__flz16_inline(%85 __x: u16) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %277: u32 [synthetic];
// DEFAULT-NEXT:         if eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%85))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))))
// DEFAULT-NEXT:             write<u32>(%277, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<u32>(%277, add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)), call<u32, signature=fn(u16) -> u32>(%60, read<u16>(%85))));
// DEFAULT-NEXT:         return read<u32>(%277);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %86 @__flz8_inline(%87 __x: u8) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %278: u32 [synthetic];
// DEFAULT-NEXT:         if eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%87))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))))
// DEFAULT-NEXT:             write<u32>(%278, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<u32>(%278, add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)), call<u32, signature=fn(u8) -> u32>(%62, read<u8>(%87))));
// DEFAULT-NEXT:         return read<u32>(%278);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %88 @__flo64_inline(%89 __x: u64) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %279: u32 [synthetic];
// DEFAULT-NEXT:         if eq<u64>(read<u64>(%89), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<u32>(%279, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<u32>(%279, add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)), call<u32, signature=fn(u64) -> u32>(%48, read<u64>(%89))));
// DEFAULT-NEXT:         return read<u32>(%279);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %90 @__flo32_inline(%91 __x: u32) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %280: u32 [synthetic];
// DEFAULT-NEXT:         if eq<u32>(read<u32>(%91), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:             write<u32>(%280, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<u32>(%280, add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)), call<u32, signature=fn(u32) -> u32>(%50, read<u32>(%91))));
// DEFAULT-NEXT:         return read<u32>(%280);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %92 @__flo16_inline(%93 __x: u16) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %281: u32 [synthetic];
// DEFAULT-NEXT:         if eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%93))), const<i32>(0))
// DEFAULT-NEXT:             write<u32>(%281, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<u32>(%281, add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)), call<u32, signature=fn(u16) -> u32>(%52, read<u16>(%93))));
// DEFAULT-NEXT:         return read<u32>(%281);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %94 @__flo8_inline(%95 __x: u8) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %282: u32 [synthetic];
// DEFAULT-NEXT:         if eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%95))), const<i32>(0))
// DEFAULT-NEXT:             write<u32>(%282, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<u32>(%282, add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)), call<u32, signature=fn(u8) -> u32>(%54, read<u8>(%95))));
// DEFAULT-NEXT:         return read<u32>(%282);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %96 @__ftz64_inline(%97 __x: u64) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %283: u32 [synthetic];
// DEFAULT-NEXT:         if eq<u64>(read<u64>(%97), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:             write<u32>(%283, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<u32>(%283, add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)), call<u32, signature=fn(u64) -> u32>(%72, read<u64>(%97))));
// DEFAULT-NEXT:         return read<u32>(%283);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %98 @__ftz32_inline(%99 __x: u32) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %284: u32 [synthetic];
// DEFAULT-NEXT:         if eq<u32>(read<u32>(%99), reinterpret<u32, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:             write<u32>(%284, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<u32>(%284, add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)), call<u32, signature=fn(u32) -> u32>(%74, read<u32>(%99))));
// DEFAULT-NEXT:         return read<u32>(%284);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %100 @__ftz16_inline(%101 __x: u16) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %285: u32 [synthetic];
// DEFAULT-NEXT:         if eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%101))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))))
// DEFAULT-NEXT:             write<u32>(%285, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<u32>(%285, add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)), call<u32, signature=fn(u16) -> u32>(%76, read<u16>(%101))));
// DEFAULT-NEXT:         return read<u32>(%285);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %102 @__ftz8_inline(%103 __x: u8) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %286: u32 [synthetic];
// DEFAULT-NEXT:         if eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%103))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))))
// DEFAULT-NEXT:             write<u32>(%286, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<u32>(%286, add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)), call<u32, signature=fn(u8) -> u32>(%78, read<u8>(%103))));
// DEFAULT-NEXT:         return read<u32>(%286);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %104 @__fto64_inline(%105 __x: u64) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %287: u32 [synthetic];
// DEFAULT-NEXT:         if eq<u64>(read<u64>(%105), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<u32>(%287, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<u32>(%287, add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)), call<u32, signature=fn(u64) -> u32>(%64, read<u64>(%105))));
// DEFAULT-NEXT:         return read<u32>(%287);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %106 @__fto32_inline(%107 __x: u32) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %288: u32 [synthetic];
// DEFAULT-NEXT:         if eq<u32>(read<u32>(%107), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:             write<u32>(%288, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<u32>(%288, add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)), call<u32, signature=fn(u32) -> u32>(%66, read<u32>(%107))));
// DEFAULT-NEXT:         return read<u32>(%288);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %108 @__fto16_inline(%109 __x: u16) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %289: u32 [synthetic];
// DEFAULT-NEXT:         if eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%109))), const<i32>(0))
// DEFAULT-NEXT:             write<u32>(%289, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<u32>(%289, add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)), call<u32, signature=fn(u16) -> u32>(%68, read<u16>(%109))));
// DEFAULT-NEXT:         return read<u32>(%289);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %110 @__fto8_inline(%111 __x: u8) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %290: u32 [synthetic];
// DEFAULT-NEXT:         if eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%111))), const<i32>(0))
// DEFAULT-NEXT:             write<u32>(%290, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<u32>(%290, add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)), call<u32, signature=fn(u8) -> u32>(%70, read<u8>(%111))));
// DEFAULT-NEXT:         return read<u32>(%290);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %112 @__cz64_inline(%113 __x: u64) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return sub<u32, overflow=wrap>(const<u32>(64), reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn(u64) -> i32>(__builtin_popcountll, read<u64>(%113))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %114 @__co64_inline(%115 __x: u64) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn(u64) -> i32>(__builtin_popcountll, read<u64>(%115)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %116 @__hsb64_inline(%117 __x: u64) -> bool [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return gt<u64>(xor<u64>(read<u64>(%117), sub<u64, overflow=wrap>(read<u64>(%117), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), sub<u64, overflow=wrap>(read<u64>(%117), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %118 @__hsb32_inline(%119 __x: u32) -> bool [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return gt<u32>(xor<u32>(read<u32>(%119), sub<u32, overflow=wrap>(read<u32>(%119), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))), sub<u32, overflow=wrap>(read<u32>(%119), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %120 @__bw64_inline(%121 __x: u64) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return sub<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(64)), call<u32, signature=fn(u64) -> u32>(%48, read<u64>(%121)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %122 @__bf64_inline(%123 __x: u64) -> u64 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %291: u64 [synthetic];
// DEFAULT-NEXT:         if eq<u64>(read<u64>(%123), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<u64>(%291, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<u64>(%291, shl<u64, overflow=wrap, amount_out_of_range=ub>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), sub<u32, overflow=wrap>(call<u32, signature=fn(u64) -> u32>(%120, read<u64>(%123)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))));
// DEFAULT-NEXT:         return read<u64>(%291);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %124 @__bc64_inline(%125 __x: u64) -> u64 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %292: u64 [synthetic];
// DEFAULT-NEXT:         if le<u64>(read<u64>(%125), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             write<u64>(%292, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<u64>(%292, shl<u64, overflow=wrap, amount_out_of_range=ub>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), sub<u32, overflow=wrap>(call<u32, signature=fn(u64) -> u32>(%120, sub<u64, overflow=wrap>(read<u64>(%125), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))));
// DEFAULT-NEXT:         return read<u64>(%292);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %126 @c23_stdbit() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %127 value: u32 [storage=automatic] = const<u32>(176);
// DEFAULT-NEXT:         let %293: u32 [synthetic];
// DEFAULT-NEXT:         if eq<u64>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))))
// DEFAULT-NEXT:             write<u32>(%293, call<u32, signature=fn(u64) -> u32>(%64, widen<u64, reason=arg>(read<u32>(%127))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             let %294: u32 [synthetic];
// DEFAULT-NEXT:             if eq<u64>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))
// DEFAULT-NEXT:                 write<u32>(%294, call<u32, signature=fn(u32) -> u32>(%66, read<u32>(%127)));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 let %295: u32 [synthetic];
// DEFAULT-NEXT:                 if eq<u64>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:                     write<u32>(%295, call<u32, signature=fn(u16) -> u32>(%68, truncate<u16, reason=explicit, fits=unknown>(read<u32>(%127))));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<u32>(%295, call<u32, signature=fn(u8) -> u32>(%70, truncate<u8, reason=explicit, fits=unknown>(read<u32>(%127))));
// DEFAULT-NEXT:                 write<u32>(%294, read<u32>(%295));
// DEFAULT-NEXT:             write<u32>(%293, read<u32>(%294));
// DEFAULT-NEXT:         let %296: u32 [synthetic];
// DEFAULT-NEXT:         if eq<u64>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))))
// DEFAULT-NEXT:             write<u32>(%296, call<u32, signature=fn(u64) -> u32>(%80, widen<u64, reason=arg>(read<u32>(%127))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             let %297: u32 [synthetic];
// DEFAULT-NEXT:             if eq<u64>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))
// DEFAULT-NEXT:                 write<u32>(%297, call<u32, signature=fn(u32) -> u32>(%82, read<u32>(%127)));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 let %298: u32 [synthetic];
// DEFAULT-NEXT:                 if eq<u64>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:                     write<u32>(%298, call<u32, signature=fn(u16) -> u32>(%84, truncate<u16, reason=explicit, fits=unknown>(read<u32>(%127))));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<u32>(%298, call<u32, signature=fn(u8) -> u32>(%86, truncate<u8, reason=explicit, fits=unknown>(read<u32>(%127))));
// DEFAULT-NEXT:                 write<u32>(%297, read<u32>(%298));
// DEFAULT-NEXT:             write<u32>(%296, read<u32>(%297));
// DEFAULT-NEXT:         let %299: u32 [synthetic];
// DEFAULT-NEXT:         if eq<u64>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))))
// DEFAULT-NEXT:             write<u32>(%299, call<u32, signature=fn(u64) -> u32>(%88, widen<u64, reason=arg>(read<u32>(%127))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             let %300: u32 [synthetic];
// DEFAULT-NEXT:             if eq<u64>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))
// DEFAULT-NEXT:                 write<u32>(%300, call<u32, signature=fn(u32) -> u32>(%90, read<u32>(%127)));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 let %301: u32 [synthetic];
// DEFAULT-NEXT:                 if eq<u64>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:                     write<u32>(%301, call<u32, signature=fn(u16) -> u32>(%92, truncate<u16, reason=explicit, fits=unknown>(read<u32>(%127))));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<u32>(%301, call<u32, signature=fn(u8) -> u32>(%94, truncate<u8, reason=explicit, fits=unknown>(read<u32>(%127))));
// DEFAULT-NEXT:                 write<u32>(%300, read<u32>(%301));
// DEFAULT-NEXT:             write<u32>(%299, read<u32>(%300));
// DEFAULT-NEXT:         let %302: u32 [synthetic];
// DEFAULT-NEXT:         if eq<u64>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))))
// DEFAULT-NEXT:             write<u32>(%302, call<u32, signature=fn(u64) -> u32>(%96, widen<u64, reason=arg>(read<u32>(%127))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             let %303: u32 [synthetic];
// DEFAULT-NEXT:             if eq<u64>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))
// DEFAULT-NEXT:                 write<u32>(%303, call<u32, signature=fn(u32) -> u32>(%98, read<u32>(%127)));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 let %304: u32 [synthetic];
// DEFAULT-NEXT:                 if eq<u64>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:                     write<u32>(%304, call<u32, signature=fn(u16) -> u32>(%100, truncate<u16, reason=explicit, fits=unknown>(read<u32>(%127))));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<u32>(%304, call<u32, signature=fn(u8) -> u32>(%102, truncate<u8, reason=explicit, fits=unknown>(read<u32>(%127))));
// DEFAULT-NEXT:                 write<u32>(%303, read<u32>(%304));
// DEFAULT-NEXT:             write<u32>(%302, read<u32>(%303));
// DEFAULT-NEXT:         let %305: u32 [synthetic];
// DEFAULT-NEXT:         if eq<u64>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))))
// DEFAULT-NEXT:             write<u32>(%305, call<u32, signature=fn(u64) -> u32>(%104, widen<u64, reason=arg>(read<u32>(%127))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             let %306: u32 [synthetic];
// DEFAULT-NEXT:             if eq<u64>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))
// DEFAULT-NEXT:                 write<u32>(%306, call<u32, signature=fn(u32) -> u32>(%106, read<u32>(%127)));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 let %307: u32 [synthetic];
// DEFAULT-NEXT:                 if eq<u64>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:                     write<u32>(%307, call<u32, signature=fn(u16) -> u32>(%108, truncate<u16, reason=explicit, fits=unknown>(read<u32>(%127))));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<u32>(%307, call<u32, signature=fn(u8) -> u32>(%110, truncate<u8, reason=explicit, fits=unknown>(read<u32>(%127))));
// DEFAULT-NEXT:                 write<u32>(%306, read<u32>(%307));
// DEFAULT-NEXT:             write<u32>(%305, read<u32>(%306));
// DEFAULT-NEXT:         let %308: i32 [synthetic];
// DEFAULT-NEXT:         if le<u64>(const<u64>(4), const<u64>(4))
// DEFAULT-NEXT:             write<i32>(%308, from_bool<i32, reason=promotion>(call<bool, signature=fn(u32) -> bool>(%118, const<u32>(64))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<i32>(%308, from_bool<i32, reason=promotion>(call<bool, signature=fn(u64) -> bool>(%116, widen<u64, reason=arg>(const<u32>(64)))));
// DEFAULT-NEXT:         return reinterpret<i32, reason=explicit, fits=unknown>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(sub<u32, overflow=wrap>(call<u32, signature=fn(u64) -> u32>(%48, widen<u64, reason=arg>(read<u32>(%127))), truncate<u32, reason=explicit, fits=unknown>(mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), sub<u64, overflow=wrap>(const<u64>(8), const<u64>(4))))), call<u32, signature=fn(u64) -> u32>(%56, shl<u64, overflow=wrap, amount_out_of_range=ub>(widen<u64, reason=explicit>(read<u32>(%127)), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), sub<u64, overflow=wrap>(const<u64>(8), const<u64>(4)))))), read<u32>(%293)), call<u32, signature=fn(u64) -> u32>(%72, widen<u64, reason=arg>(read<u32>(%127)))), read<u32>(%296)), read<u32>(%299)), read<u32>(%302)), read<u32>(%305)), sub<u32, overflow=wrap>(call<u32, signature=fn(u64) -> u32>(%112, widen<u64, reason=arg>(read<u32>(%127))), truncate<u32, reason=explicit, fits=unknown>(mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), sub<u64, overflow=wrap>(const<u64>(8), const<u64>(4)))))), call<u32, signature=fn(u64) -> u32>(%114, widen<u64, reason=arg>(read<u32>(%127)))), reinterpret<u32, reason=usual_arith, fits=always>(from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(read<i32>(%308), const<i32>(0))))), call<u32, signature=fn(u64) -> u32>(%120, widen<u64, reason=arg>(read<u32>(%127)))), truncate<u32, reason=explicit, fits=unknown>(call<u64, signature=fn(u64) -> u64>(%122, widen<u64, reason=arg>(read<u32>(%127))))), truncate<u32, reason=explicit, fits=unknown>(call<u64, signature=fn(u64) -> u64>(%124, widen<u64, reason=arg>(read<u32>(%127))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %128 @c23_checked_arithmetic() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %129 result: i32 [storage=automatic];
// DEFAULT-NEXT:         let %130 total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %309: i32 [synthetic] = read<i32>(%130);
// DEFAULT-NEXT:         let %310: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%309), from_bool<i32, reason=promotion>(logical_and<bool>(not<bool>(overflow_add<bool>(const<i32>(20), const<i32>(22), deref(addr_of<ptr<i32>>(%129)))), eq<i32>(read<i32>(%129), const<i32>(42)))));
// DEFAULT-NEXT:         write<i32>(%130, read<i32>(%310));
// DEFAULT-NEXT:         let %311: i32 [synthetic] = read<i32>(%130);
// DEFAULT-NEXT:         let %312: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%311), from_bool<i32, reason=promotion>(logical_and<bool>(not<bool>(overflow_sub<bool>(const<i32>(50), const<i32>(8), deref(addr_of<ptr<i32>>(%129)))), eq<i32>(read<i32>(%129), const<i32>(42)))));
// DEFAULT-NEXT:         write<i32>(%130, read<i32>(%312));
// DEFAULT-NEXT:         let %313: i32 [synthetic] = read<i32>(%130);
// DEFAULT-NEXT:         let %314: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%313), from_bool<i32, reason=promotion>(logical_and<bool>(not<bool>(overflow_mul<bool>(const<i32>(6), const<i32>(7), deref(addr_of<ptr<i32>>(%129)))), eq<i32>(read<i32>(%129), const<i32>(42)))));
// DEFAULT-NEXT:         write<i32>(%130, read<i32>(%314));
// DEFAULT-NEXT:         let %315: i32 [synthetic] = read<i32>(%130);
// DEFAULT-NEXT:         let %316: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%315), from_bool<i32, reason=promotion>(overflow_add<bool>(const<i32>(2147483647), const<i32>(1), deref(addr_of<ptr<i32>>(%129)))));
// DEFAULT-NEXT:         write<i32>(%130, read<i32>(%316));
// DEFAULT-NEXT:         return read<i32>(%130);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %131 @c23_utf8() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %132 input_state: @type14 [storage=automatic] = aggregate<@type14, zero_fill=true>();
// DEFAULT-NEXT:         let %133 output_state: @type14 [storage=automatic] = aggregate<@type14, zero_fill=true>();
// DEFAULT-NEXT:         let %134 character: u8 [storage=automatic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %135 output: array<i8, 16> [storage=automatic] [align=16] = aggregate<array<i8, 16>, zero_fill=true>();
// DEFAULT-NEXT:         let %136 input_size: u64 [storage=automatic] = call<u64, signature=fn(ptr<u8>, ptr<const i8>, u64, ptr<@type14>) -> u64>(%42, addr_of<ptr<u8>>(%134), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%244)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), addr_of<ptr<@type14>>(%132));
// DEFAULT-NEXT:         let %137 output_size: u64 [storage=automatic] = call<u64, signature=fn(ptr<i8>, u8, ptr<@type14>) -> u64>(%43, array_decay<ptr<i8>, length=Some(16)>(%135), read<u8>(%134), addr_of<ptr<@type14>>(%133));
// DEFAULT-NEXT:         let %138 atomic_character: atomic u8 [storage=automatic] = read<u8>(%134);
// DEFAULT-NEXT:         write<u8, atomic=seq_cst>(deref(addr_of<ptr<atomic u8>>(%138)), const<u8>(66));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(eq<u64>(read<u64>(%136), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), from_bool<i32, reason=promotion>(eq<u64>(read<u64>(%137), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))), from_bool<i32, reason=promotion>(eq<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(16)>(%135), const<i32>(0))))), const<i32>(65)))), from_bool<i32, reason=promotion>(eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, atomic=seq_cst>(deref(addr_of<ptr<atomic u8>>(%138))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(const<u8>(66)))))), from_bool<i32, reason=promotion>(gt<i32>(const<i32>(2), const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %139 @c23_memory() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %140 source: array<i8, 7> [storage=automatic] = code_units<array<i8, 7>>([97, 98, 99, 100, 101, 102, 0]);
// DEFAULT-NEXT:         let %141 destination: array<i8, 8> [storage=automatic] = aggregate<array<i8, 8>, zero_fill=true>();
// DEFAULT-NEXT:         let %142 secret: array<i8, 7> [storage=automatic] = code_units<array<i8, 7>>([115, 101, 99, 114, 101, 116, 0]);
// DEFAULT-NEXT:         let %143 first_copy: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         let %144 second_copy: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         let %145 phrase: ptr<const i8> [storage=automatic] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(12)>(%245));
// DEFAULT-NEXT:         let %146 mutable_phrase: array<i8, 12> [storage=automatic] = code_units<array<i8, 12>>([104, 101, 108, 108, 111, 32, 119, 111, 114, 108, 100, 0]);
// DEFAULT-NEXT:         let %147 stop: ptr<void> [storage=automatic] = call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, i32, u64) -> ptr<void>>(%26, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%141)), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%140)), const<i32>(99), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6))));
// DEFAULT-NEXT:         let %148 total: i32 [storage=automatic] = from_bool<i32, reason=assign>(logical_and<bool>(eq<ptr<void>>(read<ptr<void>>(%147), pointer_cast<ptr<void>, reason=usual_arith>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(8)>(%141), const<i32>(3)))), eq<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(8)>(%141), const<i32>(2))))), const<i32>(99))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%27, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%142)), const<i32>(0), const<u64>(7));
// DEFAULT-NEXT:         let %317: i32 [synthetic] = read<i32>(%148);
// DEFAULT-NEXT:         let %318: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%317), from_bool<i32, reason=promotion>(logical_and<bool>(eq<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(7)>(%142), const<i32>(0))))), const<i32>(0)), eq<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(7)>(%142), const<i32>(5))))), const<i32>(0)))));
// DEFAULT-NEXT:         write<i32>(%148, read<i32>(%318));
// DEFAULT-NEXT:         write<ptr<i8>>(%143, call<ptr<i8>, signature=fn(ptr<const i8>) -> ptr<i8>>(%30, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%246))));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<const i8>) -> ptr<i8>>(%30, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%246)));
// DEFAULT-NEXT:         write<ptr<i8>>(%144, call<ptr<i8>, signature=fn(ptr<const i8>, u64) -> ptr<i8>>(%31, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%247)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3)))));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<const i8>, u64) -> ptr<i8>>(%31, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%247)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))));
// DEFAULT-NEXT:         let %319: i32 [synthetic] = read<i32>(%148);
// DEFAULT-NEXT:         let %320: bool [synthetic];
// DEFAULT-NEXT:         if ne<ptr<i8>>(read<ptr<i8>>(%143), null<ptr<i8>>)
// DEFAULT-NEXT:             write<bool>(%320, eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%29, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%143)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%248))), const<i32>(0)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%320, const<bool>(false));
// DEFAULT-NEXT:         let %321: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%319), from_bool<i32, reason=promotion>(read<bool>(%320)));
// DEFAULT-NEXT:         write<i32>(%148, read<i32>(%321));
// DEFAULT-NEXT:         let %322: i32 [synthetic] = read<i32>(%148);
// DEFAULT-NEXT:         let %323: bool [synthetic];
// DEFAULT-NEXT:         if ne<ptr<i8>>(read<ptr<i8>>(%144), null<ptr<i8>>)
// DEFAULT-NEXT:             write<bool>(%323, eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%29, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%144)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%249))), const<i32>(0)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%323, const<bool>(false));
// DEFAULT-NEXT:         let %324: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%322), from_bool<i32, reason=promotion>(read<bool>(%323)));
// DEFAULT-NEXT:         write<i32>(%148, read<i32>(%324));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%143)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%25, pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%144)));
// DEFAULT-NEXT:         let %149 const_hit: ptr<const i8> [storage=automatic] = pointer_cast<ptr<const i8>, reason=assign>(call<ptr<i8>, signature=fn(ptr<const i8>, i32) -> ptr<i8>>(%32, read<ptr<const i8>>(%145), const<i32>(119)));
// DEFAULT-NEXT:         let %150 mut_hit: ptr<i8> [storage=automatic] = call<ptr<i8>, signature=fn(ptr<const i8>, i32) -> ptr<i8>>(%32, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(12)>(%146)), const<i32>(119));
// DEFAULT-NEXT:         let %325: i32 [synthetic] = read<i32>(%148);
// DEFAULT-NEXT:         let %326: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%325), from_bool<i32, reason=promotion>(logical_and<bool>(ne<ptr<const i8>>(read<ptr<const i8>>(%149), null<ptr<const i8>>), ne<ptr<i8>>(read<ptr<i8>>(%150), null<ptr<i8>>))));
// DEFAULT-NEXT:         write<i32>(%148, read<i32>(%326));
// DEFAULT-NEXT:         let %327: i32 [synthetic] = read<i32>(%148);
// DEFAULT-NEXT:         let %328: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%327), from_bool<i32, reason=promotion>(ne<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%28, pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i8>>(%145)), const<i32>(111), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(11)))), null<ptr<void>>)));
// DEFAULT-NEXT:         write<i32>(%148, read<i32>(%328));
// DEFAULT-NEXT:         let %329: i32 [synthetic] = read<i32>(%148);
// DEFAULT-NEXT:         let %330: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%329), from_bool<i32, reason=promotion>(ne<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%28, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(12)>(%146)), const<i32>(111), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(11)))), null<ptr<void>>)));
// DEFAULT-NEXT:         write<i32>(%148, read<i32>(%330));
// DEFAULT-NEXT:         let %331: i32 [synthetic] = read<i32>(%148);
// DEFAULT-NEXT:         let %332: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%331), from_bool<i32, reason=promotion>(ne<ptr<i8>>(call<ptr<i8>, signature=fn(ptr<const i8>, ptr<const i8>) -> ptr<i8>>(%33, read<ptr<const i8>>(%145), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%250))), null<ptr<i8>>)));
// DEFAULT-NEXT:         write<i32>(%148, read<i32>(%332));
// DEFAULT-NEXT:         let %333: i32 [synthetic] = read<i32>(%148);
// DEFAULT-NEXT:         let %334: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%333), from_bool<i32, reason=promotion>(ne<ptr<i8>>(call<ptr<i8>, signature=fn(ptr<const i8>, ptr<const i8>) -> ptr<i8>>(%33, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(12)>(%146)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%251))), null<ptr<i8>>)));
// DEFAULT-NEXT:         write<i32>(%148, read<i32>(%334));
// DEFAULT-NEXT:         return read<i32>(%148);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %151 @c23_time() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %152 timestamp: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(0));
// DEFAULT-NEXT:         let %153 utc: @type19 [storage=automatic] = aggregate<@type19, zero_fill=true>();
// DEFAULT-NEXT:         let %154 local: @type19 [storage=automatic] = aggregate<@type19, zero_fill=true>();
// DEFAULT-NEXT:         let %155 resolution: @type18 [storage=automatic] = aggregate<@type18, zero_fill=true>();
// DEFAULT-NEXT:         let %156 month: array<i8, 32> [storage=automatic] [align=16] = aggregate<array<i8, 32>, zero_fill=true>();
// DEFAULT-NEXT:         let %157 wide_month: array<i32, 32> [storage=automatic] [align=16] = aggregate<array<i32, 32>, zero_fill=true>();
// DEFAULT-NEXT:         let %158 total: i32 [storage=automatic] = from_bool<i32, reason=assign>(eq<ptr<@type19>>(call<ptr<@type19>, signature=fn(ptr<const i64>, ptr<@type19>) -> ptr<@type19>>(%36, pointer_cast<ptr<const i64>, reason=arg>(addr_of<ptr<i64>>(%152)), addr_of<ptr<@type19>>(%153)), addr_of<ptr<@type19>>(%153)));
// DEFAULT-NEXT:         let %335: i32 [synthetic] = read<i32>(%158);
// DEFAULT-NEXT:         let %336: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%335), from_bool<i32, reason=promotion>(eq<ptr<@type19>>(call<ptr<@type19>, signature=fn(ptr<const i64>, ptr<@type19>) -> ptr<@type19>>(%37, pointer_cast<ptr<const i64>, reason=arg>(addr_of<ptr<i64>>(%152)), addr_of<ptr<@type19>>(%154)), addr_of<ptr<@type19>>(%154))));
// DEFAULT-NEXT:         write<i32>(%158, read<i32>(%336));
// DEFAULT-NEXT:         let %337: i32 [synthetic] = read<i32>(%158);
// DEFAULT-NEXT:         let %338: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%337), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<@type18>, i32) -> i32>(%39, addr_of<ptr<@type18>>(%155), const<i32>(1)), const<i32>(1))));
// DEFAULT-NEXT:         write<i32>(%158, read<i32>(%338));
// DEFAULT-NEXT:         let %339: i32 [synthetic] = read<i32>(%158);
// DEFAULT-NEXT:         let %340: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%339), from_bool<i32, reason=promotion>(logical_or<bool>(gt<i64>(read<i64>(field0(%155)), widen<i64, reason=usual_arith>(const<i32>(0))), gt<i64>(read<i64>(field1(%155)), widen<i64, reason=usual_arith>(const<i32>(0))))));
// DEFAULT-NEXT:         write<i32>(%158, read<i32>(%340));
// DEFAULT-NEXT:         let %341: i32 [synthetic] = read<i32>(%158);
// DEFAULT-NEXT:         let %342: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%341), from_bool<i32, reason=promotion>(eq<i64>(call<i64, signature=fn(ptr<@type19>) -> i64>(%38, addr_of<ptr<@type19>>(%153)), widen<i64, reason=usual_arith>(const<i32>(0)))));
// DEFAULT-NEXT:         write<i32>(%158, read<i32>(%342));
// DEFAULT-NEXT:         let %343: i32 [synthetic] = read<i32>(%158);
// DEFAULT-NEXT:         let %344: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%343), from_bool<i32, reason=promotion>(eq<u64>(call<u64, signature=fn(ptr<i8>, u64, ptr<const i8>, ptr<const @type19>) -> u64>(%35, array_decay<ptr<i8>, length=Some(32)>(%156), const<u64>(32), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%252)), pointer_cast<ptr<const @type19>, reason=arg>(addr_of<ptr<@type19>>(%153))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(7))))));
// DEFAULT-NEXT:         write<i32>(%158, read<i32>(%344));
// DEFAULT-NEXT:         let %345: i32 [synthetic] = read<i32>(%158);
// DEFAULT-NEXT:         let %346: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%345), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%29, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(32)>(%156)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%253))), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%158, read<i32>(%346));
// DEFAULT-NEXT:         let %347: i32 [synthetic] = read<i32>(%158);
// DEFAULT-NEXT:         let %348: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%347), from_bool<i32, reason=promotion>(eq<u64>(call<u64, signature=fn(ptr<i32>, u64, ptr<const i32>, ptr<const @type19>) -> u64>(%47, array_decay<ptr<i32>, length=Some(32)>(%157), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(32))), pointer_cast<ptr<const i32>, reason=arg>(array_decay<ptr<i32>, length=Some(4)>(%254)), pointer_cast<ptr<const @type19>, reason=arg>(addr_of<ptr<@type19>>(%153))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(7))))));
// DEFAULT-NEXT:         write<i32>(%158, read<i32>(%348));
// DEFAULT-NEXT:         let %349: i32 [synthetic] = read<i32>(%158);
// DEFAULT-NEXT:         let %350: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%349), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const i32>, ptr<const i32>) -> i32>(%44, pointer_cast<ptr<const i32>, reason=arg>(array_decay<ptr<i32>, length=Some(32)>(%157)), pointer_cast<ptr<const i32>, reason=arg>(array_decay<ptr<i32>, length=Some(8)>(%255))), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%158, read<i32>(%350));
// DEFAULT-NEXT:         let %159 const_month: ptr<const i32> [storage=automatic] = pointer_cast<ptr<const i32>, reason=assign>(array_decay<ptr<i32>, length=Some(32)>(%157));
// DEFAULT-NEXT:         let %351: i32 [synthetic] = read<i32>(%158);
// DEFAULT-NEXT:         let %352: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%351), from_bool<i32, reason=promotion>(ne<ptr<i32>>(call<ptr<i32>, signature=fn(ptr<const i32>, i32) -> ptr<i32>>(%45, read<ptr<const i32>>(%159), const<i32>(110)), null<ptr<i32>>)));
// DEFAULT-NEXT:         write<i32>(%158, read<i32>(%352));
// DEFAULT-NEXT:         let %353: i32 [synthetic] = read<i32>(%158);
// DEFAULT-NEXT:         let %354: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%353), from_bool<i32, reason=promotion>(ne<ptr<i32>>(call<ptr<i32>, signature=fn(ptr<const i32>, i32) -> ptr<i32>>(%45, pointer_cast<ptr<const i32>, reason=arg>(array_decay<ptr<i32>, length=Some(32)>(%157)), const<i32>(110)), null<ptr<i32>>)));
// DEFAULT-NEXT:         write<i32>(%158, read<i32>(%354));
// DEFAULT-NEXT:         let %355: i32 [synthetic] = read<i32>(%158);
// DEFAULT-NEXT:         let %356: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%355), from_bool<i32, reason=promotion>(ne<ptr<i32>>(call<ptr<i32>, signature=fn(ptr<const i32>, ptr<const i32>) -> ptr<i32>>(%46, read<ptr<const i32>>(%159), pointer_cast<ptr<const i32>, reason=arg>(array_decay<ptr<i32>, length=Some(4)>(%256))), null<ptr<i32>>)));
// DEFAULT-NEXT:         write<i32>(%158, read<i32>(%356));
// DEFAULT-NEXT:         let %357: i32 [synthetic] = read<i32>(%158);
// DEFAULT-NEXT:         let %358: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%357), from_bool<i32, reason=promotion>(ne<ptr<i32>>(call<ptr<i32>, signature=fn(ptr<const i32>, ptr<const i32>) -> ptr<i32>>(%46, pointer_cast<ptr<const i32>, reason=arg>(array_decay<ptr<i32>, length=Some(32)>(%157)), pointer_cast<ptr<const i32>, reason=arg>(array_decay<ptr<i32>, length=Some(4)>(%257))), null<ptr<i32>>)));
// DEFAULT-NEXT:         write<i32>(%158, read<i32>(%358));
// DEFAULT-NEXT:         return read<i32>(%158);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %160 @c23_io() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %161 output: array<i8, 64> [storage=automatic] [align=16] = aggregate<array<i8, 64>, zero_fill=true>();
// DEFAULT-NEXT:         let %162 float_output: array<i8, 16> [storage=automatic] [align=16] = aggregate<array<i8, 16>, zero_fill=true>();
// DEFAULT-NEXT:         let %163 double_output: array<i8, 16> [storage=automatic] [align=16] = aggregate<array<i8, 16>, zero_fill=true>();
// DEFAULT-NEXT:         let %164 long_double_output: array<i8, 16> [storage=automatic] [align=16] = aggregate<array<i8, 16>, zero_fill=true>();
// DEFAULT-NEXT:         let %165 binary_value: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:         let %166 exact_value: u16 [storage=automatic] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %167 fast_value: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         let %168 written: i32 [storage=automatic] = call<i32, signature=fn(ptr<i8>, u64, ptr<const i8>, ...) -> i32>(%18, array_decay<ptr<i8>, length=Some(64)>(%161), const<u64>(64), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%258)), const<u32>(13), reinterpret<i32, reason=vararg, fits=unknown>(widen<u32, reason=vararg>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(21))))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(34))));
// DEFAULT-NEXT:         let %169 scanned: i32 [storage=automatic] = call<i32, signature=fn(ptr<const i8>, ptr<const i8>, ...) -> i32>(%19, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(11)>(%259)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%260)), addr_of<ptr<u32>>(%165), addr_of<ptr<u16>>(%166), addr_of<ptr<u64>>(%167));
// DEFAULT-NEXT:         let %170 floating_written: i32 [storage=automatic] = add<i32, overflow=ub>(add<i32, overflow=ub>(call<i32, signature=fn(ptr<i8>, u64, ptr<const i8>, f32) -> i32>(%21, array_decay<ptr<i8>, length=Some(16)>(%162), const<u64>(16), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%261)), const<f32>(1.5)), call<i32, signature=fn(ptr<i8>, u64, ptr<const i8>, f64) -> i32>(%20, array_decay<ptr<i8>, length=Some(16)>(%163), const<u64>(16), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%262)), const<f64>(2.5))), call<i32, signature=fn(ptr<i8>, u64, ptr<const i8>, f80) -> i32>(%22, array_decay<ptr<i8>, length=Some(16)>(%164), const<u64>(16), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%263)), const<f80>(3.5)));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(%168), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%29, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(64)>(%161)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(11)>(%264))), const<i32>(0)))), read<i32>(%169)), from_bool<i32, reason=promotion>(eq<u32>(read<u32>(%165), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(11))))), from_bool<i32, reason=promotion>(eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%166))), const<i32>(55)))), from_bool<i32, reason=promotion>(eq<u64>(read<u64>(%167), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(89)))))), read<i32>(%170)), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%29, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%162)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%265))), const<i32>(0)))), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%29, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%163)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%266))), const<i32>(0)))), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%29, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%164)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%267))), const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %171 @c23_limits() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %172 integer_widths: i32 [storage=automatic] = add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(const<i32>(1), const<i32>(8)), const<i32>(8)), const<i32>(8)), const<i32>(16)), const<i32>(16)), const<i32>(32)), const<i32>(32)), const<i32>(64)), const<i32>(64)), const<i32>(64)), const<i32>(64)), const<i32>(64)), const<i32>(64));
// DEFAULT-NEXT:         let %173 floating_limits: i32 [storage=automatic] = add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(gt<f32, exceptions=ignore>(const<f32>(1e-45), const<f32>(0.0))), from_bool<i32, reason=promotion>(gt<f64, exceptions=ignore>(const<f64>(5e-324), const<f64>(0.0)))), from_bool<i32, reason=promotion>(gt<f80, exceptions=ignore>(const<f80>(3.64519953188247460253E-4951), const<f80>(0)))), from_bool<i32, reason=promotion>(le<f32, exceptions=ignore>(const<f32>(3.4028235e38), const<f32>(3.4028235e38)))), from_bool<i32, reason=promotion>(le<f64, exceptions=ignore>(const<f64>(1.7976931348623157e308), const<f64>(1.7976931348623157e308)))), from_bool<i32, reason=promotion>(le<f80, exceptions=ignore>(const<f80>(1.18973149535723176502E+4932), const<f80>(1.18973149535723176502E+4932)))), from_bool<i32, reason=promotion>(ge<i32>(const<i32>(1), neg<i32, overflow=ub>(const<i32>(1))))), from_bool<i32, reason=promotion>(ge<i32>(const<i32>(1), neg<i32, overflow=ub>(const<i32>(1))))), from_bool<i32, reason=promotion>(ge<i32>(const<i32>(1), neg<i32, overflow=ub>(const<i32>(1))))), from_bool<i32, reason=promotion>(eq<u64>(const<u64>(4), const<u64>(4)))), from_bool<i32, reason=promotion>(eq<u64>(const<u64>(8), const<u64>(8)))), from_bool<i32, reason=promotion>(eq<u64>(const<u64>(16), const<u64>(16))));
// DEFAULT-NEXT:         let %174 header_versions: i32 [storage=automatic] = add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(eq<i64>(const<i64>(202311), const<i64>(202311))), from_bool<i32, reason=promotion>(eq<i64>(const<i64>(202311), const<i64>(202311)))), from_bool<i32, reason=promotion>(eq<i64>(const<i64>(202311), const<i64>(202311)))), from_bool<i32, reason=promotion>(eq<i64>(const<i64>(202311), const<i64>(202311)))), from_bool<i32, reason=promotion>(eq<i64>(const<i64>(202311), const<i64>(202311)))), from_bool<i32, reason=promotion>(eq<i64>(const<i64>(202311), const<i64>(202311)))), const<i32>(1));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(%172), read<i32>(%173)), read<i32>(%174));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %175 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%17, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(19)>(%268)), call<i32, signature=fn() -> i32>(%126), call<i32, signature=fn() -> i32>(%128), call<i32, signature=fn() -> i32>(%131), call<i32, signature=fn() -> i32>(%139), call<i32, signature=fn() -> i32>(%151), add<i32, overflow=ub>(call<i32, signature=fn() -> i32>(%160), call<i32, signature=fn() -> i32>(%171)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
