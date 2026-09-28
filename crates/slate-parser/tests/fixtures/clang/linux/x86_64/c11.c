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
// DEFAULT-NEXT:     type @type16 _IO_FILE = struct {
// DEFAULT-NEXT:         field0 _flags: i32;
// DEFAULT-NEXT:         field1 _IO_read_ptr: ptr<i8>;
// DEFAULT-NEXT:         field2 _IO_read_end: ptr<i8>;
// DEFAULT-NEXT:         field3 _IO_read_base: ptr<i8>;
// DEFAULT-NEXT:         field4 _IO_write_base: ptr<i8>;
// DEFAULT-NEXT:         field5 _IO_write_ptr: ptr<i8>;
// DEFAULT-NEXT:         field6 _IO_write_end: ptr<i8>;
// DEFAULT-NEXT:         field7 _IO_buf_base: ptr<i8>;
// DEFAULT-NEXT:         field8 _IO_buf_end: ptr<i8>;
// DEFAULT-NEXT:         field9 _IO_save_base: ptr<i8>;
// DEFAULT-NEXT:         field10 _IO_backup_base: ptr<i8>;
// DEFAULT-NEXT:         field11 _IO_save_end: ptr<i8>;
// DEFAULT-NEXT:         field12 _markers: ptr<@type19>;
// DEFAULT-NEXT:         field13 _chain: ptr<@type16>;
// DEFAULT-NEXT:         field14 _fileno: i32;
// DEFAULT-NEXT:         field15 _flags2: i32 : 24;
// DEFAULT-NEXT:         field16 _short_backupbuf: array<i8, 1>;
// DEFAULT-NEXT:         field17 _old_offset: i64;
// DEFAULT-NEXT:         field18 _cur_column: u16;
// DEFAULT-NEXT:         field19 _vtable_offset: i8;
// DEFAULT-NEXT:         field20 _shortbuf: array<i8, 1>;
// DEFAULT-NEXT:         field21 _lock: ptr<void>;
// DEFAULT-NEXT:         field22 _offset: i64;
// DEFAULT-NEXT:         field23 _codecvt: ptr<@type20>;
// DEFAULT-NEXT:         field24 _wide_data: ptr<@type21>;
// DEFAULT-NEXT:         field25 _freeres_list: ptr<@type16>;
// DEFAULT-NEXT:         field26 _freeres_buf: ptr<void>;
// DEFAULT-NEXT:         field27 _prevchain: ptr<ptr<@type16>>;
// DEFAULT-NEXT:         field28 _mode: i32;
// DEFAULT-NEXT:         field29 _unused3: i32;
// DEFAULT-NEXT:         field30 _total_written: u64;
// DEFAULT-NEXT:         field31 _unused2: array<i8, 8>;
// DEFAULT-NEXT:     } [size=216, align=8, offsets=[0, 8, 16, 24, 32, 40, 48, 56, 64, 72, 80, 88, 96, 104, 112, 116, 119, 120, 128, 130, 131, 136, 144, 152, 160, 168, 176, 184, 192, 196, 200, 208], bit_offsets=[None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, Some(928), None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None], bit_units=[(116, 3)], field_units=[None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, Some(0), None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None]];
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
// DEFAULT-NEXT:     global %141 c11_aligned_buffer: array<u8, 64> [storage=static] [align=64] [linkage=internal];
// DEFAULT-NEXT:     global %142 c11_atomic_total: atomic i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %143 c11_thread_local_value: i32 [storage=thread] [linkage=internal];
// DEFAULT-NEXT:     global %144 c11_evaluation_total: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %145 c11_never_flag: volatile i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %146 c11_once_total: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %257 .str257: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([119, 120, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %162 utf8_text: array<i8, 3> [storage=static] [const] = code_units<array<i8, 3>>([206, 169, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %163 utf16_text: array<u16, 2> [storage=static] [const] = code_units<array<u16, 2>>([937, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %164 utf32_text: array<u32, 2> [storage=static] [const] = code_units<array<u32, 2>>([128578, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %259 .str259: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([65, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %260 .str260: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([66, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %261 .str261: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 108, 97, 116, 101, 45, 99, 49, 49, 45, 101, 120, 99, 108, 117, 115, 105, 118, 101, 46, 116, 109, 112, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %262 .str262: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 108, 97, 116, 101, 45, 99, 49, 49, 45, 101, 120, 99, 108, 117, 115, 105, 118, 101, 46, 116, 109, 112, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %263 .str263: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 108, 97, 116, 101, 45, 99, 49, 49, 45, 101, 120, 99, 108, 117, 115, 105, 118, 101, 46, 116, 109, 112, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %264 .str264: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 108, 97, 116, 101, 45, 99, 49, 49, 45, 101, 120, 99, 108, 117, 115, 105, 118, 101, 46, 116, 109, 112, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %265 .str265: array<i8, 54> [storage=static] = code_units<array<i8, 54>>([37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @cimag(%206 __z: complex<f64>) -> f64 [linkage=external] [memory=none] [abi=sysv64(native_c) -> scalar];
// DEFAULT-NEXT:     fn %3 @creal(%207 __z: complex<f64>) -> f64 [linkage=external] [memory=none] [abi=sysv64(native_c) -> scalar];
// DEFAULT-NEXT:     fn %4 @__errno_location() -> ptr<i32> [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %34 @remove(%208 __filename: ptr<const i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %36 @fclose(%209 __stream: ptr<@type16>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %39 @fopen(%210 __filename: ptr<const i8> [restrict], %211 __modes: ptr<const i8> [restrict]) -> ptr<@type16> [linkage=external];
// DEFAULT-NEXT:     fn %41 @printf(%212 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %43 @putchar(%213 __c: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %50 @free(%214 __ptr: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %53 @aligned_alloc(%215 __alignment: u64, %216 __size: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %55 @at_quick_exit(%217 __func: ptr<fn() -> void>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %57 @quick_exit(%218 __status: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %61 @call_once(%219 __flag: ptr<@type25>, %220 __func: ptr<fn() -> void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %64 @timespec_get(%221 __ts: ptr<@type22>, %222 __base: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %86 @thrd_create(%223 __thr: ptr<u64>, %224 __func: ptr<fn(ptr<void>) -> i32>, %225 __arg: ptr<void>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %89 @thrd_join(%226 __thr: u64, %227 __res: ptr<i32>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %92 @mtx_init(%228 __mutex: ptr<@type34>, %229 __type: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %94 @mtx_lock(%230 __mutex: ptr<@type34>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %96 @mtx_unlock(%231 __mutex: ptr<@type34>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %98 @mtx_destroy(%232 __mutex: ptr<@type34>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %102 @cnd_init(%235 __cond: ptr<@type36>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %104 @cnd_destroy(%236 __COND: ptr<@type36>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %107 @tss_create(%237 __tss_id: ptr<u32>, %238 __destructor: ptr<fn(ptr<void>) -> void>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %109 @tss_get(%239 __tss_id: u32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %112 @tss_set(%240 __tss_id: u32, %241 __val: ptr<void>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %114 @tss_delete(%242 __tss_id: u32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %122 @mbrtoc16(%243 __pc16: ptr<u16> [restrict], %244 __s: ptr<const i8> [restrict], %245 __n: u64, %246 __p: ptr<@type13> [restrict]) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %126 @c16rtomb(%247 __s: ptr<i8> [restrict], %248 __c16: u16, %249 __ps: ptr<@type13> [restrict]) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %131 @mbrtoc32(%250 __pc32: ptr<u32> [restrict], %251 __s: ptr<const i8> [restrict], %252 __n: u64, %253 __p: ptr<@type13> [restrict]) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %135 @c32rtomb(%254 __s: ptr<i8> [restrict], %255 __c32: u32, %256 __ps: ptr<@type13> [restrict]) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %147 @c11_make_temporary(%148 base: i32) -> @type45 [linkage=internal] [abi=sysv64(scalar) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %149 result: @type45 [storage=automatic] = aggregate<@type45, zero_fill=false>(field0 = aggregate<array<i32, 3>, zero_fill=false>(index0 = read<i32>(%148), index1 = add<i32, overflow=ub>(read<i32>(%148), const<i32>(1)), index2 = add<i32, overflow=ub>(read<i32>(%148), const<i32>(2))));
// DEFAULT-NEXT:         return copy<@type45, reason=return>(read<@type45>(%149));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %150 @c11_evaluation_step(%151 value: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %266: i32 [synthetic] = read<i32>(%144);
// DEFAULT-NEXT:         let %267: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%266), read<i32>(%151));
// DEFAULT-NEXT:         write<i32>(%144, read<i32>(%267));
// DEFAULT-NEXT:         return mul<i32, overflow=ub>(read<i32>(%151), const<i32>(2));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %152 @c11_thread_worker(%153 argument: ptr<void>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %154 increment: i32 [storage=automatic] = read<i32>(deref(pointer_cast<ptr<i32>, reason=explicit>(read<ptr<void>>(%153))));
// DEFAULT-NEXT:         write<i32>(%143, const<i32>(29));
// DEFAULT-NEXT:         write<i32>(deref(call<ptr<i32>, signature=fn() -> ptr<i32>>(%4)), const<i32>(34));
// DEFAULT-NEXT:         let %268: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i32>>(%142)), add<i32, overflow=wrap>(old<i32>, read<i32>(%154)));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%143), from_bool<i32, reason=promotion>(eq<i32>(read<i32>(deref(call<ptr<i32>, signature=fn() -> ptr<i32>>(%4))), const<i32>(34))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %155 @c11_quick_handler() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %269: i32 [synthetic] = update<i32, result=old, atomic=relaxed>(deref(addr_of<ptr<atomic i32>>(%142)), add<i32, overflow=wrap>(old<i32>, const<i32>(100)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %156 @c11_once_handler() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %270: i32 [synthetic] = read<i32>(%146);
// DEFAULT-NEXT:         let %271: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%270), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%146, read<i32>(%271));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %157 @c11_never_return(%158 status: i32) -> void [linkage=internal] [noreturn] [fallthrough=ub] {
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%57, read<i32>(%158));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %159 @c11_open_exclusive(%160 path: ptr<const i8>) -> ptr<@type16> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<ptr<@type16>, signature=fn(ptr<const i8>, ptr<const i8>) -> ptr<@type16>>(%39, read<ptr<const i8>>(%160), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%257)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %161 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %165 utf16_character: u16 [storage=automatic] = const<u16>(937);
// DEFAULT-NEXT:         let %166 utf32_character: u32 [storage=automatic] = const<u32>(128578);
// DEFAULT-NEXT:         let %167 anonymous: @type41 [storage=automatic] = aggregate<@type41, zero_fill=true>(field0 = aggregate<@type42, zero_fill=false>(field0 = const<i32>(0)));
// DEFAULT-NEXT:         let %168 aligned_object: @type44 [storage=automatic] = aggregate<@type44, zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         let %169 utf16_state: @type13 [storage=automatic] = aggregate<@type13, zero_fill=true>(field0 = const<i32>(0));
// DEFAULT-NEXT:         let %170 utf32_state: @type13 [storage=automatic] = aggregate<@type13, zero_fill=true>(field0 = const<i32>(0));
// DEFAULT-NEXT:         let %171 converted16: u16 [storage=automatic] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %172 converted32: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:         let %173 multibyte16: array<i8, 16> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %174 multibyte32: array<i8, 16> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %175 current_time: @type22 [storage=automatic] = aggregate<@type22, zero_fill=false>(field0 = widen<i64, reason=assign>(const<i32>(0)), field1 = widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         let %176 thread: u64 [storage=automatic];
// DEFAULT-NEXT:         let %177 mutex: @type34 [storage=automatic];
// DEFAULT-NEXT:         let %178 condition: @type36 [storage=automatic];
// DEFAULT-NEXT:         let %179 once_control: @type25 [storage=automatic] = aggregate<@type25, zero_fill=false>(field0 = const<i32>(0));
// DEFAULT-NEXT:         let %180 thread_key: u32 [storage=automatic];
// DEFAULT-NEXT:         let %181 thread_increment: i32 [storage=automatic] = const<i32>(7);
// DEFAULT-NEXT:         let %182 thread_result: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %183 thread_created: i32 [storage=automatic];
// DEFAULT-NEXT:         let %184 thread_joined: i32 [storage=automatic];
// DEFAULT-NEXT:         let %185 aligned_memory: ptr<void> [storage=automatic];
// DEFAULT-NEXT:         let %186 exclusive_first: ptr<@type16> [storage=automatic];
// DEFAULT-NEXT:         let %187 exclusive_second: ptr<@type16> [storage=automatic];
// DEFAULT-NEXT:         let %188 complex_value: complex<f64> [storage=automatic];
// DEFAULT-NEXT:         let %189 alignment_total: i32 [storage=automatic];
// DEFAULT-NEXT:         let %190 unicode_total: i32 [storage=automatic];
// DEFAULT-NEXT:         let %191 generic_total: i32 [storage=automatic];
// DEFAULT-NEXT:         let %192 anonymous_total: i32 [storage=automatic];
// DEFAULT-NEXT:         let %193 evaluation_total: i32 [storage=automatic];
// DEFAULT-NEXT:         let %194 temporary_total: i32 [storage=automatic];
// DEFAULT-NEXT:         let %195 static_assert_total: i32 [storage=automatic];
// DEFAULT-NEXT:         let %196 optional_total: i32 [storage=automatic];
// DEFAULT-NEXT:         let %197 atomic_total: i32 [storage=automatic];
// DEFAULT-NEXT:         let %198 thread_total: i32 [storage=automatic];
// DEFAULT-NEXT:         let %199 concurrency_total: i32 [storage=automatic];
// DEFAULT-NEXT:         let %200 conversion_total: i32 [storage=automatic];
// DEFAULT-NEXT:         let %201 quick_total: i32 [storage=automatic];
// DEFAULT-NEXT:         let %202 exclusive_total: i32 [storage=automatic];
// DEFAULT-NEXT:         let %203 timespec_total: i32 [storage=automatic];
// DEFAULT-NEXT:         let %204 complex_total: i32 [storage=automatic];
// DEFAULT-NEXT:         let %205 limits_total: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%189, add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=always>(const<u64>(4))), reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=always>(const<u64>(32)))), from_bool<i32, reason=promotion>(eq<u64>(rem<u64, by_zero=ub>(ptr_to_int<u64, reason=explicit>(array_decay<ptr<u8>, length=Some(64)>(%141)), widen<u64, reason=usual_arith>(const<u32>(64))), widen<u64, reason=usual_arith>(const<u32>(0))))), from_bool<i32, reason=promotion>(eq<u64>(rem<u64, by_zero=ub>(ptr_to_int<u64, reason=explicit>(addr_of<ptr<@type44>>(%168)), widen<u64, reason=usual_arith>(const<u32>(32))), widen<u64, reason=usual_arith>(const<u32>(0))))));
// DEFAULT-NEXT:         write<i32>(%190, add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(%162), const<i32>(0))))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(%162), const<i32>(1)))))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(deref(ptr_offset<ptr<const u16>, subtract=false, element=u16, overflow=ub>(array_decay<ptr<const u16>, length=Some(2)>(%163), const<i32>(0))))))), reinterpret<i32, reason=explicit, fits=unknown>(read<u32>(deref(ptr_offset<ptr<const u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<const u32>, length=Some(2)>(%164), const<i32>(0)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%165)))), reinterpret<i32, reason=explicit, fits=unknown>(read<u32>(%166))));
// DEFAULT-NEXT:         write<i32>(%191, add<i32, overflow=ub>(add<i32, overflow=ub>(const<i32>(11), const<i32>(22)), const<i32>(33)));
// DEFAULT-NEXT:         write<i32>(field0(field0(%167)), const<i32>(31));
// DEFAULT-NEXT:         write<i32>(field0(field1(%167)), const<i32>(37));
// DEFAULT-NEXT:         write<i32>(field1(field1(%167)), const<i32>(41));
// DEFAULT-NEXT:         write<i32>(%192, add<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(field0(field0(%167))), read<i32>(field0(field1(%167)))), read<i32>(field1(field1(%167)))));
// DEFAULT-NEXT:         write<i32>(%144, const<i32>(0));
// DEFAULT-NEXT:         write<i32>(%193, add<i32, overflow=ub>(add<i32, overflow=ub>(call<i32, signature=fn(i32) -> i32>(%150, const<i32>(2)), call<i32, signature=fn(i32) -> i32>(%150, const<i32>(3))), read<i32>(%144)));
// DEFAULT-NEXT:         add<i32, overflow=ub>(add<i32, overflow=ub>(call<i32, signature=fn(i32) -> i32>(%150, const<i32>(2)), call<i32, signature=fn(i32) -> i32>(%150, const<i32>(3))), read<i32>(%144));
// DEFAULT-NEXT:         write<i32>(%194, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(field0(temporary %258 = call<@type45, signature=fn(i32) -> @type45, abi=sysv64(scalar) -> native_c>(%147, const<i32>(43)))), const<i32>(1)))));
// DEFAULT-NEXT:         read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(field0(temporary %258 = call<@type45, signature=fn(i32) -> @type45, abi=sysv64(scalar) -> native_c>(%147, const<i32>(43)))), const<i32>(1))));
// DEFAULT-NEXT:         write<i32>(%195, const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%196, add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(const<i32>(0), const<i32>(0)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)));
// DEFAULT-NEXT:         write<i32>(deref(addr_of<ptr<atomic i32>>(%142)), const<i32>(5));
// DEFAULT-NEXT:         write<i32>(%143, const<i32>(17));
// DEFAULT-NEXT:         write<i32>(deref(call<ptr<i32>, signature=fn() -> ptr<i32>>(%4)), const<i32>(0));
// DEFAULT-NEXT:         write<i32>(%183, call<i32, signature=fn(ptr<u64>, ptr<fn(ptr<void>) -> i32>, ptr<void>) -> i32>(%86, addr_of<ptr<u64>>(%176), function_decay<ptr<fn(ptr<void>) -> i32>>(%152), pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i32>>(%181))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<u64>, ptr<fn(ptr<void>) -> i32>, ptr<void>) -> i32>(%86, addr_of<ptr<u64>>(%176), function_decay<ptr<fn(ptr<void>) -> i32>>(%152), pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i32>>(%181)));
// DEFAULT-NEXT:         let %272: i32 [synthetic];
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%183), const<i32>(0))
// DEFAULT-NEXT:             write<i32>(%272, call<i32, signature=fn(u64, ptr<i32>) -> i32>(%89, read<u64>(%176), addr_of<ptr<i32>>(%182)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<i32>(%272, neg<i32, overflow=ub>(const<i32>(1)));
// DEFAULT-NEXT:         write<i32>(%184, read<i32>(%272));
// DEFAULT-NEXT:         write<i32>(%197, read<i32, atomic=seq_cst>(deref(addr_of<ptr<atomic i32>>(%142))));
// DEFAULT-NEXT:         write<i32>(%198, add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(eq<i32>(read<i32>(%183), const<i32>(0))), from_bool<i32, reason=promotion>(eq<i32>(read<i32>(%184), const<i32>(0)))), read<i32>(%182)), read<i32>(%143)), from_bool<i32, reason=promotion>(eq<i32>(read<i32>(deref(call<ptr<i32>, signature=fn() -> ptr<i32>>(%4))), const<i32>(0)))));
// DEFAULT-NEXT:         write<i32>(%199, const<i32>(0));
// DEFAULT-NEXT:         if eq<i32>(call<i32, signature=fn(ptr<@type34>, i32) -> i32>(%92, addr_of<ptr<@type34>>(%177), const<i32>(0)), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %273: i32 [synthetic] = read<i32>(%199);
// DEFAULT-NEXT:                 let %274: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%273), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%199, read<i32>(%274));
// DEFAULT-NEXT:                 let %275: i32 [synthetic] = read<i32>(%199);
// DEFAULT-NEXT:                 let %276: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%275), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<@type34>) -> i32>(%94, addr_of<ptr<@type34>>(%177)), const<i32>(0))));
// DEFAULT-NEXT:                 write<i32>(%199, read<i32>(%276));
// DEFAULT-NEXT:                 let %277: i32 [synthetic] = read<i32>(%199);
// DEFAULT-NEXT:                 let %278: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%277), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<@type34>) -> i32>(%96, addr_of<ptr<@type34>>(%177)), const<i32>(0))));
// DEFAULT-NEXT:                 write<i32>(%199, read<i32>(%278));
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<@type34>) -> void>(%98, addr_of<ptr<@type34>>(%177));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         if eq<i32>(call<i32, signature=fn(ptr<@type36>) -> i32>(%102, addr_of<ptr<@type36>>(%178)), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %279: i32 [synthetic] = read<i32>(%199);
// DEFAULT-NEXT:                 let %280: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%279), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%199, read<i32>(%280));
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<@type36>) -> void>(%104, addr_of<ptr<@type36>>(%178));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         write<i32>(%146, const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type25>, ptr<fn() -> void>) -> void>(%61, addr_of<ptr<@type25>>(%179), function_decay<ptr<fn() -> void>>(%156));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type25>, ptr<fn() -> void>) -> void>(%61, addr_of<ptr<@type25>>(%179), function_decay<ptr<fn() -> void>>(%156));
// DEFAULT-NEXT:         let %281: i32 [synthetic] = read<i32>(%199);
// DEFAULT-NEXT:         let %282: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%281), read<i32>(%146));
// DEFAULT-NEXT:         write<i32>(%199, read<i32>(%282));
// DEFAULT-NEXT:         if eq<i32>(call<i32, signature=fn(ptr<u32>, ptr<fn(ptr<void>) -> void>) -> i32>(%107, addr_of<ptr<u32>>(%180), null<ptr<fn(ptr<void>) -> void>>), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %283: i32 [synthetic] = read<i32>(%199);
// DEFAULT-NEXT:                 let %284: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%283), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%199, read<i32>(%284));
// DEFAULT-NEXT:                 let %285: i32 [synthetic] = read<i32>(%199);
// DEFAULT-NEXT:                 let %286: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%285), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(u32, ptr<void>) -> i32>(%112, read<u32>(%180), pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i32>>(%181))), const<i32>(0))));
// DEFAULT-NEXT:                 write<i32>(%199, read<i32>(%286));
// DEFAULT-NEXT:                 let %287: i32 [synthetic] = read<i32>(%199);
// DEFAULT-NEXT:                 let %288: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%287), from_bool<i32, reason=promotion>(eq<ptr<void>>(call<ptr<void>, signature=fn(u32) -> ptr<void>>(%109, read<u32>(%180)), pointer_cast<ptr<void>, reason=usual_arith>(addr_of<ptr<i32>>(%181)))));
// DEFAULT-NEXT:                 write<i32>(%199, read<i32>(%288));
// DEFAULT-NEXT:                 call<void, signature=fn(u32) -> void>(%114, read<u32>(%180));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         let %289: i32 [synthetic] = read<i32>(%199);
// DEFAULT-NEXT:         let %290: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%289), from_bool<i32, reason=promotion>(ge<i32>(const<i32>(4), const<i32>(1))));
// DEFAULT-NEXT:         write<i32>(%199, read<i32>(%290));
// DEFAULT-NEXT:         write<u16>(%171, reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         write<u32>(%172, reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<i32>(%200, add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(call<u64, signature=fn(ptr<u16>, ptr<const i8>, u64, ptr<@type13>) -> u64>(%122, addr_of<ptr<u16>>(%171), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%259)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), addr_of<ptr<@type13>>(%169)))), reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(call<u64, signature=fn(ptr<i8>, u16, ptr<@type13>) -> u64>(%126, array_decay<ptr<i8>, length=Some(16)>(%173), const<u16>(65), addr_of<ptr<@type13>>(%169))))), reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(call<u64, signature=fn(ptr<u32>, ptr<const i8>, u64, ptr<@type13>) -> u64>(%131, addr_of<ptr<u32>>(%172), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%260)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), addr_of<ptr<@type13>>(%170))))), reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(call<u64, signature=fn(ptr<i8>, u32, ptr<@type13>) -> u64>(%135, array_decay<ptr<i8>, length=Some(16)>(%174), const<u32>(66), addr_of<ptr<@type13>>(%170))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%171)))), reinterpret<i32, reason=explicit, fits=unknown>(read<u32>(%172))), widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(16)>(%173), const<i32>(0)))))), widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(16)>(%174), const<i32>(0)))))));
// DEFAULT-NEXT:         add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(call<u64, signature=fn(ptr<u16>, ptr<const i8>, u64, ptr<@type13>) -> u64>(%122, addr_of<ptr<u16>>(%171), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%259)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), addr_of<ptr<@type13>>(%169)))), reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(call<u64, signature=fn(ptr<i8>, u16, ptr<@type13>) -> u64>(%126, array_decay<ptr<i8>, length=Some(16)>(%173), const<u16>(65), addr_of<ptr<@type13>>(%169))))), reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(call<u64, signature=fn(ptr<u32>, ptr<const i8>, u64, ptr<@type13>) -> u64>(%131, addr_of<ptr<u32>>(%172), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%260)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), addr_of<ptr<@type13>>(%170))))), reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(call<u64, signature=fn(ptr<i8>, u32, ptr<@type13>) -> u64>(%135, array_decay<ptr<i8>, length=Some(16)>(%174), const<u32>(66), addr_of<ptr<@type13>>(%170))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%171)))), reinterpret<i32, reason=explicit, fits=unknown>(read<u32>(%172))), widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(16)>(%173), const<i32>(0)))))), widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(16)>(%174), const<i32>(0))))));
// DEFAULT-NEXT:         write<ptr<void>>(%185, call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%53, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(64))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(64)))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%53, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(64))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(64))));
// DEFAULT-NEXT:         let %291: i32 [synthetic] = read<i32>(%189);
// DEFAULT-NEXT:         let %292: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%291), from_bool<i32, reason=promotion>(logical_and<bool>(ne<ptr<void>>(read<ptr<void>>(%185), null<ptr<void>>), eq<u64>(rem<u64, by_zero=ub>(ptr_to_int<u64, reason=explicit>(read<ptr<void>>(%185)), widen<u64, reason=usual_arith>(const<u32>(64))), widen<u64, reason=usual_arith>(const<u32>(0))))));
// DEFAULT-NEXT:         write<i32>(%189, read<i32>(%292));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%50, read<ptr<void>>(%185));
// DEFAULT-NEXT:         write<i32>(%201, from_bool<i32, reason=assign>(eq<i32>(call<i32, signature=fn(ptr<fn() -> void>) -> i32>(%55, function_decay<ptr<fn() -> void>>(%155)), const<i32>(0))));
// DEFAULT-NEXT:         from_bool<i32, reason=assign>(eq<i32>(call<i32, signature=fn(ptr<fn() -> void>) -> i32>(%55, function_decay<ptr<fn() -> void>>(%155)), const<i32>(0)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>) -> i32>(%34, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(24)>(%261)));
// DEFAULT-NEXT:         write<ptr<@type16>>(%186, call<ptr<@type16>, signature=fn(ptr<const i8>) -> ptr<@type16>>(%159, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(24)>(%262))));
// DEFAULT-NEXT:         call<ptr<@type16>, signature=fn(ptr<const i8>) -> ptr<@type16>>(%159, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(24)>(%262)));
// DEFAULT-NEXT:         write<ptr<@type16>>(%187, call<ptr<@type16>, signature=fn(ptr<const i8>) -> ptr<@type16>>(%159, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(24)>(%263))));
// DEFAULT-NEXT:         call<ptr<@type16>, signature=fn(ptr<const i8>) -> ptr<@type16>>(%159, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(24)>(%263)));
// DEFAULT-NEXT:         write<i32>(%202, from_bool<i32, reason=assign>(logical_and<bool>(ne<ptr<@type16>>(read<ptr<@type16>>(%186), null<ptr<@type16>>), eq<ptr<@type16>>(read<ptr<@type16>>(%187), null<ptr<@type16>>))));
// DEFAULT-NEXT:         if ne<ptr<@type16>>(read<ptr<@type16>>(%186), null<ptr<@type16>>)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<@type16>) -> i32>(%36, read<ptr<@type16>>(%186));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         if ne<ptr<@type16>>(read<ptr<@type16>>(%187), null<ptr<@type16>>)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<@type16>) -> i32>(%36, read<ptr<@type16>>(%187));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>) -> i32>(%34, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(24)>(%264)));
// DEFAULT-NEXT:         write<i32>(%203, from_bool<i32, reason=assign>(logical_and<bool>(logical_and<bool>(eq<i32>(call<i32, signature=fn(ptr<@type22>, i32) -> i32>(%64, addr_of<ptr<@type22>>(%175), const<i32>(1)), const<i32>(1)), ge<i64>(read<i64>(field1(%175)), widen<i64, reason=usual_arith>(const<i32>(0)))), lt<i64>(read<i64>(field1(%175)), const<i64>(1000000000)))));
// DEFAULT-NEXT:         from_bool<i32, reason=assign>(logical_and<bool>(logical_and<bool>(eq<i32>(call<i32, signature=fn(ptr<@type22>, i32) -> i32>(%64, addr_of<ptr<@type22>>(%175), const<i32>(1)), const<i32>(1)), ge<i64>(read<i64>(field1(%175)), widen<i64, reason=usual_arith>(const<i32>(0)))), lt<i64>(read<i64>(field1(%175)), const<i64>(1000000000))));
// DEFAULT-NEXT:         write<complex<f64>>(%188, aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.0), index1 = const<f64>(3.0)));
// DEFAULT-NEXT:         write<i32>(%204, add<i32, overflow=ub>(from_bool<i32, reason=promotion>(eq<f64, exceptions=ignore>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(native_c) -> scalar>(%3, read<complex<f64>>(%188)), const<f64>(2.0))), from_bool<i32, reason=promotion>(eq<f64, exceptions=ignore>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(native_c) -> scalar>(%1, read<complex<f64>>(%188)), const<f64>(3.0)))));
// DEFAULT-NEXT:         write<i32>(%205, add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ge<i32>(const<i32>(9), const<i32>(6))), from_bool<i32, reason=promotion>(ge<i32>(const<i32>(17), const<i32>(10)))), from_bool<i32, reason=promotion>(ge<i32>(const<i32>(21), const<i32>(10)))), from_bool<i32, reason=promotion>(gt<f32, exceptions=ignore>(const<f32>(1e-45), const<f32>(0.0)))), from_bool<i32, reason=promotion>(gt<f64, exceptions=ignore>(const<f64>(5e-324), const<f64>(0.0)))), from_bool<i32, reason=promotion>(gt<f80, exceptions=ignore>(const<f80>(3.64519953188247460253E-4951), const<f80>(0)))), from_bool<i32, reason=promotion>(ge<i32>(const<i32>(1), neg<i32, overflow=ub>(const<i32>(1))))), from_bool<i32, reason=promotion>(ge<i32>(const<i32>(1), neg<i32, overflow=ub>(const<i32>(1))))), from_bool<i32, reason=promotion>(ge<i32>(const<i32>(1), neg<i32, overflow=ub>(const<i32>(1))))));
// DEFAULT-NEXT:         let %293: i32 [synthetic] = read<i32>(%205);
// DEFAULT-NEXT:         let %294: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%293), const<i32>(0));
// DEFAULT-NEXT:         write<i32>(%205, read<i32>(%294));
// DEFAULT-NEXT:         if ne<i32>(read<i32, volatile>(%145), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<void, signature=fn(i32) -> void>(%157, const<i32>(99));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%41, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(54)>(%265)), read<i32>(%189), read<i32>(%190), read<i32>(%191), read<i32>(%192), read<i32>(%193), read<i32>(%194), read<i32>(%195), read<i32>(%196), read<i32>(%197), read<i32>(%198), read<i32>(%199), read<i32>(%200), read<i32>(%201), read<i32>(%202), read<i32>(%203), read<i32>(%204), read<i32>(%205), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(i32) -> i32>(%43, const<i32>(10));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
