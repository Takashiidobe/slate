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

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING: #![feature(f128)]
// COMMON-LOWERING-NEXT: #![feature(c_variadic)]
// COMMON-LOWERING-NEXT: #![allow(
// COMMON-LOWERING-NEXT:     dead_code,
// COMMON-LOWERING-NEXT:     unused,
// COMMON-LOWERING-NEXT:     non_camel_case_types,
// COMMON-LOWERING-NEXT:     non_snake_case,
// COMMON-LOWERING-NEXT:     non_upper_case_globals,
// COMMON-LOWERING-NEXT:     arithmetic_overflow,
// COMMON-LOWERING-NEXT:     unconditional_panic,
// COMMON-LOWERING-NEXT:     suspicious_runtime_symbol_definitions,
// COMMON-LOWERING-NEXT:     unpredictable_function_pointer_comparisons,
// COMMON-LOWERING-NEXT:     unused_comparisons
// COMMON-LOWERING-NEXT: )]
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn __muldc3(__a: f64, __b: f64, __c: f64, __d: f64) -> num_complex::Complex<f64>;
// COMMON-LOWERING-NEXT:     fn __divdc3(__a: f64, __b: f64, __c: f64, __d: f64) -> num_complex::Complex<f64>;
// COMMON-LOWERING-NEXT:     fn __mulsc3(__a: f32, __b: f32, __c: f32, __d: f32) -> num_complex::Complex<f32>;
// COMMON-LOWERING-NEXT:     fn __divsc3(__a: f32, __b: f32, __c: f32, __d: f32) -> num_complex::Complex<f32>;
// COMMON-LOWERING-NEXT:     fn __multc3(__a: f128, __b: f128, __c: f128, __d: f128) -> num_complex::Complex<f128>;
// COMMON-LOWERING-NEXT:     fn __divtc3(__a: f128, __b: f128, __c: f128, __d: f128) -> num_complex::Complex<f128>;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT:         {
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT:         {
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[allow(non_camel_case_types)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy, PartialEq, Eq, Debug, Hash)]
// COMMON-LOWERING-NEXT: enum C99TrailingComma {
// COMMON-LOWERING-NEXT:     C99_ENUM_VALUE = 17,
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct C99Flexible {
// COMMON-LOWERING-NEXT:     count: u64,
// COMMON-LOWERING-NEXT:     values: [i32; 0],
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: struct C99Pair {
// COMMON-LOWERING-NEXT:     first: i32,
// COMMON-LOWERING-NEXT:     second: i32,
// COMMON-LOWERING-NEXT: static mut c99_external_identifier_with_more_than_thirty_one_significant_characters: i32 = 5;
// COMMON-LOWERING-NEXT: static mut slash_comment_value: i32 = 3;
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn malloc(_0: usize) -> *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     fn free(_0: *mut core::ffi::c_void);
// COMMON-LOWERING-NEXT:     fn feclearexcept(_0: i32) -> i32;
// COMMON-LOWERING-NEXT:     fn fetestexcept(_0: i32) -> i32;
// COMMON-LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let mut __retval: i32 = 0;
// COMMON-LOWERING-NEXT:     let mut α: i32 = 0;
// COMMON-LOWERING-NEXT:     let mut enhanced_arithmetic: i32 = 0;
// COMMON-LOWERING-NEXT:     let mut flexible_total: i32 = 0;
// COMMON-LOWERING-NEXT:     let mut flexible: *mut C99Flexible = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let mut length: i32 = 0;
// COMMON-LOWERING-NEXT:     let mut saved_stack: *mut u8 = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let mut vm_total: i32 = 0;
// COMMON-LOWERING-NEXT:     let mut initializer_seed: i32 = 0;
// COMMON-LOWERING-NEXT:     let mut nonconstant_initializer: C99Pair = C99Pair {
// COMMON-LOWERING-NEXT:         first: 0,
// COMMON-LOWERING-NEXT:         second: 0,
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let mut designated_initializer: C99Pair = C99Pair {
// COMMON-LOWERING-NEXT:         first: 0,
// COMMON-LOWERING-NEXT:         second: 0,
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let mut const_value: i32 = 0;
// COMMON-LOWERING-NEXT:     let mut idempotent_const_value: i32 = 0;
// COMMON-LOWERING-NEXT:     let mut volatile_value: i32 = 0;
// COMMON-LOWERING-NEXT:     let mut idempotent_volatile_value: i32 = 0;
// COMMON-LOWERING-NEXT:     let mut restricted_value: i32 = 0;
// COMMON-LOWERING-NEXT:     let mut restricted_pointer: *mut i32 = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let mut hexadecimal_float: f64 = 0.0;
// COMMON-LOWERING-NEXT:     let mut compound_pair: C99Pair = C99Pair {
// COMMON-LOWERING-NEXT:         first: 0,
// COMMON-LOWERING-NEXT:         second: 0,
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let mut compound_array_value: i32 = 0;
// COMMON-LOWERING-NEXT:     let mut _compoundliteral: [i32; 2] = [0; 2];
// COMMON-LOWERING-NEXT:     let mut signed_quotient: i32 = 0;
// COMMON-LOWERING-NEXT:     let mut signed_remainder: i32 = 0;
// COMMON-LOWERING-NEXT:     let mut mixed_order: i32 = 0;
// COMMON-LOWERING-NEXT:     let mut declaration_after_statement: i32 = 0;
// COMMON-LOWERING-NEXT:     let mut for_total: i32 = 0;
// COMMON-LOWERING-NEXT:     let mut qualified_values: [i32; 3] = [0; 3];
// COMMON-LOWERING-NEXT:     let mut macro_total: i32 = 0;
// COMMON-LOWERING-NEXT:     let mut translation_limit_total: i32 = 0;
// COMMON-LOWERING-NEXT:     let mut fenv_clear: i32 = 0;
// COMMON-LOWERING-NEXT:     let mut fenv_flags: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 7;
// COMMON-LOWERING-NEXT:     α = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 4;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = -9000000000i64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 18000000000u64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = 1.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = 2.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = 0.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = 0.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = 1.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} * {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} * {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f32> = num_complex::Complex {
// COMMON-LOWERING-NEXT:         re: {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:         im: {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 3.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 4.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = 0.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = 0.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = 1.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f32> = num_complex::Complex {
// COMMON-LOWERING-NEXT:         re: {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:         im: {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.re as f64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.im as f64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex {
// COMMON-LOWERING-NEXT:         re: {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:         im: {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.re;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.im;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} * {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} * {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex {
// COMMON-LOWERING-NEXT:         re: {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:         im: {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = 0.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = 0.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = 1.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f32> = num_complex::Complex {
// COMMON-LOWERING-NEXT:         re: {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:         im: {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:         re: {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:         im: {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:         re: {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:         im: {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = -9000000000i64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 18000000000u64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = 1.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = 2.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = 0.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = 0.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = 1.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} * {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} * {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f32> = num_complex::Complex {
// COMMON-LOWERING-NEXT:         re: {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:         im: {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 3.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 4.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = 0.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = 0.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = 1.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f32> = num_complex::Complex {
// COMMON-LOWERING-NEXT:         re: {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:         im: {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.re as f64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.im as f64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex {
// COMMON-LOWERING-NEXT:         re: {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:         im: {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.re;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.im;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} * {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} * {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex {
// COMMON-LOWERING-NEXT:         re: {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:         im: {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = 0.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = 0.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = 1.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f32> = num_complex::Complex {
// COMMON-LOWERING-NEXT:         re: {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:         im: {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:         re: {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:         im: {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:         re: {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:         im: {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     enhanced_arithmetic = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     flexible_total = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 8;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 3;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 4;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} * {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe { malloc({{__v[0-9]+}} as usize) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut C99Flexible = {{__v[0-9]+}} as *mut C99Flexible;
// COMMON-LOWERING-NEXT:     flexible = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: *mut C99Flexible = flexible;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: *mut C99Flexible = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 2;
// COMMON-LOWERING-NEXT:             __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:             std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 3;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut C99Flexible = flexible;
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         (*{{__v[0-9]+}}).count = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let mut index: u64 = 0;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: u64 = 0;
// COMMON-LOWERING-NEXT:         index = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: u64 = index;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: *mut C99Flexible = flexible;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: u64 = unsafe { (*{{__v[0-9]+}}).count };
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: u64 = index;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: u64 = index;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: *mut C99Flexible = flexible;
// COMMON-LOWERING-NEXT:                 unsafe {
// COMMON-LOWERING-NEXT:                     *(*{{__v[0-9]+}}).values.as_mut_ptr().add({{__v[0-9]+}} as usize) = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: u64 = index;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: *mut C99Flexible = flexible;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = unsafe { *(*{{__v[0-9]+}}).values.as_mut_ptr().add({{__v[0-9]+}} as usize) };
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = flexible_total;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                 flexible_total = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: u64 = index;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} + 1;
// COMMON-LOWERING-NEXT:             index = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut C99Flexible = flexible;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     unsafe { free({{__v[0-9]+}} as *mut core::ffi::c_void) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 3;
// COMMON-LOWERING-NEXT:     length = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = length;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} as u64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = 0usize as *mut u8;
// COMMON-LOWERING-NEXT:     saved_stack = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let mut variable_length_array: Vec<i32> = vec![0; {{__v[0-9]+}} as usize];
// COMMON-LOWERING-NEXT:             let mut index2: i32 = 0;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:             index2 = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             loop {
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = index2;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = length;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                 if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                     break;
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:                 {
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = index2;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = 4;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = index2;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: *mut i32 =
// COMMON-LOWERING-NEXT:                         unsafe { variable_length_array.as_mut_ptr().offset({{__v[0-9]+}} as isize) };
// COMMON-LOWERING-NEXT:                     unsafe {
// COMMON-LOWERING-NEXT:                         *{{__v[0-9]+}} = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = index2;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// COMMON-LOWERING-NEXT:                 index2 = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = length;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = c99_vm_sum({{__v[0-9]+}}, variable_length_array.as_mut_ptr());
// COMMON-LOWERING-NEXT:         vm_total = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 19;
// COMMON-LOWERING-NEXT:         initializer_seed = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = initializer_seed;
// COMMON-LOWERING-NEXT:         nonconstant_initializer.first = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = initializer_seed;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         nonconstant_initializer.second = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: C99Pair = C99Pair {
// COMMON-LOWERING-NEXT:             first: 22,
// COMMON-LOWERING-NEXT:             second: 23,
// COMMON-LOWERING-NEXT:         };
// COMMON-LOWERING-NEXT:         designated_initializer = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: [i32; 4] = [27, 0, 29, 0];
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 31;
// COMMON-LOWERING-NEXT:         const_value = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = const_value;
// COMMON-LOWERING-NEXT:         idempotent_const_value = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 37;
// COMMON-LOWERING-NEXT:         unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(volatile_value), {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(volatile_value)) };
// COMMON-LOWERING-NEXT:         unsafe {
// COMMON-LOWERING-NEXT:             std::ptr::write_volatile(std::ptr::addr_of_mut!(idempotent_volatile_value), {{__v[0-9]+}})
// COMMON-LOWERING-NEXT:         };
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 41;
// COMMON-LOWERING-NEXT:         restricted_value = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         restricted_pointer = std::ptr::addr_of_mut!(restricted_value);
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: f64 = 3.0;
// COMMON-LOWERING-NEXT:         hexadecimal_float = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: C99Pair = C99Pair {
// COMMON-LOWERING-NEXT:             first: 43,
// COMMON-LOWERING-NEXT:             second: 47,
// COMMON-LOWERING-NEXT:         };
// COMMON-LOWERING-NEXT:         compound_pair = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i64 = 1;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: *mut i32 = _compoundliteral.as_mut_ptr() as *mut i32;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 51;
// COMMON-LOWERING-NEXT:         unsafe {
// COMMON-LOWERING-NEXT:             *{{__v[0-9]+}} = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i64 = 1;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(1) };
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 53;
// COMMON-LOWERING-NEXT:         unsafe {
// COMMON-LOWERING-NEXT:             *{{__v[0-9]+}} = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = _compoundliteral[({{__v[0-9]+}} as usize)];
// COMMON-LOWERING-NEXT:         compound_array_value = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = -7;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 3;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} / {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         signed_quotient = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = -7;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 3;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} % {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         signed_remainder = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 59;
// COMMON-LOWERING-NEXT:         mixed_order = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 2;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = mixed_order;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         mixed_order = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 61;
// COMMON-LOWERING-NEXT:         declaration_after_statement = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:         for_total = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             let mut index3: i32 = 0;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:             index3 = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             loop {
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = index3;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = 3;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                 if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                     break;
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:                 {
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = index3;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = for_total;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     for_total = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = index3;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// COMMON-LOWERING-NEXT:                 index3 = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: [i32; 3] = [2, 3, 5];
// COMMON-LOWERING-NEXT:         qualified_values = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 7;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 11;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 13;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = sum3({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:         macro_total = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = c99_thirty_two_parameters(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:         );
// COMMON-LOWERING-NEXT:         translation_limit_total = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 8;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = unsafe { feclearexcept({{__v[0-9]+}} as i32) };
// COMMON-LOWERING-NEXT:         fenv_clear = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = unsafe { fetestexcept({{__v[0-9]+}} as i32) };
// COMMON-LOWERING-NEXT:         fenv_flags = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = α;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 =
// COMMON-LOWERING-NEXT:             unsafe { c99_external_identifier_with_more_than_thirty_one_significant_characters };
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = unsafe { slash_comment_value };
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = enhanced_arithmetic;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = flexible_total;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = vm_total;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = nonconstant_initializer.first;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = nonconstant_initializer.second;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = designated_initializer.first;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = designated_initializer.second;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i64 = 0;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = designated_array[({{__v[0-9]+}} as usize)];
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i64 = 2;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = designated_array[({{__v[0-9]+}} as usize)];
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = idempotent_const_value;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 =
// COMMON-LOWERING-NEXT:             unsafe { std::ptr::read_volatile(std::ptr::addr_of!(idempotent_volatile_value)) };
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: *mut i32 = restricted_pointer;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: f64 = hexadecimal_float;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = compound_pair.first;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = compound_pair.second;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = compound_array_value;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = signed_quotient;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = signed_remainder;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = mixed_order;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = declaration_after_statement;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = for_total;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = c99_inline_square({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: *mut i32 = qualified_values.as_mut_ptr() as *mut i32;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = c99_qualified_array_sum({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = macro_total;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = translation_limit_total;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = C99TrailingComma::C99_ENUM_VALUE as i32;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = fenv_clear;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = fenv_flags;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i64 = 0;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i64 = 1;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = c99_restrict_sum(
// COMMON-LOWERING-NEXT:             std::ptr::addr_of_mut!(qualified_values[({{__v[0-9]+}} as usize)]),
// COMMON-LOWERING-NEXT:             std::ptr::addr_of_mut!(qualified_values[({{__v[0-9]+}} as usize)]),
// COMMON-LOWERING-NEXT:         );
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:             printf(
// COMMON-LOWERING-NEXT:                 {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:                 {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:                 {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:                 {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:                 {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:                 {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:                 {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:                 {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:                 {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:                 {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:                 {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:                 {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:                 {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:                 {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:                 {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:                 {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:                 {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:                 {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:                 {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:                 {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:                 {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:                 {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:                 {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:                 {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:                 {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:                 {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:                 {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:                 {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:                 {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:                 {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             )
// COMMON-LOWERING-NEXT:         };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: fn c99_vm_sum({{arg[0-9]+}}: i32, {{arg[0-9]+}}: *mut i32) -> i32 {
// COMMON-LOWERING-NEXT:     let mut length: i32 = 0;
// COMMON-LOWERING-NEXT:     let mut values: *mut i32 = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let mut total: i32 = 0;
// COMMON-LOWERING-NEXT:     length = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     values = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     total = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let mut index: i32 = 0;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:         index = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = index;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = length;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = index;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: *mut i32 = values;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = total;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                 total = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = index;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// COMMON-LOWERING-NEXT:             index = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = total;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: fn sum3({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) -> i32 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{arg[0-9]+}} + {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: fn c99_thirty_two_parameters(
// COMMON-LOWERING-NEXT:     {{arg[0-9]+}}: i32,
// COMMON-LOWERING-NEXT:     {{arg[0-9]+}}: i32,
// COMMON-LOWERING-NEXT:     {{arg[0-9]+}}: i32,
// COMMON-LOWERING-NEXT:     {{arg[0-9]+}}: i32,
// COMMON-LOWERING-NEXT:     {{arg[0-9]+}}: i32,
// COMMON-LOWERING-NEXT:     {{arg[0-9]+}}: i32,
// COMMON-LOWERING-NEXT:     {{arg[0-9]+}}: i32,
// COMMON-LOWERING-NEXT:     {{arg[0-9]+}}: i32,
// COMMON-LOWERING-NEXT:     {{arg[0-9]+}}: i32,
// COMMON-LOWERING-NEXT:     {{arg[0-9]+}}: i32,
// COMMON-LOWERING-NEXT:     {{arg[0-9]+}}: i32,
// COMMON-LOWERING-NEXT:     {{arg[0-9]+}}: i32,
// COMMON-LOWERING-NEXT:     {{arg[0-9]+}}: i32,
// COMMON-LOWERING-NEXT:     {{arg[0-9]+}}: i32,
// COMMON-LOWERING-NEXT:     {{arg[0-9]+}}: i32,
// COMMON-LOWERING-NEXT:     {{arg[0-9]+}}: i32,
// COMMON-LOWERING-NEXT:     {{arg[0-9]+}}: i32,
// COMMON-LOWERING-NEXT:     {{arg[0-9]+}}: i32,
// COMMON-LOWERING-NEXT:     {{arg[0-9]+}}: i32,
// COMMON-LOWERING-NEXT:     {{arg[0-9]+}}: i32,
// COMMON-LOWERING-NEXT:     {{arg[0-9]+}}: i32,
// COMMON-LOWERING-NEXT:     {{arg[0-9]+}}: i32,
// COMMON-LOWERING-NEXT:     {{arg[0-9]+}}: i32,
// COMMON-LOWERING-NEXT:     {{arg[0-9]+}}: i32,
// COMMON-LOWERING-NEXT:     {{arg[0-9]+}}: i32,
// COMMON-LOWERING-NEXT:     {{arg[0-9]+}}: i32,
// COMMON-LOWERING-NEXT:     {{arg[0-9]+}}: i32,
// COMMON-LOWERING-NEXT:     {{arg[0-9]+}}: i32,
// COMMON-LOWERING-NEXT:     {{arg[0-9]+}}: i32,
// COMMON-LOWERING-NEXT:     {{arg[0-9]+}}: i32,
// COMMON-LOWERING-NEXT:     {{arg[0-9]+}}: i32,
// COMMON-LOWERING-NEXT:     {{arg[0-9]+}}: i32,
// COMMON-LOWERING-NEXT: ) -> i32 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{arg[0-9]+}} + {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: fn c99_inline_square({{arg[0-9]+}}: i32) -> i32 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{arg[0-9]+}} * {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: fn c99_qualified_array_sum({{arg[0-9]+}}: *mut i32) -> i32 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = unsafe { {{arg[0-9]+}}.add(0) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = unsafe { {{arg[0-9]+}}.add(1) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 2;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = unsafe { {{arg[0-9]+}}.add(2) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: fn c99_restrict_sum({{arg[0-9]+}}: *mut i32, {{arg[0-9]+}}: *mut i32) -> i32 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { *{{arg[0-9]+}} };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { *{{arg[0-9]+}} };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT: #[repr(C, align(16))]
// LOWERING-X86_64-GNU-NEXT: struct LongDouble([u8; 10]);
// LOWERING-X86_64-GNU-NEXT: impl core::ops::Add for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     type Output = LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn add(self, __o: LongDouble) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:         __slate_f80_add(self, __o)
// LOWERING-X86_64-GNU-NEXT: impl core::ops::Sub for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     type Output = LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn sub(self, __o: LongDouble) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:         __slate_f80_sub(self, __o)
// LOWERING-X86_64-GNU-NEXT: impl core::ops::Mul for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     type Output = LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn mul(self, __o: LongDouble) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:         __slate_f80_mul(self, __o)
// LOWERING-X86_64-GNU-NEXT: impl core::ops::Div for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     type Output = LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn div(self, __o: LongDouble) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:         __slate_f80_div(self, __o)
// LOWERING-X86_64-GNU-NEXT: impl core::ops::AddAssign for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     fn add_assign(&mut self, __o: LongDouble) {
// LOWERING-X86_64-GNU-NEXT:             *self = __slate_f80_add(*self, __o);
// LOWERING-X86_64-GNU-NEXT: impl core::ops::SubAssign for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     fn sub_assign(&mut self, __o: LongDouble) {
// LOWERING-X86_64-GNU-NEXT:             *self = __slate_f80_sub(*self, __o);
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT: impl core::ops::MulAssign for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     fn mul_assign(&mut self, __o: LongDouble) {
// LOWERING-X86_64-GNU-NEXT:         {
// LOWERING-X86_64-GNU-NEXT:             *self = __slate_f80_mul(*self, __o);
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT: impl core::ops::DivAssign for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     fn div_assign(&mut self, __o: LongDouble) {
// LOWERING-X86_64-GNU-NEXT:         {
// LOWERING-X86_64-GNU-NEXT:             *self = __slate_f80_div(*self, __o);
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT: impl core::ops::Neg for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     type Output = LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn neg(self) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:         __slate_f80_neg(self)
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT: impl core::cmp::PartialEq for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     fn eq(&self, __other: &LongDouble) -> bool {
// LOWERING-X86_64-GNU-NEXT:         __slate_f80_eq(*self, *__other)
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT: impl core::cmp::PartialOrd for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     fn partial_cmp(&self, __other: &LongDouble) -> Option<std::cmp::Ordering> {
// LOWERING-X86_64-GNU-NEXT:         if __slate_f80_lt(*self, *__other) {
// LOWERING-X86_64-GNU-NEXT:             Some(std::cmp::Ordering::Less)
// LOWERING-X86_64-GNU-NEXT:         } else {
// LOWERING-X86_64-GNU-NEXT:             if __slate_f80_gt(*self, *__other) {
// LOWERING-X86_64-GNU-NEXT:                 Some(std::cmp::Ordering::Greater)
// LOWERING-X86_64-GNU-NEXT:             } else {
// LOWERING-X86_64-GNU-NEXT:                 if __slate_f80_eq(*self, *__other) {
// LOWERING-X86_64-GNU-NEXT:                     Some(std::cmp::Ordering::Equal)
// LOWERING-X86_64-GNU-NEXT:                 } else {
// LOWERING-X86_64-GNU-NEXT:                     None
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[derive(Clone, Copy)]
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT:     let mut designated_array: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 160, 1, 64]);
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 1, 64]);
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_f32({{__v[0-9]+}}.re);
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_f32({{__v[0-9]+}}.im);
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.re;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.im;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}} * {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}} * {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 160, 1, 64]);
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 1, 64]);
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_f32({{__v[0-9]+}}.re);
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_f32({{__v[0-9]+}}.im);
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.re;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}}.im;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}} * {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}} * {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:         {
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:         *designated_array = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:         {
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: i32 = 32;
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: i32 = 4;
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} | {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: i32 = 16;
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} | {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} | {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: i32 = 1;
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} | {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: i32 = 32;
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: i32 = 4;
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} | {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: i32 = 16;
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} | {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: i32 = 8;
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} | {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: i32 = 1;
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} | {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = b"%s %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = b"main\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: i32 = 8;
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: unsafe extern "C" {
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_cf80_div(
// LOWERING-X86_64-GNU-NEXT:         __a: num_complex::Complex<LongDouble>,
// LOWERING-X86_64-GNU-NEXT:         __b: num_complex::Complex<LongDouble>,
// LOWERING-X86_64-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_cf80_mul(
// LOWERING-X86_64-GNU-NEXT:         __a: num_complex::Complex<LongDouble>,
// LOWERING-X86_64-GNU-NEXT:         __b: num_complex::Complex<LongDouble>,
// LOWERING-X86_64-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f128_nexttoward(__from: f128, __toward: f128) -> f128;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_abs(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_acos(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_acosh(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_add(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_asin(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_asinh(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_atan(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_atanh(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_cbrt(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_ceil(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_copysign(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_cos(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_cosh(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_div(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_eq(__a: LongDouble, __b: LongDouble) -> bool;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_exp(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_exp2(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_expm1(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_fdim(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_floor(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_fma(__a: LongDouble, __b: LongDouble, __c: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_fmax(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_fmin(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_fmod(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_fract(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_bool(__a: bool) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_f32(__a: f32) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_f64(__a: f64) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_i128(__a: i128) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_i16(__a: i16) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_i32(__a: i32) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_i64(__a: i64) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_i8(__a: i8) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_u128(__a: u128) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_u16(__a: u16) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_u32(__a: u32) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_u64(__a: u64) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_u8(__a: u8) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_ge(__a: LongDouble, __b: LongDouble) -> bool;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_gt(__a: LongDouble, __b: LongDouble) -> bool;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_hypot(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_is_fp_class(__a: LongDouble, __flags: i32) -> bool;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_le(__a: LongDouble, __b: LongDouble) -> bool;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_log(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_log10(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_log1p(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_log2(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_lt(__a: LongDouble, __b: LongDouble) -> bool;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_mul(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_ne(__a: LongDouble, __b: LongDouble) -> bool;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_nearbyint(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_neg(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_pow(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_powi(__a: LongDouble, __n: i32) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_remainder(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_rint(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_round(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_signbit(__a: LongDouble) -> bool;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_sin(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_sinh(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_sqrt(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_sub(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_tan(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_tanh(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_bool(__a: LongDouble) -> bool;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_f32(__a: LongDouble) -> f32;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_f64(__a: LongDouble) -> f64;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_i128(__a: LongDouble) -> i128;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_i16(__a: LongDouble) -> i16;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_i32(__a: LongDouble) -> i32;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_i64(__a: LongDouble) -> i64;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_i8(__a: LongDouble) -> i8;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_u128(__a: LongDouble) -> u128;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_u16(__a: LongDouble) -> u16;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_u32(__a: LongDouble) -> u32;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_u64(__a: LongDouble) -> u64;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_u8(__a: LongDouble) -> u8;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_trunc(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT: }
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT:     let mut designated_array: [i32; 4] = [0; 4];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 5.000000e+00f128;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 6.000000e+00f128;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.re as f128;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.im as f128;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.re;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.im;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} * {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} * {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 5.000000e+00f128;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 6.000000e+00f128;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.re as f128;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.im as f128;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.re;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}}.im;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} * {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} * {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// LOWERING-AARCH64-GNU-NEXT:         designated_array = {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: i32 = 31;
// LOWERING-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: i32 = 31;
// LOWERING-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = b"%s %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = b"main\0".as_ptr() as *mut u8;
// SLATE-FILECHECK-END lowering-aarch64-gnu

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES: #![feature(f128)]
// COMMON-REWRITES-NEXT: #![feature(c_variadic)]
// COMMON-REWRITES-NEXT: #![allow(
// COMMON-REWRITES-NEXT:     dead_code,
// COMMON-REWRITES-NEXT:     unused,
// COMMON-REWRITES-NEXT:     non_camel_case_types,
// COMMON-REWRITES-NEXT:     non_snake_case,
// COMMON-REWRITES-NEXT:     non_upper_case_globals,
// COMMON-REWRITES-NEXT:     arithmetic_overflow,
// COMMON-REWRITES-NEXT:     unconditional_panic,
// COMMON-REWRITES-NEXT:     suspicious_runtime_symbol_definitions,
// COMMON-REWRITES-NEXT:     unpredictable_function_pointer_comparisons,
// COMMON-REWRITES-NEXT:     unused_comparisons
// COMMON-REWRITES-NEXT: )]
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn __muldc3(__a: f64, __b: f64, __c: f64, __d: f64) -> num_complex::Complex<f64>;
// COMMON-REWRITES-NEXT:     fn __divdc3(__a: f64, __b: f64, __c: f64, __d: f64) -> num_complex::Complex<f64>;
// COMMON-REWRITES-NEXT:     fn __mulsc3(__a: f32, __b: f32, __c: f32, __d: f32) -> num_complex::Complex<f32>;
// COMMON-REWRITES-NEXT:     fn __divsc3(__a: f32, __b: f32, __c: f32, __d: f32) -> num_complex::Complex<f32>;
// COMMON-REWRITES-NEXT:     fn __multc3(__a: f128, __b: f128, __c: f128, __d: f128) -> num_complex::Complex<f128>;
// COMMON-REWRITES-NEXT:     fn __divtc3(__a: f128, __b: f128, __c: f128, __d: f128) -> num_complex::Complex<f128>;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[allow(non_camel_case_types)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy, PartialEq, Eq, Debug, Hash)]
// COMMON-REWRITES-NEXT: enum C99TrailingComma {
// COMMON-REWRITES-NEXT:     C99_ENUM_VALUE = 17,
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct C99Flexible {
// COMMON-REWRITES-NEXT:     count: u64,
// COMMON-REWRITES-NEXT:     values: [i32; 0],
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: struct C99Pair {
// COMMON-REWRITES-NEXT:     first: i32,
// COMMON-REWRITES-NEXT:     second: i32,
// COMMON-REWRITES-NEXT: static mut c99_external_identifier_with_more_than_thirty_one_significant_characters: i32 = 5;
// COMMON-REWRITES-NEXT: static mut slash_comment_value: i32 = 3;
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn malloc(_0: usize) -> *mut core::ffi::c_void;
// COMMON-REWRITES-NEXT:     fn free(_0: *mut core::ffi::c_void);
// COMMON-REWRITES-NEXT:     fn feclearexcept(_0: i32) -> i32;
// COMMON-REWRITES-NEXT:     fn fetestexcept(_0: i32) -> i32;
// COMMON-REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     let mut __retval: i32 = 0;
// COMMON-REWRITES-NEXT:     let mut α: i32 = 7;
// COMMON-REWRITES-NEXT:     let mut enhanced_arithmetic: i32 = 0;
// COMMON-REWRITES-NEXT:     let mut flexible_total: i32 = 0;
// COMMON-REWRITES-NEXT:     let mut flexible: *mut C99Flexible = std::ptr::null_mut();
// COMMON-REWRITES-NEXT:     let mut length: i32 = 3;
// COMMON-REWRITES-NEXT:     let mut vm_total: i32 = 0;
// COMMON-REWRITES-NEXT:     let mut initializer_seed: i32 = 19;
// COMMON-REWRITES-NEXT:     let mut nonconstant_initializer: C99Pair = C99Pair {
// COMMON-REWRITES-NEXT:         first: 0,
// COMMON-REWRITES-NEXT:         second: 0,
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let mut designated_initializer: C99Pair = C99Pair {
// COMMON-REWRITES-NEXT:         first: 0,
// COMMON-REWRITES-NEXT:         second: 0,
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let mut const_value: i32 = 31;
// COMMON-REWRITES-NEXT:     let mut idempotent_const_value: i32 = 0;
// COMMON-REWRITES-NEXT:     let mut volatile_value: i32 = 0;
// COMMON-REWRITES-NEXT:     let mut idempotent_volatile_value: i32 = 0;
// COMMON-REWRITES-NEXT:     let mut restricted_value: i32 = 41;
// COMMON-REWRITES-NEXT:     let mut restricted_pointer: *mut i32 = std::ptr::null_mut();
// COMMON-REWRITES-NEXT:     let mut hexadecimal_float: f64 = 0.0;
// COMMON-REWRITES-NEXT:     let mut compound_pair: C99Pair = C99Pair {
// COMMON-REWRITES-NEXT:         first: 0,
// COMMON-REWRITES-NEXT:         second: 0,
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let mut compound_array_value: i32 = 0;
// COMMON-REWRITES-NEXT:     let mut _compoundliteral: [i32; 2] = [0; 2];
// COMMON-REWRITES-NEXT:     let mut signed_quotient: i32 = 0;
// COMMON-REWRITES-NEXT:     let mut signed_remainder: i32 = 0;
// COMMON-REWRITES-NEXT:     let mut mixed_order: i32 = 59;
// COMMON-REWRITES-NEXT:     let mut declaration_after_statement: i32 = 61;
// COMMON-REWRITES-NEXT:     let mut for_total: i32 = 0;
// COMMON-REWRITES-NEXT:     let mut qualified_values: [i32; 3] = [0; 3];
// COMMON-REWRITES-NEXT:     let mut macro_total: i32 = 0;
// COMMON-REWRITES-NEXT:     let mut translation_limit_total: i32 = 0;
// COMMON-REWRITES-NEXT:     let mut fenv_clear: i32 = 0;
// COMMON-REWRITES-NEXT:     let mut fenv_flags: i32 = 0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 4;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = -9000000000i64;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = 18000000000u64;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f32 = 2.0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f32 = 0.0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f32> = num_complex::Complex {
// COMMON-REWRITES-NEXT:         re: 1.0 + {{__v[0-9]+}} * (0.0 + {{__v[0-9]+}}),
// COMMON-REWRITES-NEXT:         im: {{__v[0-9]+}} * 1.0,
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = 4.0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f32 = 0.0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f32 = 1.0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f32> = num_complex::Complex {
// COMMON-REWRITES-NEXT:         re: 0.0 + {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:         im: {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex {
// COMMON-REWRITES-NEXT:         re: {{__v[0-9]+}}.re as f64,
// COMMON-REWRITES-NEXT:         im: {{__v[0-9]+}}.im as f64,
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex {
// COMMON-REWRITES-NEXT:         re: 3.0 + {{__v[0-9]+}} * {{__v[0-9]+}}.re,
// COMMON-REWRITES-NEXT:         im: {{__v[0-9]+}} * {{__v[0-9]+}}.im,
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f32 = 0.0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f32 = 1.0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f32> = num_complex::Complex {
// COMMON-REWRITES-NEXT:         re: 0.0 + {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:         im: {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:         im: {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:         re: {{__v[0-9]+}} + {{__v[0-9]+}} * {{__v[0-9]+}}.re,
// COMMON-REWRITES-NEXT:         im: {{__v[0-9]+}} * {{__v[0-9]+}}.im,
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f32 = 2.0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = 4.0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f32 = 0.0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f32 = 1.0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f32> = num_complex::Complex {
// COMMON-REWRITES-NEXT:         re: 0.0 + {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:         im: {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex {
// COMMON-REWRITES-NEXT:         re: {{__v[0-9]+}}.re as f64,
// COMMON-REWRITES-NEXT:         im: {{__v[0-9]+}}.im as f64,
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = ({{__v[0-9]+}} as i32)
// COMMON-REWRITES-NEXT:         + (({{__v[0-9]+}} == -9000000000i64) as i32)
// COMMON-REWRITES-NEXT:         + (({{__v[0-9]+}} == 18000000000u64) as i32)
// COMMON-REWRITES-NEXT:         + (({{__v[0-9]+}}
// COMMON-REWRITES-NEXT:             == num_complex::Complex {
// COMMON-REWRITES-NEXT:                 re: 1.0 + {{__v[0-9]+}} * (0.0 + (0.0 as f32)),
// COMMON-REWRITES-NEXT:                 im: {{__v[0-9]+}} * 1.0,
// COMMON-REWRITES-NEXT:             }) as i32)
// COMMON-REWRITES-NEXT:         + (({{__v[0-9]+}}
// COMMON-REWRITES-NEXT:             == num_complex::Complex {
// COMMON-REWRITES-NEXT:                 re: 3.0 + {{__v[0-9]+}} * {{__v[0-9]+}}.re,
// COMMON-REWRITES-NEXT:                 im: {{__v[0-9]+}} * {{__v[0-9]+}}.im,
// COMMON-REWRITES-NEXT:             }) as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f32 = 0.0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f32 = 1.0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f32> = num_complex::Complex {
// COMMON-REWRITES-NEXT:         re: 0.0 + {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     enhanced_arithmetic = {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:         + (({{__v[0-9]+}}
// COMMON-REWRITES-NEXT:             == num_complex::Complex {
// COMMON-REWRITES-NEXT:                 re: {{__v[0-9]+}} + {{__v[0-9]+}} * {{__v[0-9]+}}.re,
// COMMON-REWRITES-NEXT:                 im: {{__v[0-9]+}} * {{__v[0-9]+}}.im,
// COMMON-REWRITES-NEXT:             }) as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = 4;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = 8 + 3 * {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe { malloc({{__v[0-9]+}} as usize) };
// COMMON-REWRITES-NEXT:     flexible = {{__v[0-9]+}} as *mut C99Flexible;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = flexible == std::ptr::null_mut();
// COMMON-REWRITES-NEXT:     if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         __retval = 2;
// COMMON-REWRITES-NEXT:         std::process::exit(__retval as i32);
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         (*flexible).count = 3;
// COMMON-REWRITES-NEXT:     for index in 0..unsafe { (*flexible).count } {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = (index as i32) + 1;
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: u64 = index;
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: *mut C99Flexible = flexible;
// COMMON-REWRITES-NEXT:         unsafe {
// COMMON-REWRITES-NEXT:             *(*{{__v[0-9]+}}).values.as_mut_ptr().add({{__v[0-9]+}} as usize) = {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: u64 = index;
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: *mut C99Flexible = flexible;
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = unsafe { *(*{{__v[0-9]+}}).values.as_mut_ptr().add({{__v[0-9]+}} as usize) };
// COMMON-REWRITES-NEXT:         flexible_total += {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     unsafe { free(flexible as *mut core::ffi::c_void) };
// COMMON-REWRITES-NEXT:     let mut variable_length_array: Vec<i32> = vec![0; (length as u64) as usize];
// COMMON-REWRITES-NEXT:     for index2 in 0..length {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = index2 + 4;
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: *mut i32 = unsafe {
// COMMON-REWRITES-NEXT:             variable_length_array
// COMMON-REWRITES-NEXT:                 .as_mut_ptr()
// COMMON-REWRITES-NEXT:                 .offset((index2 as i64) as isize)
// COMMON-REWRITES-NEXT:         };
// COMMON-REWRITES-NEXT:         unsafe {
// COMMON-REWRITES-NEXT:             *{{__v[0-9]+}} = {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     vm_total = c99_vm_sum(length, variable_length_array.as_mut_ptr());
// COMMON-REWRITES-NEXT:     nonconstant_initializer.first = initializer_seed;
// COMMON-REWRITES-NEXT:     nonconstant_initializer.second = initializer_seed + 1;
// COMMON-REWRITES-NEXT:     designated_initializer = C99Pair {
// COMMON-REWRITES-NEXT:         first: 22,
// COMMON-REWRITES-NEXT:         second: 23,
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     idempotent_const_value = const_value;
// COMMON-REWRITES-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(volatile_value), 37 as i32) };
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         std::ptr::write_volatile(std::ptr::addr_of_mut!(idempotent_volatile_value), unsafe {
// COMMON-REWRITES-NEXT:             std::ptr::read_volatile(std::ptr::addr_of!(volatile_value))
// COMMON-REWRITES-NEXT:         })
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     restricted_pointer = std::ptr::addr_of_mut!(restricted_value);
// COMMON-REWRITES-NEXT:     hexadecimal_float = 3.0;
// COMMON-REWRITES-NEXT:     compound_pair = C99Pair {
// COMMON-REWRITES-NEXT:         first: 43,
// COMMON-REWRITES-NEXT:         second: 47,
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i32 = _compoundliteral.as_mut_ptr() as *mut i32;
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         *{{__v[0-9]+}} = 51;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(1) };
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         *{{__v[0-9]+}} = 53;
// COMMON-REWRITES-NEXT:     compound_array_value = _compoundliteral[1];
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 3;
// COMMON-REWRITES-NEXT:     signed_quotient = -7 / {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 3;
// COMMON-REWRITES-NEXT:     signed_remainder = -7 % {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     mixed_order += 2;
// COMMON-REWRITES-NEXT:     for index3 in 0..3 {
// COMMON-REWRITES-NEXT:         for_total += index3;
// COMMON-REWRITES-NEXT:     qualified_values = [2, 3, 5];
// COMMON-REWRITES-NEXT:     macro_total = sum3(7 as i32, 11 as i32, 13 as i32);
// COMMON-REWRITES-NEXT:     translation_limit_total = c99_thirty_two_parameters(
// COMMON-REWRITES-NEXT:         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
// COMMON-REWRITES-NEXT:         1, 1,
// COMMON-REWRITES-NEXT:     );
// COMMON-REWRITES-NEXT:         c"%s %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n"
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = α;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 =
// COMMON-REWRITES-NEXT:         unsafe { c99_external_identifier_with_more_than_thirty_one_significant_characters };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { slash_comment_value };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = enhanced_arithmetic;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = flexible_total;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = vm_total;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = nonconstant_initializer.first + nonconstant_initializer.second;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = designated_initializer.first + designated_initializer.second;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = designated_array[0] + designated_array[2];
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = idempotent_const_value;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 =
// COMMON-REWRITES-NEXT:         unsafe { std::ptr::read_volatile(std::ptr::addr_of!(idempotent_volatile_value)) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { *restricted_pointer };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = hexadecimal_float as i32;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = compound_pair.first + compound_pair.second;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = compound_array_value;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = signed_quotient;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = signed_remainder;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = mixed_order;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = declaration_after_statement;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = for_total;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = c99_inline_square(8);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i32 = qualified_values.as_mut_ptr() as *mut i32;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = macro_total;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = translation_limit_total;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = C99TrailingComma::C99_ENUM_VALUE as i32;
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         printf(
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             c99_qualified_array_sum({{__v[0-9]+}}),
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             fenv_clear,
// COMMON-REWRITES-NEXT:             fenv_flags,
// COMMON-REWRITES-NEXT:             c99_restrict_sum(
// COMMON-REWRITES-NEXT:                 unsafe { &(*std::ptr::addr_of_mut!(qualified_values[0])) },
// COMMON-REWRITES-NEXT:                 unsafe { &(*std::ptr::addr_of_mut!(qualified_values[1])) },
// COMMON-REWRITES-NEXT:             ),
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     std::process::exit(__retval as i32);
// COMMON-REWRITES-NEXT: fn c99_vm_sum({{arg[0-9]+}}: i32, {{arg[0-9]+}}: *mut i32) -> i32 {
// COMMON-REWRITES-NEXT:     let __arg40_view = unsafe { std::slice::from_raw_parts({{arg[0-9]+}} as *const i32, {{arg[0-9]+}} as usize) };
// COMMON-REWRITES-NEXT:     let mut length: i32 = {{arg[0-9]+}};
// COMMON-REWRITES-NEXT:     let mut values: *mut i32 = {{arg[0-9]+}};
// COMMON-REWRITES-NEXT:     let mut total: i32 = 0;
// COMMON-REWRITES-NEXT:     let mut index: i32 = 0;
// COMMON-REWRITES-NEXT:     while index < length {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: *mut i32 = values;
// COMMON-REWRITES-NEXT:         unsafe { {{__v[0-9]+}}.offset((index as i64) as isize) };
// COMMON-REWRITES-NEXT:         total += unsafe { __arg40_view[(index as usize)] };
// COMMON-REWRITES-NEXT:         index += 1;
// COMMON-REWRITES-NEXT:     total
// COMMON-REWRITES-NEXT: fn sum3({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) -> i32 {
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}} + {{arg[0-9]+}} + {{arg[0-9]+}}
// COMMON-REWRITES-NEXT: fn c99_thirty_two_parameters(
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}}: i32,
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}}: i32,
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}}: i32,
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}}: i32,
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}}: i32,
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}}: i32,
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}}: i32,
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}}: i32,
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}}: i32,
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}}: i32,
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}}: i32,
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}}: i32,
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}}: i32,
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}}: i32,
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}}: i32,
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}}: i32,
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}}: i32,
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}}: i32,
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}}: i32,
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}}: i32,
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}}: i32,
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}}: i32,
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}}: i32,
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}}: i32,
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}}: i32,
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}}: i32,
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}}: i32,
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}}: i32,
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}}: i32,
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}}: i32,
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}}: i32,
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}}: i32,
// COMMON-REWRITES-NEXT: ) -> i32 {
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}} + {{arg[0-9]+}}
// COMMON-REWRITES-NEXT:         + {{arg[0-9]+}}
// COMMON-REWRITES-NEXT:         + {{arg[0-9]+}}
// COMMON-REWRITES-NEXT:         + {{arg[0-9]+}}
// COMMON-REWRITES-NEXT:         + {{arg[0-9]+}}
// COMMON-REWRITES-NEXT:         + {{arg[0-9]+}}
// COMMON-REWRITES-NEXT:         + {{arg[0-9]+}}
// COMMON-REWRITES-NEXT:         + {{arg[0-9]+}}
// COMMON-REWRITES-NEXT:         + {{arg[0-9]+}}
// COMMON-REWRITES-NEXT:         + {{arg[0-9]+}}
// COMMON-REWRITES-NEXT:         + {{arg[0-9]+}}
// COMMON-REWRITES-NEXT:         + {{arg[0-9]+}}
// COMMON-REWRITES-NEXT:         + {{arg[0-9]+}}
// COMMON-REWRITES-NEXT:         + {{arg[0-9]+}}
// COMMON-REWRITES-NEXT:         + {{arg[0-9]+}}
// COMMON-REWRITES-NEXT:         + {{arg[0-9]+}}
// COMMON-REWRITES-NEXT:         + {{arg[0-9]+}}
// COMMON-REWRITES-NEXT:         + {{arg[0-9]+}}
// COMMON-REWRITES-NEXT:         + {{arg[0-9]+}}
// COMMON-REWRITES-NEXT:         + {{arg[0-9]+}}
// COMMON-REWRITES-NEXT:         + {{arg[0-9]+}}
// COMMON-REWRITES-NEXT:         + {{arg[0-9]+}}
// COMMON-REWRITES-NEXT:         + {{arg[0-9]+}}
// COMMON-REWRITES-NEXT:         + {{arg[0-9]+}}
// COMMON-REWRITES-NEXT:         + {{arg[0-9]+}}
// COMMON-REWRITES-NEXT:         + {{arg[0-9]+}}
// COMMON-REWRITES-NEXT:         + {{arg[0-9]+}}
// COMMON-REWRITES-NEXT:         + {{arg[0-9]+}}
// COMMON-REWRITES-NEXT:         + {{arg[0-9]+}}
// COMMON-REWRITES-NEXT:         + {{arg[0-9]+}}
// COMMON-REWRITES-NEXT:         + {{arg[0-9]+}}
// COMMON-REWRITES-NEXT: fn c99_inline_square({{arg[0-9]+}}: i32) -> i32 {
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}} * {{arg[0-9]+}}
// COMMON-REWRITES-NEXT: fn c99_qualified_array_sum({{arg[0-9]+}}: *mut i32) -> i32 {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i32 = unsafe { {{arg[0-9]+}}.add(0) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i32 = unsafe { {{arg[0-9]+}}.add(1) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + unsafe { *{{__v[0-9]+}} };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i32 = unsafe { {{arg[0-9]+}}.add(2) };
// COMMON-REWRITES-NEXT:     {{__v[0-9]+}} + unsafe { *{{__v[0-9]+}} }
// COMMON-REWRITES-NEXT: fn c99_restrict_sum({{arg[0-9]+}}: &i32, {{arg[0-9]+}}: &i32) -> i32 {
// COMMON-REWRITES-NEXT:     (unsafe { *({{arg[0-9]+}} as *const i32) }) + unsafe { *({{arg[0-9]+}} as *const i32) }
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-NEXT: #[repr(C, align(16))]
// REWRITES-X86_64-GNU-NEXT: struct LongDouble([u8; 10]);
// REWRITES-X86_64-GNU-NEXT: impl core::ops::Add for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     type Output = LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn add(self, __o: LongDouble) -> LongDouble {
// REWRITES-X86_64-GNU-NEXT:         __slate_f80_add(self, __o)
// REWRITES-X86_64-GNU-NEXT: impl core::ops::Sub for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     type Output = LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn sub(self, __o: LongDouble) -> LongDouble {
// REWRITES-X86_64-GNU-NEXT:         __slate_f80_sub(self, __o)
// REWRITES-X86_64-GNU-NEXT: impl core::ops::Mul for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     type Output = LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn mul(self, __o: LongDouble) -> LongDouble {
// REWRITES-X86_64-GNU-NEXT:         __slate_f80_mul(self, __o)
// REWRITES-X86_64-GNU-NEXT: impl core::ops::Div for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     type Output = LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn div(self, __o: LongDouble) -> LongDouble {
// REWRITES-X86_64-GNU-NEXT:         __slate_f80_div(self, __o)
// REWRITES-X86_64-GNU-NEXT: impl core::ops::AddAssign for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     fn add_assign(&mut self, __o: LongDouble) {
// REWRITES-X86_64-GNU-NEXT:         {
// REWRITES-X86_64-GNU-NEXT:             *self = __slate_f80_add(*self, __o);
// REWRITES-X86_64-GNU-NEXT: impl core::ops::SubAssign for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     fn sub_assign(&mut self, __o: LongDouble) {
// REWRITES-X86_64-GNU-NEXT:         {
// REWRITES-X86_64-GNU-NEXT:             *self = __slate_f80_sub(*self, __o);
// REWRITES-X86_64-GNU-NEXT: impl core::ops::MulAssign for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     fn mul_assign(&mut self, __o: LongDouble) {
// REWRITES-X86_64-GNU-NEXT:         {
// REWRITES-X86_64-GNU-NEXT:             *self = __slate_f80_mul(*self, __o);
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT: impl core::ops::DivAssign for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     fn div_assign(&mut self, __o: LongDouble) {
// REWRITES-X86_64-GNU-NEXT:         {
// REWRITES-X86_64-GNU-NEXT:             *self = __slate_f80_div(*self, __o);
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT: impl core::ops::Neg for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     type Output = LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn neg(self) -> LongDouble {
// REWRITES-X86_64-GNU-NEXT:         __slate_f80_neg(self)
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT: impl core::cmp::PartialEq for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     fn eq(&self, __other: &LongDouble) -> bool {
// REWRITES-X86_64-GNU-NEXT:         __slate_f80_eq(*self, *__other)
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT: impl core::cmp::PartialOrd for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     fn partial_cmp(&self, __other: &LongDouble) -> Option<std::cmp::Ordering> {
// REWRITES-X86_64-GNU-NEXT:         if __slate_f80_lt(*self, *__other) {
// REWRITES-X86_64-GNU-NEXT:             Some(std::cmp::Ordering::Less)
// REWRITES-X86_64-GNU-NEXT:         } else {
// REWRITES-X86_64-GNU-NEXT:             if __slate_f80_gt(*self, *__other) {
// REWRITES-X86_64-GNU-NEXT:                 Some(std::cmp::Ordering::Greater)
// REWRITES-X86_64-GNU-NEXT:             } else {
// REWRITES-X86_64-GNU-NEXT:                 if __slate_f80_eq(*self, *__other) {
// REWRITES-X86_64-GNU-NEXT:                     Some(std::cmp::Ordering::Equal)
// REWRITES-X86_64-GNU-NEXT:                 } else {
// REWRITES-X86_64-GNU-NEXT:                     None
// REWRITES-X86_64-GNU-NEXT:                 }
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[derive(Clone, Copy)]
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT:     let mut designated_array: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 160, 1, 64]);
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 1, 64]);
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_f32({{__v[0-9]+}}.re);
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_f32({{__v[0-9]+}}.im);
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-X86_64-GNU-NEXT:         re: {{__v[0-9]+}},
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 160, 1, 64]);
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 1, 64]);
// REWRITES-X86_64-GNU-NEXT:         im: {{__v[0-9]+}},
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_f32({{__v[0-9]+}}.re);
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_f32({{__v[0-9]+}}.im);
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<LongDouble> = num_complex::Complex {
// REWRITES-X86_64-GNU-NEXT:         re: {{__v[0-9]+}},
// REWRITES-X86_64-GNU-NEXT:         im: {{__v[0-9]+}},
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     *designated_array = [27, 0, 29, 0];
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = 4;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = 32 | {{__v[0-9]+}} | 16 | 8 | 1;
// REWRITES-X86_64-GNU-NEXT:     fenv_clear = unsafe { feclearexcept({{__v[0-9]+}} as i32) };
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = 4;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = 32 | {{__v[0-9]+}} | 16 | 8 | 1;
// REWRITES-X86_64-GNU-NEXT:     fenv_flags = unsafe { fetestexcept({{__v[0-9]+}} as i32) };
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 =
// REWRITES-X86_64-GNU-NEXT:             .as_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = c"main".as_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: unsafe extern "C" {
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_cf80_div(
// REWRITES-X86_64-GNU-NEXT:         __a: num_complex::Complex<LongDouble>,
// REWRITES-X86_64-GNU-NEXT:         __b: num_complex::Complex<LongDouble>,
// REWRITES-X86_64-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_cf80_mul(
// REWRITES-X86_64-GNU-NEXT:         __a: num_complex::Complex<LongDouble>,
// REWRITES-X86_64-GNU-NEXT:         __b: num_complex::Complex<LongDouble>,
// REWRITES-X86_64-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f128_nexttoward(__from: f128, __toward: f128) -> f128;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_abs(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_acos(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_acosh(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_add(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_asin(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_asinh(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_atan(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_atanh(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_cbrt(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_ceil(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_copysign(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_cos(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_cosh(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_div(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_eq(__a: LongDouble, __b: LongDouble) -> bool;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_exp(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_exp2(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_expm1(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_fdim(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_floor(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_fma(__a: LongDouble, __b: LongDouble, __c: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_fmax(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_fmin(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_fmod(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_fract(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_bool(__a: bool) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_f32(__a: f32) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_f64(__a: f64) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_i128(__a: i128) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_i16(__a: i16) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_i32(__a: i32) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_i64(__a: i64) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_i8(__a: i8) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_u128(__a: u128) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_u16(__a: u16) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_u32(__a: u32) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_u64(__a: u64) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_u8(__a: u8) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_ge(__a: LongDouble, __b: LongDouble) -> bool;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_gt(__a: LongDouble, __b: LongDouble) -> bool;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_hypot(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_is_fp_class(__a: LongDouble, __flags: i32) -> bool;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_le(__a: LongDouble, __b: LongDouble) -> bool;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_log(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_log10(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_log1p(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_log2(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_lt(__a: LongDouble, __b: LongDouble) -> bool;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_mul(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_ne(__a: LongDouble, __b: LongDouble) -> bool;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_nearbyint(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_neg(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_pow(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_powi(__a: LongDouble, __n: i32) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_remainder(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_rint(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_round(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_signbit(__a: LongDouble) -> bool;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_sin(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_sinh(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_sqrt(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_sub(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_tan(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_tanh(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_bool(__a: LongDouble) -> bool;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_f32(__a: LongDouble) -> f32;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_f64(__a: LongDouble) -> f64;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_i128(__a: LongDouble) -> i128;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_i16(__a: LongDouble) -> i16;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_i32(__a: LongDouble) -> i32;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_i64(__a: LongDouble) -> i64;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_i8(__a: LongDouble) -> i8;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_u128(__a: LongDouble) -> u128;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_u16(__a: LongDouble) -> u16;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_u32(__a: LongDouble) -> u32;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_u64(__a: LongDouble) -> u64;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_u8(__a: LongDouble) -> u8;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_trunc(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT: }
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-NEXT:     let mut designated_array: [i32; 4] = [0; 4];
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 5.000000e+00f128;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 6.000000e+00f128;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// REWRITES-AARCH64-GNU-NEXT:         re: {{__v[0-9]+}}.re as f128,
// REWRITES-AARCH64-GNU-NEXT:         im: {{__v[0-9]+}}.im as f128,
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 5.000000e+00f128;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 6.000000e+00f128;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: num_complex::Complex<f128> = num_complex::Complex {
// REWRITES-AARCH64-GNU-NEXT:         re: {{__v[0-9]+}}.re as f128,
// REWRITES-AARCH64-GNU-NEXT:         im: {{__v[0-9]+}}.im as f128,
// REWRITES-AARCH64-GNU-NEXT:     designated_array = [27, 0, 29, 0];
// REWRITES-AARCH64-GNU-NEXT:     fenv_clear = unsafe { feclearexcept(31 as i32) };
// REWRITES-AARCH64-GNU-NEXT:     fenv_flags = unsafe { fetestexcept(31 as i32) };
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 =
// REWRITES-AARCH64-GNU-NEXT:             .as_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = c"main".as_ptr() as *mut u8;
// SLATE-FILECHECK-END rewrites-aarch64-gnu
