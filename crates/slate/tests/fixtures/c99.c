#include <complex.h>
#include <fenv.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define C99_SUM3(first, ...) sum3(first, __VA_ARGS__)

struct C99Pair {
  int first;
  int second;
};

struct C99Flexible {
  size_t count;
  int    values[];
};

enum C99TrailingComma {
  C99_ENUM_VALUE = 17,
};

typedef const int    C99ConstInt;
typedef volatile int C99VolatileInt;
typedef int *restrict C99RestrictedIntPointer;

int c99_external_identifier_with_more_than_thirty_one_significant_characters =
    5;
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
  return p01 + p02 + p03 + p04 + p05 + p06 + p07 + p08 + p09 + p10 + p11 + p12 +
         p13 + p14 + p15 + p16 + p17 + p18 + p19 + p20 + p21 + p22 + p23 + p24 +
         p25 + p26 + p27 + p28 + p29 + p30 + p31 + p32;
}

int main(void) {
  int                \u03b1                = 7;
  _Bool              boolean_value         = 4;
  long long          signed_long_long      = -9000000000LL;
  unsigned long long unsigned_long_long    = 18000000000ULL;
  float _Complex float_complex             = 1.0f + 2.0f * I;
  double _Complex double_complex           = 3.0 + 4.0 * I;
  long double _Complex long_double_complex = 5.0L + 6.0L * I;
  int enhanced_arithmetic =
      boolean_value + (signed_long_long == -9000000000LL) +
      (unsigned_long_long == 18000000000ULL) +
      (float_complex == 1.0f + 2.0f * I) + (double_complex == 3.0 + 4.0 * I) +
      (long_double_complex == 5.0L + 6.0L * I);

  int                 flexible_total = 0;
  struct C99Flexible *flexible =
      malloc(sizeof(*flexible) + 3 * sizeof(flexible->values[0]));
  if (flexible == NULL) {
    return 2;
  }
  flexible->count = 3;
  for (size_t index = 0; index < flexible->count; ++index) {
    flexible->values[index]  = (int)index + 1;
    flexible_total          += flexible->values[index];
  }
  free(flexible);

  int length = 3;
  int variable_length_array[length];
  for (int index = 0; index < length; ++index) {
    variable_length_array[index] = index + 4;
  }
  int vm_total = c99_vm_sum(length, &variable_length_array);

  int            initializer_seed        = 19;
  struct C99Pair nonconstant_initializer = {initializer_seed,
                                            initializer_seed + 1};
  struct C99Pair designated_initializer  = {.second = 23, .first = 22};
  int            designated_array[4]     = {[2] = 29, [0] = 27};

  C99ConstInt             const_value                 = 31;
  const C99ConstInt       idempotent_const_value      = const_value;
  C99VolatileInt          volatile_value              = 37;
  volatile C99VolatileInt idempotent_volatile_value   = volatile_value;
  int                     restricted_value            = 41;
  C99RestrictedIntPointer restrict restricted_pointer = &restricted_value;

  double         hexadecimal_float    = 0x1.8p+1;
  struct C99Pair compound_pair        = (struct C99Pair){43, 47};
  int            compound_array_value = ((int[]){51, 53})[1];
  int            signed_quotient      = -7 / 3;
  int            signed_remainder     = -7 % 3;

  int mixed_order                  = 59;
  mixed_order                     += 2;
  int declaration_after_statement  = 61;

  int for_total = 0;
  for (int index = 0; index < 3; ++index) {
    for_total += index;
  }

  int qualified_values[3] = {2, 3, 5};
  int macro_total         = C99_SUM3(7, 11, 13);
  int translation_limit_total =
      c99_thirty_two_parameters(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                                1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);

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

// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
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
// LOWERING-NEXT:     fn add(self, o: LongDouble) -> LongDouble {
// LOWERING-NEXT:         __slate_f80_add(self, o)
// LOWERING-NEXT:     }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::ops::Sub for LongDouble {
// LOWERING-NEXT:     type Output = LongDouble;
// LOWERING-NEXT:     fn sub(self, o: LongDouble) -> LongDouble {
// LOWERING-NEXT:         __slate_f80_sub(self, o)
// LOWERING-NEXT:     }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::ops::Mul for LongDouble {
// LOWERING-NEXT:     type Output = LongDouble;
// LOWERING-NEXT:     fn mul(self, o: LongDouble) -> LongDouble {
// LOWERING-NEXT:         __slate_f80_mul(self, o)
// LOWERING-NEXT:     }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::ops::Div for LongDouble {
// LOWERING-NEXT:     type Output = LongDouble;
// LOWERING-NEXT:     fn div(self, o: LongDouble) -> LongDouble {
// LOWERING-NEXT:         __slate_f80_div(self, o)
// LOWERING-NEXT:     }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::ops::AddAssign for LongDouble {
// LOWERING-NEXT:     fn add_assign(&mut self, o: LongDouble) {
// LOWERING-NEXT:         {
// LOWERING-NEXT:             *self = __slate_f80_add(*self, o);
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::ops::SubAssign for LongDouble {
// LOWERING-NEXT:     fn sub_assign(&mut self, o: LongDouble) {
// LOWERING-NEXT:         {
// LOWERING-NEXT:             *self = __slate_f80_sub(*self, o);
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::ops::MulAssign for LongDouble {
// LOWERING-NEXT:     fn mul_assign(&mut self, o: LongDouble) {
// LOWERING-NEXT:         {
// LOWERING-NEXT:             *self = __slate_f80_mul(*self, o);
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::ops::DivAssign for LongDouble {
// LOWERING-NEXT:     fn div_assign(&mut self, o: LongDouble) {
// LOWERING-NEXT:         {
// LOWERING-NEXT:             *self = __slate_f80_div(*self, o);
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
// LOWERING-NEXT:     fn eq(&self, other: &LongDouble) -> bool {
// LOWERING-NEXT:         __slate_f80_eq(*self, *other)
// LOWERING-NEXT:     }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::cmp::PartialOrd for LongDouble {
// LOWERING-NEXT:     fn partial_cmp(&self, other: &LongDouble) -> Option<std::cmp::Ordering> {
// LOWERING-NEXT:         if __slate_f80_lt(*self, *other) {
// LOWERING-NEXT:             Some(std::cmp::Ordering::Less)
// LOWERING-NEXT:         } else {
// LOWERING-NEXT:             if __slate_f80_gt(*self, *other) {
// LOWERING-NEXT:                 Some(std::cmp::Ordering::Greater)
// LOWERING-NEXT:             } else {
// LOWERING-NEXT:                 if __slate_f80_eq(*self, *other) {
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
// LOWERING-NEXT: enum C99TrailingComma {
// LOWERING-NEXT:     C99_ENUM_VALUE = 17,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct C99Flexible {
// LOWERING-NEXT:     count: u64,
// LOWERING-NEXT:     values: [i32; 0],
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct C99Pair {
// LOWERING-NEXT:     first: i32,
// LOWERING-NEXT:     second: i32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut c99_external_identifier_with_more_than_thirty_one_significant_characters: i32 = 5;
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut slash_comment_value: i32 = 3;
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn malloc(_0: usize) -> *mut core::ffi::c_void;
// LOWERING-NEXT:     fn free(_0: *mut core::ffi::c_void);
// LOWERING-NEXT:     fn feclearexcept(_0: i32) -> i32;
// LOWERING-NEXT:     fn fetestexcept(_0: i32) -> i32;
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut α: i32 = 0;
// LOWERING-NEXT:     let mut enhanced_arithmetic: i32 = 0;
// LOWERING-NEXT:     let mut flexible_total: i32 = 0;
// LOWERING-NEXT:     let mut flexible: *mut C99Flexible = std::ptr::null_mut();
// LOWERING-NEXT:     let mut length: i32 = 0;
// LOWERING-NEXT:     let mut saved_stack: *mut u8 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut vm_total: i32 = 0;
// LOWERING-NEXT:     let mut initializer_seed: i32 = 0;
// LOWERING-NEXT:     let mut nonconstant_initializer: C99Pair = C99Pair {
// LOWERING-NEXT:         first: 0,
// LOWERING-NEXT:         second: 0,
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let mut designated_initializer: C99Pair = C99Pair {
// LOWERING-NEXT:         first: 0,
// LOWERING-NEXT:         second: 0,
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let mut designated_array: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// LOWERING-NEXT:     let mut const_value: i32 = 0;
// LOWERING-NEXT:     let mut idempotent_const_value: i32 = 0;
// LOWERING-NEXT:     let mut volatile_value: i32 = 0;
// LOWERING-NEXT:     let mut idempotent_volatile_value: i32 = 0;
// LOWERING-NEXT:     let mut restricted_value: i32 = 0;
// LOWERING-NEXT:     let mut restricted_pointer: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut hexadecimal_float: f64 = 0.0;
// LOWERING-NEXT:     let mut compound_pair: C99Pair = C99Pair {
// LOWERING-NEXT:         first: 0,
// LOWERING-NEXT:         second: 0,
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let mut compound_array_value: i32 = 0;
// LOWERING-NEXT:     let mut _compoundliteral: [i32; 2] = [0; 2];
// LOWERING-NEXT:     let mut signed_quotient: i32 = 0;
// LOWERING-NEXT:     let mut signed_remainder: i32 = 0;
// LOWERING-NEXT:     let mut mixed_order: i32 = 0;
// LOWERING-NEXT:     let mut declaration_after_statement: i32 = 0;
// LOWERING-NEXT:     let mut for_total: i32 = 0;
// LOWERING-NEXT:     let mut qualified_values: [i32; 3] = [0; 3];
// LOWERING-NEXT:     let mut macro_total: i32 = 0;
// LOWERING-NEXT:     let mut translation_limit_total: i32 = 0;
// LOWERING-NEXT:     let mut fenv_clear: i32 = 0;
// LOWERING-NEXT:     let mut fenv_flags: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 7;
// LOWERING-NEXT:     α = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 4;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = -9000000000i64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 18000000000u64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = 1.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = 2.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = 0.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = 0.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = 1.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<f32> = num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 3.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 4.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = 0.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = 0.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = 1.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<f32> = num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.re as f64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.im as f64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.re;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.im;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 160, 1, 64]);
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 1, 64]);
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = 0.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = 0.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = 1.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<f32> = num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = __slate_f80_from_f32({{_v[0-9]+}}.re);
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = __slate_f80_from_f32({{_v[0-9]+}}.im);
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.re;
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.im;
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = -9000000000i64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 18000000000u64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = 1.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = 2.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = 0.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = 0.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = 1.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<f32> = num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 3.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 4.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = 0.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = 0.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = 1.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<f32> = num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.re as f64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.im as f64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.re;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}}.im;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 160, 1, 64]);
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 1, 64]);
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = 0.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = 0.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = 1.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<f32> = num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = __slate_f80_from_f32({{_v[0-9]+}}.re);
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = __slate_f80_from_f32({{_v[0-9]+}}.im);
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.re;
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}}.im;
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-NEXT:         re: {{_v[0-9]+}},
// LOWERING-NEXT:         im: {{_v[0-9]+}},
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     enhanced_arithmetic = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     flexible_total = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 4;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe { malloc({{_v[0-9]+}} as usize) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut C99Flexible = {{_v[0-9]+}} as *mut C99Flexible;
// LOWERING-NEXT:     flexible = {{_v[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut C99Flexible = flexible;
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut C99Flexible = std::ptr::null_mut();
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:         if {{_v[0-9]+}} {
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 2;
// LOWERING-NEXT:             __retval = {{_v[0-9]+}};
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:             std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut C99Flexible = flexible;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*{{_v[0-9]+}}).count = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut index: u64 = 0;
// LOWERING-NEXT:         let {{_v[0-9]+}}: u64 = 0;
// LOWERING-NEXT:         index = {{_v[0-9]+}};
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let {{_v[0-9]+}}: u64 = index;
// LOWERING-NEXT:             let {{_v[0-9]+}}: *mut C99Flexible = flexible;
// LOWERING-NEXT:             let {{_v[0-9]+}}: u64 = unsafe { (*{{_v[0-9]+}}).count };
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 let {{_v[0-9]+}}: u64 = index;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:                 let {{_v[0-9]+}}: u64 = index;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: *mut C99Flexible = flexible;
// LOWERING-NEXT:                 unsafe {
// LOWERING-NEXT:                     *(*{{_v[0-9]+}}).values.as_mut_ptr().add({{_v[0-9]+}} as usize) = {{_v[0-9]+}};
// LOWERING-NEXT:                 }
// LOWERING-NEXT:                 let {{_v[0-9]+}}: u64 = index;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: *mut C99Flexible = flexible;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { *(*{{_v[0-9]+}}).values.as_mut_ptr().add({{_v[0-9]+}} as usize) };
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = flexible_total;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:                 flexible_total = {{_v[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: u64 = index;
// LOWERING-NEXT:             let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} + 1;
// LOWERING-NEXT:             index = {{_v[0-9]+}};
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut C99Flexible = flexible;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     unsafe { free({{_v[0-9]+}} as *mut core::ffi::c_void) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:     length = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = length;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} as u64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u8 = 0usize as *mut u8;
// LOWERING-NEXT:     saved_stack = {{_v[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut variable_length_array: Vec<i32> = vec![0; {{_v[0-9]+}} as usize];
// LOWERING-NEXT:         {
// LOWERING-NEXT:             let mut index2: i32 = 0;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:             index2 = {{_v[0-9]+}};
// LOWERING-NEXT:             loop {
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = index2;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = length;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-NEXT:                 if !{{_v[0-9]+}} {
// LOWERING-NEXT:                     break;
// LOWERING-NEXT:                 }
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = index2;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = 4;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = index2;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut i32 =
// LOWERING-NEXT:                         unsafe { variable_length_array.as_mut_ptr().offset({{_v[0-9]+}} as isize) };
// LOWERING-NEXT:                     unsafe {
// LOWERING-NEXT:                         *{{_v[0-9]+}} = {{_v[0-9]+}};
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = index2;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-NEXT:                 index2 = {{_v[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = length;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = c99_vm_sum({{_v[0-9]+}}, variable_length_array.as_mut_ptr());
// LOWERING-NEXT:         vm_total = {{_v[0-9]+}};
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 19;
// LOWERING-NEXT:         initializer_seed = {{_v[0-9]+}};
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = initializer_seed;
// LOWERING-NEXT:         nonconstant_initializer.first = {{_v[0-9]+}};
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = initializer_seed;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:         nonconstant_initializer.second = {{_v[0-9]+}};
// LOWERING-NEXT:         let {{_v[0-9]+}}: C99Pair = C99Pair {
// LOWERING-NEXT:             first: 22,
// LOWERING-NEXT:             second: 23,
// LOWERING-NEXT:         };
// LOWERING-NEXT:         designated_initializer = {{_v[0-9]+}};
// LOWERING-NEXT:         let {{_v[0-9]+}}: [i32; 4] = [27, 0, 29, 0];
// LOWERING-NEXT:         *designated_array = {{_v[0-9]+}};
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 31;
// LOWERING-NEXT:         const_value = {{_v[0-9]+}};
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = const_value;
// LOWERING-NEXT:         idempotent_const_value = {{_v[0-9]+}};
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 37;
// LOWERING-NEXT:         unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(volatile_value), {{_v[0-9]+}}) };
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(volatile_value)) };
// LOWERING-NEXT:         unsafe {
// LOWERING-NEXT:             std::ptr::write_volatile(std::ptr::addr_of_mut!(idempotent_volatile_value), {{_v[0-9]+}})
// LOWERING-NEXT:         };
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 41;
// LOWERING-NEXT:         restricted_value = {{_v[0-9]+}};
// LOWERING-NEXT:         restricted_pointer = std::ptr::addr_of_mut!(restricted_value);
// LOWERING-NEXT:         let {{_v[0-9]+}}: f64 = 3.0;
// LOWERING-NEXT:         hexadecimal_float = {{_v[0-9]+}};
// LOWERING-NEXT:         let {{_v[0-9]+}}: C99Pair = C99Pair {
// LOWERING-NEXT:             first: 43,
// LOWERING-NEXT:             second: 47,
// LOWERING-NEXT:         };
// LOWERING-NEXT:         compound_pair = {{_v[0-9]+}};
// LOWERING-NEXT:         let {{_v[0-9]+}}: i64 = 1;
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut i32 = _compoundliteral.as_mut_ptr() as *mut i32;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 51;
// LOWERING-NEXT:         unsafe {
// LOWERING-NEXT:             *{{_v[0-9]+}} = {{_v[0-9]+}};
// LOWERING-NEXT:         }
// LOWERING-NEXT:         let {{_v[0-9]+}}: i64 = 1;
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.add(1) };
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 53;
// LOWERING-NEXT:         unsafe {
// LOWERING-NEXT:             *{{_v[0-9]+}} = {{_v[0-9]+}};
// LOWERING-NEXT:         }
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = _compoundliteral[({{_v[0-9]+}} as usize)];
// LOWERING-NEXT:         compound_array_value = {{_v[0-9]+}};
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = -7;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} / {{_v[0-9]+}};
// LOWERING-NEXT:         signed_quotient = {{_v[0-9]+}};
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = -7;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} % {{_v[0-9]+}};
// LOWERING-NEXT:         signed_remainder = {{_v[0-9]+}};
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 59;
// LOWERING-NEXT:         mixed_order = {{_v[0-9]+}};
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 2;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = mixed_order;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:         mixed_order = {{_v[0-9]+}};
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 61;
// LOWERING-NEXT:         declaration_after_statement = {{_v[0-9]+}};
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         for_total = {{_v[0-9]+}};
// LOWERING-NEXT:         {
// LOWERING-NEXT:             let mut index3: i32 = 0;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:             index3 = {{_v[0-9]+}};
// LOWERING-NEXT:             loop {
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = index3;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-NEXT:                 if !{{_v[0-9]+}} {
// LOWERING-NEXT:                     break;
// LOWERING-NEXT:                 }
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = index3;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = for_total;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:                     for_total = {{_v[0-9]+}};
// LOWERING-NEXT:                 }
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = index3;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-NEXT:                 index3 = {{_v[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:         let {{_v[0-9]+}}: [i32; 3] = [2, 3, 5];
// LOWERING-NEXT:         qualified_values = {{_v[0-9]+}};
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 7;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 11;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 13;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = sum3({{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-NEXT:         macro_total = {{_v[0-9]+}};
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = c99_thirty_two_parameters(
// LOWERING-NEXT:             {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}},
// LOWERING-NEXT:         );
// LOWERING-NEXT:         translation_limit_total = {{_v[0-9]+}};
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 32;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 4;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} | {{_v[0-9]+}};
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 16;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} | {{_v[0-9]+}};
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 8;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} | {{_v[0-9]+}};
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} | {{_v[0-9]+}};
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = unsafe { feclearexcept({{_v[0-9]+}} as i32) };
// LOWERING-NEXT:         fenv_clear = {{_v[0-9]+}};
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 32;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 4;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} | {{_v[0-9]+}};
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 16;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} | {{_v[0-9]+}};
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 8;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} | {{_v[0-9]+}};
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} | {{_v[0-9]+}};
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = unsafe { fetestexcept({{_v[0-9]+}} as i32) };
// LOWERING-NEXT:         fenv_flags = {{_v[0-9]+}};
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut i8 = b"%s %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut i8 = b"main\0".as_ptr() as *mut i8;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = α;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 =
// LOWERING-NEXT:             unsafe { c99_external_identifier_with_more_than_thirty_one_significant_characters };
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = unsafe { slash_comment_value };
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = enhanced_arithmetic;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = flexible_total;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = vm_total;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = nonconstant_initializer.first;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = nonconstant_initializer.second;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = designated_initializer.first;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = designated_initializer.second;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:         let {{_v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = designated_array[({{_v[0-9]+}} as usize)];
// LOWERING-NEXT:         let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = designated_array[({{_v[0-9]+}} as usize)];
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = idempotent_const_value;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 =
// LOWERING-NEXT:             unsafe { std::ptr::read_volatile(std::ptr::addr_of!(idempotent_volatile_value)) };
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut i32 = restricted_pointer;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:         let {{_v[0-9]+}}: f64 = hexadecimal_float;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = compound_pair.first;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = compound_pair.second;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = compound_array_value;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = signed_quotient;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = signed_remainder;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = mixed_order;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = declaration_after_statement;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = for_total;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 8;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = c99_inline_square({{_v[0-9]+}});
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut i32 = qualified_values.as_mut_ptr() as *mut i32;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = c99_qualified_array_sum({{_v[0-9]+}});
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = macro_total;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = translation_limit_total;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = C99TrailingComma::C99_ENUM_VALUE as i32;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = fenv_clear;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = fenv_flags;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i64 = 1;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = c99_restrict_sum(
// LOWERING-NEXT:             std::ptr::addr_of_mut!(qualified_values[({{_v[0-9]+}} as usize)]),
// LOWERING-NEXT:             std::ptr::addr_of_mut!(qualified_values[({{_v[0-9]+}} as usize)]),
// LOWERING-NEXT:         );
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:             printf(
// LOWERING-NEXT:                 {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:                 {{_v[0-9]+}},
// LOWERING-NEXT:                 {{_v[0-9]+}},
// LOWERING-NEXT:                 {{_v[0-9]+}},
// LOWERING-NEXT:                 {{_v[0-9]+}},
// LOWERING-NEXT:                 {{_v[0-9]+}},
// LOWERING-NEXT:                 {{_v[0-9]+}},
// LOWERING-NEXT:                 {{_v[0-9]+}},
// LOWERING-NEXT:                 {{_v[0-9]+}},
// LOWERING-NEXT:                 {{_v[0-9]+}},
// LOWERING-NEXT:                 {{_v[0-9]+}},
// LOWERING-NEXT:                 {{_v[0-9]+}},
// LOWERING-NEXT:                 {{_v[0-9]+}},
// LOWERING-NEXT:                 {{_v[0-9]+}},
// LOWERING-NEXT:                 {{_v[0-9]+}},
// LOWERING-NEXT:                 {{_v[0-9]+}},
// LOWERING-NEXT:                 {{_v[0-9]+}},
// LOWERING-NEXT:                 {{_v[0-9]+}},
// LOWERING-NEXT:                 {{_v[0-9]+}},
// LOWERING-NEXT:                 {{_v[0-9]+}},
// LOWERING-NEXT:                 {{_v[0-9]+}},
// LOWERING-NEXT:                 {{_v[0-9]+}},
// LOWERING-NEXT:                 {{_v[0-9]+}},
// LOWERING-NEXT:                 {{_v[0-9]+}},
// LOWERING-NEXT:                 {{_v[0-9]+}},
// LOWERING-NEXT:                 {{_v[0-9]+}},
// LOWERING-NEXT:                 {{_v[0-9]+}},
// LOWERING-NEXT:                 {{_v[0-9]+}},
// LOWERING-NEXT:                 {{_v[0-9]+}},
// LOWERING-NEXT:                 {{_v[0-9]+}},
// LOWERING-NEXT:             )
// LOWERING-NEXT:         };
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn c99_vm_sum({{arg[0-9]+}}: i32, {{arg[0-9]+}}: *mut i32) -> i32 {
// LOWERING-NEXT:     let mut length: i32 = 0;
// LOWERING-NEXT:     let mut values: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut total: i32 = 0;
// LOWERING-NEXT:     length = {{arg[0-9]+}};
// LOWERING-NEXT:     values = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     total = {{_v[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut index: i32 = 0;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         index = {{_v[0-9]+}};
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = index;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = length;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = index;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: *mut i32 = values;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.offset({{_v[0-9]+}} as isize) };
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = total;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:                 total = {{_v[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = index;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-NEXT:             index = {{_v[0-9]+}};
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = total;
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn sum3({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) -> i32 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{arg[0-9]+}} + {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{arg[0-9]+}};
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn c99_thirty_two_parameters(
// LOWERING-NEXT:     {{arg[0-9]+}}: i32,
// LOWERING-NEXT:     {{arg[0-9]+}}: i32,
// LOWERING-NEXT:     {{arg[0-9]+}}: i32,
// LOWERING-NEXT:     {{arg[0-9]+}}: i32,
// LOWERING-NEXT:     {{arg[0-9]+}}: i32,
// LOWERING-NEXT:     {{arg[0-9]+}}: i32,
// LOWERING-NEXT:     {{arg[0-9]+}}: i32,
// LOWERING-NEXT:     {{arg[0-9]+}}: i32,
// LOWERING-NEXT:     {{arg[0-9]+}}: i32,
// LOWERING-NEXT:     {{arg[0-9]+}}: i32,
// LOWERING-NEXT:     {{arg[0-9]+}}: i32,
// LOWERING-NEXT:     {{arg[0-9]+}}: i32,
// LOWERING-NEXT:     {{arg[0-9]+}}: i32,
// LOWERING-NEXT:     {{arg[0-9]+}}: i32,
// LOWERING-NEXT:     {{arg[0-9]+}}: i32,
// LOWERING-NEXT:     {{arg[0-9]+}}: i32,
// LOWERING-NEXT:     {{arg[0-9]+}}: i32,
// LOWERING-NEXT:     {{arg[0-9]+}}: i32,
// LOWERING-NEXT:     {{arg[0-9]+}}: i32,
// LOWERING-NEXT:     {{arg[0-9]+}}: i32,
// LOWERING-NEXT:     {{arg[0-9]+}}: i32,
// LOWERING-NEXT:     {{arg[0-9]+}}: i32,
// LOWERING-NEXT:     {{arg[0-9]+}}: i32,
// LOWERING-NEXT:     {{arg[0-9]+}}: i32,
// LOWERING-NEXT:     {{arg[0-9]+}}: i32,
// LOWERING-NEXT:     {{arg[0-9]+}}: i32,
// LOWERING-NEXT:     {{arg[0-9]+}}: i32,
// LOWERING-NEXT:     {{arg[0-9]+}}: i32,
// LOWERING-NEXT:     {{arg[0-9]+}}: i32,
// LOWERING-NEXT:     {{arg[0-9]+}}: i32,
// LOWERING-NEXT:     {{arg[0-9]+}}: i32,
// LOWERING-NEXT:     {{arg[0-9]+}}: i32,
// LOWERING-NEXT: ) -> i32 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{arg[0-9]+}} + {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{arg[0-9]+}};
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn c99_inline_square({{arg[0-9]+}}: i32) -> i32 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{arg[0-9]+}} * {{arg[0-9]+}};
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn c99_qualified_array_sum({{arg[0-9]+}}: *mut i32) -> i32 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = unsafe { {{arg[0-9]+}}.add(0) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = unsafe { {{arg[0-9]+}}.add(1) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = unsafe { {{arg[0-9]+}}.add(2) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn c99_restrict_sum({{arg[0-9]+}}: *mut i32, {{arg[0-9]+}}: *mut i32) -> i32 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { *{{arg[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { *{{arg[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     safe fn __slate_cf80_div(
// LOWERING-NEXT:         a: num_complex::Complex<LongDouble>,
// LOWERING-NEXT:         b: num_complex::Complex<LongDouble>,
// LOWERING-NEXT:     ) -> num_complex::Complex<LongDouble>;
// LOWERING-NEXT:     safe fn __slate_cf80_mul(
// LOWERING-NEXT:         a: num_complex::Complex<LongDouble>,
// LOWERING-NEXT:         b: num_complex::Complex<LongDouble>,
// LOWERING-NEXT:     ) -> num_complex::Complex<LongDouble>;
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
// REWRITES: #![feature(c_variadic)]
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
// REWRITES-NEXT:     fn add(self, o: LongDouble) -> LongDouble {
// REWRITES-NEXT:         __slate_f80_add(self, o)
// REWRITES-NEXT:     }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::ops::Sub for LongDouble {
// REWRITES-NEXT:     type Output = LongDouble;
// REWRITES-NEXT:     fn sub(self, o: LongDouble) -> LongDouble {
// REWRITES-NEXT:         __slate_f80_sub(self, o)
// REWRITES-NEXT:     }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::ops::Mul for LongDouble {
// REWRITES-NEXT:     type Output = LongDouble;
// REWRITES-NEXT:     fn mul(self, o: LongDouble) -> LongDouble {
// REWRITES-NEXT:         __slate_f80_mul(self, o)
// REWRITES-NEXT:     }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::ops::Div for LongDouble {
// REWRITES-NEXT:     type Output = LongDouble;
// REWRITES-NEXT:     fn div(self, o: LongDouble) -> LongDouble {
// REWRITES-NEXT:         __slate_f80_div(self, o)
// REWRITES-NEXT:     }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::ops::AddAssign for LongDouble {
// REWRITES-NEXT:     fn add_assign(&mut self, o: LongDouble) {
// REWRITES-NEXT:         {
// REWRITES-NEXT:             *self = __slate_f80_add(*self, o);
// REWRITES-NEXT:         }
// REWRITES-NEXT:     }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::ops::SubAssign for LongDouble {
// REWRITES-NEXT:     fn sub_assign(&mut self, o: LongDouble) {
// REWRITES-NEXT:         {
// REWRITES-NEXT:             *self = __slate_f80_sub(*self, o);
// REWRITES-NEXT:         }
// REWRITES-NEXT:     }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::ops::MulAssign for LongDouble {
// REWRITES-NEXT:     fn mul_assign(&mut self, o: LongDouble) {
// REWRITES-NEXT:         {
// REWRITES-NEXT:             *self = __slate_f80_mul(*self, o);
// REWRITES-NEXT:         }
// REWRITES-NEXT:     }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::ops::DivAssign for LongDouble {
// REWRITES-NEXT:     fn div_assign(&mut self, o: LongDouble) {
// REWRITES-NEXT:         {
// REWRITES-NEXT:             *self = __slate_f80_div(*self, o);
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
// REWRITES-NEXT:     fn eq(&self, other: &LongDouble) -> bool {
// REWRITES-NEXT:         __slate_f80_eq(*self, *other)
// REWRITES-NEXT:     }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::cmp::PartialOrd for LongDouble {
// REWRITES-NEXT:     fn partial_cmp(&self, other: &LongDouble) -> Option<std::cmp::Ordering> {
// REWRITES-NEXT:         if __slate_f80_lt(*self, *other) {
// REWRITES-NEXT:             Some(std::cmp::Ordering::Less)
// REWRITES-NEXT:         } else {
// REWRITES-NEXT:             if __slate_f80_gt(*self, *other) {
// REWRITES-NEXT:                 Some(std::cmp::Ordering::Greater)
// REWRITES-NEXT:             } else {
// REWRITES-NEXT:                 if __slate_f80_eq(*self, *other) {
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
// REWRITES-NEXT: enum C99TrailingComma {
// REWRITES-NEXT:     C99_ENUM_VALUE = 17,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct C99Flexible {
// REWRITES-NEXT:     count: u64,
// REWRITES-NEXT:     values: [i32; 0],
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct C99Pair {
// REWRITES-NEXT:     first: i32,
// REWRITES-NEXT:     second: i32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut c99_external_identifier_with_more_than_thirty_one_significant_characters: i32 = 5;
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut slash_comment_value: i32 = 3;
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn malloc(_0: usize) -> *mut core::ffi::c_void;
// REWRITES-NEXT:     fn free(_0: *mut core::ffi::c_void);
// REWRITES-NEXT:     fn feclearexcept(_0: i32) -> i32;
// REWRITES-NEXT:     fn fetestexcept(_0: i32) -> i32;
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let mut __retval: i32 = 0;
// REWRITES-NEXT:     let mut α: i32 = 7;
// REWRITES-NEXT:     let mut enhanced_arithmetic: i32 = 0;
// REWRITES-NEXT:     let mut flexible_total: i32 = 0;
// REWRITES-NEXT:     let mut flexible: *mut C99Flexible = std::ptr::null_mut();
// REWRITES-NEXT:     let mut length: i32 = 3;
// REWRITES-NEXT:     let mut vm_total: i32 = 0;
// REWRITES-NEXT:     let mut initializer_seed: i32 = 19;
// REWRITES-NEXT:     let mut nonconstant_initializer: C99Pair = C99Pair {
// REWRITES-NEXT:         first: 0,
// REWRITES-NEXT:         second: 0,
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let mut designated_initializer: C99Pair = C99Pair {
// REWRITES-NEXT:         first: 0,
// REWRITES-NEXT:         second: 0,
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let mut designated_array: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// REWRITES-NEXT:     let mut const_value: i32 = 31;
// REWRITES-NEXT:     let mut idempotent_const_value: i32 = 0;
// REWRITES-NEXT:     let mut volatile_value: i32 = 0;
// REWRITES-NEXT:     let mut idempotent_volatile_value: i32 = 0;
// REWRITES-NEXT:     let mut restricted_value: i32 = 41;
// REWRITES-NEXT:     let mut restricted_pointer: *mut i32 = std::ptr::null_mut();
// REWRITES-NEXT:     let mut hexadecimal_float: f64 = 0.0;
// REWRITES-NEXT:     let mut compound_pair: C99Pair = C99Pair {
// REWRITES-NEXT:         first: 0,
// REWRITES-NEXT:         second: 0,
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let mut compound_array_value: i32 = 0;
// REWRITES-NEXT:     let mut _compoundliteral: [i32; 2] = [0; 2];
// REWRITES-NEXT:     let mut signed_quotient: i32 = 0;
// REWRITES-NEXT:     let mut signed_remainder: i32 = 0;
// REWRITES-NEXT:     let mut mixed_order: i32 = 59;
// REWRITES-NEXT:     let mut declaration_after_statement: i32 = 61;
// REWRITES-NEXT:     let mut for_total: i32 = 0;
// REWRITES-NEXT:     let mut qualified_values: [i32; 3] = [0; 3];
// REWRITES-NEXT:     let mut macro_total: i32 = 0;
// REWRITES-NEXT:     let mut translation_limit_total: i32 = 0;
// REWRITES-NEXT:     let mut fenv_clear: i32 = 0;
// REWRITES-NEXT:     let mut fenv_flags: i32 = 0;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = 4;
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = -9000000000i64;
// REWRITES-NEXT:     let {{_v[0-9]+}}: u64 = 18000000000u64;
// REWRITES-NEXT:     let {{_v[0-9]+}}: f32 = 2.0;
// REWRITES-NEXT:     let {{_v[0-9]+}}: f32 = 0.0;
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<f32> = num_complex::Complex {
// REWRITES-NEXT:         re: 1.0 + {{_v[0-9]+}} * (0.0 + {{_v[0-9]+}}),
// REWRITES-NEXT:         im: {{_v[0-9]+}} * 1.0,
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: f64 = 4.0;
// REWRITES-NEXT:     let {{_v[0-9]+}}: f32 = 0.0;
// REWRITES-NEXT:     let {{_v[0-9]+}}: f32 = 1.0;
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<f32> = num_complex::Complex {
// REWRITES-NEXT:         re: 0.0 + {{_v[0-9]+}},
// REWRITES-NEXT:         im: {{_v[0-9]+}},
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex {
// REWRITES-NEXT:         re: {{_v[0-9]+}}.re as f64,
// REWRITES-NEXT:         im: {{_v[0-9]+}}.im as f64,
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex {
// REWRITES-NEXT:         re: 3.0 + {{_v[0-9]+}} * {{_v[0-9]+}}.re,
// REWRITES-NEXT:         im: {{_v[0-9]+}} * {{_v[0-9]+}}.im,
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 160, 1, 64]);
// REWRITES-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 1, 64]);
// REWRITES-NEXT:     let {{_v[0-9]+}}: f32 = 0.0;
// REWRITES-NEXT:     let {{_v[0-9]+}}: f32 = 1.0;
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<f32> = num_complex::Complex {
// REWRITES-NEXT:         re: 0.0 + {{_v[0-9]+}},
// REWRITES-NEXT:         im: {{_v[0-9]+}},
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: LongDouble = __slate_f80_from_f32({{_v[0-9]+}}.re);
// REWRITES-NEXT:     let {{_v[0-9]+}}: LongDouble = __slate_f80_from_f32({{_v[0-9]+}}.im);
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} };
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-NEXT:         re: {{_v[0-9]+}} + {{_v[0-9]+}} * {{_v[0-9]+}}.re,
// REWRITES-NEXT:         im: {{_v[0-9]+}} * {{_v[0-9]+}}.im,
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: f32 = 2.0;
// REWRITES-NEXT:     let {{_v[0-9]+}}: f64 = 4.0;
// REWRITES-NEXT:     let {{_v[0-9]+}}: f32 = 0.0;
// REWRITES-NEXT:     let {{_v[0-9]+}}: f32 = 1.0;
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<f32> = num_complex::Complex {
// REWRITES-NEXT:         re: 0.0 + {{_v[0-9]+}},
// REWRITES-NEXT:         im: {{_v[0-9]+}},
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex {
// REWRITES-NEXT:         re: {{_v[0-9]+}}.re as f64,
// REWRITES-NEXT:         im: {{_v[0-9]+}}.im as f64,
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = ({{_v[0-9]+}} as i32)
// REWRITES-NEXT:         + (({{_v[0-9]+}} == -9000000000i64) as i32)
// REWRITES-NEXT:         + (({{_v[0-9]+}} == 18000000000u64) as i32)
// REWRITES-NEXT:         + (({{_v[0-9]+}}
// REWRITES-NEXT:             == num_complex::Complex {
// REWRITES-NEXT:                 re: 1.0 + {{_v[0-9]+}} * (0.0 + (0.0 as f32)),
// REWRITES-NEXT:                 im: {{_v[0-9]+}} * 1.0,
// REWRITES-NEXT:             }) as i32)
// REWRITES-NEXT:         + (({{_v[0-9]+}}
// REWRITES-NEXT:             == num_complex::Complex {
// REWRITES-NEXT:                 re: 3.0 + {{_v[0-9]+}} * {{_v[0-9]+}}.re,
// REWRITES-NEXT:                 im: {{_v[0-9]+}} * {{_v[0-9]+}}.im,
// REWRITES-NEXT:             }) as i32);
// REWRITES-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 160, 1, 64]);
// REWRITES-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 1, 64]);
// REWRITES-NEXT:     let {{_v[0-9]+}}: f32 = 0.0;
// REWRITES-NEXT:     let {{_v[0-9]+}}: f32 = 1.0;
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<f32> = num_complex::Complex {
// REWRITES-NEXT:         re: 0.0 + {{_v[0-9]+}},
// REWRITES-NEXT:         im: {{_v[0-9]+}},
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: LongDouble = __slate_f80_from_f32({{_v[0-9]+}}.re);
// REWRITES-NEXT:     let {{_v[0-9]+}}: LongDouble = __slate_f80_from_f32({{_v[0-9]+}}.im);
// REWRITES-NEXT:     let {{_v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex { re: {{_v[0-9]+}}, im: {{_v[0-9]+}} };
// REWRITES-NEXT:     enhanced_arithmetic = {{_v[0-9]+}}
// REWRITES-NEXT:         + (({{_v[0-9]+}}
// REWRITES-NEXT:             == num_complex::Complex {
// REWRITES-NEXT:                 re: {{_v[0-9]+}} + {{_v[0-9]+}} * {{_v[0-9]+}}.re,
// REWRITES-NEXT:                 im: {{_v[0-9]+}} * {{_v[0-9]+}}.im,
// REWRITES-NEXT:             }) as i32);
// REWRITES-NEXT:     let {{_v[0-9]+}}: u64 = 4;
// REWRITES-NEXT:     let {{_v[0-9]+}}: u64 = 8 + 3 * {{_v[0-9]+}};
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe { malloc({{_v[0-9]+}} as usize) };
// REWRITES-NEXT:     flexible = {{_v[0-9]+}} as *mut C99Flexible;
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = flexible == std::ptr::null_mut();
// REWRITES-NEXT:     if {{_v[0-9]+}} {
// REWRITES-NEXT:         __retval = 2;
// REWRITES-NEXT:         std::process::exit(__retval as i32);
// REWRITES-NEXT:     }
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*flexible).count = 3;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     for index in 0..unsafe { (*flexible).count } {
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = (index as i32) + 1;
// REWRITES-NEXT:         let {{_v[0-9]+}}: u64 = index;
// REWRITES-NEXT:         let {{_v[0-9]+}}: *mut C99Flexible = flexible;
// REWRITES-NEXT:         unsafe {
// REWRITES-NEXT:             *(*{{_v[0-9]+}}).values.as_mut_ptr().add({{_v[0-9]+}} as usize) = {{_v[0-9]+}};
// REWRITES-NEXT:         }
// REWRITES-NEXT:         let {{_v[0-9]+}}: u64 = index;
// REWRITES-NEXT:         let {{_v[0-9]+}}: *mut C99Flexible = flexible;
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = unsafe { *(*{{_v[0-9]+}}).values.as_mut_ptr().add({{_v[0-9]+}} as usize) };
// REWRITES-NEXT:         flexible_total += {{_v[0-9]+}};
// REWRITES-NEXT:     }
// REWRITES-NEXT:     unsafe { free(flexible as *mut core::ffi::c_void) };
// REWRITES-NEXT:     let mut variable_length_array: Vec<i32> = vec![0; (length as u64) as usize];
// REWRITES-NEXT:     for index2 in 0..length {
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = index2 + 4;
// REWRITES-NEXT:         let {{_v[0-9]+}}: *mut i32 = unsafe {
// REWRITES-NEXT:             variable_length_array
// REWRITES-NEXT:                 .as_mut_ptr()
// REWRITES-NEXT:                 .offset((index2 as i64) as isize)
// REWRITES-NEXT:         };
// REWRITES-NEXT:         unsafe {
// REWRITES-NEXT:             *{{_v[0-9]+}} = {{_v[0-9]+}};
// REWRITES-NEXT:         }
// REWRITES-NEXT:     }
// REWRITES-NEXT:     vm_total = c99_vm_sum(length, variable_length_array.as_mut_ptr());
// REWRITES-NEXT:     nonconstant_initializer.first = initializer_seed;
// REWRITES-NEXT:     nonconstant_initializer.second = initializer_seed + 1;
// REWRITES-NEXT:     designated_initializer = C99Pair {
// REWRITES-NEXT:         first: 22,
// REWRITES-NEXT:         second: 23,
// REWRITES-NEXT:     };
// REWRITES-NEXT:     *designated_array = [27, 0, 29, 0];
// REWRITES-NEXT:     idempotent_const_value = const_value;
// REWRITES-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(volatile_value), 37 as i32) };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         std::ptr::write_volatile(std::ptr::addr_of_mut!(idempotent_volatile_value), unsafe {
// REWRITES-NEXT:             std::ptr::read_volatile(std::ptr::addr_of!(volatile_value))
// REWRITES-NEXT:         })
// REWRITES-NEXT:     };
// REWRITES-NEXT:     restricted_pointer = std::ptr::addr_of_mut!(restricted_value);
// REWRITES-NEXT:     hexadecimal_float = 3.0;
// REWRITES-NEXT:     compound_pair = C99Pair {
// REWRITES-NEXT:         first: 43,
// REWRITES-NEXT:         second: 47,
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i32 = _compoundliteral.as_mut_ptr() as *mut i32;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         *{{_v[0-9]+}} = 51;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.add(1) };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         *{{_v[0-9]+}} = 53;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     compound_array_value = _compoundliteral[1];
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = 3;
// REWRITES-NEXT:     signed_quotient = -7 / {{_v[0-9]+}};
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = 3;
// REWRITES-NEXT:     signed_remainder = -7 % {{_v[0-9]+}};
// REWRITES-NEXT:     mixed_order += 2;
// REWRITES-NEXT:     for index3 in 0..3 {
// REWRITES-NEXT:         for_total += index3;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     qualified_values = [2, 3, 5];
// REWRITES-NEXT:     macro_total = sum3(7 as i32, 11 as i32, 13 as i32);
// REWRITES-NEXT:     translation_limit_total = c99_thirty_two_parameters(
// REWRITES-NEXT:         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
// REWRITES-NEXT:         1, 1,
// REWRITES-NEXT:     );
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = 4;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = 32 | {{_v[0-9]+}} | 16 | 8 | 1;
// REWRITES-NEXT:     fenv_clear = unsafe { feclearexcept({{_v[0-9]+}} as i32) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = 4;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = 32 | {{_v[0-9]+}} | 16 | 8 | 1;
// REWRITES-NEXT:     fenv_flags = unsafe { fetestexcept({{_v[0-9]+}} as i32) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 =
// REWRITES-NEXT:         c"%s %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n"
// REWRITES-NEXT:             .as_ptr() as *mut i8;
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = c"main".as_ptr() as *mut i8;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = α;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 =
// REWRITES-NEXT:         unsafe { c99_external_identifier_with_more_than_thirty_one_significant_characters };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { slash_comment_value };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = enhanced_arithmetic;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = flexible_total;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = vm_total;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = nonconstant_initializer.first + nonconstant_initializer.second;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = designated_initializer.first + designated_initializer.second;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = designated_array[0] + designated_array[2];
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = idempotent_const_value;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 =
// REWRITES-NEXT:         unsafe { std::ptr::read_volatile(std::ptr::addr_of!(idempotent_volatile_value)) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { *restricted_pointer };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = hexadecimal_float as i32;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = compound_pair.first + compound_pair.second;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = compound_array_value;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = signed_quotient;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = signed_remainder;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = mixed_order;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = declaration_after_statement;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = for_total;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = c99_inline_square(8);
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i32 = qualified_values.as_mut_ptr() as *mut i32;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = macro_total;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = translation_limit_total;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = C99TrailingComma::C99_ENUM_VALUE as i32;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_char,
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:             c99_qualified_array_sum({{_v[0-9]+}}),
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:             fenv_clear,
// REWRITES-NEXT:             fenv_flags,
// REWRITES-NEXT:             c99_restrict_sum(
// REWRITES-NEXT:                 unsafe { &(*std::ptr::addr_of_mut!(qualified_values[0])) },
// REWRITES-NEXT:                 unsafe { &(*std::ptr::addr_of_mut!(qualified_values[1])) },
// REWRITES-NEXT:             ),
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn c99_vm_sum({{arg[0-9]+}}: i32, {{arg[0-9]+}}: *mut i32) -> i32 {
// REWRITES-NEXT:     let __arg40_view = unsafe { std::slice::from_raw_parts({{arg[0-9]+}} as *const i32, {{arg[0-9]+}} as usize) };
// REWRITES-NEXT:     let mut length: i32 = {{arg[0-9]+}};
// REWRITES-NEXT:     let mut values: *mut i32 = {{arg[0-9]+}};
// REWRITES-NEXT:     let mut total: i32 = 0;
// REWRITES-NEXT:     let mut index: i32 = 0;
// REWRITES-NEXT:     while index < length {
// REWRITES-NEXT:         let {{_v[0-9]+}}: *mut i32 = values;
// REWRITES-NEXT:         unsafe { {{_v[0-9]+}}.offset((index as i64) as isize) };
// REWRITES-NEXT:         total += unsafe { __arg40_view[(index as usize)] };
// REWRITES-NEXT:         index += 1;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     total
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn sum3({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) -> i32 {
// REWRITES-NEXT:     {{arg[0-9]+}} + {{arg[0-9]+}} + {{arg[0-9]+}}
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn c99_thirty_two_parameters(
// REWRITES-NEXT:     {{arg[0-9]+}}: i32,
// REWRITES-NEXT:     {{arg[0-9]+}}: i32,
// REWRITES-NEXT:     {{arg[0-9]+}}: i32,
// REWRITES-NEXT:     {{arg[0-9]+}}: i32,
// REWRITES-NEXT:     {{arg[0-9]+}}: i32,
// REWRITES-NEXT:     {{arg[0-9]+}}: i32,
// REWRITES-NEXT:     {{arg[0-9]+}}: i32,
// REWRITES-NEXT:     {{arg[0-9]+}}: i32,
// REWRITES-NEXT:     {{arg[0-9]+}}: i32,
// REWRITES-NEXT:     {{arg[0-9]+}}: i32,
// REWRITES-NEXT:     {{arg[0-9]+}}: i32,
// REWRITES-NEXT:     {{arg[0-9]+}}: i32,
// REWRITES-NEXT:     {{arg[0-9]+}}: i32,
// REWRITES-NEXT:     {{arg[0-9]+}}: i32,
// REWRITES-NEXT:     {{arg[0-9]+}}: i32,
// REWRITES-NEXT:     {{arg[0-9]+}}: i32,
// REWRITES-NEXT:     {{arg[0-9]+}}: i32,
// REWRITES-NEXT:     {{arg[0-9]+}}: i32,
// REWRITES-NEXT:     {{arg[0-9]+}}: i32,
// REWRITES-NEXT:     {{arg[0-9]+}}: i32,
// REWRITES-NEXT:     {{arg[0-9]+}}: i32,
// REWRITES-NEXT:     {{arg[0-9]+}}: i32,
// REWRITES-NEXT:     {{arg[0-9]+}}: i32,
// REWRITES-NEXT:     {{arg[0-9]+}}: i32,
// REWRITES-NEXT:     {{arg[0-9]+}}: i32,
// REWRITES-NEXT:     {{arg[0-9]+}}: i32,
// REWRITES-NEXT:     {{arg[0-9]+}}: i32,
// REWRITES-NEXT:     {{arg[0-9]+}}: i32,
// REWRITES-NEXT:     {{arg[0-9]+}}: i32,
// REWRITES-NEXT:     {{arg[0-9]+}}: i32,
// REWRITES-NEXT:     {{arg[0-9]+}}: i32,
// REWRITES-NEXT:     {{arg[0-9]+}}: i32,
// REWRITES-NEXT: ) -> i32 {
// REWRITES-NEXT:     {{arg[0-9]+}} + {{arg[0-9]+}}
// REWRITES-NEXT:         + {{arg[0-9]+}}
// REWRITES-NEXT:         + {{arg[0-9]+}}
// REWRITES-NEXT:         + {{arg[0-9]+}}
// REWRITES-NEXT:         + {{arg[0-9]+}}
// REWRITES-NEXT:         + {{arg[0-9]+}}
// REWRITES-NEXT:         + {{arg[0-9]+}}
// REWRITES-NEXT:         + {{arg[0-9]+}}
// REWRITES-NEXT:         + {{arg[0-9]+}}
// REWRITES-NEXT:         + {{arg[0-9]+}}
// REWRITES-NEXT:         + {{arg[0-9]+}}
// REWRITES-NEXT:         + {{arg[0-9]+}}
// REWRITES-NEXT:         + {{arg[0-9]+}}
// REWRITES-NEXT:         + {{arg[0-9]+}}
// REWRITES-NEXT:         + {{arg[0-9]+}}
// REWRITES-NEXT:         + {{arg[0-9]+}}
// REWRITES-NEXT:         + {{arg[0-9]+}}
// REWRITES-NEXT:         + {{arg[0-9]+}}
// REWRITES-NEXT:         + {{arg[0-9]+}}
// REWRITES-NEXT:         + {{arg[0-9]+}}
// REWRITES-NEXT:         + {{arg[0-9]+}}
// REWRITES-NEXT:         + {{arg[0-9]+}}
// REWRITES-NEXT:         + {{arg[0-9]+}}
// REWRITES-NEXT:         + {{arg[0-9]+}}
// REWRITES-NEXT:         + {{arg[0-9]+}}
// REWRITES-NEXT:         + {{arg[0-9]+}}
// REWRITES-NEXT:         + {{arg[0-9]+}}
// REWRITES-NEXT:         + {{arg[0-9]+}}
// REWRITES-NEXT:         + {{arg[0-9]+}}
// REWRITES-NEXT:         + {{arg[0-9]+}}
// REWRITES-NEXT:         + {{arg[0-9]+}}
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn c99_inline_square({{arg[0-9]+}}: i32) -> i32 {
// REWRITES-NEXT:     {{arg[0-9]+}} * {{arg[0-9]+}}
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn c99_qualified_array_sum({{arg[0-9]+}}: *mut i32) -> i32 {
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i32 = unsafe { {{arg[0-9]+}}.add(0) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { *{{_v[0-9]+}} };
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i32 = unsafe { {{arg[0-9]+}}.add(1) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + unsafe { *{{_v[0-9]+}} };
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i32 = unsafe { {{arg[0-9]+}}.add(2) };
// REWRITES-NEXT:     {{_v[0-9]+}} + unsafe { *{{_v[0-9]+}} }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn c99_restrict_sum({{arg[0-9]+}}: &i32, {{arg[0-9]+}}: &i32) -> i32 {
// REWRITES-NEXT:     (unsafe { *({{arg[0-9]+}} as *const i32) }) + unsafe { *({{arg[0-9]+}} as *const i32) }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     safe fn __slate_cf80_div(
// REWRITES-NEXT:         a: num_complex::Complex<LongDouble>,
// REWRITES-NEXT:         b: num_complex::Complex<LongDouble>,
// REWRITES-NEXT:     ) -> num_complex::Complex<LongDouble>;
// REWRITES-NEXT:     safe fn __slate_cf80_mul(
// REWRITES-NEXT:         a: num_complex::Complex<LongDouble>,
// REWRITES-NEXT:         b: num_complex::Complex<LongDouble>,
// REWRITES-NEXT:     ) -> num_complex::Complex<LongDouble>;
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
