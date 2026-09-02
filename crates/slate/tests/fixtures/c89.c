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
// LOWERING-NEXT: mod __slate_bitfields {
// LOWERING-NEXT:     #[bitfields::bitfield(
// LOWERING-NEXT:         u8,
// LOWERING-NEXT:         new = false,
// LOWERING-NEXT:         from_into_bits = false,
// LOWERING-NEXT:         from_traits = false,
// LOWERING-NEXT:         default = false,
// LOWERING-NEXT:         debug = false,
// LOWERING-NEXT:         builder = false,
// LOWERING-NEXT:         bit_ops = false
// LOWERING-NEXT:     )]
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
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct C89Bits {
// LOWERING-NEXT:     __bitfield_0: __slate_bitfields::__SlateBitfield_C89Bits_0,
// LOWERING-NEXT:     __bitfield_1: [u8; 3],
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: union C89Number {
// LOWERING-NEXT:     integer: i32,
// LOWERING-NEXT:     real: f64,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct C89Point {
// LOWERING-NEXT:     x: i32,
// LOWERING-NEXT:     y: i32,
// LOWERING-NEXT: }
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
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: struct __SlateVaArg {
// LOWERING-NEXT:     value: Box<dyn std::any::Any>,
// LOWERING-NEXT:     size: usize,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl __SlateVaArg {
// LOWERING-NEXT:     fn new<T: 'static>(value: T) -> Self {
// LOWERING-NEXT:         Self {
// LOWERING-NEXT:             value: Box::new(value),
// LOWERING-NEXT:             size: std::mem::size_of::<T>(),
// LOWERING-NEXT:         }
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
// LOWERING-NEXT:         Self {
// LOWERING-NEXT:             args: Some(std::rc::Rc::new(args)),
// LOWERING-NEXT:             index: 0,
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-EMPTY:
// LOWERING-NEXT:     const fn empty() -> Self {
// LOWERING-NEXT:         Self {
// LOWERING-NEXT:             args: None,
// LOWERING-NEXT:             index: 0,
// LOWERING-NEXT:         }
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
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = {{arg[0-9]+}} as *mut i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = {{arg[0-9]+}} as *mut i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *{{_v[0-9]+}} = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: extern "C" fn c89_add({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) -> i32 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{arg[0-9]+}} + {{arg[0-9]+}};
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn c89_static_local() -> i32 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { c89_static_local_calls };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         c89_static_local_calls = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { c89_static_local_calls };
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe fn c89_variadic_sum({{arg[0-9]+}}: i32, mut __slate_va_args: __SlateVaArgs) -> i32 {
// LOWERING-NEXT:     let mut count: i32 = 0;
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
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn c89_control_flow({{arg[0-9]+}}: i32) -> i32 {
// LOWERING-NEXT:     let mut value: i32 = 0;
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut result: i32 = 0;
// LOWERING-NEXT:     let mut index: i32 = 0;
// LOWERING-NEXT:     let mut {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let mut {{__state[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     '{{__dispatch[0-9]+}}: loop {
// LOWERING-NEXT:         match {{__state[0-9]+}} {
// LOWERING-NEXT:             0 => {
// LOWERING-NEXT:                 value = {{arg[0-9]+}};
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:                 result = {{_v[0-9]+}};
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:                 index = {{_v[0-9]+}};
// LOWERING-NEXT:                 {{__state[0-9]+}} = 1;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             1 => {
// LOWERING-NEXT:                 {{__state[0-9]+}} = 2;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             2 => {
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = index;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = value;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-NEXT:                 if {{_v[0-9]+}} {
// LOWERING-NEXT:                     {{__state[0-9]+}} = 3;
// LOWERING-NEXT:                 } else {
// LOWERING-NEXT:                     {{__state[0-9]+}} = 14;
// LOWERING-NEXT:                 }
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             3 => {
// LOWERING-NEXT:                 {{__state[0-9]+}} = 4;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             4 => {
// LOWERING-NEXT:                 {{__state[0-9]+}} = 5;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             5 => {
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = index;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:                 if {{_v[0-9]+}} {
// LOWERING-NEXT:                     {{__state[0-9]+}} = 6;
// LOWERING-NEXT:                 } else {
// LOWERING-NEXT:                     {{__state[0-9]+}} = 7;
// LOWERING-NEXT:                 }
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             6 => {
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = index;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-NEXT:                 index = {{_v[0-9]+}};
// LOWERING-NEXT:                 {{__state[0-9]+}} = 2;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             7 => {
// LOWERING-NEXT:                 {{__state[0-9]+}} = 8;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             8 => {
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = index;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = result;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:                 result = {{_v[0-9]+}};
// LOWERING-NEXT:                 {{__state[0-9]+}} = 9;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             9 => {
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = result;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = 20;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: bool = {{_v[0-9]+}} > {{_v[0-9]+}};
// LOWERING-NEXT:                 if {{_v[0-9]+}} {
// LOWERING-NEXT:                     {{__state[0-9]+}} = 10;
// LOWERING-NEXT:                 } else {
// LOWERING-NEXT:                     {{__state[0-9]+}} = 11;
// LOWERING-NEXT:                 }
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             10 => {
// LOWERING-NEXT:                 {{__state[0-9]+}} = 14;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             11 => {
// LOWERING-NEXT:                 {{__state[0-9]+}} = 12;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             12 => {
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = index;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-NEXT:                 index = {{_v[0-9]+}};
// LOWERING-NEXT:                 {{__state[0-9]+}} = 13;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             13 => {
// LOWERING-NEXT:                 {{__state[0-9]+}} = 2;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             14 => {
// LOWERING-NEXT:                 {{__state[0-9]+}} = 15;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             15 => {
// LOWERING-NEXT:                 {{__state[0-9]+}} = 16;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             16 => {
// LOWERING-NEXT:                 {{__state[0-9]+}} = 18;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             17 => {
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = result;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = 6;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: bool = {{_v[0-9]+}} > {{_v[0-9]+}};
// LOWERING-NEXT:                 if {{_v[0-9]+}} {
// LOWERING-NEXT:                     {{__state[0-9]+}} = 18;
// LOWERING-NEXT:                 } else {
// LOWERING-NEXT:                     {{__state[0-9]+}} = 21;
// LOWERING-NEXT:                 }
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             18 => {
// LOWERING-NEXT:                 {{__state[0-9]+}} = 19;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             19 => {
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = result;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} - 1;
// LOWERING-NEXT:                 result = {{_v[0-9]+}};
// LOWERING-NEXT:                 {{__state[0-9]+}} = 20;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             20 => {
// LOWERING-NEXT:                 {{__state[0-9]+}} = 17;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             21 => {
// LOWERING-NEXT:                 {{__state[0-9]+}} = 22;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             22 => {
// LOWERING-NEXT:                 {{__state[0-9]+}} = 23;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             23 => {
// LOWERING-NEXT:                 {{_v[0-9]+}} = value;
// LOWERING-NEXT:                 {{__state[0-9]+}} = 24;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             24 => {
// LOWERING-NEXT:                 match {{_v[0-9]+}} {
// LOWERING-NEXT:                     4 => {
// LOWERING-NEXT:                         {{__state[0-9]+}} = 26;
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                     _ => {
// LOWERING-NEXT:                         {{__state[0-9]+}} = 27;
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             25 => {
// LOWERING-NEXT:                 {{__state[0-9]+}} = 26;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             26 => {
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = 10;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = result;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:                 result = {{_v[0-9]+}};
// LOWERING-NEXT:                 {{__state[0-9]+}} = 28;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             27 => {
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:                 result = {{_v[0-9]+}};
// LOWERING-NEXT:                 {{__state[0-9]+}} = 28;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             28 => {
// LOWERING-NEXT:                 {{__state[0-9]+}} = 29;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             29 => {
// LOWERING-NEXT:                 {{__state[0-9]+}} = 30;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             30 => {
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = result;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = 16;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:                 if {{_v[0-9]+}} {
// LOWERING-NEXT:                     {{__state[0-9]+}} = 31;
// LOWERING-NEXT:                 } else {
// LOWERING-NEXT:                     {{__state[0-9]+}} = 32;
// LOWERING-NEXT:                 }
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             31 => {
// LOWERING-NEXT:                 {{__state[0-9]+}} = 34;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             32 => {
// LOWERING-NEXT:                 {{__state[0-9]+}} = 33;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             33 => {
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = -1;
// LOWERING-NEXT:                 result = {{_v[0-9]+}};
// LOWERING-NEXT:                 {{__state[0-9]+}} = 34;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             34 => {
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
// LOWERING-NEXT:     let mut unsigned_value: u64 = 0;
// LOWERING-NEXT:     let mut point: C89Point = C89Point { x: 0, y: 0 };
// LOWERING-NEXT:     let mut bits: C89Bits = C89Bits {
// LOWERING-NEXT:         __bitfield_0: unsafe {
// LOWERING-NEXT:             std::mem::transmute::<u8, __slate_bitfields::__SlateBitfield_C89Bits_0>(0)
// LOWERING-NEXT:         },
// LOWERING-NEXT:         __bitfield_1: [0; 3],
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let mut number: C89Number = unsafe { std::mem::zeroed::<C89Number>() };
// LOWERING-NEXT:     let mut array: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// LOWERING-NEXT:     let mut copied_value: i32 = 0;
// LOWERING-NEXT:     let mut source_value: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = -5;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 29;
// LOWERING-NEXT:     unsigned_value = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i8 = -7;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = 2.5;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 3.5;
// LOWERING-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 144, 1, 64]);
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 90;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = C89Color::C89_GREEN as u32;
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
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 43;
// LOWERING-NEXT:     source_value = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     copied_value = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void =
// LOWERING-NEXT:         std::ptr::addr_of_mut!(copied_value) as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void =
// LOWERING-NEXT:         std::ptr::addr_of_mut!(source_value) as *mut core::ffi::c_void;
// LOWERING-NEXT:     c89_store(
// LOWERING-NEXT:         {{_v[0-9]+}} as *mut core::ffi::c_void,
// LOWERING-NEXT:         {{_v[0-9]+}} as *mut core::ffi::c_void,
// LOWERING-NEXT:     );
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 4;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 17;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} / {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} % {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 5;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} << {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} | {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} ^ {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 31;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} & {{_v[0-9]+}};
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
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 47;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} { {{_v[0-9]+}} } else { {{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 49;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = c89_static_local();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 10;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = c89_static_local();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 4;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 5;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 7;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         c89_variadic_sum(
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             __SlateVaArgs::new(vec![
// LOWERING-NEXT:                 __SlateVaArg::new({{_v[0-9]+}}),
// LOWERING-NEXT:                 __SlateVaArg::new({{_v[0-9]+}}),
// LOWERING-NEXT:                 __SlateVaArg::new({{_v[0-9]+}}),
// LOWERING-NEXT:                 __SlateVaArg::new({{_v[0-9]+}}),
// LOWERING-NEXT:             ]),
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 4;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = c89_control_flow({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { c89_external_value };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { c89_const_global };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(c89_volatile_global)) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { c89_joined_value };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 13;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = unsigned_value;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = __slate_f80_to_i32({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = point.x;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = point.y;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = (bits.__bitfield_0.low() as u32) << 29 >> 29;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = (bits.__bitfield_0.high() as i32) << 28 >> 28;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { number.integer };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.add(0) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.add(3) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = copied_value;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 53;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 6;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { Some(c89_add).unwrap()({{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         printf(
// LOWERING-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
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
// REWRITES-NEXT: mod __slate_bitfields {
// REWRITES-NEXT:     #[bitfields::bitfield(
// REWRITES-NEXT:         u8,
// REWRITES-NEXT:         new = false,
// REWRITES-NEXT:         from_into_bits = false,
// REWRITES-NEXT:         from_traits = false,
// REWRITES-NEXT:         default = false,
// REWRITES-NEXT:         debug = false,
// REWRITES-NEXT:         builder = false,
// REWRITES-NEXT:         bit_ops = false
// REWRITES-NEXT:     )]
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
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct C89Bits {
// REWRITES-NEXT:     __bitfield_0: __slate_bitfields::__SlateBitfield_C89Bits_0,
// REWRITES-NEXT:     __bitfield_1: [u8; 3],
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: union C89Number {
// REWRITES-NEXT:     integer: i32,
// REWRITES-NEXT:     real: f64,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct C89Point {
// REWRITES-NEXT:     x: i32,
// REWRITES-NEXT:     y: i32,
// REWRITES-NEXT: }
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
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: struct __SlateVaArg {
// REWRITES-NEXT:     value: Box<dyn std::any::Any>,
// REWRITES-NEXT:     size: usize,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl __SlateVaArg {
// REWRITES-NEXT:     fn new<T: 'static>(value: T) -> Self {
// REWRITES-NEXT:         Self {
// REWRITES-NEXT:             value: Box::new(value),
// REWRITES-NEXT:             size: std::mem::size_of::<T>(),
// REWRITES-NEXT:         }
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
// REWRITES-NEXT:         Self {
// REWRITES-NEXT:             args: Some(std::rc::Rc::new(args)),
// REWRITES-NEXT:             index: 0,
// REWRITES-NEXT:         }
// REWRITES-NEXT:     }
// REWRITES-EMPTY:
// REWRITES-NEXT:     const fn empty() -> Self {
// REWRITES-NEXT:         Self {
// REWRITES-NEXT:             args: None,
// REWRITES-NEXT:             index: 0,
// REWRITES-NEXT:         }
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
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         *({{arg[0-9]+}} as *mut i32) = unsafe { *({{arg[0-9]+}} as *mut i32) };
// REWRITES-NEXT:     }
// REWRITES-NEXT:     return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: extern "C" fn c89_add({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) -> i32 {
// REWRITES-NEXT:     return {{arg[0-9]+}} + {{arg[0-9]+}};
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn c89_static_local() -> i32 {
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         c89_static_local_calls = (unsafe { c89_static_local_calls }) + 1;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     return unsafe { c89_static_local_calls };
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe fn c89_variadic_sum(mut count: i32, mut __slate_va_args: __SlateVaArgs) -> i32 {
// REWRITES-NEXT:     let mut arguments: __SlateVaArgs = __SlateVaArgs::empty();
// REWRITES-NEXT:     let mut index: i32 = 0;
// REWRITES-NEXT:     let mut total: i32 = 0;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         arguments = __slate_va_args.clone();
// REWRITES-NEXT:     }
// REWRITES-NEXT:     index = 0;
// REWRITES-NEXT:     loop {
// REWRITES-NEXT:         if !(index < count) {
// REWRITES-NEXT:             break;
// REWRITES-NEXT:         }
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = unsafe { arguments.next_arg::<i32>() };
// REWRITES-NEXT:         total += {{_v[0-9]+}};
// REWRITES-NEXT:         index += 1;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     return total;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn c89_control_flow({{arg[0-9]+}}: i32) -> i32 {
// REWRITES-NEXT:     let mut value: i32 = 0;
// REWRITES-NEXT:     let mut __retval: i32 = 0;
// REWRITES-NEXT:     let mut result: i32 = 0;
// REWRITES-NEXT:     let mut index: i32 = 0;
// REWRITES-NEXT:     let mut {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT:     let mut {{__state[0-9]+}}: i32 = 0;
// REWRITES-NEXT:     '{{__dispatch[0-9]+}}: loop {
// REWRITES-NEXT:         match {{__state[0-9]+}} {
// REWRITES-NEXT:             0 => {
// REWRITES-NEXT:                 value = {{arg[0-9]+}};
// REWRITES-NEXT:                 result = 0;
// REWRITES-NEXT:                 index = 0;
// REWRITES-NEXT:                 {{__state[0-9]+}} = 1;
// REWRITES-NEXT:                 continue '{{__dispatch[0-9]+}};
// REWRITES-NEXT:             }
// REWRITES-NEXT:             1 => {
// REWRITES-NEXT:                 {{__state[0-9]+}} = 2;
// REWRITES-NEXT:                 continue '{{__dispatch[0-9]+}};
// REWRITES-NEXT:             }
// REWRITES-NEXT:             2 => {
// REWRITES-NEXT:                 if index < value {
// REWRITES-NEXT:                     {{__state[0-9]+}} = 3;
// REWRITES-NEXT:                 } else {
// REWRITES-NEXT:                     {{__state[0-9]+}} = 14;
// REWRITES-NEXT:                 }
// REWRITES-NEXT:                 continue '{{__dispatch[0-9]+}};
// REWRITES-NEXT:             }
// REWRITES-NEXT:             3 => {
// REWRITES-NEXT:                 {{__state[0-9]+}} = 4;
// REWRITES-NEXT:                 continue '{{__dispatch[0-9]+}};
// REWRITES-NEXT:             }
// REWRITES-NEXT:             4 => {
// REWRITES-NEXT:                 {{__state[0-9]+}} = 5;
// REWRITES-NEXT:                 continue '{{__dispatch[0-9]+}};
// REWRITES-NEXT:             }
// REWRITES-NEXT:             5 => {
// REWRITES-NEXT:                 let {{_v[0-9]+}}: bool = index == 1;
// REWRITES-NEXT:                 if {{_v[0-9]+}} {
// REWRITES-NEXT:                     {{__state[0-9]+}} = 6;
// REWRITES-NEXT:                 } else {
// REWRITES-NEXT:                     {{__state[0-9]+}} = 7;
// REWRITES-NEXT:                 }
// REWRITES-NEXT:                 continue '{{__dispatch[0-9]+}};
// REWRITES-NEXT:             }
// REWRITES-NEXT:             6 => {
// REWRITES-NEXT:                 index += 1;
// REWRITES-NEXT:                 {{__state[0-9]+}} = 2;
// REWRITES-NEXT:                 continue '{{__dispatch[0-9]+}};
// REWRITES-NEXT:             }
// REWRITES-NEXT:             7 => {
// REWRITES-NEXT:                 {{__state[0-9]+}} = 8;
// REWRITES-NEXT:                 continue '{{__dispatch[0-9]+}};
// REWRITES-NEXT:             }
// REWRITES-NEXT:             8 => {
// REWRITES-NEXT:                 result += index;
// REWRITES-NEXT:                 {{__state[0-9]+}} = 9;
// REWRITES-NEXT:                 continue '{{__dispatch[0-9]+}};
// REWRITES-NEXT:             }
// REWRITES-NEXT:             9 => {
// REWRITES-NEXT:                 let {{_v[0-9]+}}: bool = result > 20;
// REWRITES-NEXT:                 if {{_v[0-9]+}} {
// REWRITES-NEXT:                     {{__state[0-9]+}} = 10;
// REWRITES-NEXT:                 } else {
// REWRITES-NEXT:                     {{__state[0-9]+}} = 11;
// REWRITES-NEXT:                 }
// REWRITES-NEXT:                 continue '{{__dispatch[0-9]+}};
// REWRITES-NEXT:             }
// REWRITES-NEXT:             10 => {
// REWRITES-NEXT:                 {{__state[0-9]+}} = 14;
// REWRITES-NEXT:                 continue '{{__dispatch[0-9]+}};
// REWRITES-NEXT:             }
// REWRITES-NEXT:             11 => {
// REWRITES-NEXT:                 {{__state[0-9]+}} = 12;
// REWRITES-NEXT:                 continue '{{__dispatch[0-9]+}};
// REWRITES-NEXT:             }
// REWRITES-NEXT:             12 => {
// REWRITES-NEXT:                 index += 1;
// REWRITES-NEXT:                 {{__state[0-9]+}} = 13;
// REWRITES-NEXT:                 continue '{{__dispatch[0-9]+}};
// REWRITES-NEXT:             }
// REWRITES-NEXT:             13 => {
// REWRITES-NEXT:                 {{__state[0-9]+}} = 2;
// REWRITES-NEXT:                 continue '{{__dispatch[0-9]+}};
// REWRITES-NEXT:             }
// REWRITES-NEXT:             14 => {
// REWRITES-NEXT:                 {{__state[0-9]+}} = 15;
// REWRITES-NEXT:                 continue '{{__dispatch[0-9]+}};
// REWRITES-NEXT:             }
// REWRITES-NEXT:             15 => {
// REWRITES-NEXT:                 {{__state[0-9]+}} = 16;
// REWRITES-NEXT:                 continue '{{__dispatch[0-9]+}};
// REWRITES-NEXT:             }
// REWRITES-NEXT:             16 => {
// REWRITES-NEXT:                 {{__state[0-9]+}} = 18;
// REWRITES-NEXT:                 continue '{{__dispatch[0-9]+}};
// REWRITES-NEXT:             }
// REWRITES-NEXT:             17 => {
// REWRITES-NEXT:                 let {{_v[0-9]+}}: bool = result > 6;
// REWRITES-NEXT:                 if {{_v[0-9]+}} {
// REWRITES-NEXT:                     {{__state[0-9]+}} = 18;
// REWRITES-NEXT:                 } else {
// REWRITES-NEXT:                     {{__state[0-9]+}} = 21;
// REWRITES-NEXT:                 }
// REWRITES-NEXT:                 continue '{{__dispatch[0-9]+}};
// REWRITES-NEXT:             }
// REWRITES-NEXT:             18 => {
// REWRITES-NEXT:                 {{__state[0-9]+}} = 19;
// REWRITES-NEXT:                 continue '{{__dispatch[0-9]+}};
// REWRITES-NEXT:             }
// REWRITES-NEXT:             19 => {
// REWRITES-NEXT:                 result -= 1;
// REWRITES-NEXT:                 {{__state[0-9]+}} = 20;
// REWRITES-NEXT:                 continue '{{__dispatch[0-9]+}};
// REWRITES-NEXT:             }
// REWRITES-NEXT:             20 => {
// REWRITES-NEXT:                 {{__state[0-9]+}} = 17;
// REWRITES-NEXT:                 continue '{{__dispatch[0-9]+}};
// REWRITES-NEXT:             }
// REWRITES-NEXT:             21 => {
// REWRITES-NEXT:                 {{__state[0-9]+}} = 22;
// REWRITES-NEXT:                 continue '{{__dispatch[0-9]+}};
// REWRITES-NEXT:             }
// REWRITES-NEXT:             22 => {
// REWRITES-NEXT:                 {{__state[0-9]+}} = 23;
// REWRITES-NEXT:                 continue '{{__dispatch[0-9]+}};
// REWRITES-NEXT:             }
// REWRITES-NEXT:             23 => {
// REWRITES-NEXT:                 {{_v[0-9]+}} = value;
// REWRITES-NEXT:                 {{__state[0-9]+}} = 24;
// REWRITES-NEXT:                 continue '{{__dispatch[0-9]+}};
// REWRITES-NEXT:             }
// REWRITES-NEXT:             24 => {
// REWRITES-NEXT:                 match {{_v[0-9]+}} {
// REWRITES-NEXT:                     4 => {
// REWRITES-NEXT:                         {{__state[0-9]+}} = 26;
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     _ => {
// REWRITES-NEXT:                         {{__state[0-9]+}} = 27;
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                 }
// REWRITES-NEXT:                 continue '{{__dispatch[0-9]+}};
// REWRITES-NEXT:             }
// REWRITES-NEXT:             25 => {
// REWRITES-NEXT:                 {{__state[0-9]+}} = 26;
// REWRITES-NEXT:                 continue '{{__dispatch[0-9]+}};
// REWRITES-NEXT:             }
// REWRITES-NEXT:             26 => {
// REWRITES-NEXT:                 result += 10;
// REWRITES-NEXT:                 {{__state[0-9]+}} = 28;
// REWRITES-NEXT:                 continue '{{__dispatch[0-9]+}};
// REWRITES-NEXT:             }
// REWRITES-NEXT:             27 => {
// REWRITES-NEXT:                 result = 0;
// REWRITES-NEXT:                 {{__state[0-9]+}} = 28;
// REWRITES-NEXT:                 continue '{{__dispatch[0-9]+}};
// REWRITES-NEXT:             }
// REWRITES-NEXT:             28 => {
// REWRITES-NEXT:                 {{__state[0-9]+}} = 29;
// REWRITES-NEXT:                 continue '{{__dispatch[0-9]+}};
// REWRITES-NEXT:             }
// REWRITES-NEXT:             29 => {
// REWRITES-NEXT:                 {{__state[0-9]+}} = 30;
// REWRITES-NEXT:                 continue '{{__dispatch[0-9]+}};
// REWRITES-NEXT:             }
// REWRITES-NEXT:             30 => {
// REWRITES-NEXT:                 let {{_v[0-9]+}}: bool = result == 16;
// REWRITES-NEXT:                 if {{_v[0-9]+}} {
// REWRITES-NEXT:                     {{__state[0-9]+}} = 31;
// REWRITES-NEXT:                 } else {
// REWRITES-NEXT:                     {{__state[0-9]+}} = 32;
// REWRITES-NEXT:                 }
// REWRITES-NEXT:                 continue '{{__dispatch[0-9]+}};
// REWRITES-NEXT:             }
// REWRITES-NEXT:             31 => {
// REWRITES-NEXT:                 {{__state[0-9]+}} = 34;
// REWRITES-NEXT:                 continue '{{__dispatch[0-9]+}};
// REWRITES-NEXT:             }
// REWRITES-NEXT:             32 => {
// REWRITES-NEXT:                 {{__state[0-9]+}} = 33;
// REWRITES-NEXT:                 continue '{{__dispatch[0-9]+}};
// REWRITES-NEXT:             }
// REWRITES-NEXT:             33 => {
// REWRITES-NEXT:                 result = -1;
// REWRITES-NEXT:                 {{__state[0-9]+}} = 34;
// REWRITES-NEXT:                 continue '{{__dispatch[0-9]+}};
// REWRITES-NEXT:             }
// REWRITES-NEXT:             34 => {
// REWRITES-NEXT:                 __retval = result;
// REWRITES-NEXT:                 return __retval;
// REWRITES-NEXT:             }
// REWRITES-NEXT:             _ => {
// REWRITES-NEXT:                 unreachable!();
// REWRITES-NEXT:             }
// REWRITES-NEXT:         }
// REWRITES-NEXT:     }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let mut unsigned_value: u64 = 0;
// REWRITES-NEXT:     let mut point: C89Point = C89Point { x: 0, y: 0 };
// REWRITES-NEXT:     let mut bits: C89Bits = C89Bits {
// REWRITES-NEXT:         __bitfield_0: unsafe {
// REWRITES-NEXT:             std::mem::transmute::<u8, __slate_bitfields::__SlateBitfield_C89Bits_0>(0)
// REWRITES-NEXT:         },
// REWRITES-NEXT:         __bitfield_1: [0; 3],
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let mut number: C89Number = unsafe { std::mem::zeroed::<C89Number>() };
// REWRITES-NEXT:     let mut array: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// REWRITES-NEXT:     let mut copied_value: i32 = 0;
// REWRITES-NEXT:     let mut source_value: i32 = 0;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = 2;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = 3;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = -5;
// REWRITES-NEXT:     unsigned_value = 29;
// REWRITES-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 144, 1, 64]);
// REWRITES-NEXT:     let {{_v[0-9]+}}: u32 = C89Color::C89_GREEN as u32;
// REWRITES-NEXT:     point.x = 31;
// REWRITES-NEXT:     point.y = 37;
// REWRITES-NEXT:     bits.__bitfield_0.set_low((6 as u32) << 29 >> 29);
// REWRITES-NEXT:     bits.__bitfield_0.set_high((-3 as i32) << 28 >> 28);
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         number.integer = 41;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 0;
// REWRITES-NEXT:     array[({{_v[0-9]+}} as usize)] = 1;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 1;
// REWRITES-NEXT:     array[({{_v[0-9]+}} as usize)] = 2;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// REWRITES-NEXT:     array[({{_v[0-9]+}} as usize)] = 3;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// REWRITES-NEXT:     array[({{_v[0-9]+}} as usize)] = 4;
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i32 = array.as_mut_ptr() as *mut i32;
// REWRITES-NEXT:     source_value = 43;
// REWRITES-NEXT:     copied_value = 0;
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void =
// REWRITES-NEXT:         std::ptr::addr_of_mut!(copied_value) as *mut core::ffi::c_void;
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void =
// REWRITES-NEXT:         std::ptr::addr_of_mut!(source_value) as *mut core::ffi::c_void;
// REWRITES-NEXT:     c89_store(
// REWRITES-NEXT:         {{_v[0-9]+}} as *mut core::ffi::c_void,
// REWRITES-NEXT:         {{_v[0-9]+}} as *mut core::ffi::c_void,
// REWRITES-NEXT:     );
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = 3;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = 17;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = 3;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = (({{_v[0-9]+}} + {{_v[0-9]+}}) * 4 - {{_v[0-9]+}}) / {{_v[0-9]+}} % {{_v[0-9]+}};
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = 5;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = 2;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = 31;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = (({{_v[0-9]+}} << {{_v[0-9]+}} | 3) ^ {{_v[0-9]+}}) & {{_v[0-9]+}};
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < 0;
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = unsigned_value > 0;
// REWRITES-NEXT:         {{_v[0-9]+}}
// REWRITES-NEXT:     } else {
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = false;
// REWRITES-NEXT:         {{_v[0-9]+}}
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = true;
// REWRITES-NEXT:         {{_v[0-9]+}}
// REWRITES-NEXT:     } else {
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// REWRITES-NEXT:         {{_v[0-9]+}}
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = 47;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} { {{_v[0-9]+}} } else { {{_v[0-9]+}} };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = 49;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1 + {{_v[0-9]+}};
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = 3 - {{_v[0-9]+}};
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = c89_static_local();
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} * 10;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = c89_static_local();
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = unsafe {
// REWRITES-NEXT:         c89_variadic_sum(
// REWRITES-NEXT:             4,
// REWRITES-NEXT:             __SlateVaArgs::new(vec![
// REWRITES-NEXT:                 __SlateVaArg::new(2 as i32),
// REWRITES-NEXT:                 __SlateVaArg::new(3 as i32),
// REWRITES-NEXT:                 __SlateVaArg::new(5 as i32),
// REWRITES-NEXT:                 __SlateVaArg::new(7 as i32),
// REWRITES-NEXT:             ]),
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = c89_control_flow(4);
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 =
// REWRITES-NEXT:         c"%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n"
// REWRITES-NEXT:             .as_ptr() as *mut i8;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { c89_external_value };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { c89_const_global };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(c89_volatile_global)) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { c89_joined_value };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = unsigned_value as i32;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = __slate_f80_to_i32({{_v[0-9]+}});
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = point.x + point.y;
// REWRITES-NEXT:     let {{_v[0-9]+}}: u32 = (bits.__bitfield_0.low() as u32) << 29 >> 29;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = (bits.__bitfield_0.high() as i32) << 28 >> 28;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { number.integer };
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.add(0) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { *{{_v[0-9]+}} };
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.add(3) };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_char,
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:             13 as i32,
// REWRITES-NEXT:             1 as i32,
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:             (-7 as i8) as i32,
// REWRITES-NEXT:             (2.5 as f32) as i32,
// REWRITES-NEXT:             (3.5 as f64) as i32,
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:             90 as i32,
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:             {{_v[0-9]+}} + unsafe { *{{_v[0-9]+}} },
// REWRITES-NEXT:             copied_value,
// REWRITES-NEXT:             unsafe { Some(c89_add).unwrap()(53 as i32, 6 as i32) },
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:             {{_v[0-9]+}} + {{_v[0-9]+}},
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     std::process::exit(0 as i32);
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
