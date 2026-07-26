#include <complex.h>
#include <fenv.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#pragma STDC FENV_ACCESS ON
#pragma STDC FP_CONTRACT OFF
#pragma STDC CX_LIMITED_RANGE OFF

#define C99_PRAGMA(value) _Pragma(#value)
#define C99_SUM3(first, ...) sum3(first, __VA_ARGS__)

C99_PRAGMA(STDC FP_CONTRACT OFF)

struct C99Pair {
  int first;
  int second;
};

struct C99Flexible {
  size_t count;
  int values[];
};

enum C99TrailingComma {
  C99_ENUM_VALUE = 17,
};

typedef const int C99ConstInt;
typedef volatile int C99VolatileInt;
typedef int *restrict C99RestrictedIntPointer;

int c99_external_identifier_with_more_than_thirty_one_significant_characters = 5;
static int slash_comment_value = 3; //

static inline int c99_inline_square(int value) { return value * value; }

static int sum3(int first, int second, int third) {
  return first + second + third;
}

static int c99_restrict_sum(const int *restrict left,
                            const int *restrict right) {
  return *left + *right;
}

static int c99_qualified_array_sum(int values[static const restrict 3]) {
  return values[0] + values[1] + values[2];
}

static int c99_vm_sum(int length, int (*values)[length]) {
  int total = 0;
  for (int index = 0; index < length; ++index) {
    total += (*values)[index];
  }
  return total;
}

static int c99_thirty_two_parameters(
    int p01, int p02, int p03, int p04, int p05, int p06, int p07, int p08,
    int p09, int p10, int p11, int p12, int p13, int p14, int p15, int p16,
    int p17, int p18, int p19, int p20, int p21, int p22, int p23, int p24,
    int p25, int p26, int p27, int p28, int p29, int p30, int p31, int p32) {
  return p01 + p02 + p03 + p04 + p05 + p06 + p07 + p08 + p09 + p10 + p11 +
         p12 + p13 + p14 + p15 + p16 + p17 + p18 + p19 + p20 + p21 + p22 +
         p23 + p24 + p25 + p26 + p27 + p28 + p29 + p30 + p31 + p32;
}

int main(void) {
  int \u03b1 = 7;
  _Bool boolean_value = 4;
  long long signed_long_long = -9000000000LL;
  unsigned long long unsigned_long_long = 18000000000ULL;
  float _Complex float_complex = 1.0f + 2.0f * I;
  double _Complex double_complex = 3.0 + 4.0 * I;
  long double _Complex long_double_complex = 5.0L + 6.0L * I;
  int enhanced_arithmetic =
      boolean_value + (signed_long_long == -9000000000LL) +
      (unsigned_long_long == 18000000000ULL) +
      (float_complex == 1.0f + 2.0f * I) +
      (double_complex == 3.0 + 4.0 * I) +
      (long_double_complex == 5.0L + 6.0L * I);

  int flexible_total = 0;
  struct C99Flexible *flexible =
      malloc(sizeof(*flexible) + 3 * sizeof(flexible->values[0]));
  if (flexible == NULL) {
    return 2;
  }
  flexible->count = 3;
  for (size_t index = 0; index < flexible->count; ++index) {
    flexible->values[index] = (int)index + 1;
    flexible_total += flexible->values[index];
  }
  free(flexible);

  int length = 3;
  int variable_length_array[length];
  for (int index = 0; index < length; ++index) {
    variable_length_array[index] = index + 4;
  }
  int vm_total = c99_vm_sum(length, &variable_length_array);

  int initializer_seed = 19;
  struct C99Pair nonconstant_initializer = {initializer_seed,
                                             initializer_seed + 1};
  struct C99Pair designated_initializer = {.second = 23, .first = 22};
  int designated_array[4] = {[2] = 29, [0] = 27};

  C99ConstInt const_value = 31;
  const C99ConstInt idempotent_const_value = const_value;
  C99VolatileInt volatile_value = 37;
  volatile C99VolatileInt idempotent_volatile_value = volatile_value;
  int restricted_value = 41;
  C99RestrictedIntPointer restrict restricted_pointer = &restricted_value;

  double hexadecimal_float = 0x1.8p+1;
  struct C99Pair compound_pair = (struct C99Pair){43, 47};
  int compound_array_value = ((int[]){51, 53})[1];
  int signed_quotient = -7 / 3;
  int signed_remainder = -7 % 3;

  int mixed_order = 59;
  mixed_order += 2;
  int declaration_after_statement = 61;

  int for_total = 0;
  for (int index = 0; index < 3; ++index) {
    for_total += index;
  }

  int qualified_values[3] = {2, 3, 5};
  int macro_total = C99_SUM3(7, 11, 13);
  int translation_limit_total = c99_thirty_two_parameters(
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1);

  int fenv_clear = feclearexcept(FE_ALL_EXCEPT);
  int fenv_flags = fetestexcept(FE_ALL_EXCEPT);

  printf(
      "%s %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d "
      "%d %d %d %d %d %d %d %d %d\n",
      __func__, \u03b1,
      c99_external_identifier_with_more_than_thirty_one_significant_characters,
      slash_comment_value, enhanced_arithmetic, flexible_total, vm_total,
      nonconstant_initializer.first + nonconstant_initializer.second,
      designated_initializer.first + designated_initializer.second,
      designated_array[0] + designated_array[2], idempotent_const_value,
      idempotent_volatile_value, *restricted_pointer, (int)hexadecimal_float,
      compound_pair.first + compound_pair.second, compound_array_value,
      signed_quotient, signed_remainder, mixed_order,
      declaration_after_statement, for_total, c99_inline_square(8),
      c99_qualified_array_sum(qualified_values), macro_total,
      translation_limit_total, C99_ENUM_VALUE, fenv_clear, fenv_flags,
      c99_restrict_sum(&qualified_values[0], &qualified_values[1]));
}
