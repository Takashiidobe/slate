#define __STDC_WANT_LIB_EXT1__ 1

#include <complex.h>
#include <errno.h>
#include <float.h>
#include <limits.h>
#include <stdalign.h>
#include <stdatomic.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>
#include <string.h>
#include <threads.h>
#include <time.h>
#include <uchar.h>
#include <wchar.h>

#ifdef __STDC_ANALYZABLE__
#define C11_ANALYZABLE_VALUE 1
#else
#define C11_ANALYZABLE_VALUE 0
#endif

#ifdef __STDC_LIB_EXT1__
#define C11_LIB_EXT1_VALUE 1
#else
#define C11_LIB_EXT1_VALUE 0
#endif

#ifdef __STDC_NO_ATOMICS__
#define C11_ATOMICS_VALUE 0
#else
#define C11_ATOMICS_VALUE 1
#endif

#ifdef __STDC_NO_COMPLEX__
#define C11_COMPLEX_VALUE 0
#else
#define C11_COMPLEX_VALUE 1
#endif

#ifdef __STDC_NO_THREADS__
#define C11_THREADS_VALUE 0
#else
#define C11_THREADS_VALUE 1
#endif

#ifdef __STDC_NO_VLA__
#define C11_VLA_VALUE 0
#else
#define C11_VLA_VALUE 1
#endif

struct C11Anonymous {
  union {
    int    integer;
    double real;
  };
  struct {
    int x;
    int y;
  };
};

struct C11OverAligned {
  _Alignas(32) unsigned char value;
};

struct C11Temporary {
  int values[3];
};

_Alignas(64) static unsigned char c11_aligned_buffer[64];
static _Atomic int       c11_atomic_total;
static _Thread_local int c11_thread_local_value;
static int               c11_evaluation_total;
static volatile int      c11_never_flag;
static int               c11_once_total;

_Static_assert(_Alignof(struct C11OverAligned) >= 32, "over-aligned structure");
_Static_assert(sizeof(char16_t) >= 2, "UTF-16 code unit");
_Static_assert(sizeof(char32_t) >= 4, "UTF-32 code unit");

#define C11_TYPE_KIND(value)                                                   \
  _Generic((value), int: 11, double: 22, char *: 33, default: 44)

static struct C11Temporary c11_make_temporary(int base) {
  struct C11Temporary result = {{base, base + 1, base + 2}};
  return result;
}

static int c11_evaluation_step(int value) {
  c11_evaluation_total += value;
  return value * 2;
}

static int c11_thread_worker(void *argument) {
  int increment          = *(int *)argument;
  c11_thread_local_value = 29;
  errno                  = ERANGE;
  atomic_fetch_add_explicit(&c11_atomic_total, increment, memory_order_seq_cst);
  return c11_thread_local_value + (errno == ERANGE);
}

static void c11_quick_handler(void) {
  atomic_fetch_add_explicit(&c11_atomic_total, 100, memory_order_relaxed);
}

static void c11_once_handler(void) { c11_once_total += 1; }

static noreturn void c11_never_return(int status) { quick_exit(status); }

static FILE *c11_open_exclusive(const char *path) { return fopen(path, "wx"); }

