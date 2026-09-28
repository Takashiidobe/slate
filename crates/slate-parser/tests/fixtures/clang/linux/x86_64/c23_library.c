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
// DEFAULT-NEXT:     global %322 .str322: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([65, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %323 .str323: array<i8, 12> [storage=static] = code_units<array<i8, 12>>([104, 101, 108, 108, 111, 32, 119, 111, 114, 108, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %324 .str324: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([99, 50, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %325 .str325: array<i8, 8> [storage=static] = code_units<array<i8, 8>>([108, 105, 98, 114, 97, 114, 121, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %326 .str326: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([99, 50, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %327 .str327: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([108, 105, 98, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %328 .str328: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([119, 111, 114, 108, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %329 .str329: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([119, 111, 114, 108, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %330 .str330: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 79, 66, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %331 .str331: array<i8, 8> [storage=static] = code_units<array<i8, 8>>([74, 97, 110, 117, 97, 114, 121, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %332 .str332: array<i32, 4> [storage=static] = code_units<array<i32, 4>>([37, 79, 66, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %333 .str333: array<i32, 8> [storage=static] = code_units<array<i32, 8>>([74, 97, 110, 117, 97, 114, 121, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %334 .str334: array<i32, 4> [storage=static] = code_units<array<i32, 4>>([74, 97, 110, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %335 .str335: array<i32, 4> [storage=static] = code_units<array<i32, 4>>([74, 97, 110, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %336 .str336: array<i8, 16> [storage=static] = code_units<array<i8, 16>>([37, 98, 32, 37, 119, 49, 54, 117, 32, 37, 119, 102, 49, 54, 117, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %337 .str337: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([49, 48, 49, 49, 32, 53, 53, 32, 56, 57, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %338 .str338: array<i8, 16> [storage=static] = code_units<array<i8, 16>>([37, 98, 32, 37, 119, 49, 54, 117, 32, 37, 119, 102, 49, 54, 117, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %339 .str339: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([37, 46, 49, 102, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %340 .str340: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([37, 46, 49, 102, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %341 .str341: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([37, 46, 49, 102, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %342 .str342: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([49, 49, 48, 49, 32, 50, 49, 32, 51, 52, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %343 .str343: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([49, 46, 53, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %344 .str344: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([50, 46, 53, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %345 .str345: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([51, 46, 53, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %346 .str346: array<i8, 19> [storage=static] = code_units<array<i8, 19>>([37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %18 @printf(%244 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %22 @snprintf(%245 __s: ptr<i8> [restrict], %246 __maxlen: u64, %247 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %25 @sscanf(%248 __s: ptr<const i8> [restrict], %249 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external] [asm_name="__isoc23_sscanf"];
// DEFAULT-NEXT:     fn %32 @strfromd(%252 __dest: ptr<i8>, %253 __size: u64, %254 __format: ptr<const i8>, %255 __f: f64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %37 @strfromf(%256 __dest: ptr<i8>, %257 __size: u64, %258 __format: ptr<const i8>, %259 __f: f32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %42 @strfroml(%260 __dest: ptr<i8>, %261 __size: u64, %262 __format: ptr<const i8>, %263 __f: f80) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %46 @free(%264 __ptr: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %51 @memccpy(%265 __dest: ptr<void> [restrict], %266 __src: ptr<const void> [restrict], %267 __c: i32, %268 __n: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %55 @memset_explicit(%269 __s: ptr<void>, %270 __c: i32, %271 __n: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %59 @memchr(%272 __s: ptr<const void>, %273 __c: i32, %274 __n: u64) -> ptr<void> [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %62 @strcmp(%275 __s1: ptr<const i8>, %276 __s2: ptr<const i8>) -> i32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %64 @strdup(%277 __s: ptr<const i8>) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %67 @strndup(%278 __string: ptr<const i8>, %279 __n: u64) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %70 @strchr(%280 __s: ptr<const i8>, %281 __c: i32) -> ptr<i8> [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %73 @strstr(%282 __haystack: ptr<const i8>, %283 __needle: ptr<const i8>) -> ptr<i8> [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %79 @strftime(%284 __s: ptr<i8> [restrict], %285 __maxsize: u64, %286 __format: ptr<const i8> [restrict], %287 __tp: ptr<const @type19> [restrict]) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %82 @gmtime_r(%288 __timer: ptr<const i64> [restrict], %289 __tp: ptr<@type19> [restrict]) -> ptr<@type19> [linkage=external];
// DEFAULT-NEXT:     fn %85 @localtime_r(%290 __timer: ptr<const i64> [restrict], %291 __tp: ptr<@type19> [restrict]) -> ptr<@type19> [linkage=external];
// DEFAULT-NEXT:     fn %87 @timegm(%292 __tp: ptr<@type19>) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %90 @timespec_getres(%293 __ts: ptr<@type18>, %294 __base: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %97 @mbrtoc8(%295 __pc8: ptr<u8> [restrict], %296 __s: ptr<const i8> [restrict], %297 __n: u64, %298 __p: ptr<@type14> [restrict]) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %101 @c8rtomb(%299 __s: ptr<i8> [restrict], %300 __c8: u8, %301 __ps: ptr<@type14> [restrict]) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %104 @wcscmp(%302 __s1: ptr<const i32>, %303 __s2: ptr<const i32>) -> i32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %107 @wcschr(%304 __wcs: ptr<const i32>, %305 __wc: i32) -> ptr<i32> [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %110 @wcsstr(%306 __haystack: ptr<const i32>, %307 __needle: ptr<const i32>) -> ptr<i32> [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %115 @wcsftime(%308 __s: ptr<i32> [restrict], %309 __maxsize: u64, %310 __format: ptr<const i32> [restrict], %311 __tp: ptr<const @type19> [restrict]) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %313 @__builtin_clzll(%312 <unnamed>: u64) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %116 @__clz64_inline(%117 __x: u64) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<u32>(eq<u64>(read<u64>(%117), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), const<u32>(64), reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn(u64) -> i32>(%313, read<u64>(%117))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %315 @__builtin_clz(%314 <unnamed>: u32) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %118 @__clz32_inline(%119 __x: u32) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<u32>(eq<u32>(read<u32>(%119), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))), const<u32>(32), reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn(u32) -> i32>(%315, read<u32>(%119))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %120 @__clz16_inline(%121 __x: u16) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return sub<u32, overflow=wrap>(call<u32, signature=fn(u32) -> u32>(%118, widen<u32, reason=arg>(read<u16>(%121))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(16)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %122 @__clz8_inline(%123 __x: u8) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return sub<u32, overflow=wrap>(call<u32, signature=fn(u32) -> u32>(%118, widen<u32, reason=arg>(read<u8>(%123))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(24)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %124 @__clo64_inline(%125 __x: u64) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<u32, signature=fn(u64) -> u32>(%116, not<u64>(read<u64>(%125)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %126 @__clo32_inline(%127 __x: u32) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<u32, signature=fn(u32) -> u32>(%118, not<u32>(read<u32>(%127)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %128 @__clo16_inline(%129 __x: u16) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<u32, signature=fn(u16) -> u32>(%120, reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(not<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%129)))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %130 @__clo8_inline(%131 __x: u8) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<u32, signature=fn(u8) -> u32>(%122, reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(not<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%131)))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %317 @__builtin_ctzll(%316 <unnamed>: u64) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %132 @__ctz64_inline(%133 __x: u64) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<u32>(eq<u64>(read<u64>(%133), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), const<u32>(64), reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn(u64) -> i32>(%317, read<u64>(%133))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %319 @__builtin_ctz(%318 <unnamed>: u32) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %134 @__ctz32_inline(%135 __x: u32) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<u32>(eq<u32>(read<u32>(%135), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))), const<u32>(32), reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn(u32) -> i32>(%319, read<u32>(%135))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %136 @__ctz16_inline(%137 __x: u16) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<u32>(eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%137))), const<i32>(0)), const<u32>(16), reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn(u32) -> i32>(%319, widen<u32, reason=arg>(read<u16>(%137)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %138 @__ctz8_inline(%139 __x: u8) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<u32>(eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%139))), const<i32>(0)), const<u32>(8), reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn(u32) -> i32>(%319, widen<u32, reason=arg>(read<u8>(%139)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %140 @__cto64_inline(%141 __x: u64) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<u32, signature=fn(u64) -> u32>(%132, not<u64>(read<u64>(%141)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %142 @__cto32_inline(%143 __x: u32) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<u32, signature=fn(u32) -> u32>(%134, not<u32>(read<u32>(%143)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %144 @__cto16_inline(%145 __x: u16) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<u32, signature=fn(u16) -> u32>(%136, reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(not<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%145)))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %146 @__cto8_inline(%147 __x: u8) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<u32, signature=fn(u8) -> u32>(%138, reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(not<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%147)))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %148 @__flz64_inline(%149 __x: u64) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %347: u32 [synthetic];
// DEFAULT-NEXT:         if eq<u64>(read<u64>(%149), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:             write<u32>(%347, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<u32>(%347, add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)), call<u32, signature=fn(u64) -> u32>(%124, read<u64>(%149))));
// DEFAULT-NEXT:         return read<u32>(%347);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %150 @__flz32_inline(%151 __x: u32) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %348: u32 [synthetic];
// DEFAULT-NEXT:         if eq<u32>(read<u32>(%151), reinterpret<u32, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:             write<u32>(%348, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<u32>(%348, add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)), call<u32, signature=fn(u32) -> u32>(%126, read<u32>(%151))));
// DEFAULT-NEXT:         return read<u32>(%348);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %152 @__flz16_inline(%153 __x: u16) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %349: u32 [synthetic];
// DEFAULT-NEXT:         if eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%153))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))))
// DEFAULT-NEXT:             write<u32>(%349, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<u32>(%349, add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)), call<u32, signature=fn(u16) -> u32>(%128, read<u16>(%153))));
// DEFAULT-NEXT:         return read<u32>(%349);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %154 @__flz8_inline(%155 __x: u8) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %350: u32 [synthetic];
// DEFAULT-NEXT:         if eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%155))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))))
// DEFAULT-NEXT:             write<u32>(%350, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<u32>(%350, add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)), call<u32, signature=fn(u8) -> u32>(%130, read<u8>(%155))));
// DEFAULT-NEXT:         return read<u32>(%350);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %156 @__flo64_inline(%157 __x: u64) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %351: u32 [synthetic];
// DEFAULT-NEXT:         if eq<u64>(read<u64>(%157), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<u32>(%351, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<u32>(%351, add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)), call<u32, signature=fn(u64) -> u32>(%116, read<u64>(%157))));
// DEFAULT-NEXT:         return read<u32>(%351);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %158 @__flo32_inline(%159 __x: u32) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %352: u32 [synthetic];
// DEFAULT-NEXT:         if eq<u32>(read<u32>(%159), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:             write<u32>(%352, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<u32>(%352, add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)), call<u32, signature=fn(u32) -> u32>(%118, read<u32>(%159))));
// DEFAULT-NEXT:         return read<u32>(%352);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %160 @__flo16_inline(%161 __x: u16) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %353: u32 [synthetic];
// DEFAULT-NEXT:         if eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%161))), const<i32>(0))
// DEFAULT-NEXT:             write<u32>(%353, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<u32>(%353, add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)), call<u32, signature=fn(u16) -> u32>(%120, read<u16>(%161))));
// DEFAULT-NEXT:         return read<u32>(%353);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %162 @__flo8_inline(%163 __x: u8) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %354: u32 [synthetic];
// DEFAULT-NEXT:         if eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%163))), const<i32>(0))
// DEFAULT-NEXT:             write<u32>(%354, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<u32>(%354, add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)), call<u32, signature=fn(u8) -> u32>(%122, read<u8>(%163))));
// DEFAULT-NEXT:         return read<u32>(%354);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %164 @__ftz64_inline(%165 __x: u64) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %355: u32 [synthetic];
// DEFAULT-NEXT:         if eq<u64>(read<u64>(%165), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:             write<u32>(%355, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<u32>(%355, add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)), call<u32, signature=fn(u64) -> u32>(%140, read<u64>(%165))));
// DEFAULT-NEXT:         return read<u32>(%355);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %166 @__ftz32_inline(%167 __x: u32) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %356: u32 [synthetic];
// DEFAULT-NEXT:         if eq<u32>(read<u32>(%167), reinterpret<u32, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:             write<u32>(%356, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<u32>(%356, add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)), call<u32, signature=fn(u32) -> u32>(%142, read<u32>(%167))));
// DEFAULT-NEXT:         return read<u32>(%356);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %168 @__ftz16_inline(%169 __x: u16) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %357: u32 [synthetic];
// DEFAULT-NEXT:         if eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%169))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))))
// DEFAULT-NEXT:             write<u32>(%357, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<u32>(%357, add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)), call<u32, signature=fn(u16) -> u32>(%144, read<u16>(%169))));
// DEFAULT-NEXT:         return read<u32>(%357);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %170 @__ftz8_inline(%171 __x: u8) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %358: u32 [synthetic];
// DEFAULT-NEXT:         if eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%171))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))))))
// DEFAULT-NEXT:             write<u32>(%358, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<u32>(%358, add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)), call<u32, signature=fn(u8) -> u32>(%146, read<u8>(%171))));
// DEFAULT-NEXT:         return read<u32>(%358);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %172 @__fto64_inline(%173 __x: u64) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %359: u32 [synthetic];
// DEFAULT-NEXT:         if eq<u64>(read<u64>(%173), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<u32>(%359, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<u32>(%359, add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)), call<u32, signature=fn(u64) -> u32>(%132, read<u64>(%173))));
// DEFAULT-NEXT:         return read<u32>(%359);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %174 @__fto32_inline(%175 __x: u32) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %360: u32 [synthetic];
// DEFAULT-NEXT:         if eq<u32>(read<u32>(%175), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:             write<u32>(%360, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<u32>(%360, add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)), call<u32, signature=fn(u32) -> u32>(%134, read<u32>(%175))));
// DEFAULT-NEXT:         return read<u32>(%360);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %176 @__fto16_inline(%177 __x: u16) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %361: u32 [synthetic];
// DEFAULT-NEXT:         if eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%177))), const<i32>(0))
// DEFAULT-NEXT:             write<u32>(%361, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<u32>(%361, add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)), call<u32, signature=fn(u16) -> u32>(%136, read<u16>(%177))));
// DEFAULT-NEXT:         return read<u32>(%361);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %178 @__fto8_inline(%179 __x: u8) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %362: u32 [synthetic];
// DEFAULT-NEXT:         if eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%179))), const<i32>(0))
// DEFAULT-NEXT:             write<u32>(%362, reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<u32>(%362, add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)), call<u32, signature=fn(u8) -> u32>(%138, read<u8>(%179))));
// DEFAULT-NEXT:         return read<u32>(%362);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %321 @__builtin_popcountll(%320 <unnamed>: u64) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %180 @__cz64_inline(%181 __x: u64) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return sub<u32, overflow=wrap>(const<u32>(64), reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn(u64) -> i32>(%321, read<u64>(%181))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %182 @__co64_inline(%183 __x: u64) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn(u64) -> i32>(%321, read<u64>(%183)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %184 @__hsb64_inline(%185 __x: u64) -> bool [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return gt<u64>(xor<u64>(read<u64>(%185), sub<u64, overflow=wrap>(read<u64>(%185), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), sub<u64, overflow=wrap>(read<u64>(%185), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %186 @__hsb32_inline(%187 __x: u32) -> bool [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return gt<u32>(xor<u32>(read<u32>(%187), sub<u32, overflow=wrap>(read<u32>(%187), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))), sub<u32, overflow=wrap>(read<u32>(%187), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %188 @__bw64_inline(%189 __x: u64) -> u32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return sub<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(64)), call<u32, signature=fn(u64) -> u32>(%116, read<u64>(%189)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %190 @__bf64_inline(%191 __x: u64) -> u64 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %363: u64 [synthetic];
// DEFAULT-NEXT:         if eq<u64>(read<u64>(%191), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<u64>(%363, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<u64>(%363, shl<u64, overflow=wrap, amount_out_of_range=ub>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))), sub<u32, overflow=wrap>(call<u32, signature=fn(u64) -> u32>(%188, read<u64>(%191)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))));
// DEFAULT-NEXT:         return read<u64>(%363);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %192 @__bc64_inline(%193 __x: u64) -> u64 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %364: u64 [synthetic];
// DEFAULT-NEXT:         if le<u64>(read<u64>(%193), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             write<u64>(%364, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<u64>(%364, shl<u64, overflow=wrap, amount_out_of_range=ub>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))), sub<u32, overflow=wrap>(call<u32, signature=fn(u64) -> u32>(%188, sub<u64, overflow=wrap>(read<u64>(%193), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))));
// DEFAULT-NEXT:         return read<u64>(%364);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %194 @c23_stdbit() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %195 value: u32 [storage=automatic] = const<u32>(176);
// DEFAULT-NEXT:         let %365: u32 [synthetic];
// DEFAULT-NEXT:         if eq<u64>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))))
// DEFAULT-NEXT:             write<u32>(%365, call<u32, signature=fn(u64) -> u32>(%132, widen<u64, reason=arg>(read<u32>(%195))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             let %366: u32 [synthetic];
// DEFAULT-NEXT:             if eq<u64>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))
// DEFAULT-NEXT:                 write<u32>(%366, call<u32, signature=fn(u32) -> u32>(%134, read<u32>(%195)));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 let %367: u32 [synthetic];
// DEFAULT-NEXT:                 if eq<u64>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:                     write<u32>(%367, call<u32, signature=fn(u16) -> u32>(%136, truncate<u16, reason=explicit, fits=unknown>(read<u32>(%195))));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<u32>(%367, call<u32, signature=fn(u8) -> u32>(%138, truncate<u8, reason=explicit, fits=unknown>(read<u32>(%195))));
// DEFAULT-NEXT:                 write<u32>(%366, read<u32>(%367));
// DEFAULT-NEXT:             write<u32>(%365, read<u32>(%366));
// DEFAULT-NEXT:         let %368: u32 [synthetic];
// DEFAULT-NEXT:         if eq<u64>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))))
// DEFAULT-NEXT:             write<u32>(%368, call<u32, signature=fn(u64) -> u32>(%148, widen<u64, reason=arg>(read<u32>(%195))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             let %369: u32 [synthetic];
// DEFAULT-NEXT:             if eq<u64>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))
// DEFAULT-NEXT:                 write<u32>(%369, call<u32, signature=fn(u32) -> u32>(%150, read<u32>(%195)));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 let %370: u32 [synthetic];
// DEFAULT-NEXT:                 if eq<u64>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:                     write<u32>(%370, call<u32, signature=fn(u16) -> u32>(%152, truncate<u16, reason=explicit, fits=unknown>(read<u32>(%195))));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<u32>(%370, call<u32, signature=fn(u8) -> u32>(%154, truncate<u8, reason=explicit, fits=unknown>(read<u32>(%195))));
// DEFAULT-NEXT:                 write<u32>(%369, read<u32>(%370));
// DEFAULT-NEXT:             write<u32>(%368, read<u32>(%369));
// DEFAULT-NEXT:         let %371: u32 [synthetic];
// DEFAULT-NEXT:         if eq<u64>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))))
// DEFAULT-NEXT:             write<u32>(%371, call<u32, signature=fn(u64) -> u32>(%156, widen<u64, reason=arg>(read<u32>(%195))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             let %372: u32 [synthetic];
// DEFAULT-NEXT:             if eq<u64>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))
// DEFAULT-NEXT:                 write<u32>(%372, call<u32, signature=fn(u32) -> u32>(%158, read<u32>(%195)));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 let %373: u32 [synthetic];
// DEFAULT-NEXT:                 if eq<u64>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:                     write<u32>(%373, call<u32, signature=fn(u16) -> u32>(%160, truncate<u16, reason=explicit, fits=unknown>(read<u32>(%195))));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<u32>(%373, call<u32, signature=fn(u8) -> u32>(%162, truncate<u8, reason=explicit, fits=unknown>(read<u32>(%195))));
// DEFAULT-NEXT:                 write<u32>(%372, read<u32>(%373));
// DEFAULT-NEXT:             write<u32>(%371, read<u32>(%372));
// DEFAULT-NEXT:         let %374: u32 [synthetic];
// DEFAULT-NEXT:         if eq<u64>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))))
// DEFAULT-NEXT:             write<u32>(%374, call<u32, signature=fn(u64) -> u32>(%164, widen<u64, reason=arg>(read<u32>(%195))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             let %375: u32 [synthetic];
// DEFAULT-NEXT:             if eq<u64>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))
// DEFAULT-NEXT:                 write<u32>(%375, call<u32, signature=fn(u32) -> u32>(%166, read<u32>(%195)));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 let %376: u32 [synthetic];
// DEFAULT-NEXT:                 if eq<u64>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:                     write<u32>(%376, call<u32, signature=fn(u16) -> u32>(%168, truncate<u16, reason=explicit, fits=unknown>(read<u32>(%195))));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<u32>(%376, call<u32, signature=fn(u8) -> u32>(%170, truncate<u8, reason=explicit, fits=unknown>(read<u32>(%195))));
// DEFAULT-NEXT:                 write<u32>(%375, read<u32>(%376));
// DEFAULT-NEXT:             write<u32>(%374, read<u32>(%375));
// DEFAULT-NEXT:         let %377: u32 [synthetic];
// DEFAULT-NEXT:         if eq<u64>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))))
// DEFAULT-NEXT:             write<u32>(%377, call<u32, signature=fn(u64) -> u32>(%172, widen<u64, reason=arg>(read<u32>(%195))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             let %378: u32 [synthetic];
// DEFAULT-NEXT:             if eq<u64>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))
// DEFAULT-NEXT:                 write<u32>(%378, call<u32, signature=fn(u32) -> u32>(%174, read<u32>(%195)));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 let %379: u32 [synthetic];
// DEFAULT-NEXT:                 if eq<u64>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:                     write<u32>(%379, call<u32, signature=fn(u16) -> u32>(%176, truncate<u16, reason=explicit, fits=unknown>(read<u32>(%195))));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<u32>(%379, call<u32, signature=fn(u8) -> u32>(%178, truncate<u8, reason=explicit, fits=unknown>(read<u32>(%195))));
// DEFAULT-NEXT:                 write<u32>(%378, read<u32>(%379));
// DEFAULT-NEXT:             write<u32>(%377, read<u32>(%378));
// DEFAULT-NEXT:         let %380: i32 [synthetic];
// DEFAULT-NEXT:         if le<u64>(const<u64>(4), const<u64>(4))
// DEFAULT-NEXT:             write<i32>(%380, from_bool<i32, reason=promotion>(call<bool, signature=fn(u32) -> bool>(%186, const<u32>(64))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<i32>(%380, from_bool<i32, reason=promotion>(call<bool, signature=fn(u64) -> bool>(%184, widen<u64, reason=arg>(const<u32>(64)))));
// DEFAULT-NEXT:         return reinterpret<i32, reason=explicit, fits=unknown>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(add<u32, overflow=wrap>(sub<u32, overflow=wrap>(call<u32, signature=fn(u64) -> u32>(%116, widen<u64, reason=arg>(read<u32>(%195))), truncate<u32, reason=explicit, fits=unknown>(mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), sub<u64, overflow=wrap>(const<u64>(8), const<u64>(4))))), call<u32, signature=fn(u64) -> u32>(%124, shl<u64, overflow=wrap, amount_out_of_range=ub>(widen<u64, reason=explicit>(read<u32>(%195)), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), sub<u64, overflow=wrap>(const<u64>(8), const<u64>(4)))))), read<u32>(%365)), call<u32, signature=fn(u64) -> u32>(%140, widen<u64, reason=arg>(read<u32>(%195)))), read<u32>(%368)), read<u32>(%371)), read<u32>(%374)), read<u32>(%377)), sub<u32, overflow=wrap>(call<u32, signature=fn(u64) -> u32>(%180, widen<u64, reason=arg>(read<u32>(%195))), truncate<u32, reason=explicit, fits=unknown>(mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))), sub<u64, overflow=wrap>(const<u64>(8), const<u64>(4)))))), call<u32, signature=fn(u64) -> u32>(%182, widen<u64, reason=arg>(read<u32>(%195)))), reinterpret<u32, reason=usual_arith, fits=always>(from_bool<i32, reason=promotion>(ne<i32, reason=explicit>(read<i32>(%380), const<i32>(0))))), call<u32, signature=fn(u64) -> u32>(%188, widen<u64, reason=arg>(read<u32>(%195)))), truncate<u32, reason=explicit, fits=unknown>(call<u64, signature=fn(u64) -> u64>(%190, widen<u64, reason=arg>(read<u32>(%195))))), truncate<u32, reason=explicit, fits=unknown>(call<u64, signature=fn(u64) -> u64>(%192, widen<u64, reason=arg>(read<u32>(%195))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %196 @c23_checked_arithmetic() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %197 result: i32 [storage=automatic];
// DEFAULT-NEXT:         let %198 total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %381: i32 [synthetic] = read<i32>(%198);
// DEFAULT-NEXT:         let %382: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%381), from_bool<i32, reason=promotion>(logical_and<bool>(not<bool>(overflow_add<bool>(const<i32>(20), const<i32>(22), deref(addr_of<ptr<i32>>(%197)))), eq<i32>(read<i32>(%197), const<i32>(42)))));
// DEFAULT-NEXT:         write<i32>(%198, read<i32>(%382));
// DEFAULT-NEXT:         let %383: i32 [synthetic] = read<i32>(%198);
// DEFAULT-NEXT:         let %384: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%383), from_bool<i32, reason=promotion>(logical_and<bool>(not<bool>(overflow_sub<bool>(const<i32>(50), const<i32>(8), deref(addr_of<ptr<i32>>(%197)))), eq<i32>(read<i32>(%197), const<i32>(42)))));
// DEFAULT-NEXT:         write<i32>(%198, read<i32>(%384));
// DEFAULT-NEXT:         let %385: i32 [synthetic] = read<i32>(%198);
// DEFAULT-NEXT:         let %386: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%385), from_bool<i32, reason=promotion>(logical_and<bool>(not<bool>(overflow_mul<bool>(const<i32>(6), const<i32>(7), deref(addr_of<ptr<i32>>(%197)))), eq<i32>(read<i32>(%197), const<i32>(42)))));
// DEFAULT-NEXT:         write<i32>(%198, read<i32>(%386));
// DEFAULT-NEXT:         let %387: i32 [synthetic] = read<i32>(%198);
// DEFAULT-NEXT:         let %388: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%387), from_bool<i32, reason=promotion>(overflow_add<bool>(const<i32>(2147483647), const<i32>(1), deref(addr_of<ptr<i32>>(%197)))));
// DEFAULT-NEXT:         write<i32>(%198, read<i32>(%388));
// DEFAULT-NEXT:         return read<i32>(%198);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %199 @c23_utf8() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %200 input_state: @type14 [storage=automatic] = aggregate<@type14, zero_fill=true>();
// DEFAULT-NEXT:         let %201 output_state: @type14 [storage=automatic] = aggregate<@type14, zero_fill=true>();
// DEFAULT-NEXT:         let %202 character: u8 [storage=automatic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %203 output: array<i8, 16> [storage=automatic] [align=16] = aggregate<array<i8, 16>, zero_fill=true>();
// DEFAULT-NEXT:         let %204 input_size: u64 [storage=automatic] = call<u64, signature=fn(ptr<u8>, ptr<const i8>, u64, ptr<@type14>) -> u64>(%97, addr_of<ptr<u8>>(%202), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%322)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), addr_of<ptr<@type14>>(%200));
// DEFAULT-NEXT:         let %205 output_size: u64 [storage=automatic] = call<u64, signature=fn(ptr<i8>, u8, ptr<@type14>) -> u64>(%101, array_decay<ptr<i8>, length=Some(16)>(%203), read<u8>(%202), addr_of<ptr<@type14>>(%201));
// DEFAULT-NEXT:         let %206 atomic_character: atomic u8 [storage=automatic] = read<u8>(%202);
// DEFAULT-NEXT:         write<u8, atomic=seq_cst>(deref(addr_of<ptr<atomic u8>>(%206)), const<u8>(66));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(eq<u64>(read<u64>(%204), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))), from_bool<i32, reason=promotion>(eq<u64>(read<u64>(%205), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))), from_bool<i32, reason=promotion>(eq<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(16)>(%203), const<i32>(0))))), const<i32>(65)))), from_bool<i32, reason=promotion>(eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, atomic=seq_cst>(deref(addr_of<ptr<atomic u8>>(%206))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(const<u8>(66)))))), from_bool<i32, reason=promotion>(gt<i32>(const<i32>(2), const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %207 @c23_memory() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %208 source: array<i8, 7> [storage=automatic] = code_units<array<i8, 7>>([97, 98, 99, 100, 101, 102, 0]);
// DEFAULT-NEXT:         let %209 destination: array<i8, 8> [storage=automatic] = aggregate<array<i8, 8>, zero_fill=true>();
// DEFAULT-NEXT:         let %210 secret: array<i8, 7> [storage=automatic] = code_units<array<i8, 7>>([115, 101, 99, 114, 101, 116, 0]);
// DEFAULT-NEXT:         let %211 first_copy: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         let %212 second_copy: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         let %213 phrase: ptr<const i8> [storage=automatic] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(12)>(%323));
// DEFAULT-NEXT:         let %214 mutable_phrase: array<i8, 12> [storage=automatic] = code_units<array<i8, 12>>([104, 101, 108, 108, 111, 32, 119, 111, 114, 108, 100, 0]);
// DEFAULT-NEXT:         let %215 stop: ptr<void> [storage=automatic] = call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, i32, u64) -> ptr<void>>(%51, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%209)), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%208)), const<i32>(99), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6))));
// DEFAULT-NEXT:         let %216 total: i32 [storage=automatic] = from_bool<i32, reason=assign>(logical_and<bool>(eq<ptr<void>>(read<ptr<void>>(%215), pointer_cast<ptr<void>, reason=usual_arith>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(8)>(%209), const<i32>(3)))), eq<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(8)>(%209), const<i32>(2))))), const<i32>(99))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%55, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%210)), const<i32>(0), const<u64>(7));
// DEFAULT-NEXT:         let %389: i32 [synthetic] = read<i32>(%216);
// DEFAULT-NEXT:         let %390: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%389), from_bool<i32, reason=promotion>(logical_and<bool>(eq<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(7)>(%210), const<i32>(0))))), const<i32>(0)), eq<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(7)>(%210), const<i32>(5))))), const<i32>(0)))));
// DEFAULT-NEXT:         write<i32>(%216, read<i32>(%390));
// DEFAULT-NEXT:         write<ptr<i8>>(%211, call<ptr<i8>, signature=fn(ptr<const i8>) -> ptr<i8>>(%64, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%324))));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<const i8>) -> ptr<i8>>(%64, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%324)));
// DEFAULT-NEXT:         write<ptr<i8>>(%212, call<ptr<i8>, signature=fn(ptr<const i8>, u64) -> ptr<i8>>(%67, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%325)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3)))));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<const i8>, u64) -> ptr<i8>>(%67, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%325)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))));
// DEFAULT-NEXT:         let %391: i32 [synthetic] = read<i32>(%216);
// DEFAULT-NEXT:         let %392: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%391), from_bool<i32, reason=promotion>(logical_and<bool>(ne<ptr<i8>>(read<ptr<i8>>(%211), null<ptr<i8>>), eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%62, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%211)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%326))), const<i32>(0)))));
// DEFAULT-NEXT:         write<i32>(%216, read<i32>(%392));
// DEFAULT-NEXT:         let %393: i32 [synthetic] = read<i32>(%216);
// DEFAULT-NEXT:         let %394: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%393), from_bool<i32, reason=promotion>(logical_and<bool>(ne<ptr<i8>>(read<ptr<i8>>(%212), null<ptr<i8>>), eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%62, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%212)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%327))), const<i32>(0)))));
// DEFAULT-NEXT:         write<i32>(%216, read<i32>(%394));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%46, pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%211)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%46, pointer_cast<ptr<void>, reason=arg>(read<ptr<i8>>(%212)));
// DEFAULT-NEXT:         let %217 const_hit: ptr<const i8> [storage=automatic] = pointer_cast<ptr<const i8>, reason=explicit>(call<ptr<i8>, signature=fn(ptr<const i8>, i32) -> ptr<i8>>(%70, read<ptr<const i8>>(%213), const<i32>(119)));
// DEFAULT-NEXT:         let %218 mut_hit: ptr<i8> [storage=automatic] = call<ptr<i8>, signature=fn(ptr<const i8>, i32) -> ptr<i8>>(%70, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(12)>(%214)), const<i32>(119));
// DEFAULT-NEXT:         let %395: i32 [synthetic] = read<i32>(%216);
// DEFAULT-NEXT:         let %396: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%395), from_bool<i32, reason=promotion>(logical_and<bool>(ne<ptr<const i8>>(read<ptr<const i8>>(%217), null<ptr<const i8>>), ne<ptr<i8>>(read<ptr<i8>>(%218), null<ptr<i8>>))));
// DEFAULT-NEXT:         write<i32>(%216, read<i32>(%396));
// DEFAULT-NEXT:         let %397: i32 [synthetic] = read<i32>(%216);
// DEFAULT-NEXT:         let %398: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%397), from_bool<i32, reason=promotion>(ne<ptr<const void>>(pointer_cast<ptr<const void>, reason=explicit>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%59, pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i8>>(%213)), const<i32>(111), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(11))))), null<ptr<const void>>)));
// DEFAULT-NEXT:         write<i32>(%216, read<i32>(%398));
// DEFAULT-NEXT:         let %399: i32 [synthetic] = read<i32>(%216);
// DEFAULT-NEXT:         let %400: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%399), from_bool<i32, reason=promotion>(ne<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%59, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(12)>(%214)), const<i32>(111), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(11)))), null<ptr<void>>)));
// DEFAULT-NEXT:         write<i32>(%216, read<i32>(%400));
// DEFAULT-NEXT:         let %401: i32 [synthetic] = read<i32>(%216);
// DEFAULT-NEXT:         let %402: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%401), from_bool<i32, reason=promotion>(ne<ptr<const i8>>(pointer_cast<ptr<const i8>, reason=explicit>(call<ptr<i8>, signature=fn(ptr<const i8>, ptr<const i8>) -> ptr<i8>>(%73, read<ptr<const i8>>(%213), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%328)))), null<ptr<const i8>>)));
// DEFAULT-NEXT:         write<i32>(%216, read<i32>(%402));
// DEFAULT-NEXT:         let %403: i32 [synthetic] = read<i32>(%216);
// DEFAULT-NEXT:         let %404: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%403), from_bool<i32, reason=promotion>(ne<ptr<i8>>(call<ptr<i8>, signature=fn(ptr<const i8>, ptr<const i8>) -> ptr<i8>>(%73, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(12)>(%214)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%329))), null<ptr<i8>>)));
// DEFAULT-NEXT:         write<i32>(%216, read<i32>(%404));
// DEFAULT-NEXT:         return read<i32>(%216);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %219 @c23_time() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %220 timestamp: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(0));
// DEFAULT-NEXT:         let %221 utc: @type19 [storage=automatic] = aggregate<@type19, zero_fill=true>();
// DEFAULT-NEXT:         let %222 local: @type19 [storage=automatic] = aggregate<@type19, zero_fill=true>();
// DEFAULT-NEXT:         let %223 resolution: @type18 [storage=automatic] = aggregate<@type18, zero_fill=true>();
// DEFAULT-NEXT:         let %224 month: array<i8, 32> [storage=automatic] [align=16] = aggregate<array<i8, 32>, zero_fill=true>();
// DEFAULT-NEXT:         let %225 wide_month: array<i32, 32> [storage=automatic] [align=16] = aggregate<array<i32, 32>, zero_fill=true>();
// DEFAULT-NEXT:         let %226 total: i32 [storage=automatic] = from_bool<i32, reason=assign>(eq<ptr<@type19>>(call<ptr<@type19>, signature=fn(ptr<const i64>, ptr<@type19>) -> ptr<@type19>>(%82, pointer_cast<ptr<const i64>, reason=arg>(addr_of<ptr<i64>>(%220)), addr_of<ptr<@type19>>(%221)), addr_of<ptr<@type19>>(%221)));
// DEFAULT-NEXT:         let %405: i32 [synthetic] = read<i32>(%226);
// DEFAULT-NEXT:         let %406: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%405), from_bool<i32, reason=promotion>(eq<ptr<@type19>>(call<ptr<@type19>, signature=fn(ptr<const i64>, ptr<@type19>) -> ptr<@type19>>(%85, pointer_cast<ptr<const i64>, reason=arg>(addr_of<ptr<i64>>(%220)), addr_of<ptr<@type19>>(%222)), addr_of<ptr<@type19>>(%222))));
// DEFAULT-NEXT:         write<i32>(%226, read<i32>(%406));
// DEFAULT-NEXT:         let %407: i32 [synthetic] = read<i32>(%226);
// DEFAULT-NEXT:         let %408: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%407), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<@type18>, i32) -> i32>(%90, addr_of<ptr<@type18>>(%223), const<i32>(1)), const<i32>(1))));
// DEFAULT-NEXT:         write<i32>(%226, read<i32>(%408));
// DEFAULT-NEXT:         let %409: i32 [synthetic] = read<i32>(%226);
// DEFAULT-NEXT:         let %410: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%409), from_bool<i32, reason=promotion>(logical_or<bool>(gt<i64>(read<i64>(field0(%223)), widen<i64, reason=usual_arith>(const<i32>(0))), gt<i64>(read<i64>(field1(%223)), widen<i64, reason=usual_arith>(const<i32>(0))))));
// DEFAULT-NEXT:         write<i32>(%226, read<i32>(%410));
// DEFAULT-NEXT:         let %411: i32 [synthetic] = read<i32>(%226);
// DEFAULT-NEXT:         let %412: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%411), from_bool<i32, reason=promotion>(eq<i64>(call<i64, signature=fn(ptr<@type19>) -> i64>(%87, addr_of<ptr<@type19>>(%221)), widen<i64, reason=usual_arith>(const<i32>(0)))));
// DEFAULT-NEXT:         write<i32>(%226, read<i32>(%412));
// DEFAULT-NEXT:         let %413: i32 [synthetic] = read<i32>(%226);
// DEFAULT-NEXT:         let %414: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%413), from_bool<i32, reason=promotion>(eq<u64>(call<u64, signature=fn(ptr<i8>, u64, ptr<const i8>, ptr<const @type19>) -> u64>(%79, array_decay<ptr<i8>, length=Some(32)>(%224), const<u64>(32), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%330)), pointer_cast<ptr<const @type19>, reason=arg>(addr_of<ptr<@type19>>(%221))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(7))))));
// DEFAULT-NEXT:         write<i32>(%226, read<i32>(%414));
// DEFAULT-NEXT:         let %415: i32 [synthetic] = read<i32>(%226);
// DEFAULT-NEXT:         let %416: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%415), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%62, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(32)>(%224)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%331))), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%226, read<i32>(%416));
// DEFAULT-NEXT:         let %417: i32 [synthetic] = read<i32>(%226);
// DEFAULT-NEXT:         let %418: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%417), from_bool<i32, reason=promotion>(eq<u64>(call<u64, signature=fn(ptr<i32>, u64, ptr<const i32>, ptr<const @type19>) -> u64>(%115, array_decay<ptr<i32>, length=Some(32)>(%225), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(32))), pointer_cast<ptr<const i32>, reason=arg>(array_decay<ptr<i32>, length=Some(4)>(%332)), pointer_cast<ptr<const @type19>, reason=arg>(addr_of<ptr<@type19>>(%221))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(7))))));
// DEFAULT-NEXT:         write<i32>(%226, read<i32>(%418));
// DEFAULT-NEXT:         let %419: i32 [synthetic] = read<i32>(%226);
// DEFAULT-NEXT:         let %420: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%419), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const i32>, ptr<const i32>) -> i32>(%104, pointer_cast<ptr<const i32>, reason=arg>(array_decay<ptr<i32>, length=Some(32)>(%225)), pointer_cast<ptr<const i32>, reason=arg>(array_decay<ptr<i32>, length=Some(8)>(%333))), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%226, read<i32>(%420));
// DEFAULT-NEXT:         let %227 const_month: ptr<const i32> [storage=automatic] = pointer_cast<ptr<const i32>, reason=assign>(array_decay<ptr<i32>, length=Some(32)>(%225));
// DEFAULT-NEXT:         let %421: i32 [synthetic] = read<i32>(%226);
// DEFAULT-NEXT:         let %422: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%421), from_bool<i32, reason=promotion>(ne<ptr<const i32>>(pointer_cast<ptr<const i32>, reason=explicit>(call<ptr<i32>, signature=fn(ptr<const i32>, i32) -> ptr<i32>>(%107, read<ptr<const i32>>(%227), const<i32>(110))), null<ptr<const i32>>)));
// DEFAULT-NEXT:         write<i32>(%226, read<i32>(%422));
// DEFAULT-NEXT:         let %423: i32 [synthetic] = read<i32>(%226);
// DEFAULT-NEXT:         let %424: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%423), from_bool<i32, reason=promotion>(ne<ptr<i32>>(call<ptr<i32>, signature=fn(ptr<const i32>, i32) -> ptr<i32>>(%107, pointer_cast<ptr<const i32>, reason=arg>(array_decay<ptr<i32>, length=Some(32)>(%225)), const<i32>(110)), null<ptr<i32>>)));
// DEFAULT-NEXT:         write<i32>(%226, read<i32>(%424));
// DEFAULT-NEXT:         let %425: i32 [synthetic] = read<i32>(%226);
// DEFAULT-NEXT:         let %426: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%425), from_bool<i32, reason=promotion>(ne<ptr<const i32>>(pointer_cast<ptr<const i32>, reason=explicit>(call<ptr<i32>, signature=fn(ptr<const i32>, ptr<const i32>) -> ptr<i32>>(%110, read<ptr<const i32>>(%227), pointer_cast<ptr<const i32>, reason=arg>(array_decay<ptr<i32>, length=Some(4)>(%334)))), null<ptr<const i32>>)));
// DEFAULT-NEXT:         write<i32>(%226, read<i32>(%426));
// DEFAULT-NEXT:         let %427: i32 [synthetic] = read<i32>(%226);
// DEFAULT-NEXT:         let %428: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%427), from_bool<i32, reason=promotion>(ne<ptr<i32>>(call<ptr<i32>, signature=fn(ptr<const i32>, ptr<const i32>) -> ptr<i32>>(%110, pointer_cast<ptr<const i32>, reason=arg>(array_decay<ptr<i32>, length=Some(32)>(%225)), pointer_cast<ptr<const i32>, reason=arg>(array_decay<ptr<i32>, length=Some(4)>(%335))), null<ptr<i32>>)));
// DEFAULT-NEXT:         write<i32>(%226, read<i32>(%428));
// DEFAULT-NEXT:         return read<i32>(%226);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %228 @c23_io() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %229 output: array<i8, 64> [storage=automatic] [align=16] = aggregate<array<i8, 64>, zero_fill=true>();
// DEFAULT-NEXT:         let %230 float_output: array<i8, 16> [storage=automatic] [align=16] = aggregate<array<i8, 16>, zero_fill=true>();
// DEFAULT-NEXT:         let %231 double_output: array<i8, 16> [storage=automatic] [align=16] = aggregate<array<i8, 16>, zero_fill=true>();
// DEFAULT-NEXT:         let %232 long_double_output: array<i8, 16> [storage=automatic] [align=16] = aggregate<array<i8, 16>, zero_fill=true>();
// DEFAULT-NEXT:         let %233 binary_value: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:         let %234 exact_value: u16 [storage=automatic] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %235 fast_value: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         let %236 written: i32 [storage=automatic] = call<i32, signature=fn(ptr<i8>, u64, ptr<const i8>, ...) -> i32>(%22, array_decay<ptr<i8>, length=Some(64)>(%229), const<u64>(64), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%336)), const<u32>(13), reinterpret<i32, reason=vararg, fits=unknown>(widen<u32, reason=vararg>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=always>(const<i32>(21))))), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(34))));
// DEFAULT-NEXT:         let %237 scanned: i32 [storage=automatic] = call<i32, signature=fn(ptr<const i8>, ptr<const i8>, ...) -> i32>(%25, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(11)>(%337)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%338)), addr_of<ptr<u32>>(%233), addr_of<ptr<u16>>(%234), addr_of<ptr<u64>>(%235));
// DEFAULT-NEXT:         let %238 floating_written: i32 [storage=automatic] = add<i32, overflow=ub>(add<i32, overflow=ub>(call<i32, signature=fn(ptr<i8>, u64, ptr<const i8>, f32) -> i32>(%37, array_decay<ptr<i8>, length=Some(16)>(%230), const<u64>(16), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%339)), const<f32>(1.5)), call<i32, signature=fn(ptr<i8>, u64, ptr<const i8>, f64) -> i32>(%32, array_decay<ptr<i8>, length=Some(16)>(%231), const<u64>(16), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%340)), const<f64>(2.5))), call<i32, signature=fn(ptr<i8>, u64, ptr<const i8>, f80) -> i32>(%42, array_decay<ptr<i8>, length=Some(16)>(%232), const<u64>(16), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%341)), const<f80>(3.5)));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(%236), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%62, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(64)>(%229)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(11)>(%342))), const<i32>(0)))), read<i32>(%237)), from_bool<i32, reason=promotion>(eq<u32>(read<u32>(%233), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(11))))), from_bool<i32, reason=promotion>(eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%234))), const<i32>(55)))), from_bool<i32, reason=promotion>(eq<u64>(read<u64>(%235), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(89)))))), read<i32>(%238)), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%62, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%230)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%343))), const<i32>(0)))), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%62, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%231)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%344))), const<i32>(0)))), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%62, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%232)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%345))), const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %239 @c23_limits() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %240 integer_widths: i32 [storage=automatic] = add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(const<i32>(1), const<i32>(8)), const<i32>(8)), const<i32>(8)), const<i32>(16)), const<i32>(16)), const<i32>(32)), const<i32>(32)), const<i32>(64)), const<i32>(64)), const<i32>(64)), const<i32>(64)), const<i32>(64)), const<i32>(64));
// DEFAULT-NEXT:         let %241 floating_limits: i32 [storage=automatic] = add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(gt<f32, exceptions=ignore>(const<f32>(1e-45), const<f32>(0.0))), from_bool<i32, reason=promotion>(gt<f64, exceptions=ignore>(const<f64>(5e-324), const<f64>(0.0)))), from_bool<i32, reason=promotion>(gt<f80, exceptions=ignore>(const<f80>(3.64519953188247460253E-4951), const<f80>(0)))), from_bool<i32, reason=promotion>(le<f32, exceptions=ignore>(const<f32>(3.4028235e38), const<f32>(3.4028235e38)))), from_bool<i32, reason=promotion>(le<f64, exceptions=ignore>(const<f64>(1.7976931348623157e308), const<f64>(1.7976931348623157e308)))), from_bool<i32, reason=promotion>(le<f80, exceptions=ignore>(const<f80>(1.18973149535723176502E+4932), const<f80>(1.18973149535723176502E+4932)))), from_bool<i32, reason=promotion>(ge<i32>(const<i32>(1), neg<i32, overflow=ub>(const<i32>(1))))), from_bool<i32, reason=promotion>(ge<i32>(const<i32>(1), neg<i32, overflow=ub>(const<i32>(1))))), from_bool<i32, reason=promotion>(ge<i32>(const<i32>(1), neg<i32, overflow=ub>(const<i32>(1))))), from_bool<i32, reason=promotion>(eq<u64>(const<u64>(4), const<u64>(4)))), from_bool<i32, reason=promotion>(eq<u64>(const<u64>(8), const<u64>(8)))), from_bool<i32, reason=promotion>(eq<u64>(const<u64>(16), const<u64>(16))));
// DEFAULT-NEXT:         let %242 header_versions: i32 [storage=automatic] = add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(eq<i64>(const<i64>(202311), const<i64>(202311))), from_bool<i32, reason=promotion>(eq<i64>(const<i64>(202311), const<i64>(202311)))), from_bool<i32, reason=promotion>(eq<i64>(const<i64>(202311), const<i64>(202311)))), from_bool<i32, reason=promotion>(eq<i64>(const<i64>(202311), const<i64>(202311)))), from_bool<i32, reason=promotion>(eq<i64>(const<i64>(202311), const<i64>(202311)))), from_bool<i32, reason=promotion>(eq<i64>(const<i64>(202311), const<i64>(202311)))), const<i32>(1));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(%240), read<i32>(%241)), read<i32>(%242));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %243 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%18, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(19)>(%346)), call<i32, signature=fn() -> i32>(%194), call<i32, signature=fn() -> i32>(%196), call<i32, signature=fn() -> i32>(%199), call<i32, signature=fn() -> i32>(%207), call<i32, signature=fn() -> i32>(%219), add<i32, overflow=ub>(call<i32, signature=fn() -> i32>(%228), call<i32, signature=fn() -> i32>(%239)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
