#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>

#define C89_JOIN_RAW(left, right) left##right
#define C89_JOIN(left, right)     C89_JOIN_RAW(left, right)
#define C89_STRINGIFY_RAW(value)  #value
#define C89_STRINGIFY(value)      C89_STRINGIFY_RAW(value)
#define C89_MACRO_NUMBER          13

#if C89_MACRO_NUMBER == 12
#define C89_CONDITIONAL_VALUE 0
#elif defined(C89_MACRO_NUMBER)
#define C89_CONDITIONAL_VALUE C89_MACRO_NUMBER
#else
#define C89_CONDITIONAL_VALUE 0
#endif

enum C89Color { C89_RED = 3, C89_GREEN = 5, C89_BLUE = 7 };

struct C89Point {
  int x;
  int y;
};

struct C89Bits {
  unsigned low  : 3;
  signed   high : 4;
};

union C89Number {
  int    integer;
  double real;
};

typedef signed int C89SignedInt;
typedef int        (*C89BinaryOperation)(int, int);

int                 c89_external_value           = 11;
static const int    c89_const_global             = 17;
static volatile int c89_volatile_global          = 19;
static int          C89_JOIN(c89_joined_, value) = 23;

static int  c89_add(int left, int right);
static void c89_store(void *destination, const void *source);

static int c89_add(int left, int right) { return left + right; }

static void c89_store(void *destination, const void *source) {
  int       *output;
  const int *input;
  output  = (int *)destination;
  input   = (const int *)source;
  *output = *input;
}

static int c89_variadic_sum(int count, ...) {
  va_list arguments;
  int     index;
  int     total;
  total = 0;
  va_start(arguments, count);
  for (index = 0; index < count; ++index) {
    total += va_arg(arguments, int);
  }
  va_end(arguments);
  return total;
}

static int c89_static_local(void) {
  static int calls  = 0;
  calls            += 1;
  return calls;
}

static int c89_control_flow(int value) {
  int result;
  int index;
  result = 0;
  index  = 0;
  while (index < value) {
    if (index == 1) {
      ++index;
      continue;
    }
    result += index;
    if (result > 20) {
      break;
    }
    ++index;
  }
  do {
    --result;
  } while (result > 6);
  switch (value) {
  case 4:
    result += 10;
    break;
  default:
    result = 0;
    break;
  }
  if (result == 16) {
    goto c89_done;
  }
  result = -1;
c89_done:
  return result;
}

