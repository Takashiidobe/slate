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
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 47;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: i32 = __retval;
// LOWERING-NEXT:     return _v1;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn c23_unnamed_parameter(arg2: i32, arg3: i32) -> i32 {
// LOWERING-NEXT:     let mut _59: i32 = 0;
// LOWERING-NEXT:     let mut value: i32 = 0;
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     _59 = arg2;
// LOWERING-NEXT:     value = arg3;
// LOWERING-NEXT:     let _v0: i32 = value;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: i32 = __retval;
// LOWERING-NEXT:     return _v1;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn c23_label_declaration(arg1: i32) -> i32 {
// LOWERING-NEXT:     let mut value: i32 = 0;
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut result: i32 = 0;
// LOWERING-NEXT:     let mut __state0: i32 = 0;
// LOWERING-NEXT:     '__dispatch0: loop {
// LOWERING-NEXT:         match __state0 {
// LOWERING-NEXT:             0 => {
// LOWERING-NEXT:                 value = arg1;
// LOWERING-NEXT:                 __state0 = 1;
// LOWERING-NEXT:                 continue '__dispatch0;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             1 => {
// LOWERING-NEXT:                 let _v0: i32 = value;
// LOWERING-NEXT:                 let _v1: i32 = 1;
// LOWERING-NEXT:                 let _v2: i32 = _v0 + _v1;
// LOWERING-NEXT:                 result = _v2;
// LOWERING-NEXT:                 let _v3: i32 = result;
// LOWERING-NEXT:                 __retval = _v3;
// LOWERING-NEXT:                 let _v4: i32 = __retval;
// LOWERING-NEXT:                 return _v4;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             _ => {
// LOWERING-NEXT:                 unreachable!();
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn c23_switch_fallthrough(arg0: i32) -> i32 {
// LOWERING-NEXT:     let mut value: i32 = 0;
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut result: i32 = 0;
// LOWERING-NEXT:     value = arg0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     result = _v0;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let _v1: i32 = value;
// LOWERING-NEXT:         {
// LOWERING-NEXT:             let __switch_value0 = _v1;
// LOWERING-NEXT:             let mut __switch_case0: i32 = match __switch_value0 { 1 => 0, 2 => 1, _ => 2 };
// LOWERING-NEXT:             '__switch0: loop {
// LOWERING-NEXT:                 match __switch_case0 {
// LOWERING-NEXT:                     0 => {
// LOWERING-NEXT:                         let _v2: i32 = 3;
// LOWERING-NEXT:                         let _v3: i32 = result;
// LOWERING-NEXT:                         let _v4: i32 = _v3 + _v2;
// LOWERING-NEXT:                         result = _v4;
// LOWERING-NEXT:                         __switch_case0 = 1;
// LOWERING-NEXT:                         continue '__switch0;
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                     1 => {
// LOWERING-NEXT:                         let _v5: i32 = 5;
// LOWERING-NEXT:                         let _v6: i32 = result;
// LOWERING-NEXT:                         let _v7: i32 = _v6 + _v5;
// LOWERING-NEXT:                         result = _v7;
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
// LOWERING-NEXT:     let _v8: i32 = result;
// LOWERING-NEXT:     __retval = _v8;
// LOWERING-NEXT:     let _v9: i32 = __retval;
// LOWERING-NEXT:     return _v9;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe fn c23_relaxed_variadic(mut __slate_va_args: __SlateVaArgs) -> i32 {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut arguments: __SlateVaArgs = __SlateVaArgs::empty();
// LOWERING-NEXT:     let mut first: i32 = 0;
// LOWERING-NEXT:     let mut second: i32 = 0;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         arguments = __slate_va_args.clone();
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v0: i32 = unsafe { arguments.next_arg::<i32>() };
// LOWERING-NEXT:     first = _v0;
// LOWERING-NEXT:     let _v1: i32 = unsafe { arguments.next_arg::<i32>() };
// LOWERING-NEXT:     second = _v1;
// LOWERING-NEXT:     let _v2: i32 = first;
// LOWERING-NEXT:     let _v3: i32 = second;
// LOWERING-NEXT:     let _v4: i32 = _v2 + _v3;
// LOWERING-NEXT:     __retval = _v4;
// LOWERING-NEXT:     let _v5: i32 = __retval;
// LOWERING-NEXT:     return _v5;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn c23_never_return() -> ! {
// LOWERING-NEXT:     let _v0: i32 = 99;
// LOWERING-NEXT:     unsafe { exit(_v0 as i32) }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn c23_label_before_brace() {
// LOWERING-NEXT:     let mut __state0: i32 = 0;
// LOWERING-NEXT:     '__dispatch0: loop {
// LOWERING-NEXT:         match __state0 {
// LOWERING-NEXT:             0 => {
// LOWERING-NEXT:                 __state0 = 1;
// LOWERING-NEXT:                 continue '__dispatch0;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             1 => {
// LOWERING-NEXT:                 return;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             _ => {
// LOWERING-NEXT:                 break '__dispatch0;
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut local_constant: i32 = 0;
// LOWERING-NEXT:     let mut aligned_value: aligned::Aligned<aligned::A32, i32> = aligned::Aligned(0);
// LOWERING-NEXT:     let mut inferred_value: i32 = 0;
// LOWERING-NEXT:     let mut same_type_value: i32 = 0;
// LOWERING-NEXT:     let mut qualified_value: i32 = 0;
// LOWERING-NEXT:     let mut unqualified_value: i32 = 0;
// LOWERING-NEXT:     let mut signed_precise: aligned::Aligned<aligned::A4, bitint::BInt<17, 1, 4>> = aligned::Aligned(bitint::BInt::<17, 1, 4>::ZERO);
// LOWERING-NEXT:     let mut unsigned_precise: aligned::Aligned<aligned::A4, bitint::BUint<17, 1, 4>> = aligned::Aligned(bitint::BUint::<17, 1, 4>::ZERO);
// LOWERING-NEXT:     let mut binary_value: i32 = 0;
// LOWERING-NEXT:     let mut utf8_character: u8 = 0;
// LOWERING-NEXT:     let mut empty_struct: C23Empty = C23Empty { first: 0, second: 0 };
// LOWERING-NEXT:     let mut empty_array: [i32; 3] = [0; 3];
// LOWERING-NEXT:     let mut qualified_array: [i32; 3] = [0; 3];
// LOWERING-NEXT:     let mut fixed_value: u16 = 0;
// LOWERING-NEXT:     let mut wide_value: u64 = 0;
// LOWERING-NEXT:     let mut null_value: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let mut null_pointer: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut boolean_value: bool = false;
// LOWERING-NEXT:     let mut false_value: bool = false;
// LOWERING-NEXT:     let mut static_compound_value: i32 = 0;
// LOWERING-NEXT:     let mut language_total: i32 = 0;
// LOWERING-NEXT:     let mut attribute_total: i32 = 0;
// LOWERING-NEXT:     let mut preprocessor_total: i32 = 0;
// LOWERING-NEXT:     let mut type_total: i32 = 0;
// LOWERING-NEXT:     let mut control_total: i32 = 0;
// LOWERING-NEXT:     let mut removal_total: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: i32 = 61;
// LOWERING-NEXT:     local_constant = _v1;
// LOWERING-NEXT:     let _v2: i32 = 3;
// LOWERING-NEXT:     *aligned_value = _v2;
// LOWERING-NEXT:     let _v3: i32 = 67;
// LOWERING-NEXT:     inferred_value = _v3;
// LOWERING-NEXT:     let _v4: i32 = 71;
// LOWERING-NEXT:     same_type_value = _v4;
// LOWERING-NEXT:     let _v5: i32 = 73;
// LOWERING-NEXT:     qualified_value = _v5;
// LOWERING-NEXT:     let _v6: i32 = 79;
// LOWERING-NEXT:     unqualified_value = _v6;
// LOWERING-NEXT:     let _v7: i32 = 5;
// LOWERING-NEXT:     let _v8: bitint::BInt<17, 1, 4> = bitint::BInt::<17, 1, 4>::from_decimal_str("-12345");
// LOWERING-NEXT:     *signed_precise = _v8;
// LOWERING-NEXT:     let _v9: bitint::BUint<17, 1, 4> = bitint::BUint::<17, 1, 4>::from_decimal_str("100000");
// LOWERING-NEXT:     *unsigned_precise = _v9;
// LOWERING-NEXT:     let _v10: i32 = 165;
// LOWERING-NEXT:     binary_value = _v10;
// LOWERING-NEXT:     let _v11: u8 = 90;
// LOWERING-NEXT:     utf8_character = _v11;
// LOWERING-NEXT:     empty_struct = C23Empty { first: 0, second: 0 };
// LOWERING-NEXT:     empty_array = [0, 0, 0];
// LOWERING-NEXT:     qualified_array = [2, 3, 5];
// LOWERING-NEXT:     let _v12: u16 = 37;
// LOWERING-NEXT:     fixed_value = _v12;
// LOWERING-NEXT:     let _v13: u64 = 8589934591u64;
// LOWERING-NEXT:     wide_value = _v13;
// LOWERING-NEXT:     let _v14: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     null_value = _v14;
// LOWERING-NEXT:     let _v15: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     null_pointer = _v15;
// LOWERING-NEXT:     let _v16: bool = true;
// LOWERING-NEXT:     boolean_value = _v16;
// LOWERING-NEXT:     let _v17: bool = false;
// LOWERING-NEXT:     false_value = _v17;
// LOWERING-NEXT:     let _v18: i32 = 83;
// LOWERING-NEXT:     static_compound_value = _v18;
// LOWERING-NEXT:     let _v19: i32 = unsafe { c23_file_constant };
// LOWERING-NEXT:     let _v20: i32 = local_constant;
// LOWERING-NEXT:     let _v21: i32 = _v19 + _v20;
// LOWERING-NEXT:     let _v22: i32 = inferred_value;
// LOWERING-NEXT:     let _v23: i32 = _v21 + _v22;
// LOWERING-NEXT:     let _v24: i32 = same_type_value;
// LOWERING-NEXT:     let _v25: i32 = _v23 + _v24;
// LOWERING-NEXT:     let _v26: i32 = unqualified_value;
// LOWERING-NEXT:     let _v27: i32 = _v25 + _v26;
// LOWERING-NEXT:     let _v28: bitint::BInt<17, 1, 4> = *signed_precise;
// LOWERING-NEXT:     let _v29: i32 = _v28.to_i128() as i32;
// LOWERING-NEXT:     let _v30: i32 = _v27 + _v29;
// LOWERING-NEXT:     let _v31: bitint::BUint<17, 1, 4> = *unsigned_precise;
// LOWERING-NEXT:     let _v32: i32 = _v31.to_u128() as i32;
// LOWERING-NEXT:     let _v33: i32 = _v30 + _v32;
// LOWERING-NEXT:     let _v34: i32 = binary_value;
// LOWERING-NEXT:     let _v35: i32 = _v33 + _v34;
// LOWERING-NEXT:     let _v36: i32 = *aligned_value;
// LOWERING-NEXT:     let _v37: i32 = _v35 + _v36;
// LOWERING-NEXT:     let _v38: u64 = _v37 as u64;
// LOWERING-NEXT:     let _v39: u64 = 4;
// LOWERING-NEXT:     let _v40: u64 = _v38 + _v39;
// LOWERING-NEXT:     let _v41: u64 = _v7 as u64;
// LOWERING-NEXT:     let _v42: u64 = _v40 + _v41;
// LOWERING-NEXT:     let _v43: u8 = utf8_character;
// LOWERING-NEXT:     let _v44: u64 = _v43 as u64;
// LOWERING-NEXT:     let _v45: u64 = _v42 + _v44;
// LOWERING-NEXT:     let _v46: i64 = 0;
// LOWERING-NEXT:     let _v47: u8 = unsafe { main_utf8_text[(_v46 as usize)] };
// LOWERING-NEXT:     let _v48: u64 = _v47 as u64;
// LOWERING-NEXT:     let _v49: u64 = _v45 + _v48;
// LOWERING-NEXT:     let _v50: i64 = 1;
// LOWERING-NEXT:     let _v51: u8 = unsafe { main_utf8_text[(_v50 as usize)] };
// LOWERING-NEXT:     let _v52: u64 = _v51 as u64;
// LOWERING-NEXT:     let _v53: u64 = _v49 + _v52;
// LOWERING-NEXT:     let _v54: i32 = empty_struct.first;
// LOWERING-NEXT:     let _v55: u64 = _v54 as u64;
// LOWERING-NEXT:     let _v56: u64 = _v53 + _v55;
// LOWERING-NEXT:     let _v57: i64 = 0;
// LOWERING-NEXT:     let _v58: i32 = empty_array[(_v57 as usize)];
// LOWERING-NEXT:     let _v59: u64 = _v58 as u64;
// LOWERING-NEXT:     let _v60: u64 = _v56 + _v59;
// LOWERING-NEXT:     let _v61: i64 = 0;
// LOWERING-NEXT:     let _v62: i32 = qualified_array[(_v61 as usize)];
// LOWERING-NEXT:     let _v63: u64 = _v62 as u64;
// LOWERING-NEXT:     let _v64: u64 = _v60 + _v63;
// LOWERING-NEXT:     let _v65: i64 = 2;
// LOWERING-NEXT:     let _v66: i32 = qualified_array[(_v65 as usize)];
// LOWERING-NEXT:     let _v67: u64 = _v66 as u64;
// LOWERING-NEXT:     let _v68: u64 = _v64 + _v67;
// LOWERING-NEXT:     let _v69: u16 = fixed_value;
// LOWERING-NEXT:     let _v70: u64 = _v69 as u64;
// LOWERING-NEXT:     let _v71: u64 = _v68 + _v70;
// LOWERING-NEXT:     let _v72: u64 = wide_value;
// LOWERING-NEXT:     let _v73: u64 = 8589934591u64;
// LOWERING-NEXT:     let _v74: bool = _v72 == _v73;
// LOWERING-NEXT:     let _v75: i32 = _v74 as i32;
// LOWERING-NEXT:     let _v76: u64 = _v75 as u64;
// LOWERING-NEXT:     let _v77: u64 = _v71 + _v76;
// LOWERING-NEXT:     let _v78: i32 = static_compound_value;
// LOWERING-NEXT:     let _v79: u64 = _v78 as u64;
// LOWERING-NEXT:     let _v80: u64 = _v77 + _v79;
// LOWERING-NEXT:     let _v81: u64 = 7;
// LOWERING-NEXT:     let _v82: u64 = _v80 + _v81;
// LOWERING-NEXT:     let _v83: i32 = 11;
// LOWERING-NEXT:     let _v84: i32 = 13;
// LOWERING-NEXT:     let _v85: i32 = _v83 + _v84;
// LOWERING-NEXT:     let _v86: u64 = _v85 as u64;
// LOWERING-NEXT:     let _v87: u64 = _v82 + _v86;
// LOWERING-NEXT:     let _v88: u64 = 1;
// LOWERING-NEXT:     let _v89: u64 = _v87 + _v88;
// LOWERING-NEXT:     let _v90: u64 = 2;
// LOWERING-NEXT:     let _v91: u64 = _v89 + _v90;
// LOWERING-NEXT:     let _v92: u64 = 3;
// LOWERING-NEXT:     let _v93: u64 = _v91 + _v92;
// LOWERING-NEXT:     let _v94: u64 = 4;
// LOWERING-NEXT:     let _v95: u64 = _v93 + _v94;
// LOWERING-NEXT:     let _v96: i32 = _v95 as i32;
// LOWERING-NEXT:     language_total = _v96;
// LOWERING-NEXT:     let _v97: i32 = c23_nodiscard_value();
// LOWERING-NEXT:     attribute_total = _v97;
// LOWERING-NEXT:     let _v98: i32 = 23;
// LOWERING-NEXT:     let _v99: i32 = 29;
// LOWERING-NEXT:     let _v100: i32 = _v98 + _v99;
// LOWERING-NEXT:     let _v101: i32 = 1;
// LOWERING-NEXT:     let _v102: i32 = _v100 + _v101;
// LOWERING-NEXT:     let _v103: i32 = 1;
// LOWERING-NEXT:     let _v104: i32 = _v102 + _v103;
// LOWERING-NEXT:     let _v105: i32 = 0;
// LOWERING-NEXT:     let _v106: i32 = _v104 + _v105;
// LOWERING-NEXT:     let _v107: i32 = 0;
// LOWERING-NEXT:     let _v108: i32 = _v106 + _v107;
// LOWERING-NEXT:     let _v109: i32 = 0;
// LOWERING-NEXT:     let _v110: i32 = _v108 + _v109;
// LOWERING-NEXT:     let _v111: i32 = 0;
// LOWERING-NEXT:     let _v112: i32 = _v110 + _v111;
// LOWERING-NEXT:     let _v113: i64 = 0;
// LOWERING-NEXT:     let _v114: u8 = unsafe { c23_embedded[(_v113 as usize)] };
// LOWERING-NEXT:     let _v115: i32 = _v114 as i32;
// LOWERING-NEXT:     let _v116: i32 = _v112 + _v115;
// LOWERING-NEXT:     let _v117: i64 = 1;
// LOWERING-NEXT:     let _v118: u8 = unsafe { c23_embedded[(_v117 as usize)] };
// LOWERING-NEXT:     let _v119: i32 = _v118 as i32;
// LOWERING-NEXT:     let _v120: i32 = _v116 + _v119;
// LOWERING-NEXT:     let _v121: i64 = 2;
// LOWERING-NEXT:     let _v122: u8 = unsafe { c23_embedded[(_v121 as usize)] };
// LOWERING-NEXT:     let _v123: i32 = _v122 as i32;
// LOWERING-NEXT:     let _v124: i32 = _v120 + _v123;
// LOWERING-NEXT:     preprocessor_total = _v124;
// LOWERING-NEXT:     let _v125: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let _v126: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let _v127: bool = _v125 == _v126;
// LOWERING-NEXT:     let _v128: i32 = _v127 as i32;
// LOWERING-NEXT:     let _v129: *mut i32 = null_pointer;
// LOWERING-NEXT:     let _v130: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let _v131: bool = _v129 == _v130;
// LOWERING-NEXT:     let _v132: i32 = _v131 as i32;
// LOWERING-NEXT:     let _v133: i32 = _v128 + _v132;
// LOWERING-NEXT:     let _v134: bool = boolean_value;
// LOWERING-NEXT:     let _v135: i32 = _v134 as i32;
// LOWERING-NEXT:     let _v136: i32 = _v133 + _v135;
// LOWERING-NEXT:     let _v137: bool = false_value;
// LOWERING-NEXT:     let _v138: bool = !_v137;
// LOWERING-NEXT:     let _v139: i32 = _v138 as i32;
// LOWERING-NEXT:     let _v140: i32 = _v136 + _v139;
// LOWERING-NEXT:     let _v141: i32 = unsafe { c23_thread_value };
// LOWERING-NEXT:     let _v142: i32 = _v140 + _v141;
// LOWERING-NEXT:     type_total = _v142;
// LOWERING-NEXT:     let _v143: i32 = 89;
// LOWERING-NEXT:     let _v144: i32 = 97;
// LOWERING-NEXT:     let _v145: i32 = c23_unnamed_parameter(_v143, _v144);
// LOWERING-NEXT:     let _v146: i32 = 101;
// LOWERING-NEXT:     let _v147: i32 = c23_label_declaration(_v146);
// LOWERING-NEXT:     let _v148: i32 = _v145 + _v147;
// LOWERING-NEXT:     let _v149: i32 = 1;
// LOWERING-NEXT:     let _v150: i32 = c23_switch_fallthrough(_v149);
// LOWERING-NEXT:     let _v151: i32 = _v148 + _v150;
// LOWERING-NEXT:     let _v152: i32 = 103;
// LOWERING-NEXT:     let _v153: i32 = 107;
// LOWERING-NEXT:     let _v154: i32 = unsafe { c23_relaxed_variadic(__SlateVaArgs::new(vec![__SlateVaArg::new(_v152), __SlateVaArg::new(_v153)])) };
// LOWERING-NEXT:     let _v155: i32 = _v151 + _v154;
// LOWERING-NEXT:     control_total = _v155;
// LOWERING-NEXT:     let _v156: i64 = 202311;
// LOWERING-NEXT:     let _v157: i64 = 202311;
// LOWERING-NEXT:     let _v158: bool = _v156 == _v157;
// LOWERING-NEXT:     let _v159: i32 = _v158 as i32;
// LOWERING-NEXT:     removal_total = _v159;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let _v160: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(c23_never_flag)) };
// LOWERING-NEXT:         let _v161: bool = _v160 != 0;
// LOWERING-NEXT:         if _v161 {
// LOWERING-NEXT:             c23_never_return();
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     c23_label_before_brace();
// LOWERING-NEXT:     let _v162: *mut i8 = b"%d %d %d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v163: i32 = language_total;
// LOWERING-NEXT:     let _v164: i32 = attribute_total;
// LOWERING-NEXT:     let _v165: i32 = preprocessor_total;
// LOWERING-NEXT:     let _v166: i32 = type_total;
// LOWERING-NEXT:     let _v167: i32 = control_total;
// LOWERING-NEXT:     let _v168: i32 = removal_total;
// LOWERING-NEXT:     let _v169: i32 = unsafe { printf(_v162 as *const i8, _v163, _v164, _v165, _v166, _v167, _v168) };
// LOWERING-NEXT:     let _v170: i32 = 0;
// LOWERING-NEXT:     __retval = _v170;
// LOWERING-NEXT:     let _v171: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v171 as i32);
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
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: __retval = 47;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn c23_unnamed_parameter(arg2: i32, arg3: i32) -> i32 {
// REWRITES-NEXT: let mut _59: i32 = arg2;
// REWRITES-NEXT: let mut value: i32 = arg3;
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: __retval = value;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn c23_label_declaration(arg1: i32) -> i32 {
// REWRITES-NEXT: let mut value: i32 = 0;
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut result: i32 = 0;
// REWRITES-NEXT: let mut __state0: i32 = 0;
// REWRITES-NEXT: '__dispatch0: loop {
// REWRITES-NEXT:         match __state0 {
// REWRITES-NEXT:             0 => {
// REWRITES-NEXT:                         value = arg1;
// REWRITES-NEXT:                         __state0 = 1;
// REWRITES-NEXT:                         continue '__dispatch0;
// REWRITES-NEXT:             }
// REWRITES-NEXT:             1 => {
// REWRITES-NEXT:                         let _v1: i32 = 1;
// REWRITES-NEXT:                         result = value + _v1;
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
// REWRITES-NEXT: fn c23_switch_fallthrough(arg0: i32) -> i32 {
// REWRITES-NEXT: let mut value: i32 = arg0;
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut result: i32 = 0;
// REWRITES-NEXT: result = 0;
// REWRITES-NEXT: {
// REWRITES-NEXT:         {
// REWRITES-NEXT:                     let __switch_value0 = value;
// REWRITES-NEXT:                     let mut __switch_case0: i32 = match __switch_value0 { 1 => 0, 2 => 1, _ => 2 };
// REWRITES-NEXT:                     '__switch0: loop {
// REWRITES-NEXT:                                     match __switch_case0 {
// REWRITES-NEXT:                                         0 => {
// REWRITES-NEXT:                                                             let _v2: i32 = 3;
// REWRITES-NEXT:                                                             result = result + _v2;
// REWRITES-NEXT:                                                             __switch_case0 = 1;
// REWRITES-NEXT:                                                             continue '__switch0;
// REWRITES-NEXT:                                         }
// REWRITES-NEXT:                                         1 => {
// REWRITES-NEXT:                                                             let _v5: i32 = 5;
// REWRITES-NEXT:                                                             result = result + _v5;
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
// REWRITES-NEXT: __retval = result;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe fn c23_relaxed_variadic(mut __slate_va_args: __SlateVaArgs) -> i32 {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut arguments: __SlateVaArgs = __SlateVaArgs::empty();
// REWRITES-NEXT: let mut first: i32 = 0;
// REWRITES-NEXT: let mut second: i32 = 0;
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         arguments = __slate_va_args.clone();
// REWRITES-NEXT: }
// REWRITES-NEXT: first = unsafe { arguments.next_arg::<i32>() };
// REWRITES-NEXT: second = unsafe { arguments.next_arg::<i32>() };
// REWRITES-NEXT: __retval = first + second;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn c23_never_return() -> ! {
// REWRITES-NEXT: let _v0: i32 = 99;
// REWRITES-NEXT: unsafe { std::process::exit(_v0 as i32) }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn c23_label_before_brace() {
// REWRITES-NEXT: let mut __state0: i32 = 0;
// REWRITES-NEXT: '__dispatch0: loop {
// REWRITES-NEXT:         match __state0 {
// REWRITES-NEXT:             0 => {
// REWRITES-NEXT:                         __state0 = 1;
// REWRITES-NEXT:                         continue '__dispatch0;
// REWRITES-NEXT:             }
// REWRITES-NEXT:             1 => {
// REWRITES-NEXT:                         return;
// REWRITES-NEXT:             }
// REWRITES-NEXT:             _ => {
// REWRITES-NEXT:                         break '__dispatch0;
// REWRITES-NEXT:             }
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut local_constant: i32 = 0;
// REWRITES-NEXT: let mut aligned_value: aligned::Aligned<aligned::A32, i32> = aligned::Aligned(0);
// REWRITES-NEXT: let mut inferred_value: i32 = 0;
// REWRITES-NEXT: let mut same_type_value: i32 = 0;
// REWRITES-NEXT: let mut qualified_value: i32 = 0;
// REWRITES-NEXT: let mut unqualified_value: i32 = 0;
// REWRITES-NEXT: let mut signed_precise: aligned::Aligned<aligned::A4, bitint::BInt<17, 1, 4>> = aligned::Aligned(bitint::BInt::<17, 1, 4>::ZERO);
// REWRITES-NEXT: let mut unsigned_precise: aligned::Aligned<aligned::A4, bitint::BUint<17, 1, 4>> = aligned::Aligned(bitint::BUint::<17, 1, 4>::ZERO);
// REWRITES-NEXT: let mut binary_value: i32 = 0;
// REWRITES-NEXT: let mut utf8_character: u8 = 0;
// REWRITES-NEXT: let mut empty_struct: C23Empty = C23Empty { first: 0, second: 0 };
// REWRITES-NEXT: let mut empty_array: [i32; 3] = [0; 3];
// REWRITES-NEXT: let mut qualified_array: [i32; 3] = [0; 3];
// REWRITES-NEXT: let mut fixed_value: u16 = 0;
// REWRITES-NEXT: let mut wide_value: u64 = 0;
// REWRITES-NEXT: let mut null_value: *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-NEXT: let mut null_pointer: *mut i32 = std::ptr::null_mut();
// REWRITES-NEXT: let mut boolean_value: bool = false;
// REWRITES-NEXT: let mut false_value: bool = false;
// REWRITES-NEXT: let mut static_compound_value: i32 = 0;
// REWRITES-NEXT: let mut language_total: i32 = 0;
// REWRITES-NEXT: let mut attribute_total: i32 = 0;
// REWRITES-NEXT: let mut preprocessor_total: i32 = 0;
// REWRITES-NEXT: let mut type_total: i32 = 0;
// REWRITES-NEXT: let mut control_total: i32 = 0;
// REWRITES-NEXT: let mut removal_total: i32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: local_constant = 61;
// REWRITES-NEXT: *aligned_value = 3;
// REWRITES-NEXT: inferred_value = 67;
// REWRITES-NEXT: same_type_value = 71;
// REWRITES-NEXT: qualified_value = 73;
// REWRITES-NEXT: unqualified_value = 79;
// REWRITES-NEXT: let _v7: i32 = 5;
// REWRITES-NEXT: let _v8: bitint::BInt<17, 1, 4> = bitint::BInt::<17, 1, 4>::from_decimal_str("-12345");
// REWRITES-NEXT: *signed_precise = _v8;
// REWRITES-NEXT: let _v9: bitint::BUint<17, 1, 4> = bitint::BUint::<17, 1, 4>::from_decimal_str("100000");
// REWRITES-NEXT: *unsigned_precise = _v9;
// REWRITES-NEXT: binary_value = 165;
// REWRITES-NEXT: utf8_character = 90;
// REWRITES-NEXT: empty_struct = C23Empty { first: 0, second: 0 };
// REWRITES-NEXT: empty_array = [0, 0, 0];
// REWRITES-NEXT: qualified_array = [2, 3, 5];
// REWRITES-NEXT: fixed_value = 37;
// REWRITES-NEXT: wide_value = 8589934591u64;
// REWRITES-NEXT: null_value = std::ptr::null_mut();
// REWRITES-NEXT: null_pointer = std::ptr::null_mut();
// REWRITES-NEXT: boolean_value = true;
// REWRITES-NEXT: false_value = false;
// REWRITES-NEXT: static_compound_value = 83;
// REWRITES-NEXT: let _v27: i32 = (unsafe { c23_file_constant }) + local_constant + inferred_value + same_type_value + unqualified_value;
// REWRITES-NEXT: let _v28: bitint::BInt<17, 1, 4> = *signed_precise;
// REWRITES-NEXT: let _v29: i32 = _v28.to_i128() as i32;
// REWRITES-NEXT: let _v30: i32 = _v27 + _v29;
// REWRITES-NEXT: let _v31: bitint::BUint<17, 1, 4> = *unsigned_precise;
// REWRITES-NEXT: let _v32: i32 = _v31.to_u128() as i32;
// REWRITES-NEXT: let _v39: u64 = 4;
// REWRITES-NEXT: let _v46: i64 = 0;
// REWRITES-NEXT: let _v50: i64 = 1;
// REWRITES-NEXT: let _v57: i64 = 0;
// REWRITES-NEXT: let _v61: i64 = 0;
// REWRITES-NEXT: let _v65: i64 = 2;
// REWRITES-NEXT: let _v81: u64 = 7;
// REWRITES-NEXT: let _v83: i32 = 11;
// REWRITES-NEXT: let _v84: i32 = 13;
// REWRITES-NEXT: let _v88: u64 = 1;
// REWRITES-NEXT: let _v90: u64 = 2;
// REWRITES-NEXT: let _v92: u64 = 3;
// REWRITES-NEXT: let _v94: u64 = 4;
// REWRITES-NEXT: language_total = (((_v30 + _v32 + binary_value + *aligned_value) as u64) + _v39 + (_v7 as u64) + (utf8_character as u64) + ((unsafe { main_utf8_text[(_v46 as usize)] }) as u64) + ((unsafe { main_utf8_text[(_v50 as usize)] }) as u64) + (empty_struct.first as u64) + (empty_array[(_v57 as usize)] as u64) + (qualified_array[(_v61 as usize)] as u64) + (qualified_array[(_v65 as usize)] as u64) + (fixed_value as u64) + (((wide_value == 8589934591u64) as i32) as u64) + (static_compound_value as u64) + _v81 + ((_v83 + _v84) as u64) + _v88 + _v90 + _v92 + _v94) as i32;
// REWRITES-NEXT: attribute_total = c23_nodiscard_value();
// REWRITES-NEXT: let _v98: i32 = 23;
// REWRITES-NEXT: let _v99: i32 = 29;
// REWRITES-NEXT: let _v101: i32 = 1;
// REWRITES-NEXT: let _v103: i32 = 1;
// REWRITES-NEXT: let _v105: i32 = 0;
// REWRITES-NEXT: let _v107: i32 = 0;
// REWRITES-NEXT: let _v109: i32 = 0;
// REWRITES-NEXT: let _v111: i32 = 0;
// REWRITES-NEXT: let _v113: i64 = 0;
// REWRITES-NEXT: let _v117: i64 = 1;
// REWRITES-NEXT: let _v121: i64 = 2;
// REWRITES-NEXT: preprocessor_total = _v98 + _v99 + _v101 + _v103 + _v105 + _v107 + _v109 + _v111 + ((unsafe { c23_embedded[(_v113 as usize)] }) as i32) + ((unsafe { c23_embedded[(_v117 as usize)] }) as i32) + ((unsafe { c23_embedded[(_v121 as usize)] }) as i32);
// REWRITES-NEXT: let _v125: *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-NEXT: let _v126: *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-NEXT: let _v130: *mut i32 = std::ptr::null_mut();
// REWRITES-NEXT: let _v136: i32 = ((_v125 == _v126) as i32) + ((null_pointer == _v130) as i32) + (boolean_value as i32);
// REWRITES-NEXT: let _v138: bool = !false_value;
// REWRITES-NEXT: type_total = _v136 + (_v138 as i32) + unsafe { c23_thread_value };
// REWRITES-NEXT: let _v143: i32 = 89;
// REWRITES-NEXT: let _v144: i32 = 97;
// REWRITES-NEXT: let _v145: i32 = c23_unnamed_parameter(_v143, _v144);
// REWRITES-NEXT: let _v146: i32 = 101;
// REWRITES-NEXT: let _v147: i32 = c23_label_declaration(_v146);
// REWRITES-NEXT: let _v148: i32 = _v145 + _v147;
// REWRITES-NEXT: let _v149: i32 = 1;
// REWRITES-NEXT: let _v150: i32 = c23_switch_fallthrough(_v149);
// REWRITES-NEXT: let _v151: i32 = _v148 + _v150;
// REWRITES-NEXT: let _v152: i32 = 103;
// REWRITES-NEXT: let _v153: i32 = 107;
// REWRITES-NEXT: let _v154: i32 = unsafe { c23_relaxed_variadic(__SlateVaArgs::new(vec![__SlateVaArg::new(_v152), __SlateVaArg::new(_v153)])) };
// REWRITES-NEXT: control_total = _v151 + _v154;
// REWRITES-NEXT: let _v156: i64 = 202311;
// REWRITES-NEXT: let _v157: i64 = 202311;
// REWRITES-NEXT: removal_total = (_v156 == _v157) as i32;
// REWRITES-NEXT: {
// REWRITES-NEXT:         let _v160: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(c23_never_flag)) };
// REWRITES-NEXT:         let _v161: bool = _v160 != 0;
// REWRITES-NEXT:         if _v161 {
// REWRITES-NEXT:                     c23_never_return();
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: c23_label_before_brace();
// REWRITES-NEXT: let _v162: *mut i8 = b"%d %d %d %d %d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v169: i32 = unsafe { printf(_v162 as *const i8, language_total, attribute_total, preprocessor_total, type_total, control_total, removal_total) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
