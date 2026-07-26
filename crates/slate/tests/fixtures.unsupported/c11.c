#define __STDC_WANT_LIB_EXT1__ 1

#include <complex.h>
#include <errno.h>
#include <float.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdalign.h>
#include <stdatomic.h>
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
    int integer;
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
static _Atomic int c11_atomic_total;
static _Thread_local int c11_thread_local_value;
static int c11_evaluation_total;
static volatile int c11_never_flag;
static int c11_once_total;

_Static_assert(_Alignof(struct C11OverAligned) >= 32,
               "over-aligned structure");
_Static_assert(sizeof(char16_t) >= 2, "UTF-16 code unit");
_Static_assert(sizeof(char32_t) >= 4, "UTF-32 code unit");

#define C11_TYPE_KIND(value)                                                 \
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
  int increment = *(int *)argument;
  c11_thread_local_value = 29;
  errno = ERANGE;
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
  static const char utf8_text[] = u8"\u03a9";
  static const char16_t utf16_text[] = u"\u03a9";
  static const char32_t utf32_text[] = U"\U0001f642";
  char16_t utf16_character = u'\u03a9';
  char32_t utf32_character = U'\U0001f642';
  struct C11Anonymous anonymous = {0};
  struct C11OverAligned aligned_object = {0};
  mbstate_t utf16_state = {0};
  mbstate_t utf32_state = {0};
  char16_t converted16 = 0;
  char32_t converted32 = 0;
  char multibyte16[MB_LEN_MAX];
  char multibyte32[MB_LEN_MAX];
  struct timespec current_time = {0, 0};
  thrd_t thread;
  mtx_t mutex;
  cnd_t condition;
  once_flag once_control = ONCE_FLAG_INIT;
  tss_t thread_key;
  int thread_increment = 7;
  int thread_result = 0;
  int thread_created;
  int thread_joined;
  void *aligned_memory;
  FILE *exclusive_first;
  FILE *exclusive_second;
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
  int bounds_total;
#endif

  _Static_assert(sizeof(utf8_text) == 3, "UTF-8 literal size");

  alignment_total =
      (int)_Alignof(int) + (int)_Alignof(struct C11OverAligned) +
      (((uintptr_t)c11_aligned_buffer % 64U) == 0U) +
      (((uintptr_t)&aligned_object % 32U) == 0U);

  unicode_total =
      (unsigned char)utf8_text[0] + (unsigned char)utf8_text[1] +
      utf16_text[0] + (int)utf32_text[0] + utf16_character +
      (int)utf32_character;

  generic_total =
      C11_TYPE_KIND(1) + C11_TYPE_KIND(1.0) + C11_TYPE_KIND((char *)0);

  anonymous.integer = 31;
  anonymous.x = 37;
  anonymous.y = 41;
  anonymous_total = anonymous.integer + anonymous.x + anonymous.y;

  c11_evaluation_total = 0;
  evaluation_total = c11_evaluation_step(2) + c11_evaluation_step(3) +
                     c11_evaluation_total;

  temporary_total = c11_make_temporary(43).values[1];
  static_assert_total = 1;
  optional_total =
      C11_ANALYZABLE_VALUE + C11_LIB_EXT1_VALUE + C11_ATOMICS_VALUE +
      C11_COMPLEX_VALUE + C11_THREADS_VALUE + C11_VLA_VALUE;

  atomic_init(&c11_atomic_total, 5);
  c11_thread_local_value = 17;
  errno = 0;
  thread_created = thrd_create(&thread, c11_thread_worker, &thread_increment);
  thread_joined =
      thread_created == thrd_success ? thrd_join(thread, &thread_result) : -1;
  atomic_total = atomic_load_explicit(&c11_atomic_total, memory_order_seq_cst);
  thread_total =
      (thread_created == thrd_success) + (thread_joined == thrd_success) +
      thread_result + c11_thread_local_value + (errno == 0);

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
    concurrency_total +=
        tss_set(thread_key, &thread_increment) == thrd_success;
    concurrency_total += tss_get(thread_key) == &thread_increment;
    tss_delete(thread_key);
  }
  concurrency_total += TSS_DTOR_ITERATIONS >= 1;

  converted16 = 0;
  converted32 = 0;
  conversion_total =
      (int)mbrtoc16(&converted16, "A", 1, &utf16_state) +
      (int)c16rtomb(multibyte16, u'A', &utf16_state) +
      (int)mbrtoc32(&converted32, "B", 1, &utf32_state) +
      (int)c32rtomb(multibyte32, U'B', &utf32_state) + converted16 +
      (int)converted32 + multibyte16[0] + multibyte32[0];

  aligned_memory = aligned_alloc(64, 64);
  alignment_total +=
      aligned_memory != NULL && ((uintptr_t)aligned_memory % 64U) == 0U;
  free(aligned_memory);

  quick_total = at_quick_exit(c11_quick_handler) == 0;

  remove("slate-c11-exclusive.tmp");
  exclusive_first = c11_open_exclusive("slate-c11-exclusive.tmp");
  exclusive_second = c11_open_exclusive("slate-c11-exclusive.tmp");
  exclusive_total = exclusive_first != NULL && exclusive_second == NULL;
  if (exclusive_first != NULL) {
    fclose(exclusive_first);
  }
  if (exclusive_second != NULL) {
    fclose(exclusive_second);
  }
  remove("slate-c11-exclusive.tmp");

  timespec_total =
      timespec_get(&current_time, TIME_UTC) == TIME_UTC &&
      current_time.tv_nsec >= 0 && current_time.tv_nsec < 1000000000L;

  complex_value = CMPLX(2.0, 3.0);
  complex_total =
      (creal(complex_value) == 2.0) + (cimag(complex_value) == 3.0);

  limits_total =
      (FLT_DECIMAL_DIG >= 6) + (DBL_DECIMAL_DIG >= 10) +
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

  printf(
      "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
      alignment_total, unicode_total, generic_total, anonymous_total,
      evaluation_total, temporary_total, static_assert_total, optional_total,
      atomic_total, thread_total, concurrency_total, conversion_total,
      quick_total, exclusive_total, timespec_total, complex_total, limits_total,
#ifdef __STDC_LIB_EXT1__
      bounds_total
#else
      0
#endif
  );
  putchar('\n');
  return 0;
}
