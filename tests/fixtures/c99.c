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
// LOWERING-NEXT: enum C99TrailingComma {
// LOWERING-NEXT:     C99_ENUM_VALUE = 17,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct C99Flexible {
// LOWERING-NEXT:     count: u64,
// LOWERING-NEXT:     values: [i32; 0],
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct C99Pair {
// LOWERING-NEXT:     first: i32,
// LOWERING-NEXT:     second: i32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
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
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn c99_vm_sum(arg39: i32, arg40: *mut i32) -> i32 {
// LOWERING-NEXT:     let mut length: i32 = 0;
// LOWERING-NEXT:     let mut values: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut total: i32 = 0;
// LOWERING-NEXT:     length = arg39;
// LOWERING-NEXT:     values = arg40;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     total = _v0;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut index: i32 = 0;
// LOWERING-NEXT:         let _v1: i32 = 0;
// LOWERING-NEXT:         index = _v1;
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let _v2: i32 = index;
// LOWERING-NEXT:             let _v3: i32 = length;
// LOWERING-NEXT:             let _v4: bool = _v2 < _v3;
// LOWERING-NEXT:             if !_v4 {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 let _v5: i32 = index;
// LOWERING-NEXT:                 let _v6: i64 = _v5 as i64;
// LOWERING-NEXT:                 let _v7: *mut i32 = values;
// LOWERING-NEXT:                 let _v8: *mut i32 = unsafe { _v7.offset(_v6 as isize) };
// LOWERING-NEXT:                 let _v9: i32 = unsafe { *_v8 };
// LOWERING-NEXT:                 let _v10: i32 = total;
// LOWERING-NEXT:                 let _v11: i32 = _v10 + _v9;
// LOWERING-NEXT:                 total = _v11;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let _v12: i32 = index;
// LOWERING-NEXT:             let _v13: i32 = _v12 + 1;
// LOWERING-NEXT:             index = _v13;
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v14: i32 = total;
// LOWERING-NEXT:     __retval = _v14;
// LOWERING-NEXT:     let _v15: i32 = __retval;
// LOWERING-NEXT:     return _v15;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn sum3(arg36: i32, arg37: i32, arg38: i32) -> i32 {
// LOWERING-NEXT:     let mut first: i32 = 0;
// LOWERING-NEXT:     let mut second: i32 = 0;
// LOWERING-NEXT:     let mut third: i32 = 0;
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     first = arg36;
// LOWERING-NEXT:     second = arg37;
// LOWERING-NEXT:     third = arg38;
// LOWERING-NEXT:     let _v0: i32 = first;
// LOWERING-NEXT:     let _v1: i32 = second;
// LOWERING-NEXT:     let _v2: i32 = _v0 + _v1;
// LOWERING-NEXT:     let _v3: i32 = third;
// LOWERING-NEXT:     let _v4: i32 = _v2 + _v3;
// LOWERING-NEXT:     __retval = _v4;
// LOWERING-NEXT:     let _v5: i32 = __retval;
// LOWERING-NEXT:     return _v5;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn c99_thirty_two_parameters(arg4: i32, arg5: i32, arg6: i32, arg7: i32, arg8: i32, arg9: i32, arg10: i32, arg11: i32, arg12: i32, arg13: i32, arg14: i32, arg15: i32, arg16: i32, arg17: i32, arg18: i32, arg19: i32, arg20: i32, arg21: i32, arg22: i32, arg23: i32, arg24: i32, arg25: i32, arg26: i32, arg27: i32, arg28: i32, arg29: i32, arg30: i32, arg31: i32, arg32: i32, arg33: i32, arg34: i32, arg35: i32) -> i32 {
// LOWERING-NEXT:     let mut p01: i32 = 0;
// LOWERING-NEXT:     let mut p02: i32 = 0;
// LOWERING-NEXT:     let mut p03: i32 = 0;
// LOWERING-NEXT:     let mut p04: i32 = 0;
// LOWERING-NEXT:     let mut p05: i32 = 0;
// LOWERING-NEXT:     let mut p06: i32 = 0;
// LOWERING-NEXT:     let mut p07: i32 = 0;
// LOWERING-NEXT:     let mut p08: i32 = 0;
// LOWERING-NEXT:     let mut p09: i32 = 0;
// LOWERING-NEXT:     let mut p10: i32 = 0;
// LOWERING-NEXT:     let mut p11: i32 = 0;
// LOWERING-NEXT:     let mut p12: i32 = 0;
// LOWERING-NEXT:     let mut p13: i32 = 0;
// LOWERING-NEXT:     let mut p14: i32 = 0;
// LOWERING-NEXT:     let mut p15: i32 = 0;
// LOWERING-NEXT:     let mut p16: i32 = 0;
// LOWERING-NEXT:     let mut p17: i32 = 0;
// LOWERING-NEXT:     let mut p18: i32 = 0;
// LOWERING-NEXT:     let mut p19: i32 = 0;
// LOWERING-NEXT:     let mut p20: i32 = 0;
// LOWERING-NEXT:     let mut p21: i32 = 0;
// LOWERING-NEXT:     let mut p22: i32 = 0;
// LOWERING-NEXT:     let mut p23: i32 = 0;
// LOWERING-NEXT:     let mut p24: i32 = 0;
// LOWERING-NEXT:     let mut p25: i32 = 0;
// LOWERING-NEXT:     let mut p26: i32 = 0;
// LOWERING-NEXT:     let mut p27: i32 = 0;
// LOWERING-NEXT:     let mut p28: i32 = 0;
// LOWERING-NEXT:     let mut p29: i32 = 0;
// LOWERING-NEXT:     let mut p30: i32 = 0;
// LOWERING-NEXT:     let mut p31: i32 = 0;
// LOWERING-NEXT:     let mut p32: i32 = 0;
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     p01 = arg4;
// LOWERING-NEXT:     p02 = arg5;
// LOWERING-NEXT:     p03 = arg6;
// LOWERING-NEXT:     p04 = arg7;
// LOWERING-NEXT:     p05 = arg8;
// LOWERING-NEXT:     p06 = arg9;
// LOWERING-NEXT:     p07 = arg10;
// LOWERING-NEXT:     p08 = arg11;
// LOWERING-NEXT:     p09 = arg12;
// LOWERING-NEXT:     p10 = arg13;
// LOWERING-NEXT:     p11 = arg14;
// LOWERING-NEXT:     p12 = arg15;
// LOWERING-NEXT:     p13 = arg16;
// LOWERING-NEXT:     p14 = arg17;
// LOWERING-NEXT:     p15 = arg18;
// LOWERING-NEXT:     p16 = arg19;
// LOWERING-NEXT:     p17 = arg20;
// LOWERING-NEXT:     p18 = arg21;
// LOWERING-NEXT:     p19 = arg22;
// LOWERING-NEXT:     p20 = arg23;
// LOWERING-NEXT:     p21 = arg24;
// LOWERING-NEXT:     p22 = arg25;
// LOWERING-NEXT:     p23 = arg26;
// LOWERING-NEXT:     p24 = arg27;
// LOWERING-NEXT:     p25 = arg28;
// LOWERING-NEXT:     p26 = arg29;
// LOWERING-NEXT:     p27 = arg30;
// LOWERING-NEXT:     p28 = arg31;
// LOWERING-NEXT:     p29 = arg32;
// LOWERING-NEXT:     p30 = arg33;
// LOWERING-NEXT:     p31 = arg34;
// LOWERING-NEXT:     p32 = arg35;
// LOWERING-NEXT:     let _v0: i32 = p01;
// LOWERING-NEXT:     let _v1: i32 = p02;
// LOWERING-NEXT:     let _v2: i32 = _v0 + _v1;
// LOWERING-NEXT:     let _v3: i32 = p03;
// LOWERING-NEXT:     let _v4: i32 = _v2 + _v3;
// LOWERING-NEXT:     let _v5: i32 = p04;
// LOWERING-NEXT:     let _v6: i32 = _v4 + _v5;
// LOWERING-NEXT:     let _v7: i32 = p05;
// LOWERING-NEXT:     let _v8: i32 = _v6 + _v7;
// LOWERING-NEXT:     let _v9: i32 = p06;
// LOWERING-NEXT:     let _v10: i32 = _v8 + _v9;
// LOWERING-NEXT:     let _v11: i32 = p07;
// LOWERING-NEXT:     let _v12: i32 = _v10 + _v11;
// LOWERING-NEXT:     let _v13: i32 = p08;
// LOWERING-NEXT:     let _v14: i32 = _v12 + _v13;
// LOWERING-NEXT:     let _v15: i32 = p09;
// LOWERING-NEXT:     let _v16: i32 = _v14 + _v15;
// LOWERING-NEXT:     let _v17: i32 = p10;
// LOWERING-NEXT:     let _v18: i32 = _v16 + _v17;
// LOWERING-NEXT:     let _v19: i32 = p11;
// LOWERING-NEXT:     let _v20: i32 = _v18 + _v19;
// LOWERING-NEXT:     let _v21: i32 = p12;
// LOWERING-NEXT:     let _v22: i32 = _v20 + _v21;
// LOWERING-NEXT:     let _v23: i32 = p13;
// LOWERING-NEXT:     let _v24: i32 = _v22 + _v23;
// LOWERING-NEXT:     let _v25: i32 = p14;
// LOWERING-NEXT:     let _v26: i32 = _v24 + _v25;
// LOWERING-NEXT:     let _v27: i32 = p15;
// LOWERING-NEXT:     let _v28: i32 = _v26 + _v27;
// LOWERING-NEXT:     let _v29: i32 = p16;
// LOWERING-NEXT:     let _v30: i32 = _v28 + _v29;
// LOWERING-NEXT:     let _v31: i32 = p17;
// LOWERING-NEXT:     let _v32: i32 = _v30 + _v31;
// LOWERING-NEXT:     let _v33: i32 = p18;
// LOWERING-NEXT:     let _v34: i32 = _v32 + _v33;
// LOWERING-NEXT:     let _v35: i32 = p19;
// LOWERING-NEXT:     let _v36: i32 = _v34 + _v35;
// LOWERING-NEXT:     let _v37: i32 = p20;
// LOWERING-NEXT:     let _v38: i32 = _v36 + _v37;
// LOWERING-NEXT:     let _v39: i32 = p21;
// LOWERING-NEXT:     let _v40: i32 = _v38 + _v39;
// LOWERING-NEXT:     let _v41: i32 = p22;
// LOWERING-NEXT:     let _v42: i32 = _v40 + _v41;
// LOWERING-NEXT:     let _v43: i32 = p23;
// LOWERING-NEXT:     let _v44: i32 = _v42 + _v43;
// LOWERING-NEXT:     let _v45: i32 = p24;
// LOWERING-NEXT:     let _v46: i32 = _v44 + _v45;
// LOWERING-NEXT:     let _v47: i32 = p25;
// LOWERING-NEXT:     let _v48: i32 = _v46 + _v47;
// LOWERING-NEXT:     let _v49: i32 = p26;
// LOWERING-NEXT:     let _v50: i32 = _v48 + _v49;
// LOWERING-NEXT:     let _v51: i32 = p27;
// LOWERING-NEXT:     let _v52: i32 = _v50 + _v51;
// LOWERING-NEXT:     let _v53: i32 = p28;
// LOWERING-NEXT:     let _v54: i32 = _v52 + _v53;
// LOWERING-NEXT:     let _v55: i32 = p29;
// LOWERING-NEXT:     let _v56: i32 = _v54 + _v55;
// LOWERING-NEXT:     let _v57: i32 = p30;
// LOWERING-NEXT:     let _v58: i32 = _v56 + _v57;
// LOWERING-NEXT:     let _v59: i32 = p31;
// LOWERING-NEXT:     let _v60: i32 = _v58 + _v59;
// LOWERING-NEXT:     let _v61: i32 = p32;
// LOWERING-NEXT:     let _v62: i32 = _v60 + _v61;
// LOWERING-NEXT:     __retval = _v62;
// LOWERING-NEXT:     let _v63: i32 = __retval;
// LOWERING-NEXT:     return _v63;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn c99_inline_square(arg3: i32) -> i32 {
// LOWERING-NEXT:     let mut value: i32 = 0;
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     value = arg3;
// LOWERING-NEXT:     let _v0: i32 = value;
// LOWERING-NEXT:     let _v1: i32 = value;
// LOWERING-NEXT:     let _v2: i32 = _v0 * _v1;
// LOWERING-NEXT:     __retval = _v2;
// LOWERING-NEXT:     let _v3: i32 = __retval;
// LOWERING-NEXT:     return _v3;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn c99_qualified_array_sum(arg2: *mut i32) -> i32 {
// LOWERING-NEXT:     let mut values: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     values = arg2;
// LOWERING-NEXT:     let _v0: i64 = 0;
// LOWERING-NEXT:     let _v1: *mut i32 = values;
// LOWERING-NEXT:     let _v2: *mut i32 = unsafe { _v1.add(0) };
// LOWERING-NEXT:     let _v3: i32 = unsafe { *_v2 };
// LOWERING-NEXT:     let _v4: i64 = 1;
// LOWERING-NEXT:     let _v5: *mut i32 = values;
// LOWERING-NEXT:     let _v6: *mut i32 = unsafe { _v5.add(1) };
// LOWERING-NEXT:     let _v7: i32 = unsafe { *_v6 };
// LOWERING-NEXT:     let _v8: i32 = _v3 + _v7;
// LOWERING-NEXT:     let _v9: i64 = 2;
// LOWERING-NEXT:     let _v10: *mut i32 = values;
// LOWERING-NEXT:     let _v11: *mut i32 = unsafe { _v10.add(2) };
// LOWERING-NEXT:     let _v12: i32 = unsafe { *_v11 };
// LOWERING-NEXT:     let _v13: i32 = _v8 + _v12;
// LOWERING-NEXT:     __retval = _v13;
// LOWERING-NEXT:     let _v14: i32 = __retval;
// LOWERING-NEXT:     return _v14;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn c99_restrict_sum(arg0: *mut i32, arg1: *mut i32) -> i32 {
// LOWERING-NEXT:     let mut left: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut right: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     left = arg0;
// LOWERING-NEXT:     right = arg1;
// LOWERING-NEXT:     let _v0: *mut i32 = left;
// LOWERING-NEXT:     let _v1: i32 = unsafe { *_v0 };
// LOWERING-NEXT:     let _v2: *mut i32 = right;
// LOWERING-NEXT:     let _v3: i32 = unsafe { *_v2 };
// LOWERING-NEXT:     let _v4: i32 = _v1 + _v3;
// LOWERING-NEXT:     __retval = _v4;
// LOWERING-NEXT:     let _v5: i32 = __retval;
// LOWERING-NEXT:     return _v5;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut α: i32 = 0;
// LOWERING-NEXT:     let mut boolean_value: bool = false;
// LOWERING-NEXT:     let mut signed_long_long: i64 = 0;
// LOWERING-NEXT:     let mut unsigned_long_long: u64 = 0;
// LOWERING-NEXT:     let mut float_complex: num_complex::Complex<f32> = num_complex::Complex { re: 0.0, im: 0.0 };
// LOWERING-NEXT:     let mut double_complex: num_complex::Complex<f64> = num_complex::Complex { re: 0.0, im: 0.0 };
// LOWERING-NEXT:     let mut long_double_complex: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// LOWERING-NEXT:     let mut enhanced_arithmetic: i32 = 0;
// LOWERING-NEXT:     let mut flexible_total: i32 = 0;
// LOWERING-NEXT:     let mut flexible: *mut C99Flexible = std::ptr::null_mut();
// LOWERING-NEXT:     let mut length: i32 = 0;
// LOWERING-NEXT:     let mut saved_stack: *mut u8 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut vm_total: i32 = 0;
// LOWERING-NEXT:     let mut initializer_seed: i32 = 0;
// LOWERING-NEXT:     let mut nonconstant_initializer: C99Pair = C99Pair { first: 0, second: 0 };
// LOWERING-NEXT:     let mut designated_initializer: C99Pair = C99Pair { first: 0, second: 0 };
// LOWERING-NEXT:     let mut designated_array: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// LOWERING-NEXT:     let mut const_value: i32 = 0;
// LOWERING-NEXT:     let mut idempotent_const_value: i32 = 0;
// LOWERING-NEXT:     let mut volatile_value: i32 = 0;
// LOWERING-NEXT:     let mut idempotent_volatile_value: i32 = 0;
// LOWERING-NEXT:     let mut restricted_value: i32 = 0;
// LOWERING-NEXT:     let mut restricted_pointer: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut hexadecimal_float: f64 = 0.0;
// LOWERING-NEXT:     let mut compound_pair: C99Pair = C99Pair { first: 0, second: 0 };
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
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: i32 = 7;
// LOWERING-NEXT:     α = _v1;
// LOWERING-NEXT:     let _v2: i32 = 4;
// LOWERING-NEXT:     let _v3: bool = _v2 != 0;
// LOWERING-NEXT:     boolean_value = _v3;
// LOWERING-NEXT:     let _v4: i64 = -9000000000i64;
// LOWERING-NEXT:     signed_long_long = _v4;
// LOWERING-NEXT:     let _v5: u64 = 18000000000u64;
// LOWERING-NEXT:     unsigned_long_long = _v5;
// LOWERING-NEXT:     let _v6: f32 = 1.0;
// LOWERING-NEXT:     let _v7: f32 = 2.0;
// LOWERING-NEXT:     let _v8: f32 = 0.0;
// LOWERING-NEXT:     let _v9: f32 = 0.0;
// LOWERING-NEXT:     let _v10: f32 = 1.0;
// LOWERING-NEXT:     let _v11: f32 = _v8 + _v9;
// LOWERING-NEXT:     let _v12: f32 = _v7 * _v11;
// LOWERING-NEXT:     let _v13: f32 = _v7 * _v10;
// LOWERING-NEXT:     let _v14: f32 = _v6 + _v12;
// LOWERING-NEXT:     let _v15: num_complex::Complex<f32> = num_complex::Complex { re: _v14, im: _v13 };
// LOWERING-NEXT:     float_complex = _v15;
// LOWERING-NEXT:     let _v16: f64 = 3.0;
// LOWERING-NEXT:     let _v17: f64 = 4.0;
// LOWERING-NEXT:     let _v18: f32 = 0.0;
// LOWERING-NEXT:     let _v19: f32 = 0.0;
// LOWERING-NEXT:     let _v20: f32 = 1.0;
// LOWERING-NEXT:     let _v21: f32 = _v18 + _v19;
// LOWERING-NEXT:     let _v22: num_complex::Complex<f32> = num_complex::Complex { re: _v21, im: _v20 };
// LOWERING-NEXT:     let _v23: f32 = _v22.re;
// LOWERING-NEXT:     let _v24: f32 = _v22.im;
// LOWERING-NEXT:     let _v25: f64 = _v23 as f64;
// LOWERING-NEXT:     let _v26: f64 = _v24 as f64;
// LOWERING-NEXT:     let _v27: num_complex::Complex<f64> = num_complex::Complex { re: _v25, im: _v26 };
// LOWERING-NEXT:     let _v28: f64 = _v27.re;
// LOWERING-NEXT:     let _v29: f64 = _v27.im;
// LOWERING-NEXT:     let _v30: f64 = _v17 * _v28;
// LOWERING-NEXT:     let _v31: f64 = _v17 * _v29;
// LOWERING-NEXT:     let _v32: f64 = _v16 + _v30;
// LOWERING-NEXT:     let _v33: num_complex::Complex<f64> = num_complex::Complex { re: _v32, im: _v31 };
// LOWERING-NEXT:     double_complex = _v33;
// LOWERING-NEXT:     let _v34: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 160, 1, 64]);
// LOWERING-NEXT:     let _v35: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 1, 64]);
// LOWERING-NEXT:     let _v36: f32 = 0.0;
// LOWERING-NEXT:     let _v37: f32 = 0.0;
// LOWERING-NEXT:     let _v38: f32 = 1.0;
// LOWERING-NEXT:     let _v39: f32 = _v36 + _v37;
// LOWERING-NEXT:     let _v40: num_complex::Complex<f32> = num_complex::Complex { re: _v39, im: _v38 };
// LOWERING-NEXT:     let _v41: f32 = _v40.re;
// LOWERING-NEXT:     let _v42: f32 = _v40.im;
// LOWERING-NEXT:     let _v43: LongDouble = __slate_f80_from_f32(_v41);
// LOWERING-NEXT:     let _v44: LongDouble = __slate_f80_from_f32(_v42);
// LOWERING-NEXT:     let _v45: num_complex::Complex<LongDouble> = num_complex::Complex { re: _v43, im: _v44 };
// LOWERING-NEXT:     let _v46: LongDouble = _v45.re;
// LOWERING-NEXT:     let _v47: LongDouble = _v45.im;
// LOWERING-NEXT:     let _v48: LongDouble = _v35 * _v46;
// LOWERING-NEXT:     let _v49: LongDouble = _v35 * _v47;
// LOWERING-NEXT:     let _v50: LongDouble = _v34 + _v48;
// LOWERING-NEXT:     let _v51: num_complex::Complex<LongDouble> = num_complex::Complex { re: _v50, im: _v49 };
// LOWERING-NEXT:     long_double_complex = _v51;
// LOWERING-NEXT:     let _v52: bool = boolean_value;
// LOWERING-NEXT:     let _v53: i32 = _v52 as i32;
// LOWERING-NEXT:     let _v54: i64 = signed_long_long;
// LOWERING-NEXT:     let _v55: i64 = -9000000000i64;
// LOWERING-NEXT:     let _v56: bool = _v54 == _v55;
// LOWERING-NEXT:     let _v57: i32 = _v56 as i32;
// LOWERING-NEXT:     let _v58: i32 = _v53 + _v57;
// LOWERING-NEXT:     let _v59: u64 = unsigned_long_long;
// LOWERING-NEXT:     let _v60: u64 = 18000000000u64;
// LOWERING-NEXT:     let _v61: bool = _v59 == _v60;
// LOWERING-NEXT:     let _v62: i32 = _v61 as i32;
// LOWERING-NEXT:     let _v63: i32 = _v58 + _v62;
// LOWERING-NEXT:     let _v64: num_complex::Complex<f32> = float_complex;
// LOWERING-NEXT:     let _v65: f32 = 1.0;
// LOWERING-NEXT:     let _v66: f32 = 2.0;
// LOWERING-NEXT:     let _v67: f32 = 0.0;
// LOWERING-NEXT:     let _v68: f32 = 0.0;
// LOWERING-NEXT:     let _v69: f32 = 1.0;
// LOWERING-NEXT:     let _v70: f32 = _v67 + _v68;
// LOWERING-NEXT:     let _v71: f32 = _v66 * _v70;
// LOWERING-NEXT:     let _v72: f32 = _v66 * _v69;
// LOWERING-NEXT:     let _v73: f32 = _v65 + _v71;
// LOWERING-NEXT:     let _v74: num_complex::Complex<f32> = num_complex::Complex { re: _v73, im: _v72 };
// LOWERING-NEXT:     let _v75: bool = _v64 == _v74;
// LOWERING-NEXT:     let _v76: i32 = _v75 as i32;
// LOWERING-NEXT:     let _v77: i32 = _v63 + _v76;
// LOWERING-NEXT:     let _v78: num_complex::Complex<f64> = double_complex;
// LOWERING-NEXT:     let _v79: f64 = 3.0;
// LOWERING-NEXT:     let _v80: f64 = 4.0;
// LOWERING-NEXT:     let _v81: f32 = 0.0;
// LOWERING-NEXT:     let _v82: f32 = 0.0;
// LOWERING-NEXT:     let _v83: f32 = 1.0;
// LOWERING-NEXT:     let _v84: f32 = _v81 + _v82;
// LOWERING-NEXT:     let _v85: num_complex::Complex<f32> = num_complex::Complex { re: _v84, im: _v83 };
// LOWERING-NEXT:     let _v86: f32 = _v85.re;
// LOWERING-NEXT:     let _v87: f32 = _v85.im;
// LOWERING-NEXT:     let _v88: f64 = _v86 as f64;
// LOWERING-NEXT:     let _v89: f64 = _v87 as f64;
// LOWERING-NEXT:     let _v90: num_complex::Complex<f64> = num_complex::Complex { re: _v88, im: _v89 };
// LOWERING-NEXT:     let _v91: f64 = _v90.re;
// LOWERING-NEXT:     let _v92: f64 = _v90.im;
// LOWERING-NEXT:     let _v93: f64 = _v80 * _v91;
// LOWERING-NEXT:     let _v94: f64 = _v80 * _v92;
// LOWERING-NEXT:     let _v95: f64 = _v79 + _v93;
// LOWERING-NEXT:     let _v96: num_complex::Complex<f64> = num_complex::Complex { re: _v95, im: _v94 };
// LOWERING-NEXT:     let _v97: bool = _v78 == _v96;
// LOWERING-NEXT:     let _v98: i32 = _v97 as i32;
// LOWERING-NEXT:     let _v99: i32 = _v77 + _v98;
// LOWERING-NEXT:     let _v100: num_complex::Complex<LongDouble> = long_double_complex;
// LOWERING-NEXT:     let _v101: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 160, 1, 64]);
// LOWERING-NEXT:     let _v102: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 1, 64]);
// LOWERING-NEXT:     let _v103: f32 = 0.0;
// LOWERING-NEXT:     let _v104: f32 = 0.0;
// LOWERING-NEXT:     let _v105: f32 = 1.0;
// LOWERING-NEXT:     let _v106: f32 = _v103 + _v104;
// LOWERING-NEXT:     let _v107: num_complex::Complex<f32> = num_complex::Complex { re: _v106, im: _v105 };
// LOWERING-NEXT:     let _v108: f32 = _v107.re;
// LOWERING-NEXT:     let _v109: f32 = _v107.im;
// LOWERING-NEXT:     let _v110: LongDouble = __slate_f80_from_f32(_v108);
// LOWERING-NEXT:     let _v111: LongDouble = __slate_f80_from_f32(_v109);
// LOWERING-NEXT:     let _v112: num_complex::Complex<LongDouble> = num_complex::Complex { re: _v110, im: _v111 };
// LOWERING-NEXT:     let _v113: LongDouble = _v112.re;
// LOWERING-NEXT:     let _v114: LongDouble = _v112.im;
// LOWERING-NEXT:     let _v115: LongDouble = _v102 * _v113;
// LOWERING-NEXT:     let _v116: LongDouble = _v102 * _v114;
// LOWERING-NEXT:     let _v117: LongDouble = _v101 + _v115;
// LOWERING-NEXT:     let _v118: num_complex::Complex<LongDouble> = num_complex::Complex { re: _v117, im: _v116 };
// LOWERING-NEXT:     let _v119: bool = _v100 == _v118;
// LOWERING-NEXT:     let _v120: i32 = _v119 as i32;
// LOWERING-NEXT:     let _v121: i32 = _v99 + _v120;
// LOWERING-NEXT:     enhanced_arithmetic = _v121;
// LOWERING-NEXT:     let _v122: i32 = 0;
// LOWERING-NEXT:     flexible_total = _v122;
// LOWERING-NEXT:     let _v123: u64 = 8;
// LOWERING-NEXT:     let _v124: u64 = 3;
// LOWERING-NEXT:     let _v125: u64 = 4;
// LOWERING-NEXT:     let _v126: u64 = _v124 * _v125;
// LOWERING-NEXT:     let _v127: u64 = _v123 + _v126;
// LOWERING-NEXT:     let _v128: *mut core::ffi::c_void = unsafe { malloc(_v127 as usize) };
// LOWERING-NEXT:     let _v129: *mut C99Flexible = _v128 as *mut C99Flexible;
// LOWERING-NEXT:     flexible = _v129;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let _v130: *mut C99Flexible = flexible;
// LOWERING-NEXT:         let _v131: *mut C99Flexible = std::ptr::null_mut();
// LOWERING-NEXT:         let _v132: bool = _v130 == _v131;
// LOWERING-NEXT:         if _v132 {
// LOWERING-NEXT:             let _v133: i32 = 2;
// LOWERING-NEXT:             __retval = _v133;
// LOWERING-NEXT:             let _v134: i32 = __retval;
// LOWERING-NEXT:             std::process::exit(_v134 as i32);
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v135: u64 = 3;
// LOWERING-NEXT:     let _v136: *mut C99Flexible = flexible;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*_v136).count = _v135;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut index: u64 = 0;
// LOWERING-NEXT:         let _v137: u64 = 0;
// LOWERING-NEXT:         index = _v137;
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let _v138: u64 = index;
// LOWERING-NEXT:             let _v139: *mut C99Flexible = flexible;
// LOWERING-NEXT:             let _v140: u64 = unsafe { (*_v139).count };
// LOWERING-NEXT:             let _v141: bool = _v138 < _v140;
// LOWERING-NEXT:             if !_v141 {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 let _v142: u64 = index;
// LOWERING-NEXT:                 let _v143: i32 = _v142 as i32;
// LOWERING-NEXT:                 let _v144: i32 = 1;
// LOWERING-NEXT:                 let _v145: i32 = _v143 + _v144;
// LOWERING-NEXT:                 let _v146: u64 = index;
// LOWERING-NEXT:                 let _v147: *mut C99Flexible = flexible;
// LOWERING-NEXT:                 unsafe {
// LOWERING-NEXT:                     *(*_v147).values.as_mut_ptr().add(_v146 as usize) = _v145;
// LOWERING-NEXT:                 }
// LOWERING-NEXT:                 let _v148: u64 = index;
// LOWERING-NEXT:                 let _v149: *mut C99Flexible = flexible;
// LOWERING-NEXT:                 let _v150: i32 = unsafe { *(*_v149).values.as_mut_ptr().add(_v148 as usize) };
// LOWERING-NEXT:                 let _v151: i32 = flexible_total;
// LOWERING-NEXT:                 let _v152: i32 = _v151 + _v150;
// LOWERING-NEXT:                 flexible_total = _v152;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let _v153: u64 = index;
// LOWERING-NEXT:             let _v154: u64 = _v153 + 1;
// LOWERING-NEXT:             index = _v154;
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v155: *mut C99Flexible = flexible;
// LOWERING-NEXT:     let _v156: *mut core::ffi::c_void = _v155 as *mut core::ffi::c_void;
// LOWERING-NEXT:     unsafe { free(_v156 as *mut core::ffi::c_void) };
// LOWERING-NEXT:     let _v157: i32 = 3;
// LOWERING-NEXT:     length = _v157;
// LOWERING-NEXT:     let _v158: i32 = length;
// LOWERING-NEXT:     let _v159: u64 = _v158 as u64;
// LOWERING-NEXT:     let _v160: *mut u8 = 0usize as *mut u8;
// LOWERING-NEXT:     saved_stack = _v160;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut variable_length_array: Vec<i32> = vec![0; _v159 as usize];
// LOWERING-NEXT:         {
// LOWERING-NEXT:             let mut index2: i32 = 0;
// LOWERING-NEXT:             let _v161: i32 = 0;
// LOWERING-NEXT:             index2 = _v161;
// LOWERING-NEXT:             loop {
// LOWERING-NEXT:                 let _v162: i32 = index2;
// LOWERING-NEXT:                 let _v163: i32 = length;
// LOWERING-NEXT:                 let _v164: bool = _v162 < _v163;
// LOWERING-NEXT:                 if !_v164 {
// LOWERING-NEXT:                     break;
// LOWERING-NEXT:                 }
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let _v165: i32 = index2;
// LOWERING-NEXT:                     let _v166: i32 = 4;
// LOWERING-NEXT:                     let _v167: i32 = _v165 + _v166;
// LOWERING-NEXT:                     let _v168: i32 = index2;
// LOWERING-NEXT:                     let _v169: i64 = _v168 as i64;
// LOWERING-NEXT:                     let _v170: *mut i32 = unsafe { variable_length_array.as_mut_ptr().offset(_v169 as isize) };
// LOWERING-NEXT:                     unsafe {
// LOWERING-NEXT:                         *_v170 = _v167;
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:                 let _v171: i32 = index2;
// LOWERING-NEXT:                 let _v172: i32 = _v171 + 1;
// LOWERING-NEXT:                 index2 = _v172;
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:         let _v173: i32 = length;
// LOWERING-NEXT:         let _v174: i32 = c99_vm_sum(_v173, variable_length_array.as_mut_ptr());
// LOWERING-NEXT:         vm_total = _v174;
// LOWERING-NEXT:         let _v175: i32 = 19;
// LOWERING-NEXT:         initializer_seed = _v175;
// LOWERING-NEXT:         let _v176: i32 = initializer_seed;
// LOWERING-NEXT:         nonconstant_initializer.first = _v176;
// LOWERING-NEXT:         let _v177: i32 = initializer_seed;
// LOWERING-NEXT:         let _v178: i32 = 1;
// LOWERING-NEXT:         let _v179: i32 = _v177 + _v178;
// LOWERING-NEXT:         nonconstant_initializer.second = _v179;
// LOWERING-NEXT:         designated_initializer = C99Pair { first: 22, second: 23 };
// LOWERING-NEXT:         *designated_array = [27, 0, 29, 0];
// LOWERING-NEXT:         let _v180: i32 = 31;
// LOWERING-NEXT:         const_value = _v180;
// LOWERING-NEXT:         let _v181: i32 = const_value;
// LOWERING-NEXT:         idempotent_const_value = _v181;
// LOWERING-NEXT:         let _v182: i32 = 37;
// LOWERING-NEXT:         unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(volatile_value), _v182) };
// LOWERING-NEXT:         let _v183: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(volatile_value)) };
// LOWERING-NEXT:         unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(idempotent_volatile_value), _v183) };
// LOWERING-NEXT:         let _v184: i32 = 41;
// LOWERING-NEXT:         restricted_value = _v184;
// LOWERING-NEXT:         restricted_pointer = std::ptr::addr_of_mut!(restricted_value);
// LOWERING-NEXT:         let _v185: f64 = 3.0;
// LOWERING-NEXT:         hexadecimal_float = _v185;
// LOWERING-NEXT:         compound_pair = C99Pair { first: 43, second: 47 };
// LOWERING-NEXT:         let _v186: i64 = 1;
// LOWERING-NEXT:         let _v187: *mut i32 = _compoundliteral.as_mut_ptr() as *mut i32;
// LOWERING-NEXT:         let _v188: i32 = 51;
// LOWERING-NEXT:         unsafe {
// LOWERING-NEXT:             *_v187 = _v188;
// LOWERING-NEXT:         }
// LOWERING-NEXT:         let _v189: i64 = 1;
// LOWERING-NEXT:         let _v190: *mut i32 = unsafe { _v187.add(1) };
// LOWERING-NEXT:         let _v191: i32 = 53;
// LOWERING-NEXT:         unsafe {
// LOWERING-NEXT:             *_v190 = _v191;
// LOWERING-NEXT:         }
// LOWERING-NEXT:         let _v192: i32 = _compoundliteral[(_v186 as usize)];
// LOWERING-NEXT:         compound_array_value = _v192;
// LOWERING-NEXT:         let _v193: i32 = -7;
// LOWERING-NEXT:         let _v194: i32 = 3;
// LOWERING-NEXT:         let _v195: i32 = _v193 / _v194;
// LOWERING-NEXT:         signed_quotient = _v195;
// LOWERING-NEXT:         let _v196: i32 = -7;
// LOWERING-NEXT:         let _v197: i32 = 3;
// LOWERING-NEXT:         let _v198: i32 = _v196 % _v197;
// LOWERING-NEXT:         signed_remainder = _v198;
// LOWERING-NEXT:         let _v199: i32 = 59;
// LOWERING-NEXT:         mixed_order = _v199;
// LOWERING-NEXT:         let _v200: i32 = 2;
// LOWERING-NEXT:         let _v201: i32 = mixed_order;
// LOWERING-NEXT:         let _v202: i32 = _v201 + _v200;
// LOWERING-NEXT:         mixed_order = _v202;
// LOWERING-NEXT:         let _v203: i32 = 61;
// LOWERING-NEXT:         declaration_after_statement = _v203;
// LOWERING-NEXT:         let _v204: i32 = 0;
// LOWERING-NEXT:         for_total = _v204;
// LOWERING-NEXT:         {
// LOWERING-NEXT:             let mut index3: i32 = 0;
// LOWERING-NEXT:             let _v205: i32 = 0;
// LOWERING-NEXT:             index3 = _v205;
// LOWERING-NEXT:             loop {
// LOWERING-NEXT:                 let _v206: i32 = index3;
// LOWERING-NEXT:                 let _v207: i32 = 3;
// LOWERING-NEXT:                 let _v208: bool = _v206 < _v207;
// LOWERING-NEXT:                 if !_v208 {
// LOWERING-NEXT:                     break;
// LOWERING-NEXT:                 }
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let _v209: i32 = index3;
// LOWERING-NEXT:                     let _v210: i32 = for_total;
// LOWERING-NEXT:                     let _v211: i32 = _v210 + _v209;
// LOWERING-NEXT:                     for_total = _v211;
// LOWERING-NEXT:                 }
// LOWERING-NEXT:                 let _v212: i32 = index3;
// LOWERING-NEXT:                 let _v213: i32 = _v212 + 1;
// LOWERING-NEXT:                 index3 = _v213;
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:         qualified_values = [2, 3, 5];
// LOWERING-NEXT:         let _v214: i32 = 7;
// LOWERING-NEXT:         let _v215: i32 = 11;
// LOWERING-NEXT:         let _v216: i32 = 13;
// LOWERING-NEXT:         let _v217: i32 = sum3(_v214, _v215, _v216);
// LOWERING-NEXT:         macro_total = _v217;
// LOWERING-NEXT:         let _v218: i32 = 1;
// LOWERING-NEXT:         let _v219: i32 = 1;
// LOWERING-NEXT:         let _v220: i32 = 1;
// LOWERING-NEXT:         let _v221: i32 = 1;
// LOWERING-NEXT:         let _v222: i32 = 1;
// LOWERING-NEXT:         let _v223: i32 = 1;
// LOWERING-NEXT:         let _v224: i32 = 1;
// LOWERING-NEXT:         let _v225: i32 = 1;
// LOWERING-NEXT:         let _v226: i32 = 1;
// LOWERING-NEXT:         let _v227: i32 = 1;
// LOWERING-NEXT:         let _v228: i32 = 1;
// LOWERING-NEXT:         let _v229: i32 = 1;
// LOWERING-NEXT:         let _v230: i32 = 1;
// LOWERING-NEXT:         let _v231: i32 = 1;
// LOWERING-NEXT:         let _v232: i32 = 1;
// LOWERING-NEXT:         let _v233: i32 = 1;
// LOWERING-NEXT:         let _v234: i32 = 1;
// LOWERING-NEXT:         let _v235: i32 = 1;
// LOWERING-NEXT:         let _v236: i32 = 1;
// LOWERING-NEXT:         let _v237: i32 = 1;
// LOWERING-NEXT:         let _v238: i32 = 1;
// LOWERING-NEXT:         let _v239: i32 = 1;
// LOWERING-NEXT:         let _v240: i32 = 1;
// LOWERING-NEXT:         let _v241: i32 = 1;
// LOWERING-NEXT:         let _v242: i32 = 1;
// LOWERING-NEXT:         let _v243: i32 = 1;
// LOWERING-NEXT:         let _v244: i32 = 1;
// LOWERING-NEXT:         let _v245: i32 = 1;
// LOWERING-NEXT:         let _v246: i32 = 1;
// LOWERING-NEXT:         let _v247: i32 = 1;
// LOWERING-NEXT:         let _v248: i32 = 1;
// LOWERING-NEXT:         let _v249: i32 = 1;
// LOWERING-NEXT:         let _v250: i32 = c99_thirty_two_parameters(_v218, _v219, _v220, _v221, _v222, _v223, _v224, _v225, _v226, _v227, _v228, _v229, _v230, _v231, _v232, _v233, _v234, _v235, _v236, _v237, _v238, _v239, _v240, _v241, _v242, _v243, _v244, _v245, _v246, _v247, _v248, _v249);
// LOWERING-NEXT:         translation_limit_total = _v250;
// LOWERING-NEXT:         let _v251: i32 = 32;
// LOWERING-NEXT:         let _v252: i32 = 4;
// LOWERING-NEXT:         let _v253: i32 = _v251 | _v252;
// LOWERING-NEXT:         let _v254: i32 = 16;
// LOWERING-NEXT:         let _v255: i32 = _v253 | _v254;
// LOWERING-NEXT:         let _v256: i32 = 8;
// LOWERING-NEXT:         let _v257: i32 = _v255 | _v256;
// LOWERING-NEXT:         let _v258: i32 = 1;
// LOWERING-NEXT:         let _v259: i32 = _v257 | _v258;
// LOWERING-NEXT:         let _v260: i32 = unsafe { feclearexcept(_v259 as i32) };
// LOWERING-NEXT:         fenv_clear = _v260;
// LOWERING-NEXT:         let _v261: i32 = 32;
// LOWERING-NEXT:         let _v262: i32 = 4;
// LOWERING-NEXT:         let _v263: i32 = _v261 | _v262;
// LOWERING-NEXT:         let _v264: i32 = 16;
// LOWERING-NEXT:         let _v265: i32 = _v263 | _v264;
// LOWERING-NEXT:         let _v266: i32 = 8;
// LOWERING-NEXT:         let _v267: i32 = _v265 | _v266;
// LOWERING-NEXT:         let _v268: i32 = 1;
// LOWERING-NEXT:         let _v269: i32 = _v267 | _v268;
// LOWERING-NEXT:         let _v270: i32 = unsafe { fetestexcept(_v269 as i32) };
// LOWERING-NEXT:         fenv_flags = _v270;
// LOWERING-NEXT:         let _v271: *mut i8 = b"%s %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:         let _v272: *mut i8 = b"main\0".as_ptr() as *mut i8;
// LOWERING-NEXT:         let _v273: i32 = α;
// LOWERING-NEXT:         let _v274: i32 = unsafe { c99_external_identifier_with_more_than_thirty_one_significant_characters };
// LOWERING-NEXT:         let _v275: i32 = unsafe { slash_comment_value };
// LOWERING-NEXT:         let _v276: i32 = enhanced_arithmetic;
// LOWERING-NEXT:         let _v277: i32 = flexible_total;
// LOWERING-NEXT:         let _v278: i32 = vm_total;
// LOWERING-NEXT:         let _v279: i32 = nonconstant_initializer.first;
// LOWERING-NEXT:         let _v280: i32 = nonconstant_initializer.second;
// LOWERING-NEXT:         let _v281: i32 = _v279 + _v280;
// LOWERING-NEXT:         let _v282: i32 = designated_initializer.first;
// LOWERING-NEXT:         let _v283: i32 = designated_initializer.second;
// LOWERING-NEXT:         let _v284: i32 = _v282 + _v283;
// LOWERING-NEXT:         let _v285: i64 = 0;
// LOWERING-NEXT:         let _v286: i32 = designated_array[(_v285 as usize)];
// LOWERING-NEXT:         let _v287: i64 = 2;
// LOWERING-NEXT:         let _v288: i32 = designated_array[(_v287 as usize)];
// LOWERING-NEXT:         let _v289: i32 = _v286 + _v288;
// LOWERING-NEXT:         let _v290: i32 = idempotent_const_value;
// LOWERING-NEXT:         let _v291: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(idempotent_volatile_value)) };
// LOWERING-NEXT:         let _v292: *mut i32 = restricted_pointer;
// LOWERING-NEXT:         let _v293: i32 = unsafe { *_v292 };
// LOWERING-NEXT:         let _v294: f64 = hexadecimal_float;
// LOWERING-NEXT:         let _v295: i32 = _v294 as i32;
// LOWERING-NEXT:         let _v296: i32 = compound_pair.first;
// LOWERING-NEXT:         let _v297: i32 = compound_pair.second;
// LOWERING-NEXT:         let _v298: i32 = _v296 + _v297;
// LOWERING-NEXT:         let _v299: i32 = compound_array_value;
// LOWERING-NEXT:         let _v300: i32 = signed_quotient;
// LOWERING-NEXT:         let _v301: i32 = signed_remainder;
// LOWERING-NEXT:         let _v302: i32 = mixed_order;
// LOWERING-NEXT:         let _v303: i32 = declaration_after_statement;
// LOWERING-NEXT:         let _v304: i32 = for_total;
// LOWERING-NEXT:         let _v305: i32 = 8;
// LOWERING-NEXT:         let _v306: i32 = c99_inline_square(_v305);
// LOWERING-NEXT:         let _v307: *mut i32 = qualified_values.as_mut_ptr() as *mut i32;
// LOWERING-NEXT:         let _v308: i32 = c99_qualified_array_sum(_v307);
// LOWERING-NEXT:         let _v309: i32 = macro_total;
// LOWERING-NEXT:         let _v310: i32 = translation_limit_total;
// LOWERING-NEXT:         let _v311: i32 = C99TrailingComma::C99_ENUM_VALUE as i32;
// LOWERING-NEXT:         let _v312: i32 = fenv_clear;
// LOWERING-NEXT:         let _v313: i32 = fenv_flags;
// LOWERING-NEXT:         let _v314: i64 = 0;
// LOWERING-NEXT:         let _v315: i64 = 1;
// LOWERING-NEXT:         let _v316: i32 = c99_restrict_sum(std::ptr::addr_of_mut!(qualified_values[(_v314 as usize)]), std::ptr::addr_of_mut!(qualified_values[(_v315 as usize)]));
// LOWERING-NEXT:         let _v317: i32 = unsafe { printf(_v271 as *const i8, _v272, _v273, _v274, _v275, _v276, _v277, _v278, _v281, _v284, _v289, _v290, _v291, _v293, _v295, _v298, _v299, _v300, _v301, _v302, _v303, _v304, _v306, _v308, _v309, _v310, _v311, _v312, _v313, _v316) };
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v318: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v318 as i32);
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
// REWRITES: #![feature(c_variadic)]
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
// REWRITES-NEXT: enum C99TrailingComma {
// REWRITES-NEXT:     C99_ENUM_VALUE = 17,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct C99Flexible {
// REWRITES-NEXT:     count: u64,
// REWRITES-NEXT:     values: [i32; 0],
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct C99Pair {
// REWRITES-NEXT:     first: i32,
// REWRITES-NEXT:     second: i32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
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
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn c99_vm_sum(arg39: i32, arg40: *mut i32) -> i32 {
// REWRITES-NEXT: let __arg40_view = unsafe { std::slice::from_raw_parts(arg40 as *const i32, arg39 as usize) };
// REWRITES-NEXT: let mut length: i32 = arg39;
// REWRITES-NEXT: let mut values: *mut i32 = arg40;
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut total: i32 = 0;
// REWRITES-NEXT: total = 0;
// REWRITES-NEXT: {
// REWRITES-NEXT:         let mut index: i32 = 0;
// REWRITES-NEXT:         index = 0;
// REWRITES-NEXT:         loop {
// REWRITES-NEXT:                     if !(index < length) {
// REWRITES-NEXT:                                     break;
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     {
// REWRITES-NEXT:                                     let _v7: *mut i32 = values;
// REWRITES-NEXT:                                     let _v8: *mut i32 = unsafe { _v7.offset((index as i64) as isize) };
// REWRITES-NEXT:                                     total = total + unsafe { __arg40_view[(index as usize)] };
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     index = index + 1;
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: __retval = total;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn sum3(arg36: i32, arg37: i32, arg38: i32) -> i32 {
// REWRITES-NEXT: let mut first: i32 = arg36;
// REWRITES-NEXT: let mut second: i32 = arg37;
// REWRITES-NEXT: let mut third: i32 = arg38;
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: __retval = first + second + third;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn c99_thirty_two_parameters(arg4: i32, arg5: i32, arg6: i32, arg7: i32, arg8: i32, arg9: i32, arg10: i32, arg11: i32, arg12: i32, arg13: i32, arg14: i32, arg15: i32, arg16: i32, arg17: i32, arg18: i32, arg19: i32, arg20: i32, arg21: i32, arg22: i32, arg23: i32, arg24: i32, arg25: i32, arg26: i32, arg27: i32, arg28: i32, arg29: i32, arg30: i32, arg31: i32, arg32: i32, arg33: i32, arg34: i32, arg35: i32) -> i32 {
// REWRITES-NEXT: let mut p01: i32 = arg4;
// REWRITES-NEXT: let mut p02: i32 = arg5;
// REWRITES-NEXT: let mut p03: i32 = arg6;
// REWRITES-NEXT: let mut p04: i32 = arg7;
// REWRITES-NEXT: let mut p05: i32 = arg8;
// REWRITES-NEXT: let mut p06: i32 = arg9;
// REWRITES-NEXT: let mut p07: i32 = arg10;
// REWRITES-NEXT: let mut p08: i32 = arg11;
// REWRITES-NEXT: let mut p09: i32 = arg12;
// REWRITES-NEXT: let mut p10: i32 = arg13;
// REWRITES-NEXT: let mut p11: i32 = arg14;
// REWRITES-NEXT: let mut p12: i32 = arg15;
// REWRITES-NEXT: let mut p13: i32 = arg16;
// REWRITES-NEXT: let mut p14: i32 = arg17;
// REWRITES-NEXT: let mut p15: i32 = arg18;
// REWRITES-NEXT: let mut p16: i32 = arg19;
// REWRITES-NEXT: let mut p17: i32 = arg20;
// REWRITES-NEXT: let mut p18: i32 = arg21;
// REWRITES-NEXT: let mut p19: i32 = arg22;
// REWRITES-NEXT: let mut p20: i32 = arg23;
// REWRITES-NEXT: let mut p21: i32 = arg24;
// REWRITES-NEXT: let mut p22: i32 = arg25;
// REWRITES-NEXT: let mut p23: i32 = arg26;
// REWRITES-NEXT: let mut p24: i32 = arg27;
// REWRITES-NEXT: let mut p25: i32 = arg28;
// REWRITES-NEXT: let mut p26: i32 = arg29;
// REWRITES-NEXT: let mut p27: i32 = arg30;
// REWRITES-NEXT: let mut p28: i32 = arg31;
// REWRITES-NEXT: let mut p29: i32 = arg32;
// REWRITES-NEXT: let mut p30: i32 = arg33;
// REWRITES-NEXT: let mut p31: i32 = arg34;
// REWRITES-NEXT: let mut p32: i32 = arg35;
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: __retval = p01 + p02 + p03 + p04 + p05 + p06 + p07 + p08 + p09 + p10 + p11 + p12 + p13 + p14 + p15 + p16 + p17 + p18 + p19 + p20 + p21 + p22 + p23 + p24 + p25 + p26 + p27 + p28 + p29 + p30 + p31 + p32;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn c99_inline_square(arg3: i32) -> i32 {
// REWRITES-NEXT: let mut value: i32 = arg3;
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: __retval = value * value;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn c99_qualified_array_sum(arg2: *mut i32) -> i32 {
// REWRITES-NEXT: let mut values: *mut i32 = arg2;
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let _v0: i64 = 0;
// REWRITES-NEXT: let _v1: *mut i32 = values;
// REWRITES-NEXT: let _v2: *mut i32 = unsafe { _v1.add(0) };
// REWRITES-NEXT: let _v3: i32 = unsafe { *_v2 };
// REWRITES-NEXT: let _v4: i64 = 1;
// REWRITES-NEXT: let _v5: *mut i32 = values;
// REWRITES-NEXT: let _v6: *mut i32 = unsafe { _v5.add(1) };
// REWRITES-NEXT: let _v8: i32 = _v3 + unsafe { *_v6 };
// REWRITES-NEXT: let _v9: i64 = 2;
// REWRITES-NEXT: let _v10: *mut i32 = values;
// REWRITES-NEXT: let _v11: *mut i32 = unsafe { _v10.add(2) };
// REWRITES-NEXT: __retval = _v8 + unsafe { *_v11 };
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn c99_restrict_sum(arg0: &i32, arg1: &i32) -> i32 {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: __retval = (unsafe { *((arg0 as *const i32) as *mut i32) }) + unsafe { *((arg1 as *const i32) as *mut i32) };
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut α: i32 = 0;
// REWRITES-NEXT: let mut boolean_value: bool = false;
// REWRITES-NEXT: let mut signed_long_long: i64 = 0;
// REWRITES-NEXT: let mut unsigned_long_long: u64 = 0;
// REWRITES-NEXT: let mut float_complex: num_complex::Complex<f32> = num_complex::Complex { re: 0.0, im: 0.0 };
// REWRITES-NEXT: let mut double_complex: num_complex::Complex<f64> = num_complex::Complex { re: 0.0, im: 0.0 };
// REWRITES-NEXT: let mut long_double_complex: num_complex::Complex<LongDouble> = num_complex::Complex { re: LongDouble([0; 10]), im: LongDouble([0; 10]) };
// REWRITES-NEXT: let mut enhanced_arithmetic: i32 = 0;
// REWRITES-NEXT: let mut flexible_total: i32 = 0;
// REWRITES-NEXT: let mut flexible: *mut C99Flexible = std::ptr::null_mut();
// REWRITES-NEXT: let mut length: i32 = 0;
// REWRITES-NEXT: let mut saved_stack: *mut u8 = std::ptr::null_mut();
// REWRITES-NEXT: let mut vm_total: i32 = 0;
// REWRITES-NEXT: let mut initializer_seed: i32 = 0;
// REWRITES-NEXT: let mut nonconstant_initializer: C99Pair = C99Pair { first: 0, second: 0 };
// REWRITES-NEXT: let mut designated_initializer: C99Pair = C99Pair { first: 0, second: 0 };
// REWRITES-NEXT: let mut designated_array: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// REWRITES-NEXT: let mut const_value: i32 = 0;
// REWRITES-NEXT: let mut idempotent_const_value: i32 = 0;
// REWRITES-NEXT: let mut volatile_value: i32 = 0;
// REWRITES-NEXT: let mut idempotent_volatile_value: i32 = 0;
// REWRITES-NEXT: let mut restricted_value: i32 = 0;
// REWRITES-NEXT: let mut restricted_pointer: *mut i32 = std::ptr::null_mut();
// REWRITES-NEXT: let mut hexadecimal_float: f64 = 0.0;
// REWRITES-NEXT: let mut compound_pair: C99Pair = C99Pair { first: 0, second: 0 };
// REWRITES-NEXT: let mut compound_array_value: i32 = 0;
// REWRITES-NEXT: let mut _compoundliteral: [i32; 2] = [0; 2];
// REWRITES-NEXT: let mut signed_quotient: i32 = 0;
// REWRITES-NEXT: let mut signed_remainder: i32 = 0;
// REWRITES-NEXT: let mut mixed_order: i32 = 0;
// REWRITES-NEXT: let mut declaration_after_statement: i32 = 0;
// REWRITES-NEXT: let mut for_total: i32 = 0;
// REWRITES-NEXT: let mut qualified_values: [i32; 3] = [0; 3];
// REWRITES-NEXT: let mut macro_total: i32 = 0;
// REWRITES-NEXT: let mut translation_limit_total: i32 = 0;
// REWRITES-NEXT: let mut fenv_clear: i32 = 0;
// REWRITES-NEXT: let mut fenv_flags: i32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: α = 7;
// REWRITES-NEXT: let _v2: i32 = 4;
// REWRITES-NEXT: boolean_value = _v2 != 0;
// REWRITES-NEXT: signed_long_long = -9000000000i64;
// REWRITES-NEXT: unsigned_long_long = 18000000000u64;
// REWRITES-NEXT: let _v6: f32 = 1.0;
// REWRITES-NEXT: let _v7: f32 = 2.0;
// REWRITES-NEXT: let _v8: f32 = 0.0;
// REWRITES-NEXT: let _v9: f32 = 0.0;
// REWRITES-NEXT: let _v10: f32 = 1.0;
// REWRITES-NEXT: float_complex = num_complex::Complex { re: _v6 + _v7 * (_v8 + _v9), im: _v7 * _v10 };
// REWRITES-NEXT: let _v16: f64 = 3.0;
// REWRITES-NEXT: let _v17: f64 = 4.0;
// REWRITES-NEXT: let _v18: f32 = 0.0;
// REWRITES-NEXT: let _v19: f32 = 0.0;
// REWRITES-NEXT: let _v20: f32 = 1.0;
// REWRITES-NEXT: let _v22: num_complex::Complex<f32> = num_complex::Complex { re: _v18 + _v19, im: _v20 };
// REWRITES-NEXT: let _v27: num_complex::Complex<f64> = num_complex::Complex { re: _v22.re as f64, im: _v22.im as f64 };
// REWRITES-NEXT: double_complex = num_complex::Complex { re: _v16 + _v17 * _v27.re, im: _v17 * _v27.im };
// REWRITES-NEXT: let _v34: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 160, 1, 64]);
// REWRITES-NEXT: let _v35: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 1, 64]);
// REWRITES-NEXT: let _v36: f32 = 0.0;
// REWRITES-NEXT: let _v37: f32 = 0.0;
// REWRITES-NEXT: let _v38: f32 = 1.0;
// REWRITES-NEXT: let _v40: num_complex::Complex<f32> = num_complex::Complex { re: _v36 + _v37, im: _v38 };
// REWRITES-NEXT: let _v41: f32 = _v40.re;
// REWRITES-NEXT: let _v42: f32 = _v40.im;
// REWRITES-NEXT: let _v43: LongDouble = __slate_f80_from_f32(_v41);
// REWRITES-NEXT: let _v44: LongDouble = __slate_f80_from_f32(_v42);
// REWRITES-NEXT: let _v45: num_complex::Complex<LongDouble> = num_complex::Complex { re: _v43, im: _v44 };
// REWRITES-NEXT: long_double_complex = num_complex::Complex { re: _v34 + _v35 * _v45.re, im: _v35 * _v45.im };
// REWRITES-NEXT: let _v65: f32 = 1.0;
// REWRITES-NEXT: let _v66: f32 = 2.0;
// REWRITES-NEXT: let _v67: f32 = 0.0;
// REWRITES-NEXT: let _v68: f32 = 0.0;
// REWRITES-NEXT: let _v69: f32 = 1.0;
// REWRITES-NEXT: let _v79: f64 = 3.0;
// REWRITES-NEXT: let _v80: f64 = 4.0;
// REWRITES-NEXT: let _v81: f32 = 0.0;
// REWRITES-NEXT: let _v82: f32 = 0.0;
// REWRITES-NEXT: let _v83: f32 = 1.0;
// REWRITES-NEXT: let _v85: num_complex::Complex<f32> = num_complex::Complex { re: _v81 + _v82, im: _v83 };
// REWRITES-NEXT: let _v90: num_complex::Complex<f64> = num_complex::Complex { re: _v85.re as f64, im: _v85.im as f64 };
// REWRITES-NEXT: let _v99: i32 = (boolean_value as i32) + ((signed_long_long == -9000000000i64) as i32) + ((unsigned_long_long == 18000000000u64) as i32) + ((float_complex == num_complex::Complex { re: _v65 + _v66 * (_v67 + _v68), im: _v66 * _v69 }) as i32) + ((double_complex == num_complex::Complex { re: _v79 + _v80 * _v90.re, im: _v80 * _v90.im }) as i32);
// REWRITES-NEXT: let _v100: num_complex::Complex<LongDouble> = long_double_complex;
// REWRITES-NEXT: let _v101: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 160, 1, 64]);
// REWRITES-NEXT: let _v102: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 1, 64]);
// REWRITES-NEXT: let _v103: f32 = 0.0;
// REWRITES-NEXT: let _v104: f32 = 0.0;
// REWRITES-NEXT: let _v105: f32 = 1.0;
// REWRITES-NEXT: let _v107: num_complex::Complex<f32> = num_complex::Complex { re: _v103 + _v104, im: _v105 };
// REWRITES-NEXT: let _v108: f32 = _v107.re;
// REWRITES-NEXT: let _v109: f32 = _v107.im;
// REWRITES-NEXT: let _v110: LongDouble = __slate_f80_from_f32(_v108);
// REWRITES-NEXT: let _v111: LongDouble = __slate_f80_from_f32(_v109);
// REWRITES-NEXT: let _v112: num_complex::Complex<LongDouble> = num_complex::Complex { re: _v110, im: _v111 };
// REWRITES-NEXT: enhanced_arithmetic = _v99 + ((_v100 == num_complex::Complex { re: _v101 + _v102 * _v112.re, im: _v102 * _v112.im }) as i32);
// REWRITES-NEXT: flexible_total = 0;
// REWRITES-NEXT: let _v123: u64 = 8;
// REWRITES-NEXT: let _v124: u64 = 3;
// REWRITES-NEXT: let _v125: u64 = 4;
// REWRITES-NEXT: let _v128: *mut core::ffi::c_void = unsafe { malloc((_v123 + _v124 * _v125) as usize) };
// REWRITES-NEXT: flexible = _v128 as *mut C99Flexible;
// REWRITES-NEXT: {
// REWRITES-NEXT:         let _v131: *mut C99Flexible = std::ptr::null_mut();
// REWRITES-NEXT:         let _v132: bool = flexible == _v131;
// REWRITES-NEXT:         if _v132 {
// REWRITES-NEXT:                     __retval = 2;
// REWRITES-NEXT:                     std::process::exit(__retval as i32);
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         (*flexible).count = 3;
// REWRITES-NEXT: }
// REWRITES-NEXT: {
// REWRITES-NEXT:         let mut index: u64 = 0;
// REWRITES-NEXT:         index = 0;
// REWRITES-NEXT:         loop {
// REWRITES-NEXT:                     if !(index < unsafe { (*flexible).count }) {
// REWRITES-NEXT:                                     break;
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     {
// REWRITES-NEXT:                                     let _v144: i32 = 1;
// REWRITES-NEXT:                                     let _v145: i32 = (index as i32) + _v144;
// REWRITES-NEXT:                                     let _v146: u64 = index;
// REWRITES-NEXT:                                     let _v147: *mut C99Flexible = flexible;
// REWRITES-NEXT:                                     unsafe {
// REWRITES-NEXT:                                                         *(*_v147).values.as_mut_ptr().add(_v146 as usize) = _v145;
// REWRITES-NEXT:                                     }
// REWRITES-NEXT:                                     let _v148: u64 = index;
// REWRITES-NEXT:                                     let _v149: *mut C99Flexible = flexible;
// REWRITES-NEXT:                                     let _v150: i32 = unsafe { *(*_v149).values.as_mut_ptr().add(_v148 as usize) };
// REWRITES-NEXT:                                     flexible_total = flexible_total + _v150;
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     index = index + 1;
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: unsafe { free((flexible as *mut core::ffi::c_void) as *mut core::ffi::c_void) };
// REWRITES-NEXT: length = 3;
// REWRITES-NEXT: let _v159: u64 = length as u64;
// REWRITES-NEXT: saved_stack = 0usize as *mut u8;
// REWRITES-NEXT: {
// REWRITES-NEXT:         let mut variable_length_array: Vec<i32> = vec![0; _v159 as usize];
// REWRITES-NEXT:         {
// REWRITES-NEXT:                     let mut index2: i32 = 0;
// REWRITES-NEXT:                     index2 = 0;
// REWRITES-NEXT:                     loop {
// REWRITES-NEXT:                                     if !(index2 < length) {
// REWRITES-NEXT:                                                         break;
// REWRITES-NEXT:                                     }
// REWRITES-NEXT:                                     {
// REWRITES-NEXT:                                                         let _v166: i32 = 4;
// REWRITES-NEXT:                                                         let _v167: i32 = index2 + _v166;
// REWRITES-NEXT:                                                         let _v170: *mut i32 = unsafe { variable_length_array.as_mut_ptr().offset((index2 as i64) as isize) };
// REWRITES-NEXT:                                                         unsafe {
// REWRITES-NEXT:                                                                                 *_v170 = _v167;
// REWRITES-NEXT:                                                         }
// REWRITES-NEXT:                                     }
// REWRITES-NEXT:                                     index2 = index2 + 1;
// REWRITES-NEXT:                     }
// REWRITES-NEXT:         }
// REWRITES-NEXT:         vm_total = c99_vm_sum(length, variable_length_array.as_mut_ptr());
// REWRITES-NEXT:         initializer_seed = 19;
// REWRITES-NEXT:         nonconstant_initializer.first = initializer_seed;
// REWRITES-NEXT:         let _v178: i32 = 1;
// REWRITES-NEXT:         nonconstant_initializer.second = initializer_seed + _v178;
// REWRITES-NEXT:         designated_initializer = C99Pair { first: 22, second: 23 };
// REWRITES-NEXT:         *designated_array = [27, 0, 29, 0];
// REWRITES-NEXT:         const_value = 31;
// REWRITES-NEXT:         idempotent_const_value = const_value;
// REWRITES-NEXT:         let _v182: i32 = 37;
// REWRITES-NEXT:         unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(volatile_value), _v182) };
// REWRITES-NEXT:         let _v183: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(volatile_value)) };
// REWRITES-NEXT:         unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(idempotent_volatile_value), _v183) };
// REWRITES-NEXT:         restricted_value = 41;
// REWRITES-NEXT:         restricted_pointer = std::ptr::addr_of_mut!(restricted_value);
// REWRITES-NEXT:         hexadecimal_float = 3.0;
// REWRITES-NEXT:         compound_pair = C99Pair { first: 43, second: 47 };
// REWRITES-NEXT:         let _v186: i64 = 1;
// REWRITES-NEXT:         let _v187: *mut i32 = _compoundliteral.as_mut_ptr() as *mut i32;
// REWRITES-NEXT:         unsafe {
// REWRITES-NEXT:                     *_v187 = 51;
// REWRITES-NEXT:         }
// REWRITES-NEXT:         let _v189: i64 = 1;
// REWRITES-NEXT:         let _v190: *mut i32 = unsafe { _v187.add(1) };
// REWRITES-NEXT:         unsafe {
// REWRITES-NEXT:                     *_v190 = 53;
// REWRITES-NEXT:         }
// REWRITES-NEXT:         compound_array_value = _compoundliteral[(_v186 as usize)];
// REWRITES-NEXT:         let _v193: i32 = -7;
// REWRITES-NEXT:         let _v194: i32 = 3;
// REWRITES-NEXT:         signed_quotient = _v193 / _v194;
// REWRITES-NEXT:         let _v196: i32 = -7;
// REWRITES-NEXT:         let _v197: i32 = 3;
// REWRITES-NEXT:         signed_remainder = _v196 % _v197;
// REWRITES-NEXT:         mixed_order = 59;
// REWRITES-NEXT:         let _v200: i32 = 2;
// REWRITES-NEXT:         mixed_order = mixed_order + _v200;
// REWRITES-NEXT:         declaration_after_statement = 61;
// REWRITES-NEXT:         for_total = 0;
// REWRITES-NEXT:         {
// REWRITES-NEXT:                     let mut index3: i32 = 0;
// REWRITES-NEXT:                     index3 = 0;
// REWRITES-NEXT:                     loop {
// REWRITES-NEXT:                                     let _v207: i32 = 3;
// REWRITES-NEXT:                                     if !(index3 < _v207) {
// REWRITES-NEXT:                                                         break;
// REWRITES-NEXT:                                     }
// REWRITES-NEXT:                                     {
// REWRITES-NEXT:                                                         for_total = for_total + index3;
// REWRITES-NEXT:                                     }
// REWRITES-NEXT:                                     index3 = index3 + 1;
// REWRITES-NEXT:                     }
// REWRITES-NEXT:         }
// REWRITES-NEXT:         qualified_values = [2, 3, 5];
// REWRITES-NEXT:         let _v214: i32 = 7;
// REWRITES-NEXT:         let _v215: i32 = 11;
// REWRITES-NEXT:         let _v216: i32 = 13;
// REWRITES-NEXT:         macro_total = sum3(_v214, _v215, _v216);
// REWRITES-NEXT:         let _v218: i32 = 1;
// REWRITES-NEXT:         let _v219: i32 = 1;
// REWRITES-NEXT:         let _v220: i32 = 1;
// REWRITES-NEXT:         let _v221: i32 = 1;
// REWRITES-NEXT:         let _v222: i32 = 1;
// REWRITES-NEXT:         let _v223: i32 = 1;
// REWRITES-NEXT:         let _v224: i32 = 1;
// REWRITES-NEXT:         let _v225: i32 = 1;
// REWRITES-NEXT:         let _v226: i32 = 1;
// REWRITES-NEXT:         let _v227: i32 = 1;
// REWRITES-NEXT:         let _v228: i32 = 1;
// REWRITES-NEXT:         let _v229: i32 = 1;
// REWRITES-NEXT:         let _v230: i32 = 1;
// REWRITES-NEXT:         let _v231: i32 = 1;
// REWRITES-NEXT:         let _v232: i32 = 1;
// REWRITES-NEXT:         let _v233: i32 = 1;
// REWRITES-NEXT:         let _v234: i32 = 1;
// REWRITES-NEXT:         let _v235: i32 = 1;
// REWRITES-NEXT:         let _v236: i32 = 1;
// REWRITES-NEXT:         let _v237: i32 = 1;
// REWRITES-NEXT:         let _v238: i32 = 1;
// REWRITES-NEXT:         let _v239: i32 = 1;
// REWRITES-NEXT:         let _v240: i32 = 1;
// REWRITES-NEXT:         let _v241: i32 = 1;
// REWRITES-NEXT:         let _v242: i32 = 1;
// REWRITES-NEXT:         let _v243: i32 = 1;
// REWRITES-NEXT:         let _v244: i32 = 1;
// REWRITES-NEXT:         let _v245: i32 = 1;
// REWRITES-NEXT:         let _v246: i32 = 1;
// REWRITES-NEXT:         let _v247: i32 = 1;
// REWRITES-NEXT:         let _v248: i32 = 1;
// REWRITES-NEXT:         let _v249: i32 = 1;
// REWRITES-NEXT:         translation_limit_total = c99_thirty_two_parameters(_v218, _v219, _v220, _v221, _v222, _v223, _v224, _v225, _v226, _v227, _v228, _v229, _v230, _v231, _v232, _v233, _v234, _v235, _v236, _v237, _v238, _v239, _v240, _v241, _v242, _v243, _v244, _v245, _v246, _v247, _v248, _v249);
// REWRITES-NEXT:         let _v251: i32 = 32;
// REWRITES-NEXT:         let _v252: i32 = 4;
// REWRITES-NEXT:         let _v254: i32 = 16;
// REWRITES-NEXT:         let _v256: i32 = 8;
// REWRITES-NEXT:         let _v258: i32 = 1;
// REWRITES-NEXT:         fenv_clear = unsafe { feclearexcept((_v251 | _v252 | _v254 | _v256 | _v258) as i32) };
// REWRITES-NEXT:         let _v261: i32 = 32;
// REWRITES-NEXT:         let _v262: i32 = 4;
// REWRITES-NEXT:         let _v264: i32 = 16;
// REWRITES-NEXT:         let _v266: i32 = 8;
// REWRITES-NEXT:         let _v268: i32 = 1;
// REWRITES-NEXT:         fenv_flags = unsafe { fetestexcept((_v261 | _v262 | _v264 | _v266 | _v268) as i32) };
// REWRITES-NEXT:         let _v271: *mut i8 = b"%s %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT:         let _v272: *mut i8 = b"main\0".as_ptr() as *mut i8;
// REWRITES-NEXT:         let _v273: i32 = α;
// REWRITES-NEXT:         let _v274: i32 = unsafe { c99_external_identifier_with_more_than_thirty_one_significant_characters };
// REWRITES-NEXT:         let _v275: i32 = unsafe { slash_comment_value };
// REWRITES-NEXT:         let _v276: i32 = enhanced_arithmetic;
// REWRITES-NEXT:         let _v277: i32 = flexible_total;
// REWRITES-NEXT:         let _v278: i32 = vm_total;
// REWRITES-NEXT:         let _v281: i32 = nonconstant_initializer.first + nonconstant_initializer.second;
// REWRITES-NEXT:         let _v284: i32 = designated_initializer.first + designated_initializer.second;
// REWRITES-NEXT:         let _v285: i64 = 0;
// REWRITES-NEXT:         let _v287: i64 = 2;
// REWRITES-NEXT:         let _v289: i32 = designated_array[(_v285 as usize)] + designated_array[(_v287 as usize)];
// REWRITES-NEXT:         let _v290: i32 = idempotent_const_value;
// REWRITES-NEXT:         let _v291: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(idempotent_volatile_value)) };
// REWRITES-NEXT:         let _v293: i32 = unsafe { *restricted_pointer };
// REWRITES-NEXT:         let _v295: i32 = hexadecimal_float as i32;
// REWRITES-NEXT:         let _v298: i32 = compound_pair.first + compound_pair.second;
// REWRITES-NEXT:         let _v299: i32 = compound_array_value;
// REWRITES-NEXT:         let _v300: i32 = signed_quotient;
// REWRITES-NEXT:         let _v301: i32 = signed_remainder;
// REWRITES-NEXT:         let _v302: i32 = mixed_order;
// REWRITES-NEXT:         let _v303: i32 = declaration_after_statement;
// REWRITES-NEXT:         let _v304: i32 = for_total;
// REWRITES-NEXT:         let _v305: i32 = 8;
// REWRITES-NEXT:         let _v306: i32 = c99_inline_square(_v305);
// REWRITES-NEXT:         let _v307: *mut i32 = qualified_values.as_mut_ptr() as *mut i32;
// REWRITES-NEXT:         let _v308: i32 = c99_qualified_array_sum(_v307);
// REWRITES-NEXT:         let _v309: i32 = macro_total;
// REWRITES-NEXT:         let _v310: i32 = translation_limit_total;
// REWRITES-NEXT:         let _v311: i32 = C99TrailingComma::C99_ENUM_VALUE as i32;
// REWRITES-NEXT:         let _v312: i32 = fenv_clear;
// REWRITES-NEXT:         let _v313: i32 = fenv_flags;
// REWRITES-NEXT:         let _v314: i64 = 0;
// REWRITES-NEXT:         let _v315: i64 = 1;
// REWRITES-NEXT:         let _v316: i32 = c99_restrict_sum(unsafe { &(*std::ptr::addr_of_mut!(qualified_values[(_v314 as usize)])) }, unsafe { &(*std::ptr::addr_of_mut!(qualified_values[(_v315 as usize)])) });
// REWRITES-NEXT:         let _v317: i32 = unsafe { printf(_v271 as *const i8, _v272, _v273, _v274, _v275, _v276, _v277, _v278, _v281, _v284, _v289, _v290, _v291, _v293, _v295, _v298, _v299, _v300, _v301, _v302, _v303, _v304, _v306, _v308, _v309, _v310, _v311, _v312, _v313, _v316) };
// REWRITES-NEXT: }
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
