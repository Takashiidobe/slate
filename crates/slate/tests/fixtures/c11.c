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

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![feature(thread_local)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn cimag(__z: num_complex::Complex<f64>) -> f64;
// CHECK-NEXT:     fn creal(__z: num_complex::Complex<f64>) -> f64;
// CHECK-NEXT:     fn __errno_location() -> *mut i32;
// CHECK-NEXT:     fn remove(__filename: *const i8) -> i32;
// CHECK-NEXT:     fn fclose(__stream: *mut _IO_FILE) -> i32;
// CHECK-NEXT:     fn fopen(__filename: *const i8, __modes: *const i8) -> *mut _IO_FILE;
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT:     fn putchar(__c: i32) -> i32;
// CHECK-NEXT:     fn free(__ptr: *mut ());
// CHECK-NEXT:     fn aligned_alloc(__alignment: u64, __size: u64) -> *mut ();
// CHECK-NEXT:     fn at_quick_exit(__func: Option<unsafe extern "C-unwind" fn()>) -> i32;
// CHECK-NEXT:     fn quick_exit(__status: i32);
// CHECK-NEXT:     fn call_once(
// CHECK-NEXT:         __flag: *mut [[SLATERECORD_0:__SlateRecord[0-9]+]],
// CHECK-NEXT:         __func: Option<unsafe extern "C-unwind" fn()>,
// CHECK-NEXT:     );
// CHECK-NEXT:     fn timespec_get(__ts: *mut timespec, __base: i32) -> i32;
// CHECK-NEXT:     fn thrd_create(
// CHECK-NEXT:         __thr: *mut u64,
// CHECK-NEXT:         __func: Option<unsafe extern "C-unwind" fn(*mut ()) -> i32>,
// CHECK-NEXT:         __arg: *mut (),
// CHECK-NEXT:     ) -> i32;
// CHECK-NEXT:     fn thrd_join(__thr: u64, __res: *mut i32) -> i32;
// CHECK-NEXT:     fn mtx_init(__mutex: *mut [[SLATERECORD_1:__SlateRecord[0-9]+]], __type: i32) -> i32;
// CHECK-NEXT:     fn mtx_lock(__mutex: *mut [[SLATERECORD_1]]) -> i32;
// CHECK-NEXT:     fn mtx_unlock(__mutex: *mut [[SLATERECORD_1]]) -> i32;
// CHECK-NEXT:     fn mtx_destroy(__mutex: *mut [[SLATERECORD_1]]);
// CHECK-NEXT:     fn cnd_init(__cond: *mut [[SLATERECORD_2:__SlateRecord[0-9]+]]) -> i32;
// CHECK-NEXT:     fn cnd_destroy(__COND: *mut [[SLATERECORD_2]]);
// CHECK-NEXT:     fn tss_create(
// CHECK-NEXT:         __tss_id: *mut u32,
// CHECK-NEXT:         __destructor: Option<unsafe extern "C-unwind" fn(*mut ())>,
// CHECK-NEXT:     ) -> i32;
// CHECK-NEXT:     fn tss_get(__tss_id: u32) -> *mut ();
// CHECK-NEXT:     fn tss_set(__tss_id: u32, __val: *mut ()) -> i32;
// CHECK-NEXT:     fn tss_delete(__tss_id: u32);
// CHECK-NEXT:     fn mbrtoc16(
// CHECK-NEXT:         __pc16: *mut u16,
// CHECK-NEXT:         __s: *const i8,
// CHECK-NEXT:         __n: u64,
// CHECK-NEXT:         __p: *mut [[SLATERECORD_3:__SlateRecord[0-9]+]],
// CHECK-NEXT:     ) -> u64;
// CHECK-NEXT:     fn c16rtomb(__s: *mut i8, __c16: u16, __ps: *mut [[SLATERECORD_3]]) -> u64;
// CHECK-NEXT:     fn mbrtoc32(
// CHECK-NEXT:         __pc32: *mut u32,
// CHECK-NEXT:         __s: *const i8,
// CHECK-NEXT:         __n: u64,
// CHECK-NEXT:         __p: *mut [[SLATERECORD_3]],
// CHECK-NEXT:     ) -> u64;
// CHECK-NEXT:     fn c32rtomb(__s: *mut i8, __c32: u32, __ps: *mut [[SLATERECORD_3]]) -> u64;
// CHECK-NEXT:     safe fn __slate_f80_add(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_sub(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_mul(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_div(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_copysign(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_fmax(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_fmin(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_powi(__a: LongDouble, __n: i32) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_neg(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     fn __slate_f80_va_arg(__ap: *mut core::ffi::VaList<'static>) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_abs(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_ceil(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_floor(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_sqrt(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_cbrt(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_exp(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_exp2(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_expm1(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_log(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_log2(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_log10(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_log1p(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_sin(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_cos(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_tan(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_asin(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_acos(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_atan(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_sinh(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_cosh(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_tanh(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_asinh(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_acosh(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_atanh(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_nearbyint(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_fract(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_round(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_trunc(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_rint(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_fmod(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_remainder(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_pow(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_fdim(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_hypot(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_signbit(__a: LongDouble) -> bool;
// CHECK-NEXT:     safe fn __slate_f80_is_fp_class(__a: LongDouble, __flags: i32) -> bool;
// CHECK-NEXT:     safe fn __slate_f80_fma(
// CHECK-NEXT:         __a: LongDouble,
// CHECK-NEXT:         __b: LongDouble,
// CHECK-NEXT:         __c: LongDouble,
// CHECK-NEXT:     ) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_lt(__a: LongDouble, __b: LongDouble) -> bool;
// CHECK-NEXT:     safe fn __slate_f80_le(__a: LongDouble, __b: LongDouble) -> bool;
// CHECK-NEXT:     safe fn __slate_f80_gt(__a: LongDouble, __b: LongDouble) -> bool;
// CHECK-NEXT:     safe fn __slate_f80_ge(__a: LongDouble, __b: LongDouble) -> bool;
// CHECK-NEXT:     safe fn __slate_f80_eq(__a: LongDouble, __b: LongDouble) -> bool;
// CHECK-NEXT:     safe fn __slate_f80_ne(__a: LongDouble, __b: LongDouble) -> bool;
// CHECK-NEXT:     safe fn __slate_f80_from_i8(__a: i8) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_u8(__a: u8) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_i16(__a: i16) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_u16(__a: u16) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_i32(__a: i32) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_u32(__a: u32) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_i64(__a: i64) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_u64(__a: u64) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_i128(__a: i128) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_u128(__a: u128) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_f32(__a: f32) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_f64(__a: f64) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_bool(__a: bool) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_to_i8(__a: LongDouble) -> i8;
// CHECK-NEXT:     safe fn __slate_f80_to_u8(__a: LongDouble) -> u8;
// CHECK-NEXT:     safe fn __slate_f80_to_i16(__a: LongDouble) -> i16;
// CHECK-NEXT:     safe fn __slate_f80_to_u16(__a: LongDouble) -> u16;
// CHECK-NEXT:     safe fn __slate_f80_to_i32(__a: LongDouble) -> i32;
// CHECK-NEXT:     safe fn __slate_f80_to_u32(__a: LongDouble) -> u32;
// CHECK-NEXT:     safe fn __slate_f80_to_i64(__a: LongDouble) -> i64;
// CHECK-NEXT:     safe fn __slate_f80_to_u64(__a: LongDouble) -> u64;
// CHECK-NEXT:     safe fn __slate_f80_to_i128(__a: LongDouble) -> i128;
// CHECK-NEXT:     safe fn __slate_f80_to_u128(__a: LongDouble) -> u128;
// CHECK-NEXT:     safe fn __slate_f80_to_f32(__a: LongDouble) -> f32;
// CHECK-NEXT:     safe fn __slate_f80_to_f64(__a: LongDouble) -> f64;
// CHECK-NEXT:     safe fn __slate_f80_to_bool(__a: LongDouble) -> bool;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct LongDouble([u8; 10]);
// CHECK-NEXT: impl core::ops::Add for LongDouble {
// CHECK-NEXT:     type Output = LongDouble;
// CHECK-NEXT:     fn add(self, __o: LongDouble) -> LongDouble {
// CHECK-NEXT:         __slate_f80_add(self, __o)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: impl core::ops::Sub for LongDouble {
// CHECK-NEXT:     type Output = LongDouble;
// CHECK-NEXT:     fn sub(self, __o: LongDouble) -> LongDouble {
// CHECK-NEXT:         __slate_f80_sub(self, __o)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: impl core::ops::Mul for LongDouble {
// CHECK-NEXT:     type Output = LongDouble;
// CHECK-NEXT:     fn mul(self, __o: LongDouble) -> LongDouble {
// CHECK-NEXT:         __slate_f80_mul(self, __o)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: impl core::ops::Div for LongDouble {
// CHECK-NEXT:     type Output = LongDouble;
// CHECK-NEXT:     fn div(self, __o: LongDouble) -> LongDouble {
// CHECK-NEXT:         __slate_f80_div(self, __o)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: impl core::ops::AddAssign for LongDouble {
// CHECK-NEXT:     fn add_assign(&mut self, __o: LongDouble) {
// CHECK-NEXT:         {
// CHECK-NEXT:             *self = __slate_f80_add(*self, __o);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: impl core::ops::SubAssign for LongDouble {
// CHECK-NEXT:     fn sub_assign(&mut self, __o: LongDouble) {
// CHECK-NEXT:         {
// CHECK-NEXT:             *self = __slate_f80_sub(*self, __o);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: impl core::ops::MulAssign for LongDouble {
// CHECK-NEXT:     fn mul_assign(&mut self, __o: LongDouble) {
// CHECK-NEXT:         {
// CHECK-NEXT:             *self = __slate_f80_mul(*self, __o);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: impl core::ops::DivAssign for LongDouble {
// CHECK-NEXT:     fn div_assign(&mut self, __o: LongDouble) {
// CHECK-NEXT:         {
// CHECK-NEXT:             *self = __slate_f80_div(*self, __o);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: impl core::ops::Neg for LongDouble {
// CHECK-NEXT:     type Output = LongDouble;
// CHECK-NEXT:     fn neg(self) -> LongDouble {
// CHECK-NEXT:         __slate_f80_neg(self)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: impl core::cmp::PartialEq for LongDouble {
// CHECK-NEXT:     fn eq(&self, __other: &LongDouble) -> bool {
// CHECK-NEXT:         __slate_f80_eq(*self, *__other)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: impl core::cmp::PartialOrd for LongDouble {
// CHECK-NEXT:     fn partial_cmp(&self, __other: &LongDouble) -> Option<std::cmp::Ordering> {
// CHECK-NEXT:         if __slate_f80_lt(*self, *__other) {
// CHECK-NEXT:             Some(std::cmp::Ordering::Less)
// CHECK-NEXT:         } else {
// CHECK-NEXT:             if __slate_f80_gt(*self, *__other) {
// CHECK-NEXT:                 Some(std::cmp::Ordering::Greater)
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 if __slate_f80_eq(*self, *__other) {
// CHECK-NEXT:                     Some(std::cmp::Ordering::Equal)
// CHECK-NEXT:                 } else {
// CHECK-NEXT:                     None
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_3]] {
// CHECK-NEXT:     __count: i32,
// CHECK-NEXT:     __value: [[SLATERECORD_4:__SlateRecord[0-9]+]],
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union [[SLATERECORD_4]] {
// CHECK-NEXT:     __wch: u32,
// CHECK-NEXT:     __wchb: [i8; 4],
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct _IO_FILE {
// CHECK-NEXT:     _flags: i32,
// CHECK-NEXT:     _IO_read_ptr: *mut i8,
// CHECK-NEXT:     _IO_read_end: *mut i8,
// CHECK-NEXT:     _IO_read_base: *mut i8,
// CHECK-NEXT:     _IO_write_base: *mut i8,
// CHECK-NEXT:     _IO_write_ptr: *mut i8,
// CHECK-NEXT:     _IO_write_end: *mut i8,
// CHECK-NEXT:     _IO_buf_base: *mut i8,
// CHECK-NEXT:     _IO_buf_end: *mut i8,
// CHECK-NEXT:     _IO_save_base: *mut i8,
// CHECK-NEXT:     _IO_backup_base: *mut i8,
// CHECK-NEXT:     _IO_save_end: *mut i8,
// CHECK-NEXT:     _markers: *mut _IO_marker,
// CHECK-NEXT:     _chain: *mut _IO_FILE,
// CHECK-NEXT:     _fileno: i32,
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits16U0,
// CHECK-NEXT:     _short_backupbuf: [i8; 1],
// CHECK-NEXT:     _old_offset: i64,
// CHECK-NEXT:     _cur_column: u16,
// CHECK-NEXT:     _vtable_offset: i8,
// CHECK-NEXT:     _shortbuf: [i8; 1],
// CHECK-NEXT:     _lock: *mut (),
// CHECK-NEXT:     _offset: i64,
// CHECK-NEXT:     _codecvt: *mut _IO_codecvt,
// CHECK-NEXT:     _wide_data: *mut _IO_wide_data,
// CHECK-NEXT:     _freeres_list: *mut _IO_FILE,
// CHECK-NEXT:     _freeres_buf: *mut (),
// CHECK-NEXT:     _prevchain: *mut *mut _IO_FILE,
// CHECK-NEXT:     _mode: i32,
// CHECK-NEXT:     _unused3: i32,
// CHECK-NEXT:     _total_written: u64,
// CHECK-NEXT:     _unused2: [i8; 8],
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct _IO_marker {}
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct _IO_codecvt {}
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct _IO_wide_data {}
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_0]] {
// CHECK-NEXT:     __data: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct timespec {
// CHECK-NEXT:     tv_sec: i64,
// CHECK-NEXT:     tv_nsec: i64,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union [[SLATERECORD_1]] {
// CHECK-NEXT:     __size: [i8; 40],
// CHECK-NEXT:     __align: i64,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union [[SLATERECORD_2]] {
// CHECK-NEXT:     __size: [i8; 48],
// CHECK-NEXT:     __align: i64,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct C11Anonymous {
// CHECK-NEXT:     [[SLATE_ANON_0:__slate_anon_[0-9]+]]: [[SLATERECORD_5:__SlateRecord[0-9]+]],
// CHECK-NEXT:     [[SLATE_ANON_1:__slate_anon_[0-9]+]]: [[SLATERECORD_6:__SlateRecord[0-9]+]],
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union [[SLATERECORD_5]] {
// CHECK-NEXT:     integer: i32,
// CHECK-NEXT:     real: f64,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_6]] {
// CHECK-NEXT:     x: i32,
// CHECK-NEXT:     y: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(32))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct C11OverAligned {
// CHECK-NEXT:     value: u8,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct C11Temporary {
// CHECK-NEXT:     values: [i32; 3],
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: #[repr(C, align(64))]
// CHECK-NEXT: struct __SlateAlign64<T>(T);
// CHECK-NEXT: mod __slate_bits {
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         [u8;3],
// CHECK-NEXT:         c_names = true,
// CHECK-NEXT:         new = false,
// CHECK-NEXT:         from_into_bits = false,
// CHECK-NEXT:         from_traits = false,
// CHECK-NEXT:         default = false,
// CHECK-NEXT:         debug = false,
// CHECK-NEXT:         builder = false,
// CHECK-NEXT:         bit_ops = false
// CHECK-NEXT:     )]
// CHECK-NEXT:     pub struct __SlateBits16U0 {
// CHECK-NEXT:         #[bits(24)]
// CHECK-NEXT:         pub _flags2: i32,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: static mut c11_aligned_buffer: __SlateAlign64<[u8; 64]> = __SlateAlign64(unsafe {
// CHECK-NEXT:     std::mem::zeroed()
// CHECK-NEXT: });
// CHECK-NEXT: static mut c11_atomic_total: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: #[thread_local]
// CHECK-NEXT: static mut c11_thread_local_value: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut c11_evaluation_total: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut c11_never_flag: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut c11_once_total: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn c11_make_temporary(base: i32) -> C11Temporary {
// CHECK-NEXT:     let result: C11Temporary = C11Temporary {
// CHECK-NEXT:         values: [base, base + 1, base + 2],
// CHECK-NEXT:     };
// CHECK-NEXT:     return result;
// CHECK-NEXT: }
// CHECK-NEXT: fn c11_evaluation_step(value: i32) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         c11_evaluation_total = c11_evaluation_total + value;
// CHECK-NEXT:     }
// CHECK-NEXT:     value * 2
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.c11_thread_worker")]
// CHECK-NEXT: extern "C-unwind" fn c11_thread_worker(argument: *mut ()) -> i32 {
// CHECK-NEXT:     let increment: i32 = unsafe { *(argument as *mut i32) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         c11_thread_local_value = 29;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *__errno_location() = 34;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]] = increment;
// CHECK-NEXT:         let [[T_1:__t[0-9]+]] = std::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(c11_atomic_total) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_add([[T_0]], std::sync::atomic::Ordering::SeqCst);
// CHECK-NEXT:         [[T_1]]
// CHECK-NEXT:     };
// CHECK-NEXT:     (unsafe { c11_thread_local_value })
// CHECK-NEXT:         + (((unsafe { *__errno_location() }) == 34) as i32)
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.c11_quick_handler")]
// CHECK-NEXT: extern "C-unwind" fn c11_quick_handler() {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]] = 100i32;
// CHECK-NEXT:         let [[T_1]] = std::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(c11_atomic_total) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_add([[T_0]], std::sync::atomic::Ordering::Relaxed);
// CHECK-NEXT:         [[T_1]]
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.c11_once_handler")]
// CHECK-NEXT: extern "C-unwind" fn c11_once_handler() {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         c11_once_total = c11_once_total + 1;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn c11_never_return(status: i32) {
// CHECK-NEXT:     unsafe { quick_exit(status) };
// CHECK-NEXT: }
// CHECK-NEXT: fn c11_open_exclusive(path: *const i8) -> *mut _IO_FILE {
// CHECK-NEXT:     unsafe { fopen(path, c"wx".as_ptr()) }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0:__slate_compound_[0-9]+]]: C11Temporary = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let utf16_character: u16 = 937;
// CHECK-NEXT:     let utf32_character: u32 = 128578;
// CHECK-NEXT:     let mut anonymous: C11Anonymous = {
// CHECK-NEXT:         let mut [[T_1]]: C11Anonymous = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_1]].[[SLATE_ANON_0]] = {
// CHECK-NEXT:             let mut [[T_0]]: [[SLATERECORD_5]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:             [[T_0]].integer = 0i32;
// CHECK-NEXT:             [[T_0]]
// CHECK-NEXT:         };
// CHECK-NEXT:         [[T_1]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut aligned_object: C11OverAligned = {
// CHECK-NEXT:         let mut [[T_2:__t[0-9]+]]: C11OverAligned = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_2]].value = 0u8;
// CHECK-NEXT:         [[T_2]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut utf16_state: [[SLATERECORD_3]] = {
// CHECK-NEXT:         let mut [[T_3:__t[0-9]+]]: [[SLATERECORD_3]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_3]].__count = 0i32;
// CHECK-NEXT:         [[T_3]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut utf32_state: [[SLATERECORD_3]] = {
// CHECK-NEXT:         let mut [[T_4:__t[0-9]+]]: [[SLATERECORD_3]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_4]].__count = 0i32;
// CHECK-NEXT:         [[T_4]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut converted16: u16 = 0;
// CHECK-NEXT:     let mut converted32: u32 = 0;
// CHECK-NEXT:     let mut multibyte16: __SlateAlign16<[i8; 16]> = __SlateAlign16([0i8; 16]);
// CHECK-NEXT:     let mut multibyte32: __SlateAlign16<[i8; 16]> = __SlateAlign16([0i8; 16]);
// CHECK-NEXT:     let mut current_time: timespec = timespec { tv_sec: 0, tv_nsec: 0 };
// CHECK-NEXT:     let mut thread: u64 = 0;
// CHECK-NEXT:     let mut mutex: [[SLATERECORD_1]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut condition: [[SLATERECORD_2]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut once_control: [[SLATERECORD_0]] = [[SLATERECORD_0]] { __data: 0 };
// CHECK-NEXT:     let mut thread_key: u32 = 0;
// CHECK-NEXT:     let mut thread_increment: i32 = 7;
// CHECK-NEXT:     let mut thread_result: i32 = 0;
// CHECK-NEXT:     let mut thread_created: i32 = 0;
// CHECK-NEXT:     let mut thread_joined: i32 = 0;
// CHECK-NEXT:     let mut alignment_total: i32 = 4 + 32
// CHECK-NEXT:         + ((((unsafe { std::ptr::addr_of_mut!(c11_aligned_buffer.0) as *mut u8 }) as u64)
// CHECK-NEXT:             % 64 == 0) as i32)
// CHECK-NEXT:         + (((std::ptr::addr_of_mut!(aligned_object) as u64) % 32 == 0) as i32);
// CHECK-NEXT:     let unicode_total: i32 = ((((unsafe { utf8_text[0] }) as u8) as u32) as i32)
// CHECK-NEXT:         + ((((unsafe { utf8_text[1] }) as u8) as u32) as i32)
// CHECK-NEXT:         + (((unsafe { utf16_text[0] }) as u32) as i32)
// CHECK-NEXT:         + ((unsafe { utf32_text[0] }) as i32) + ((utf16_character as u32) as i32)
// CHECK-NEXT:         + (utf32_character as i32);
// CHECK-NEXT:     let generic_total: i32 = 11 + 22 + 33;
// CHECK-NEXT:     let mut anonymous_total: i32 = 0;
// CHECK-NEXT:     let mut evaluation_total: i32 = 0;
// CHECK-NEXT:     let mut temporary_total: i32 = 0;
// CHECK-NEXT:     let static_assert_total: i32 = 1;
// CHECK-NEXT:     let optional_total: i32 = 0 + 0 + 1 + 1 + 1 + 1;
// CHECK-NEXT:     let mut atomic_total: i32 = 0;
// CHECK-NEXT:     let mut thread_total: i32 = 0;
// CHECK-NEXT:     let mut concurrency_total: i32 = 0;
// CHECK-NEXT:     let mut conversion_total: i32 = 0;
// CHECK-NEXT:     let mut quick_total: i32 = 0;
// CHECK-NEXT:     let mut exclusive_total: i32 = 0;
// CHECK-NEXT:     let mut timespec_total: i32 = 0;
// CHECK-NEXT:     let mut complex_total: i32 = 0;
// CHECK-NEXT:     let mut limits_total: i32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         anonymous.[[SLATE_ANON_0]].integer = 31;
// CHECK-NEXT:     }
// CHECK-NEXT:     anonymous.[[SLATE_ANON_1]].x = 37;
// CHECK-NEXT:     anonymous.[[SLATE_ANON_1]].y = 41;
// CHECK-NEXT:     anonymous_total = (unsafe { anonymous.[[SLATE_ANON_0]].integer })
// CHECK-NEXT:         + anonymous.[[SLATE_ANON_1]].x + anonymous.[[SLATE_ANON_1]].y;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         c11_evaluation_total = 0;
// CHECK-NEXT:     }
// CHECK-NEXT:     evaluation_total = c11_evaluation_step(2i32) + c11_evaluation_step(3i32)
// CHECK-NEXT:         + unsafe { c11_evaluation_total };
// CHECK-NEXT:     temporary_total = unsafe {
// CHECK-NEXT:         *((*{
// CHECK-NEXT:             [[SLATE_COMPOUND_0]] = c11_make_temporary(43i32);
// CHECK-NEXT:             std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:         })
// CHECK-NEXT:             .values
// CHECK-NEXT:             .as_mut_ptr() as *mut i32)
// CHECK-NEXT:             .add(1)
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *std::ptr::addr_of_mut!(c11_atomic_total) = 5;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         c11_thread_local_value = 17;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *__errno_location() = 0;
// CHECK-NEXT:     }
// CHECK-NEXT:     thread_created = unsafe {
// CHECK-NEXT:         thrd_create(
// CHECK-NEXT:             std::ptr::addr_of_mut!(thread),
// CHECK-NEXT:             Some(c11_thread_worker),
// CHECK-NEXT:             std::ptr::addr_of_mut!(thread_increment) as *mut (),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: i32;
// CHECK-NEXT:     if thread_created == 0 {
// CHECK-NEXT:         [[V_0]] = unsafe { thrd_join(thread, std::ptr::addr_of_mut!(thread_result)) };
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_0]] = -1;
// CHECK-NEXT:     }
// CHECK-NEXT:     thread_joined = [[V_0]];
// CHECK-NEXT:     atomic_total = unsafe {
// CHECK-NEXT:         std::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(c11_atomic_total) as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .load(std::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:     };
// CHECK-NEXT:     thread_total = ((thread_created == 0) as i32) + ((thread_joined == 0) as i32)
// CHECK-NEXT:         + thread_result + unsafe { c11_thread_local_value }
// CHECK-NEXT:         + (((unsafe { *__errno_location() }) == 0) as i32);
// CHECK-NEXT:     if (unsafe { mtx_init(std::ptr::addr_of_mut!(mutex), 0i32) }) == 0 {
// CHECK-NEXT:         concurrency_total += 1;
// CHECK-NEXT:         let [[V_1:__v[0-9]+]]: i32 = ((unsafe { mtx_lock(std::ptr::addr_of_mut!(mutex)) }) == 0)
// CHECK-NEXT:             as i32;
// CHECK-NEXT:         concurrency_total += [[V_1]];
// CHECK-NEXT:         let [[V_2:__v[0-9]+]]: i32 = ((unsafe { mtx_unlock(std::ptr::addr_of_mut!(mutex)) }) == 0)
// CHECK-NEXT:             as i32;
// CHECK-NEXT:         concurrency_total += [[V_2]];
// CHECK-NEXT:         unsafe { mtx_destroy(std::ptr::addr_of_mut!(mutex)) };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { cnd_init(std::ptr::addr_of_mut!(condition)) }) == 0 {
// CHECK-NEXT:         concurrency_total += 1;
// CHECK-NEXT:         unsafe { cnd_destroy(std::ptr::addr_of_mut!(condition)) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         c11_once_total = 0;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { call_once(std::ptr::addr_of_mut!(once_control), Some(c11_once_handler)) };
// CHECK-NEXT:     unsafe { call_once(std::ptr::addr_of_mut!(once_control), Some(c11_once_handler)) };
// CHECK-NEXT:     concurrency_total += unsafe { c11_once_total };
// CHECK-NEXT:     if (unsafe { tss_create(std::ptr::addr_of_mut!(thread_key), None) }) == 0 {
// CHECK-NEXT:         concurrency_total += 1;
// CHECK-NEXT:         let [[V_3:__v[0-9]+]]: i32 = ((unsafe {
// CHECK-NEXT:             tss_set(thread_key, std::ptr::addr_of_mut!(thread_increment) as *mut ())
// CHECK-NEXT:         }) == 0) as i32;
// CHECK-NEXT:         concurrency_total += [[V_3]];
// CHECK-NEXT:         let [[V_4:__v[0-9]+]]: i32 = ((unsafe { tss_get(thread_key) })
// CHECK-NEXT:             == (std::ptr::addr_of_mut!(thread_increment) as *mut ())) as i32;
// CHECK-NEXT:         concurrency_total += [[V_4]];
// CHECK-NEXT:         unsafe { tss_delete(thread_key) };
// CHECK-NEXT:     }
// CHECK-NEXT:     concurrency_total += true as i32;
// CHECK-NEXT:     conversion_total = (((unsafe {
// CHECK-NEXT:         mbrtoc16(
// CHECK-NEXT:             std::ptr::addr_of_mut!(converted16),
// CHECK-NEXT:             c"A".as_ptr(),
// CHECK-NEXT:             1u64,
// CHECK-NEXT:             std::ptr::addr_of_mut!(utf16_state),
// CHECK-NEXT:         )
// CHECK-NEXT:     }) as u32) as i32)
// CHECK-NEXT:         + (((unsafe {
// CHECK-NEXT:             c16rtomb(
// CHECK-NEXT:                 multibyte16.0.as_mut_ptr() as *mut i8,
// CHECK-NEXT:                 65u16,
// CHECK-NEXT:                 std::ptr::addr_of_mut!(utf16_state),
// CHECK-NEXT:             )
// CHECK-NEXT:         }) as u32) as i32)
// CHECK-NEXT:         + (((unsafe {
// CHECK-NEXT:             mbrtoc32(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(converted32),
// CHECK-NEXT:                 c"B".as_ptr(),
// CHECK-NEXT:                 1u64,
// CHECK-NEXT:                 std::ptr::addr_of_mut!(utf32_state),
// CHECK-NEXT:             )
// CHECK-NEXT:         }) as u32) as i32)
// CHECK-NEXT:         + (((unsafe {
// CHECK-NEXT:             c32rtomb(
// CHECK-NEXT:                 multibyte32.0.as_mut_ptr() as *mut i8,
// CHECK-NEXT:                 66u32,
// CHECK-NEXT:                 std::ptr::addr_of_mut!(utf32_state),
// CHECK-NEXT:             )
// CHECK-NEXT:         }) as u32) as i32) + ((converted16 as u32) as i32) + (converted32 as i32)
// CHECK-NEXT:         + (multibyte16.0[0] as i32) + (multibyte32.0[0] as i32);
// CHECK-NEXT:     let aligned_memory: *mut () = unsafe { aligned_alloc(64u64, 64u64) };
// CHECK-NEXT:     let [[V_5:__v[0-9]+]]: i32 = alignment_total;
// CHECK-NEXT:     alignment_total = [[V_5]]
// CHECK-NEXT:         + ((!aligned_memory.is_null() && (aligned_memory as u64) % 64 == 0) as i32);
// CHECK-NEXT:     unsafe { free(aligned_memory) };
// CHECK-NEXT:     quick_total = ((unsafe { at_quick_exit(Some(c11_quick_handler)) }) == 0) as i32;
// CHECK-NEXT:     unsafe { remove(c"slate-c11-exclusive.tmp".as_ptr()) };
// CHECK-NEXT:     let exclusive_first: *mut _IO_FILE = c11_open_exclusive(
// CHECK-NEXT:         c"slate-c11-exclusive.tmp".as_ptr(),
// CHECK-NEXT:     );
// CHECK-NEXT:     let exclusive_second: *mut _IO_FILE = c11_open_exclusive(
// CHECK-NEXT:         c"slate-c11-exclusive.tmp".as_ptr(),
// CHECK-NEXT:     );
// CHECK-NEXT:     exclusive_total = (!exclusive_first.is_null() && exclusive_second.is_null()) as i32;
// CHECK-NEXT:     if !exclusive_first.is_null() {
// CHECK-NEXT:         unsafe { fclose(exclusive_first) };
// CHECK-NEXT:     }
// CHECK-NEXT:     if !exclusive_second.is_null() {
// CHECK-NEXT:         unsafe { fclose(exclusive_second) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { remove(c"slate-c11-exclusive.tmp".as_ptr()) };
// CHECK-NEXT:     timespec_total = ((unsafe {
// CHECK-NEXT:         timespec_get(std::ptr::addr_of_mut!(current_time), 1i32)
// CHECK-NEXT:     }) == 1 && current_time.tv_nsec >= 0 && current_time.tv_nsec < 1000000000) as i32;
// CHECK-NEXT:     let complex_value: num_complex::Complex<f64> = num_complex::Complex {
// CHECK-NEXT:         re: 2.0f64,
// CHECK-NEXT:         im: 3.0f64,
// CHECK-NEXT:     };
// CHECK-NEXT:     complex_total = (((unsafe { creal(complex_value) }) == 2.0f64) as i32)
// CHECK-NEXT:         + (((unsafe { cimag(complex_value) }) == 3.0f64) as i32);
// CHECK-NEXT:     limits_total = (true as i32) + (true as i32) + (true as i32)
// CHECK-NEXT:         + ((1e-45f32 > 0.0f32) as i32) + ((5e-324f64 > 0.0f64) as i32)
// CHECK-NEXT:         + ((LongDouble([1, 0, 0, 0, 0, 0, 0, 0, 0, 0])
// CHECK-NEXT:             > LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0])) as i32)
// CHECK-NEXT:         + ((1i32 >= -1i32) as i32) + ((1i32 >= -1i32) as i32) + ((1i32 >= -1i32) as i32);
// CHECK-NEXT:     limits_total += 0;
// CHECK-NEXT:     if (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(c11_never_flag)) }) != 0 {
// CHECK-NEXT:         c11_never_return(99i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     print!(
// CHECK-NEXT:         "{} {} {} {} {} {} {} {} {} {} {} {} {} {} {} {} {} {}", alignment_total,
// CHECK-NEXT:         unicode_total, generic_total, anonymous_total, evaluation_total, temporary_total,
// CHECK-NEXT:         static_assert_total, optional_total, atomic_total, thread_total,
// CHECK-NEXT:         concurrency_total, conversion_total, quick_total, exclusive_total,
// CHECK-NEXT:         timespec_total, complex_total, limits_total, 0i32
// CHECK-NEXT:     );
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { putchar(10i32) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: static mut utf8_text: [i8; 3] = [-50, -87, 0];
// CHECK-NEXT: static mut utf16_text: [u16; 2] = [937, 0];
// CHECK-NEXT: static mut utf32_text: [u32; 2] = [128578, 0];
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
