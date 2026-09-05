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
// LOWERING-NEXT: #![allow(
// LOWERING-NEXT:     dead_code,
// LOWERING-NEXT:     unused,
// LOWERING-NEXT:     non_camel_case_types,
// LOWERING-NEXT:     non_snake_case,
// LOWERING-NEXT:     non_upper_case_globals,
// LOWERING-NEXT:     arithmetic_overflow,
// LOWERING-NEXT:     unconditional_panic,
// LOWERING-NEXT:     suspicious_runtime_symbol_definitions,
// LOWERING-NEXT:     unpredictable_function_pointer_comparisons,
// LOWERING-NEXT:     unused_comparisons
// LOWERING-NEXT: )]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn __muldc3(__a: f64, __b: f64, __c: f64, __d: f64) -> num_complex::Complex<f64>;
// LOWERING-NEXT:     fn __divdc3(__a: f64, __b: f64, __c: f64, __d: f64) -> num_complex::Complex<f64>;
// LOWERING-NEXT:     fn __mulsc3(__a: f32, __b: f32, __c: f32, __d: f32) -> num_complex::Complex<f32>;
// LOWERING-NEXT:     fn __divsc3(__a: f32, __b: f32, __c: f32, __d: f32) -> num_complex::Complex<f32>;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C, align(16))]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct LongDouble([u8; 10]);
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::ops::Add for LongDouble {
// LOWERING-NEXT:     type Output = LongDouble;
// LOWERING-NEXT:     fn add(self, __o: LongDouble) -> LongDouble {
// LOWERING-NEXT:         __slate_f80_add(self, __o)
// LOWERING-NEXT:     }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::ops::Sub for LongDouble {
// LOWERING-NEXT:     type Output = LongDouble;
// LOWERING-NEXT:     fn sub(self, __o: LongDouble) -> LongDouble {
// LOWERING-NEXT:         __slate_f80_sub(self, __o)
// LOWERING-NEXT:     }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::ops::Mul for LongDouble {
// LOWERING-NEXT:     type Output = LongDouble;
// LOWERING-NEXT:     fn mul(self, __o: LongDouble) -> LongDouble {
// LOWERING-NEXT:         __slate_f80_mul(self, __o)
// LOWERING-NEXT:     }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::ops::Div for LongDouble {
// LOWERING-NEXT:     type Output = LongDouble;
// LOWERING-NEXT:     fn div(self, __o: LongDouble) -> LongDouble {
// LOWERING-NEXT:         __slate_f80_div(self, __o)
// LOWERING-NEXT:     }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::ops::AddAssign for LongDouble {
// LOWERING-NEXT:     fn add_assign(&mut self, __o: LongDouble) {
// LOWERING-NEXT:         {
// LOWERING-NEXT:             *self = __slate_f80_add(*self, __o);
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::ops::SubAssign for LongDouble {
// LOWERING-NEXT:     fn sub_assign(&mut self, __o: LongDouble) {
// LOWERING-NEXT:         {
// LOWERING-NEXT:             *self = __slate_f80_sub(*self, __o);
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::ops::MulAssign for LongDouble {
// LOWERING-NEXT:     fn mul_assign(&mut self, __o: LongDouble) {
// LOWERING-NEXT:         {
// LOWERING-NEXT:             *self = __slate_f80_mul(*self, __o);
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::ops::DivAssign for LongDouble {
// LOWERING-NEXT:     fn div_assign(&mut self, __o: LongDouble) {
// LOWERING-NEXT:         {
// LOWERING-NEXT:             *self = __slate_f80_div(*self, __o);
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::ops::Neg for LongDouble {
// LOWERING-NEXT:     type Output = LongDouble;
// LOWERING-NEXT:     fn neg(self) -> LongDouble {
// LOWERING-NEXT:         __slate_f80_neg(self)
// LOWERING-NEXT:     }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::cmp::PartialEq for LongDouble {
// LOWERING-NEXT:     fn eq(&self, __other: &LongDouble) -> bool {
// LOWERING-NEXT:         __slate_f80_eq(*self, *__other)
// LOWERING-NEXT:     }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::cmp::PartialOrd for LongDouble {
// LOWERING-NEXT:     fn partial_cmp(&self, __other: &LongDouble) -> Option<std::cmp::Ordering> {
// LOWERING-NEXT:         if __slate_f80_lt(*self, *__other) {
// LOWERING-NEXT:             Some(std::cmp::Ordering::Less)
// LOWERING-NEXT:         } else {
// LOWERING-NEXT:             if __slate_f80_gt(*self, *__other) {
// LOWERING-NEXT:                 Some(std::cmp::Ordering::Greater)
// LOWERING-NEXT:             } else {
// LOWERING-NEXT:                 if __slate_f80_eq(*self, *__other) {
// LOWERING-NEXT:                     Some(std::cmp::Ordering::Equal)
// LOWERING-NEXT:                 } else {
// LOWERING-NEXT:                     None
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
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
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct C11Anonymous {
// LOWERING-NEXT:     __slate_anon_0: {{anon_[0-9]+}},
// LOWERING-NEXT:     __slate_anon_1: {{anon_[0-9]+}},
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C, align(32))]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct C11OverAligned {
// LOWERING-NEXT:     value: u8,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct C11Temporary {
// LOWERING-NEXT:     values: [i32; 3],
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct __mbstate_t {
// LOWERING-NEXT:     __count: i32,
// LOWERING-NEXT:     __value: {{anon_[0-9]+}},
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: union {{anon_[0-9]+}} {
// LOWERING-NEXT:     integer: i32,
// LOWERING-NEXT:     __slate_anon_1: f64,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct {{anon_[0-9]+}} {
// LOWERING-NEXT:     x: i32,
// LOWERING-NEXT:     y: i32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: union {{anon_[0-9]+}} {
// LOWERING-NEXT:     __wch: i32,
// LOWERING-NEXT:     __wchb: [i8; 4],
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: union cnd_t {
// LOWERING-NEXT:     __size: [i8; 48],
// LOWERING-NEXT:     __align: i64,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: union mtx_t {
// LOWERING-NEXT:     __size: [i8; 40],
// LOWERING-NEXT:     __align: i64,
// LOWERING-NEXT: }
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
// LOWERING-NEXT:     fn thrd_create(
// LOWERING-NEXT:         _0: *mut u64,
// LOWERING-NEXT:         _1: Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void) -> i32>,
// LOWERING-NEXT:         _2: *mut core::ffi::c_void,
// LOWERING-NEXT:     ) -> i32;
// LOWERING-NEXT:     fn thrd_join(_0: u64, _1: *mut i32) -> i32;
// LOWERING-NEXT:     fn mtx_init(_0: *mut mtx_t, _1: i32) -> i32;
// LOWERING-NEXT:     fn mtx_lock(_0: *mut mtx_t) -> i32;
// LOWERING-NEXT:     fn mtx_unlock(_0: *mut mtx_t) -> i32;
// LOWERING-NEXT:     fn mtx_destroy(_0: *mut mtx_t);
// LOWERING-NEXT:     fn cnd_init(_0: *mut cnd_t) -> i32;
// LOWERING-NEXT:     fn cnd_destroy(_0: *mut cnd_t);
// LOWERING-NEXT:     fn call_once(_0: *mut i32, _1: Option<unsafe extern "C-unwind" fn()>);
// LOWERING-NEXT:     fn tss_create(
// LOWERING-NEXT:         _0: *mut u32,
// LOWERING-NEXT:         _1: Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void)>,
// LOWERING-NEXT:     ) -> i32;
// LOWERING-NEXT:     fn tss_set(_0: u32, _1: *mut core::ffi::c_void) -> i32;
// LOWERING-NEXT:     fn tss_get(_0: u32) -> *mut core::ffi::c_void;
// LOWERING-NEXT:     fn tss_delete(_0: u32);
// LOWERING-NEXT:     fn mbrtoc16(
// LOWERING-NEXT:         _0: *mut u16,
// LOWERING-NEXT:         _1: *const core::ffi::c_char,
// LOWERING-NEXT:         _2: usize,
// LOWERING-NEXT:         _3: *mut __mbstate_t,
// LOWERING-NEXT:     ) -> usize;
// LOWERING-NEXT:     fn c16rtomb(_0: *mut core::ffi::c_char, _1: u16, _2: *mut __mbstate_t) -> usize;
// LOWERING-NEXT:     fn mbrtoc32(
// LOWERING-NEXT:         _0: *mut u32,
// LOWERING-NEXT:         _1: *const core::ffi::c_char,
// LOWERING-NEXT:         _2: usize,
// LOWERING-NEXT:         _3: *mut __mbstate_t,
// LOWERING-NEXT:     ) -> usize;
// LOWERING-NEXT:     fn c32rtomb(_0: *mut core::ffi::c_char, _1: u32, _2: *mut __mbstate_t) -> usize;
// LOWERING-NEXT:     fn aligned_alloc(_0: usize, _1: usize) -> *mut core::ffi::c_void;
// LOWERING-NEXT:     fn free(_0: *mut core::ffi::c_void);
// LOWERING-NEXT:     fn at_quick_exit(_0: Option<unsafe extern "C-unwind" fn()>) -> i32;
// LOWERING-NEXT:     fn remove(_0: *const core::ffi::c_char) -> i32;
// LOWERING-NEXT:     fn fclose(_0: *mut libc::FILE) -> i32;
// LOWERING-NEXT:     fn timespec_get(_0: *mut libc::timespec, _1: i32) -> i32;
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT:     fn putchar(_0: i32) -> i32;
// LOWERING-NEXT:     fn fopen(_0: *const core::ffi::c_char, _1: *const core::ffi::c_char) -> *mut libc::FILE;
// LOWERING-NEXT:     fn quick_exit(_0: i32) -> !;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut anonymous: C11Anonymous = C11Anonymous {
// LOWERING-NEXT:         __slate_anon_0: unsafe { std::mem::zeroed::<{{anon_[0-9]+}}>() },
// LOWERING-NEXT:         __slate_anon_1: {{anon_[0-9]+}} { x: 0, y: 0 },
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let mut aligned_object: aligned::Aligned<aligned::A32, C11OverAligned> =
// LOWERING-NEXT:         aligned::Aligned(C11OverAligned { value: 0 });
// LOWERING-NEXT:     let mut utf16_state: __mbstate_t = __mbstate_t {
// LOWERING-NEXT:         __count: 0,
// LOWERING-NEXT:         __value: unsafe { std::mem::zeroed::<{{anon_[0-9]+}}>() },
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let mut utf32_state: __mbstate_t = __mbstate_t {
// LOWERING-NEXT:         __count: 0,
// LOWERING-NEXT:         __value: unsafe { std::mem::zeroed::<{{anon_[0-9]+}}>() },
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let mut converted16: u16 = 0;
// LOWERING-NEXT:     let mut converted32: u32 = 0;
// LOWERING-NEXT:     let mut multibyte16: [i8; 4] = [0; 4];
// LOWERING-NEXT:     let mut multibyte32: [i8; 4] = [0; 4];
// LOWERING-NEXT:     let mut current_time: libc::timespec = libc::timespec {
// LOWERING-NEXT:         tv_sec: 0,
// LOWERING-NEXT:         tv_nsec: 0,
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let mut thread: u64 = 0;
// LOWERING-NEXT:     let mut mutex: mtx_t = unsafe { std::mem::zeroed::<mtx_t>() };
// LOWERING-NEXT:     let mut condition: cnd_t = unsafe { std::mem::zeroed::<cnd_t>() };
// LOWERING-NEXT:     let mut once_control: i32 = 0;
// LOWERING-NEXT:     let mut thread_key: u32 = 0;
// LOWERING-NEXT:     let mut thread_increment: i32 = 0;
// LOWERING-NEXT:     let mut thread_result: i32 = 0;
// LOWERING-NEXT:     let mut aligned_memory: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let mut exclusive_first: *mut libc::FILE = std::ptr::null_mut();
// LOWERING-NEXT:     let mut exclusive_second: *mut libc::FILE = std::ptr::null_mut();
// LOWERING-NEXT:     let mut concurrency_total: i32 = 0;
// LOWERING-NEXT:     let mut ref_tmp0: C11Temporary = C11Temporary { values: [0; 3] };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u16 = 937;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = 128578;
// LOWERING-NEXT:     let {{_v[0-9]+}}: C11Anonymous = C11Anonymous {
// LOWERING-NEXT:         __slate_anon_0: unsafe { std::mem::zeroed::<{{anon_[0-9]+}}>() },
// LOWERING-NEXT:         __slate_anon_1: {{anon_[0-9]+}} { x: 0, y: 0 },
// LOWERING-NEXT:     };
// LOWERING-NEXT:     anonymous = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: C11OverAligned = C11OverAligned { value: 0 };
// LOWERING-NEXT:     *aligned_object = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: __mbstate_t = __mbstate_t {
// LOWERING-NEXT:         __count: 0,
// LOWERING-NEXT:         __value: unsafe { std::mem::zeroed::<{{anon_[0-9]+}}>() },
// LOWERING-NEXT:     };
// LOWERING-NEXT:     utf16_state = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: __mbstate_t = __mbstate_t {
// LOWERING-NEXT:         __count: 0,
// LOWERING-NEXT:         __value: unsafe { std::mem::zeroed::<{{anon_[0-9]+}}>() },
// LOWERING-NEXT:     };
// LOWERING-NEXT:     utf32_state = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u16 = 0;
// LOWERING-NEXT:     converted16 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = 0;
// LOWERING-NEXT:     converted32 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: libc::timespec = libc::timespec {
// LOWERING-NEXT:         tv_sec: 0,
// LOWERING-NEXT:         tv_nsec: 0,
// LOWERING-NEXT:     };
// LOWERING-NEXT:     current_time = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     once_control = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 7;
// LOWERING-NEXT:     thread_increment = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     thread_result = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 4;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = std::mem::align_of::<C11OverAligned>() as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(c11_aligned_buffer).cast::<u8>();
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} as u64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} % {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = std::ptr::addr_of_mut!(*aligned_object) as u64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} % {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i8 = unsafe { main_utf8_text[({{_v[0-9]+}} as usize)] };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u8 = {{_v[0-9]+}} as u8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i8 = unsafe { main_utf8_text[({{_v[0-9]+}} as usize)] };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u8 = {{_v[0-9]+}} as u8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u16 = unsafe { main_utf16_text[({{_v[0-9]+}} as usize)] };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = unsafe { main_utf32_text[({{_v[0-9]+}} as usize)] };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 11;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 22;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 33;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 31;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         anonymous.__slate_anon_0.integer = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 37;
// LOWERING-NEXT:     anonymous.__slate_anon_1.x = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 41;
// LOWERING-NEXT:     anonymous.__slate_anon_1.y = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { anonymous.__slate_anon_0.integer };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = anonymous.__slate_anon_1.x;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = anonymous.__slate_anon_1.y;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         c11_evaluation_total = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = c11_evaluation_step({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = c11_evaluation_step({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { c11_evaluation_total };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 43;
// LOWERING-NEXT:     let {{_v[0-9]+}}: C11Temporary = c11_make_temporary({{_v[0-9]+}});
// LOWERING-NEXT:     ref_tmp0 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = ref_tmp0.values[({{_v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 5;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         c11_atomic_total = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 17;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         c11_thread_local_value = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = unsafe { __errno_location() };
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *{{_v[0-9]+}} = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void =
// LOWERING-NEXT:         std::ptr::addr_of_mut!(thread_increment) as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         thrd_create(
// LOWERING-NEXT:             std::ptr::addr_of_mut!(thread) as *mut u64,
// LOWERING-NEXT:             Some(c11_thread_worker),
// LOWERING-NEXT:             {{_v[0-9]+}} as *mut core::ffi::c_void,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: u64 = thread;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:             thrd_join(
// LOWERING-NEXT:                 {{_v[0-9]+}} as u64,
// LOWERING-NEXT:                 std::ptr::addr_of_mut!(thread_result) as *mut i32,
// LOWERING-NEXT:             )
// LOWERING-NEXT:         };
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = -1;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(c11_atomic_total))
// LOWERING-NEXT:             .load(std::sync::atomic::Ordering::SeqCst)
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = thread_result;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { c11_thread_local_value };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = unsafe { __errno_location() };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     concurrency_total = {{_v[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 =
// LOWERING-NEXT:             unsafe { mtx_init(std::ptr::addr_of_mut!(mutex) as *mut mtx_t, {{_v[0-9]+}} as i32) };
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:         if {{_v[0-9]+}} {
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = concurrency_total;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:             concurrency_total = {{_v[0-9]+}};
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = unsafe { mtx_lock(std::ptr::addr_of_mut!(mutex) as *mut mtx_t) };
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = concurrency_total;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:             concurrency_total = {{_v[0-9]+}};
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = unsafe { mtx_unlock(std::ptr::addr_of_mut!(mutex) as *mut mtx_t) };
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = concurrency_total;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:             concurrency_total = {{_v[0-9]+}};
// LOWERING-NEXT:             unsafe { mtx_destroy(std::ptr::addr_of_mut!(mutex) as *mut mtx_t) };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = unsafe { cnd_init(std::ptr::addr_of_mut!(condition) as *mut cnd_t) };
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:         if {{_v[0-9]+}} {
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = concurrency_total;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:             concurrency_total = {{_v[0-9]+}};
// LOWERING-NEXT:             unsafe { cnd_destroy(std::ptr::addr_of_mut!(condition) as *mut cnd_t) };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         c11_once_total = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         call_once(
// LOWERING-NEXT:             std::ptr::addr_of_mut!(once_control) as *mut i32,
// LOWERING-NEXT:             Some(c11_once_handler),
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         call_once(
// LOWERING-NEXT:             std::ptr::addr_of_mut!(once_control) as *mut i32,
// LOWERING-NEXT:             Some(c11_once_handler),
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { c11_once_total };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = concurrency_total;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     concurrency_total = {{_v[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{_v[0-9]+}}: Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void)> = None;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 =
// LOWERING-NEXT:             unsafe { tss_create(std::ptr::addr_of_mut!(thread_key) as *mut u32, {{_v[0-9]+}}) };
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:         if {{_v[0-9]+}} {
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = concurrency_total;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:             concurrency_total = {{_v[0-9]+}};
// LOWERING-NEXT:             let {{_v[0-9]+}}: u32 = thread_key;
// LOWERING-NEXT:             let {{_v[0-9]+}}: *mut core::ffi::c_void =
// LOWERING-NEXT:                 std::ptr::addr_of_mut!(thread_increment) as *mut core::ffi::c_void;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = unsafe { tss_set({{_v[0-9]+}} as u32, {{_v[0-9]+}} as *mut core::ffi::c_void) };
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = concurrency_total;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:             concurrency_total = {{_v[0-9]+}};
// LOWERING-NEXT:             let {{_v[0-9]+}}: u32 = thread_key;
// LOWERING-NEXT:             let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe { tss_get({{_v[0-9]+}} as u32) };
// LOWERING-NEXT:             let {{_v[0-9]+}}: *mut core::ffi::c_void =
// LOWERING-NEXT:                 std::ptr::addr_of_mut!(thread_increment) as *mut core::ffi::c_void;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = concurrency_total;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:             concurrency_total = {{_v[0-9]+}};
// LOWERING-NEXT:             let {{_v[0-9]+}}: u32 = thread_key;
// LOWERING-NEXT:             unsafe { tss_delete({{_v[0-9]+}} as u32) };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 4;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} >= {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = concurrency_total;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     concurrency_total = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u16 = 0;
// LOWERING-NEXT:     converted16 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = 0;
// LOWERING-NEXT:     converted32 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"A\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = (unsafe {
// LOWERING-NEXT:         mbrtoc16(
// LOWERING-NEXT:             std::ptr::addr_of_mut!(converted16) as *mut u16,
// LOWERING-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{_v[0-9]+}} as usize,
// LOWERING-NEXT:             std::ptr::addr_of_mut!(utf16_state) as *mut __mbstate_t,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     }) as u64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = multibyte16.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u16 = 65;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = (unsafe {
// LOWERING-NEXT:         c16rtomb(
// LOWERING-NEXT:             {{_v[0-9]+}} as *mut core::ffi::c_char,
// LOWERING-NEXT:             {{_v[0-9]+}} as u16,
// LOWERING-NEXT:             std::ptr::addr_of_mut!(utf16_state) as *mut __mbstate_t,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     }) as u64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"B\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = (unsafe {
// LOWERING-NEXT:         mbrtoc32(
// LOWERING-NEXT:             std::ptr::addr_of_mut!(converted32) as *mut u32,
// LOWERING-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{_v[0-9]+}} as usize,
// LOWERING-NEXT:             std::ptr::addr_of_mut!(utf32_state) as *mut __mbstate_t,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     }) as u64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = multibyte32.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = 66;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = (unsafe {
// LOWERING-NEXT:         c32rtomb(
// LOWERING-NEXT:             {{_v[0-9]+}} as *mut core::ffi::c_char,
// LOWERING-NEXT:             {{_v[0-9]+}} as u32,
// LOWERING-NEXT:             std::ptr::addr_of_mut!(utf32_state) as *mut __mbstate_t,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     }) as u64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u16 = converted16;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = converted32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i8 = multibyte16[({{_v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i8 = multibyte32[({{_v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe { aligned_alloc({{_v[0-9]+}} as usize, {{_v[0-9]+}} as usize) };
// LOWERING-NEXT:     aligned_memory = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = aligned_memory;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut core::ffi::c_void = aligned_memory;
// LOWERING-NEXT:         let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} as u64;
// LOWERING-NEXT:         let {{_v[0-9]+}}: u64 = 64;
// LOWERING-NEXT:         let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} % {{_v[0-9]+}};
// LOWERING-NEXT:         let {{_v[0-9]+}}: u64 = 0;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = aligned_memory;
// LOWERING-NEXT:     unsafe { free({{_v[0-9]+}} as *mut core::ffi::c_void) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { at_quick_exit(Some(c11_quick_handler)) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"slate-c11-exclusive.tmp\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { remove({{_v[0-9]+}} as *const core::ffi::c_char) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"slate-c11-exclusive.tmp\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut libc::FILE = c11_open_exclusive({{_v[0-9]+}});
// LOWERING-NEXT:     exclusive_first = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"slate-c11-exclusive.tmp\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut libc::FILE = c11_open_exclusive({{_v[0-9]+}});
// LOWERING-NEXT:     exclusive_second = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut libc::FILE = exclusive_first;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut libc::FILE = std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut libc::FILE = exclusive_second;
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut libc::FILE = std::ptr::null_mut();
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut libc::FILE = exclusive_first;
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut libc::FILE = std::ptr::null_mut();
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:         if {{_v[0-9]+}} {
// LOWERING-NEXT:             let {{_v[0-9]+}}: *mut libc::FILE = exclusive_first;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = unsafe { fclose({{_v[0-9]+}} as *mut libc::FILE) };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut libc::FILE = exclusive_second;
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut libc::FILE = std::ptr::null_mut();
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:         if {{_v[0-9]+}} {
// LOWERING-NEXT:             let {{_v[0-9]+}}: *mut libc::FILE = exclusive_second;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = unsafe { fclose({{_v[0-9]+}} as *mut libc::FILE) };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"slate-c11-exclusive.tmp\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { remove({{_v[0-9]+}} as *const core::ffi::c_char) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         timespec_get(
// LOWERING-NEXT:             std::ptr::addr_of_mut!(current_time) as *mut libc::timespec,
// LOWERING-NEXT:             {{_v[0-9]+}} as i32,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: i64 = current_time.tv_nsec;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} >= {{_v[0-9]+}};
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: i64 = current_time.tv_nsec;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i64 = 1000000000;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex { re: 2.0, im: 3.0 };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.re;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 2.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.im;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 3.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 9 as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 6;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} >= {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 17 as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 10;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} >= {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 21 as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 10;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} >= {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = 0.0000000000000000000000000000000000000000000014013;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = 0.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} > {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
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
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(c11_never_flag)) };
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:         if {{_v[0-9]+}} {
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 99;
// LOWERING-NEXT:             c11_never_return({{_v[0-9]+}});
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 =
// LOWERING-NEXT:         b"%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = concurrency_total;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         printf(
// LOWERING-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 10;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { putchar({{_v[0-9]+}} as i32) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn c11_evaluation_step({{arg[0-9]+}}: i32) -> i32 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { c11_evaluation_total };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{arg[0-9]+}};
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         c11_evaluation_total = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{arg[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn c11_make_temporary({{arg[0-9]+}}: i32) -> C11Temporary {
// LOWERING-NEXT:     let mut __retval: C11Temporary = C11Temporary { values: [0; 3] };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = std::ptr::addr_of_mut!(__retval.values) as *mut i32;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *{{_v[0-9]+}} = {{arg[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.add(1) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *{{_v[0-9]+}} = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.add(2) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *{{_v[0-9]+}} = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: C11Temporary = __retval;
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: extern "C-unwind" fn c11_thread_worker({{arg[0-9]+}}: *mut core::ffi::c_void) -> i32 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = {{arg[0-9]+}} as *mut i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 29;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         c11_thread_local_value = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 34;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = unsafe { __errno_location() };
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *{{_v[0-9]+}} = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(c11_atomic_total))
// LOWERING-NEXT:             .fetch_add({{_v[0-9]+}}, std::sync::atomic::Ordering::SeqCst)
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { c11_thread_local_value };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = unsafe { __errno_location() };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 34;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: extern "C-unwind" fn c11_once_handler() {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { c11_once_total };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         c11_once_total = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: extern "C-unwind" fn c11_quick_handler() {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 100;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(c11_atomic_total))
// LOWERING-NEXT:             .fetch_add({{_v[0-9]+}}, std::sync::atomic::Ordering::Relaxed)
// LOWERING-NEXT:     };
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn c11_open_exclusive({{arg[0-9]+}}: *mut i8) -> *mut libc::FILE {
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"wx\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut libc::FILE = unsafe {
// LOWERING-NEXT:         fopen(
// LOWERING-NEXT:             {{arg[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn c11_never_return({{arg[0-9]+}}: i32) -> ! {
// LOWERING-NEXT:     unsafe { quick_exit({{arg[0-9]+}} as i32) }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     safe fn __slate_cf80_div(
// LOWERING-NEXT:         __a: num_complex::Complex<LongDouble>,
// LOWERING-NEXT:         __b: num_complex::Complex<LongDouble>,
// LOWERING-NEXT:     ) -> num_complex::Complex<LongDouble>;
// LOWERING-NEXT:     safe fn __slate_cf80_mul(
// LOWERING-NEXT:         __a: num_complex::Complex<LongDouble>,
// LOWERING-NEXT:         __b: num_complex::Complex<LongDouble>,
// LOWERING-NEXT:     ) -> num_complex::Complex<LongDouble>;
// LOWERING-NEXT:     safe fn __slate_f80_abs(__a: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_add(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_ceil(__a: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_copysign(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_div(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_eq(__a: LongDouble, __b: LongDouble) -> bool;
// LOWERING-NEXT:     safe fn __slate_f80_floor(__a: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_fma(__a: LongDouble, __b: LongDouble, __c: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_fmax(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_fmin(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_fract(__a: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_from_bool(__a: bool) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_from_f32(__a: f32) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_from_f64(__a: f64) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_from_i128(__a: i128) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_from_i16(__a: i16) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_from_i32(__a: i32) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_from_i64(__a: i64) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_from_i8(__a: i8) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_from_u128(__a: u128) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_from_u16(__a: u16) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_from_u32(__a: u32) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_from_u64(__a: u64) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_from_u8(__a: u8) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_ge(__a: LongDouble, __b: LongDouble) -> bool;
// LOWERING-NEXT:     safe fn __slate_f80_gt(__a: LongDouble, __b: LongDouble) -> bool;
// LOWERING-NEXT:     safe fn __slate_f80_is_fp_class(__a: LongDouble, __flags: i32) -> bool;
// LOWERING-NEXT:     safe fn __slate_f80_le(__a: LongDouble, __b: LongDouble) -> bool;
// LOWERING-NEXT:     safe fn __slate_f80_lt(__a: LongDouble, __b: LongDouble) -> bool;
// LOWERING-NEXT:     safe fn __slate_f80_mul(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_ne(__a: LongDouble, __b: LongDouble) -> bool;
// LOWERING-NEXT:     safe fn __slate_f80_neg(__a: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_rint(__a: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_round(__a: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_signbit(__a: LongDouble) -> bool;
// LOWERING-NEXT:     safe fn __slate_f80_sub(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_to_bool(__a: LongDouble) -> bool;
// LOWERING-NEXT:     safe fn __slate_f80_to_f32(__a: LongDouble) -> f32;
// LOWERING-NEXT:     safe fn __slate_f80_to_f64(__a: LongDouble) -> f64;
// LOWERING-NEXT:     safe fn __slate_f80_to_i128(__a: LongDouble) -> i128;
// LOWERING-NEXT:     safe fn __slate_f80_to_i16(__a: LongDouble) -> i16;
// LOWERING-NEXT:     safe fn __slate_f80_to_i32(__a: LongDouble) -> i32;
// LOWERING-NEXT:     safe fn __slate_f80_to_i64(__a: LongDouble) -> i64;
// LOWERING-NEXT:     safe fn __slate_f80_to_i8(__a: LongDouble) -> i8;
// LOWERING-NEXT:     safe fn __slate_f80_to_u128(__a: LongDouble) -> u128;
// LOWERING-NEXT:     safe fn __slate_f80_to_u16(__a: LongDouble) -> u16;
// LOWERING-NEXT:     safe fn __slate_f80_to_u32(__a: LongDouble) -> u32;
// LOWERING-NEXT:     safe fn __slate_f80_to_u64(__a: LongDouble) -> u64;
// LOWERING-NEXT:     safe fn __slate_f80_to_u8(__a: LongDouble) -> u8;
// LOWERING-NEXT:     safe fn __slate_f80_trunc(__a: LongDouble) -> LongDouble;
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(thread_local)]
// REWRITES-NEXT: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(
// REWRITES-NEXT:     dead_code,
// REWRITES-NEXT:     unused,
// REWRITES-NEXT:     non_camel_case_types,
// REWRITES-NEXT:     non_snake_case,
// REWRITES-NEXT:     non_upper_case_globals,
// REWRITES-NEXT:     arithmetic_overflow,
// REWRITES-NEXT:     unconditional_panic,
// REWRITES-NEXT:     suspicious_runtime_symbol_definitions,
// REWRITES-NEXT:     unpredictable_function_pointer_comparisons,
// REWRITES-NEXT:     unused_comparisons
// REWRITES-NEXT: )]
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn __muldc3(__a: f64, __b: f64, __c: f64, __d: f64) -> num_complex::Complex<f64>;
// REWRITES-NEXT:     fn __divdc3(__a: f64, __b: f64, __c: f64, __d: f64) -> num_complex::Complex<f64>;
// REWRITES-NEXT:     fn __mulsc3(__a: f32, __b: f32, __c: f32, __d: f32) -> num_complex::Complex<f32>;
// REWRITES-NEXT:     fn __divsc3(__a: f32, __b: f32, __c: f32, __d: f32) -> num_complex::Complex<f32>;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C, align(16))]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct LongDouble([u8; 10]);
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::ops::Add for LongDouble {
// REWRITES-NEXT:     type Output = LongDouble;
// REWRITES-NEXT:     fn add(self, __o: LongDouble) -> LongDouble {
// REWRITES-NEXT:         __slate_f80_add(self, __o)
// REWRITES-NEXT:     }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::ops::Sub for LongDouble {
// REWRITES-NEXT:     type Output = LongDouble;
// REWRITES-NEXT:     fn sub(self, __o: LongDouble) -> LongDouble {
// REWRITES-NEXT:         __slate_f80_sub(self, __o)
// REWRITES-NEXT:     }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::ops::Mul for LongDouble {
// REWRITES-NEXT:     type Output = LongDouble;
// REWRITES-NEXT:     fn mul(self, __o: LongDouble) -> LongDouble {
// REWRITES-NEXT:         __slate_f80_mul(self, __o)
// REWRITES-NEXT:     }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::ops::Div for LongDouble {
// REWRITES-NEXT:     type Output = LongDouble;
// REWRITES-NEXT:     fn div(self, __o: LongDouble) -> LongDouble {
// REWRITES-NEXT:         __slate_f80_div(self, __o)
// REWRITES-NEXT:     }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::ops::AddAssign for LongDouble {
// REWRITES-NEXT:     fn add_assign(&mut self, __o: LongDouble) {
// REWRITES-NEXT:         {
// REWRITES-NEXT:             *self = __slate_f80_add(*self, __o);
// REWRITES-NEXT:         }
// REWRITES-NEXT:     }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::ops::SubAssign for LongDouble {
// REWRITES-NEXT:     fn sub_assign(&mut self, __o: LongDouble) {
// REWRITES-NEXT:         {
// REWRITES-NEXT:             *self = __slate_f80_sub(*self, __o);
// REWRITES-NEXT:         }
// REWRITES-NEXT:     }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::ops::MulAssign for LongDouble {
// REWRITES-NEXT:     fn mul_assign(&mut self, __o: LongDouble) {
// REWRITES-NEXT:         {
// REWRITES-NEXT:             *self = __slate_f80_mul(*self, __o);
// REWRITES-NEXT:         }
// REWRITES-NEXT:     }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::ops::DivAssign for LongDouble {
// REWRITES-NEXT:     fn div_assign(&mut self, __o: LongDouble) {
// REWRITES-NEXT:         {
// REWRITES-NEXT:             *self = __slate_f80_div(*self, __o);
// REWRITES-NEXT:         }
// REWRITES-NEXT:     }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::ops::Neg for LongDouble {
// REWRITES-NEXT:     type Output = LongDouble;
// REWRITES-NEXT:     fn neg(self) -> LongDouble {
// REWRITES-NEXT:         __slate_f80_neg(self)
// REWRITES-NEXT:     }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::cmp::PartialEq for LongDouble {
// REWRITES-NEXT:     fn eq(&self, __other: &LongDouble) -> bool {
// REWRITES-NEXT:         __slate_f80_eq(*self, *__other)
// REWRITES-NEXT:     }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::cmp::PartialOrd for LongDouble {
// REWRITES-NEXT:     fn partial_cmp(&self, __other: &LongDouble) -> Option<std::cmp::Ordering> {
// REWRITES-NEXT:         if __slate_f80_lt(*self, *__other) {
// REWRITES-NEXT:             Some(std::cmp::Ordering::Less)
// REWRITES-NEXT:         } else {
// REWRITES-NEXT:             if __slate_f80_gt(*self, *__other) {
// REWRITES-NEXT:                 Some(std::cmp::Ordering::Greater)
// REWRITES-NEXT:             } else {
// REWRITES-NEXT:                 if __slate_f80_eq(*self, *__other) {
// REWRITES-NEXT:                     Some(std::cmp::Ordering::Equal)
// REWRITES-NEXT:                 } else {
// REWRITES-NEXT:                     None
// REWRITES-NEXT:                 }
// REWRITES-NEXT:             }
// REWRITES-NEXT:         }
// REWRITES-NEXT:     }
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
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct C11Anonymous {
// REWRITES-NEXT:     __slate_anon_0: {{anon_[0-9]+}},
// REWRITES-NEXT:     __slate_anon_1: {{anon_[0-9]+}},
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C, align(32))]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct C11OverAligned {
// REWRITES-NEXT:     value: u8,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct C11Temporary {
// REWRITES-NEXT:     values: [i32; 3],
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct __mbstate_t {
// REWRITES-NEXT:     __count: i32,
// REWRITES-NEXT:     __value: {{anon_[0-9]+}},
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: union {{anon_[0-9]+}} {
// REWRITES-NEXT:     integer: i32,
// REWRITES-NEXT:     __slate_anon_1: f64,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct {{anon_[0-9]+}} {
// REWRITES-NEXT:     x: i32,
// REWRITES-NEXT:     y: i32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: union {{anon_[0-9]+}} {
// REWRITES-NEXT:     __wch: i32,
// REWRITES-NEXT:     __wchb: [i8; 4],
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: union cnd_t {
// REWRITES-NEXT:     __size: [i8; 48],
// REWRITES-NEXT:     __align: i64,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: union mtx_t {
// REWRITES-NEXT:     __size: [i8; 40],
// REWRITES-NEXT:     __align: i64,
// REWRITES-NEXT: }
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
// REWRITES-NEXT:     fn thrd_create(
// REWRITES-NEXT:         _0: *mut u64,
// REWRITES-NEXT:         _1: Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void) -> i32>,
// REWRITES-NEXT:         _2: *mut core::ffi::c_void,
// REWRITES-NEXT:     ) -> i32;
// REWRITES-NEXT:     fn thrd_join(_0: u64, _1: *mut i32) -> i32;
// REWRITES-NEXT:     fn mtx_init(_0: *mut mtx_t, _1: i32) -> i32;
// REWRITES-NEXT:     fn mtx_lock(_0: *mut mtx_t) -> i32;
// REWRITES-NEXT:     fn mtx_unlock(_0: *mut mtx_t) -> i32;
// REWRITES-NEXT:     fn mtx_destroy(_0: *mut mtx_t);
// REWRITES-NEXT:     fn cnd_init(_0: *mut cnd_t) -> i32;
// REWRITES-NEXT:     fn cnd_destroy(_0: *mut cnd_t);
// REWRITES-NEXT:     fn call_once(_0: *mut i32, _1: Option<unsafe extern "C-unwind" fn()>);
// REWRITES-NEXT:     fn tss_create(
// REWRITES-NEXT:         _0: *mut u32,
// REWRITES-NEXT:         _1: Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void)>,
// REWRITES-NEXT:     ) -> i32;
// REWRITES-NEXT:     fn tss_set(_0: u32, _1: *mut core::ffi::c_void) -> i32;
// REWRITES-NEXT:     fn tss_get(_0: u32) -> *mut core::ffi::c_void;
// REWRITES-NEXT:     fn tss_delete(_0: u32);
// REWRITES-NEXT:     fn mbrtoc16(
// REWRITES-NEXT:         _0: *mut u16,
// REWRITES-NEXT:         _1: *const core::ffi::c_char,
// REWRITES-NEXT:         _2: usize,
// REWRITES-NEXT:         _3: *mut __mbstate_t,
// REWRITES-NEXT:     ) -> usize;
// REWRITES-NEXT:     fn c16rtomb(_0: *mut core::ffi::c_char, _1: u16, _2: *mut __mbstate_t) -> usize;
// REWRITES-NEXT:     fn mbrtoc32(
// REWRITES-NEXT:         _0: *mut u32,
// REWRITES-NEXT:         _1: *const core::ffi::c_char,
// REWRITES-NEXT:         _2: usize,
// REWRITES-NEXT:         _3: *mut __mbstate_t,
// REWRITES-NEXT:     ) -> usize;
// REWRITES-NEXT:     fn c32rtomb(_0: *mut core::ffi::c_char, _1: u32, _2: *mut __mbstate_t) -> usize;
// REWRITES-NEXT:     fn aligned_alloc(_0: usize, _1: usize) -> *mut core::ffi::c_void;
// REWRITES-NEXT:     fn free(_0: *mut core::ffi::c_void);
// REWRITES-NEXT:     fn at_quick_exit(_0: Option<unsafe extern "C-unwind" fn()>) -> i32;
// REWRITES-NEXT:     fn remove(_0: *const core::ffi::c_char) -> i32;
// REWRITES-NEXT:     fn fclose(_0: *mut libc::FILE) -> i32;
// REWRITES-NEXT:     fn timespec_get(_0: *mut libc::timespec, _1: i32) -> i32;
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT:     fn putchar(_0: i32) -> i32;
// REWRITES-NEXT:     fn fopen(_0: *const core::ffi::c_char, _1: *const core::ffi::c_char) -> *mut libc::FILE;
// REWRITES-NEXT:     fn quick_exit(_0: i32) -> !;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let mut anonymous: C11Anonymous = C11Anonymous {
// REWRITES-NEXT:         __slate_anon_0: unsafe { std::mem::zeroed::<{{anon_[0-9]+}}>() },
// REWRITES-NEXT:         __slate_anon_1: {{anon_[0-9]+}} { x: 0, y: 0 },
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let mut aligned_object: aligned::Aligned<aligned::A32, C11OverAligned> =
// REWRITES-NEXT:         aligned::Aligned(C11OverAligned { value: 0 });
// REWRITES-NEXT:     let mut utf16_state: __mbstate_t = __mbstate_t {
// REWRITES-NEXT:         __count: 0,
// REWRITES-NEXT:         __value: unsafe { std::mem::zeroed::<{{anon_[0-9]+}}>() },
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let mut utf32_state: __mbstate_t = __mbstate_t {
// REWRITES-NEXT:         __count: 0,
// REWRITES-NEXT:         __value: unsafe { std::mem::zeroed::<{{anon_[0-9]+}}>() },
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let mut converted16: u16 = 0;
// REWRITES-NEXT:     let mut converted32: u32 = 0;
// REWRITES-NEXT:     let mut multibyte16: [i8; 4] = [0; 4];
// REWRITES-NEXT:     let mut multibyte32: [i8; 4] = [0; 4];
// REWRITES-NEXT:     let mut current_time: libc::timespec = libc::timespec {
// REWRITES-NEXT:         tv_sec: 0,
// REWRITES-NEXT:         tv_nsec: 0,
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let mut thread: u64 = 0;
// REWRITES-NEXT:     let mut mutex: mtx_t = unsafe { std::mem::zeroed::<mtx_t>() };
// REWRITES-NEXT:     let mut condition: cnd_t = unsafe { std::mem::zeroed::<cnd_t>() };
// REWRITES-NEXT:     let mut once_control: i32 = 0;
// REWRITES-NEXT:     let mut thread_key: u32 = 0;
// REWRITES-NEXT:     let mut thread_increment: i32 = 7;
// REWRITES-NEXT:     let mut thread_result: i32 = 0;
// REWRITES-NEXT:     let mut aligned_memory: *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-NEXT:     let mut exclusive_first: *mut libc::FILE = std::ptr::null_mut();
// REWRITES-NEXT:     let mut exclusive_second: *mut libc::FILE = std::ptr::null_mut();
// REWRITES-NEXT:     let mut concurrency_total: i32 = 0;
// REWRITES-NEXT:     let mut ref_tmp0: C11Temporary = C11Temporary { values: [0; 3] };
// REWRITES-NEXT:     anonymous = C11Anonymous {
// REWRITES-NEXT:         __slate_anon_0: unsafe { std::mem::zeroed::<{{anon_[0-9]+}}>() },
// REWRITES-NEXT:         __slate_anon_1: {{anon_[0-9]+}} { x: 0, y: 0 },
// REWRITES-NEXT:     };
// REWRITES-NEXT:     *aligned_object = C11OverAligned { value: 0 };
// REWRITES-NEXT:     utf16_state = __mbstate_t {
// REWRITES-NEXT:         __count: 0,
// REWRITES-NEXT:         __value: unsafe { std::mem::zeroed::<{{anon_[0-9]+}}>() },
// REWRITES-NEXT:     };
// REWRITES-NEXT:     utf32_state = __mbstate_t {
// REWRITES-NEXT:         __count: 0,
// REWRITES-NEXT:         __value: unsafe { std::mem::zeroed::<{{anon_[0-9]+}}>() },
// REWRITES-NEXT:     };
// REWRITES-NEXT:     current_time = libc::timespec {
// REWRITES-NEXT:         tv_sec: 0,
// REWRITES-NEXT:         tv_nsec: 0,
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = 4;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = std::mem::align_of::<C11OverAligned>() as i32;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(c11_aligned_buffer).cast::<u8>();
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + ((({{_v[0-9]+}} as u64) % 64 == 0) as i32);
// REWRITES-NEXT:     let {{_v[0-9]+}}: u64 = std::ptr::addr_of_mut!(*aligned_object) as u64;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + (({{_v[0-9]+}} % 32 == 0) as i32);
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = (((unsafe { main_utf8_text[0] }) as u8) as i32)
// REWRITES-NEXT:         + (((unsafe { main_utf8_text[1] }) as u8) as i32)
// REWRITES-NEXT:         + ((unsafe { main_utf16_text[0] }) as i32)
// REWRITES-NEXT:         + ((unsafe { main_utf32_text[0] }) as i32)
// REWRITES-NEXT:         + ((937 as u16) as i32)
// REWRITES-NEXT:         + ((128578 as u32) as i32);
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = 22;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = 11 + {{_v[0-9]+}} + 33;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         anonymous.__slate_anon_0.integer = 31;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     anonymous.__slate_anon_1.x = 37;
// REWRITES-NEXT:     anonymous.__slate_anon_1.y = 41;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = (unsafe { anonymous.__slate_anon_0.integer })
// REWRITES-NEXT:         + anonymous.__slate_anon_1.x
// REWRITES-NEXT:         + anonymous.__slate_anon_1.y;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         c11_evaluation_total = 0;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = c11_evaluation_step(2);
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = c11_evaluation_step(3);
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}} + unsafe { c11_evaluation_total };
// REWRITES-NEXT:     ref_tmp0 = c11_make_temporary(43);
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = ref_tmp0.values[1];
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = 0 + {{_v[0-9]+}} + 1 + 1 + 1 + 1;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         c11_atomic_total = 5;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         c11_thread_local_value = 17;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i32 = unsafe { __errno_location() };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         *{{_v[0-9]+}} = {{_v[0-9]+}};
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void =
// REWRITES-NEXT:         std::ptr::addr_of_mut!(thread_increment) as *mut core::ffi::c_void;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = unsafe {
// REWRITES-NEXT:         thrd_create(
// REWRITES-NEXT:             std::ptr::addr_of_mut!(thread) as *mut u64,
// REWRITES-NEXT:             Some(c11_thread_worker),
// REWRITES-NEXT:             {{_v[0-9]+}} as *mut core::ffi::c_void,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == 0;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} {
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = unsafe {
// REWRITES-NEXT:             thrd_join(
// REWRITES-NEXT:                 thread as u64,
// REWRITES-NEXT:                 std::ptr::addr_of_mut!(thread_result) as *mut i32,
// REWRITES-NEXT:             )
// REWRITES-NEXT:         };
// REWRITES-NEXT:         {{_v[0-9]+}}
// REWRITES-NEXT:     } else {
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = -1;
// REWRITES-NEXT:         {{_v[0-9]+}}
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = unsafe {
// REWRITES-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(c11_atomic_total))
// REWRITES-NEXT:             .load(std::sync::atomic::Ordering::SeqCst)
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = (({{_v[0-9]+}} == 0) as i32)
// REWRITES-NEXT:         + (({{_v[0-9]+}} == 0) as i32)
// REWRITES-NEXT:         + thread_result
// REWRITES-NEXT:         + unsafe { c11_thread_local_value };
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i32 = unsafe { __errno_location() };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + (((unsafe { *{{_v[0-9]+}} }) == 0) as i32);
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { mtx_init(std::ptr::addr_of_mut!(mutex) as *mut mtx_t, 0 as i32) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == 0;
// REWRITES-NEXT:     if {{_v[0-9]+}} {
// REWRITES-NEXT:         concurrency_total += 1;
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = unsafe { mtx_lock(std::ptr::addr_of_mut!(mutex) as *mut mtx_t) };
// REWRITES-NEXT:         concurrency_total += ({{_v[0-9]+}} == 0) as i32;
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = unsafe { mtx_unlock(std::ptr::addr_of_mut!(mutex) as *mut mtx_t) };
// REWRITES-NEXT:         concurrency_total += ({{_v[0-9]+}} == 0) as i32;
// REWRITES-NEXT:         unsafe { mtx_destroy(std::ptr::addr_of_mut!(mutex) as *mut mtx_t) };
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { cnd_init(std::ptr::addr_of_mut!(condition) as *mut cnd_t) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == 0;
// REWRITES-NEXT:     if {{_v[0-9]+}} {
// REWRITES-NEXT:         concurrency_total += 1;
// REWRITES-NEXT:         unsafe { cnd_destroy(std::ptr::addr_of_mut!(condition) as *mut cnd_t) };
// REWRITES-NEXT:     }
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         c11_once_total = 0;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         call_once(
// REWRITES-NEXT:             std::ptr::addr_of_mut!(once_control) as *mut i32,
// REWRITES-NEXT:             Some(c11_once_handler),
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         call_once(
// REWRITES-NEXT:             std::ptr::addr_of_mut!(once_control) as *mut i32,
// REWRITES-NEXT:             Some(c11_once_handler),
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     concurrency_total += unsafe { c11_once_total };
// REWRITES-NEXT:     let {{_v[0-9]+}}: Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void)> = None;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { tss_create(std::ptr::addr_of_mut!(thread_key) as *mut u32, {{_v[0-9]+}}) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == 0;
// REWRITES-NEXT:     if {{_v[0-9]+}} {
// REWRITES-NEXT:         concurrency_total += 1;
// REWRITES-NEXT:         let {{_v[0-9]+}}: u32 = thread_key;
// REWRITES-NEXT:         let {{_v[0-9]+}}: *mut core::ffi::c_void =
// REWRITES-NEXT:             std::ptr::addr_of_mut!(thread_increment) as *mut core::ffi::c_void;
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = unsafe { tss_set({{_v[0-9]+}} as u32, {{_v[0-9]+}} as *mut core::ffi::c_void) };
// REWRITES-NEXT:         concurrency_total += ({{_v[0-9]+}} == 0) as i32;
// REWRITES-NEXT:         let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe { tss_get(thread_key as u32) };
// REWRITES-NEXT:         let {{_v[0-9]+}}: *mut core::ffi::c_void =
// REWRITES-NEXT:             std::ptr::addr_of_mut!(thread_increment) as *mut core::ffi::c_void;
// REWRITES-NEXT:         concurrency_total += ({{_v[0-9]+}} == {{_v[0-9]+}}) as i32;
// REWRITES-NEXT:         unsafe { tss_delete(thread_key as u32) };
// REWRITES-NEXT:     }
// REWRITES-NEXT:     concurrency_total += (4 >= 1) as i32;
// REWRITES-NEXT:     let {{_v[0-9]+}}: u64 = (unsafe {
// REWRITES-NEXT:         mbrtoc16(
// REWRITES-NEXT:             std::ptr::addr_of_mut!(converted16) as *mut u16,
// REWRITES-NEXT:             c"A".as_ptr(),
// REWRITES-NEXT:             (1 as u64) as usize,
// REWRITES-NEXT:             std::ptr::addr_of_mut!(utf16_state) as *mut __mbstate_t,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     }) as u64;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = multibyte16.as_mut_ptr() as *mut i8;
// REWRITES-NEXT:     let {{_v[0-9]+}}: u64 = (unsafe {
// REWRITES-NEXT:         c16rtomb(
// REWRITES-NEXT:             {{_v[0-9]+}} as *mut core::ffi::c_char,
// REWRITES-NEXT:             65 as u16,
// REWRITES-NEXT:             std::ptr::addr_of_mut!(utf16_state) as *mut __mbstate_t,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     }) as u64;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + ({{_v[0-9]+}} as i32);
// REWRITES-NEXT:     let {{_v[0-9]+}}: u64 = (unsafe {
// REWRITES-NEXT:         mbrtoc32(
// REWRITES-NEXT:             std::ptr::addr_of_mut!(converted32) as *mut u32,
// REWRITES-NEXT:             c"B".as_ptr(),
// REWRITES-NEXT:             (1 as u64) as usize,
// REWRITES-NEXT:             std::ptr::addr_of_mut!(utf32_state) as *mut __mbstate_t,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     }) as u64;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + ({{_v[0-9]+}} as i32);
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = multibyte32.as_mut_ptr() as *mut i8;
// REWRITES-NEXT:     let {{_v[0-9]+}}: u64 = (unsafe {
// REWRITES-NEXT:         c32rtomb(
// REWRITES-NEXT:             {{_v[0-9]+}} as *mut core::ffi::c_char,
// REWRITES-NEXT:             66 as u32,
// REWRITES-NEXT:             std::ptr::addr_of_mut!(utf32_state) as *mut __mbstate_t,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     }) as u64;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}}
// REWRITES-NEXT:         + ({{_v[0-9]+}} as i32)
// REWRITES-NEXT:         + (converted16 as i32)
// REWRITES-NEXT:         + (converted32 as i32)
// REWRITES-NEXT:         + (multibyte16[0] as i32)
// REWRITES-NEXT:         + (multibyte32[0] as i32);
// REWRITES-NEXT:     aligned_memory = unsafe { aligned_alloc((64 as u64) as usize, (64 as u64) as usize) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = if aligned_memory != std::ptr::null_mut() {
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = (aligned_memory as u64) % 64 == 0;
// REWRITES-NEXT:         {{_v[0-9]+}}
// REWRITES-NEXT:     } else {
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = false;
// REWRITES-NEXT:         {{_v[0-9]+}}
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + ({{_v[0-9]+}} as i32);
// REWRITES-NEXT:     unsafe { free(aligned_memory as *mut core::ffi::c_void) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { at_quick_exit(Some(c11_quick_handler)) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = ({{_v[0-9]+}} == 0) as i32;
// REWRITES-NEXT:     unsafe { remove(c"slate-c11-exclusive.tmp".as_ptr()) };
// REWRITES-NEXT:     exclusive_first = c11_open_exclusive(c"slate-c11-exclusive.tmp".as_ptr() as *mut i8);
// REWRITES-NEXT:     exclusive_second = c11_open_exclusive(c"slate-c11-exclusive.tmp".as_ptr() as *mut i8);
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = if exclusive_first != std::ptr::null_mut() {
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = exclusive_second == std::ptr::null_mut();
// REWRITES-NEXT:         {{_v[0-9]+}}
// REWRITES-NEXT:     } else {
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = false;
// REWRITES-NEXT:         {{_v[0-9]+}}
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = exclusive_first != std::ptr::null_mut();
// REWRITES-NEXT:     if {{_v[0-9]+}} {
// REWRITES-NEXT:         unsafe { fclose(exclusive_first as *mut libc::FILE) };
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = exclusive_second != std::ptr::null_mut();
// REWRITES-NEXT:     if {{_v[0-9]+}} {
// REWRITES-NEXT:         unsafe { fclose(exclusive_second as *mut libc::FILE) };
// REWRITES-NEXT:     }
// REWRITES-NEXT:     unsafe { remove(c"slate-c11-exclusive.tmp".as_ptr()) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = unsafe {
// REWRITES-NEXT:         timespec_get(
// REWRITES-NEXT:             std::ptr::addr_of_mut!(current_time) as *mut libc::timespec,
// REWRITES-NEXT:             1 as i32,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} == 1 {
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = current_time.tv_nsec >= 0;
// REWRITES-NEXT:         {{_v[0-9]+}}
// REWRITES-NEXT:     } else {
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = false;
// REWRITES-NEXT:         {{_v[0-9]+}}
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = current_time.tv_nsec < 1000000000;
// REWRITES-NEXT:         {{_v[0-9]+}}
// REWRITES-NEXT:     } else {
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = false;
// REWRITES-NEXT:         {{_v[0-9]+}}
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex { re: 2.0, im: 3.0 };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = (({{_v[0-9]+}}.re == 2.0) as i32) + (({{_v[0-9]+}}.im == 3.0) as i32);
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = (((9 as i32) >= 6) as i32) + (((17 as i32) >= 10) as i32) + (((21 as i32) >= 10) as i32) + ((0.0000000000000000000000000000000000000000000014013 > (0.0 as f32)) as i32) + ((0.000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000005 > (0.0 as f64)) as i32) + ((LongDouble([1, 0, 0, 0, 0, 0, 0, 0, 0, 0]) > LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0])) as i32) + ((1 >= -1) as i32) + ((1 >= -1) as i32) + ((1 >= -1) as i32) + 0;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(c11_never_flag)) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// REWRITES-NEXT:     if {{_v[0-9]+}} {
// REWRITES-NEXT:         c11_never_return(99);
// REWRITES-NEXT:     }
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             c"%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d".as_ptr(),
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:             1 as i32,
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:             concurrency_total,
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:             0 as i32,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     unsafe { putchar(10 as i32) };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn c11_evaluation_step({{arg[0-9]+}}: i32) -> i32 {
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         c11_evaluation_total = (unsafe { c11_evaluation_total }) + {{arg[0-9]+}};
// REWRITES-NEXT:     }
// REWRITES-NEXT:     {{arg[0-9]+}} * 2
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn c11_make_temporary({{arg[0-9]+}}: i32) -> C11Temporary {
// REWRITES-NEXT:     let mut __retval: C11Temporary = C11Temporary { values: [0; 3] };
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i32 = std::ptr::addr_of_mut!(__retval.values) as *mut i32;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         *{{_v[0-9]+}} = {{arg[0-9]+}};
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.add(1) };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         *{{_v[0-9]+}} = {{arg[0-9]+}} + 1;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.add(2) };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         *{{_v[0-9]+}} = {{arg[0-9]+}} + 2;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: extern "C-unwind" fn c11_thread_worker({{arg[0-9]+}}: *mut core::ffi::c_void) -> i32 {
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { *({{arg[0-9]+}} as *mut i32) };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         c11_thread_local_value = 29;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = 34;
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i32 = unsafe { __errno_location() };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         *{{_v[0-9]+}} = {{_v[0-9]+}};
// REWRITES-NEXT:     }
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(c11_atomic_total))
// REWRITES-NEXT:             .fetch_add({{_v[0-9]+}}, std::sync::atomic::Ordering::SeqCst)
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { c11_thread_local_value };
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i32 = unsafe { __errno_location() };
// REWRITES-NEXT:     {{_v[0-9]+}} + (((unsafe { *{{_v[0-9]+}} }) == 34) as i32)
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: extern "C-unwind" fn c11_once_handler() {
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         c11_once_total = (unsafe { c11_once_total }) + 1;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: extern "C-unwind" fn c11_quick_handler() {
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = 100;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(c11_atomic_total))
// REWRITES-NEXT:             .fetch_add({{_v[0-9]+}}, std::sync::atomic::Ordering::Relaxed)
// REWRITES-NEXT:     };
// REWRITES-NEXT:     return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn c11_open_exclusive({{arg[0-9]+}}: *mut i8) -> *mut libc::FILE {
// REWRITES-NEXT:     unsafe { fopen({{arg[0-9]+}} as *const core::ffi::c_char, c"wx".as_ptr()) }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn c11_never_return({{arg[0-9]+}}: i32) -> ! {
// REWRITES-NEXT:     unsafe { quick_exit({{arg[0-9]+}} as i32) }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     safe fn __slate_cf80_div(
// REWRITES-NEXT:         __a: num_complex::Complex<LongDouble>,
// REWRITES-NEXT:         __b: num_complex::Complex<LongDouble>,
// REWRITES-NEXT:     ) -> num_complex::Complex<LongDouble>;
// REWRITES-NEXT:     safe fn __slate_cf80_mul(
// REWRITES-NEXT:         __a: num_complex::Complex<LongDouble>,
// REWRITES-NEXT:         __b: num_complex::Complex<LongDouble>,
// REWRITES-NEXT:     ) -> num_complex::Complex<LongDouble>;
// REWRITES-NEXT:     safe fn __slate_f80_abs(__a: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_add(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_ceil(__a: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_copysign(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_div(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_eq(__a: LongDouble, __b: LongDouble) -> bool;
// REWRITES-NEXT:     safe fn __slate_f80_floor(__a: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_fma(__a: LongDouble, __b: LongDouble, __c: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_fmax(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_fmin(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_fract(__a: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_from_bool(__a: bool) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_from_f32(__a: f32) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_from_f64(__a: f64) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_from_i128(__a: i128) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_from_i16(__a: i16) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_from_i32(__a: i32) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_from_i64(__a: i64) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_from_i8(__a: i8) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_from_u128(__a: u128) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_from_u16(__a: u16) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_from_u32(__a: u32) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_from_u64(__a: u64) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_from_u8(__a: u8) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_ge(__a: LongDouble, __b: LongDouble) -> bool;
// REWRITES-NEXT:     safe fn __slate_f80_gt(__a: LongDouble, __b: LongDouble) -> bool;
// REWRITES-NEXT:     safe fn __slate_f80_is_fp_class(__a: LongDouble, __flags: i32) -> bool;
// REWRITES-NEXT:     safe fn __slate_f80_le(__a: LongDouble, __b: LongDouble) -> bool;
// REWRITES-NEXT:     safe fn __slate_f80_lt(__a: LongDouble, __b: LongDouble) -> bool;
// REWRITES-NEXT:     safe fn __slate_f80_mul(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_ne(__a: LongDouble, __b: LongDouble) -> bool;
// REWRITES-NEXT:     safe fn __slate_f80_neg(__a: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_rint(__a: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_round(__a: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_signbit(__a: LongDouble) -> bool;
// REWRITES-NEXT:     safe fn __slate_f80_sub(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_to_bool(__a: LongDouble) -> bool;
// REWRITES-NEXT:     safe fn __slate_f80_to_f32(__a: LongDouble) -> f32;
// REWRITES-NEXT:     safe fn __slate_f80_to_f64(__a: LongDouble) -> f64;
// REWRITES-NEXT:     safe fn __slate_f80_to_i128(__a: LongDouble) -> i128;
// REWRITES-NEXT:     safe fn __slate_f80_to_i16(__a: LongDouble) -> i16;
// REWRITES-NEXT:     safe fn __slate_f80_to_i32(__a: LongDouble) -> i32;
// REWRITES-NEXT:     safe fn __slate_f80_to_i64(__a: LongDouble) -> i64;
// REWRITES-NEXT:     safe fn __slate_f80_to_i8(__a: LongDouble) -> i8;
// REWRITES-NEXT:     safe fn __slate_f80_to_u128(__a: LongDouble) -> u128;
// REWRITES-NEXT:     safe fn __slate_f80_to_u16(__a: LongDouble) -> u16;
// REWRITES-NEXT:     safe fn __slate_f80_to_u32(__a: LongDouble) -> u32;
// REWRITES-NEXT:     safe fn __slate_f80_to_u64(__a: LongDouble) -> u64;
// REWRITES-NEXT:     safe fn __slate_f80_to_u8(__a: LongDouble) -> u8;
// REWRITES-NEXT:     safe fn __slate_f80_trunc(__a: LongDouble) -> LongDouble;
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
