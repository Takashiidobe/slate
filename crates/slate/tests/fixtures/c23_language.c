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

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING: #![feature(thread_local)]
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
// COMMON-LOWERING-NEXT: #[deprecated(note = "C23 warning directive probe")]
// COMMON-LOWERING-NEXT: const __SLATE_WARNING_0: () = {};
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: const _: () = __SLATE_WARNING_0;
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[allow(non_camel_case_types)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy, PartialEq, Eq, Debug, Hash)]
// COMMON-LOWERING-NEXT: enum C23Fixed {
// COMMON-LOWERING-NEXT:     C23_FIXED_FIRST = 0,
// COMMON-LOWERING-NEXT:     C23_FIXED_SECOND = 1,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[allow(non_camel_case_types)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy, PartialEq, Eq, Debug, Hash)]
// COMMON-LOWERING-NEXT: enum C23Wide {
// COMMON-LOWERING-NEXT:     C23_WIDE_VALUE = 0,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct C23Empty {
// COMMON-LOWERING-NEXT:     first: i32,
// COMMON-LOWERING-NEXT:     second: i32,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: static mut c23_embedded: [u8; 3] = [67, 50, 51];
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: static mut c23_file_constant: i32 = 59;
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: static mut c23_never_flag: i32 = 0;
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[thread_local]
// COMMON-LOWERING-NEXT: static mut c23_thread_value: i32 = 53;
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: static mut main_utf8_text: [u8; 3] = [206, 169, 0];
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT:     fn exit(_0: i32) -> !;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: struct __SlateVaArg {
// COMMON-LOWERING-NEXT:     value: Box<dyn std::any::Any>,
// COMMON-LOWERING-NEXT:     size: usize,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: impl __SlateVaArg {
// COMMON-LOWERING-NEXT:     fn new<T: 'static>(value: T) -> Self {
// COMMON-LOWERING-NEXT:         Self {
// COMMON-LOWERING-NEXT:             value: Box::new(value),
// COMMON-LOWERING-NEXT:             size: std::mem::size_of::<T>(),
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-EMPTY:
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
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[derive(Clone)]
// COMMON-LOWERING-NEXT: struct __SlateVaArgs {
// COMMON-LOWERING-NEXT:     args: Option<std::rc::Rc<Vec<__SlateVaArg>>>,
// COMMON-LOWERING-NEXT:     index: usize,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: impl __SlateVaArgs {
// COMMON-LOWERING-NEXT:     fn new(args: Vec<__SlateVaArg>) -> Self {
// COMMON-LOWERING-NEXT:         Self {
// COMMON-LOWERING-NEXT:             args: Some(std::rc::Rc::new(args)),
// COMMON-LOWERING-NEXT:             index: 0,
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT:     const fn empty() -> Self {
// COMMON-LOWERING-NEXT:         Self {
// COMMON-LOWERING-NEXT:             args: None,
// COMMON-LOWERING-NEXT:             index: 0,
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT:     fn next_arg<T: Copy + 'static>(&mut self) -> T {
// COMMON-LOWERING-NEXT:         let index = self.index;
// COMMON-LOWERING-NEXT:         self.index += 1;
// COMMON-LOWERING-NEXT:         if std::mem::size_of::<T>() == 0 {
// COMMON-LOWERING-NEXT:             return unsafe { std::mem::zeroed() };
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:         let args = self.args.as_ref().expect("va_arg with no arguments");
// COMMON-LOWERING-NEXT:         args[index].read::<T>()
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let mut empty_struct: C23Empty = C23Empty {
// COMMON-LOWERING-NEXT:         first: 0,
// COMMON-LOWERING-NEXT:         second: 0,
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let mut empty_array: [i32; 3] = [0; 3];
// COMMON-LOWERING-NEXT:     let mut qualified_array: [i32; 3] = [0; 3];
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 61;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 3;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 67;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 71;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 73;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 79;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 5;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bitint::BInt<17, 1, 4> = bitint::BInt::<17, 1, 4>::from_decimal_str("-12345");
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bitint::BUint<17, 1, 4> = bitint::BUint::<17, 1, 4>::from_decimal_str("100000");
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 165;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = 90;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: C23Empty = C23Empty {
// COMMON-LOWERING-NEXT:         first: 0,
// COMMON-LOWERING-NEXT:         second: 0,
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     empty_struct = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: [i32; 3] = [0; 3];
// COMMON-LOWERING-NEXT:     empty_array = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: [i32; 3] = [2, 3, 5];
// COMMON-LOWERING-NEXT:     qualified_array = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u16 = 37;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 8589934591u64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = true;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = false;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 83;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { c23_file_constant };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}}.to_i128() as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}}.to_u128() as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} as u64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 4;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} as u64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} as u64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = unsafe { main_utf8_text[({{__v[0-9]+}} as usize)] };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} as u64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = unsafe { main_utf8_text[({{__v[0-9]+}} as usize)] };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} as u64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = empty_struct.first;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} as u64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = empty_array[({{__v[0-9]+}} as usize)];
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} as u64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = qualified_array[({{__v[0-9]+}} as usize)];
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} as u64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 2;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = qualified_array[({{__v[0-9]+}} as usize)];
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} as u64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} as u64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 8589934591u64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} as u64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} as u64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 7;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 11;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 13;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} as u64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 2;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 3;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 4;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = c23_nodiscard_value();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 23;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 29;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = unsafe { c23_embedded[({{__v[0-9]+}} as usize)] };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = unsafe { c23_embedded[({{__v[0-9]+}} as usize)] };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 2;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = unsafe { c23_embedded[({{__v[0-9]+}} as usize)] };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { c23_thread_value };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 89;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 97;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = c23_unnamed_parameter({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 101;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = c23_label_declaration({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = c23_switch_fallthrough({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 103;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 107;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         c23_relaxed_variadic(__SlateVaArgs::new(vec![
// COMMON-LOWERING-NEXT:             __SlateVaArg::new({{__v[0-9]+}}),
// COMMON-LOWERING-NEXT:             __SlateVaArg::new({{__v[0-9]+}}),
// COMMON-LOWERING-NEXT:         ]))
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 202311;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 202311;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(c23_never_flag)) };
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:         if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:             c23_never_return();
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     c23_label_before_brace();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         printf(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
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
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[must_use]
// COMMON-LOWERING-NEXT: fn c23_nodiscard_value() -> i32 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 47;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn c23_unnamed_parameter({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) -> i32 {
// COMMON-LOWERING-NEXT:     return {{arg[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn c23_label_declaration({{arg[0-9]+}}: i32) -> i32 {
// COMMON-LOWERING-NEXT:     let mut value: i32 = 0;
// COMMON-LOWERING-NEXT:     let mut __retval: i32 = 0;
// COMMON-LOWERING-NEXT:     let mut result: i32 = 0;
// COMMON-LOWERING-NEXT:     let mut {{__state[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     '{{__dispatch[0-9]+}}: loop {
// COMMON-LOWERING-NEXT:         match {{__state[0-9]+}} {
// COMMON-LOWERING-NEXT:             0 => {
// COMMON-LOWERING-NEXT:                 value = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:                 {{__state[0-9]+}} = 1;
// COMMON-LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             1 => {
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = value;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                 result = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = result;
// COMMON-LOWERING-NEXT:                 __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:                 return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             _ => {
// COMMON-LOWERING-NEXT:                 unreachable!();
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn c23_switch_fallthrough({{arg[0-9]+}}: i32) -> i32 {
// COMMON-LOWERING-NEXT:     let mut value: i32 = 0;
// COMMON-LOWERING-NEXT:     let mut result: i32 = 0;
// COMMON-LOWERING-NEXT:     value = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     result = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = value;
// COMMON-LOWERING-NEXT:         {
// COMMON-LOWERING-NEXT:             let __switch_value0 = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             let mut __switch_case0: i32 = match __switch_value0 {
// COMMON-LOWERING-NEXT:                 1 => 0,
// COMMON-LOWERING-NEXT:                 2 => 1,
// COMMON-LOWERING-NEXT:                 _ => 2,
// COMMON-LOWERING-NEXT:             };
// COMMON-LOWERING-NEXT:             '__switch0: loop {
// COMMON-LOWERING-NEXT:                 match __switch_case0 {
// COMMON-LOWERING-NEXT:                     0 => {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = 3;
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = result;
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         result = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         __switch_case0 = 1;
// COMMON-LOWERING-NEXT:                         continue '__switch0;
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                     1 => {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = 5;
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = result;
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         result = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         break '__switch0;
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                     2 => {
// COMMON-LOWERING-NEXT:                         break '__switch0;
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                     _ => {
// COMMON-LOWERING-NEXT:                         break '__switch0;
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = result;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: unsafe fn c23_relaxed_variadic(mut __slate_va_args: __SlateVaArgs) -> i32 {
// COMMON-LOWERING-NEXT:     let mut arguments: __SlateVaArgs = __SlateVaArgs::empty();
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         arguments = __slate_va_args.clone();
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { arguments.next_arg::<i32>() };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { arguments.next_arg::<i32>() };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn c23_never_return() -> ! {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 99;
// COMMON-LOWERING-NEXT:     unsafe { exit({{__v[0-9]+}} as i32) }
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn c23_label_before_brace() {
// COMMON-LOWERING-NEXT:     let mut {{__state[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     '{{__dispatch[0-9]+}}: loop {
// COMMON-LOWERING-NEXT:         match {{__state[0-9]+}} {
// COMMON-LOWERING-NEXT:             0 => {
// COMMON-LOWERING-NEXT:                 {{__state[0-9]+}} = 1;
// COMMON-LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             1 => {
// COMMON-LOWERING-NEXT:                 return;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             _ => {
// COMMON-LOWERING-NEXT:                 break '{{__dispatch[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d %d %d %d %d\n\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT: #[repr(C)]
// LOWERING-AARCH64-GNU-NEXT: #[derive(Clone, Copy)]
// LOWERING-AARCH64-GNU-NEXT: struct __va_list {
// LOWERING-AARCH64-GNU-NEXT:     __slate_empty: [u8; 0],
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %d %d %d %d %d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-EMPTY:
// LOWERING-AARCH64-GNU-NEXT: }
// SLATE-FILECHECK-END lowering-aarch64-gnu

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES: #![feature(thread_local)]
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
// COMMON-REWRITES-NEXT: #[deprecated(note = "C23 warning directive probe")]
// COMMON-REWRITES-NEXT: const __SLATE_WARNING_0: () = {};
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: const _: () = __SLATE_WARNING_0;
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[allow(non_camel_case_types)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy, PartialEq, Eq, Debug, Hash)]
// COMMON-REWRITES-NEXT: enum C23Fixed {
// COMMON-REWRITES-NEXT:     C23_FIXED_FIRST = 0,
// COMMON-REWRITES-NEXT:     C23_FIXED_SECOND = 1,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[allow(non_camel_case_types)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy, PartialEq, Eq, Debug, Hash)]
// COMMON-REWRITES-NEXT: enum C23Wide {
// COMMON-REWRITES-NEXT:     C23_WIDE_VALUE = 0,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct C23Empty {
// COMMON-REWRITES-NEXT:     first: i32,
// COMMON-REWRITES-NEXT:     second: i32,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: static mut c23_embedded: [u8; 3] = [67, 50, 51];
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: static mut c23_file_constant: i32 = 59;
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: static mut c23_never_flag: i32 = 0;
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[thread_local]
// COMMON-REWRITES-NEXT: static mut c23_thread_value: i32 = 53;
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: static mut main_utf8_text: [u8; 3] = [206, 169, 0];
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT:     fn exit(_0: i32) -> !;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: struct __SlateVaArg {
// COMMON-REWRITES-NEXT:     value: Box<dyn std::any::Any>,
// COMMON-REWRITES-NEXT:     size: usize,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: impl __SlateVaArg {
// COMMON-REWRITES-NEXT:     fn new<T: 'static>(value: T) -> Self {
// COMMON-REWRITES-NEXT:         Self {
// COMMON-REWRITES-NEXT:             value: Box::new(value),
// COMMON-REWRITES-NEXT:             size: std::mem::size_of::<T>(),
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-EMPTY:
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
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[derive(Clone)]
// COMMON-REWRITES-NEXT: struct __SlateVaArgs {
// COMMON-REWRITES-NEXT:     args: Option<std::rc::Rc<Vec<__SlateVaArg>>>,
// COMMON-REWRITES-NEXT:     index: usize,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: impl __SlateVaArgs {
// COMMON-REWRITES-NEXT:     fn new(args: Vec<__SlateVaArg>) -> Self {
// COMMON-REWRITES-NEXT:         Self {
// COMMON-REWRITES-NEXT:             args: Some(std::rc::Rc::new(args)),
// COMMON-REWRITES-NEXT:             index: 0,
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT:     const fn empty() -> Self {
// COMMON-REWRITES-NEXT:         Self {
// COMMON-REWRITES-NEXT:             args: None,
// COMMON-REWRITES-NEXT:             index: 0,
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT:     fn next_arg<T: Copy + 'static>(&mut self) -> T {
// COMMON-REWRITES-NEXT:         let index = self.index;
// COMMON-REWRITES-NEXT:         self.index += 1;
// COMMON-REWRITES-NEXT:         if std::mem::size_of::<T>() == 0 {
// COMMON-REWRITES-NEXT:             return unsafe { std::mem::zeroed() };
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         let args = self.args.as_ref().expect("va_arg with no arguments");
// COMMON-REWRITES-NEXT:         args[index].read::<T>()
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     let mut empty_struct: C23Empty = C23Empty {
// COMMON-REWRITES-NEXT:         first: 0,
// COMMON-REWRITES-NEXT:         second: 0,
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let mut empty_array: [i32; 3] = [0; 3];
// COMMON-REWRITES-NEXT:     let mut qualified_array: [i32; 3] = [2, 3, 5];
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 61;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 67;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 71;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 79;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bitint::BInt<17, 1, 4> = bitint::BInt::<17, 1, 4>::from_decimal_str("-12345");
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bitint::BUint<17, 1, 4> = bitint::BUint::<17, 1, 4>::from_decimal_str("100000");
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = 8589934591u64;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i32 = std::ptr::null_mut();
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = true;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = (unsafe { c23_file_constant }) + {{__v[0-9]+}} + {{__v[0-9]+}} + {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}}.to_i128() as i32;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}}.to_u128() as i32;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = ((({{__v[0-9]+}} + {{__v[0-9]+}} + (165 as i32) + (3 as i32)) as u64)
// COMMON-REWRITES-NEXT:         + 4
// COMMON-REWRITES-NEXT:         + ((5 as i32) as u64)
// COMMON-REWRITES-NEXT:         + ((90 as u8) as u64)
// COMMON-REWRITES-NEXT:         + ((unsafe { main_utf8_text[0] }) as u64)
// COMMON-REWRITES-NEXT:         + ((unsafe { main_utf8_text[1] }) as u64)
// COMMON-REWRITES-NEXT:         + (empty_struct.first as u64)
// COMMON-REWRITES-NEXT:         + (empty_array[0] as u64)
// COMMON-REWRITES-NEXT:         + (qualified_array[0] as u64)
// COMMON-REWRITES-NEXT:         + (qualified_array[2] as u64)
// COMMON-REWRITES-NEXT:         + ((37 as u16) as u64)
// COMMON-REWRITES-NEXT:         + ((({{__v[0-9]+}} == 8589934591u64) as i32) as u64)
// COMMON-REWRITES-NEXT:         + ((83 as i32) as u64)
// COMMON-REWRITES-NEXT:         + 7
// COMMON-REWRITES-NEXT:         + ((11 + (13 as i32)) as u64)
// COMMON-REWRITES-NEXT:         + 1
// COMMON-REWRITES-NEXT:         + 2
// COMMON-REWRITES-NEXT:         + 3
// COMMON-REWRITES-NEXT:         + 4) as i32;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = c23_nodiscard_value();
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 29;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 23
// COMMON-REWRITES-NEXT:         + {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:         + 1
// COMMON-REWRITES-NEXT:         + 1
// COMMON-REWRITES-NEXT:         + 0
// COMMON-REWRITES-NEXT:         + 0
// COMMON-REWRITES-NEXT:         + 0
// COMMON-REWRITES-NEXT:         + 0
// COMMON-REWRITES-NEXT:         + ((unsafe { c23_embedded[0] }) as i32)
// COMMON-REWRITES-NEXT:         + ((unsafe { c23_embedded[1] }) as i32)
// COMMON-REWRITES-NEXT:         + ((unsafe { c23_embedded[2] }) as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = ((std::ptr::null_mut() == {{__v[0-9]+}}) as i32)
// COMMON-REWRITES-NEXT:         + (({{__v[0-9]+}} == std::ptr::null_mut()) as i32)
// COMMON-REWRITES-NEXT:         + ({{__v[0-9]+}} as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + ({{__v[0-9]+}} as i32) + unsafe { c23_thread_value };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = c23_unnamed_parameter(89, 97);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = c23_label_declaration(101);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = c23_switch_fallthrough(1);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-REWRITES-NEXT:         c23_relaxed_variadic(__SlateVaArgs::new(vec![
// COMMON-REWRITES-NEXT:             __SlateVaArg::new(103 as i32),
// COMMON-REWRITES-NEXT:             __SlateVaArg::new(107 as i32),
// COMMON-REWRITES-NEXT:         ]))
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(c23_never_flag)) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-REWRITES-NEXT:     if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         c23_never_return();
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     c23_label_before_brace();
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         printf(
// COMMON-REWRITES-NEXT:             c"%d %d %d %d %d %d\n".as_ptr(),
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             (202311 == (202311 as i64)) as i32,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[must_use]
// COMMON-REWRITES-NEXT: fn c23_nodiscard_value() -> i32 {
// COMMON-REWRITES-NEXT:     47
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn c23_unnamed_parameter({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) -> i32 {
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}}
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn c23_label_declaration(mut {{__v[0-9]+}}: i32) -> i32 {
// COMMON-REWRITES-NEXT:     let mut __retval: i32 = 0;
// COMMON-REWRITES-NEXT:     let mut result: i32 = 0;
// COMMON-REWRITES-NEXT:     result = {{__v[0-9]+}} + 1;
// COMMON-REWRITES-NEXT:     __retval = result;
// COMMON-REWRITES-NEXT:     __retval
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn c23_switch_fallthrough(mut {{__v[0-9]+}}: i32) -> i32 {
// COMMON-REWRITES-NEXT:     let mut result: i32 = 0;
// COMMON-REWRITES-NEXT:     match {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         1 => {
// COMMON-REWRITES-NEXT:             result += 3;
// COMMON-REWRITES-NEXT:             let _v9: i32 = 5;
// COMMON-REWRITES-NEXT:             let _v10: i32 = result;
// COMMON-REWRITES-NEXT:             let _v11: i32 = _v10 + _v9;
// COMMON-REWRITES-NEXT:             result = _v11;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         2 => {
// COMMON-REWRITES-NEXT:             result += 5;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         _ => {}
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     result
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: unsafe fn c23_relaxed_variadic(mut __slate_va_args: __SlateVaArgs) -> i32 {
// COMMON-REWRITES-NEXT:     let mut arguments: __SlateVaArgs = __SlateVaArgs::empty();
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         arguments = __slate_va_args.clone();
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { arguments.next_arg::<i32>() };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { arguments.next_arg::<i32>() };
// COMMON-REWRITES-NEXT:     {{__v[0-9]+}} + {{__v[0-9]+}}
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn c23_never_return() -> ! {
// COMMON-REWRITES-NEXT:     unsafe { std::process::exit(99 as i32) }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn c23_label_before_brace() {
// COMMON-REWRITES-NEXT:     return;
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-NEXT: #[repr(C)]
// REWRITES-AARCH64-GNU-NEXT: #[derive(Clone, Copy)]
// REWRITES-AARCH64-GNU-NEXT: struct __va_list {
// REWRITES-AARCH64-GNU-NEXT:     __slate_empty: [u8; 0],
// REWRITES-AARCH64-GNU-EMPTY:
// REWRITES-AARCH64-GNU-NEXT: }
// SLATE-FILECHECK-END rewrites-aarch64-gnu
