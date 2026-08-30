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
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
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
// LOWERING-NEXT: mod __slate_bitfields {
// LOWERING-NEXT:     #[bitfields::bitfield(u8, new = false, from_into_bits = false, from_traits = false, default = false, debug = false, builder = false, bit_ops = false)]
// LOWERING-NEXT:     pub struct __SlateBitfield_C89Bits_0 {
// LOWERING-NEXT:         #[bits(3)]
// LOWERING-NEXT:         pub low: u32,
// LOWERING-NEXT:         #[bits(4)]
// LOWERING-NEXT:         pub high: i32,
// LOWERING-NEXT:         #[bits(1)]
// LOWERING-NEXT:         pub _reserved_0: u128,
// LOWERING-NEXT:     }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[allow(non_camel_case_types)]
// LOWERING-NEXT: #[derive(Clone, Copy, PartialEq, Eq, Debug, Hash)]
// LOWERING-NEXT: enum C89Color {
// LOWERING-NEXT:     C89_RED = 3,
// LOWERING-NEXT:     C89_GREEN = 5,
// LOWERING-NEXT:     C89_BLUE = 7,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct C89Bits {
// LOWERING-NEXT:     __bitfield_0: __slate_bitfields::__SlateBitfield_C89Bits_0,
// LOWERING-NEXT:     __bitfield_1: [u8; 3],
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: union C89Number {
// LOWERING-NEXT:     integer: i32,
// LOWERING-NEXT:     real: f64,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct C89Point {
// LOWERING-NEXT:     x: i32,
// LOWERING-NEXT:     y: i32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut c89_const_global: i32 = 17;
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut c89_external_value: i32 = 11;
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut c89_joined_value: i32 = 23;
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut c89_static_local_calls: i32 = 0;
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut c89_volatile_global: i32 = 19;
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: struct __SlateVaArg {
// LOWERING-NEXT:     value: Box<dyn std::any::Any>,
// LOWERING-NEXT:     size: usize,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl __SlateVaArg {
// LOWERING-NEXT:     fn new<T: 'static>(value: T) -> Self {
// LOWERING-NEXT:         Self { value: Box::new(value), size: std::mem::size_of::<T>() }
// LOWERING-NEXT:     }
// LOWERING-EMPTY:
// LOWERING-NEXT:     fn read<T: Copy + 'static>(&self) -> T {
// LOWERING-NEXT:         if let Some(value) = self.value.downcast_ref::<T>() {
// LOWERING-NEXT:             return *value;
// LOWERING-NEXT:         }
// LOWERING-NEXT:         assert!(self.size >= std::mem::size_of::<T>());
// LOWERING-NEXT:         unsafe {
// LOWERING-NEXT:             std::ptr::read_unaligned(
// LOWERING-NEXT:                 (self.value.as_ref() as *const dyn std::any::Any) as *const () as *const T,
// LOWERING-NEXT:             )
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[derive(Clone)]
// LOWERING-NEXT: struct __SlateVaArgs {
// LOWERING-NEXT:     args: Option<std::rc::Rc<Vec<__SlateVaArg>>>,
// LOWERING-NEXT:     index: usize,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl __SlateVaArgs {
// LOWERING-NEXT:     fn new(args: Vec<__SlateVaArg>) -> Self {
// LOWERING-NEXT:         Self { args: Some(std::rc::Rc::new(args)), index: 0 }
// LOWERING-NEXT:     }
// LOWERING-EMPTY:
// LOWERING-NEXT:     const fn empty() -> Self {
// LOWERING-NEXT:         Self { args: None, index: 0 }
// LOWERING-NEXT:     }
// LOWERING-EMPTY:
// LOWERING-NEXT:     fn next_arg<T: Copy + 'static>(&mut self) -> T {
// LOWERING-NEXT:         if std::mem::size_of::<T>() == 0 {
// LOWERING-NEXT:             return unsafe { std::mem::zeroed() };
// LOWERING-NEXT:         }
// LOWERING-NEXT:         let args = self.args.as_ref().expect("va_arg with no arguments");
// LOWERING-NEXT:         let value = args[self.index].read::<T>();
// LOWERING-NEXT:         self.index += 1;
// LOWERING-NEXT:         value
// LOWERING-NEXT:     }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn c89_store({{arg[0-9]+}}: *mut core::ffi::c_void, {{arg[0-9]+}}: *mut core::ffi::c_void) {
// LOWERING-NEXT:     let mut destination: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let mut source: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let mut output: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut input: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     destination = {{arg[0-9]+}};
// LOWERING-NEXT:     source = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = destination;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = {{_v[0-9]+}} as *mut i32;
// LOWERING-NEXT:     output = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = source;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = {{_v[0-9]+}} as *mut i32;
// LOWERING-NEXT:     input = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = input;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = output;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *{{_v[0-9]+}} = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: extern "C" fn c89_add({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) -> i32 {
// LOWERING-NEXT:     let mut left: i32 = 0;
// LOWERING-NEXT:     let mut right: i32 = 0;
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     left = {{arg[0-9]+}};
// LOWERING-NEXT:     right = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = left;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = right;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn c89_static_local() -> i32 {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { c89_static_local_calls };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         c89_static_local_calls = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { c89_static_local_calls };
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe fn c89_variadic_sum({{arg[0-9]+}}: i32, mut __slate_va_args: __SlateVaArgs) -> i32 {
// LOWERING-NEXT:     let mut count: i32 = 0;
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut arguments: __SlateVaArgs = __SlateVaArgs::empty();
// LOWERING-NEXT:     let mut index: i32 = 0;
// LOWERING-NEXT:     let mut total: i32 = 0;
// LOWERING-NEXT:     count = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     total = {{_v[0-9]+}};
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         arguments = __slate_va_args.clone();
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         index = {{_v[0-9]+}};
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = index;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = count;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { arguments.next_arg::<i32>() };
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
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn c89_control_flow({{arg[0-9]+}}: i32) -> i32 {
// LOWERING-NEXT:     let mut value: i32 = 0;
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut result: i32 = 0;
// LOWERING-NEXT:     let mut index: i32 = 0;
// LOWERING-NEXT:     let mut {{__state[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     '{{__dispatch[0-9]+}}: loop {
// LOWERING-NEXT:         match {{__state[0-9]+}} {
// LOWERING-NEXT:             0 => {
// LOWERING-NEXT:                 value = {{arg[0-9]+}};
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:                 result = {{_v[0-9]+}};
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:                 index = {{_v[0-9]+}};
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     '__loop1: loop {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = index;
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = value;
// LOWERING-NEXT:                         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-NEXT:                         if !{{_v[0-9]+}} {
// LOWERING-NEXT:                             break;
// LOWERING-NEXT:                         }
// LOWERING-NEXT:                         '__continue1: {
// LOWERING-NEXT:                             {
// LOWERING-NEXT:                                 {
// LOWERING-NEXT:                                     let {{_v[0-9]+}}: i32 = index;
// LOWERING-NEXT:                                     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:                                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:                                     if {{_v[0-9]+}} {
// LOWERING-NEXT:                                         let {{_v[0-9]+}}: i32 = index;
// LOWERING-NEXT:                                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-NEXT:                                         index = {{_v[0-9]+}};
// LOWERING-NEXT:                                         break '__continue1;
// LOWERING-NEXT:                                     }
// LOWERING-NEXT:                                 }
// LOWERING-NEXT:                                 let {{_v[0-9]+}}: i32 = index;
// LOWERING-NEXT:                                 let {{_v[0-9]+}}: i32 = result;
// LOWERING-NEXT:                                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:                                 result = {{_v[0-9]+}};
// LOWERING-NEXT:                                 {
// LOWERING-NEXT:                                     let {{_v[0-9]+}}: i32 = result;
// LOWERING-NEXT:                                     let {{_v[0-9]+}}: i32 = 20;
// LOWERING-NEXT:                                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} > {{_v[0-9]+}};
// LOWERING-NEXT:                                     if {{_v[0-9]+}} {
// LOWERING-NEXT:                                         break '__loop1;
// LOWERING-NEXT:                                     }
// LOWERING-NEXT:                                 }
// LOWERING-NEXT:                                 let {{_v[0-9]+}}: i32 = index;
// LOWERING-NEXT:                                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-NEXT:                                 index = {{_v[0-9]+}};
// LOWERING-NEXT:                             }
// LOWERING-NEXT:                         }
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     loop {
// LOWERING-NEXT:                         {
// LOWERING-NEXT:                             let {{_v[0-9]+}}: i32 = result;
// LOWERING-NEXT:                             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} - 1;
// LOWERING-NEXT:                             result = {{_v[0-9]+}};
// LOWERING-NEXT:                         }
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = result;
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = 6;
// LOWERING-NEXT:                         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} > {{_v[0-9]+}};
// LOWERING-NEXT:                         if !{{_v[0-9]+}} {
// LOWERING-NEXT:                             break;
// LOWERING-NEXT:                         }
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = value;
// LOWERING-NEXT:                     {
// LOWERING-NEXT:                         let __switch_value2 = {{_v[0-9]+}};
// LOWERING-NEXT:                         let mut __switch_case2: i32 = match __switch_value2 { 4 => 0, _ => 1 };
// LOWERING-NEXT:                         '__switch2: loop {
// LOWERING-NEXT:                             match __switch_case2 {
// LOWERING-NEXT:                                 0 => {
// LOWERING-NEXT:                                     let {{_v[0-9]+}}: i32 = 10;
// LOWERING-NEXT:                                     let {{_v[0-9]+}}: i32 = result;
// LOWERING-NEXT:                                     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:                                     result = {{_v[0-9]+}};
// LOWERING-NEXT:                                     break '__switch2;
// LOWERING-NEXT:                                 }
// LOWERING-NEXT:                                 1 => {
// LOWERING-NEXT:                                     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:                                     result = {{_v[0-9]+}};
// LOWERING-NEXT:                                     break '__switch2;
// LOWERING-NEXT:                                 }
// LOWERING-NEXT:                                 _ => {
// LOWERING-NEXT:                                     break '__switch2;
// LOWERING-NEXT:                                 }
// LOWERING-NEXT:                             }
// LOWERING-NEXT:                         }
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = result;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = 16;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-NEXT:                         {{__state[0-9]+}} = 1;
// LOWERING-NEXT:                         continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = -1;
// LOWERING-NEXT:                 result = {{_v[0-9]+}};
// LOWERING-NEXT:                 {{__state[0-9]+}} = 1;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             1 => {
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = result;
// LOWERING-NEXT:                 __retval = {{_v[0-9]+}};
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:                 return {{_v[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             _ => {
// LOWERING-NEXT:                 unreachable!();
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut automatic_value: i32 = 0;
// LOWERING-NEXT:     let mut register_value: i32 = 0;
// LOWERING-NEXT:     let mut signed_value: i32 = 0;
// LOWERING-NEXT:     let mut unsigned_value: u64 = 0;
// LOWERING-NEXT:     let mut signed_character: i8 = 0;
// LOWERING-NEXT:     let mut float_value: f32 = 0.0;
// LOWERING-NEXT:     let mut double_value: f64 = 0.0;
// LOWERING-NEXT:     let mut long_double_value: LongDouble = LongDouble([0; 10]);
// LOWERING-NEXT:     let mut wide_character: i32 = 0;
// LOWERING-NEXT:     let mut color: aligned::Aligned<aligned::A4, C89Color> = aligned::Aligned(C89Color::C89_RED);
// LOWERING-NEXT:     let mut point: C89Point = C89Point { x: 0, y: 0 };
// LOWERING-NEXT:     let mut bits: C89Bits = C89Bits { __bitfield_0: unsafe { std::mem::transmute::<u8, __slate_bitfields::__SlateBitfield_C89Bits_0>(0) }, __bitfield_1: [0; 3] };
// LOWERING-NEXT:     let mut number: C89Number = C89Number { integer: 0 };
// LOWERING-NEXT:     let mut array: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// LOWERING-NEXT:     let mut copied_value: i32 = 0;
// LOWERING-NEXT:     let mut source_value: i32 = 0;
// LOWERING-NEXT:     let mut pointer: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut const_pointer: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut operation: Option<unsafe extern "C" fn(i32, i32) -> i32> = None;
// LOWERING-NEXT:     let mut arithmetic: i32 = 0;
// LOWERING-NEXT:     let mut bitwise: i32 = 0;
// LOWERING-NEXT:     let mut logical: i32 = 0;
// LOWERING-NEXT:     let mut conditional: i32 = 0;
// LOWERING-NEXT:     let mut comma_value: i32 = 0;
// LOWERING-NEXT:     let mut string_length: i32 = 0;
// LOWERING-NEXT:     let mut static_calls: i32 = 0;
// LOWERING-NEXT:     let mut variadic_total: i32 = 0;
// LOWERING-NEXT:     let mut control_total: i32 = 0;
// LOWERING-NEXT:     let mut standard_macro: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 2;
// LOWERING-NEXT:     automatic_value = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:     register_value = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = -5;
// LOWERING-NEXT:     signed_value = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 29;
// LOWERING-NEXT:     unsigned_value = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i8 = -7;
// LOWERING-NEXT:     signed_character = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = 2.5;
// LOWERING-NEXT:     float_value = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 3.5;
// LOWERING-NEXT:     double_value = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 144, 1, 64]);
// LOWERING-NEXT:     long_double_value = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 90;
// LOWERING-NEXT:     wide_character = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = C89Color::C89_GREEN as u32;
// LOWERING-NEXT:     *color = C89Color::C89_GREEN;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 31;
// LOWERING-NEXT:     point.x = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 37;
// LOWERING-NEXT:     point.y = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = 6;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = ({{_v[0-9]+}} as u32) << 29 >> 29;
// LOWERING-NEXT:     bits.__bitfield_0.set_low(({{_v[0-9]+}} as u32) << 29 >> 29);
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = -3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = ({{_v[0-9]+}} as i32) << 28 >> 28;
// LOWERING-NEXT:     bits.__bitfield_0.set_high(({{_v[0-9]+}} as i32) << 28 >> 28);
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 41;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         number.integer = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     array[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 1;
// LOWERING-NEXT:     array[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     array[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 4;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     array[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = array.as_mut_ptr() as *mut i32;
// LOWERING-NEXT:     pointer = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = pointer;
// LOWERING-NEXT:     const_pointer = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 43;
// LOWERING-NEXT:     source_value = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     copied_value = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::addr_of_mut!(copied_value) as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::addr_of_mut!(source_value) as *mut core::ffi::c_void;
// LOWERING-NEXT:     c89_store({{_v[0-9]+}} as *mut core::ffi::c_void, {{_v[0-9]+}} as *mut core::ffi::c_void);
// LOWERING-NEXT:     operation = unsafe { std::mem::transmute::<*const (), Option<unsafe extern "C" fn(i32, i32) -> i32>>(c89_add as *const ()) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = automatic_value;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = register_value;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 4;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     arithmetic = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 17;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = arithmetic;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} / {{_v[0-9]+}};
// LOWERING-NEXT:     arithmetic = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = arithmetic;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} % {{_v[0-9]+}};
// LOWERING-NEXT:     arithmetic = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 5;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} | {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} ^ {{_v[0-9]+}};
// LOWERING-NEXT:     bitwise = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 31;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = bitwise;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} & {{_v[0-9]+}};
// LOWERING-NEXT:     bitwise = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = signed_value;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: u64 = unsigned_value;
// LOWERING-NEXT:         let {{_v[0-9]+}}: u64 = 0;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} > {{_v[0-9]+}};
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = true;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     logical = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = logical;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 47;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} { {{_v[0-9]+}} } else { {{_v[0-9]+}} };
// LOWERING-NEXT:     conditional = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = automatic_value;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     automatic_value = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = automatic_value;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 49;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     comma_value = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     string_length = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = c89_static_local();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 10;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = c89_static_local();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     static_calls = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 4;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 5;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 7;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { c89_variadic_sum({{_v[0-9]+}}, __SlateVaArgs::new(vec![__SlateVaArg::new({{_v[0-9]+}}), __SlateVaArg::new({{_v[0-9]+}}), __SlateVaArg::new({{_v[0-9]+}}), __SlateVaArg::new({{_v[0-9]+}})])) };
// LOWERING-NEXT:     variadic_total = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 4;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = c89_control_flow({{_v[0-9]+}});
// LOWERING-NEXT:     control_total = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     standard_macro = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { c89_external_value };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { c89_const_global };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(c89_volatile_global)) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { c89_joined_value };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 13;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = standard_macro;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = signed_value;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = unsigned_value;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i8 = signed_character;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = float_value;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = double_value;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = long_double_value;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = __slate_f80_to_i32({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = wide_character;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = *color as u32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = point.x;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = point.y;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = (bits.__bitfield_0.low() as u32) << 29 >> 29;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = (bits.__bitfield_0.high() as i32) << 28 >> 28;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { number.integer };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = const_pointer;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.add(0) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = const_pointer;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.add(3) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = copied_value;
// LOWERING-NEXT:     let {{_v[0-9]+}}: Option<unsafe extern "C" fn(i32, i32) -> i32> = operation;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 53;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 6;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { {{_v[0-9]+}}.unwrap()({{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = arithmetic;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = bitwise;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = logical;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = conditional;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = comma_value;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = string_length;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = static_calls;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = variadic_total;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = control_total;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
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
// REWRITES-NEXT: mod __slate_bitfields {
// REWRITES-NEXT:     #[bitfields::bitfield(u8, new = false, from_into_bits = false, from_traits = false, default = false, debug = false, builder = false, bit_ops = false)]
// REWRITES-NEXT:     pub struct __SlateBitfield_C89Bits_0 {
// REWRITES-NEXT:         #[bits(3)]
// REWRITES-NEXT:         pub low: u32,
// REWRITES-NEXT:         #[bits(4)]
// REWRITES-NEXT:         pub high: i32,
// REWRITES-NEXT:         #[bits(1)]
// REWRITES-NEXT:         pub _reserved_0: u128,
// REWRITES-NEXT:     }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[allow(non_camel_case_types)]
// REWRITES-NEXT: #[derive(Clone, Copy, PartialEq, Eq, Debug, Hash)]
// REWRITES-NEXT: enum C89Color {
// REWRITES-NEXT:     C89_RED = 3,
// REWRITES-NEXT:     C89_GREEN = 5,
// REWRITES-NEXT:     C89_BLUE = 7,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct C89Bits {
// REWRITES-NEXT:     __bitfield_0: __slate_bitfields::__SlateBitfield_C89Bits_0,
// REWRITES-NEXT:     __bitfield_1: [u8; 3],
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: union C89Number {
// REWRITES-NEXT:     integer: i32,
// REWRITES-NEXT:     real: f64,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct C89Point {
// REWRITES-NEXT:     x: i32,
// REWRITES-NEXT:     y: i32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut c89_const_global: i32 = 17;
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut c89_external_value: i32 = 11;
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut c89_joined_value: i32 = 23;
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut c89_static_local_calls: i32 = 0;
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut c89_volatile_global: i32 = 19;
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: struct __SlateVaArg {
// REWRITES-NEXT:     value: Box<dyn std::any::Any>,
// REWRITES-NEXT:     size: usize,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl __SlateVaArg {
// REWRITES-NEXT:     fn new<T: 'static>(value: T) -> Self {
// REWRITES-NEXT:         Self { value: Box::new(value), size: std::mem::size_of::<T>() }
// REWRITES-NEXT:     }
// REWRITES-EMPTY:
// REWRITES-NEXT:     fn read<T: Copy + 'static>(&self) -> T {
// REWRITES-NEXT:         if let Some(value) = self.value.downcast_ref::<T>() {
// REWRITES-NEXT:             return *value;
// REWRITES-NEXT:         }
// REWRITES-NEXT:         assert!(self.size >= std::mem::size_of::<T>());
// REWRITES-NEXT:         unsafe {
// REWRITES-NEXT:             std::ptr::read_unaligned(
// REWRITES-NEXT:                 (self.value.as_ref() as *const dyn std::any::Any) as *const () as *const T,
// REWRITES-NEXT:             )
// REWRITES-NEXT:         }
// REWRITES-NEXT:     }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[derive(Clone)]
// REWRITES-NEXT: struct __SlateVaArgs {
// REWRITES-NEXT:     args: Option<std::rc::Rc<Vec<__SlateVaArg>>>,
// REWRITES-NEXT:     index: usize,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl __SlateVaArgs {
// REWRITES-NEXT:     fn new(args: Vec<__SlateVaArg>) -> Self {
// REWRITES-NEXT:         Self { args: Some(std::rc::Rc::new(args)), index: 0 }
// REWRITES-NEXT:     }
// REWRITES-EMPTY:
// REWRITES-NEXT:     const fn empty() -> Self {
// REWRITES-NEXT:         Self { args: None, index: 0 }
// REWRITES-NEXT:     }
// REWRITES-EMPTY:
// REWRITES-NEXT:     fn next_arg<T: Copy + 'static>(&mut self) -> T {
// REWRITES-NEXT:         if std::mem::size_of::<T>() == 0 {
// REWRITES-NEXT:             return unsafe { std::mem::zeroed() };
// REWRITES-NEXT:         }
// REWRITES-NEXT:         let args = self.args.as_ref().expect("va_arg with no arguments");
// REWRITES-NEXT:         let value = args[self.index].read::<T>();
// REWRITES-NEXT:         self.index += 1;
// REWRITES-NEXT:         value
// REWRITES-NEXT:     }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn c89_store({{arg[0-9]+}}: *mut core::ffi::c_void, {{arg[0-9]+}}: *mut core::ffi::c_void) {
// REWRITES-NEXT: let mut destination: *mut core::ffi::c_void = {{arg[0-9]+}};
// REWRITES-NEXT: let mut source: *mut core::ffi::c_void = {{arg[0-9]+}};
// REWRITES-NEXT: let mut output: *mut i32 = std::ptr::null_mut();
// REWRITES-NEXT: let mut input: *mut i32 = std::ptr::null_mut();
// REWRITES-NEXT: output = destination as *mut i32;
// REWRITES-NEXT: input = source as *mut i32;
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         *output = unsafe { *input };
// REWRITES-NEXT: }
// REWRITES-NEXT: return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: extern "C" fn c89_add({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) -> i32 {
// REWRITES-NEXT: let mut left: i32 = {{arg[0-9]+}};
// REWRITES-NEXT: let mut right: i32 = {{arg[0-9]+}};
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: __retval = left + right;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn c89_static_local() -> i32 {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         c89_static_local_calls = (unsafe { c89_static_local_calls }) + {{_v[0-9]+}};
// REWRITES-NEXT: }
// REWRITES-NEXT: __retval = unsafe { c89_static_local_calls };
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe fn c89_variadic_sum({{arg[0-9]+}}: i32, mut __slate_va_args: __SlateVaArgs) -> i32 {
// REWRITES-NEXT: let mut count: i32 = {{arg[0-9]+}};
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut arguments: __SlateVaArgs = __SlateVaArgs::empty();
// REWRITES-NEXT: let mut index: i32 = 0;
// REWRITES-NEXT: let mut total: i32 = 0;
// REWRITES-NEXT: total = 0;
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         arguments = __slate_va_args.clone();
// REWRITES-NEXT: }
// REWRITES-NEXT: {
// REWRITES-NEXT:         index = 0;
// REWRITES-NEXT:         loop {
// REWRITES-NEXT:                     if !(index < count) {
// REWRITES-NEXT:                                     break;
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     {
// REWRITES-NEXT:                                     let {{_v[0-9]+}}: i32 = unsafe { arguments.next_arg::<i32>() };
// REWRITES-NEXT:                                     total = total + {{_v[0-9]+}};
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     index = index + 1;
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: __retval = total;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn c89_control_flow({{arg[0-9]+}}: i32) -> i32 {
// REWRITES-NEXT: let mut value: i32 = 0;
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut result: i32 = 0;
// REWRITES-NEXT: let mut index: i32 = 0;
// REWRITES-NEXT: let mut {{__state[0-9]+}}: i32 = 0;
// REWRITES-NEXT: '{{__dispatch[0-9]+}}: loop {
// REWRITES-NEXT:         match {{__state[0-9]+}} {
// REWRITES-NEXT:             0 => {
// REWRITES-NEXT:                         value = {{arg[0-9]+}};
// REWRITES-NEXT:                         result = 0;
// REWRITES-NEXT:                         index = 0;
// REWRITES-NEXT:                         '__loop1: loop {
// REWRITES-NEXT:                                         if !(index < value) {
// REWRITES-NEXT:                                                             break;
// REWRITES-NEXT:                                         }
// REWRITES-NEXT:                                         '__continue1: {
// REWRITES-NEXT:                                                             {
// REWRITES-NEXT:                                                                                     {
// REWRITES-NEXT:                                                                                                                 let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT:                                                                                                                 if index == {{_v[0-9]+}} {
// REWRITES-NEXT:                                                                                                                                                 index = index + 1;
// REWRITES-NEXT:                                                                                                                                                 break '__continue1;
// REWRITES-NEXT:                                                                                                                 }
// REWRITES-NEXT:                                                                                     }
// REWRITES-NEXT:                                                                                     result = result + index;
// REWRITES-NEXT:                                                                                     {
// REWRITES-NEXT:                                                                                                                 let {{_v[0-9]+}}: i32 = 20;
// REWRITES-NEXT:                                                                                                                 if result > {{_v[0-9]+}} {
// REWRITES-NEXT:                                                                                                                                                 break '__loop1;
// REWRITES-NEXT:                                                                                                                 }
// REWRITES-NEXT:                                                                                     }
// REWRITES-NEXT:                                                                                     index = index + 1;
// REWRITES-NEXT:                                                             }
// REWRITES-NEXT:                                         }
// REWRITES-NEXT:                         }
// REWRITES-NEXT:                         loop {
// REWRITES-NEXT:                                         {
// REWRITES-NEXT:                                                             result = result - 1;
// REWRITES-NEXT:                                         }
// REWRITES-NEXT:                                         let {{_v[0-9]+}}: i32 = 6;
// REWRITES-NEXT:                                         if !(result > {{_v[0-9]+}}) {
// REWRITES-NEXT:                                                             break;
// REWRITES-NEXT:                                         }
// REWRITES-NEXT:                         }
// REWRITES-NEXT:                         {
// REWRITES-NEXT:                                         {
// REWRITES-NEXT:                                                             let __switch_value2 = value;
// REWRITES-NEXT:                                                             let mut __switch_case2: i32 = match __switch_value2 { 4 => 0, _ => 1 };
// REWRITES-NEXT:                                                             '__switch2: loop {
// REWRITES-NEXT:                                                                                     match __switch_case2 {
// REWRITES-NEXT:                                                                                         0 => {
// REWRITES-NEXT:                                                                                                                     let {{_v[0-9]+}}: i32 = 10;
// REWRITES-NEXT:                                                                                                                     result = result + {{_v[0-9]+}};
// REWRITES-NEXT:                                                                                                                     break '__switch2;
// REWRITES-NEXT:                                                                                         }
// REWRITES-NEXT:                                                                                         1 => {
// REWRITES-NEXT:                                                                                                                     result = 0;
// REWRITES-NEXT:                                                                                                                     break '__switch2;
// REWRITES-NEXT:                                                                                         }
// REWRITES-NEXT:                                                                                         _ => {
// REWRITES-NEXT:                                                                                                                     break '__switch2;
// REWRITES-NEXT:                                                                                         }
// REWRITES-NEXT:                                                                                     }
// REWRITES-NEXT:                                                             }
// REWRITES-NEXT:                                         }
// REWRITES-NEXT:                         }
// REWRITES-NEXT:                         {
// REWRITES-NEXT:                                         let {{_v[0-9]+}}: i32 = 16;
// REWRITES-NEXT:                                         if result == {{_v[0-9]+}} {
// REWRITES-NEXT:                                                             {{__state[0-9]+}} = 1;
// REWRITES-NEXT:                                                             continue '{{__dispatch[0-9]+}};
// REWRITES-NEXT:                                         }
// REWRITES-NEXT:                         }
// REWRITES-NEXT:                         result = -1;
// REWRITES-NEXT:                         {{__state[0-9]+}} = 1;
// REWRITES-NEXT:                         continue '{{__dispatch[0-9]+}};
// REWRITES-NEXT:             }
// REWRITES-NEXT:             1 => {
// REWRITES-NEXT:                         __retval = result;
// REWRITES-NEXT:                         return __retval;
// REWRITES-NEXT:             }
// REWRITES-NEXT:             _ => {
// REWRITES-NEXT:                         unreachable!();
// REWRITES-NEXT:             }
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut automatic_value: i32 = 0;
// REWRITES-NEXT: let mut register_value: i32 = 0;
// REWRITES-NEXT: let mut signed_value: i32 = 0;
// REWRITES-NEXT: let mut unsigned_value: u64 = 0;
// REWRITES-NEXT: let mut signed_character: i8 = 0;
// REWRITES-NEXT: let mut float_value: f32 = 0.0;
// REWRITES-NEXT: let mut double_value: f64 = 0.0;
// REWRITES-NEXT: let mut long_double_value: LongDouble = LongDouble([0; 10]);
// REWRITES-NEXT: let mut wide_character: i32 = 0;
// REWRITES-NEXT: let mut color: aligned::Aligned<aligned::A4, C89Color> = aligned::Aligned(C89Color::C89_RED);
// REWRITES-NEXT: let mut point: C89Point = C89Point { x: 0, y: 0 };
// REWRITES-NEXT: let mut bits: C89Bits = C89Bits { __bitfield_0: unsafe { std::mem::transmute::<u8, __slate_bitfields::__SlateBitfield_C89Bits_0>(0) }, __bitfield_1: [0; 3] };
// REWRITES-NEXT: let mut number: C89Number = C89Number { integer: 0 };
// REWRITES-NEXT: let mut array: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// REWRITES-NEXT: let mut copied_value: i32 = 0;
// REWRITES-NEXT: let mut source_value: i32 = 0;
// REWRITES-NEXT: let mut pointer: *mut i32 = std::ptr::null_mut();
// REWRITES-NEXT: let mut const_pointer: *mut i32 = std::ptr::null_mut();
// REWRITES-NEXT: let mut operation: Option<unsafe extern "C" fn(i32, i32) -> i32> = None;
// REWRITES-NEXT: let mut arithmetic: i32 = 0;
// REWRITES-NEXT: let mut bitwise: i32 = 0;
// REWRITES-NEXT: let mut logical: i32 = 0;
// REWRITES-NEXT: let mut conditional: i32 = 0;
// REWRITES-NEXT: let mut comma_value: i32 = 0;
// REWRITES-NEXT: let mut string_length: i32 = 0;
// REWRITES-NEXT: let mut static_calls: i32 = 0;
// REWRITES-NEXT: let mut variadic_total: i32 = 0;
// REWRITES-NEXT: let mut control_total: i32 = 0;
// REWRITES-NEXT: let mut standard_macro: i32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: automatic_value = 2;
// REWRITES-NEXT: register_value = 3;
// REWRITES-NEXT: signed_value = -5;
// REWRITES-NEXT: unsigned_value = 29;
// REWRITES-NEXT: signed_character = -7;
// REWRITES-NEXT: float_value = 2.5;
// REWRITES-NEXT: double_value = 3.5;
// REWRITES-NEXT: long_double_value = LongDouble([0, 0, 0, 0, 0, 0, 0, 144, 1, 64]);
// REWRITES-NEXT: wide_character = 90;
// REWRITES-NEXT: let {{_v[0-9]+}}: u32 = C89Color::C89_GREEN as u32;
// REWRITES-NEXT: *color = C89Color::C89_GREEN;
// REWRITES-NEXT: point.x = 31;
// REWRITES-NEXT: point.y = 37;
// REWRITES-NEXT: let {{_v[0-9]+}}: u32 = 6;
// REWRITES-NEXT: let {{_v[0-9]+}}: u32 = ({{_v[0-9]+}} as u32) << 29 >> 29;
// REWRITES-NEXT: bits.__bitfield_0.set_low(({{_v[0-9]+}} as u32) << 29 >> 29);
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = -3;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = ({{_v[0-9]+}} as i32) << 28 >> 28;
// REWRITES-NEXT: bits.__bitfield_0.set_high(({{_v[0-9]+}} as i32) << 28 >> 28);
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         number.integer = 41;
// REWRITES-NEXT: }
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = 0;
// REWRITES-NEXT: array[({{_v[0-9]+}} as usize)] = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = 1;
// REWRITES-NEXT: array[({{_v[0-9]+}} as usize)] = 2;
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = 2;
// REWRITES-NEXT: array[({{_v[0-9]+}} as usize)] = 3;
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = 3;
// REWRITES-NEXT: array[({{_v[0-9]+}} as usize)] = 4;
// REWRITES-NEXT: pointer = array.as_mut_ptr() as *mut i32;
// REWRITES-NEXT: const_pointer = pointer;
// REWRITES-NEXT: source_value = 43;
// REWRITES-NEXT: copied_value = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::addr_of_mut!(copied_value) as *mut core::ffi::c_void;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::addr_of_mut!(source_value) as *mut core::ffi::c_void;
// REWRITES-NEXT: c89_store({{_v[0-9]+}} as *mut core::ffi::c_void, {{_v[0-9]+}} as *mut core::ffi::c_void);
// REWRITES-NEXT: operation = unsafe { std::mem::transmute::<*const (), Option<unsafe extern "C" fn(i32, i32) -> i32>>(c89_add as *const ()) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 4;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 3;
// REWRITES-NEXT: arithmetic = (automatic_value + register_value) * {{_v[0-9]+}} - {{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 17;
// REWRITES-NEXT: arithmetic = arithmetic / {{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 3;
// REWRITES-NEXT: arithmetic = arithmetic % {{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 5;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 3;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 2;
// REWRITES-NEXT: bitwise = ({{_v[0-9]+}} << {{_v[0-9]+}} | {{_v[0-9]+}}) ^ {{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 31;
// REWRITES-NEXT: bitwise = bitwise & {{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: bool = if signed_value < {{_v[0-9]+}} {
// REWRITES-NEXT:         let {{_v[0-9]+}}: u64 = 0;
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = unsigned_value > {{_v[0-9]+}};
// REWRITES-NEXT:     {{_v[0-9]+}}
// REWRITES-NEXT: } else {
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = false;
// REWRITES-NEXT:     {{_v[0-9]+}}
// REWRITES-NEXT: };
// REWRITES-NEXT: let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = true;
// REWRITES-NEXT:     {{_v[0-9]+}}
// REWRITES-NEXT: } else {
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// REWRITES-NEXT:     {{_v[0-9]+}}
// REWRITES-NEXT: };
// REWRITES-NEXT: logical = {{_v[0-9]+}} as i32;
// REWRITES-NEXT: let {{_v[0-9]+}}: bool = logical != 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 47;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: conditional = if {{_v[0-9]+}} { {{_v[0-9]+}} } else { {{_v[0-9]+}} };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: automatic_value = automatic_value + {{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 49;
// REWRITES-NEXT: comma_value = automatic_value + {{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 3;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: string_length = {{_v[0-9]+}} - {{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = c89_static_local();
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 10;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} * {{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = c89_static_local();
// REWRITES-NEXT: static_calls = {{_v[0-9]+}} + {{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 4;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 2;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 3;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 5;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 7;
// REWRITES-NEXT: variadic_total = unsafe { c89_variadic_sum({{_v[0-9]+}}, __SlateVaArgs::new(vec![__SlateVaArg::new({{_v[0-9]+}}), __SlateVaArg::new({{_v[0-9]+}}), __SlateVaArg::new({{_v[0-9]+}}), __SlateVaArg::new({{_v[0-9]+}})])) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 4;
// REWRITES-NEXT: control_total = c89_control_flow({{_v[0-9]+}});
// REWRITES-NEXT: standard_macro = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { c89_external_value };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { c89_const_global };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(c89_volatile_global)) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { c89_joined_value };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 13;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = standard_macro;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = signed_value;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsigned_value as i32;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = signed_character as i32;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = float_value as i32;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = double_value as i32;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = __slate_f80_to_i32(long_double_value);
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = wide_character;
// REWRITES-NEXT: let {{_v[0-9]+}}: u32 = *color as u32;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = point.x + point.y;
// REWRITES-NEXT: let {{_v[0-9]+}}: u32 = (bits.__bitfield_0.low() as u32) << 29 >> 29;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = (bits.__bitfield_0.high() as i32) << 28 >> 28;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { number.integer };
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i32 = const_pointer;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.add(0) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { *{{_v[0-9]+}} };
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = 3;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i32 = const_pointer;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.add(3) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + unsafe { *{{_v[0-9]+}} };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = copied_value;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 53;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 6;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { operation.unwrap()({{_v[0-9]+}}, {{_v[0-9]+}}) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, arithmetic, bitwise, logical, conditional, comma_value, string_length, static_calls, variadic_total + control_total) };
// REWRITES-NEXT: __retval = 0;
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
