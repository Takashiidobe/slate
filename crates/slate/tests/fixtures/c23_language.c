#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <uchar.h>

#pragma STDC FENV_ROUND FE_TONEAREST
#ifdef __STDC_IEC_60559_DFP__
#pragma STDC FENV_DEC_ROUND FE_DEC_TONEAREST
#endif

#warning C23 warning directive probe

#define C23_OPTIONAL(base, ...) ((base)__VA_OPT__(+(__VA_ARGS__)))

#define C23_TYPE_KIND(T) _Generic(T, int: 1, double: 2, char *: 3, default: 4)

#define C23_BRANCH_VALUE 1
#if 0
#define C23_ELIFDEF_VALUE 0
#elifdef C23_BRANCH_VALUE
#define C23_ELIFDEF_VALUE 23
#else
#define C23_ELIFDEF_VALUE 0
#endif

#if 0
#define C23_ELIFNDEF_VALUE 0
#elifndef C23_MISSING_VALUE
#define C23_ELIFNDEF_VALUE 29
#else
#define C23_ELIFNDEF_VALUE 0
#endif

#if __has_include(<stdint.h>)
#define C23_HAS_INCLUDE_VALUE 1
#else
#define C23_HAS_INCLUDE_VALUE 0
#endif

#if __has_embed("c23_embed.bin") == __STDC_EMBED_FOUND__
#define C23_HAS_EMBED_VALUE 1
#else
#define C23_HAS_EMBED_VALUE 0
#endif

#if __has_c_attribute(reproducible)
#define C23_REPRODUCIBLE_VALUE 1
#else
#define C23_REPRODUCIBLE_VALUE 0
#endif

#if __has_c_attribute(unsequenced)
#define C23_UNSEQUENCED_VALUE 1
#else
#define C23_UNSEQUENCED_VALUE 0
#endif

#ifdef __STDC_IEC_60559_DFP__
#define C23_DECIMAL_VALUE 1
#else
#define C23_DECIMAL_VALUE 0
#endif

#define C23_STORAGE_COMPOUND_VALUE 0

static const unsigned char c23_embedded[] = {
#embed "c23_embed.bin" limit(3)
};

enum C23Fixed : unsigned short { C23_FIXED_FIRST = 31, C23_FIXED_SECOND = 37 };

enum C23Wide { C23_WIDE_VALUE = 0x1ffffffff };

struct C23Empty {
  int first;
  int second;
};

typedef int C23Array[3];

[[deprecated("C23 deprecated attribute")]]
static int c23_deprecated_value = 41;

[[maybe_unused, maybe_unused]]
static int c23_maybe_unused_value = 43;

[[nodiscard("C23 nodiscard attribute")]]
static int c23_nodiscard_value(void) {
  return 47;
}

[[noreturn]]
static void c23_never_return(void) {
  exit(99);
}

static thread_local int c23_thread_value = 53;
static volatile int     c23_never_flag;

constexpr int c23_file_constant = 59;

static int c23_unnamed_parameter(int, int value) { return value; }

static int c23_label_declaration(int value) {
  goto c23_label;
c23_label:
  int result = value + 1;
  return result;
}

static void c23_label_before_brace(void) {
  goto c23_end;
c23_end:
}

static int c23_switch_fallthrough(int value) {
  int result = 0;
  switch (value) {
  case 1:
    result += 3;
    [[fallthrough]];
  case 2:
    result += 5;
    break;
  default:
    break;
  }
  return result;
}

static int c23_relaxed_variadic(...) {
  va_list arguments;
  int     first;
  int     second;
  va_start(arguments);
  first  = va_arg(arguments, int);
  second = va_arg(arguments, int);
  va_end(arguments);
  return first + second;
}

