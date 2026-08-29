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
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(thread_local)]
// LOWERING-NEXT: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn __muldc3(a: f64, b: f64, c: f64, d: f64) -> num_complex::Complex<f64>;
// LOWERING-NEXT:     fn __divdc3(a: f64, b: f64, c: f64, d: f64) -> num_complex::Complex<f64>;
// LOWERING-NEXT:     fn __mulsc3(a: f32, b: f32, c: f32, d: f32) -> num_complex::Complex<f32>;
// LOWERING-NEXT:     fn __divsc3(a: f32, b: f32, c: f32, d: f32) -> num_complex::Complex<f32>;
// LOWERING-NEXT: }
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
// LOWERING-NEXT: struct C11Anonymous {
// LOWERING-NEXT:     __slate_anon_0: anon_0,
// LOWERING-NEXT:     __slate_anon_1: anon_1,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C, align(32))]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct C11OverAligned {
// LOWERING-NEXT:     value: u8,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct C11Temporary {
// LOWERING-NEXT:     values: [i32; 3],
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct __mbstate_t {
// LOWERING-NEXT:     __count: i32,
// LOWERING-NEXT:     __value: anon_2,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: union anon_0 {
// LOWERING-NEXT:     integer: i32,
// LOWERING-NEXT:     __slate_anon_1: f64,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct anon_1 {
// LOWERING-NEXT:     x: i32,
// LOWERING-NEXT:     y: i32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: union anon_2 {
// LOWERING-NEXT:     __wch: i32,
// LOWERING-NEXT:     __wchb: [i8; 4],
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct anon_struct {
// LOWERING-NEXT:     __slate_anon_0: u64,
// LOWERING-NEXT:     __slate_anon_1: i32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: union cnd_t {
// LOWERING-NEXT:     __size: [i8; 48],
// LOWERING-NEXT:     __align: i64,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: union mtx_t {
// LOWERING-NEXT:     __size: [i8; 40],
// LOWERING-NEXT:     __align: i64,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut c11_aligned_buffer: aligned::Aligned<aligned::A64, [u8; 64]> = aligned::Aligned([0; 64]);
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut c11_atomic_total: i32 = 0;
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut c11_evaluation_total: i32 = 0;
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut c11_never_flag: i32 = 0;
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut c11_once_total: i32 = 0;
// LOWERING-EMPTY:
// LOWERING-NEXT: #[thread_local]
// LOWERING-NEXT: static mut c11_thread_local_value: i32 = 0;
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut main_utf16_text: [u16; 2] = [937, 0];
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut main_utf32_text: [u32; 2] = [128578, 0];
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut main_utf8_text: [i8; 3] = [-50, -87, 0];
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn __errno_location() -> *mut i32;
// LOWERING-NEXT:     fn thrd_create(_0: *mut u64, _1: Option<unsafe extern "C" fn(*mut core::ffi::c_void) -> i32>, _2: *mut core::ffi::c_void) -> i32;
// LOWERING-NEXT:     fn thrd_join(_0: u64, _1: *mut i32) -> i32;
// LOWERING-NEXT:     fn mtx_init(_0: *mut mtx_t, _1: i32) -> i32;
// LOWERING-NEXT:     fn mtx_lock(_0: *mut mtx_t) -> i32;
// LOWERING-NEXT:     fn mtx_unlock(_0: *mut mtx_t) -> i32;
// LOWERING-NEXT:     fn mtx_destroy(_0: *mut mtx_t);
// LOWERING-NEXT:     fn cnd_init(_0: *mut cnd_t) -> i32;
// LOWERING-NEXT:     fn cnd_destroy(_0: *mut cnd_t);
// LOWERING-NEXT:     fn call_once(_0: *mut i32, _1: Option<unsafe extern "C" fn()>);
// LOWERING-NEXT:     fn tss_create(_0: *mut u32, _1: Option<unsafe extern "C" fn(*mut core::ffi::c_void)>) -> i32;
// LOWERING-NEXT:     fn tss_set(_0: u32, _1: *mut core::ffi::c_void) -> i32;
// LOWERING-NEXT:     fn tss_get(_0: u32) -> *mut core::ffi::c_void;
// LOWERING-NEXT:     fn tss_delete(_0: u32);
// LOWERING-NEXT:     fn mbrtoc16(_0: *mut u16, _1: *const i8, _2: usize, _3: *mut __mbstate_t) -> usize;
// LOWERING-NEXT:     fn c16rtomb(_0: *mut i8, _1: u16, _2: *mut __mbstate_t) -> usize;
// LOWERING-NEXT:     fn mbrtoc32(_0: *mut u32, _1: *const i8, _2: usize, _3: *mut __mbstate_t) -> usize;
// LOWERING-NEXT:     fn c32rtomb(_0: *mut i8, _1: u32, _2: *mut __mbstate_t) -> usize;
// LOWERING-NEXT:     fn aligned_alloc(_0: usize, _1: usize) -> *mut core::ffi::c_void;
// LOWERING-NEXT:     fn free(_0: *mut core::ffi::c_void);
// LOWERING-NEXT:     fn at_quick_exit(_0: Option<unsafe extern "C" fn()>) -> i32;
// LOWERING-NEXT:     fn remove(_0: *const i8) -> i32;
// LOWERING-NEXT:     fn fopen(_0: *const i8, _1: *const i8) -> *mut libc::FILE;
// LOWERING-NEXT:     fn fclose(_0: *mut libc::FILE) -> i32;
// LOWERING-NEXT:     fn timespec_get(_0: *mut libc::timespec, _1: i32) -> i32;
// LOWERING-NEXT:     fn quick_exit(_0: i32) -> !;
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT:     fn putchar(_0: i32) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn c11_evaluation_step(arg4: i32) -> i32 {
// LOWERING-NEXT:     let mut value: i32 = 0;
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     value = arg4;
// LOWERING-NEXT:     let _v0: i32 = value;
// LOWERING-NEXT:     let _v1: i32 = unsafe { c11_evaluation_total };
// LOWERING-NEXT:     let _v2: i32 = _v1 + _v0;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         c11_evaluation_total = _v2;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v3: i32 = value;
// LOWERING-NEXT:     let _v4: i32 = 2;
// LOWERING-NEXT:     let _v5: i32 = _v3 * _v4;
// LOWERING-NEXT:     __retval = _v5;
// LOWERING-NEXT:     let _v6: i32 = __retval;
// LOWERING-NEXT:     return _v6;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn c11_make_temporary(arg3: i32) -> anon_struct {
// LOWERING-NEXT:     let mut coerce: anon_struct = anon_struct { __slate_anon_0: 0, __slate_anon_1: 0 };
// LOWERING-NEXT:     let mut base: i32 = 0;
// LOWERING-NEXT:     let mut __retval: C11Temporary = C11Temporary { values: [0; 3] };
// LOWERING-NEXT:     base = arg3;
// LOWERING-NEXT:     let _v0: *mut i32 = std::ptr::addr_of_mut!(__retval.values) as *mut i32;
// LOWERING-NEXT:     let _v1: i32 = base;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *_v0 = _v1;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v2: i64 = 1;
// LOWERING-NEXT:     let _v3: *mut i32 = unsafe { _v0.add(1) };
// LOWERING-NEXT:     let _v4: i32 = base;
// LOWERING-NEXT:     let _v5: i32 = 1;
// LOWERING-NEXT:     let _v6: i32 = _v4 + _v5;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *_v3 = _v6;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v7: i64 = 2;
// LOWERING-NEXT:     let _v8: *mut i32 = unsafe { _v0.add(2) };
// LOWERING-NEXT:     let _v9: i32 = base;
// LOWERING-NEXT:     let _v10: i32 = 2;
// LOWERING-NEXT:     let _v11: i32 = _v9 + _v10;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *_v8 = _v11;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v12: C11Temporary = __retval;
// LOWERING-NEXT:     let _v13: *mut C11Temporary = std::ptr::addr_of_mut!(coerce) as *mut C11Temporary;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *_v13 = _v12;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v14: anon_struct = coerce;
// LOWERING-NEXT:     return _v14;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: extern "C" fn c11_thread_worker(arg2: *mut core::ffi::c_void) -> i32 {
// LOWERING-NEXT:     let mut argument: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut increment: i32 = 0;
// LOWERING-NEXT:     argument = arg2;
// LOWERING-NEXT:     let _v0: *mut core::ffi::c_void = argument;
// LOWERING-NEXT:     let _v1: *mut i32 = _v0 as *mut i32;
// LOWERING-NEXT:     let _v2: i32 = unsafe { *_v1 };
// LOWERING-NEXT:     increment = _v2;
// LOWERING-NEXT:     let _v3: i32 = 29;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         c11_thread_local_value = _v3;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v4: i32 = 34;
// LOWERING-NEXT:     let _v5: *mut i32 = unsafe { __errno_location() };
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *_v5 = _v4;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v6: i32 = increment;
// LOWERING-NEXT:     let _v7: i32 = unsafe { std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(c11_atomic_total)).fetch_add(_v6, std::sync::atomic::Ordering::SeqCst) };
// LOWERING-NEXT:     let _v8: i32 = unsafe { c11_thread_local_value };
// LOWERING-NEXT:     let _v9: *mut i32 = unsafe { __errno_location() };
// LOWERING-NEXT:     let _v10: i32 = unsafe { *_v9 };
// LOWERING-NEXT:     let _v11: i32 = 34;
// LOWERING-NEXT:     let _v12: bool = _v10 == _v11;
// LOWERING-NEXT:     let _v13: i32 = _v12 as i32;
// LOWERING-NEXT:     let _v14: i32 = _v8 + _v13;
// LOWERING-NEXT:     __retval = _v14;
// LOWERING-NEXT:     let _v15: i32 = __retval;
// LOWERING-NEXT:     return _v15;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: extern "C" fn c11_once_handler() {
// LOWERING-NEXT:     let _v0: i32 = 1;
// LOWERING-NEXT:     let _v1: i32 = unsafe { c11_once_total };
// LOWERING-NEXT:     let _v2: i32 = _v1 + _v0;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         c11_once_total = _v2;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: extern "C" fn c11_quick_handler() {
// LOWERING-NEXT:     let _v0: i32 = 100;
// LOWERING-NEXT:     let _v1: i32 = unsafe { std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(c11_atomic_total)).fetch_add(_v0, std::sync::atomic::Ordering::Relaxed) };
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn c11_open_exclusive(arg1: *mut i8) -> *mut libc::FILE {
// LOWERING-NEXT:     let mut path: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut __retval: *mut libc::FILE = std::ptr::null_mut();
// LOWERING-NEXT:     path = arg1;
// LOWERING-NEXT:     let _v0: *mut i8 = path;
// LOWERING-NEXT:     let _v1: *mut i8 = b"wx\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v2: *mut libc::FILE = unsafe { fopen(_v0 as *const i8, _v1 as *const i8) };
// LOWERING-NEXT:     __retval = _v2;
// LOWERING-NEXT:     let _v3: *mut libc::FILE = __retval;
// LOWERING-NEXT:     return _v3;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn c11_never_return(arg0: i32) -> ! {
// LOWERING-NEXT:     let mut status: i32 = 0;
// LOWERING-NEXT:     status = arg0;
// LOWERING-NEXT:     let _v0: i32 = status;
// LOWERING-NEXT:     unsafe { quick_exit(_v0 as i32) }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut coerce: anon_struct = anon_struct { __slate_anon_0: 0, __slate_anon_1: 0 };
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut utf16_character: u16 = 0;
// LOWERING-NEXT:     let mut utf32_character: u32 = 0;
// LOWERING-NEXT:     let mut anonymous: C11Anonymous = C11Anonymous { __slate_anon_0: anon_0 { integer: 0 }, __slate_anon_1: anon_1 { x: 0, y: 0 } };
// LOWERING-NEXT:     let mut aligned_object: aligned::Aligned<aligned::A32, C11OverAligned> = aligned::Aligned(C11OverAligned { value: 0 });
// LOWERING-NEXT:     let mut utf16_state: __mbstate_t = __mbstate_t { __count: 0, __value: anon_2 { __wch: 0 } };
// LOWERING-NEXT:     let mut utf32_state: __mbstate_t = __mbstate_t { __count: 0, __value: anon_2 { __wch: 0 } };
// LOWERING-NEXT:     let mut converted16: u16 = 0;
// LOWERING-NEXT:     let mut converted32: u32 = 0;
// LOWERING-NEXT:     let mut multibyte16: [i8; 4] = [0; 4];
// LOWERING-NEXT:     let mut multibyte32: [i8; 4] = [0; 4];
// LOWERING-NEXT:     let mut current_time: libc::timespec = libc::timespec { tv_sec: 0, tv_nsec: 0 };
// LOWERING-NEXT:     let mut thread: u64 = 0;
// LOWERING-NEXT:     let mut mutex: mtx_t = mtx_t { __size: [0; 40] };
// LOWERING-NEXT:     let mut condition: cnd_t = cnd_t { __size: [0; 48] };
// LOWERING-NEXT:     let mut once_control: i32 = 0;
// LOWERING-NEXT:     let mut thread_key: u32 = 0;
// LOWERING-NEXT:     let mut thread_increment: i32 = 0;
// LOWERING-NEXT:     let mut thread_result: i32 = 0;
// LOWERING-NEXT:     let mut thread_created: i32 = 0;
// LOWERING-NEXT:     let mut thread_joined: i32 = 0;
// LOWERING-NEXT:     let mut aligned_memory: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let mut exclusive_first: *mut libc::FILE = std::ptr::null_mut();
// LOWERING-NEXT:     let mut exclusive_second: *mut libc::FILE = std::ptr::null_mut();
// LOWERING-NEXT:     let mut complex_value: num_complex::Complex<f64> = num_complex::Complex { re: 0.0, im: 0.0 };
// LOWERING-NEXT:     let mut alignment_total: i32 = 0;
// LOWERING-NEXT:     let mut unicode_total: i32 = 0;
// LOWERING-NEXT:     let mut generic_total: i32 = 0;
// LOWERING-NEXT:     let mut anonymous_total: i32 = 0;
// LOWERING-NEXT:     let mut evaluation_total: i32 = 0;
// LOWERING-NEXT:     let mut temporary_total: i32 = 0;
// LOWERING-NEXT:     let mut static_assert_total: i32 = 0;
// LOWERING-NEXT:     let mut optional_total: i32 = 0;
// LOWERING-NEXT:     let mut atomic_total: i32 = 0;
// LOWERING-NEXT:     let mut thread_total: i32 = 0;
// LOWERING-NEXT:     let mut concurrency_total: i32 = 0;
// LOWERING-NEXT:     let mut conversion_total: i32 = 0;
// LOWERING-NEXT:     let mut quick_total: i32 = 0;
// LOWERING-NEXT:     let mut exclusive_total: i32 = 0;
// LOWERING-NEXT:     let mut timespec_total: i32 = 0;
// LOWERING-NEXT:     let mut complex_total: i32 = 0;
// LOWERING-NEXT:     let mut limits_total: i32 = 0;
// LOWERING-NEXT:     let mut ref_tmp0: C11Temporary = C11Temporary { values: [0; 3] };
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: u16 = 937;
// LOWERING-NEXT:     utf16_character = _v1;
// LOWERING-NEXT:     let _v2: u32 = 128578;
// LOWERING-NEXT:     utf32_character = _v2;
// LOWERING-NEXT:     anonymous = C11Anonymous { __slate_anon_0: anon_0 { integer: 0 }, __slate_anon_1: anon_1 { x: 0, y: 0 } };
// LOWERING-NEXT:     *aligned_object = C11OverAligned { value: 0 };
// LOWERING-NEXT:     utf16_state = __mbstate_t { __count: 0, __value: anon_2 { __wch: 0 } };
// LOWERING-NEXT:     utf32_state = __mbstate_t { __count: 0, __value: anon_2 { __wch: 0 } };
// LOWERING-NEXT:     let _v3: u16 = 0;
// LOWERING-NEXT:     converted16 = _v3;
// LOWERING-NEXT:     let _v4: u32 = 0;
// LOWERING-NEXT:     converted32 = _v4;
// LOWERING-NEXT:     current_time = libc::timespec { tv_sec: 0, tv_nsec: 0 };
// LOWERING-NEXT:     let _v5: i32 = 0;
// LOWERING-NEXT:     once_control = _v5;
// LOWERING-NEXT:     let _v6: i32 = 7;
// LOWERING-NEXT:     thread_increment = _v6;
// LOWERING-NEXT:     let _v7: i32 = 0;
// LOWERING-NEXT:     thread_result = _v7;
// LOWERING-NEXT:     let _v8: i32 = 4;
// LOWERING-NEXT:     let _v9: i32 = std::mem::align_of::<C11OverAligned>() as i32;
// LOWERING-NEXT:     let _v10: i32 = _v8 + _v9;
// LOWERING-NEXT:     let _v11: *mut u8 = std::ptr::addr_of_mut!(c11_aligned_buffer).cast::<u8>();
// LOWERING-NEXT:     let _v12: u64 = _v11 as u64;
// LOWERING-NEXT:     let _v13: u64 = 64;
// LOWERING-NEXT:     let _v14: u64 = _v12 % _v13;
// LOWERING-NEXT:     let _v15: u64 = 0;
// LOWERING-NEXT:     let _v16: bool = _v14 == _v15;
// LOWERING-NEXT:     let _v17: i32 = _v16 as i32;
// LOWERING-NEXT:     let _v18: i32 = _v10 + _v17;
// LOWERING-NEXT:     let _v19: u64 = std::ptr::addr_of_mut!(*aligned_object) as u64;
// LOWERING-NEXT:     let _v20: u64 = 32;
// LOWERING-NEXT:     let _v21: u64 = _v19 % _v20;
// LOWERING-NEXT:     let _v22: u64 = 0;
// LOWERING-NEXT:     let _v23: bool = _v21 == _v22;
// LOWERING-NEXT:     let _v24: i32 = _v23 as i32;
// LOWERING-NEXT:     let _v25: i32 = _v18 + _v24;
// LOWERING-NEXT:     alignment_total = _v25;
// LOWERING-NEXT:     let _v26: i64 = 0;
// LOWERING-NEXT:     let _v27: i8 = unsafe { main_utf8_text[(_v26 as usize)] };
// LOWERING-NEXT:     let _v28: u8 = _v27 as u8;
// LOWERING-NEXT:     let _v29: i32 = _v28 as i32;
// LOWERING-NEXT:     let _v30: i64 = 1;
// LOWERING-NEXT:     let _v31: i8 = unsafe { main_utf8_text[(_v30 as usize)] };
// LOWERING-NEXT:     let _v32: u8 = _v31 as u8;
// LOWERING-NEXT:     let _v33: i32 = _v32 as i32;
// LOWERING-NEXT:     let _v34: i32 = _v29 + _v33;
// LOWERING-NEXT:     let _v35: i64 = 0;
// LOWERING-NEXT:     let _v36: u16 = unsafe { main_utf16_text[(_v35 as usize)] };
// LOWERING-NEXT:     let _v37: i32 = _v36 as i32;
// LOWERING-NEXT:     let _v38: i32 = _v34 + _v37;
// LOWERING-NEXT:     let _v39: i64 = 0;
// LOWERING-NEXT:     let _v40: u32 = unsafe { main_utf32_text[(_v39 as usize)] };
// LOWERING-NEXT:     let _v41: i32 = _v40 as i32;
// LOWERING-NEXT:     let _v42: i32 = _v38 + _v41;
// LOWERING-NEXT:     let _v43: u16 = utf16_character;
// LOWERING-NEXT:     let _v44: i32 = _v43 as i32;
// LOWERING-NEXT:     let _v45: i32 = _v42 + _v44;
// LOWERING-NEXT:     let _v46: u32 = utf32_character;
// LOWERING-NEXT:     let _v47: i32 = _v46 as i32;
// LOWERING-NEXT:     let _v48: i32 = _v45 + _v47;
// LOWERING-NEXT:     unicode_total = _v48;
// LOWERING-NEXT:     let _v49: i32 = 11;
// LOWERING-NEXT:     let _v50: i32 = 22;
// LOWERING-NEXT:     let _v51: i32 = _v49 + _v50;
// LOWERING-NEXT:     let _v52: i32 = 33;
// LOWERING-NEXT:     let _v53: i32 = _v51 + _v52;
// LOWERING-NEXT:     generic_total = _v53;
// LOWERING-NEXT:     let _v54: i32 = 31;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         anonymous.__slate_anon_0.integer = _v54;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v55: i32 = 37;
// LOWERING-NEXT:     anonymous.__slate_anon_1.x = _v55;
// LOWERING-NEXT:     let _v56: i32 = 41;
// LOWERING-NEXT:     anonymous.__slate_anon_1.y = _v56;
// LOWERING-NEXT:     let _v57: i32 = unsafe { anonymous.__slate_anon_0.integer };
// LOWERING-NEXT:     let _v58: i32 = anonymous.__slate_anon_1.x;
// LOWERING-NEXT:     let _v59: i32 = _v57 + _v58;
// LOWERING-NEXT:     let _v60: i32 = anonymous.__slate_anon_1.y;
// LOWERING-NEXT:     let _v61: i32 = _v59 + _v60;
// LOWERING-NEXT:     anonymous_total = _v61;
// LOWERING-NEXT:     let _v62: i32 = 0;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         c11_evaluation_total = _v62;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v63: i32 = 2;
// LOWERING-NEXT:     let _v64: i32 = c11_evaluation_step(_v63);
// LOWERING-NEXT:     let _v65: i32 = 3;
// LOWERING-NEXT:     let _v66: i32 = c11_evaluation_step(_v65);
// LOWERING-NEXT:     let _v67: i32 = _v64 + _v66;
// LOWERING-NEXT:     let _v68: i32 = unsafe { c11_evaluation_total };
// LOWERING-NEXT:     let _v69: i32 = _v67 + _v68;
// LOWERING-NEXT:     evaluation_total = _v69;
// LOWERING-NEXT:     let _v70: i64 = 1;
// LOWERING-NEXT:     let _v71: i32 = 43;
// LOWERING-NEXT:     let _v72: anon_struct = c11_make_temporary(_v71);
// LOWERING-NEXT:     coerce = _v72;
// LOWERING-NEXT:     let _v73: *mut C11Temporary = std::ptr::addr_of_mut!(coerce) as *mut C11Temporary;
// LOWERING-NEXT:     let _v74: C11Temporary = unsafe { *_v73 };
// LOWERING-NEXT:     ref_tmp0 = _v74;
// LOWERING-NEXT:     let _v75: i32 = ref_tmp0.values[(_v70 as usize)];
// LOWERING-NEXT:     temporary_total = _v75;
// LOWERING-NEXT:     let _v76: i32 = 1;
// LOWERING-NEXT:     static_assert_total = _v76;
// LOWERING-NEXT:     let _v77: i32 = 0;
// LOWERING-NEXT:     let _v78: i32 = 0;
// LOWERING-NEXT:     let _v79: i32 = _v77 + _v78;
// LOWERING-NEXT:     let _v80: i32 = 1;
// LOWERING-NEXT:     let _v81: i32 = _v79 + _v80;
// LOWERING-NEXT:     let _v82: i32 = 1;
// LOWERING-NEXT:     let _v83: i32 = _v81 + _v82;
// LOWERING-NEXT:     let _v84: i32 = 1;
// LOWERING-NEXT:     let _v85: i32 = _v83 + _v84;
// LOWERING-NEXT:     let _v86: i32 = 1;
// LOWERING-NEXT:     let _v87: i32 = _v85 + _v86;
// LOWERING-NEXT:     optional_total = _v87;
// LOWERING-NEXT:     let _v88: i32 = 5;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         c11_atomic_total = _v88;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v89: i32 = 17;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         c11_thread_local_value = _v89;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v90: i32 = 0;
// LOWERING-NEXT:     let _v91: *mut i32 = unsafe { __errno_location() };
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *_v91 = _v90;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v92: *mut core::ffi::c_void = std::ptr::addr_of_mut!(thread_increment) as *mut core::ffi::c_void;
// LOWERING-NEXT:     let _v93: i32 = unsafe { thrd_create(std::ptr::addr_of_mut!(thread) as *mut u64, Some(c11_thread_worker), _v92 as *mut core::ffi::c_void) };
// LOWERING-NEXT:     thread_created = _v93;
// LOWERING-NEXT:     let _v94: i32 = thread_created;
// LOWERING-NEXT:     let _v95: i32 = 0;
// LOWERING-NEXT:     let _v96: bool = _v94 == _v95;
// LOWERING-NEXT:     let _v97: i32 = if _v96 {
// LOWERING-NEXT:         let _v98: u64 = thread;
// LOWERING-NEXT:         let _v99: i32 = unsafe { thrd_join(_v98 as u64, std::ptr::addr_of_mut!(thread_result) as *mut i32) };
// LOWERING-NEXT:         _v99
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let _v100: i32 = -1;
// LOWERING-NEXT:         _v100
// LOWERING-NEXT:     };
// LOWERING-NEXT:     thread_joined = _v97;
// LOWERING-NEXT:     let _v101: i32 = unsafe { std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(c11_atomic_total)).load(std::sync::atomic::Ordering::SeqCst) };
// LOWERING-NEXT:     atomic_total = _v101;
// LOWERING-NEXT:     let _v102: i32 = thread_created;
// LOWERING-NEXT:     let _v103: i32 = 0;
// LOWERING-NEXT:     let _v104: bool = _v102 == _v103;
// LOWERING-NEXT:     let _v105: i32 = _v104 as i32;
// LOWERING-NEXT:     let _v106: i32 = thread_joined;
// LOWERING-NEXT:     let _v107: i32 = 0;
// LOWERING-NEXT:     let _v108: bool = _v106 == _v107;
// LOWERING-NEXT:     let _v109: i32 = _v108 as i32;
// LOWERING-NEXT:     let _v110: i32 = _v105 + _v109;
// LOWERING-NEXT:     let _v111: i32 = thread_result;
// LOWERING-NEXT:     let _v112: i32 = _v110 + _v111;
// LOWERING-NEXT:     let _v113: i32 = unsafe { c11_thread_local_value };
// LOWERING-NEXT:     let _v114: i32 = _v112 + _v113;
// LOWERING-NEXT:     let _v115: *mut i32 = unsafe { __errno_location() };
// LOWERING-NEXT:     let _v116: i32 = unsafe { *_v115 };
// LOWERING-NEXT:     let _v117: i32 = 0;
// LOWERING-NEXT:     let _v118: bool = _v116 == _v117;
// LOWERING-NEXT:     let _v119: i32 = _v118 as i32;
// LOWERING-NEXT:     let _v120: i32 = _v114 + _v119;
// LOWERING-NEXT:     thread_total = _v120;
// LOWERING-NEXT:     let _v121: i32 = 0;
// LOWERING-NEXT:     concurrency_total = _v121;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let _v122: i32 = 0;
// LOWERING-NEXT:         let _v123: i32 = unsafe { mtx_init(std::ptr::addr_of_mut!(mutex) as *mut mtx_t, _v122 as i32) };
// LOWERING-NEXT:         let _v124: i32 = 0;
// LOWERING-NEXT:         let _v125: bool = _v123 == _v124;
// LOWERING-NEXT:         if _v125 {
// LOWERING-NEXT:             let _v126: i32 = 1;
// LOWERING-NEXT:             let _v127: i32 = concurrency_total;
// LOWERING-NEXT:             let _v128: i32 = _v127 + _v126;
// LOWERING-NEXT:             concurrency_total = _v128;
// LOWERING-NEXT:             let _v129: i32 = unsafe { mtx_lock(std::ptr::addr_of_mut!(mutex) as *mut mtx_t) };
// LOWERING-NEXT:             let _v130: i32 = 0;
// LOWERING-NEXT:             let _v131: bool = _v129 == _v130;
// LOWERING-NEXT:             let _v132: i32 = _v131 as i32;
// LOWERING-NEXT:             let _v133: i32 = concurrency_total;
// LOWERING-NEXT:             let _v134: i32 = _v133 + _v132;
// LOWERING-NEXT:             concurrency_total = _v134;
// LOWERING-NEXT:             let _v135: i32 = unsafe { mtx_unlock(std::ptr::addr_of_mut!(mutex) as *mut mtx_t) };
// LOWERING-NEXT:             let _v136: i32 = 0;
// LOWERING-NEXT:             let _v137: bool = _v135 == _v136;
// LOWERING-NEXT:             let _v138: i32 = _v137 as i32;
// LOWERING-NEXT:             let _v139: i32 = concurrency_total;
// LOWERING-NEXT:             let _v140: i32 = _v139 + _v138;
// LOWERING-NEXT:             concurrency_total = _v140;
// LOWERING-NEXT:             unsafe { mtx_destroy(std::ptr::addr_of_mut!(mutex) as *mut mtx_t) };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let _v141: i32 = unsafe { cnd_init(std::ptr::addr_of_mut!(condition) as *mut cnd_t) };
// LOWERING-NEXT:         let _v142: i32 = 0;
// LOWERING-NEXT:         let _v143: bool = _v141 == _v142;
// LOWERING-NEXT:         if _v143 {
// LOWERING-NEXT:             let _v144: i32 = 1;
// LOWERING-NEXT:             let _v145: i32 = concurrency_total;
// LOWERING-NEXT:             let _v146: i32 = _v145 + _v144;
// LOWERING-NEXT:             concurrency_total = _v146;
// LOWERING-NEXT:             unsafe { cnd_destroy(std::ptr::addr_of_mut!(condition) as *mut cnd_t) };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v147: i32 = 0;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         c11_once_total = _v147;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     unsafe { call_once(std::ptr::addr_of_mut!(once_control) as *mut i32, Some(c11_once_handler)) };
// LOWERING-NEXT:     unsafe { call_once(std::ptr::addr_of_mut!(once_control) as *mut i32, Some(c11_once_handler)) };
// LOWERING-NEXT:     let _v148: i32 = unsafe { c11_once_total };
// LOWERING-NEXT:     let _v149: i32 = concurrency_total;
// LOWERING-NEXT:     let _v150: i32 = _v149 + _v148;
// LOWERING-NEXT:     concurrency_total = _v150;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let _v151: Option<unsafe extern "C" fn(*mut core::ffi::c_void)> = None;
// LOWERING-NEXT:         let _v152: i32 = unsafe { tss_create(std::ptr::addr_of_mut!(thread_key) as *mut u32, _v151) };
// LOWERING-NEXT:         let _v153: i32 = 0;
// LOWERING-NEXT:         let _v154: bool = _v152 == _v153;
// LOWERING-NEXT:         if _v154 {
// LOWERING-NEXT:             let _v155: i32 = 1;
// LOWERING-NEXT:             let _v156: i32 = concurrency_total;
// LOWERING-NEXT:             let _v157: i32 = _v156 + _v155;
// LOWERING-NEXT:             concurrency_total = _v157;
// LOWERING-NEXT:             let _v158: u32 = thread_key;
// LOWERING-NEXT:             let _v159: *mut core::ffi::c_void = std::ptr::addr_of_mut!(thread_increment) as *mut core::ffi::c_void;
// LOWERING-NEXT:             let _v160: i32 = unsafe { tss_set(_v158 as u32, _v159 as *mut core::ffi::c_void) };
// LOWERING-NEXT:             let _v161: i32 = 0;
// LOWERING-NEXT:             let _v162: bool = _v160 == _v161;
// LOWERING-NEXT:             let _v163: i32 = _v162 as i32;
// LOWERING-NEXT:             let _v164: i32 = concurrency_total;
// LOWERING-NEXT:             let _v165: i32 = _v164 + _v163;
// LOWERING-NEXT:             concurrency_total = _v165;
// LOWERING-NEXT:             let _v166: u32 = thread_key;
// LOWERING-NEXT:             let _v167: *mut core::ffi::c_void = unsafe { tss_get(_v166 as u32) };
// LOWERING-NEXT:             let _v168: *mut core::ffi::c_void = std::ptr::addr_of_mut!(thread_increment) as *mut core::ffi::c_void;
// LOWERING-NEXT:             let _v169: bool = _v167 == _v168;
// LOWERING-NEXT:             let _v170: i32 = _v169 as i32;
// LOWERING-NEXT:             let _v171: i32 = concurrency_total;
// LOWERING-NEXT:             let _v172: i32 = _v171 + _v170;
// LOWERING-NEXT:             concurrency_total = _v172;
// LOWERING-NEXT:             let _v173: u32 = thread_key;
// LOWERING-NEXT:             unsafe { tss_delete(_v173 as u32) };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v174: i32 = 4;
// LOWERING-NEXT:     let _v175: i32 = 1;
// LOWERING-NEXT:     let _v176: bool = _v174 >= _v175;
// LOWERING-NEXT:     let _v177: i32 = _v176 as i32;
// LOWERING-NEXT:     let _v178: i32 = concurrency_total;
// LOWERING-NEXT:     let _v179: i32 = _v178 + _v177;
// LOWERING-NEXT:     concurrency_total = _v179;
// LOWERING-NEXT:     let _v180: u16 = 0;
// LOWERING-NEXT:     converted16 = _v180;
// LOWERING-NEXT:     let _v181: u32 = 0;
// LOWERING-NEXT:     converted32 = _v181;
// LOWERING-NEXT:     let _v182: *mut i8 = b"A\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v183: u64 = 1;
// LOWERING-NEXT:     let _v184: u64 = (unsafe { mbrtoc16(std::ptr::addr_of_mut!(converted16) as *mut u16, _v182 as *const i8, _v183 as usize, std::ptr::addr_of_mut!(utf16_state) as *mut __mbstate_t) }) as u64;
// LOWERING-NEXT:     let _v185: i32 = _v184 as i32;
// LOWERING-NEXT:     let _v186: *mut i8 = multibyte16.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let _v187: u16 = 65;
// LOWERING-NEXT:     let _v188: u64 = (unsafe { c16rtomb(_v186 as *mut i8, _v187 as u16, std::ptr::addr_of_mut!(utf16_state) as *mut __mbstate_t) }) as u64;
// LOWERING-NEXT:     let _v189: i32 = _v188 as i32;
// LOWERING-NEXT:     let _v190: i32 = _v185 + _v189;
// LOWERING-NEXT:     let _v191: *mut i8 = b"B\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v192: u64 = 1;
// LOWERING-NEXT:     let _v193: u64 = (unsafe { mbrtoc32(std::ptr::addr_of_mut!(converted32) as *mut u32, _v191 as *const i8, _v192 as usize, std::ptr::addr_of_mut!(utf32_state) as *mut __mbstate_t) }) as u64;
// LOWERING-NEXT:     let _v194: i32 = _v193 as i32;
// LOWERING-NEXT:     let _v195: i32 = _v190 + _v194;
// LOWERING-NEXT:     let _v196: *mut i8 = multibyte32.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let _v197: u32 = 66;
// LOWERING-NEXT:     let _v198: u64 = (unsafe { c32rtomb(_v196 as *mut i8, _v197 as u32, std::ptr::addr_of_mut!(utf32_state) as *mut __mbstate_t) }) as u64;
// LOWERING-NEXT:     let _v199: i32 = _v198 as i32;
// LOWERING-NEXT:     let _v200: i32 = _v195 + _v199;
// LOWERING-NEXT:     let _v201: u16 = converted16;
// LOWERING-NEXT:     let _v202: i32 = _v201 as i32;
// LOWERING-NEXT:     let _v203: i32 = _v200 + _v202;
// LOWERING-NEXT:     let _v204: u32 = converted32;
// LOWERING-NEXT:     let _v205: i32 = _v204 as i32;
// LOWERING-NEXT:     let _v206: i32 = _v203 + _v205;
// LOWERING-NEXT:     let _v207: i64 = 0;
// LOWERING-NEXT:     let _v208: i8 = multibyte16[(_v207 as usize)];
// LOWERING-NEXT:     let _v209: i32 = _v208 as i32;
// LOWERING-NEXT:     let _v210: i32 = _v206 + _v209;
// LOWERING-NEXT:     let _v211: i64 = 0;
// LOWERING-NEXT:     let _v212: i8 = multibyte32[(_v211 as usize)];
// LOWERING-NEXT:     let _v213: i32 = _v212 as i32;
// LOWERING-NEXT:     let _v214: i32 = _v210 + _v213;
// LOWERING-NEXT:     conversion_total = _v214;
// LOWERING-NEXT:     let _v215: u64 = 64;
// LOWERING-NEXT:     let _v216: u64 = 64;
// LOWERING-NEXT:     let _v217: *mut core::ffi::c_void = unsafe { aligned_alloc(_v215 as usize, _v216 as usize) };
// LOWERING-NEXT:     aligned_memory = _v217;
// LOWERING-NEXT:     let _v218: *mut core::ffi::c_void = aligned_memory;
// LOWERING-NEXT:     let _v219: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let _v220: bool = _v218 != _v219;
// LOWERING-NEXT:     let _v221: bool = if _v220 {
// LOWERING-NEXT:         let _v222: *mut core::ffi::c_void = aligned_memory;
// LOWERING-NEXT:         let _v223: u64 = _v222 as u64;
// LOWERING-NEXT:         let _v224: u64 = 64;
// LOWERING-NEXT:         let _v225: u64 = _v223 % _v224;
// LOWERING-NEXT:         let _v226: u64 = 0;
// LOWERING-NEXT:         let _v227: bool = _v225 == _v226;
// LOWERING-NEXT:         _v227
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let _v228: bool = false;
// LOWERING-NEXT:         _v228
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let _v229: i32 = _v221 as i32;
// LOWERING-NEXT:     let _v230: i32 = alignment_total;
// LOWERING-NEXT:     let _v231: i32 = _v230 + _v229;
// LOWERING-NEXT:     alignment_total = _v231;
// LOWERING-NEXT:     let _v232: *mut core::ffi::c_void = aligned_memory;
// LOWERING-NEXT:     unsafe { free(_v232 as *mut core::ffi::c_void) };
// LOWERING-NEXT:     let _v233: i32 = unsafe { at_quick_exit(Some(c11_quick_handler)) };
// LOWERING-NEXT:     let _v234: i32 = 0;
// LOWERING-NEXT:     let _v235: bool = _v233 == _v234;
// LOWERING-NEXT:     let _v236: i32 = _v235 as i32;
// LOWERING-NEXT:     quick_total = _v236;
// LOWERING-NEXT:     let _v237: *mut i8 = b"slate-c11-exclusive.tmp\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v238: i32 = unsafe { remove(_v237 as *const i8) };
// LOWERING-NEXT:     let _v239: *mut i8 = b"slate-c11-exclusive.tmp\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v240: *mut libc::FILE = c11_open_exclusive(_v239);
// LOWERING-NEXT:     exclusive_first = _v240;
// LOWERING-NEXT:     let _v241: *mut i8 = b"slate-c11-exclusive.tmp\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v242: *mut libc::FILE = c11_open_exclusive(_v241);
// LOWERING-NEXT:     exclusive_second = _v242;
// LOWERING-NEXT:     let _v243: *mut libc::FILE = exclusive_first;
// LOWERING-NEXT:     let _v244: *mut libc::FILE = std::ptr::null_mut();
// LOWERING-NEXT:     let _v245: bool = _v243 != _v244;
// LOWERING-NEXT:     let _v246: bool = if _v245 {
// LOWERING-NEXT:         let _v247: *mut libc::FILE = exclusive_second;
// LOWERING-NEXT:         let _v248: *mut libc::FILE = std::ptr::null_mut();
// LOWERING-NEXT:         let _v249: bool = _v247 == _v248;
// LOWERING-NEXT:         _v249
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let _v250: bool = false;
// LOWERING-NEXT:         _v250
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let _v251: i32 = _v246 as i32;
// LOWERING-NEXT:     exclusive_total = _v251;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let _v252: *mut libc::FILE = exclusive_first;
// LOWERING-NEXT:         let _v253: *mut libc::FILE = std::ptr::null_mut();
// LOWERING-NEXT:         let _v254: bool = _v252 != _v253;
// LOWERING-NEXT:         if _v254 {
// LOWERING-NEXT:             let _v255: *mut libc::FILE = exclusive_first;
// LOWERING-NEXT:             let _v256: i32 = unsafe { fclose(_v255 as *mut libc::FILE) };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let _v257: *mut libc::FILE = exclusive_second;
// LOWERING-NEXT:         let _v258: *mut libc::FILE = std::ptr::null_mut();
// LOWERING-NEXT:         let _v259: bool = _v257 != _v258;
// LOWERING-NEXT:         if _v259 {
// LOWERING-NEXT:             let _v260: *mut libc::FILE = exclusive_second;
// LOWERING-NEXT:             let _v261: i32 = unsafe { fclose(_v260 as *mut libc::FILE) };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v262: *mut i8 = b"slate-c11-exclusive.tmp\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v263: i32 = unsafe { remove(_v262 as *const i8) };
// LOWERING-NEXT:     let _v264: i32 = 1;
// LOWERING-NEXT:     let _v265: i32 = unsafe { timespec_get(std::ptr::addr_of_mut!(current_time) as *mut libc::timespec, _v264 as i32) };
// LOWERING-NEXT:     let _v266: i32 = 1;
// LOWERING-NEXT:     let _v267: bool = _v265 == _v266;
// LOWERING-NEXT:     let _v268: bool = if _v267 {
// LOWERING-NEXT:         let _v269: i64 = current_time.tv_nsec;
// LOWERING-NEXT:         let _v270: i64 = 0;
// LOWERING-NEXT:         let _v271: bool = _v269 >= _v270;
// LOWERING-NEXT:         _v271
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let _v272: bool = false;
// LOWERING-NEXT:         _v272
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let _v273: bool = if _v268 {
// LOWERING-NEXT:         let _v274: i64 = current_time.tv_nsec;
// LOWERING-NEXT:         let _v275: i64 = 1000000000;
// LOWERING-NEXT:         let _v276: bool = _v274 < _v275;
// LOWERING-NEXT:         _v276
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let _v277: bool = false;
// LOWERING-NEXT:         _v277
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let _v278: i32 = _v273 as i32;
// LOWERING-NEXT:     timespec_total = _v278;
// LOWERING-NEXT:     let _v279: num_complex::Complex<f64> = num_complex::Complex { re: 2.0, im: 3.0 };
// LOWERING-NEXT:     complex_value = _v279;
// LOWERING-NEXT:     let _v280: num_complex::Complex<f64> = complex_value;
// LOWERING-NEXT:     let _v281: f64 = _v280.re;
// LOWERING-NEXT:     let _v282: f64 = 2.0;
// LOWERING-NEXT:     let _v283: bool = _v281 == _v282;
// LOWERING-NEXT:     let _v284: i32 = _v283 as i32;
// LOWERING-NEXT:     let _v285: num_complex::Complex<f64> = complex_value;
// LOWERING-NEXT:     let _v286: f64 = _v285.im;
// LOWERING-NEXT:     let _v287: f64 = 3.0;
// LOWERING-NEXT:     let _v288: bool = _v286 == _v287;
// LOWERING-NEXT:     let _v289: i32 = _v288 as i32;
// LOWERING-NEXT:     let _v290: i32 = _v284 + _v289;
// LOWERING-NEXT:     complex_total = _v290;
// LOWERING-NEXT:     let _v291: i32 = 9 as i32;
// LOWERING-NEXT:     let _v292: i32 = 6;
// LOWERING-NEXT:     let _v293: bool = _v291 >= _v292;
// LOWERING-NEXT:     let _v294: i32 = _v293 as i32;
// LOWERING-NEXT:     let _v295: i32 = 17 as i32;
// LOWERING-NEXT:     let _v296: i32 = 10;
// LOWERING-NEXT:     let _v297: bool = _v295 >= _v296;
// LOWERING-NEXT:     let _v298: i32 = _v297 as i32;
// LOWERING-NEXT:     let _v299: i32 = _v294 + _v298;
// LOWERING-NEXT:     let _v300: i32 = 21 as i32;
// LOWERING-NEXT:     let _v301: i32 = 10;
// LOWERING-NEXT:     let _v302: bool = _v300 >= _v301;
// LOWERING-NEXT:     let _v303: i32 = _v302 as i32;
// LOWERING-NEXT:     let _v304: i32 = _v299 + _v303;
// LOWERING-NEXT:     let _v305: f32 = 0.0000000000000000000000000000000000000000000014013;
// LOWERING-NEXT:     let _v306: f32 = 0.0;
// LOWERING-NEXT:     let _v307: bool = _v305 > _v306;
// LOWERING-NEXT:     let _v308: i32 = _v307 as i32;
// LOWERING-NEXT:     let _v309: i32 = _v304 + _v308;
// LOWERING-NEXT:     let _v310: f64 = 0.000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000005;
// LOWERING-NEXT:     let _v311: f64 = 0.0;
// LOWERING-NEXT:     let _v312: bool = _v310 > _v311;
// LOWERING-NEXT:     let _v313: i32 = _v312 as i32;
// LOWERING-NEXT:     let _v314: i32 = _v309 + _v313;
// LOWERING-NEXT:     let _v315: LongDouble = LongDouble([1, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// LOWERING-NEXT:     let _v316: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// LOWERING-NEXT:     let _v317: bool = _v315 > _v316;
// LOWERING-NEXT:     let _v318: i32 = _v317 as i32;
// LOWERING-NEXT:     let _v319: i32 = _v314 + _v318;
// LOWERING-NEXT:     let _v320: i32 = 1;
// LOWERING-NEXT:     let _v321: i32 = -1;
// LOWERING-NEXT:     let _v322: bool = _v320 >= _v321;
// LOWERING-NEXT:     let _v323: i32 = _v322 as i32;
// LOWERING-NEXT:     let _v324: i32 = _v319 + _v323;
// LOWERING-NEXT:     let _v325: i32 = 1;
// LOWERING-NEXT:     let _v326: i32 = -1;
// LOWERING-NEXT:     let _v327: bool = _v325 >= _v326;
// LOWERING-NEXT:     let _v328: i32 = _v327 as i32;
// LOWERING-NEXT:     let _v329: i32 = _v324 + _v328;
// LOWERING-NEXT:     let _v330: i32 = 1;
// LOWERING-NEXT:     let _v331: i32 = -1;
// LOWERING-NEXT:     let _v332: bool = _v330 >= _v331;
// LOWERING-NEXT:     let _v333: i32 = _v332 as i32;
// LOWERING-NEXT:     let _v334: i32 = _v329 + _v333;
// LOWERING-NEXT:     limits_total = _v334;
// LOWERING-NEXT:     let _v335: i32 = 0;
// LOWERING-NEXT:     let _v336: i32 = limits_total;
// LOWERING-NEXT:     let _v337: i32 = _v336 + _v335;
// LOWERING-NEXT:     limits_total = _v337;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let _v338: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(c11_never_flag)) };
// LOWERING-NEXT:         let _v339: bool = _v338 != 0;
// LOWERING-NEXT:         if _v339 {
// LOWERING-NEXT:             let _v340: i32 = 99;
// LOWERING-NEXT:             c11_never_return(_v340);
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v341: *mut i8 = b"%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v342: i32 = alignment_total;
// LOWERING-NEXT:     let _v343: i32 = unicode_total;
// LOWERING-NEXT:     let _v344: i32 = generic_total;
// LOWERING-NEXT:     let _v345: i32 = anonymous_total;
// LOWERING-NEXT:     let _v346: i32 = evaluation_total;
// LOWERING-NEXT:     let _v347: i32 = temporary_total;
// LOWERING-NEXT:     let _v348: i32 = static_assert_total;
// LOWERING-NEXT:     let _v349: i32 = optional_total;
// LOWERING-NEXT:     let _v350: i32 = atomic_total;
// LOWERING-NEXT:     let _v351: i32 = thread_total;
// LOWERING-NEXT:     let _v352: i32 = concurrency_total;
// LOWERING-NEXT:     let _v353: i32 = conversion_total;
// LOWERING-NEXT:     let _v354: i32 = quick_total;
// LOWERING-NEXT:     let _v355: i32 = exclusive_total;
// LOWERING-NEXT:     let _v356: i32 = timespec_total;
// LOWERING-NEXT:     let _v357: i32 = complex_total;
// LOWERING-NEXT:     let _v358: i32 = limits_total;
// LOWERING-NEXT:     let _v359: i32 = 0;
// LOWERING-NEXT:     let _v360: i32 = unsafe { printf(_v341 as *const i8, _v342, _v343, _v344, _v345, _v346, _v347, _v348, _v349, _v350, _v351, _v352, _v353, _v354, _v355, _v356, _v357, _v358, _v359) };
// LOWERING-NEXT:     let _v361: i32 = 10;
// LOWERING-NEXT:     let _v362: i32 = unsafe { putchar(_v361 as i32) };
// LOWERING-NEXT:     let _v363: i32 = 0;
// LOWERING-NEXT:     __retval = _v363;
// LOWERING-NEXT:     let _v364: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v364 as i32);
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
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(thread_local)]
// REWRITES-NEXT: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn __muldc3(a: f64, b: f64, c: f64, d: f64) -> num_complex::Complex<f64>;
// REWRITES-NEXT:     fn __divdc3(a: f64, b: f64, c: f64, d: f64) -> num_complex::Complex<f64>;
// REWRITES-NEXT:     fn __mulsc3(a: f32, b: f32, c: f32, d: f32) -> num_complex::Complex<f32>;
// REWRITES-NEXT:     fn __divsc3(a: f32, b: f32, c: f32, d: f32) -> num_complex::Complex<f32>;
// REWRITES-NEXT: }
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
// REWRITES-NEXT: struct C11Anonymous {
// REWRITES-NEXT:     __slate_anon_0: anon_0,
// REWRITES-NEXT:     __slate_anon_1: anon_1,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C, align(32))]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct C11OverAligned {
// REWRITES-NEXT:     value: u8,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct C11Temporary {
// REWRITES-NEXT:     values: [i32; 3],
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct __mbstate_t {
// REWRITES-NEXT:     __count: i32,
// REWRITES-NEXT:     __value: anon_2,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: union anon_0 {
// REWRITES-NEXT:     integer: i32,
// REWRITES-NEXT:     __slate_anon_1: f64,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct anon_1 {
// REWRITES-NEXT:     x: i32,
// REWRITES-NEXT:     y: i32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: union anon_2 {
// REWRITES-NEXT:     __wch: i32,
// REWRITES-NEXT:     __wchb: [i8; 4],
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct anon_struct {
// REWRITES-NEXT:     __slate_anon_0: u64,
// REWRITES-NEXT:     __slate_anon_1: i32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: union cnd_t {
// REWRITES-NEXT:     __size: [i8; 48],
// REWRITES-NEXT:     __align: i64,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: union mtx_t {
// REWRITES-NEXT:     __size: [i8; 40],
// REWRITES-NEXT:     __align: i64,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut c11_aligned_buffer: aligned::Aligned<aligned::A64, [u8; 64]> = aligned::Aligned([0; 64]);
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut c11_atomic_total: i32 = 0;
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut c11_evaluation_total: i32 = 0;
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut c11_never_flag: i32 = 0;
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut c11_once_total: i32 = 0;
// REWRITES-EMPTY:
// REWRITES-NEXT: #[thread_local]
// REWRITES-NEXT: static mut c11_thread_local_value: i32 = 0;
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut main_utf16_text: [u16; 2] = [937, 0];
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut main_utf32_text: [u32; 2] = [128578, 0];
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut main_utf8_text: [i8; 3] = [-50, -87, 0];
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn __errno_location() -> *mut i32;
// REWRITES-NEXT:     fn thrd_create(_0: *mut u64, _1: Option<unsafe extern "C" fn(*mut core::ffi::c_void) -> i32>, _2: *mut core::ffi::c_void) -> i32;
// REWRITES-NEXT:     fn thrd_join(_0: u64, _1: *mut i32) -> i32;
// REWRITES-NEXT:     fn mtx_init(_0: *mut mtx_t, _1: i32) -> i32;
// REWRITES-NEXT:     fn mtx_lock(_0: *mut mtx_t) -> i32;
// REWRITES-NEXT:     fn mtx_unlock(_0: *mut mtx_t) -> i32;
// REWRITES-NEXT:     fn mtx_destroy(_0: *mut mtx_t);
// REWRITES-NEXT:     fn cnd_init(_0: *mut cnd_t) -> i32;
// REWRITES-NEXT:     fn cnd_destroy(_0: *mut cnd_t);
// REWRITES-NEXT:     fn call_once(_0: *mut i32, _1: Option<unsafe extern "C" fn()>);
// REWRITES-NEXT:     fn tss_create(_0: *mut u32, _1: Option<unsafe extern "C" fn(*mut core::ffi::c_void)>) -> i32;
// REWRITES-NEXT:     fn tss_set(_0: u32, _1: *mut core::ffi::c_void) -> i32;
// REWRITES-NEXT:     fn tss_get(_0: u32) -> *mut core::ffi::c_void;
// REWRITES-NEXT:     fn tss_delete(_0: u32);
// REWRITES-NEXT:     fn mbrtoc16(_0: *mut u16, _1: *const i8, _2: usize, _3: *mut __mbstate_t) -> usize;
// REWRITES-NEXT:     fn c16rtomb(_0: *mut i8, _1: u16, _2: *mut __mbstate_t) -> usize;
// REWRITES-NEXT:     fn mbrtoc32(_0: *mut u32, _1: *const i8, _2: usize, _3: *mut __mbstate_t) -> usize;
// REWRITES-NEXT:     fn c32rtomb(_0: *mut i8, _1: u32, _2: *mut __mbstate_t) -> usize;
// REWRITES-NEXT:     fn aligned_alloc(_0: usize, _1: usize) -> *mut core::ffi::c_void;
// REWRITES-NEXT:     fn free(_0: *mut core::ffi::c_void);
// REWRITES-NEXT:     fn at_quick_exit(_0: Option<unsafe extern "C" fn()>) -> i32;
// REWRITES-NEXT:     fn remove(_0: *const i8) -> i32;
// REWRITES-NEXT:     fn fopen(_0: *const i8, _1: *const i8) -> *mut libc::FILE;
// REWRITES-NEXT:     fn fclose(_0: *mut libc::FILE) -> i32;
// REWRITES-NEXT:     fn timespec_get(_0: *mut libc::timespec, _1: i32) -> i32;
// REWRITES-NEXT:     fn quick_exit(_0: i32) -> !;
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT:     fn putchar(_0: i32) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn c11_evaluation_step(arg4: i32) -> i32 {
// REWRITES-NEXT: let mut value: i32 = arg4;
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         c11_evaluation_total = (unsafe { c11_evaluation_total }) + value;
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v4: i32 = 2;
// REWRITES-NEXT: __retval = value * _v4;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn c11_make_temporary(arg3: i32) -> anon_struct {
// REWRITES-NEXT: let mut coerce: anon_struct = anon_struct { __slate_anon_0: 0, __slate_anon_1: 0 };
// REWRITES-NEXT: let mut base: i32 = arg3;
// REWRITES-NEXT: let mut __retval: C11Temporary = C11Temporary { values: [0; 3] };
// REWRITES-NEXT: let _v0: *mut i32 = std::ptr::addr_of_mut!(__retval.values) as *mut i32;
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         *_v0 = base;
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v2: i64 = 1;
// REWRITES-NEXT: let _v3: *mut i32 = unsafe { _v0.add(1) };
// REWRITES-NEXT: let _v5: i32 = 1;
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         *_v3 = base + _v5;
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v7: i64 = 2;
// REWRITES-NEXT: let _v8: *mut i32 = unsafe { _v0.add(2) };
// REWRITES-NEXT: let _v10: i32 = 2;
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         *_v8 = base + _v10;
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v12: C11Temporary = __retval;
// REWRITES-NEXT: let _v13: *mut C11Temporary = std::ptr::addr_of_mut!(coerce) as *mut C11Temporary;
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         *_v13 = _v12;
// REWRITES-NEXT: }
// REWRITES-NEXT: return coerce;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: extern "C" fn c11_thread_worker(arg2: *mut core::ffi::c_void) -> i32 {
// REWRITES-NEXT: let mut argument: *mut core::ffi::c_void = arg2;
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut increment: i32 = 0;
// REWRITES-NEXT: increment = unsafe { *(argument as *mut i32) };
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         c11_thread_local_value = 29;
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v4: i32 = 34;
// REWRITES-NEXT: let _v5: *mut i32 = unsafe { __errno_location() };
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         *_v5 = _v4;
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v7: i32 = unsafe { std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(c11_atomic_total)).fetch_add(increment, std::sync::atomic::Ordering::SeqCst) };
// REWRITES-NEXT: let _v8: i32 = unsafe { c11_thread_local_value };
// REWRITES-NEXT: let _v9: *mut i32 = unsafe { __errno_location() };
// REWRITES-NEXT: let _v11: i32 = 34;
// REWRITES-NEXT: __retval = _v8 + (((unsafe { *_v9 }) == _v11) as i32);
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: extern "C" fn c11_once_handler() {
// REWRITES-NEXT: let _v0: i32 = 1;
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         c11_once_total = (unsafe { c11_once_total }) + _v0;
// REWRITES-NEXT: }
// REWRITES-NEXT: return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: extern "C" fn c11_quick_handler() {
// REWRITES-NEXT: let _v0: i32 = 100;
// REWRITES-NEXT: let _v1: i32 = unsafe { std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(c11_atomic_total)).fetch_add(_v0, std::sync::atomic::Ordering::Relaxed) };
// REWRITES-NEXT: return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn c11_open_exclusive(arg1: *mut i8) -> *mut libc::FILE {
// REWRITES-NEXT: let mut path: *mut i8 = arg1;
// REWRITES-NEXT: let mut __retval: *mut libc::FILE = std::ptr::null_mut();
// REWRITES-NEXT: let _v0: *mut i8 = path;
// REWRITES-NEXT: let _v1: *mut i8 = b"wx\0".as_ptr() as *mut i8;
// REWRITES-NEXT: __retval = unsafe { fopen(_v0 as *const i8, _v1 as *const i8) };
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn c11_never_return(arg0: i32) -> ! {
// REWRITES-NEXT: let mut status: i32 = arg0;
// REWRITES-NEXT: unsafe { quick_exit(status as i32) }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut coerce: anon_struct = anon_struct { __slate_anon_0: 0, __slate_anon_1: 0 };
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut utf16_character: u16 = 0;
// REWRITES-NEXT: let mut utf32_character: u32 = 0;
// REWRITES-NEXT: let mut anonymous: C11Anonymous = C11Anonymous { __slate_anon_0: anon_0 { integer: 0 }, __slate_anon_1: anon_1 { x: 0, y: 0 } };
// REWRITES-NEXT: let mut aligned_object: aligned::Aligned<aligned::A32, C11OverAligned> = aligned::Aligned(C11OverAligned { value: 0 });
// REWRITES-NEXT: let mut utf16_state: __mbstate_t = __mbstate_t { __count: 0, __value: anon_2 { __wch: 0 } };
// REWRITES-NEXT: let mut utf32_state: __mbstate_t = __mbstate_t { __count: 0, __value: anon_2 { __wch: 0 } };
// REWRITES-NEXT: let mut converted16: u16 = 0;
// REWRITES-NEXT: let mut converted32: u32 = 0;
// REWRITES-NEXT: let mut multibyte16: [i8; 4] = [0; 4];
// REWRITES-NEXT: let mut multibyte32: [i8; 4] = [0; 4];
// REWRITES-NEXT: let mut current_time: libc::timespec = libc::timespec { tv_sec: 0, tv_nsec: 0 };
// REWRITES-NEXT: let mut thread: u64 = 0;
// REWRITES-NEXT: let mut mutex: mtx_t = mtx_t { __size: [0; 40] };
// REWRITES-NEXT: let mut condition: cnd_t = cnd_t { __size: [0; 48] };
// REWRITES-NEXT: let mut once_control: i32 = 0;
// REWRITES-NEXT: let mut thread_key: u32 = 0;
// REWRITES-NEXT: let mut thread_increment: i32 = 0;
// REWRITES-NEXT: let mut thread_result: i32 = 0;
// REWRITES-NEXT: let mut thread_created: i32 = 0;
// REWRITES-NEXT: let mut thread_joined: i32 = 0;
// REWRITES-NEXT: let mut aligned_memory: *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-NEXT: let mut exclusive_first: *mut libc::FILE = std::ptr::null_mut();
// REWRITES-NEXT: let mut exclusive_second: *mut libc::FILE = std::ptr::null_mut();
// REWRITES-NEXT: let mut complex_value: num_complex::Complex<f64> = num_complex::Complex { re: 0.0, im: 0.0 };
// REWRITES-NEXT: let mut alignment_total: i32 = 0;
// REWRITES-NEXT: let mut unicode_total: i32 = 0;
// REWRITES-NEXT: let mut generic_total: i32 = 0;
// REWRITES-NEXT: let mut anonymous_total: i32 = 0;
// REWRITES-NEXT: let mut evaluation_total: i32 = 0;
// REWRITES-NEXT: let mut temporary_total: i32 = 0;
// REWRITES-NEXT: let mut static_assert_total: i32 = 0;
// REWRITES-NEXT: let mut optional_total: i32 = 0;
// REWRITES-NEXT: let mut atomic_total: i32 = 0;
// REWRITES-NEXT: let mut thread_total: i32 = 0;
// REWRITES-NEXT: let mut concurrency_total: i32 = 0;
// REWRITES-NEXT: let mut conversion_total: i32 = 0;
// REWRITES-NEXT: let mut quick_total: i32 = 0;
// REWRITES-NEXT: let mut exclusive_total: i32 = 0;
// REWRITES-NEXT: let mut timespec_total: i32 = 0;
// REWRITES-NEXT: let mut complex_total: i32 = 0;
// REWRITES-NEXT: let mut limits_total: i32 = 0;
// REWRITES-NEXT: let mut ref_tmp0: C11Temporary = C11Temporary { values: [0; 3] };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: utf16_character = 937;
// REWRITES-NEXT: utf32_character = 128578;
// REWRITES-NEXT: anonymous = C11Anonymous { __slate_anon_0: anon_0 { integer: 0 }, __slate_anon_1: anon_1 { x: 0, y: 0 } };
// REWRITES-NEXT: *aligned_object = C11OverAligned { value: 0 };
// REWRITES-NEXT: utf16_state = __mbstate_t { __count: 0, __value: anon_2 { __wch: 0 } };
// REWRITES-NEXT: utf32_state = __mbstate_t { __count: 0, __value: anon_2 { __wch: 0 } };
// REWRITES-NEXT: converted16 = 0;
// REWRITES-NEXT: converted32 = 0;
// REWRITES-NEXT: current_time = libc::timespec { tv_sec: 0, tv_nsec: 0 };
// REWRITES-NEXT: once_control = 0;
// REWRITES-NEXT: thread_increment = 7;
// REWRITES-NEXT: thread_result = 0;
// REWRITES-NEXT: let _v8: i32 = 4;
// REWRITES-NEXT: let _v9: i32 = std::mem::align_of::<C11OverAligned>() as i32;
// REWRITES-NEXT: let _v10: i32 = _v8 + _v9;
// REWRITES-NEXT: let _v11: *mut u8 = std::ptr::addr_of_mut!(c11_aligned_buffer).cast::<u8>();
// REWRITES-NEXT: let _v13: u64 = 64;
// REWRITES-NEXT: let _v15: u64 = 0;
// REWRITES-NEXT: let _v18: i32 = _v10 + (((_v11 as u64) % _v13 == _v15) as i32);
// REWRITES-NEXT: let _v19: u64 = std::ptr::addr_of_mut!(*aligned_object) as u64;
// REWRITES-NEXT: let _v20: u64 = 32;
// REWRITES-NEXT: let _v22: u64 = 0;
// REWRITES-NEXT: alignment_total = _v18 + ((_v19 % _v20 == _v22) as i32);
// REWRITES-NEXT: let _v26: i64 = 0;
// REWRITES-NEXT: let _v30: i64 = 1;
// REWRITES-NEXT: let _v35: i64 = 0;
// REWRITES-NEXT: let _v39: i64 = 0;
// REWRITES-NEXT: unicode_total = (((unsafe { main_utf8_text[(_v26 as usize)] }) as u8) as i32) + (((unsafe { main_utf8_text[(_v30 as usize)] }) as u8) as i32) + ((unsafe { main_utf16_text[(_v35 as usize)] }) as i32) + ((unsafe { main_utf32_text[(_v39 as usize)] }) as i32) + (utf16_character as i32) + (utf32_character as i32);
// REWRITES-NEXT: let _v49: i32 = 11;
// REWRITES-NEXT: let _v50: i32 = 22;
// REWRITES-NEXT: let _v52: i32 = 33;
// REWRITES-NEXT: generic_total = _v49 + _v50 + _v52;
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         anonymous.__slate_anon_0.integer = 31;
// REWRITES-NEXT: }
// REWRITES-NEXT: anonymous.__slate_anon_1.x = 37;
// REWRITES-NEXT: anonymous.__slate_anon_1.y = 41;
// REWRITES-NEXT: anonymous_total = (unsafe { anonymous.__slate_anon_0.integer }) + anonymous.__slate_anon_1.x + anonymous.__slate_anon_1.y;
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         c11_evaluation_total = 0;
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v63: i32 = 2;
// REWRITES-NEXT: let _v64: i32 = c11_evaluation_step(_v63);
// REWRITES-NEXT: let _v65: i32 = 3;
// REWRITES-NEXT: let _v66: i32 = c11_evaluation_step(_v65);
// REWRITES-NEXT: evaluation_total = _v64 + _v66 + unsafe { c11_evaluation_total };
// REWRITES-NEXT: let _v70: i64 = 1;
// REWRITES-NEXT: let _v71: i32 = 43;
// REWRITES-NEXT: coerce = c11_make_temporary(_v71);
// REWRITES-NEXT: let _v73: *mut C11Temporary = std::ptr::addr_of_mut!(coerce) as *mut C11Temporary;
// REWRITES-NEXT: ref_tmp0 = unsafe { *_v73 };
// REWRITES-NEXT: temporary_total = ref_tmp0.values[(_v70 as usize)];
// REWRITES-NEXT: static_assert_total = 1;
// REWRITES-NEXT: let _v77: i32 = 0;
// REWRITES-NEXT: let _v78: i32 = 0;
// REWRITES-NEXT: let _v80: i32 = 1;
// REWRITES-NEXT: let _v82: i32 = 1;
// REWRITES-NEXT: let _v84: i32 = 1;
// REWRITES-NEXT: let _v86: i32 = 1;
// REWRITES-NEXT: optional_total = _v77 + _v78 + _v80 + _v82 + _v84 + _v86;
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         c11_atomic_total = 5;
// REWRITES-NEXT: }
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         c11_thread_local_value = 17;
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v90: i32 = 0;
// REWRITES-NEXT: let _v91: *mut i32 = unsafe { __errno_location() };
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         *_v91 = _v90;
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v92: *mut core::ffi::c_void = std::ptr::addr_of_mut!(thread_increment) as *mut core::ffi::c_void;
// REWRITES-NEXT: thread_created = unsafe { thrd_create(std::ptr::addr_of_mut!(thread) as *mut u64, Some(c11_thread_worker), _v92 as *mut core::ffi::c_void) };
// REWRITES-NEXT: let _v95: i32 = 0;
// REWRITES-NEXT: let _v96: bool = thread_created == _v95;
// REWRITES-NEXT: let _v97: i32 = if _v96 {
// REWRITES-NEXT:         let _v99: i32 = unsafe { thrd_join(thread as u64, std::ptr::addr_of_mut!(thread_result) as *mut i32) };
// REWRITES-NEXT:     _v99
// REWRITES-NEXT: } else {
// REWRITES-NEXT:         let _v100: i32 = -1;
// REWRITES-NEXT:     _v100
// REWRITES-NEXT: };
// REWRITES-NEXT: thread_joined = _v97;
// REWRITES-NEXT: let _v101: i32 = unsafe { std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(c11_atomic_total)).load(std::sync::atomic::Ordering::SeqCst) };
// REWRITES-NEXT: atomic_total = _v101;
// REWRITES-NEXT: let _v103: i32 = 0;
// REWRITES-NEXT: let _v107: i32 = 0;
// REWRITES-NEXT: let _v114: i32 = ((thread_created == _v103) as i32) + ((thread_joined == _v107) as i32) + thread_result + unsafe { c11_thread_local_value };
// REWRITES-NEXT: let _v115: *mut i32 = unsafe { __errno_location() };
// REWRITES-NEXT: let _v117: i32 = 0;
// REWRITES-NEXT: thread_total = _v114 + (((unsafe { *_v115 }) == _v117) as i32);
// REWRITES-NEXT: concurrency_total = 0;
// REWRITES-NEXT: {
// REWRITES-NEXT:         let _v122: i32 = 0;
// REWRITES-NEXT:         let _v123: i32 = unsafe { mtx_init(std::ptr::addr_of_mut!(mutex) as *mut mtx_t, _v122 as i32) };
// REWRITES-NEXT:         let _v124: i32 = 0;
// REWRITES-NEXT:         let _v125: bool = _v123 == _v124;
// REWRITES-NEXT:         if _v125 {
// REWRITES-NEXT:                     let _v126: i32 = 1;
// REWRITES-NEXT:                     concurrency_total = concurrency_total + _v126;
// REWRITES-NEXT:                     let _v129: i32 = unsafe { mtx_lock(std::ptr::addr_of_mut!(mutex) as *mut mtx_t) };
// REWRITES-NEXT:                     let _v130: i32 = 0;
// REWRITES-NEXT:                     concurrency_total = concurrency_total + ((_v129 == _v130) as i32);
// REWRITES-NEXT:                     let _v135: i32 = unsafe { mtx_unlock(std::ptr::addr_of_mut!(mutex) as *mut mtx_t) };
// REWRITES-NEXT:                     let _v136: i32 = 0;
// REWRITES-NEXT:                     concurrency_total = concurrency_total + ((_v135 == _v136) as i32);
// REWRITES-NEXT:                     unsafe { mtx_destroy(std::ptr::addr_of_mut!(mutex) as *mut mtx_t) };
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: {
// REWRITES-NEXT:         let _v141: i32 = unsafe { cnd_init(std::ptr::addr_of_mut!(condition) as *mut cnd_t) };
// REWRITES-NEXT:         let _v142: i32 = 0;
// REWRITES-NEXT:         let _v143: bool = _v141 == _v142;
// REWRITES-NEXT:         if _v143 {
// REWRITES-NEXT:                     let _v144: i32 = 1;
// REWRITES-NEXT:                     concurrency_total = concurrency_total + _v144;
// REWRITES-NEXT:                     unsafe { cnd_destroy(std::ptr::addr_of_mut!(condition) as *mut cnd_t) };
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         c11_once_total = 0;
// REWRITES-NEXT: }
// REWRITES-NEXT: unsafe { call_once(std::ptr::addr_of_mut!(once_control) as *mut i32, Some(c11_once_handler)) };
// REWRITES-NEXT: unsafe { call_once(std::ptr::addr_of_mut!(once_control) as *mut i32, Some(c11_once_handler)) };
// REWRITES-NEXT: concurrency_total = concurrency_total + unsafe { c11_once_total };
// REWRITES-NEXT: {
// REWRITES-NEXT:         let _v151: Option<unsafe extern "C" fn(*mut core::ffi::c_void)> = None;
// REWRITES-NEXT:         let _v152: i32 = unsafe { tss_create(std::ptr::addr_of_mut!(thread_key) as *mut u32, _v151) };
// REWRITES-NEXT:         let _v153: i32 = 0;
// REWRITES-NEXT:         let _v154: bool = _v152 == _v153;
// REWRITES-NEXT:         if _v154 {
// REWRITES-NEXT:                     let _v155: i32 = 1;
// REWRITES-NEXT:                     concurrency_total = concurrency_total + _v155;
// REWRITES-NEXT:                     let _v158: u32 = thread_key;
// REWRITES-NEXT:                     let _v159: *mut core::ffi::c_void = std::ptr::addr_of_mut!(thread_increment) as *mut core::ffi::c_void;
// REWRITES-NEXT:                     let _v160: i32 = unsafe { tss_set(_v158 as u32, _v159 as *mut core::ffi::c_void) };
// REWRITES-NEXT:                     let _v161: i32 = 0;
// REWRITES-NEXT:                     concurrency_total = concurrency_total + ((_v160 == _v161) as i32);
// REWRITES-NEXT:                     let _v167: *mut core::ffi::c_void = unsafe { tss_get(thread_key as u32) };
// REWRITES-NEXT:                     let _v168: *mut core::ffi::c_void = std::ptr::addr_of_mut!(thread_increment) as *mut core::ffi::c_void;
// REWRITES-NEXT:                     concurrency_total = concurrency_total + ((_v167 == _v168) as i32);
// REWRITES-NEXT:                     unsafe { tss_delete(thread_key as u32) };
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v174: i32 = 4;
// REWRITES-NEXT: let _v175: i32 = 1;
// REWRITES-NEXT: concurrency_total = concurrency_total + ((_v174 >= _v175) as i32);
// REWRITES-NEXT: converted16 = 0;
// REWRITES-NEXT: converted32 = 0;
// REWRITES-NEXT: let _v182: *mut i8 = b"A\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v183: u64 = 1;
// REWRITES-NEXT: let _v184: u64 = (unsafe { mbrtoc16(std::ptr::addr_of_mut!(converted16) as *mut u16, _v182 as *const i8, _v183 as usize, std::ptr::addr_of_mut!(utf16_state) as *mut __mbstate_t) }) as u64;
// REWRITES-NEXT: let _v185: i32 = _v184 as i32;
// REWRITES-NEXT: let _v186: *mut i8 = multibyte16.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let _v187: u16 = 65;
// REWRITES-NEXT: let _v188: u64 = (unsafe { c16rtomb(_v186 as *mut i8, _v187 as u16, std::ptr::addr_of_mut!(utf16_state) as *mut __mbstate_t) }) as u64;
// REWRITES-NEXT: let _v190: i32 = _v185 + (_v188 as i32);
// REWRITES-NEXT: let _v191: *mut i8 = b"B\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v192: u64 = 1;
// REWRITES-NEXT: let _v193: u64 = (unsafe { mbrtoc32(std::ptr::addr_of_mut!(converted32) as *mut u32, _v191 as *const i8, _v192 as usize, std::ptr::addr_of_mut!(utf32_state) as *mut __mbstate_t) }) as u64;
// REWRITES-NEXT: let _v195: i32 = _v190 + (_v193 as i32);
// REWRITES-NEXT: let _v196: *mut i8 = multibyte32.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let _v197: u32 = 66;
// REWRITES-NEXT: let _v198: u64 = (unsafe { c32rtomb(_v196 as *mut i8, _v197 as u32, std::ptr::addr_of_mut!(utf32_state) as *mut __mbstate_t) }) as u64;
// REWRITES-NEXT: let _v207: i64 = 0;
// REWRITES-NEXT: let _v211: i64 = 0;
// REWRITES-NEXT: conversion_total = _v195 + (_v198 as i32) + (converted16 as i32) + (converted32 as i32) + (multibyte16[(_v207 as usize)] as i32) + (multibyte32[(_v211 as usize)] as i32);
// REWRITES-NEXT: let _v215: u64 = 64;
// REWRITES-NEXT: let _v216: u64 = 64;
// REWRITES-NEXT: aligned_memory = unsafe { aligned_alloc(_v215 as usize, _v216 as usize) };
// REWRITES-NEXT: let _v219: *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-NEXT: let _v221: bool = if aligned_memory != _v219 {
// REWRITES-NEXT:         let _v224: u64 = 64;
// REWRITES-NEXT:         let _v226: u64 = 0;
// REWRITES-NEXT:         let _v227: bool = (aligned_memory as u64) % _v224 == _v226;
// REWRITES-NEXT:     _v227
// REWRITES-NEXT: } else {
// REWRITES-NEXT:         let _v228: bool = false;
// REWRITES-NEXT:     _v228
// REWRITES-NEXT: };
// REWRITES-NEXT: alignment_total = alignment_total + (_v221 as i32);
// REWRITES-NEXT: unsafe { free(aligned_memory as *mut core::ffi::c_void) };
// REWRITES-NEXT: let _v233: i32 = unsafe { at_quick_exit(Some(c11_quick_handler)) };
// REWRITES-NEXT: let _v234: i32 = 0;
// REWRITES-NEXT: quick_total = (_v233 == _v234) as i32;
// REWRITES-NEXT: let _v237: *mut i8 = b"slate-c11-exclusive.tmp\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v238: i32 = unsafe { remove(_v237 as *const i8) };
// REWRITES-NEXT: let _v239: *mut i8 = b"slate-c11-exclusive.tmp\0".as_ptr() as *mut i8;
// REWRITES-NEXT: exclusive_first = c11_open_exclusive(_v239);
// REWRITES-NEXT: let _v241: *mut i8 = b"slate-c11-exclusive.tmp\0".as_ptr() as *mut i8;
// REWRITES-NEXT: exclusive_second = c11_open_exclusive(_v241);
// REWRITES-NEXT: let _v244: *mut libc::FILE = std::ptr::null_mut();
// REWRITES-NEXT: let _v246: bool = if exclusive_first != _v244 {
// REWRITES-NEXT:         let _v248: *mut libc::FILE = std::ptr::null_mut();
// REWRITES-NEXT:         let _v249: bool = exclusive_second == _v248;
// REWRITES-NEXT:     _v249
// REWRITES-NEXT: } else {
// REWRITES-NEXT:         let _v250: bool = false;
// REWRITES-NEXT:     _v250
// REWRITES-NEXT: };
// REWRITES-NEXT: exclusive_total = _v246 as i32;
// REWRITES-NEXT: {
// REWRITES-NEXT:         let _v253: *mut libc::FILE = std::ptr::null_mut();
// REWRITES-NEXT:         let _v254: bool = exclusive_first != _v253;
// REWRITES-NEXT:         if _v254 {
// REWRITES-NEXT:                     let _v256: i32 = unsafe { fclose(exclusive_first as *mut libc::FILE) };
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: {
// REWRITES-NEXT:         let _v258: *mut libc::FILE = std::ptr::null_mut();
// REWRITES-NEXT:         let _v259: bool = exclusive_second != _v258;
// REWRITES-NEXT:         if _v259 {
// REWRITES-NEXT:                     let _v261: i32 = unsafe { fclose(exclusive_second as *mut libc::FILE) };
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v262: *mut i8 = b"slate-c11-exclusive.tmp\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v263: i32 = unsafe { remove(_v262 as *const i8) };
// REWRITES-NEXT: let _v264: i32 = 1;
// REWRITES-NEXT: let _v265: i32 = unsafe { timespec_get(std::ptr::addr_of_mut!(current_time) as *mut libc::timespec, _v264 as i32) };
// REWRITES-NEXT: let _v266: i32 = 1;
// REWRITES-NEXT: let _v268: bool = if _v265 == _v266 {
// REWRITES-NEXT:         let _v270: i64 = 0;
// REWRITES-NEXT:         let _v271: bool = current_time.tv_nsec >= _v270;
// REWRITES-NEXT:     _v271
// REWRITES-NEXT: } else {
// REWRITES-NEXT:         let _v272: bool = false;
// REWRITES-NEXT:     _v272
// REWRITES-NEXT: };
// REWRITES-NEXT: let _v273: bool = if _v268 {
// REWRITES-NEXT:         let _v275: i64 = 1000000000;
// REWRITES-NEXT:         let _v276: bool = current_time.tv_nsec < _v275;
// REWRITES-NEXT:     _v276
// REWRITES-NEXT: } else {
// REWRITES-NEXT:         let _v277: bool = false;
// REWRITES-NEXT:     _v277
// REWRITES-NEXT: };
// REWRITES-NEXT: timespec_total = _v273 as i32;
// REWRITES-NEXT: complex_value = num_complex::Complex { re: 2.0, im: 3.0 };
// REWRITES-NEXT: let _v280: num_complex::Complex<f64> = complex_value;
// REWRITES-NEXT: let _v282: f64 = 2.0;
// REWRITES-NEXT: let _v285: num_complex::Complex<f64> = complex_value;
// REWRITES-NEXT: let _v287: f64 = 3.0;
// REWRITES-NEXT: complex_total = ((_v280.re == _v282) as i32) + ((_v285.im == _v287) as i32);
// REWRITES-NEXT: let _v292: i32 = 6;
// REWRITES-NEXT: let _v296: i32 = 10;
// REWRITES-NEXT: let _v301: i32 = 10;
// REWRITES-NEXT: let _v305: f32 = 0.0000000000000000000000000000000000000000000014013;
// REWRITES-NEXT: let _v306: f32 = 0.0;
// REWRITES-NEXT: let _v310: f64 = 0.000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000005;
// REWRITES-NEXT: let _v311: f64 = 0.0;
// REWRITES-NEXT: let _v320: i32 = 1;
// REWRITES-NEXT: let _v321: i32 = -1;
// REWRITES-NEXT: let _v325: i32 = 1;
// REWRITES-NEXT: let _v326: i32 = -1;
// REWRITES-NEXT: let _v330: i32 = 1;
// REWRITES-NEXT: let _v331: i32 = -1;
// REWRITES-NEXT: limits_total = (((9 as i32) >= _v292) as i32) + (((17 as i32) >= _v296) as i32) + (((21 as i32) >= _v301) as i32) + ((_v305 > _v306) as i32) + ((_v310 > _v311) as i32) + ((LongDouble([1, 0, 0, 0, 0, 0, 0, 0, 0, 0]) > LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0])) as i32) + ((_v320 >= _v321) as i32) + ((_v325 >= _v326) as i32) + ((_v330 >= _v331) as i32);
// REWRITES-NEXT: let _v335: i32 = 0;
// REWRITES-NEXT: limits_total = limits_total + _v335;
// REWRITES-NEXT: {
// REWRITES-NEXT:         let _v338: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(c11_never_flag)) };
// REWRITES-NEXT:         let _v339: bool = _v338 != 0;
// REWRITES-NEXT:         if _v339 {
// REWRITES-NEXT:                     let _v340: i32 = 99;
// REWRITES-NEXT:                     c11_never_return(_v340);
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v341: *mut i8 = b"%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v359: i32 = 0;
// REWRITES-NEXT: let _v360: i32 = unsafe { printf(_v341 as *const i8, alignment_total, unicode_total, generic_total, anonymous_total, evaluation_total, temporary_total, static_assert_total, optional_total, atomic_total, thread_total, concurrency_total, conversion_total, quick_total, exclusive_total, timespec_total, complex_total, limits_total, _v359) };
// REWRITES-NEXT: let _v361: i32 = 10;
// REWRITES-NEXT: let _v362: i32 = unsafe { putchar(_v361 as i32) };
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
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
