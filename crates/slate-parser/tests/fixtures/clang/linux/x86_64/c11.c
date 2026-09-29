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
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE___uint16_t:[0-9]+]] __uint16_t = u16;
// DEFAULT-NEXT:     type @type[[TYPE___uint32_t:[0-9]+]] __uint32_t = u32;
// DEFAULT-NEXT:     type @type[[TYPE___uint64_t:[0-9]+]] __uint64_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE___uint_least16_t:[0-9]+]] __uint_least16_t = u16;
// DEFAULT-NEXT:     type @type[[TYPE___uint_least32_t:[0-9]+]] __uint_least32_t = u32;
// DEFAULT-NEXT:     type @type[[TYPE___off_t:[0-9]+]] __off_t = i64;
// DEFAULT-NEXT:     type @type[[TYPE___off64_t:[0-9]+]] __off64_t = i64;
// DEFAULT-NEXT:     type @type[[TYPE___time_t:[0-9]+]] __time_t = i64;
// DEFAULT-NEXT:     type @type[[TYPE___syscall_slong_t:[0-9]+]] __syscall_slong_t = i64;
// DEFAULT-NEXT:     type @type[[TYPE_uintptr_t:[0-9]+]] uintptr_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE_memory_order:[0-9]+]] memory_order = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_memory_order_relaxed:[0-9]+]] memory_order_relaxed = const<i32>(0);
// DEFAULT-NEXT:         %[[VALUE_memory_order_consume:[0-9]+]] memory_order_consume = const<i32>(1);
// DEFAULT-NEXT:         %[[VALUE_memory_order_acquire:[0-9]+]] memory_order_acquire = const<i32>(2);
// DEFAULT-NEXT:         %[[VALUE_memory_order_release:[0-9]+]] memory_order_release = const<i32>(3);
// DEFAULT-NEXT:         %[[VALUE_memory_order_acq_rel:[0-9]+]] memory_order_acq_rel = const<i32>(4);
// DEFAULT-NEXT:         %[[VALUE_memory_order_seq_cst:[0-9]+]] memory_order_seq_cst = const<i32>(5);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_memory_order_2:[0-9]+]] memory_order = @type[[TYPE_memory_order]];
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 __count: i32;
// DEFAULT-NEXT:         field1 __value: @type[[TYPE1:[0-9]+]];
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE1]] = union {
// DEFAULT-NEXT:         field0 __wch: u32;
// DEFAULT-NEXT:         field1 __wchb: array<i8, 4>;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE___mbstate_t:[0-9]+]] __mbstate_t = @type[[TYPE0]];
// DEFAULT-NEXT:     type @type[[TYPE__IO_FILE:[0-9]+]] _IO_FILE = struct {
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
// DEFAULT-NEXT:         field12 _markers: ptr<@type[[TYPE__IO_marker:[0-9]+]]>;
// DEFAULT-NEXT:         field13 _chain: ptr<@type[[TYPE__IO_FILE]]>;
// DEFAULT-NEXT:         field14 _fileno: i32;
// DEFAULT-NEXT:         field15 _flags2: i32 : 24;
// DEFAULT-NEXT:         field16 _short_backupbuf: array<i8, 1>;
// DEFAULT-NEXT:         field17 _old_offset: i64;
// DEFAULT-NEXT:         field18 _cur_column: u16;
// DEFAULT-NEXT:         field19 _vtable_offset: i8;
// DEFAULT-NEXT:         field20 _shortbuf: array<i8, 1>;
// DEFAULT-NEXT:         field21 _lock: ptr<void>;
// DEFAULT-NEXT:         field22 _offset: i64;
// DEFAULT-NEXT:         field23 _codecvt: ptr<@type[[TYPE__IO_codecvt:[0-9]+]]>;
// DEFAULT-NEXT:         field24 _wide_data: ptr<@type[[TYPE__IO_wide_data:[0-9]+]]>;
// DEFAULT-NEXT:         field25 _freeres_list: ptr<@type[[TYPE__IO_FILE]]>;
// DEFAULT-NEXT:         field26 _freeres_buf: ptr<void>;
// DEFAULT-NEXT:         field27 _prevchain: ptr<ptr<@type[[TYPE__IO_FILE]]>>;
// DEFAULT-NEXT:         field28 _mode: i32;
// DEFAULT-NEXT:         field29 _unused3: i32;
// DEFAULT-NEXT:         field30 _total_written: u64;
// DEFAULT-NEXT:         field31 _unused2: array<i8, 8>;
// DEFAULT-NEXT:     } [size=216, align=8, offsets=[0, 8, 16, 24, 32, 40, 48, 56, 64, 72, 80, 88, 96, 104, 112, 116, 119, 120, 128, 130, 131, 136, 144, 152, 160, 168, 176, 184, 192, 196, 200, 208], bit_offsets=[None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, Some(928), None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None], bit_units=[(116, 3)], field_units=[None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, Some(0), None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None]];
// DEFAULT-NEXT:     type @type[[TYPE_FILE:[0-9]+]] FILE = @type[[TYPE__IO_FILE]];
// DEFAULT-NEXT:     type @type[[TYPE__IO_marker]] _IO_marker = struct incomplete;
// DEFAULT-NEXT:     type @type[[TYPE__IO_codecvt]] _IO_codecvt = struct incomplete;
// DEFAULT-NEXT:     type @type[[TYPE__IO_wide_data]] _IO_wide_data = struct incomplete;
// DEFAULT-NEXT:     type @type[[TYPE__IO_lock_t:[0-9]+]] _IO_lock_t = void;
// DEFAULT-NEXT:     type @type[[TYPE_timespec:[0-9]+]] timespec = struct {
// DEFAULT-NEXT:         field0 tv_sec: i64;
// DEFAULT-NEXT:         field1 tv_nsec: i64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE___tss_t:[0-9]+]] __tss_t = u32;
// DEFAULT-NEXT:     type @type[[TYPE___thrd_t:[0-9]+]] __thrd_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE2:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 __data: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE___once_flag:[0-9]+]] __once_flag = @type[[TYPE2]];
// DEFAULT-NEXT:     type @type[[TYPE_once_flag:[0-9]+]] once_flag = @type[[TYPE2]];
// DEFAULT-NEXT:     type @type[[TYPE_tss_t:[0-9]+]] tss_t = u32;
// DEFAULT-NEXT:     type @type[[TYPE_tss_dtor_t:[0-9]+]] tss_dtor_t = ptr<fn(ptr<void>) -> void>;
// DEFAULT-NEXT:     type @type[[TYPE_thrd_t:[0-9]+]] thrd_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE_thrd_start_t:[0-9]+]] thrd_start_t = ptr<fn(ptr<void>) -> i32>;
// DEFAULT-NEXT:     type @type[[TYPE3:[0-9]+]] = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_memory_order_relaxed]] thrd_success = const<i32>(0);
// DEFAULT-NEXT:         %[[VALUE_memory_order_consume]] thrd_busy = const<i32>(1);
// DEFAULT-NEXT:         %[[VALUE_memory_order_acquire]] thrd_error = const<i32>(2);
// DEFAULT-NEXT:         %[[VALUE_memory_order_release]] thrd_nomem = const<i32>(3);
// DEFAULT-NEXT:         %[[VALUE_memory_order_acq_rel]] thrd_timedout = const<i32>(4);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE4:[0-9]+]] = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_memory_order_relaxed]] mtx_plain = const<i32>(0);
// DEFAULT-NEXT:         %[[VALUE_memory_order_consume]] mtx_recursive = const<i32>(1);
// DEFAULT-NEXT:         %[[VALUE_memory_order_acquire]] mtx_timed = const<i32>(2);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE5:[0-9]+]] = union {
// DEFAULT-NEXT:         field0 __size: array<i8, 40>;
// DEFAULT-NEXT:         field1 __align: i64;
// DEFAULT-NEXT:     } [size=40, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE_mtx_t:[0-9]+]] mtx_t = @type[[TYPE5]];
// DEFAULT-NEXT:     type @type[[TYPE6:[0-9]+]] = union {
// DEFAULT-NEXT:         field0 __size: array<i8, 48>;
// DEFAULT-NEXT:         field1 __align: i64;
// DEFAULT-NEXT:     } [size=48, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE_cnd_t:[0-9]+]] cnd_t = @type[[TYPE6]];
// DEFAULT-NEXT:     type @type[[TYPE_mbstate_t:[0-9]+]] mbstate_t = @type[[TYPE0]];
// DEFAULT-NEXT:     type @type[[TYPE_char16_t:[0-9]+]] char16_t = u16;
// DEFAULT-NEXT:     type @type[[TYPE_char32_t:[0-9]+]] char32_t = u32;
// DEFAULT-NEXT:     type @type[[TYPE_C11Anonymous:[0-9]+]] C11Anonymous = struct {
// DEFAULT-NEXT:         field0 <anonymous>: @type[[TYPE7:[0-9]+]];
// DEFAULT-NEXT:         field1 <anonymous>: @type[[TYPE8:[0-9]+]];
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE7]] = union {
// DEFAULT-NEXT:         field0 integer: i32;
// DEFAULT-NEXT:         field1 real: f64;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE8]] = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:         field1 y: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_C11OverAligned:[0-9]+]] C11OverAligned = struct {
// DEFAULT-NEXT:         field0 value: u8;
// DEFAULT-NEXT:     } [size=32, align=32, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_C11Temporary:[0-9]+]] C11Temporary = struct {
// DEFAULT-NEXT:         field0 values: array<i32, 3>;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0]];
// DEFAULT-NEXT:     global %[[VALUE_c11_aligned_buffer:[0-9]+]] c11_aligned_buffer: array<u8, 64> [storage=static] [align=64] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_c11_atomic_total:[0-9]+]] c11_atomic_total: atomic i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_c11_thread_local_value:[0-9]+]] c11_thread_local_value: i32 [storage=thread] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_c11_evaluation_total:[0-9]+]] c11_evaluation_total: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_c11_never_flag:[0-9]+]] c11_never_flag: volatile i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_c11_once_total:[0-9]+]] c11_once_total: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([119, 120, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_utf8_text:[0-9]+]] utf8_text: array<i8, 3> [storage=static] [const] = code_units<array<i8, 3>>([206, 169, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_utf16_text:[0-9]+]] utf16_text: array<u16, 2> [storage=static] [const] = code_units<array<u16, 2>>([937, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_utf32_text:[0-9]+]] utf32_text: array<u32, 2> [storage=static] [const] = code_units<array<u32, 2>>([128578, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([65, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([66, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_4:[0-9]+]] .str[[VALUE_str_4]]: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 108, 97, 116, 101, 45, 99, 49, 49, 45, 101, 120, 99, 108, 117, 115, 105, 118, 101, 46, 116, 109, 112, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_5:[0-9]+]] .str[[VALUE_str_5]]: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 108, 97, 116, 101, 45, 99, 49, 49, 45, 101, 120, 99, 108, 117, 115, 105, 118, 101, 46, 116, 109, 112, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_6:[0-9]+]] .str[[VALUE_str_6]]: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 108, 97, 116, 101, 45, 99, 49, 49, 45, 101, 120, 99, 108, 117, 115, 105, 118, 101, 46, 116, 109, 112, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_7:[0-9]+]] .str[[VALUE_str_7]]: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([115, 108, 97, 116, 101, 45, 99, 49, 49, 45, 101, 120, 99, 108, 117, 115, 105, 118, 101, 46, 116, 109, 112, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_8:[0-9]+]] .str[[VALUE_str_8]]: array<i8, 54> [storage=static] = code_units<array<i8, 54>>([37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_memory_order_consume]] @cimag(%[[VALUE___z:[0-9]+]] __z: complex<f64>) -> f64 [linkage=external] [memory=none] [abi=sysv64(native_c) -> scalar];
// DEFAULT-NEXT:     fn %[[VALUE_memory_order_release]] @creal(%[[VALUE___z_2:[0-9]+]] __z: complex<f64>) -> f64 [linkage=external] [memory=none] [abi=sysv64(native_c) -> scalar];
// DEFAULT-NEXT:     fn %[[VALUE_memory_order_acq_rel]] @__errno_location() -> ptr<i32> [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_remove:[0-9]+]] @remove(%[[VALUE___filename:[0-9]+]] __filename: ptr<const i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fclose:[0-9]+]] @fclose(%[[VALUE___stream:[0-9]+]] __stream: ptr<@type[[TYPE__IO_FILE]]>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fopen:[0-9]+]] @fopen(%[[VALUE___filename_2:[0-9]+]] __filename: ptr<const i8> [restrict], %[[VALUE___modes:[0-9]+]] __modes: ptr<const i8> [restrict]) -> ptr<@type[[TYPE__IO_FILE]]> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_putchar:[0-9]+]] @putchar(%[[VALUE___c:[0-9]+]] __c: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_free:[0-9]+]] @free(%[[VALUE___ptr:[0-9]+]] __ptr: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_aligned_alloc:[0-9]+]] @aligned_alloc(%[[VALUE___alignment:[0-9]+]] __alignment: u64, %[[VALUE___size:[0-9]+]] __size: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_at_quick_exit:[0-9]+]] @at_quick_exit(%[[VALUE___func:[0-9]+]] __func: ptr<fn() -> void>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_quick_exit:[0-9]+]] @quick_exit(%[[VALUE___status:[0-9]+]] __status: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_call_once:[0-9]+]] @call_once(%[[VALUE___flag:[0-9]+]] __flag: ptr<@type[[TYPE2]]>, %[[VALUE___func_2:[0-9]+]] __func: ptr<fn() -> void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_timespec_get:[0-9]+]] @timespec_get(%[[VALUE___ts:[0-9]+]] __ts: ptr<@type[[TYPE_timespec]]>, %[[VALUE___base:[0-9]+]] __base: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_thrd_create:[0-9]+]] @thrd_create(%[[VALUE___thr:[0-9]+]] __thr: ptr<u64>, %[[VALUE___func_3:[0-9]+]] __func: ptr<fn(ptr<void>) -> i32>, %[[VALUE___arg:[0-9]+]] __arg: ptr<void>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_thrd_join:[0-9]+]] @thrd_join(%[[VALUE___thr_2:[0-9]+]] __thr: u64, %[[VALUE___res:[0-9]+]] __res: ptr<i32>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_mtx_init:[0-9]+]] @mtx_init(%[[VALUE___mutex:[0-9]+]] __mutex: ptr<@type[[TYPE5]]>, %[[VALUE___type:[0-9]+]] __type: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_mtx_lock:[0-9]+]] @mtx_lock(%[[VALUE___mutex_2:[0-9]+]] __mutex: ptr<@type[[TYPE5]]>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_mtx_unlock:[0-9]+]] @mtx_unlock(%[[VALUE___mutex_3:[0-9]+]] __mutex: ptr<@type[[TYPE5]]>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_mtx_destroy:[0-9]+]] @mtx_destroy(%[[VALUE___mutex_4:[0-9]+]] __mutex: ptr<@type[[TYPE5]]>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_cnd_init:[0-9]+]] @cnd_init(%[[VALUE___cond:[0-9]+]] __cond: ptr<@type[[TYPE6]]>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_cnd_destroy:[0-9]+]] @cnd_destroy(%[[VALUE___COND:[0-9]+]] __COND: ptr<@type[[TYPE6]]>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_tss_create:[0-9]+]] @tss_create(%[[VALUE___tss_id:[0-9]+]] __tss_id: ptr<u32>, %[[VALUE___destructor:[0-9]+]] __destructor: ptr<fn(ptr<void>) -> void>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_tss_get:[0-9]+]] @tss_get(%[[VALUE___tss_id_2:[0-9]+]] __tss_id: u32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_tss_set:[0-9]+]] @tss_set(%[[VALUE___tss_id_3:[0-9]+]] __tss_id: u32, %[[VALUE___val:[0-9]+]] __val: ptr<void>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_tss_delete:[0-9]+]] @tss_delete(%[[VALUE___tss_id_4:[0-9]+]] __tss_id: u32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_mbrtoc16:[0-9]+]] @mbrtoc16(%[[VALUE___pc16:[0-9]+]] __pc16: ptr<u16> [restrict], %[[VALUE___s:[0-9]+]] __s: ptr<const i8> [restrict], %[[VALUE___n:[0-9]+]] __n: u64, %[[VALUE___p:[0-9]+]] __p: ptr<@type[[TYPE0]]> [restrict]) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_c16rtomb:[0-9]+]] @c16rtomb(%[[VALUE___s_2:[0-9]+]] __s: ptr<i8> [restrict], %[[VALUE___c16:[0-9]+]] __c16: u16, %[[VALUE___ps:[0-9]+]] __ps: ptr<@type[[TYPE0]]> [restrict]) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_mbrtoc32:[0-9]+]] @mbrtoc32(%[[VALUE___pc32:[0-9]+]] __pc32: ptr<u32> [restrict], %[[VALUE___s_3:[0-9]+]] __s: ptr<const i8> [restrict], %[[VALUE___n_2:[0-9]+]] __n: u64, %[[VALUE___p_2:[0-9]+]] __p: ptr<@type[[TYPE0]]> [restrict]) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_c32rtomb:[0-9]+]] @c32rtomb(%[[VALUE___s_4:[0-9]+]] __s: ptr<i8> [restrict], %[[VALUE___c32:[0-9]+]] __c32: u32, %[[VALUE___ps_2:[0-9]+]] __ps: ptr<@type[[TYPE0]]> [restrict]) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_c11_make_temporary:[0-9]+]] @c11_make_temporary(%[[VALUE_base:[0-9]+]] base: i32) -> @type[[TYPE_C11Temporary]] [linkage=internal] [abi=sysv64(scalar) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_result:[0-9]+]] result: @type[[TYPE_C11Temporary]] [storage=automatic] = aggregate<@type[[TYPE_C11Temporary]], zero_fill=false>(field0 = aggregate<array<i32, 3>, zero_fill=false>(index0 = read<i32>(%[[VALUE_base]]), index1 = add<i32, overflow=ub>(read<i32>(%[[VALUE_base]]), const<i32>(1)), index2 = add<i32, overflow=ub>(read<i32>(%[[VALUE_base]]), const<i32>(2))));
// DEFAULT-NEXT:         return copy<@type[[TYPE_C11Temporary]], reason=return>(read<@type[[TYPE_C11Temporary]]>(%[[VALUE_result]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_c11_evaluation_step:[0-9]+]] @c11_evaluation_step(%[[VALUE_value:[0-9]+]] value: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_c11_evaluation_total]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE0]]), read<i32>(%[[VALUE_value]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_c11_evaluation_total]], read<i32>(%[[VALUE1]]));
// DEFAULT-NEXT:         return mul<i32, overflow=ub>(read<i32>(%[[VALUE_value]]), const<i32>(2));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_c11_thread_worker:[0-9]+]] @c11_thread_worker(%[[VALUE_argument:[0-9]+]] argument: ptr<void>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_increment:[0-9]+]] increment: i32 [storage=automatic] = read<i32>(deref(pointer_cast<ptr<i32>, reason=explicit>(read<ptr<void>>(%[[VALUE_argument]]))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_c11_thread_local_value]], const<i32>(29));
// DEFAULT-NEXT:         write<i32>(deref(call<ptr<i32>, signature=fn() -> ptr<i32>>(%[[VALUE_memory_order_acq_rel]])), const<i32>(34));
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i32>>(%[[VALUE_c11_atomic_total]])), add<i32, overflow=wrap>(old<i32>, read<i32>(%[[VALUE_increment]])));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%[[VALUE_c11_thread_local_value]]), from_bool<i32, reason=promotion>(eq<i32>(read<i32>(deref(call<ptr<i32>, signature=fn() -> ptr<i32>>(%[[VALUE_memory_order_acq_rel]]))), const<i32>(34))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_c11_quick_handler:[0-9]+]] @c11_quick_handler() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: i32 [synthetic] = update<i32, result=old, atomic=relaxed>(deref(addr_of<ptr<atomic i32>>(%[[VALUE_c11_atomic_total]])), add<i32, overflow=wrap>(old<i32>, const<i32>(100)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_c11_once_handler:[0-9]+]] @c11_once_handler() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_c11_once_total]]);
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE4]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_c11_once_total]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_c11_never_return:[0-9]+]] @c11_never_return(%[[VALUE_status:[0-9]+]] status: i32) -> void [linkage=internal] [noreturn] [fallthrough=ub] {
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_quick_exit]], read<i32>(%[[VALUE_status]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_c11_open_exclusive:[0-9]+]] @c11_open_exclusive(%[[VALUE_path:[0-9]+]] path: ptr<const i8>) -> ptr<@type[[TYPE__IO_FILE]]> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<ptr<@type[[TYPE__IO_FILE]]>, signature=fn(ptr<const i8>, ptr<const i8>) -> ptr<@type[[TYPE__IO_FILE]]>>(%[[VALUE_fopen]], read<ptr<const i8>>(%[[VALUE_path]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%[[VALUE_str]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_utf16_character:[0-9]+]] utf16_character: u16 [storage=automatic] = const<u16>(937);
// DEFAULT-NEXT:         let %[[VALUE_utf32_character:[0-9]+]] utf32_character: u32 [storage=automatic] = const<u32>(128578);
// DEFAULT-NEXT:         let %[[VALUE_anonymous:[0-9]+]] anonymous: @type[[TYPE_C11Anonymous]] [storage=automatic] = aggregate<@type[[TYPE_C11Anonymous]], zero_fill=true>(field0 = aggregate<@type[[TYPE7]], zero_fill=false>(field0 = const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE_aligned_object:[0-9]+]] aligned_object: @type[[TYPE_C11OverAligned]] [storage=automatic] = aggregate<@type[[TYPE_C11OverAligned]], zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         let %[[VALUE_utf16_state:[0-9]+]] utf16_state: @type[[TYPE0]] [storage=automatic] = aggregate<@type[[TYPE0]], zero_fill=true>(field0 = const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE_utf32_state:[0-9]+]] utf32_state: @type[[TYPE0]] [storage=automatic] = aggregate<@type[[TYPE0]], zero_fill=true>(field0 = const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE_converted16:[0-9]+]] converted16: u16 [storage=automatic] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE_converted32:[0-9]+]] converted32: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE_multibyte16:[0-9]+]] multibyte16: array<i8, 16> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_multibyte32:[0-9]+]] multibyte32: array<i8, 16> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_current_time:[0-9]+]] current_time: @type[[TYPE_timespec]] [storage=automatic] = aggregate<@type[[TYPE_timespec]], zero_fill=false>(field0 = widen<i64, reason=assign>(const<i32>(0)), field1 = widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE_thread:[0-9]+]] thread: u64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_mutex:[0-9]+]] mutex: @type[[TYPE5]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_condition:[0-9]+]] condition: @type[[TYPE6]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_once_control:[0-9]+]] once_control: @type[[TYPE2]] [storage=automatic] = aggregate<@type[[TYPE2]], zero_fill=false>(field0 = const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE_thread_key:[0-9]+]] thread_key: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_thread_increment:[0-9]+]] thread_increment: i32 [storage=automatic] = const<i32>(7);
// DEFAULT-NEXT:         let %[[VALUE_thread_result:[0-9]+]] thread_result: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_thread_created:[0-9]+]] thread_created: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_thread_joined:[0-9]+]] thread_joined: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_aligned_memory:[0-9]+]] aligned_memory: ptr<void> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_exclusive_first:[0-9]+]] exclusive_first: ptr<@type[[TYPE__IO_FILE]]> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_exclusive_second:[0-9]+]] exclusive_second: ptr<@type[[TYPE__IO_FILE]]> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_complex_value:[0-9]+]] complex_value: complex<f64> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_alignment_total:[0-9]+]] alignment_total: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_unicode_total:[0-9]+]] unicode_total: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_generic_total:[0-9]+]] generic_total: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_anonymous_total:[0-9]+]] anonymous_total: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_evaluation_total:[0-9]+]] evaluation_total: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_temporary_total:[0-9]+]] temporary_total: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_static_assert_total:[0-9]+]] static_assert_total: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_optional_total:[0-9]+]] optional_total: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_atomic_total:[0-9]+]] atomic_total: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_thread_total:[0-9]+]] thread_total: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_concurrency_total:[0-9]+]] concurrency_total: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_conversion_total:[0-9]+]] conversion_total: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_quick_total:[0-9]+]] quick_total: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_exclusive_total:[0-9]+]] exclusive_total: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_timespec_total:[0-9]+]] timespec_total: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_complex_total:[0-9]+]] complex_total: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_limits_total:[0-9]+]] limits_total: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%[[VALUE_alignment_total]], add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=always>(const<u64>(4))), reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=always>(const<u64>(32)))), from_bool<i32, reason=promotion>(eq<u64>(rem<u64, by_zero=ub>(ptr_to_int<u64, reason=explicit>(array_decay<ptr<u8>, length=Some(64)>(%[[VALUE_c11_aligned_buffer]])), widen<u64, reason=usual_arith>(const<u32>(64))), widen<u64, reason=usual_arith>(const<u32>(0))))), from_bool<i32, reason=promotion>(eq<u64>(rem<u64, by_zero=ub>(ptr_to_int<u64, reason=explicit>(addr_of<ptr<@type[[TYPE_C11OverAligned]]>>(%[[VALUE_aligned_object]])), widen<u64, reason=usual_arith>(const<u32>(32))), widen<u64, reason=usual_arith>(const<u32>(0))))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_unicode_total]], add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(%[[VALUE_utf8_text]]), const<i32>(0))))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<const i8>, length=Some(3)>(%[[VALUE_utf8_text]]), const<i32>(1)))))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(deref(ptr_offset<ptr<const u16>, subtract=false, element=u16, overflow=ub>(array_decay<ptr<const u16>, length=Some(2)>(%[[VALUE_utf16_text]]), const<i32>(0))))))), reinterpret<i32, reason=explicit, fits=unknown>(read<u32>(deref(ptr_offset<ptr<const u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<const u32>, length=Some(2)>(%[[VALUE_utf32_text]]), const<i32>(0)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_utf16_character]])))), reinterpret<i32, reason=explicit, fits=unknown>(read<u32>(%[[VALUE_utf32_character]]))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_generic_total]], add<i32, overflow=ub>(add<i32, overflow=ub>(const<i32>(11), const<i32>(22)), const<i32>(33)));
// DEFAULT-NEXT:         write<i32>(field0(field0(%[[VALUE_anonymous]])), const<i32>(31));
// DEFAULT-NEXT:         write<i32>(field0(field1(%[[VALUE_anonymous]])), const<i32>(37));
// DEFAULT-NEXT:         write<i32>(field1(field1(%[[VALUE_anonymous]])), const<i32>(41));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_anonymous_total]], add<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(field0(field0(%[[VALUE_anonymous]]))), read<i32>(field0(field1(%[[VALUE_anonymous]])))), read<i32>(field1(field1(%[[VALUE_anonymous]])))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_c11_evaluation_total]], const<i32>(0));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_evaluation_total]], add<i32, overflow=ub>(add<i32, overflow=ub>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_c11_evaluation_step]], const<i32>(2)), call<i32, signature=fn(i32) -> i32>(%[[VALUE_c11_evaluation_step]], const<i32>(3))), read<i32>(%[[VALUE_c11_evaluation_total]])));
// DEFAULT-NEXT:         add<i32, overflow=ub>(add<i32, overflow=ub>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_c11_evaluation_step]], const<i32>(2)), call<i32, signature=fn(i32) -> i32>(%[[VALUE_c11_evaluation_step]], const<i32>(3))), read<i32>(%[[VALUE_c11_evaluation_total]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_temporary_total]], read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(field0(temporary %[[VALUE6:[0-9]+]] = call<@type[[TYPE_C11Temporary]], signature=fn(i32) -> @type[[TYPE_C11Temporary]], abi=sysv64(scalar) -> native_c>(%[[VALUE_c11_make_temporary]], const<i32>(43)))), const<i32>(1)))));
// DEFAULT-NEXT:         read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(field0(temporary %[[VALUE6]] = call<@type[[TYPE_C11Temporary]], signature=fn(i32) -> @type[[TYPE_C11Temporary]], abi=sysv64(scalar) -> native_c>(%[[VALUE_c11_make_temporary]], const<i32>(43)))), const<i32>(1))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_static_assert_total]], const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_optional_total]], add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(const<i32>(0), const<i32>(0)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)));
// DEFAULT-NEXT:         write<i32>(deref(addr_of<ptr<atomic i32>>(%[[VALUE_c11_atomic_total]])), const<i32>(5));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_c11_thread_local_value]], const<i32>(17));
// DEFAULT-NEXT:         write<i32>(deref(call<ptr<i32>, signature=fn() -> ptr<i32>>(%[[VALUE_memory_order_acq_rel]])), const<i32>(0));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_thread_created]], call<i32, signature=fn(ptr<u64>, ptr<fn(ptr<void>) -> i32>, ptr<void>) -> i32>(%[[VALUE_thrd_create]], addr_of<ptr<u64>>(%[[VALUE_thread]]), function_decay<ptr<fn(ptr<void>) -> i32>>(%[[VALUE_c11_thread_worker]]), pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i32>>(%[[VALUE_thread_increment]]))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<u64>, ptr<fn(ptr<void>) -> i32>, ptr<void>) -> i32>(%[[VALUE_thrd_create]], addr_of<ptr<u64>>(%[[VALUE_thread]]), function_decay<ptr<fn(ptr<void>) -> i32>>(%[[VALUE_c11_thread_worker]]), pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i32>>(%[[VALUE_thread_increment]])));
// DEFAULT-NEXT:         let %[[VALUE7:[0-9]+]]: i32 [synthetic];
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%[[VALUE_thread_created]]), const<i32>(0))
// DEFAULT-NEXT:             write<i32>(%[[VALUE7]], call<i32, signature=fn(u64, ptr<i32>) -> i32>(%[[VALUE_thrd_join]], read<u64>(%[[VALUE_thread]]), addr_of<ptr<i32>>(%[[VALUE_thread_result]])));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<i32>(%[[VALUE7]], neg<i32, overflow=ub>(const<i32>(1)));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_thread_joined]], read<i32>(%[[VALUE7]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_atomic_total]], read<i32, atomic=seq_cst>(deref(addr_of<ptr<atomic i32>>(%[[VALUE_c11_atomic_total]]))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_thread_total]], add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(eq<i32>(read<i32>(%[[VALUE_thread_created]]), const<i32>(0))), from_bool<i32, reason=promotion>(eq<i32>(read<i32>(%[[VALUE_thread_joined]]), const<i32>(0)))), read<i32>(%[[VALUE_thread_result]])), read<i32>(%[[VALUE_c11_thread_local_value]])), from_bool<i32, reason=promotion>(eq<i32>(read<i32>(deref(call<ptr<i32>, signature=fn() -> ptr<i32>>(%[[VALUE_memory_order_acq_rel]]))), const<i32>(0)))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_concurrency_total]], const<i32>(0));
// DEFAULT-NEXT:         if eq<i32>(call<i32, signature=fn(ptr<@type[[TYPE5]]>, i32) -> i32>(%[[VALUE_mtx_init]], addr_of<ptr<@type[[TYPE5]]>>(%[[VALUE_mutex]]), const<i32>(0)), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE8:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_concurrency_total]]);
// DEFAULT-NEXT:                 let %[[VALUE9:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE8]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_concurrency_total]], read<i32>(%[[VALUE9]]));
// DEFAULT-NEXT:                 let %[[VALUE10:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_concurrency_total]]);
// DEFAULT-NEXT:                 let %[[VALUE11:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE10]]), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<@type[[TYPE5]]>) -> i32>(%[[VALUE_mtx_lock]], addr_of<ptr<@type[[TYPE5]]>>(%[[VALUE_mutex]])), const<i32>(0))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_concurrency_total]], read<i32>(%[[VALUE11]]));
// DEFAULT-NEXT:                 let %[[VALUE12:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_concurrency_total]]);
// DEFAULT-NEXT:                 let %[[VALUE13:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE12]]), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(ptr<@type[[TYPE5]]>) -> i32>(%[[VALUE_mtx_unlock]], addr_of<ptr<@type[[TYPE5]]>>(%[[VALUE_mutex]])), const<i32>(0))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_concurrency_total]], read<i32>(%[[VALUE13]]));
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<@type[[TYPE5]]>) -> void>(%[[VALUE_mtx_destroy]], addr_of<ptr<@type[[TYPE5]]>>(%[[VALUE_mutex]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         if eq<i32>(call<i32, signature=fn(ptr<@type[[TYPE6]]>) -> i32>(%[[VALUE_cnd_init]], addr_of<ptr<@type[[TYPE6]]>>(%[[VALUE_condition]])), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE14:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_concurrency_total]]);
// DEFAULT-NEXT:                 let %[[VALUE15:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE14]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_concurrency_total]], read<i32>(%[[VALUE15]]));
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<@type[[TYPE6]]>) -> void>(%[[VALUE_cnd_destroy]], addr_of<ptr<@type[[TYPE6]]>>(%[[VALUE_condition]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         write<i32>(%[[VALUE_c11_once_total]], const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE2]]>, ptr<fn() -> void>) -> void>(%[[VALUE_call_once]], addr_of<ptr<@type[[TYPE2]]>>(%[[VALUE_once_control]]), function_decay<ptr<fn() -> void>>(%[[VALUE_c11_once_handler]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE2]]>, ptr<fn() -> void>) -> void>(%[[VALUE_call_once]], addr_of<ptr<@type[[TYPE2]]>>(%[[VALUE_once_control]]), function_decay<ptr<fn() -> void>>(%[[VALUE_c11_once_handler]]));
// DEFAULT-NEXT:         let %[[VALUE16:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_concurrency_total]]);
// DEFAULT-NEXT:         let %[[VALUE17:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE16]]), read<i32>(%[[VALUE_c11_once_total]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_concurrency_total]], read<i32>(%[[VALUE17]]));
// DEFAULT-NEXT:         if eq<i32>(call<i32, signature=fn(ptr<u32>, ptr<fn(ptr<void>) -> void>) -> i32>(%[[VALUE_tss_create]], addr_of<ptr<u32>>(%[[VALUE_thread_key]]), null<ptr<fn(ptr<void>) -> void>>), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE18:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_concurrency_total]]);
// DEFAULT-NEXT:                 let %[[VALUE19:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE18]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_concurrency_total]], read<i32>(%[[VALUE19]]));
// DEFAULT-NEXT:                 let %[[VALUE20:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_concurrency_total]]);
// DEFAULT-NEXT:                 let %[[VALUE21:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE20]]), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(u32, ptr<void>) -> i32>(%[[VALUE_tss_set]], read<u32>(%[[VALUE_thread_key]]), pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i32>>(%[[VALUE_thread_increment]]))), const<i32>(0))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_concurrency_total]], read<i32>(%[[VALUE21]]));
// DEFAULT-NEXT:                 let %[[VALUE22:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_concurrency_total]]);
// DEFAULT-NEXT:                 let %[[VALUE23:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE22]]), from_bool<i32, reason=promotion>(eq<ptr<void>>(call<ptr<void>, signature=fn(u32) -> ptr<void>>(%[[VALUE_tss_get]], read<u32>(%[[VALUE_thread_key]])), pointer_cast<ptr<void>, reason=usual_arith>(addr_of<ptr<i32>>(%[[VALUE_thread_increment]])))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_concurrency_total]], read<i32>(%[[VALUE23]]));
// DEFAULT-NEXT:                 call<void, signature=fn(u32) -> void>(%[[VALUE_tss_delete]], read<u32>(%[[VALUE_thread_key]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         let %[[VALUE24:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_concurrency_total]]);
// DEFAULT-NEXT:         let %[[VALUE25:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE24]]), from_bool<i32, reason=promotion>(ge<i32>(const<i32>(4), const<i32>(1))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_concurrency_total]], read<i32>(%[[VALUE25]]));
// DEFAULT-NEXT:         write<u16>(%[[VALUE_converted16]], reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_converted32]], reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_conversion_total]], add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(call<u64, signature=fn(ptr<u16>, ptr<const i8>, u64, ptr<@type[[TYPE0]]>) -> u64>(%[[VALUE_mbrtoc16]], addr_of<ptr<u16>>(%[[VALUE_converted16]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str_2]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), addr_of<ptr<@type[[TYPE0]]>>(%[[VALUE_utf16_state]])))), reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(call<u64, signature=fn(ptr<i8>, u16, ptr<@type[[TYPE0]]>) -> u64>(%[[VALUE_c16rtomb]], array_decay<ptr<i8>, length=Some(16)>(%[[VALUE_multibyte16]]), const<u16>(65), addr_of<ptr<@type[[TYPE0]]>>(%[[VALUE_utf16_state]]))))), reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(call<u64, signature=fn(ptr<u32>, ptr<const i8>, u64, ptr<@type[[TYPE0]]>) -> u64>(%[[VALUE_mbrtoc32]], addr_of<ptr<u32>>(%[[VALUE_converted32]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str_3]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), addr_of<ptr<@type[[TYPE0]]>>(%[[VALUE_utf32_state]]))))), reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(call<u64, signature=fn(ptr<i8>, u32, ptr<@type[[TYPE0]]>) -> u64>(%[[VALUE_c32rtomb]], array_decay<ptr<i8>, length=Some(16)>(%[[VALUE_multibyte32]]), const<u32>(66), addr_of<ptr<@type[[TYPE0]]>>(%[[VALUE_utf32_state]]))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_converted16]])))), reinterpret<i32, reason=explicit, fits=unknown>(read<u32>(%[[VALUE_converted32]]))), widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(16)>(%[[VALUE_multibyte16]]), const<i32>(0)))))), widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(16)>(%[[VALUE_multibyte32]]), const<i32>(0)))))));
// DEFAULT-NEXT:         add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(call<u64, signature=fn(ptr<u16>, ptr<const i8>, u64, ptr<@type[[TYPE0]]>) -> u64>(%[[VALUE_mbrtoc16]], addr_of<ptr<u16>>(%[[VALUE_converted16]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str_2]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), addr_of<ptr<@type[[TYPE0]]>>(%[[VALUE_utf16_state]])))), reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(call<u64, signature=fn(ptr<i8>, u16, ptr<@type[[TYPE0]]>) -> u64>(%[[VALUE_c16rtomb]], array_decay<ptr<i8>, length=Some(16)>(%[[VALUE_multibyte16]]), const<u16>(65), addr_of<ptr<@type[[TYPE0]]>>(%[[VALUE_utf16_state]]))))), reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(call<u64, signature=fn(ptr<u32>, ptr<const i8>, u64, ptr<@type[[TYPE0]]>) -> u64>(%[[VALUE_mbrtoc32]], addr_of<ptr<u32>>(%[[VALUE_converted32]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str_3]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))), addr_of<ptr<@type[[TYPE0]]>>(%[[VALUE_utf32_state]]))))), reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(call<u64, signature=fn(ptr<i8>, u32, ptr<@type[[TYPE0]]>) -> u64>(%[[VALUE_c32rtomb]], array_decay<ptr<i8>, length=Some(16)>(%[[VALUE_multibyte32]]), const<u32>(66), addr_of<ptr<@type[[TYPE0]]>>(%[[VALUE_utf32_state]]))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_converted16]])))), reinterpret<i32, reason=explicit, fits=unknown>(read<u32>(%[[VALUE_converted32]]))), widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(16)>(%[[VALUE_multibyte16]]), const<i32>(0)))))), widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(16)>(%[[VALUE_multibyte32]]), const<i32>(0))))));
// DEFAULT-NEXT:         write<ptr<void>>(%[[VALUE_aligned_memory]], call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_aligned_alloc]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(64))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(64)))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_aligned_alloc]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(64))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(64))));
// DEFAULT-NEXT:         let %[[VALUE26:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_alignment_total]]);
// DEFAULT-NEXT:         let %[[VALUE27:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE26]]), from_bool<i32, reason=promotion>(logical_and<bool>(ne<ptr<void>>(read<ptr<void>>(%[[VALUE_aligned_memory]]), null<ptr<void>>), eq<u64>(rem<u64, by_zero=ub>(ptr_to_int<u64, reason=explicit>(read<ptr<void>>(%[[VALUE_aligned_memory]])), widen<u64, reason=usual_arith>(const<u32>(64))), widen<u64, reason=usual_arith>(const<u32>(0))))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_alignment_total]], read<i32>(%[[VALUE27]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_free]], read<ptr<void>>(%[[VALUE_aligned_memory]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_quick_total]], from_bool<i32, reason=assign>(eq<i32>(call<i32, signature=fn(ptr<fn() -> void>) -> i32>(%[[VALUE_at_quick_exit]], function_decay<ptr<fn() -> void>>(%[[VALUE_c11_quick_handler]])), const<i32>(0))));
// DEFAULT-NEXT:         from_bool<i32, reason=assign>(eq<i32>(call<i32, signature=fn(ptr<fn() -> void>) -> i32>(%[[VALUE_at_quick_exit]], function_decay<ptr<fn() -> void>>(%[[VALUE_c11_quick_handler]])), const<i32>(0)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>) -> i32>(%[[VALUE_remove]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(24)>(%[[VALUE_str_4]])));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE__IO_FILE]]>>(%[[VALUE_exclusive_first]], call<ptr<@type[[TYPE__IO_FILE]]>, signature=fn(ptr<const i8>) -> ptr<@type[[TYPE__IO_FILE]]>>(%[[VALUE_c11_open_exclusive]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(24)>(%[[VALUE_str_5]]))));
// DEFAULT-NEXT:         call<ptr<@type[[TYPE__IO_FILE]]>, signature=fn(ptr<const i8>) -> ptr<@type[[TYPE__IO_FILE]]>>(%[[VALUE_c11_open_exclusive]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(24)>(%[[VALUE_str_5]])));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE__IO_FILE]]>>(%[[VALUE_exclusive_second]], call<ptr<@type[[TYPE__IO_FILE]]>, signature=fn(ptr<const i8>) -> ptr<@type[[TYPE__IO_FILE]]>>(%[[VALUE_c11_open_exclusive]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(24)>(%[[VALUE_str_6]]))));
// DEFAULT-NEXT:         call<ptr<@type[[TYPE__IO_FILE]]>, signature=fn(ptr<const i8>) -> ptr<@type[[TYPE__IO_FILE]]>>(%[[VALUE_c11_open_exclusive]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(24)>(%[[VALUE_str_6]])));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_exclusive_total]], from_bool<i32, reason=assign>(logical_and<bool>(ne<ptr<@type[[TYPE__IO_FILE]]>>(read<ptr<@type[[TYPE__IO_FILE]]>>(%[[VALUE_exclusive_first]]), null<ptr<@type[[TYPE__IO_FILE]]>>), eq<ptr<@type[[TYPE__IO_FILE]]>>(read<ptr<@type[[TYPE__IO_FILE]]>>(%[[VALUE_exclusive_second]]), null<ptr<@type[[TYPE__IO_FILE]]>>))));
// DEFAULT-NEXT:         if ne<ptr<@type[[TYPE__IO_FILE]]>>(read<ptr<@type[[TYPE__IO_FILE]]>>(%[[VALUE_exclusive_first]]), null<ptr<@type[[TYPE__IO_FILE]]>>)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<@type[[TYPE__IO_FILE]]>) -> i32>(%[[VALUE_fclose]], read<ptr<@type[[TYPE__IO_FILE]]>>(%[[VALUE_exclusive_first]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         if ne<ptr<@type[[TYPE__IO_FILE]]>>(read<ptr<@type[[TYPE__IO_FILE]]>>(%[[VALUE_exclusive_second]]), null<ptr<@type[[TYPE__IO_FILE]]>>)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<@type[[TYPE__IO_FILE]]>) -> i32>(%[[VALUE_fclose]], read<ptr<@type[[TYPE__IO_FILE]]>>(%[[VALUE_exclusive_second]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>) -> i32>(%[[VALUE_remove]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(24)>(%[[VALUE_str_7]])));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_timespec_total]], from_bool<i32, reason=assign>(logical_and<bool>(logical_and<bool>(eq<i32>(call<i32, signature=fn(ptr<@type[[TYPE_timespec]]>, i32) -> i32>(%[[VALUE_timespec_get]], addr_of<ptr<@type[[TYPE_timespec]]>>(%[[VALUE_current_time]]), const<i32>(1)), const<i32>(1)), ge<i64>(read<i64>(field1(%[[VALUE_current_time]])), widen<i64, reason=usual_arith>(const<i32>(0)))), lt<i64>(read<i64>(field1(%[[VALUE_current_time]])), const<i64>(1000000000)))));
// DEFAULT-NEXT:         from_bool<i32, reason=assign>(logical_and<bool>(logical_and<bool>(eq<i32>(call<i32, signature=fn(ptr<@type[[TYPE_timespec]]>, i32) -> i32>(%[[VALUE_timespec_get]], addr_of<ptr<@type[[TYPE_timespec]]>>(%[[VALUE_current_time]]), const<i32>(1)), const<i32>(1)), ge<i64>(read<i64>(field1(%[[VALUE_current_time]])), widen<i64, reason=usual_arith>(const<i32>(0)))), lt<i64>(read<i64>(field1(%[[VALUE_current_time]])), const<i64>(1000000000))));
// DEFAULT-NEXT:         write<complex<f64>>(%[[VALUE_complex_value]], aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(2.0), index1 = const<f64>(3.0)));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_complex_total]], add<i32, overflow=ub>(from_bool<i32, reason=promotion>(eq<f64, exceptions=ignore>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(native_c) -> scalar>(%[[VALUE_memory_order_release]], read<complex<f64>>(%[[VALUE_complex_value]])), const<f64>(2.0))), from_bool<i32, reason=promotion>(eq<f64, exceptions=ignore>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(native_c) -> scalar>(%[[VALUE_memory_order_consume]], read<complex<f64>>(%[[VALUE_complex_value]])), const<f64>(3.0)))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_limits_total]], add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(ge<i32>(const<i32>(9), const<i32>(6))), from_bool<i32, reason=promotion>(ge<i32>(const<i32>(17), const<i32>(10)))), from_bool<i32, reason=promotion>(ge<i32>(const<i32>(21), const<i32>(10)))), from_bool<i32, reason=promotion>(gt<f32, exceptions=ignore>(const<f32>(1e-45), const<f32>(0.0)))), from_bool<i32, reason=promotion>(gt<f64, exceptions=ignore>(const<f64>(5e-324), const<f64>(0.0)))), from_bool<i32, reason=promotion>(gt<f80, exceptions=ignore>(const<f80>(3.64519953188247460253E-4951), const<f80>(0)))), from_bool<i32, reason=promotion>(ge<i32>(const<i32>(1), neg<i32, overflow=ub>(const<i32>(1))))), from_bool<i32, reason=promotion>(ge<i32>(const<i32>(1), neg<i32, overflow=ub>(const<i32>(1))))), from_bool<i32, reason=promotion>(ge<i32>(const<i32>(1), neg<i32, overflow=ub>(const<i32>(1))))));
// DEFAULT-NEXT:         let %[[VALUE28:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_limits_total]]);
// DEFAULT-NEXT:         let %[[VALUE29:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE28]]), const<i32>(0));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_limits_total]], read<i32>(%[[VALUE29]]));
// DEFAULT-NEXT:         if ne<i32>(read<i32, volatile>(%[[VALUE_c11_never_flag]]), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<void, signature=fn(i32) -> void>(%[[VALUE_c11_never_return]], const<i32>(99));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(54)>(%[[VALUE_str_8]])), read<i32>(%[[VALUE_alignment_total]]), read<i32>(%[[VALUE_unicode_total]]), read<i32>(%[[VALUE_generic_total]]), read<i32>(%[[VALUE_anonymous_total]]), read<i32>(%[[VALUE_evaluation_total]]), read<i32>(%[[VALUE_temporary_total]]), read<i32>(%[[VALUE_static_assert_total]]), read<i32>(%[[VALUE_optional_total]]), read<i32>(%[[VALUE_atomic_total]]), read<i32>(%[[VALUE_thread_total]]), read<i32>(%[[VALUE_concurrency_total]]), read<i32>(%[[VALUE_conversion_total]]), read<i32>(%[[VALUE_quick_total]]), read<i32>(%[[VALUE_exclusive_total]]), read<i32>(%[[VALUE_timespec_total]]), read<i32>(%[[VALUE_complex_total]]), read<i32>(%[[VALUE_limits_total]]), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(i32) -> i32>(%[[VALUE_putchar]], const<i32>(10));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