int main(void) {
  constexpr int          local_constant            = 61;
  alignas(32) int        aligned_value             = 3;
  auto                   inferred_value            = 67;
  typeof(inferred_value) same_type_value           = 71;
  const int              qualified_value           = 73;
  typeof_unqual(qualified_value) unqualified_value = 79;
  int \u03b1                                       = 5;
  signed _BitInt(17) signed_precise                = -12345;
  unsigned _BitInt(17) unsigned_precise            = 100000uwb;
  int                  binary_value                = 0b1010'0101;
  char8_t              utf8_character              = u8'Z';
  static const char8_t utf8_text[]                 = u8"\u03a9";
  struct C23Empty      empty_struct                = {};
  int                  empty_array[3]              = {};
  const C23Array       qualified_array             = {2, 3, 5};
  enum C23Fixed        fixed_value                 = C23_FIXED_SECOND;
  enum C23Wide         wide_value                  = C23_WIDE_VALUE;
  nullptr_t            null_value                  = nullptr;
  int                 *null_pointer                = nullptr;
  bool                 boolean_value               = true;
  bool                 false_value                 = false;
  int                  static_compound_value       = 83;
  int                  language_total;
  int                  attribute_total;
  int                  preprocessor_total;
  int                  type_total;
  int                  control_total;
  int                  removal_total;
#ifdef __STDC_IEC_60559_DFP__
  _Decimal32 decimal_value = 1.5df;
#endif

  static_assert(sizeof(binary_value) == sizeof(int));
  _Static_assert(sizeof(signed_precise) >= 3);
  static_assert(-1 == ~0);

  language_total =
      c23_file_constant + local_constant + inferred_value + same_type_value +
      unqualified_value + (int)signed_precise + (int)unsigned_precise +
      binary_value + aligned_value + alignof(int) + \u03b1 + utf8_character +
      (unsigned char)utf8_text[0] + (unsigned char)utf8_text[1] +
      empty_struct.first + empty_array[0] + qualified_array[0] +
      qualified_array[2] + fixed_value + (int)(wide_value == C23_WIDE_VALUE) +
      static_compound_value + C23_OPTIONAL(7) + C23_OPTIONAL(11, 13) +
      C23_TYPE_KIND(int) + C23_TYPE_KIND(double) + C23_TYPE_KIND(char *) +
      C23_TYPE_KIND(long);

  attribute_total    = c23_nodiscard_value();
  preprocessor_total = C23_ELIFDEF_VALUE + C23_ELIFNDEF_VALUE +
                       C23_HAS_INCLUDE_VALUE + C23_HAS_EMBED_VALUE +
                       C23_REPRODUCIBLE_VALUE + C23_UNSEQUENCED_VALUE +
                       C23_DECIMAL_VALUE + C23_STORAGE_COMPOUND_VALUE +
                       c23_embedded[0] + c23_embedded[1] + c23_embedded[2];
  type_total         = (null_value == nullptr) + (null_pointer == nullptr) +
                       boolean_value + !false_value + c23_thread_value;
  control_total = c23_unnamed_parameter(89, 97) + c23_label_declaration(101) +
                  c23_switch_fallthrough(1) + c23_relaxed_variadic(103, 107);
  removal_total = (__STDC_VERSION__ == 202311L);

  if (c23_never_flag) {
    c23_never_return();
  }
  c23_label_before_brace();

  printf("%d %d %d %d %d %d\n", language_total, attribute_total,
         preprocessor_total, type_total, control_total, removal_total);
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(thread_local)]
// LOWERING-NEXT: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: #[deprecated(note = "C23 warning directive probe")]
// LOWERING-NEXT: const __SLATE_WARNING_0: () = {  };
// LOWERING-EMPTY:
// LOWERING-NEXT: const _: () = __SLATE_WARNING_0;
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[allow(non_camel_case_types)]
// LOWERING-NEXT: #[derive(Clone, Copy, PartialEq, Eq, Debug, Hash)]
// LOWERING-NEXT: enum C23Fixed {
// LOWERING-NEXT:     C23_FIXED_FIRST = 0,
// LOWERING-NEXT:     C23_FIXED_SECOND = 1,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[allow(non_camel_case_types)]
// LOWERING-NEXT: #[derive(Clone, Copy, PartialEq, Eq, Debug, Hash)]
// LOWERING-NEXT: enum C23Wide {
// LOWERING-NEXT:     C23_WIDE_VALUE = 0,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct C23Empty {
// LOWERING-NEXT:     first: i32,
// LOWERING-NEXT:     second: i32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut c23_embedded: [u8; 3] = [67, 50, 51];
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut c23_file_constant: i32 = 59;
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut c23_never_flag: i32 = 0;
// LOWERING-EMPTY:
// LOWERING-NEXT: #[thread_local]
// LOWERING-NEXT: static mut c23_thread_value: i32 = 53;
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut main_utf8_text: [u8; 3] = [206, 169, 0];
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn exit(_0: i32) -> !;
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
// LOWERING-NEXT: #[must_use]
// LOWERING-NEXT: fn c23_nodiscard_value() -> i32 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 47;
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn c23_unnamed_parameter({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) -> i32 {
// LOWERING-NEXT:     return {{arg[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn c23_label_declaration({{arg[0-9]+}}: i32) -> i32 {
// LOWERING-NEXT:     let mut value: i32 = 0;
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut result: i32 = 0;
// LOWERING-NEXT:     let mut {{__state[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     '{{__dispatch[0-9]+}}: loop {
// LOWERING-NEXT:         match {{__state[0-9]+}} {
// LOWERING-NEXT:             0 => {
// LOWERING-NEXT:                 value = {{arg[0-9]+}};
// LOWERING-NEXT:                 {{__state[0-9]+}} = 1;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             1 => {
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = value;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:                 result = {{_v[0-9]+}};
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
// LOWERING-NEXT: fn c23_switch_fallthrough({{arg[0-9]+}}: i32) -> i32 {
// LOWERING-NEXT:     let mut value: i32 = 0;
// LOWERING-NEXT:     let mut result: i32 = 0;
// LOWERING-NEXT:     value = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     result = {{_v[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = value;
// LOWERING-NEXT:         {
// LOWERING-NEXT:             let __switch_value0 = {{_v[0-9]+}};
// LOWERING-NEXT:             let mut __switch_case0: i32 = match __switch_value0 { 1 => 0, 2 => 1, _ => 2 };
// LOWERING-NEXT:             '__switch0: loop {
// LOWERING-NEXT:                 match __switch_case0 {
// LOWERING-NEXT:                     0 => {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = result;
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:                         result = {{_v[0-9]+}};
// LOWERING-NEXT:                         __switch_case0 = 1;
// LOWERING-NEXT:                         continue '__switch0;
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                     1 => {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = 5;
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = result;
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:                         result = {{_v[0-9]+}};
// LOWERING-NEXT:                         break '__switch0;
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                     2 => {
// LOWERING-NEXT:                         break '__switch0;
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                     _ => {
// LOWERING-NEXT:                         break '__switch0;
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = result;
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe fn c23_relaxed_variadic(mut __slate_va_args: __SlateVaArgs) -> i32 {
// LOWERING-NEXT:     let mut arguments: __SlateVaArgs = __SlateVaArgs::empty();
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         arguments = __slate_va_args.clone();
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { arguments.next_arg::<i32>() };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { arguments.next_arg::<i32>() };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn c23_never_return() -> ! {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 99;
// LOWERING-NEXT:     unsafe { exit({{_v[0-9]+}} as i32) }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn c23_label_before_brace() {
// LOWERING-NEXT:     let mut {{__state[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     '{{__dispatch[0-9]+}}: loop {
// LOWERING-NEXT:         match {{__state[0-9]+}} {
// LOWERING-NEXT:             0 => {
// LOWERING-NEXT:                 {{__state[0-9]+}} = 1;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             1 => {
// LOWERING-NEXT:                 return;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             _ => {
// LOWERING-NEXT:                 break '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut empty_struct: C23Empty = C23Empty { first: 0, second: 0 };
// LOWERING-NEXT:     let mut empty_array: [i32; 3] = [0; 3];
// LOWERING-NEXT:     let mut qualified_array: [i32; 3] = [0; 3];
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 61;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 67;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 71;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 73;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 79;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 5;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bitint::BInt<17, 1, 4> = bitint::BInt::<17, 1, 4>::from_decimal_str("-12345");
// LOWERING-NEXT:     let {{_v[0-9]+}}: bitint::BUint<17, 1, 4> = bitint::BUint::<17, 1, 4>::from_decimal_str("100000");
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 165;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u8 = 90;
// LOWERING-NEXT:     empty_struct = C23Empty { first: 0, second: 0 };
// LOWERING-NEXT:     empty_array = [0, 0, 0];
// LOWERING-NEXT:     qualified_array = [2, 3, 5];
// LOWERING-NEXT:     let {{_v[0-9]+}}: u16 = 37;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 8589934591u64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = true;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 83;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { c23_file_constant };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}}.to_i128() as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}}.to_u128() as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} as u64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 4;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} as u64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} as u64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u8 = unsafe { main_utf8_text[({{_v[0-9]+}} as usize)] };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} as u64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u8 = unsafe { main_utf8_text[({{_v[0-9]+}} as usize)] };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} as u64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = empty_struct.first;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} as u64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = empty_array[({{_v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} as u64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = qualified_array[({{_v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} as u64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = qualified_array[({{_v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} as u64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} as u64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 8589934591u64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} as u64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} as u64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 7;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 11;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 13;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} as u64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 4;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = c23_nodiscard_value();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 23;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 29;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u8 = unsafe { c23_embedded[({{_v[0-9]+}} as usize)] };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u8 = unsafe { c23_embedded[({{_v[0-9]+}} as usize)] };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u8 = unsafe { c23_embedded[({{_v[0-9]+}} as usize)] };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { c23_thread_value };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 89;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 97;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = c23_unnamed_parameter({{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 101;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = c23_label_declaration({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = c23_switch_fallthrough({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 103;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 107;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { c23_relaxed_variadic(__SlateVaArgs::new(vec![__SlateVaArg::new({{_v[0-9]+}}), __SlateVaArg::new({{_v[0-9]+}})])) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 202311;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 202311;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(c23_never_flag)) };
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:         if {{_v[0-9]+}} {
// LOWERING-NEXT:             c23_never_return();
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     c23_label_before_brace();
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %d %d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(thread_local)]
// REWRITES-NEXT: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: #[deprecated(note = "C23 warning directive probe")]
// REWRITES-NEXT: const __SLATE_WARNING_0: () = {  };
// REWRITES-EMPTY:
// REWRITES-NEXT: const _: () = __SLATE_WARNING_0;
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[allow(non_camel_case_types)]
// REWRITES-NEXT: #[derive(Clone, Copy, PartialEq, Eq, Debug, Hash)]
// REWRITES-NEXT: enum C23Fixed {
// REWRITES-NEXT:     C23_FIXED_FIRST = 0,
// REWRITES-NEXT:     C23_FIXED_SECOND = 1,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[allow(non_camel_case_types)]
// REWRITES-NEXT: #[derive(Clone, Copy, PartialEq, Eq, Debug, Hash)]
// REWRITES-NEXT: enum C23Wide {
// REWRITES-NEXT:     C23_WIDE_VALUE = 0,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct C23Empty {
// REWRITES-NEXT:     first: i32,
// REWRITES-NEXT:     second: i32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut c23_embedded: [u8; 3] = [67, 50, 51];
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut c23_file_constant: i32 = 59;
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut c23_never_flag: i32 = 0;
// REWRITES-EMPTY:
// REWRITES-NEXT: #[thread_local]
// REWRITES-NEXT: static mut c23_thread_value: i32 = 53;
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut main_utf8_text: [u8; 3] = [206, 169, 0];
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn exit(_0: i32) -> !;
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
// REWRITES-NEXT: #[must_use]
// REWRITES-NEXT: fn c23_nodiscard_value() -> i32 {
// REWRITES-NEXT: return 47;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn c23_unnamed_parameter({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) -> i32 {
// REWRITES-NEXT: return {{arg[0-9]+}};
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn c23_label_declaration({{arg[0-9]+}}: i32) -> i32 {
// REWRITES-NEXT: let mut value: i32 = 0;
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut result: i32 = 0;
// REWRITES-NEXT: let mut {{__state[0-9]+}}: i32 = 0;
// REWRITES-NEXT: '{{__dispatch[0-9]+}}: loop {
// REWRITES-NEXT:         match {{__state[0-9]+}} {
// REWRITES-NEXT:             0 => {
// REWRITES-NEXT:                         value = {{arg[0-9]+}};
// REWRITES-NEXT:                         {{__state[0-9]+}} = 1;
// REWRITES-NEXT:                         continue '{{__dispatch[0-9]+}};
// REWRITES-NEXT:             }
// REWRITES-NEXT:             1 => {
// REWRITES-NEXT:                         let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT:                         result = value + {{_v[0-9]+}};
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
// REWRITES-NEXT: fn c23_switch_fallthrough({{arg[0-9]+}}: i32) -> i32 {
// REWRITES-NEXT: let mut value: i32 = {{arg[0-9]+}};
// REWRITES-NEXT: let mut result: i32 = 0;
// REWRITES-NEXT: result = 0;
// REWRITES-NEXT: {
// REWRITES-NEXT:         {
// REWRITES-NEXT:                     let __switch_value0 = value;
// REWRITES-NEXT:                     let mut __switch_case0: i32 = match __switch_value0 { 1 => 0, 2 => 1, _ => 2 };
// REWRITES-NEXT:                     '__switch0: loop {
// REWRITES-NEXT:                                     match __switch_case0 {
// REWRITES-NEXT:                                         0 => {
// REWRITES-NEXT:                                                             let {{_v[0-9]+}}: i32 = 3;
// REWRITES-NEXT:                                                             result = result + {{_v[0-9]+}};
// REWRITES-NEXT:                                                             __switch_case0 = 1;
// REWRITES-NEXT:                                                             continue '__switch0;
// REWRITES-NEXT:                                         }
// REWRITES-NEXT:                                         1 => {
// REWRITES-NEXT:                                                             let {{_v[0-9]+}}: i32 = 5;
// REWRITES-NEXT:                                                             result = result + {{_v[0-9]+}};
// REWRITES-NEXT:                                                             break '__switch0;
// REWRITES-NEXT:                                         }
// REWRITES-NEXT:                                         2 => {
// REWRITES-NEXT:                                                             break '__switch0;
// REWRITES-NEXT:                                         }
// REWRITES-NEXT:                                         _ => {
// REWRITES-NEXT:                                                             break '__switch0;
// REWRITES-NEXT:                                         }
// REWRITES-NEXT:                                     }
// REWRITES-NEXT:                     }
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: return result;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe fn c23_relaxed_variadic(mut __slate_va_args: __SlateVaArgs) -> i32 {
// REWRITES-NEXT: let mut arguments: __SlateVaArgs = __SlateVaArgs::empty();
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         arguments = __slate_va_args.clone();
// REWRITES-NEXT: }
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { arguments.next_arg::<i32>() };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { arguments.next_arg::<i32>() };
// REWRITES-NEXT: return {{_v[0-9]+}} + {{_v[0-9]+}};
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn c23_never_return() -> ! {
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 99;
// REWRITES-NEXT: unsafe { std::process::exit({{_v[0-9]+}} as i32) }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn c23_label_before_brace() {
// REWRITES-NEXT: let mut {{__state[0-9]+}}: i32 = 0;
// REWRITES-NEXT: '{{__dispatch[0-9]+}}: loop {
// REWRITES-NEXT:         match {{__state[0-9]+}} {
// REWRITES-NEXT:             0 => {
// REWRITES-NEXT:                         {{__state[0-9]+}} = 1;
// REWRITES-NEXT:                         continue '{{__dispatch[0-9]+}};
// REWRITES-NEXT:             }
// REWRITES-NEXT:             1 => {
// REWRITES-NEXT:                         return;
// REWRITES-NEXT:             }
// REWRITES-NEXT:             _ => {
// REWRITES-NEXT:                         break '{{__dispatch[0-9]+}};
// REWRITES-NEXT:             }
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut empty_struct: C23Empty = C23Empty { first: 0, second: 0 };
// REWRITES-NEXT: let mut empty_array: [i32; 3] = [0, 0, 0];
// REWRITES-NEXT: let mut qualified_array: [i32; 3] = [2, 3, 5];
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 61;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 3;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 67;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 71;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 73;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 79;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 5;
// REWRITES-NEXT: let {{_v[0-9]+}}: bitint::BInt<17, 1, 4> = bitint::BInt::<17, 1, 4>::from_decimal_str("-12345");
// REWRITES-NEXT: let {{_v[0-9]+}}: bitint::BUint<17, 1, 4> = bitint::BUint::<17, 1, 4>::from_decimal_str("100000");
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 165;
// REWRITES-NEXT: let {{_v[0-9]+}}: u8 = 90;
// REWRITES-NEXT: let {{_v[0-9]+}}: u16 = 37;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 8589934591u64;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i32 = std::ptr::null_mut();
// REWRITES-NEXT: let {{_v[0-9]+}}: bool = true;
// REWRITES-NEXT: let {{_v[0-9]+}}: bool = false;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 83;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = (unsafe { c23_file_constant }) + {{_v[0-9]+}} + {{_v[0-9]+}} + {{_v[0-9]+}} + {{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}}.to_i128() as i32;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}}.to_u128() as i32;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 4;
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = 2;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 7;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 11;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 13;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 2;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 3;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 4;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = ((({{_v[0-9]+}} + {{_v[0-9]+}} + {{_v[0-9]+}} + {{_v[0-9]+}}) as u64) + {{_v[0-9]+}} + ({{_v[0-9]+}} as u64) + ({{_v[0-9]+}} as u64) + ((unsafe { main_utf8_text[({{_v[0-9]+}} as usize)] }) as u64) + ((unsafe { main_utf8_text[({{_v[0-9]+}} as usize)] }) as u64) + (empty_struct.first as u64) + (empty_array[({{_v[0-9]+}} as usize)] as u64) + (qualified_array[({{_v[0-9]+}} as usize)] as u64) + (qualified_array[({{_v[0-9]+}} as usize)] as u64) + ({{_v[0-9]+}} as u64) + ((({{_v[0-9]+}} == 8589934591u64) as i32) as u64) + ({{_v[0-9]+}} as u64) + {{_v[0-9]+}} + (({{_v[0-9]+}} + {{_v[0-9]+}}) as u64) + {{_v[0-9]+}} + {{_v[0-9]+}} + {{_v[0-9]+}} + {{_v[0-9]+}}) as i32;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = c23_nodiscard_value();
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 23;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 29;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = 2;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}} + {{_v[0-9]+}} + {{_v[0-9]+}} + {{_v[0-9]+}} + {{_v[0-9]+}} + {{_v[0-9]+}} + {{_v[0-9]+}} + ((unsafe { c23_embedded[({{_v[0-9]+}} as usize)] }) as i32) + ((unsafe { c23_embedded[({{_v[0-9]+}} as usize)] }) as i32) + ((unsafe { c23_embedded[({{_v[0-9]+}} as usize)] }) as i32);
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i32 = std::ptr::null_mut();
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = (({{_v[0-9]+}} == {{_v[0-9]+}}) as i32) + (({{_v[0-9]+}} == {{_v[0-9]+}}) as i32) + ({{_v[0-9]+}} as i32);
// REWRITES-NEXT: let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + ({{_v[0-9]+}} as i32) + unsafe { c23_thread_value };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 89;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 97;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = c23_unnamed_parameter({{_v[0-9]+}}, {{_v[0-9]+}});
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 101;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = c23_label_declaration({{_v[0-9]+}});
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = c23_switch_fallthrough({{_v[0-9]+}});
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 103;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 107;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { c23_relaxed_variadic(__SlateVaArgs::new(vec![__SlateVaArg::new({{_v[0-9]+}}), __SlateVaArg::new({{_v[0-9]+}})])) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = 202311;
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = 202311;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = ({{_v[0-9]+}} == {{_v[0-9]+}}) as i32;
// REWRITES-NEXT: {
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(c23_never_flag)) };
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// REWRITES-NEXT:         if {{_v[0-9]+}} {
// REWRITES-NEXT:                     c23_never_return();
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: c23_label_before_brace();
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%d %d %d %d %d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: std::process::exit({{_v[0-9]+}} as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