int main(void) {
  auto int           automatic_value;
  register int       register_value;
  extern int         c89_external_value;
  C89SignedInt       signed_value;
  unsigned long      unsigned_value;
  signed char        signed_character;
  float              float_value;
  double             double_value;
  long double        long_double_value;
  wchar_t            wide_character;
  enum C89Color      color;
  struct C89Point    point;
  struct C89Bits     bits;
  union C89Number    number;
  int                array[4];
  int                copied_value;
  int                source_value;
  int               *pointer;
  const int         *const_pointer;
  C89BinaryOperation operation;
  int                arithmetic;
  int                bitwise;
  int                logical;
  int                conditional;
  int                comma_value;
  int                string_length;
  int                static_calls;
  int                variadic_total;
  int                control_total;
  int                standard_macro;

  automatic_value   = 2;
  register_value    = 3;
  signed_value      = -5;
  unsigned_value    = 29UL;
  signed_character  = -7;
  float_value       = 2.5F;
  double_value      = 3.5;
  long_double_value = 4.5L;
  wide_character    = L'Z';
  color             = C89_GREEN;

  point.x        = 31;
  point.y        = 37;
  bits.low       = 6;
  bits.high      = -3;
  number.integer = 41;

  array[0]      = 1;
  array[1]      = 2;
  array[2]      = 3;
  array[3]      = 4;
  pointer       = array;
  const_pointer = pointer;

  source_value = 43;
  copied_value = 0;
  c89_store(&copied_value, &source_value);
  operation = c89_add;

  arithmetic      = (automatic_value + register_value) * 4 - 3;
  arithmetic     /= 17;
  arithmetic     %= 3;
  bitwise         = ((1 << 5) | 3) ^ 2;
  bitwise        &= 31;
  logical         = (signed_value < 0 && unsigned_value > 0UL) || 0;
  conditional     = logical ? 47 : 0;
  comma_value     = (automatic_value += 1, automatic_value + 49);
  string_length   = (int)sizeof(C89_STRINGIFY(C89_MACRO_NUMBER)) - 1;
  static_calls    = c89_static_local() * 10 + c89_static_local();
  variadic_total  = c89_variadic_sum(4, 2, 3, 5, 7);
  control_total   = c89_control_flow(4);

#ifdef __STDC__
  standard_macro = __STDC__;
#else
  standard_macro = 0;
#endif

  printf("%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d "
         "%d %d %d %d %d %d %d %d %d\n",
         c89_external_value, c89_const_global, c89_volatile_global,
         c89_joined_value, C89_CONDITIONAL_VALUE, standard_macro, signed_value,
         (int)unsigned_value, (int)signed_character, (int)float_value,
         (int)double_value, (int)long_double_value, (int)wide_character, color,
         point.x + point.y, bits.low, bits.high, number.integer,
         const_pointer[0] + const_pointer[3], copied_value, operation(53, 6),
         arithmetic, bitwise, logical, conditional, comma_value, string_length,
         static_calls, variadic_total + control_total);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
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
// CHECK-NEXT: struct C89Point {
// CHECK-NEXT:     x: i32,
// CHECK-NEXT:     y: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(4))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct C89Bits {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits4U0,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union C89Number {
// CHECK-NEXT:     integer: i32,
// CHECK-NEXT:     real: f64,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: mod __slate_bits {
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         u8,
// CHECK-NEXT:         c_names = true,
// CHECK-NEXT:         new = false,
// CHECK-NEXT:         from_into_bits = false,
// CHECK-NEXT:         from_traits = false,
// CHECK-NEXT:         default = false,
// CHECK-NEXT:         debug = false,
// CHECK-NEXT:         builder = false,
// CHECK-NEXT:         bit_ops = false
// CHECK-NEXT:     )]
// CHECK-NEXT:     pub struct __SlateBits4U0 {
// CHECK-NEXT:         #[bits(3)]
// CHECK-NEXT:         pub low: u32,
// CHECK-NEXT:         #[bits(4)]
// CHECK-NEXT:         pub high: i32,
// CHECK-NEXT:         #[bits(1, access = na)]
// CHECK-NEXT:         pub __slate_pad_2: u8,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: static mut c89_external_value: i32 = 11 as i32;
// CHECK-NEXT: static mut c89_const_global: i32 = 17 as i32;
// CHECK-NEXT: static mut c89_volatile_global: i32 = 19 as i32;
// CHECK-NEXT: static mut c89_joined_value: i32 = 23 as i32;
// CHECK-NEXT: extern "C-unwind" fn c89_add(left: i32, right: i32) -> i32 {
// CHECK-NEXT:     left + right
// CHECK-NEXT: }
// CHECK-NEXT: fn c89_store(destination: &mut (), source: &()) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *((destination as *mut ()) as *mut i32) = unsafe {
// CHECK-NEXT:             *((source as *const ()) as *const i32)
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C-unwind" fn c89_variadic_sum(count: i32, mut __va_args: ...) -> i32 {
// CHECK-NEXT:     let mut arguments: core::ffi::VaList<'static> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     arguments = unsafe {
// CHECK-NEXT:         core::mem::transmute::<
// CHECK-NEXT:             core::ffi::VaList<'_>,
// CHECK-NEXT:             core::ffi::VaList<'static>,
// CHECK-NEXT:         >(__va_args.clone())
// CHECK-NEXT:     };
// CHECK-NEXT:     for index in 0..count {
// CHECK-NEXT:         let [[V_0:__v[0-9]+]]: i32 = unsafe { arguments.next_arg::<i32>() };
// CHECK-NEXT:         total += [[V_0]];
// CHECK-NEXT:     }
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-NEXT: fn c89_static_local() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         calls = (unsafe { calls }) + 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { calls }
// CHECK-NEXT: }
// CHECK-NEXT: static mut calls: i32 = 0 as i32;
// CHECK-NEXT: fn c89_control_flow(value: i32) -> i32 {
// CHECK-NEXT:     let mut index: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut result: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         result = 0;
// CHECK-NEXT:         index = 0;
// CHECK-NEXT:         loop {
// CHECK-NEXT:             if index < value {
// CHECK-NEXT:                 if index == 1 {
// CHECK-NEXT:                     index += 1;
// CHECK-NEXT:                 } else {
// CHECK-NEXT:                     result += index;
// CHECK-NEXT:                     if result > 20 {
// CHECK-NEXT:                         break;
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         index += 1;
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         loop {
// CHECK-NEXT:             result -= 1;
// CHECK-NEXT:             if !(result > 6) {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: i32 = value;
// CHECK-NEXT:         if [[T_0]] == 4 {
// CHECK-NEXT:             result += 10;
// CHECK-NEXT:         } else {
// CHECK-NEXT:             result = 0;
// CHECK-NEXT:         }
// CHECK-NEXT:         if result != 16 {
// CHECK-NEXT:             result = -1;
// CHECK-NEXT:         }
// CHECK-NEXT:         return result;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut automatic_value: i32 = 2;
// CHECK-NEXT:     let register_value: i32 = 3;
// CHECK-NEXT:     let signed_value: i32 = -5;
// CHECK-NEXT:     let unsigned_value: u64 = 29;
// CHECK-NEXT:     let signed_character: i8 = -7;
// CHECK-NEXT:     let float_value: f32 = 2.5f32;
// CHECK-NEXT:     let double_value: f64 = 3.5f64;
// CHECK-NEXT:     let wide_character: i32 = 90;
// CHECK-NEXT:     let color: u32 = 5;
// CHECK-NEXT:     let mut point: C89Point = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut bits: C89Bits = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut number: C89Number = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut array: __SlateAlign16<[i32; 4]> = __SlateAlign16([0 as i32; 4]);
// CHECK-NEXT:     let mut copied_value: i32 = 0;
// CHECK-NEXT:     let mut source_value: i32 = 43;
// CHECK-NEXT:     let mut arithmetic: i32 = 0;
// CHECK-NEXT:     let mut bitwise: i32 = ((1 as i32) << (5 as i32) | 3) ^ 2;
// CHECK-NEXT:     let mut logical: i32 = 0;
// CHECK-NEXT:     let mut conditional: i32 = 0;
// CHECK-NEXT:     let mut comma_value: i32 = 0;
// CHECK-NEXT:     let string_length: i32 = 3 - 1;
// CHECK-NEXT:     let mut static_calls: i32 = 0;
// CHECK-NEXT:     let mut variadic_total: i32 = 0;
// CHECK-NEXT:     let mut control_total: i32 = 0;
// CHECK-NEXT:     let standard_macro: i32 = 1;
// CHECK-NEXT:     let long_double_value: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 144, 1, 64]);
// CHECK-NEXT:     point.x = 31;
// CHECK-NEXT:     point.y = 37;
// CHECK-NEXT:     bits.__slate_bits_0.__set_low(6 as u32);
// CHECK-NEXT:     bits.__slate_bits_0.__set_high(-(3 as i32));
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         number.integer = 41;
// CHECK-NEXT:     }
// CHECK-NEXT:     array.0[0] = 1;
// CHECK-NEXT:     array.0[1] = 2;
// CHECK-NEXT:     array.0[2] = 3;
// CHECK-NEXT:     array.0[3] = 4;
// CHECK-NEXT:     let pointer: *mut i32 = array.0.as_mut_ptr() as *mut i32;
// CHECK-NEXT:     let const_pointer: *const i32 = pointer as *const i32;
// CHECK-NEXT:     c89_store(
// CHECK-NEXT:         unsafe { &mut (*(std::ptr::addr_of_mut!(copied_value) as *mut ())) },
// CHECK-NEXT:         unsafe { &(*(std::ptr::addr_of_mut!(source_value) as *const ())) },
// CHECK-NEXT:     );
// CHECK-NEXT:     let operation: Option<unsafe extern "C-unwind" fn(i32, i32) -> i32> = Some(c89_add);
// CHECK-NEXT:     arithmetic = (automatic_value + register_value) * 4 - 3;
// CHECK-NEXT:     arithmetic /= 17;
// CHECK-NEXT:     arithmetic %= 3;
// CHECK-NEXT:     bitwise &= 31;
// CHECK-NEXT:     logical = (signed_value < 0 && unsigned_value > 0 || (0 as i32) != (0 as i32))
// CHECK-NEXT:         as i32;
// CHECK-NEXT:     conditional = if logical != 0 { 47 } else { 0 };
// CHECK-NEXT:     automatic_value += 1;
// CHECK-NEXT:     comma_value = automatic_value + 49;
// CHECK-NEXT:     static_calls = c89_static_local() * 10 + c89_static_local();
// CHECK-NEXT:     variadic_total = unsafe {
// CHECK-NEXT:         c89_variadic_sum(4 as i32, 2 as i32, 3 as i32, 5 as i32, 7 as i32)
// CHECK-NEXT:     };
// CHECK-NEXT:     control_total = c89_control_flow(4 as i32);
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {} {} {} {} {} {} {} {} {} {} {} {} {} {} {} {} {} {} {} {} {} {} {} {} {} {} {}",
// CHECK-NEXT:         unsafe { c89_external_value }, unsafe { c89_const_global }, unsafe {
// CHECK-NEXT:         std::ptr::read_volatile(std::ptr::addr_of!(c89_volatile_global)) }, unsafe {
// CHECK-NEXT:         c89_joined_value }, 13 as i32, standard_macro, signed_value, (unsigned_value as
// CHECK-NEXT:         u32) as i32, signed_character as i32, float_value as i32, double_value as i32,
// CHECK-NEXT:         __slate_f80_to_i32(long_double_value), wide_character, color as u32, point.x +
// CHECK-NEXT:         point.y, bits.__slate_bits_0.__get_low() as i32, bits.__slate_bits_0
// CHECK-NEXT:         .__get_high(), unsafe { number.integer }, (unsafe { * unsafe { const_pointer
// CHECK-NEXT:         .offset(0 as isize) } }) + unsafe { * unsafe { const_pointer.offset(3 as isize) }
// CHECK-NEXT:         }, copied_value, unsafe { operation.unwrap() (53 as i32, 6 as i32) }, arithmetic,
// CHECK-NEXT:         bitwise, logical, conditional, comma_value, string_length, static_calls,
// CHECK-NEXT:         variadic_total + control_total
// CHECK-NEXT:     );
// CHECK-NEXT:     0
// CHECK-NEXT: }
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
