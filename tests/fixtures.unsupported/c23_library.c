#include <fenv.h>
#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdatomic.h>
#include <stdbit.h>
#include <stdckdint.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <uchar.h>
#include <wchar.h>

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

static int c23_checked_arithmetic(void) {
  int result;
  int total = 0;
  total += !ckd_add(&result, 20, 22) && result == 42;
  total += !ckd_sub(&result, 50, 8) && result == 42;
  total += !ckd_mul(&result, 6, 7) && result == 42;
  total += ckd_add(&result, INT_MAX, 1);
  return total;
}

static int c23_utf8(void) {
  mbstate_t input_state = {};
  mbstate_t output_state = {};
  char8_t character = 0;
  char output[MB_LEN_MAX] = {};
  size_t input_size = mbrtoc8(&character, "A", 1, &input_state);
  size_t output_size = c8rtomb(output, character, &output_state);
  atomic_char8_t atomic_character = character;
  atomic_store(&atomic_character, u8'B');
  return (input_size == 1) + (output_size == 1) + (output[0] == 'A') +
         (atomic_load(&atomic_character) == u8'B') +
         (ATOMIC_CHAR8_T_LOCK_FREE > 0);
}

static int c23_memory(void) {
  char source[] = "abcdef";
  char destination[8] = {};
  char secret[] = "secret";
  char *first_copy;
  char *second_copy;
  void *stop = memccpy(destination, source, 'c', 6);
  int total = stop == destination + 3 && destination[2] == 'c';
  memset_explicit(secret, 0, sizeof(secret));
  total += secret[0] == 0 && secret[5] == 0;
  first_copy = strdup("c23");
  second_copy = strndup("library", 3);
  total += first_copy != nullptr && strcmp(first_copy, "c23") == 0;
  total += second_copy != nullptr && strcmp(second_copy, "lib") == 0;
  free(first_copy);
  free(second_copy);
  return total;
}

static int c23_time(void) {
  time_t timestamp = 0;
  struct tm utc = {};
  struct tm local = {};
  struct timespec resolution = {};
  char month[32] = {};
  wchar_t wide_month[32] = {};
  int total = gmtime_r(&timestamp, &utc) == &utc;
  total += localtime_r(&timestamp, &local) == &local;
  total += timespec_getres(&resolution, TIME_UTC) == TIME_UTC;
  total += resolution.tv_sec > 0 || resolution.tv_nsec > 0;
  total += timegm(&utc) == 0;
  total += strftime(month, sizeof(month), "%OB", &utc) == 7;
  total += strcmp(month, "January") == 0;
  total += wcsftime(wide_month, 32, L"%OB", &utc) == 7;
  total += wcscmp(wide_month, L"January") == 0;
  return total;
}

static int c23_io(void) {
  char output[64] = {};
  char float_output[16] = {};
  char double_output[16] = {};
  char long_double_output[16] = {};
  unsigned int binary_value = 0;
  uint16_t exact_value = 0;
  uint_fast16_t fast_value = 0;
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
  int integer_widths = BOOL_WIDTH + CHAR_WIDTH + SCHAR_WIDTH + UCHAR_WIDTH +
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
                        (__STDC_VERSION_STDBIT_H__ == 202311L);
  return integer_widths + floating_limits + header_versions;
}

int main(void) {
  printf("%d %d %d %d %d %d\n", c23_stdbit(), c23_checked_arithmetic(),
         c23_utf8(), c23_memory(), c23_time(), c23_io() + c23_limits());
  return 0;
}
