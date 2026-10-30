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

// SLATE-FILECHECK-BEGIN aarch64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![feature(f128)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: mod __slate_libgcc {
// CHECK-NEXT:     #[link(name = "gcc", kind = "static")]
// CHECK-NEXT:     unsafe extern "C" {}
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn feclearexcept([[ARG_0:arg[0-9]+]]: i32) -> i32;
// CHECK-NEXT:     fn fetestexcept([[ARG_0]]: i32) -> i32;
// CHECK-NEXT:     fn printf([[ARG_0]]: *const u8, ...) -> i32;
// CHECK-NEXT:     fn malloc([[ARG_0]]: u64) -> *mut ();
// CHECK-NEXT:     fn free([[ARG_0]]: *mut ());
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct C99Pair {
// CHECK-NEXT:     first: i32,
// CHECK-NEXT:     second: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct C99Flexible {
// CHECK-NEXT:     count: u64,
// CHECK-NEXT:     values: [i32; 0],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut c99_external_identifier_with_more_than_thirty_one_significant_characters: i32 = 5;
// CHECK-EMPTY:
// CHECK-NEXT: ///
// CHECK-NEXT: static mut slash_comment_value: i32 = 3;
// CHECK-EMPTY:
// CHECK-NEXT: fn c99_inline_square(value: i32) -> i32 {
// CHECK-NEXT:     value * value
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn sum3(first: i32, second: i32, third: i32) -> i32 {
// CHECK-NEXT:     first + second + third
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn c99_restrict_sum(left: *const i32, right: *const i32) -> i32 {
// CHECK-NEXT:     (unsafe { *left }) + unsafe { *right }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn c99_qualified_array_sum(values: *mut i32) -> i32 {
// CHECK-NEXT:     (unsafe { *values.add(0) }) + unsafe { *values.add(1) } + unsafe { *values.add(2) }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn c99_vm_sum(length: i32, values: *mut [i32; 0]) -> i32 {
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     for index in 0..length {
// CHECK-NEXT:         total += unsafe { *(values as *mut i32).offset(index as isize) };
// CHECK-NEXT:     }
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn c99_thirty_two_parameters(
// CHECK-NEXT:     p01: i32,
// CHECK-NEXT:     p02: i32,
// CHECK-NEXT:     p03: i32,
// CHECK-NEXT:     p04: i32,
// CHECK-NEXT:     p05: i32,
// CHECK-NEXT:     p06: i32,
// CHECK-NEXT:     p07: i32,
// CHECK-NEXT:     p08: i32,
// CHECK-NEXT:     p09: i32,
// CHECK-NEXT:     p10: i32,
// CHECK-NEXT:     p11: i32,
// CHECK-NEXT:     p12: i32,
// CHECK-NEXT:     p13: i32,
// CHECK-NEXT:     p14: i32,
// CHECK-NEXT:     p15: i32,
// CHECK-NEXT:     p16: i32,
// CHECK-NEXT:     p17: i32,
// CHECK-NEXT:     p18: i32,
// CHECK-NEXT:     p19: i32,
// CHECK-NEXT:     p20: i32,
// CHECK-NEXT:     p21: i32,
// CHECK-NEXT:     p22: i32,
// CHECK-NEXT:     p23: i32,
// CHECK-NEXT:     p24: i32,
// CHECK-NEXT:     p25: i32,
// CHECK-NEXT:     p26: i32,
// CHECK-NEXT:     p27: i32,
// CHECK-NEXT:     p28: i32,
// CHECK-NEXT:     p29: i32,
// CHECK-NEXT:     p30: i32,
// CHECK-NEXT:     p31: i32,
// CHECK-NEXT:     p32: i32,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     p01 + p02
// CHECK-NEXT:         + p03
// CHECK-NEXT:         + p04
// CHECK-NEXT:         + p05
// CHECK-NEXT:         + p06
// CHECK-NEXT:         + p07
// CHECK-NEXT:         + p08
// CHECK-NEXT:         + p09
// CHECK-NEXT:         + p10
// CHECK-NEXT:         + p11
// CHECK-NEXT:         + p12
// CHECK-NEXT:         + p13
// CHECK-NEXT:         + p14
// CHECK-NEXT:         + p15
// CHECK-NEXT:         + p16
// CHECK-NEXT:         + p17
// CHECK-NEXT:         + p18
// CHECK-NEXT:         + p19
// CHECK-NEXT:         + p20
// CHECK-NEXT:         + p21
// CHECK-NEXT:         + p22
// CHECK-NEXT:         + p23
// CHECK-NEXT:         + p24
// CHECK-NEXT:         + p25
// CHECK-NEXT:         + p26
// CHECK-NEXT:         + p27
// CHECK-NEXT:         + p28
// CHECK-NEXT:         + p29
// CHECK-NEXT:         + p30
// CHECK-NEXT:         + p31
// CHECK-NEXT:         + p32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0:__slate_compound_[0-9]+]]: C99Pair = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_1:__slate_compound_[0-9]+]]: [i32; 2] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let α: i32 = 7;
// CHECK-NEXT:     let boolean_value: bool = true;
// CHECK-NEXT:     let signed_long_long: i64 = -9000000000;
// CHECK-NEXT:     let unsigned_long_long: u64 = 18000000000;
// CHECK-NEXT:     let float_complex: num_complex::Complex<f32> = 1.0f32
// CHECK-NEXT:         + 2.0f32
// CHECK-NEXT:             * num_complex::Complex {
// CHECK-NEXT:                 re: 0.0f32,
// CHECK-NEXT:                 im: 1.0f32,
// CHECK-NEXT:             };
// CHECK-NEXT:     let double_complex: num_complex::Complex<f64> = 3.0f64
// CHECK-NEXT:         + 4.0f64 * {
// CHECK-NEXT:             let [[T_0:__t[0-9]+]] = num_complex::Complex {
// CHECK-NEXT:                 re: 0.0f32,
// CHECK-NEXT:                 im: 1.0f32,
// CHECK-NEXT:             };
// CHECK-NEXT:             num_complex::Complex {
// CHECK-NEXT:                 re: [[T_0]].re as f64,
// CHECK-NEXT:                 im: [[T_0]].im as f64,
// CHECK-NEXT:             }
// CHECK-NEXT:         };
// CHECK-NEXT:     let long_double_complex: num_complex::Complex<f128> = 5f128
// CHECK-NEXT:         + 6f128 * {
// CHECK-NEXT:             let [[T_1:__t[0-9]+]] = num_complex::Complex {
// CHECK-NEXT:                 re: 0.0f32,
// CHECK-NEXT:                 im: 1.0f32,
// CHECK-NEXT:             };
// CHECK-NEXT:             num_complex::Complex {
// CHECK-NEXT:                 re: [[T_1]].re as f128,
// CHECK-NEXT:                 im: [[T_1]].im as f128,
// CHECK-NEXT:             }
// CHECK-NEXT:         };
// CHECK-NEXT:     let enhanced_arithmetic: i32 = (boolean_value as i32)
// CHECK-NEXT:         + ((signed_long_long == -9000000000) as i32)
// CHECK-NEXT:         + ((unsigned_long_long == 18000000000) as i32)
// CHECK-NEXT:         + ((float_complex
// CHECK-NEXT:             == 1.0f32
// CHECK-NEXT:                 + 2.0f32
// CHECK-NEXT:                     * num_complex::Complex {
// CHECK-NEXT:                         re: 0.0f32,
// CHECK-NEXT:                         im: 1.0f32,
// CHECK-NEXT:                     }) as i32)
// CHECK-NEXT:         + ((double_complex
// CHECK-NEXT:             == 3.0f64
// CHECK-NEXT:                 + 4.0f64 * {
// CHECK-NEXT:                     let [[T_2:__t[0-9]+]] = num_complex::Complex {
// CHECK-NEXT:                         re: 0.0f32,
// CHECK-NEXT:                         im: 1.0f32,
// CHECK-NEXT:                     };
// CHECK-NEXT:                     num_complex::Complex {
// CHECK-NEXT:                         re: [[T_2]].re as f64,
// CHECK-NEXT:                         im: [[T_2]].im as f64,
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }) as i32)
// CHECK-NEXT:         + ((long_double_complex
// CHECK-NEXT:             == 5f128
// CHECK-NEXT:                 + 6f128 * {
// CHECK-NEXT:                     let [[T_3:__t[0-9]+]] = num_complex::Complex {
// CHECK-NEXT:                         re: 0.0f32,
// CHECK-NEXT:                         im: 1.0f32,
// CHECK-NEXT:                     };
// CHECK-NEXT:                     num_complex::Complex {
// CHECK-NEXT:                         re: [[T_3]].re as f128,
// CHECK-NEXT:                         im: [[T_3]].im as f128,
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }) as i32);
// CHECK-NEXT:     let mut flexible_total: i32 = 0;
// CHECK-NEXT:     let flexible: *mut C99Flexible = (unsafe { malloc(20u64) }) as *mut C99Flexible;
// CHECK-NEXT:     if flexible.is_null() {
// CHECK-NEXT:         return 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*flexible).count = 3;
// CHECK-NEXT:     }
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut index: u64 = 0;
// CHECK-NEXT:         while index < unsafe { (*flexible).count } {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *(std::ptr::addr_of_mut!((*flexible).values) as *mut i32).add(index as usize) =
// CHECK-NEXT:                     ((index as u32) as i32) + 1;
// CHECK-NEXT:             }
// CHECK-NEXT:             let [[V_0:__v[0-9]+]]: i32 = flexible_total
// CHECK-NEXT:                 + unsafe {
// CHECK-NEXT:                     *(std::ptr::addr_of_mut!((*flexible).values) as *mut i32).add(index as usize)
// CHECK-NEXT:                 };
// CHECK-NEXT:             flexible_total = [[V_0]];
// CHECK-NEXT:             index = index.wrapping_add(1);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { free(flexible as *mut ()) };
// CHECK-NEXT:     let length: i32 = 3;
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: u64 = (length as i64) as u64;
// CHECK-NEXT:     let mut variable_length_array: Vec<i32> = {
// CHECK-NEXT:         let mut __slate_vla: Vec<i32> = Vec::with_capacity([[V_1]] as usize);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             __slate_vla.as_mut_ptr().write_bytes(0u8, [[V_1]] as usize);
// CHECK-NEXT:         }
// CHECK-NEXT:         __slate_vla
// CHECK-NEXT:     };
// CHECK-NEXT:     for index in 0..length {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *(variable_length_array.as_mut_ptr() as *mut i32).offset(index as isize) = index + 4;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     let vm_total: i32 = c99_vm_sum(length, variable_length_array.as_mut_ptr() as *mut [i32; 0]);
// CHECK-NEXT:     let initializer_seed: i32 = 19;
// CHECK-NEXT:     let nonconstant_initializer: C99Pair = C99Pair {
// CHECK-NEXT:         first: initializer_seed,
// CHECK-NEXT:         second: initializer_seed + 1,
// CHECK-NEXT:     };
// CHECK-NEXT:     let designated_initializer: C99Pair = C99Pair {
// CHECK-NEXT:         first: 22,
// CHECK-NEXT:         second: 23,
// CHECK-NEXT:     };
// CHECK-NEXT:     let designated_array: [i32; 4] = {
// CHECK-NEXT:         let mut [[T_4:__t[0-9]+]]: [i32; 4] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_4]][0] = 27i32;
// CHECK-NEXT:         [[T_4]][2] = 29i32;
// CHECK-NEXT:         [[T_4]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let const_value: i32 = 31;
// CHECK-NEXT:     let idempotent_const_value: i32 = const_value;
// CHECK-NEXT:     let volatile_value: i32 = 37;
// CHECK-NEXT:     let idempotent_volatile_value: i32 =
// CHECK-NEXT:         unsafe { std::ptr::read_volatile(std::ptr::addr_of!(volatile_value)) };
// CHECK-NEXT:     let mut restricted_value: i32 = 41;
// CHECK-NEXT:     let restricted_pointer: *mut i32 = std::ptr::addr_of_mut!(restricted_value);
// CHECK-NEXT:     let hexadecimal_float: f64 = 3.0f64;
// CHECK-NEXT:     let compound_pair: C99Pair = unsafe {
// CHECK-NEXT:         *{
// CHECK-NEXT:             [[SLATE_COMPOUND_0]] = C99Pair {
// CHECK-NEXT:                 first: 43,
// CHECK-NEXT:                 second: 47,
// CHECK-NEXT:             };
// CHECK-NEXT:             std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     let compound_array_value: i32 = unsafe {
// CHECK-NEXT:         *((*{
// CHECK-NEXT:             [[SLATE_COMPOUND_1]] = [51i32, 53i32];
// CHECK-NEXT:             std::ptr::addr_of_mut!([[SLATE_COMPOUND_1]])
// CHECK-NEXT:         })
// CHECK-NEXT:         .as_mut_ptr() as *mut i32)
// CHECK-NEXT:             .add(1)
// CHECK-NEXT:     };
// CHECK-NEXT:     let signed_quotient: i32 = -7 / 3;
// CHECK-NEXT:     let signed_remainder: i32 = -7 % 3;
// CHECK-NEXT:     let mut mixed_order: i32 = 59;
// CHECK-NEXT:     mixed_order += 2;
// CHECK-NEXT:     let declaration_after_statement: i32 = 61;
// CHECK-NEXT:     let mut for_total: i32 = 0;
// CHECK-NEXT:     for index in 0i32..3i32 {
// CHECK-NEXT:         for_total += index;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut qualified_values: [i32; 3] = [2, 3, 5];
// CHECK-NEXT:     let macro_total: i32 = sum3(7i32, 11i32, 13i32);
// CHECK-NEXT:     let translation_limit_total: i32 = c99_thirty_two_parameters(
// CHECK-NEXT:         1i32, 1i32, 1i32, 1i32, 1i32, 1i32, 1i32, 1i32, 1i32, 1i32, 1i32, 1i32, 1i32, 1i32, 1i32,
// CHECK-NEXT:         1i32, 1i32, 1i32, 1i32, 1i32, 1i32, 1i32, 1i32, 1i32, 1i32, 1i32, 1i32, 1i32, 1i32, 1i32,
// CHECK-NEXT:         1i32, 1i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     let fenv_clear: i32 = unsafe { feclearexcept(31i32) };
// CHECK-NEXT:     let fenv_flags: i32 = unsafe { fetestexcept(31i32) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(c"%s %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n".as_ptr(), c"main".as_ptr() as *mut u8, α, c99_external_identifier_with_more_than_thirty_one_significant_characters, slash_comment_value, enhanced_arithmetic, flexible_total, vm_total, nonconstant_initializer.first + nonconstant_initializer.second, designated_initializer.first + designated_initializer.second, designated_array[0] + designated_array[2], idempotent_const_value, std::ptr::read_volatile(std::ptr::addr_of!(idempotent_volatile_value)), *restricted_pointer, hexadecimal_float as i32, compound_pair.first + compound_pair.second, compound_array_value, signed_quotient, signed_remainder, mixed_order, declaration_after_statement, for_total, c99_inline_square(8i32), c99_qualified_array_sum(qualified_values.as_mut_ptr() as *mut i32), macro_total, translation_limit_total, 17i32, fenv_clear, fenv_flags, c99_restrict_sum((qualified_values.as_mut_ptr() as *mut i32).add(0) as *const i32, (qualified_values.as_mut_ptr() as *mut i32).add(1) as *const i32))
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
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
