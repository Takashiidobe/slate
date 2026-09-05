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
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT:                 } else {
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: mod __slate_bitfields {
// COMMON-LOWERING-NEXT:     #[bitfields::bitfield(
// COMMON-LOWERING-NEXT:         u8,
// COMMON-LOWERING-NEXT:         new = false,
// COMMON-LOWERING-NEXT:         from_into_bits = false,
// COMMON-LOWERING-NEXT:         from_traits = false,
// COMMON-LOWERING-NEXT:         default = false,
// COMMON-LOWERING-NEXT:         debug = false,
// COMMON-LOWERING-NEXT:         builder = false,
// COMMON-LOWERING-NEXT:         bit_ops = false
// COMMON-LOWERING-NEXT:     )]
// COMMON-LOWERING-NEXT:     pub struct __SlateBitfield_C89Bits_0 {
// COMMON-LOWERING-NEXT:         #[bits(3)]
// COMMON-LOWERING-NEXT:         pub low: u32,
// COMMON-LOWERING-NEXT:         #[bits(4)]
// COMMON-LOWERING-NEXT:         pub high: i32,
// COMMON-LOWERING-NEXT:         #[bits(1)]
// COMMON-LOWERING-NEXT:         pub _reserved_0: u128,
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[allow(non_camel_case_types)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy, PartialEq, Eq, Debug, Hash)]
// COMMON-LOWERING-NEXT: enum C89Color {
// COMMON-LOWERING-NEXT:     C89_RED = 3,
// COMMON-LOWERING-NEXT:     C89_GREEN = 5,
// COMMON-LOWERING-NEXT:     C89_BLUE = 7,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct C89Bits {
// COMMON-LOWERING-NEXT:     __bitfield_0: __slate_bitfields::__SlateBitfield_C89Bits_0,
// COMMON-LOWERING-NEXT:     __bitfield_1: [u8; 3],
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: union C89Number {
// COMMON-LOWERING-NEXT:     integer: i32,
// COMMON-LOWERING-NEXT:     real: f64,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct C89Point {
// COMMON-LOWERING-NEXT:     x: i32,
// COMMON-LOWERING-NEXT:     y: i32,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: static mut c89_const_global: i32 = 17;
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: static mut c89_external_value: i32 = 11;
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: static mut c89_joined_value: i32 = 23;
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: static mut c89_static_local_calls: i32 = 0;
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: static mut c89_volatile_global: i32 = 19;
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: struct __SlateVaArg {
// COMMON-LOWERING-NEXT:     value: Box<dyn std::any::Any>,
// COMMON-LOWERING-NEXT:     size: usize,
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: impl __SlateVaArg {
// COMMON-LOWERING-NEXT:     fn new<T: 'static>(value: T) -> Self {
// COMMON-LOWERING-NEXT:         Self {
// COMMON-LOWERING-NEXT:             value: Box::new(value),
// COMMON-LOWERING-NEXT:             size: std::mem::size_of::<T>(),
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     fn read<T: Copy + 'static>(&self) -> T {
// COMMON-LOWERING-NEXT:         if let Some(value) = self.value.downcast_ref::<T>() {
// COMMON-LOWERING-NEXT:             return *value;
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:         assert!(self.size >= std::mem::size_of::<T>());
// COMMON-LOWERING-NEXT:         unsafe {
// COMMON-LOWERING-NEXT:             std::ptr::read_unaligned(
// COMMON-LOWERING-NEXT:                 (self.value.as_ref() as *const dyn std::any::Any) as *const () as *const T,
// COMMON-LOWERING-NEXT:             )
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT: #[derive(Clone)]
// COMMON-LOWERING-NEXT: struct __SlateVaArgs {
// COMMON-LOWERING-NEXT:     args: Option<std::rc::Rc<Vec<__SlateVaArg>>>,
// COMMON-LOWERING-NEXT:     index: usize,
// COMMON-LOWERING-NEXT: impl __SlateVaArgs {
// COMMON-LOWERING-NEXT:     fn new(args: Vec<__SlateVaArg>) -> Self {
// COMMON-LOWERING-NEXT:         Self {
// COMMON-LOWERING-NEXT:             args: Some(std::rc::Rc::new(args)),
// COMMON-LOWERING-NEXT:             index: 0,
// COMMON-LOWERING-NEXT:     const fn empty() -> Self {
// COMMON-LOWERING-NEXT:         Self {
// COMMON-LOWERING-NEXT:             args: None,
// COMMON-LOWERING-NEXT:             index: 0,
// COMMON-LOWERING-NEXT:     fn next_arg<T: Copy + 'static>(&mut self) -> T {
// COMMON-LOWERING-NEXT:         let index = self.index;
// COMMON-LOWERING-NEXT:         self.index += 1;
// COMMON-LOWERING-NEXT:         if std::mem::size_of::<T>() == 0 {
// COMMON-LOWERING-NEXT:             return unsafe { std::mem::zeroed() };
// COMMON-LOWERING-NEXT:         let args = self.args.as_ref().expect("va_arg with no arguments");
// COMMON-LOWERING-NEXT:         args[index].read::<T>()
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let mut unsigned_value: u64 = 0;
// COMMON-LOWERING-NEXT:     let mut point: C89Point = C89Point { x: 0, y: 0 };
// COMMON-LOWERING-NEXT:     let mut bits: C89Bits = C89Bits {
// COMMON-LOWERING-NEXT:         __bitfield_0: unsafe {
// COMMON-LOWERING-NEXT:             std::mem::transmute::<u8, __slate_bitfields::__SlateBitfield_C89Bits_0>(0)
// COMMON-LOWERING-NEXT:         },
// COMMON-LOWERING-NEXT:         __bitfield_1: [0; 3],
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let mut number: C89Number = unsafe { std::mem::zeroed::<C89Number>() };
// COMMON-LOWERING-NEXT:     let mut copied_value: i32 = 0;
// COMMON-LOWERING-NEXT:     let mut source_value: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 2;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 3;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = -5;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 29;
// COMMON-LOWERING-NEXT:     unsigned_value = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i8 = -7;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = 2.5;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 3.5;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = C89Color::C89_GREEN as u32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 31;
// COMMON-LOWERING-NEXT:     point.x = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 37;
// COMMON-LOWERING-NEXT:     point.y = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = 6;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = ({{__v[0-9]+}} as u32) << 29 >> 29;
// COMMON-LOWERING-NEXT:     bits.__bitfield_0.set_low(({{__v[0-9]+}} as u32) << 29 >> 29);
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = -3;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = ({{__v[0-9]+}} as i32) << 28 >> 28;
// COMMON-LOWERING-NEXT:     bits.__bitfield_0.set_high(({{__v[0-9]+}} as i32) << 28 >> 28);
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 41;
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         number.integer = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// COMMON-LOWERING-NEXT:     array[({{__v[0-9]+}} as usize)] = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 2;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 1;
// COMMON-LOWERING-NEXT:     array[({{__v[0-9]+}} as usize)] = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 3;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 2;
// COMMON-LOWERING-NEXT:     array[({{__v[0-9]+}} as usize)] = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 4;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 3;
// COMMON-LOWERING-NEXT:     array[({{__v[0-9]+}} as usize)] = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = array.as_mut_ptr() as *mut i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 43;
// COMMON-LOWERING-NEXT:     source_value = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     copied_value = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void =
// COMMON-LOWERING-NEXT:         std::ptr::addr_of_mut!(copied_value) as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void =
// COMMON-LOWERING-NEXT:         std::ptr::addr_of_mut!(source_value) as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     c89_store(
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}} as *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}} as *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:     );
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 4;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} * {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 3;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} - {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 17;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} / {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 3;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} % {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 5;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} << {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 3;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} | {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 2;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} ^ {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 31;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} & {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: u64 = unsigned_value;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: u64 = 0;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} > {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = true;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 47;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 49;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 3;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} - {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = c89_static_local();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 10;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} * {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = c89_static_local();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 4;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 2;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 3;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 5;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 7;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         c89_variadic_sum(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             __SlateVaArgs::new(vec![
// COMMON-LOWERING-NEXT:                 __SlateVaArg::new({{__v[0-9]+}}),
// COMMON-LOWERING-NEXT:                 __SlateVaArg::new({{__v[0-9]+}}),
// COMMON-LOWERING-NEXT:                 __SlateVaArg::new({{__v[0-9]+}}),
// COMMON-LOWERING-NEXT:                 __SlateVaArg::new({{__v[0-9]+}}),
// COMMON-LOWERING-NEXT:             ]),
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 4;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = c89_control_flow({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { c89_external_value };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { c89_const_global };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(c89_volatile_global)) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { c89_joined_value };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 13;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = unsigned_value;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = point.x;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = point.y;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = (bits.__bitfield_0.low() as u32) << 29 >> 29;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = (bits.__bitfield_0.high() as i32) << 28 >> 28;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { number.integer };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(0) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 3;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(3) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = copied_value;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 53;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 6;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { Some(c89_add).unwrap()({{__v[0-9]+}}, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         printf(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: fn c89_store({{arg[0-9]+}}: *mut core::ffi::c_void, {{arg[0-9]+}}: *mut core::ffi::c_void) {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = {{arg[0-9]+}} as *mut i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = {{arg[0-9]+}} as *mut i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     return;
// COMMON-LOWERING-NEXT: extern "C-unwind" fn c89_add({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) -> i32 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{arg[0-9]+}} + {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: fn c89_static_local() -> i32 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { c89_static_local_calls };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         c89_static_local_calls = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { c89_static_local_calls };
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: unsafe fn c89_variadic_sum({{arg[0-9]+}}: i32, mut __slate_va_args: __SlateVaArgs) -> i32 {
// COMMON-LOWERING-NEXT:     let mut count: i32 = 0;
// COMMON-LOWERING-NEXT:     let mut arguments: __SlateVaArgs = __SlateVaArgs::empty();
// COMMON-LOWERING-NEXT:     let mut index: i32 = 0;
// COMMON-LOWERING-NEXT:     let mut total: i32 = 0;
// COMMON-LOWERING-NEXT:     count = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     total = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         arguments = __slate_va_args.clone();
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:         index = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = index;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = count;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = unsafe { arguments.next_arg::<i32>() };
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = total;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                 total = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = index;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// COMMON-LOWERING-NEXT:             index = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = total;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: fn c89_control_flow({{arg[0-9]+}}: i32) -> i32 {
// COMMON-LOWERING-NEXT:     let mut value: i32 = 0;
// COMMON-LOWERING-NEXT:     let mut __retval: i32 = 0;
// COMMON-LOWERING-NEXT:     let mut result: i32 = 0;
// COMMON-LOWERING-NEXT:     let mut index: i32 = 0;
// COMMON-LOWERING-NEXT:     let mut {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let mut {{__state[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     '{{__dispatch[0-9]+}}: loop {
// COMMON-LOWERING-NEXT:         match {{__state[0-9]+}} {
// COMMON-LOWERING-NEXT:             0 => {
// COMMON-LOWERING-NEXT:                 value = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:                 result = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:                 index = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                 {{__state[0-9]+}} = 1;
// COMMON-LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             1 => {
// COMMON-LOWERING-NEXT:                 {{__state[0-9]+}} = 2;
// COMMON-LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             2 => {
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = index;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = value;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                 if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                     {{__state[0-9]+}} = 3;
// COMMON-LOWERING-NEXT:                 } else {
// COMMON-LOWERING-NEXT:                     {{__state[0-9]+}} = 14;
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             3 => {
// COMMON-LOWERING-NEXT:                 {{__state[0-9]+}} = 4;
// COMMON-LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             4 => {
// COMMON-LOWERING-NEXT:                 {{__state[0-9]+}} = 5;
// COMMON-LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             5 => {
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = index;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                 if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                     {{__state[0-9]+}} = 6;
// COMMON-LOWERING-NEXT:                 } else {
// COMMON-LOWERING-NEXT:                     {{__state[0-9]+}} = 7;
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             6 => {
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = index;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// COMMON-LOWERING-NEXT:                 index = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                 {{__state[0-9]+}} = 2;
// COMMON-LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             7 => {
// COMMON-LOWERING-NEXT:                 {{__state[0-9]+}} = 8;
// COMMON-LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             8 => {
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = index;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = result;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                 result = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                 {{__state[0-9]+}} = 9;
// COMMON-LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             9 => {
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = result;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = 20;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: bool = {{__v[0-9]+}} > {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                 if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                     {{__state[0-9]+}} = 10;
// COMMON-LOWERING-NEXT:                 } else {
// COMMON-LOWERING-NEXT:                     {{__state[0-9]+}} = 11;
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             10 => {
// COMMON-LOWERING-NEXT:                 {{__state[0-9]+}} = 14;
// COMMON-LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             11 => {
// COMMON-LOWERING-NEXT:                 {{__state[0-9]+}} = 12;
// COMMON-LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             12 => {
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = index;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// COMMON-LOWERING-NEXT:                 index = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                 {{__state[0-9]+}} = 13;
// COMMON-LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             13 => {
// COMMON-LOWERING-NEXT:                 {{__state[0-9]+}} = 2;
// COMMON-LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             14 => {
// COMMON-LOWERING-NEXT:                 {{__state[0-9]+}} = 15;
// COMMON-LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             15 => {
// COMMON-LOWERING-NEXT:                 {{__state[0-9]+}} = 16;
// COMMON-LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             16 => {
// COMMON-LOWERING-NEXT:                 {{__state[0-9]+}} = 18;
// COMMON-LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             17 => {
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = result;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = 6;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: bool = {{__v[0-9]+}} > {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                 if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                     {{__state[0-9]+}} = 18;
// COMMON-LOWERING-NEXT:                 } else {
// COMMON-LOWERING-NEXT:                     {{__state[0-9]+}} = 21;
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             18 => {
// COMMON-LOWERING-NEXT:                 {{__state[0-9]+}} = 19;
// COMMON-LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             19 => {
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = result;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} - 1;
// COMMON-LOWERING-NEXT:                 result = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                 {{__state[0-9]+}} = 20;
// COMMON-LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             20 => {
// COMMON-LOWERING-NEXT:                 {{__state[0-9]+}} = 17;
// COMMON-LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             21 => {
// COMMON-LOWERING-NEXT:                 {{__state[0-9]+}} = 22;
// COMMON-LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             22 => {
// COMMON-LOWERING-NEXT:                 {{__state[0-9]+}} = 23;
// COMMON-LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             23 => {
// COMMON-LOWERING-NEXT:                 {{__v[0-9]+}} = value;
// COMMON-LOWERING-NEXT:                 {{__state[0-9]+}} = 24;
// COMMON-LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             24 => {
// COMMON-LOWERING-NEXT:                 match {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                     4 => {
// COMMON-LOWERING-NEXT:                         {{__state[0-9]+}} = 26;
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                     _ => {
// COMMON-LOWERING-NEXT:                         {{__state[0-9]+}} = 27;
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             25 => {
// COMMON-LOWERING-NEXT:                 {{__state[0-9]+}} = 26;
// COMMON-LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             26 => {
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = 10;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = result;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                 result = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                 {{__state[0-9]+}} = 28;
// COMMON-LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             27 => {
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:                 result = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                 {{__state[0-9]+}} = 28;
// COMMON-LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             28 => {
// COMMON-LOWERING-NEXT:                 {{__state[0-9]+}} = 29;
// COMMON-LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             29 => {
// COMMON-LOWERING-NEXT:                 {{__state[0-9]+}} = 30;
// COMMON-LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             30 => {
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = result;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = 16;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                 if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                     {{__state[0-9]+}} = 31;
// COMMON-LOWERING-NEXT:                     {{__state[0-9]+}} = 32;
// COMMON-LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             31 => {
// COMMON-LOWERING-NEXT:                 {{__state[0-9]+}} = 34;
// COMMON-LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             32 => {
// COMMON-LOWERING-NEXT:                 {{__state[0-9]+}} = 33;
// COMMON-LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             33 => {
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = -1;
// COMMON-LOWERING-NEXT:                 result = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                 {{__state[0-9]+}} = 34;
// COMMON-LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             34 => {
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = result;
// COMMON-LOWERING-NEXT:                 __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:                 return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             _ => {
// COMMON-LOWERING-NEXT:                 unreachable!();
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
// LOWERING-X86_64-GNU-NEXT:         {
// LOWERING-X86_64-GNU-NEXT:             *self = __slate_f80_add(*self, __o);
// LOWERING-X86_64-GNU-NEXT: impl core::ops::SubAssign for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     fn sub_assign(&mut self, __o: LongDouble) {
// LOWERING-X86_64-GNU-NEXT:         {
// LOWERING-X86_64-GNU-NEXT:             *self = __slate_f80_sub(*self, __o);
// LOWERING-X86_64-GNU-NEXT: impl core::ops::MulAssign for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     fn mul_assign(&mut self, __o: LongDouble) {
// LOWERING-X86_64-GNU-NEXT:         {
// LOWERING-X86_64-GNU-NEXT:             *self = __slate_f80_mul(*self, __o);
// LOWERING-X86_64-GNU-NEXT: impl core::ops::DivAssign for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     fn div_assign(&mut self, __o: LongDouble) {
// LOWERING-X86_64-GNU-NEXT:         {
// LOWERING-X86_64-GNU-NEXT:             *self = __slate_f80_div(*self, __o);
// LOWERING-X86_64-GNU-NEXT: impl core::ops::Neg for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     type Output = LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn neg(self) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:         __slate_f80_neg(self)
// LOWERING-X86_64-GNU-NEXT: impl core::cmp::PartialEq for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     fn eq(&self, __other: &LongDouble) -> bool {
// LOWERING-X86_64-GNU-NEXT:         __slate_f80_eq(*self, *__other)
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
// LOWERING-X86_64-GNU-NEXT:                     None
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT:     let mut array: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 144, 1, 64]);
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = 90;
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = __slate_f80_to_i32({{__v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT:                 } else {
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
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
// LOWERING-AARCH64-GNU-NEXT: #[repr(C)]
// LOWERING-AARCH64-GNU-NEXT: struct __va_list {
// LOWERING-AARCH64-GNU-NEXT:     __slate_empty: [u8; 0],
// LOWERING-AARCH64-GNU-NEXT:     let mut array: [i32; 4] = [0; 4];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 4.500000e+00f128;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u32 = 90;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
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
// COMMON-REWRITES-NEXT:         } else {
// COMMON-REWRITES-NEXT:             } else {
// COMMON-REWRITES-NEXT:                 } else {
// COMMON-REWRITES-NEXT:                 }
// COMMON-REWRITES-NEXT:             }
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: mod __slate_bitfields {
// COMMON-REWRITES-NEXT:     #[bitfields::bitfield(
// COMMON-REWRITES-NEXT:         u8,
// COMMON-REWRITES-NEXT:         new = false,
// COMMON-REWRITES-NEXT:         from_into_bits = false,
// COMMON-REWRITES-NEXT:         from_traits = false,
// COMMON-REWRITES-NEXT:         default = false,
// COMMON-REWRITES-NEXT:         debug = false,
// COMMON-REWRITES-NEXT:         builder = false,
// COMMON-REWRITES-NEXT:         bit_ops = false
// COMMON-REWRITES-NEXT:     )]
// COMMON-REWRITES-NEXT:     pub struct __SlateBitfield_C89Bits_0 {
// COMMON-REWRITES-NEXT:         #[bits(3)]
// COMMON-REWRITES-NEXT:         pub low: u32,
// COMMON-REWRITES-NEXT:         #[bits(4)]
// COMMON-REWRITES-NEXT:         pub high: i32,
// COMMON-REWRITES-NEXT:         #[bits(1)]
// COMMON-REWRITES-NEXT:         pub _reserved_0: u128,
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[allow(non_camel_case_types)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy, PartialEq, Eq, Debug, Hash)]
// COMMON-REWRITES-NEXT: enum C89Color {
// COMMON-REWRITES-NEXT:     C89_RED = 3,
// COMMON-REWRITES-NEXT:     C89_GREEN = 5,
// COMMON-REWRITES-NEXT:     C89_BLUE = 7,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct C89Bits {
// COMMON-REWRITES-NEXT:     __bitfield_0: __slate_bitfields::__SlateBitfield_C89Bits_0,
// COMMON-REWRITES-NEXT:     __bitfield_1: [u8; 3],
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: union C89Number {
// COMMON-REWRITES-NEXT:     integer: i32,
// COMMON-REWRITES-NEXT:     real: f64,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct C89Point {
// COMMON-REWRITES-NEXT:     x: i32,
// COMMON-REWRITES-NEXT:     y: i32,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: static mut c89_const_global: i32 = 17;
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: static mut c89_external_value: i32 = 11;
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: static mut c89_joined_value: i32 = 23;
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: static mut c89_static_local_calls: i32 = 0;
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: static mut c89_volatile_global: i32 = 19;
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: struct __SlateVaArg {
// COMMON-REWRITES-NEXT:     value: Box<dyn std::any::Any>,
// COMMON-REWRITES-NEXT:     size: usize,
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: impl __SlateVaArg {
// COMMON-REWRITES-NEXT:     fn new<T: 'static>(value: T) -> Self {
// COMMON-REWRITES-NEXT:         Self {
// COMMON-REWRITES-NEXT:             value: Box::new(value),
// COMMON-REWRITES-NEXT:             size: std::mem::size_of::<T>(),
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     fn read<T: Copy + 'static>(&self) -> T {
// COMMON-REWRITES-NEXT:         if let Some(value) = self.value.downcast_ref::<T>() {
// COMMON-REWRITES-NEXT:             return *value;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         assert!(self.size >= std::mem::size_of::<T>());
// COMMON-REWRITES-NEXT:         unsafe {
// COMMON-REWRITES-NEXT:             std::ptr::read_unaligned(
// COMMON-REWRITES-NEXT:                 (self.value.as_ref() as *const dyn std::any::Any) as *const () as *const T,
// COMMON-REWRITES-NEXT:             )
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT: #[derive(Clone)]
// COMMON-REWRITES-NEXT: struct __SlateVaArgs {
// COMMON-REWRITES-NEXT:     args: Option<std::rc::Rc<Vec<__SlateVaArg>>>,
// COMMON-REWRITES-NEXT:     index: usize,
// COMMON-REWRITES-NEXT: impl __SlateVaArgs {
// COMMON-REWRITES-NEXT:     fn new(args: Vec<__SlateVaArg>) -> Self {
// COMMON-REWRITES-NEXT:         Self {
// COMMON-REWRITES-NEXT:             args: Some(std::rc::Rc::new(args)),
// COMMON-REWRITES-NEXT:             index: 0,
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     const fn empty() -> Self {
// COMMON-REWRITES-NEXT:         Self {
// COMMON-REWRITES-NEXT:             args: None,
// COMMON-REWRITES-NEXT:             index: 0,
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     fn next_arg<T: Copy + 'static>(&mut self) -> T {
// COMMON-REWRITES-NEXT:         let index = self.index;
// COMMON-REWRITES-NEXT:         self.index += 1;
// COMMON-REWRITES-NEXT:         if std::mem::size_of::<T>() == 0 {
// COMMON-REWRITES-NEXT:             return unsafe { std::mem::zeroed() };
// COMMON-REWRITES-NEXT:         let args = self.args.as_ref().expect("va_arg with no arguments");
// COMMON-REWRITES-NEXT:         args[index].read::<T>()
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     let mut unsigned_value: u64 = 0;
// COMMON-REWRITES-NEXT:     let mut point: C89Point = C89Point { x: 0, y: 0 };
// COMMON-REWRITES-NEXT:     let mut bits: C89Bits = C89Bits {
// COMMON-REWRITES-NEXT:         __bitfield_0: unsafe {
// COMMON-REWRITES-NEXT:             std::mem::transmute::<u8, __slate_bitfields::__SlateBitfield_C89Bits_0>(0)
// COMMON-REWRITES-NEXT:         },
// COMMON-REWRITES-NEXT:         __bitfield_1: [0; 3],
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let mut number: C89Number = unsafe { std::mem::zeroed::<C89Number>() };
// COMMON-REWRITES-NEXT:     let mut copied_value: i32 = 0;
// COMMON-REWRITES-NEXT:     let mut source_value: i32 = 0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 2;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 3;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = -5;
// COMMON-REWRITES-NEXT:     unsigned_value = 29;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u32 = C89Color::C89_GREEN as u32;
// COMMON-REWRITES-NEXT:     point.x = 31;
// COMMON-REWRITES-NEXT:     point.y = 37;
// COMMON-REWRITES-NEXT:     bits.__bitfield_0.set_low((6 as u32) << 29 >> 29);
// COMMON-REWRITES-NEXT:     bits.__bitfield_0.set_high((-3 as i32) << 28 >> 28);
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         number.integer = 41;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// COMMON-REWRITES-NEXT:     array[({{__v[0-9]+}} as usize)] = 1;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = 1;
// COMMON-REWRITES-NEXT:     array[({{__v[0-9]+}} as usize)] = 2;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = 2;
// COMMON-REWRITES-NEXT:     array[({{__v[0-9]+}} as usize)] = 3;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = 3;
// COMMON-REWRITES-NEXT:     array[({{__v[0-9]+}} as usize)] = 4;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i32 = array.as_mut_ptr() as *mut i32;
// COMMON-REWRITES-NEXT:     source_value = 43;
// COMMON-REWRITES-NEXT:     copied_value = 0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void =
// COMMON-REWRITES-NEXT:         std::ptr::addr_of_mut!(copied_value) as *mut core::ffi::c_void;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void =
// COMMON-REWRITES-NEXT:         std::ptr::addr_of_mut!(source_value) as *mut core::ffi::c_void;
// COMMON-REWRITES-NEXT:     c89_store(
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}} as *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}} as *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:     );
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = (({{__v[0-9]+}} + {{__v[0-9]+}}) * 4 - 3) / 17 % 3;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 5;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = (({{__v[0-9]+}} << {{__v[0-9]+}} | 3) ^ 2) & 31;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} < 0 {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = unsigned_value > 0;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = true;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 47;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} != 0 { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1 + 49;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 3 - {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = c89_static_local();
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} * 10;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = c89_static_local();
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-REWRITES-NEXT:         c89_variadic_sum(
// COMMON-REWRITES-NEXT:             4,
// COMMON-REWRITES-NEXT:             __SlateVaArgs::new(vec![
// COMMON-REWRITES-NEXT:                 __SlateVaArg::new(2 as i32),
// COMMON-REWRITES-NEXT:                 __SlateVaArg::new(3 as i32),
// COMMON-REWRITES-NEXT:                 __SlateVaArg::new(5 as i32),
// COMMON-REWRITES-NEXT:                 __SlateVaArg::new(7 as i32),
// COMMON-REWRITES-NEXT:             ]),
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = c89_control_flow(4);
// COMMON-REWRITES-NEXT:         c"%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n"
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { c89_external_value };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { c89_const_global };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(c89_volatile_global)) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { c89_joined_value };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsigned_value as i32;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = point.x + point.y;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u32 = (bits.__bitfield_0.low() as u32) << 29 >> 29;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = (bits.__bitfield_0.high() as i32) << 28 >> 28;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { number.integer };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(0) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(3) };
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         printf(
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             13 as i32,
// COMMON-REWRITES-NEXT:             1 as i32,
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             (-7 as i8) as i32,
// COMMON-REWRITES-NEXT:             (2.5 as f32) as i32,
// COMMON-REWRITES-NEXT:             (3.5 as f64) as i32,
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}} + unsafe { *{{__v[0-9]+}} },
// COMMON-REWRITES-NEXT:             copied_value,
// COMMON-REWRITES-NEXT:             unsafe { Some(c89_add).unwrap()(53 as i32, 6 as i32) },
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}} + {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: fn c89_store({{arg[0-9]+}}: *mut core::ffi::c_void, {{arg[0-9]+}}: *mut core::ffi::c_void) {
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         *({{arg[0-9]+}} as *mut i32) = unsafe { *({{arg[0-9]+}} as *mut i32) };
// COMMON-REWRITES-NEXT:     return;
// COMMON-REWRITES-NEXT: extern "C-unwind" fn c89_add({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) -> i32 {
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}} + {{arg[0-9]+}}
// COMMON-REWRITES-NEXT: fn c89_static_local() -> i32 {
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         c89_static_local_calls = (unsafe { c89_static_local_calls }) + 1;
// COMMON-REWRITES-NEXT:     unsafe { c89_static_local_calls }
// COMMON-REWRITES-NEXT: unsafe fn c89_variadic_sum(mut count: i32, mut __slate_va_args: __SlateVaArgs) -> i32 {
// COMMON-REWRITES-NEXT:     let mut arguments: __SlateVaArgs = __SlateVaArgs::empty();
// COMMON-REWRITES-NEXT:     let mut total: i32 = 0;
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         arguments = __slate_va_args.clone();
// COMMON-REWRITES-NEXT:     for index in 0..count {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = unsafe { arguments.next_arg::<i32>() };
// COMMON-REWRITES-NEXT:         total += {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     total
// COMMON-REWRITES-NEXT: fn c89_control_flow(mut value: i32) -> i32 {
// COMMON-REWRITES-NEXT:     let mut __retval: i32 = 0;
// COMMON-REWRITES-NEXT:     let mut result: i32 = 0;
// COMMON-REWRITES-NEXT:     let mut index: i32 = 0;
// COMMON-REWRITES-NEXT:     let mut {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:     result = 0;
// COMMON-REWRITES-NEXT:     index = 0;
// COMMON-REWRITES-NEXT:     loop {
// COMMON-REWRITES-NEXT:         if index < value {
// COMMON-REWRITES-NEXT:             if index == 1 {
// COMMON-REWRITES-NEXT:                 index += 1;
// COMMON-REWRITES-NEXT:                 result += index;
// COMMON-REWRITES-NEXT:                 if result > 20 {
// COMMON-REWRITES-NEXT:                     break;
// COMMON-REWRITES-NEXT:                     index += 1;
// COMMON-REWRITES-NEXT:             break;
// COMMON-REWRITES-NEXT:     loop {
// COMMON-REWRITES-NEXT:         result -= 1;
// COMMON-REWRITES-NEXT:         if !(result > 6) {
// COMMON-REWRITES-NEXT:             break;
// COMMON-REWRITES-NEXT:     {{__v[0-9]+}} = value;
// COMMON-REWRITES-NEXT:     match {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         4 => {
// COMMON-REWRITES-NEXT:             result += 10;
// COMMON-REWRITES-NEXT:         _ => {
// COMMON-REWRITES-NEXT:             result = 0;
// COMMON-REWRITES-NEXT:     if result == 16 {
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         result = -1;
// COMMON-REWRITES-NEXT:     __retval = result;
// COMMON-REWRITES-NEXT:     __retval
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
// REWRITES-X86_64-GNU-NEXT: impl core::ops::DivAssign for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     fn div_assign(&mut self, __o: LongDouble) {
// REWRITES-X86_64-GNU-NEXT:         {
// REWRITES-X86_64-GNU-NEXT:             *self = __slate_f80_div(*self, __o);
// REWRITES-X86_64-GNU-NEXT: impl core::ops::Neg for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     type Output = LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn neg(self) -> LongDouble {
// REWRITES-X86_64-GNU-NEXT:         __slate_f80_neg(self)
// REWRITES-X86_64-GNU-NEXT: impl core::cmp::PartialEq for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     fn eq(&self, __other: &LongDouble) -> bool {
// REWRITES-X86_64-GNU-NEXT:         __slate_f80_eq(*self, *__other)
// REWRITES-X86_64-GNU-NEXT: impl core::cmp::PartialOrd for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     fn partial_cmp(&self, __other: &LongDouble) -> Option<std::cmp::Ordering> {
// REWRITES-X86_64-GNU-NEXT:         if __slate_f80_lt(*self, *__other) {
// REWRITES-X86_64-GNU-NEXT:             Some(std::cmp::Ordering::Less)
// REWRITES-X86_64-GNU-NEXT:             if __slate_f80_gt(*self, *__other) {
// REWRITES-X86_64-GNU-NEXT:                 Some(std::cmp::Ordering::Greater)
// REWRITES-X86_64-GNU-NEXT:                 if __slate_f80_eq(*self, *__other) {
// REWRITES-X86_64-GNU-NEXT:                     Some(std::cmp::Ordering::Equal)
// REWRITES-X86_64-GNU-NEXT:                     None
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT:     let mut array: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 144, 1, 64]);
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 =
// REWRITES-X86_64-GNU-NEXT:             .as_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = __slate_f80_to_i32({{__v[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:             90 as i32,
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT:             } else {
// REWRITES-X86_64-GNU-NEXT:                 } else {
// REWRITES-X86_64-GNU-NEXT:                 }
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:         } else {
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     }
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
// REWRITES-AARCH64-GNU-NEXT: #[repr(C)]
// REWRITES-AARCH64-GNU-NEXT: struct __va_list {
// REWRITES-AARCH64-GNU-NEXT:     __slate_empty: [u8; 0],
// REWRITES-AARCH64-GNU-NEXT:     let mut array: [i32; 4] = [0; 4];
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: f128 = 4.500000e+00f128;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 =
// REWRITES-AARCH64-GNU-NEXT:             .as_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// REWRITES-AARCH64-GNU-NEXT:             (90 as u32) as i32,
// SLATE-FILECHECK-END rewrites-aarch64-gnu