int main(void) {
  static const char     utf8_text[]     = u8"\u03a9";
  static const char16_t utf16_text[]    = u"\u03a9";
  static const char32_t utf32_text[]    = U"\U0001f642";
  char16_t              utf16_character = u'\u03a9';
  char32_t              utf32_character = U'\U0001f642';
  struct C11Anonymous   anonymous       = {0};
  struct C11OverAligned aligned_object  = {0};
  mbstate_t             utf16_state     = {0};
  mbstate_t             utf32_state     = {0};
  char16_t              converted16     = 0;
  char32_t              converted32     = 0;
  char                  multibyte16[MB_LEN_MAX];
  char                  multibyte32[MB_LEN_MAX];
  struct timespec       current_time = {0, 0};
  thrd_t                thread;
  mtx_t                 mutex;
  cnd_t                 condition;
  once_flag             once_control = ONCE_FLAG_INIT;
  tss_t                 thread_key;
  int                   thread_increment = 7;
  int                   thread_result    = 0;
  int                   thread_created;
  int                   thread_joined;
  void                 *aligned_memory;
  FILE                 *exclusive_first;
  FILE                 *exclusive_second;
  double _Complex complex_value;
  int alignment_total;
  int unicode_total;
  int generic_total;
  int anonymous_total;
  int evaluation_total;
  int temporary_total;
  int static_assert_total;
  int optional_total;
  int atomic_total;
  int thread_total;
  int concurrency_total;
  int conversion_total;
  int quick_total;
  int exclusive_total;
  int timespec_total;
  int complex_total;
  int limits_total;
#ifdef __STDC_LIB_EXT1__
  char bounds_destination[4] = {0, 0, 0, 0};
  int  bounds_total;
#endif

  _Static_assert(sizeof(utf8_text) == 3, "UTF-8 literal size");

  alignment_total = (int)_Alignof(int) + (int)_Alignof(struct C11OverAligned) +
                    (((uintptr_t)c11_aligned_buffer % 64U) == 0U) +
                    (((uintptr_t)&aligned_object % 32U) == 0U);

  unicode_total = (unsigned char)utf8_text[0] + (unsigned char)utf8_text[1] +
                  utf16_text[0] + (int)utf32_text[0] + utf16_character +
                  (int)utf32_character;

  generic_total =
      C11_TYPE_KIND(1) + C11_TYPE_KIND(1.0) + C11_TYPE_KIND((char *)0);

  anonymous.integer = 31;
  anonymous.x       = 37;
  anonymous.y       = 41;
  anonymous_total   = anonymous.integer + anonymous.x + anonymous.y;

  c11_evaluation_total = 0;
  evaluation_total =
      c11_evaluation_step(2) + c11_evaluation_step(3) + c11_evaluation_total;

  temporary_total     = c11_make_temporary(43).values[1];
  static_assert_total = 1;
  optional_total = C11_ANALYZABLE_VALUE + C11_LIB_EXT1_VALUE +
                   C11_ATOMICS_VALUE + C11_COMPLEX_VALUE + C11_THREADS_VALUE +
                   C11_VLA_VALUE;

  atomic_init(&c11_atomic_total, 5);
  c11_thread_local_value = 17;
  errno                  = 0;
  thread_created = thrd_create(&thread, c11_thread_worker, &thread_increment);
  thread_joined =
      thread_created == thrd_success ? thrd_join(thread, &thread_result) : -1;
  atomic_total = atomic_load_explicit(&c11_atomic_total, memory_order_seq_cst);
  thread_total = (thread_created == thrd_success) +
                 (thread_joined == thrd_success) + thread_result +
                 c11_thread_local_value + (errno == 0);

  concurrency_total = 0;
  if (mtx_init(&mutex, mtx_plain) == thrd_success) {
    concurrency_total += 1;
    concurrency_total += mtx_lock(&mutex) == thrd_success;
    concurrency_total += mtx_unlock(&mutex) == thrd_success;
    mtx_destroy(&mutex);
  }
  if (cnd_init(&condition) == thrd_success) {
    concurrency_total += 1;
    cnd_destroy(&condition);
  }
  c11_once_total = 0;
  call_once(&once_control, c11_once_handler);
  call_once(&once_control, c11_once_handler);
  concurrency_total += c11_once_total;
  if (tss_create(&thread_key, NULL) == thrd_success) {
    concurrency_total += 1;
    concurrency_total += tss_set(thread_key, &thread_increment) == thrd_success;
    concurrency_total += tss_get(thread_key) == &thread_increment;
    tss_delete(thread_key);
  }
  concurrency_total += TSS_DTOR_ITERATIONS >= 1;

  converted16      = 0;
  converted32      = 0;
  conversion_total = (int)mbrtoc16(&converted16, "A", 1, &utf16_state) +
                     (int)c16rtomb(multibyte16, u'A', &utf16_state) +
                     (int)mbrtoc32(&converted32, "B", 1, &utf32_state) +
                     (int)c32rtomb(multibyte32, U'B', &utf32_state) +
                     converted16 + (int)converted32 + multibyte16[0] +
                     multibyte32[0];

  aligned_memory = aligned_alloc(64, 64);
  alignment_total +=
      aligned_memory != NULL && ((uintptr_t)aligned_memory % 64U) == 0U;
  free(aligned_memory);

  quick_total = at_quick_exit(c11_quick_handler) == 0;

  remove("slate-c11-exclusive.tmp");
  exclusive_first  = c11_open_exclusive("slate-c11-exclusive.tmp");
  exclusive_second = c11_open_exclusive("slate-c11-exclusive.tmp");
  exclusive_total  = exclusive_first != NULL && exclusive_second == NULL;
  if (exclusive_first != NULL) {
    fclose(exclusive_first);
  }
  if (exclusive_second != NULL) {
    fclose(exclusive_second);
  }
  remove("slate-c11-exclusive.tmp");

  timespec_total = timespec_get(&current_time, TIME_UTC) == TIME_UTC &&
                   current_time.tv_nsec >= 0 &&
                   current_time.tv_nsec < 1000000000L;

  complex_value = CMPLX(2.0, 3.0);
  complex_total = (creal(complex_value) == 2.0) + (cimag(complex_value) == 3.0);

  limits_total = (FLT_DECIMAL_DIG >= 6) + (DBL_DECIMAL_DIG >= 10) +
                 (LDBL_DECIMAL_DIG >= 10) + (FLT_TRUE_MIN > 0.0F) +
                 (DBL_TRUE_MIN > 0.0) + (LDBL_TRUE_MIN > 0.0L) +
                 (FLT_HAS_SUBNORM >= -1) + (DBL_HAS_SUBNORM >= -1) +
                 (LDBL_HAS_SUBNORM >= -1);

#ifdef __STDC_LIB_EXT1__
  bounds_total =
      memcpy_s(bounds_destination, sizeof(bounds_destination), "C11", 4) == 0 &&
      bounds_destination[2] == '1';
#else
  limits_total += 0;
#endif

  if (c11_never_flag) {
    c11_never_return(99);
  }

  printf("%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
         alignment_total, unicode_total, generic_total, anonymous_total,
         evaluation_total, temporary_total, static_assert_total, optional_total,
         atomic_total, thread_total, concurrency_total, conversion_total,
         quick_total, exclusive_total, timespec_total, complex_total,
         limits_total,
#ifdef __STDC_LIB_EXT1__
         bounds_total
#else
         0
#endif
  );
  putchar('\n');
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
// DEFAULT-NEXT:     type @type1 __uint16_t = u16;
// DEFAULT-NEXT:     type @type2 __uint32_t = u32;
// DEFAULT-NEXT:     type @type3 __uint64_t = u64;
// DEFAULT-NEXT:     type @type4 __uint_least16_t = u16;
// DEFAULT-NEXT:     type @type5 __uint_least32_t = u32;
// DEFAULT-NEXT:     type @type6 __off_t = i64;
// DEFAULT-NEXT:     type @type7 __off64_t = i64;
// DEFAULT-NEXT:     type @type8 __time_t = i64;
// DEFAULT-NEXT:     type @type9 __syscall_slong_t = i64;
// DEFAULT-NEXT:     type @type10 uintptr_t = u64;
// DEFAULT-NEXT:     type @type11 memory_order = enum : u32 {
// DEFAULT-NEXT:         %0 memory_order_relaxed = const<i32>(0);
// DEFAULT-NEXT:         %1 memory_order_consume = const<i32>(1);
// DEFAULT-NEXT:         %2 memory_order_acquire = const<i32>(2);
// DEFAULT-NEXT:         %3 memory_order_release = const<i32>(3);
// DEFAULT-NEXT:         %4 memory_order_acq_rel = const<i32>(4);
// DEFAULT-NEXT:         %5 memory_order_seq_cst = const<i32>(5);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type12 memory_order = @type11;
// DEFAULT-NEXT:     type @type13 = struct {
// DEFAULT-NEXT:         field0 __count: i32;
// DEFAULT-NEXT:         field1 __value: @type14;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type14 = union {
// DEFAULT-NEXT:         field0 __wch: u32;
// DEFAULT-NEXT:         field1 __wchb: array<i8, 4>;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type15 __mbstate_t = @type13;
// DEFAULT-NEXT:     type @type16 _IO_FILE = struct incomplete;
// DEFAULT-NEXT:     type @type17 FILE = @type16;
// DEFAULT-NEXT:     type @type18 _IO_lock_t = void;
// DEFAULT-NEXT:     type @type19 _IO_marker = struct incomplete;
// DEFAULT-NEXT:     type @type20 _IO_codecvt = struct incomplete;
// DEFAULT-NEXT:     type @type21 _IO_wide_data = struct incomplete;
// DEFAULT-NEXT:     type @type22 timespec = struct {
// DEFAULT-NEXT:         field0 tv_sec: i64;
// DEFAULT-NEXT:         field1 tv_nsec: i64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type23 __tss_t = u32;
// DEFAULT-NEXT:     type @type24 __thrd_t = u64;
// DEFAULT-NEXT:     type @type25 = struct {
// DEFAULT-NEXT:         field0 __data: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type26 __once_flag = @type25;
// DEFAULT-NEXT:     type @type27 once_flag = @type25;
// DEFAULT-NEXT:     type @type28 tss_t = u32;
// DEFAULT-NEXT:     type @type29 tss_dtor_t = ptr<fn(ptr<void>) -> void>;
// DEFAULT-NEXT:     type @type30 thrd_t = u64;
// DEFAULT-NEXT:     type @type31 thrd_start_t = ptr<fn(ptr<void>) -> i32>;
// DEFAULT-NEXT:     type @type32 = enum : u32 {
// DEFAULT-NEXT:         %0 thrd_success = const<i32>(0);
// DEFAULT-NEXT:         %1 thrd_busy = const<i32>(1);
// DEFAULT-NEXT:         %2 thrd_error = const<i32>(2);
// DEFAULT-NEXT:         %3 thrd_nomem = const<i32>(3);
// DEFAULT-NEXT:         %4 thrd_timedout = const<i32>(4);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type33 = enum : u32 {
// DEFAULT-NEXT:         %0 mtx_plain = const<i32>(0);
// DEFAULT-NEXT:         %1 mtx_recursive = const<i32>(1);
// DEFAULT-NEXT:         %2 mtx_timed = const<i32>(2);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type34 = union {
// DEFAULT-NEXT:         field0 __size: array<i8, 40>;
// DEFAULT-NEXT:         field1 __align: i64;
// DEFAULT-NEXT:     } [size=40, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type35 mtx_t = @type34;
// DEFAULT-NEXT:     type @type36 = union {
// DEFAULT-NEXT:         field0 __size: array<i8, 48>;
// DEFAULT-NEXT:         field1 __align: i64;
// DEFAULT-NEXT:     } [size=48, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type37 cnd_t = @type36;
// DEFAULT-NEXT:     type @type38 mbstate_t = @type13;
// DEFAULT-NEXT:     type @type39 char16_t = u16;
// DEFAULT-NEXT:     type @type40 char32_t = u32;
// DEFAULT-NEXT:     type @type41 C11Anonymous = struct {
// DEFAULT-NEXT:         field0 <anonymous>: @type42;
// DEFAULT-NEXT:         field1 <anonymous>: @type43;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type42 = union {
// DEFAULT-NEXT:         field0 integer: i32;
// DEFAULT-NEXT:         field1 real: f64;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type43 = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:         field1 y: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type44 C11OverAligned = struct {
// DEFAULT-NEXT:         field0 value: u8;
// DEFAULT-NEXT:     } [size=32, align=32, offsets=[0]];
// DEFAULT-NEXT:     type @type45 C11Temporary = struct {
// DEFAULT-NEXT:         field0 values: array<i32, 3>;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0]];
// DEFAULT-NEXT:     global %90 c11_aligned_buffer: array<u8, 64> [storage=static] [align=64] [linkage=internal];
// DEFAULT-NEXT:     global %91 c11_atomic_total: atomic i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %92 c11_thread_local_value: i32 [storage=thread] [linkage=internal];
// DEFAULT-NEXT:     global %93 c11_evaluation_total: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %94 c11_never_flag: volatile i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %95 c11_once_total: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %206 .str206: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([119, 120, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %111 utf8_text: array<i8, 3> [storage=static] [const] = code_units<array<i8, 3>>([206, 169, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %112 utf16_text: array<u16, 2> [storage=static] [const] = code_units<array<u16, 2>>([937, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %113 utf32_text: array<u32, 2> [storage=static] [const] = code_units<array<u32, 2>>([128578, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %208 .str208: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([65, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %209 .str209: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([66, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %210 .str210: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 108, 97, 116, 101, 45, 99, 49, 49, 45, 101, 120, 99, 108, 117, 115, 105, 118, 101, 46, 116, 109, 112, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %211 .str211: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 108, 97, 116, 101, 45, 99, 49, 49, 45, 101, 120, 99, 108, 117, 115, 105, 118, 101, 46, 116, 109, 112, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %212 .str212: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 108, 97, 116, 101, 45, 99, 49, 49, 45, 101, 120, 99, 108, 117, 115, 105, 118, 101, 46, 116, 109, 112, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %213 .str213: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 108, 97, 116, 101, 45, 99, 49, 49, 45, 101, 120, 99, 108, 117, 115, 105, 118, 101, 46, 116, 109, 112, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %214 .str214: array<i8, 54> [storage=static] = code_units<array<i8, 54>>([37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @cimag(%155 __z: complex<f64>) -> f64 [linkage=external] [memory=none] [abi=sysv64(native_c) -> scalar];
// DEFAULT-NEXT:     fn %1 @creal(%156 __z: complex<f64>) -> f64 [linkage=external] [memory=none] [abi=sysv64(native_c) -> scalar];
// DEFAULT-NEXT:     fn %2 @__errno_location() -> ptr<i32> [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %31 @remove(%157 __filename: ptr<const i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %32 @fclose(%158 __stream: ptr<@type16>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %33 @fopen(%159 __filename: ptr<const i8> [restrict], %160 __modes: ptr<const i8> [restrict]) -> ptr<@type16> [linkage=external];
// DEFAULT-NEXT:     fn %34 @printf(%161 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %35 @putchar(%162 __c: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %41 @free(%163 __ptr: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %42 @aligned_alloc(%164 __alignment: u64, %165 __size: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %43 @at_quick_exit(%166 __func: ptr<fn() -> void>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %44 @quick_exit(%167 __status: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %46 @call_once(%168 __flag: ptr<@type25>, %169 __func: ptr<fn() -> void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %47 @timespec_get(%170 __ts: ptr<@type22>, %171 __base: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %66 @thrd_create(%172 __thr: ptr<u64>, %173 __func: ptr<fn(ptr<void>) -> i32>, %174 __arg: ptr<void>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %67 @thrd_join(%175 __thr: u64, %176 __res: ptr<i32>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %68 @mtx_init(%177 __mutex: ptr<@type34>, %178 __type: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %69 @mtx_lock(%179 __mutex: ptr<@type34>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %70 @mtx_unlock(%180 __mutex: ptr<@type34>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %71 @mtx_destroy(%181 __mutex: ptr<@type34>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %72 @cnd_init(%184 __cond: ptr<@type36>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %73 @cnd_destroy(%185 __COND: ptr<@type36>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %74 @tss_create(%186 __tss_id: ptr<u32>, %187 __destructor: ptr<fn(ptr<void>) -> void>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %75 @tss_get(%188 __tss_id: u32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %76 @tss_set(%189 __tss_id: u32, %190 __val: ptr<void>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %77 @tss_delete(%191 __tss_id: u32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %81 @mbrtoc16(%192 __pc16: ptr<u16> [restrict], %193 __s: ptr<const i8> [restrict], %194 __n: u64, %195 __p: ptr<@type13> [restrict]) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %82 @c16rtomb(%196 __s: ptr<i8> [restrict], %197 __c16: u16, %198 __ps: ptr<@type13> [restrict]) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %83 @mbrtoc32(%199 __pc32: ptr<u32> [restrict], %200 __s: ptr<const i8> [restrict], %201 __n: u64, %202 __p: ptr<@type13> [restrict]) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %84 @c32rtomb(%203 __s: ptr<i8> [restrict], %204 __c32: u32, %205 __ps: ptr<@type13> [restrict]) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %96 @c11_make_temporary(%97 base: i32) -> @type45 [linkage=internal] [abi=sysv64(scalar) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %98 result: @type45 [storage=automatic] = aggregate<@type45, zero_fill=false>(field0 = aggregate<array<i32, 3>, zero_fill=false>(index0 = read<i32>(%97), index1 = add<i32, overflow=ub>(read<i32>(%97), const<i32>(1)), index2 = add<i32, overflow=ub>(read<i32>(%97), const<i32>(2))));
// DEFAULT-NEXT:         return copy<@type45, reason=return>(read<@type45>(%98));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %99 @c11_evaluation_step(%100 value: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %215: i32 [synthetic] = read<i32>(%93);
// DEFAULT-NEXT:         let %216: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%215), read<i32>(%100));
// DEFAULT-NEXT:         write<i32>(%93, read<i32>(%216));
// DEFAULT-NEXT:         return mul<i32, overflow=ub>(read<i32>(%100), const<i32>(2));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %101 @c11_thread_worker(%102 argument: ptr<void>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %103 increment: i32 [storage=automatic] = read<i32>(deref(pointer_cast<ptr<i32>, reason=explicit>(read<ptr<void>>(%102))));
// DEFAULT-NEXT:         write<i32>(%92, const<i32>(29));
// DEFAULT-NEXT:         write<i32>(deref(call<ptr<i32>, signature=fn() -> ptr<i32>>(%2)), const<i32>(34));
// DEFAULT-NEXT:         let %217: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i32>>(%91)), add<i32, overflow=wrap>(old<i32>, read<i32>(%103)));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%92), from_bool<i32, reason=promotion>(eq<i32>(read<i32>(deref(call<ptr<i32>, signature=fn() -> ptr<i32>>(%2))), const<i32>(34))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %104 @c11_quick_handler() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %218: i32 [synthetic] = update<i32, result=old, atomic=relaxed>(deref(addr_of<ptr<atomic i32>>(%91)), add<i32, overflow=wrap>(old<i32>, const<i32>(100)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %105 @c11_once_handler() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %219: i32 [synthetic] = read<i32>(%95);
// DEFAULT-NEXT:         let %220: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%219), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%95, read<i32>(%220));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %106 @c11_never_return(%107 status: i32) -> void [linkage=internal] [noreturn] [fallthrough=ub] {
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%44, read<i32>(%107));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %108 @c11_open_exclusive(%109 path: ptr<const i8>) -> ptr<@type16> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<ptr<@type16>, signature=fn(ptr<const i8>, ptr<const i8>) -> ptr<@type16>>(%33, read<ptr<const i8>>(%109), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%206)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %110 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %114 utf16_character: u16 [storage=automatic] = const<u16>(937);
// DEFAULT-NEXT:         let %115 utf32_character: u32 [storage=automatic] = const<u32>(128578);
// DEFAULT-NEXT:         let %116 anonymous: @type41 [storage=automatic] = aggregate<@type41, zero_fill=true>(field0 = aggregate<@type42, zero_fill=false>(field0 = const<i32>(0)));
// DEFAULT-NEXT:         let %117 aligned_object: @type44 [storage=automatic] = aggregate<@type44, zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         let %118 utf16_state: @type13 [storage=automatic] = aggregate<@type13, zero_fill=true>(field0 = const<i32>(0));
// DEFAULT-NEXT:         let %119 utf32_state: @type13 [storage=automatic] = aggregate<@type13, zero_fill=true>(field0 = const<i32>(0));
// DEFAULT-NEXT:         let %120 converted16: u16 [storage=automatic] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %121 converted32: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:         let %122 multibyte16: array<i8, 16> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %123 multibyte32: array<i8, 16> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %124 current_time: @type22 [storage=automatic] = aggregate<@type22, zero_fill=false>(field0 = widen<i64, reason=assign>(const<i32>(0)), field1 = widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         let %125 thread: u64 [storage=automatic];
// DEFAULT-NEXT:         let %126 mutex: @type34 [storage=automatic];
// DEFAULT-NEXT:         let %127 condition: @type36 [storage=automatic];
// DEFAULT-NEXT:         let %128 once_control: @type25 [storage=automatic] = aggregate<@type25, zero_fill=false>(field0 = const<i32>(0));
// DEFAULT-NEXT:         let %129 thread_key: u32 [storage=automatic];
// DEFAULT-NEXT:         let %130 thread_increment: i32 [storage=automatic] = const<i32>(7);
// DEFAULT-NEXT:         let %131 thread_result: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %132 thread_created: i32 [storage=automatic];
// DEFAULT-NEXT:         let %133 thread_joined: i32 [storage=automatic];
// DEFAULT-NEXT:         let %134 aligned_memory: ptr<void> [storage=automatic];
// DEFAULT-NEXT:         let %135 exclusive_first: ptr<@type16> [storage=automatic];
// DEFAULT-NEXT:         let %136 exclusive_second: ptr<@type16> [storage=automatic];
// DEFAULT-NEXT:         let %137 complex_value: complex<f64> [storage=automatic];
// DEFAULT-NEXT:         let %138 alignment_total: i32 [storage=automatic];
// DEFAULT-NEXT:         let %139 unicode_total: i32 [storage=automatic];
// DEFAULT-NEXT:         let %140 generic_total: i32 [storage=automatic];
// DEFAULT-NEXT:         let %141 anonymous_total: i32 [storage=automatic];
// DEFAULT-NEXT:         let %142 evaluation_total: i32 [storage=automatic];
// DEFAULT-NEXT:         let %143 temporary_total: i32 [storage=automatic];
// DEFAULT-NEXT:         let %144 static_assert_total: i32 [storage=automatic];
// DEFAULT-NEXT:         let %145 optional_total: i32 [storage=automatic];
// DEFAULT-NEXT:         let %146 atomic_total: i32 [storage=automatic];
// DEFAULT-NEXT:         let %147 thread_total: i32 [storage=automatic];
// DEFAULT-NEXT:         let %148 concurrency_total: i32 [storage=automatic];
// DEFAULT-NEXT:         let %149 conversion_total: i32 [storage=automatic];
// DEFAULT-NEXT:         let %150 quick_total: i32 [storage=automatic];
// DEFAULT-NEXT:         let %151 exclusive_total: i32 [storage=automatic];
// DEFAULT-NEXT:         let %152 timespec_total: i32 [storage=automatic];
// DEFAULT-NEXT:         let %153 complex_total: i32 [storage=automatic];
// DEFAULT-NEXT:         let %154 limits_total: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%138, add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=always>(const<u64>(4))), reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=always>(const<u64>(32)))), from_bool<i32, reason=promotion>(eq<u64>(rem<u64, by_zero=ub>(ptr_to_int<u64, reason=explicit>(array_decay<ptr<u8>, length=Some(64)>(%90)), widen<u64, reason=usual_arith>(const<u32>(64))), widen<u64, reason=usual_arith>(const<u32>(0))))), from_bool<i32, reason=promotion>(eq<u64>(rem<u64, by_zero=ub>(ptr_to_int<u64, reason=explicit>(addr_of<ptr<@type44>>(%117)), widen<u64, reason=usual_arith>(const<u32>(32))), widen<u64, reason=usual_arith>(const<u32>(0))))));
// DEFAULT-NEXT:         write<i32>(%139, add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(%111), const<i32>(0))))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(%111), const<i32>(1)))))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(deref(ptr_offset<ptr<const u16>, subtract=false, element=u16, overflow=ub>(array_decay<ptr<const u16>, length=Some(2)>(%112), const<i32>(0))))))), reinterpret<i32, reason=explicit, fits=unknown>(read<u32>(deref(ptr_offset<ptr<const u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<const u32>, length=Some(2)>(%113), const<i32>(0)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%114)))), reinterpret<i32, reason=explicit, fits=unknown>(read<u32>(%115))));
// DEFAULT-NEXT:         write<i32>(%140, add<i32, overflow=ub>(add<i32, overflow=ub>(const<i32>(11), const<i32>(22)), const<i32>(33)));
// DEFAULT-NEXT:         write<i32>(field0(field0(%116)), const<i32>(31));
// DEFAULT-NEXT:         write<i32>(field0(field1(%116)), const<i32>(37));
// DEFAULT-NEXT:         write<i32>(field1(field1(%116)), const<i32>(41));
// DEFAULT-NEXT:         write<i32>(%141, add<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(field0(field0(%116))), read<i32>(field0(field1(%116)))), read<i32>(field1(field1(%116)))));
// DEFAULT-NEXT:         write<i32>(%93, const<i32>(0));
// DEFAULT-NEXT:         write<i32>(%142, add<i32, overflow=ub>(add<i32, overflow=ub>(call<i32, signature=fn(i32) -> i32>(%99, const<i32>(2)), call<i32, signature=fn(i32) -> i32>(%99, const<i32>(3))), read<i32>(%93)));
// DEFAULT-NEXT:         add<i32, overflow=ub>(add<i32, overflow=ub>(call<i32, signature=fn(i32) -> i32>(%99, const<i32>(2)), call<i32, signature=fn(i32) -> i32>(%99, const<i32>(3))), read<i32>(%93));
// DEFAULT-NEXT:         write<i32>(%143, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(field0(temporary %207 = call<@type45, signature=fn(i32) -> @type45, abi=sysv64(scalar) -> native_c>(%96, const<i32>(43)))), const<i32>(1)))));
// DEFAULT-NEXT:         read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(field0(temporary %207 = call<@type45, signature=fn(i32) -> @type45, abi=sysv64(scalar) -> native_c>(%96, const<i32>(43)))), const<i32>(1))));
// DEFAULT-NEXT:         write<i32>(%144, const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%145, add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(const<i32>(0), const<i32>(0)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)));
// DEFAULT-NEXT:         write<i32>(deref(addr_of<ptr<atomic i32>>(%91)), const<i32>(5));
// DEFAULT-NEXT:         write<i32>(%92, const<i32>(17));
// DEFAULT-NEXT:         write<i32>(deref(call<ptr<i32>, signature=fn() -> ptr<i32>>(%2)), const<i32>(0));
// DEFAULT-NEXT:         write<i32>(%132, call<i32, signature=fn(ptr<u64>, ptr<fn(ptr<void>) -> i32>, ptr<void>) -> i32>(%66, addr_of<ptr<u64>>(%125), function_decay<ptr<fn(ptr<void>) -> i32>>(%101), pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i32>>(%130))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<u64>, ptr<fn(ptr<void>) -> i32>, ptr<void>) -> i32>(%66, addr_of<ptr<u64>>(%125), function_decay<ptr<fn(ptr<void>) -> i32>>(%101), pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i32>>(%130)));
// DEFAULT-NEXT:         let %221: i32 [synthetic];
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%132), const<i32>(0))
// DEFAULT-NEXT:             write<i32>(%221, call<i32, signature=fn(u64, ptr<i32>) -> i32>(%67, read<u64>(%125), addr_of<ptr<i32>>(%131)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<i32>(%221, neg<i32, overflow=ub>(const<i32>(1)));
// DEFAULT-NEXT:         write<i32>(%133, read<i32>(%221));
// DEFAULT-NEXT:         write<i32>(%146, read<i32, atomic=seq_cst>(deref(addr_of<ptr<atomic i32>>(%91))));
// DEFAULT-NEXT:         write<i32>(%147, add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(eq<i32>(read<i32>(%132), const<i32>(0))), from_bool<i32, reason=promotion>(eq<i32>(read<i32>(%133), const<i32>(0)))), read<i32>(%131)), read<i32>(%92)), from_bool<i32, reason=promotion>(eq<i32>(read<i32>(deref(call<ptr<i32>, signature=fn() -> ptr<i32>>(%2))), const<i32>(0)))));
// DEFAULT-NEXT:         write<i32>(%148, const<i32>(0));
// DEFAULT-NEXT:         if eq<i32>(call<i32, signature=fn(ptr<@type34>, i32) -> i32>(%68, addr_of<ptr<@type34>>(%126), const<i32>(0)), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %222: i32 [synthetic] = read<i32>(%148);
// DEFAULT-NEXT:                 let %223: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%222), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%148, read<i32>(%223));
// DEFAULT-NEXT:                 let %224: i32 [synthetic] = read<i32>(%148);
// DEFAULT-NEXT:                 let %225: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%224), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<@type34>) -> i32>(%69, addr_of<ptr<@type34>>(%126)), const<i32>(0))));
// DEFAULT-NEXT:                 write<i32>(%148, read<i32>(%225));
// DEFAULT-NEXT:                 let %226: i32 [synthetic] = read<i32>(%148);
// DEFAULT-NEXT:                 let %227: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%226), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<@type34>) -> i32>(%70, addr_of<ptr<@type34>>(%126)), const<i32>(0))));
// DEFAULT-NEXT:                 write<i32>(%148, read<i32>(%227));
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<@type34>) -> void>(%71, addr_of<ptr<@type34>>(%126));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         if eq<i32>(call<i32, signature=fn(ptr<@type36>) -> i32>(%72, addr_of<ptr<@type36>>(%127)), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %228: i32 [synthetic] = read<i32>(%148);
// DEFAULT-NEXT:                 let %229: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%228), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%148, read<i32>(%229));
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<@type36>) -> void>(%73, addr_of<ptr<@type36>>(%127));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         write<i32>(%95, const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type25>, ptr<fn() -> void>) -> void>(%46, addr_of<ptr<@type25>>(%128), function_decay<ptr<fn() -> void>>(%105));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type25>, ptr<fn() -> void>) -> void>(%46, addr_of<ptr<@type25>>(%128), function_decay<ptr<fn() -> void>>(%105));
// DEFAULT-NEXT:         let %230: i32 [synthetic] = read<i32>(%148);
// DEFAULT-NEXT:         let %231: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%230), read<i32>(%95));
// DEFAULT-NEXT:         write<i32>(%148, read<i32>(%231));
// DEFAULT-NEXT:         if eq<i32>(call<i32, signature=fn(ptr<u32>, ptr<fn(ptr<void>) -> void>) -> i32>(%74, addr_of<ptr<u32>>(%129), null<ptr<fn(ptr<void>) -> void>>), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %232: i32 [synthetic] = read<i32>(%148);
// DEFAULT-NEXT:                 let %233: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%232), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%148, read<i32>(%233));
// DEFAULT-NEXT:                 let %234: i32 [synthetic] = read<i32>(%148);
// DEFAULT-NEXT:                 let %235: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%234), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(u32, ptr<void>) -> i32>(%76, read<u32>(%129), pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i32>>(%130))), const<i32>(0))));
// DEFAULT-NEXT:                 write<i32>(%148, read<i32>(%235));
// DEFAULT-NEXT:                 let %236: i32 [synthetic] = read<i32>(%148);
// DEFAULT-NEXT:                 let %237: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%236), from_bool<i32, reason=promotion>(eq<ptr<void>>(call<ptr<void>, signature=fn(u32) -> ptr<void>>(%75, read<u32>(%129)), pointer_cast<ptr<void>, reason=usual_arith>(addr_of<ptr<i32>>(%130)))));
// DEFAULT-NEXT:                 write<i32>(%148, read<i32>(%237));
// DEFAULT-NEXT:                 call<void, signature=fn(u32) -> void>(%77, read<u32>(%129));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         let %238: i32 [synthetic] = read<i32>(%148);
// DEFAULT-NEXT:         let %239: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%238), from_bool<i32, reason=promotion>(ge<i32>(const<i32>(4), const<i32>(1))));
// DEFAULT-NEXT:         write<i32>(%148, read<i32>(%239));
// DEFAULT-NEXT:         write<u16>(%120, reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         write<u32>(%121, reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<i32>(%149, add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(call<u64, signature=fn(ptr<u16>, ptr<const i8>, u64, ptr<@type13>) -> u64>(%81, addr_of<ptr<u16>>(%120), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%208)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), addr_of<ptr<@type13>>(%118)))), reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(call<u64, signature=fn(ptr<i8>, u16, ptr<@type13>) -> u64>(%82, array_decay<ptr<i8>, length=Some(16)>(%122), const<u16>(65), addr_of<ptr<@type13>>(%118))))), reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(call<u64, signature=fn(ptr<u32>, ptr<const i8>, u64, ptr<@type13>) -> u64>(%83, addr_of<ptr<u32>>(%121), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%209)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), addr_of<ptr<@type13>>(%119))))), reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(call<u64, signature=fn(ptr<i8>, u32, ptr<@type13>) -> u64>(%84, array_decay<ptr<i8>, length=Some(16)>(%123), const<u32>(66), addr_of<ptr<@type13>>(%119))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%120)))), reinterpret<i32, reason=explicit, fits=unknown>(read<u32>(%121))), widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(16)>(%122), const<i32>(0)))))), widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(16)>(%123), const<i32>(0)))))));
// DEFAULT-NEXT:         add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(call<u64, signature=fn(ptr<u16>, ptr<const i8>, u64, ptr<@type13>) -> u64>(%81, addr_of<ptr<u16>>(%120), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%208)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), addr_of<ptr<@type13>>(%118)))), reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(call<u64, signature=fn(ptr<i8>, u16, ptr<@type13>) -> u64>(%82, array_decay<ptr<i8>, length=Some(16)>(%122), const<u16>(65), addr_of<ptr<@type13>>(%118))))), reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(call<u64, signature=fn(ptr<u32>, ptr<const i8>, u64, ptr<@type13>) -> u64>(%83, addr_of<ptr<u32>>(%121), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%209)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), addr_of<ptr<@type13>>(%119))))), reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(call<u64, signature=fn(ptr<i8>, u32, ptr<@type13>) -> u64>(%84, array_decay<ptr<i8>, length=Some(16)>(%123), const<u32>(66), addr_of<ptr<@type13>>(%119))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%120)))), reinterpret<i32, reason=explicit, fits=unknown>(read<u32>(%121))), widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(16)>(%122), const<i32>(0)))))), widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(16)>(%123), const<i32>(0))))));
// DEFAULT-NEXT:         write<ptr<void>>(%134, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%42, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(64))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(64)))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%42, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(64))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(64))));
// DEFAULT-NEXT:         let %240: i32 [synthetic] = read<i32>(%138);
// DEFAULT-NEXT:         let %241: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%240), from_bool<i32, reason=promotion>(logical_and<bool>(ne<ptr<void>>(read<ptr<void>>(%134), null<ptr<void>>), eq<u64>(rem<u64, by_zero=ub>(ptr_to_int<u64, reason=explicit>(read<ptr<void>>(%134)), widen<u64, reason=usual_arith>(const<u32>(64))), widen<u64, reason=usual_arith>(const<u32>(0))))));
// DEFAULT-NEXT:         write<i32>(%138, read<i32>(%241));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%41, read<ptr<void>>(%134));
// DEFAULT-NEXT:         write<i32>(%150, from_bool<i32, reason=assign>(eq<i32>(call<i32, signature=fn(ptr<fn() -> void>) -> i32>(%43, function_decay<ptr<fn() -> void>>(%104)), const<i32>(0))));
// DEFAULT-NEXT:         from_bool<i32, reason=assign>(eq<i32>(call<i32, signature=fn(ptr<fn() -> void>) -> i32>(%43, function_decay<ptr<fn() -> void>>(%104)), const<i32>(0)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>) -> i32>(%31, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(24)>(%210)));
// DEFAULT-NEXT:         write<ptr<@type16>>(%135, call<ptr<@type16>, signature=fn(ptr<const i8>) -> ptr<@type16>>(%108, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(24)>(%211))));
// DEFAULT-NEXT:         call<ptr<@type16>, signature=fn(ptr<const i8>) -> ptr<@type16>>(%108, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(24)>(%211)));
// DEFAULT-NEXT:         write<ptr<@type16>>(%136, call<ptr<@type16>, signature=fn(ptr<const i8>) -> ptr<@type16>>(%108, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(24)>(%212))));
// DEFAULT-NEXT:         call<ptr<@type16>, signature=fn(ptr<const i8>) -> ptr<@type16>>(%108, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(24)>(%212)));
// DEFAULT-NEXT:         write<i32>(%151, from_bool<i32, reason=assign>(logical_and<bool>(ne<ptr<@type16>>(read<ptr<@type16>>(%135), null<ptr<@type16>>), eq<ptr<@type16>>(read<ptr<@type16>>(%136), null<ptr<@type16>>))));
// DEFAULT-NEXT:         if ne<ptr<@type16>>(read<ptr<@type16>>(%135), null<ptr<@type16>>)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<@type16>) -> i32>(%32, read<ptr<@type16>>(%135));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         if ne<ptr<@type16>>(read<ptr<@type16>>(%136), null<ptr<@type16>>)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<@type16>) -> i32>(%32, read<ptr<@type16>>(%136));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>) -> i32>(%31, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(24)>(%213)));
// DEFAULT-NEXT:         write<i32>(%152, from_bool<i32, reason=assign>(logical_and<bool>(logical_and<bool>(eq<i32>(call<i32, signature=fn(ptr<@type22>, i32) -> i32>(%47, addr_of<ptr<@type22>>(%124), const<i32>(1)), const<i32>(1)), ge<i64>(read<i64>(field1(%124)), widen<i64, reason=usual_arith>(const<i32>(0)))), lt<i64>(read<i64>(field1(%124)), const<i64>(1000000000)))));
// DEFAULT-NEXT:         from_bool<i32, reason=assign>(logical_and<bool>(logical_and<bool>(eq<i32>(call<i32, signature=fn(ptr<@type22>, i32) -> i32>(%47, addr_of<ptr<@type22>>(%124), const<i32>(1)), const<i32>(1)), ge<i64>(read<i64>(field1(%124)), widen<i64, reason=usual_arith>(const<i32>(0)))), lt<i64>(read<i64>(field1(%124)), const<i64>(1000000000))));
// DEFAULT-NEXT:         write<complex<f64>>(%137, aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.0), index1 = const<f64>(3.0)));
// DEFAULT-NEXT:         write<i32>(%153, add<i32, overflow=ub>(from_bool<i32, reason=promotion>(eq<f64, exceptions=ignore>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(native_c) -> scalar>(%1, read<complex<f64>>(%137)), const<f64>(2.0))), from_bool<i32, reason=promotion>(eq<f64, exceptions=ignore>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(native_c) -> scalar>(%0, read<complex<f64>>(%137)), const<f64>(3.0)))));
// DEFAULT-NEXT:         write<i32>(%154, add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ge<i32>(const<i32>(9), const<i32>(6))), from_bool<i32, reason=promotion>(ge<i32>(const<i32>(17), const<i32>(10)))), from_bool<i32, reason=promotion>(ge<i32>(const<i32>(21), const<i32>(10)))), from_bool<i32, reason=promotion>(gt<f32, exceptions=ignore>(const<f32>(1e-45), const<f32>(0.0)))), from_bool<i32, reason=promotion>(gt<f64, exceptions=ignore>(const<f64>(5e-324), const<f64>(0.0)))), from_bool<i32, reason=promotion>(gt<f80, exceptions=ignore>(const<f80>(3.64519953188247460253E-4951), const<f80>(0)))), from_bool<i32, reason=promotion>(ge<i32>(const<i32>(1), neg<i32, overflow=ub>(const<i32>(1))))), from_bool<i32, reason=promotion>(ge<i32>(const<i32>(1), neg<i32, overflow=ub>(const<i32>(1))))), from_bool<i32, reason=promotion>(ge<i32>(const<i32>(1), neg<i32, overflow=ub>(const<i32>(1))))));
// DEFAULT-NEXT:         let %242: i32 [synthetic] = read<i32>(%154);
// DEFAULT-NEXT:         let %243: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%242), const<i32>(0));
// DEFAULT-NEXT:         write<i32>(%154, read<i32>(%243));
// DEFAULT-NEXT:         if ne<i32>(read<i32, volatile>(%94), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<void, signature=fn(i32) -> void>(%106, const<i32>(99));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%34, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(54)>(%214)), read<i32>(%138), read<i32>(%139), read<i32>(%140), read<i32>(%141), read<i32>(%142), read<i32>(%143), read<i32>(%144), read<i32>(%145), read<i32>(%146), read<i32>(%147), read<i32>(%148), read<i32>(%149), read<i32>(%150), read<i32>(%151), read<i32>(%152), read<i32>(%153), read<i32>(%154), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(i32) -> i32>(%35, const<i32>(10));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
