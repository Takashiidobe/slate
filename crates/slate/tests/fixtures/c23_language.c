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

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![feature(thread_local)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: #[deprecated(note = "C23 warning directive probe")]
// CHECK-NEXT: const __SLATE_WARNING_0: () = {};
// CHECK-NEXT: const _: () = __SLATE_WARNING_0;
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT:     fn exit(__status: i32);
// CHECK-NEXT: }
// CHECK-NEXT: static mut c23_embedded: [u8; 3] = [
// CHECK-NEXT:     ((67 as i32) as i8) as u8,
// CHECK-NEXT:     ((50 as i32) as i8) as u8,
// CHECK-NEXT:     ((51 as i32) as i8) as u8,
// CHECK-NEXT: ];
// CHECK-NEXT: #[repr(C, align(32))]
// CHECK-NEXT: struct __SlateAlign32<T>(T);
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct C23Empty {
// CHECK-NEXT:     first: i32,
// CHECK-NEXT:     second: i32,
// CHECK-NEXT: }
// CHECK-NEXT: static mut c23_deprecated_value: i32 = 41 as i32;
// CHECK-NEXT: static mut c23_maybe_unused_value: i32 = 43 as i32;
// CHECK-NEXT: fn c23_nodiscard_value() -> i32 {
// CHECK-NEXT:     47
// CHECK-NEXT: }
// CHECK-NEXT: fn c23_never_return() {
// CHECK-NEXT:     unsafe { std::process::exit(99 as i32) };
// CHECK-NEXT: }
// CHECK-NEXT: #[thread_local]
// CHECK-NEXT: static mut c23_thread_value: i32 = 53 as i32;
// CHECK-NEXT: static mut c23_never_flag: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut c23_file_constant: i32 = 59 as i32;
// CHECK-NEXT: fn c23_unnamed_parameter([[V_0:__v[0-9]+]]: i32, value: i32) -> i32 {
// CHECK-NEXT:     value
// CHECK-NEXT: }
// CHECK-NEXT: fn c23_label_declaration(value: i32) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let result: i32 = value + 1;
// CHECK-NEXT:         return result;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn c23_label_before_brace() {
// CHECK-NEXT:     unsafe { {} }
// CHECK-NEXT: }
// CHECK-NEXT: fn c23_switch_fallthrough(value: i32) -> i32 {
// CHECK-NEXT:     let mut result: i32 = 0;
// CHECK-NEXT:     match value {
// CHECK-NEXT:         1 => {
// CHECK-NEXT:             result += 3;
// CHECK-NEXT:             {}
// CHECK-NEXT:             let [[V_1:_v[0-9]+]]: i32 = result;
// CHECK-NEXT:             let [[V_2:_v[0-9]+]]: i32 = [[V_1]] + 5;
// CHECK-NEXT:             result = [[V_2]];
// CHECK-NEXT:         }
// CHECK-NEXT:         2 => {
// CHECK-NEXT:             result += 5;
// CHECK-NEXT:         }
// CHECK-NEXT:         _ => {}
// CHECK-NEXT:     }
// CHECK-NEXT:     result
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C-unwind" fn c23_relaxed_variadic(mut __va_args: ...) -> i32 {
// CHECK-NEXT:     let mut arguments: core::ffi::VaList<'static> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut first: i32 = 0;
// CHECK-NEXT:     let mut second: i32 = 0;
// CHECK-NEXT:     arguments = unsafe {
// CHECK-NEXT:         core::mem::transmute::<
// CHECK-NEXT:             core::ffi::VaList<'_>,
// CHECK-NEXT:             core::ffi::VaList<'static>,
// CHECK-NEXT:         >(__va_args.clone())
// CHECK-NEXT:     };
// CHECK-NEXT:     first = unsafe { arguments.next_arg::<i32>() };
// CHECK-NEXT:     second = unsafe { arguments.next_arg::<i32>() };
// CHECK-NEXT:     {}
// CHECK-NEXT:     first + second
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let local_constant: i32 = 61;
// CHECK-NEXT:     let aligned_value: __SlateAlign32<i32> = __SlateAlign32(3 as i32);
// CHECK-NEXT:     let inferred_value: i32 = 67;
// CHECK-NEXT:     let same_type_value: i32 = 71;
// CHECK-NEXT:     let unqualified_value: i32 = 79;
// CHECK-NEXT:     let α: i32 = 5;
// CHECK-NEXT:     let signed_precise: i32 = (-12345 as i32) << 15 >> 15;
// CHECK-NEXT:     let unsigned_precise: u32 = 100000;
// CHECK-NEXT:     let binary_value: i32 = 165;
// CHECK-NEXT:     let utf8_character: u8 = 90;
// CHECK-NEXT:     let empty_struct: C23Empty = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: C23Empty = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let empty_array: [i32; 3] = {
// CHECK-NEXT:         let mut [[T_1:__t[0-9]+]]: [i32; 3] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_1]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let qualified_array: [i32; 3] = [2, 3, 5];
// CHECK-NEXT:     let fixed_value: u16 = 37;
// CHECK-NEXT:     let wide_value: u64 = 8589934591;
// CHECK-NEXT:     let null_value: *mut () = std::ptr::null_mut::<()>();
// CHECK-NEXT:     let null_pointer: *mut i32 = std::ptr::null_mut::<i32>();
// CHECK-NEXT:     let boolean_value: bool = true as bool;
// CHECK-NEXT:     let false_value: bool = false as bool;
// CHECK-NEXT:     let static_compound_value: i32 = 83;
// CHECK-NEXT:     let language_total: i32 = (((((unsafe { c23_file_constant }) + local_constant
// CHECK-NEXT:         + inferred_value + same_type_value + unqualified_value + (signed_precise as i32)
// CHECK-NEXT:         + ((unsigned_precise as u32) as i32) + binary_value + aligned_value.0) as i64)
// CHECK-NEXT:         as u64)
// CHECK-NEXT:         .wrapping_add(4)
// CHECK-NEXT:         .wrapping_add((α as i64) as u64)
// CHECK-NEXT:         .wrapping_add((((utf8_character as u32) as i32) as i64) as u64)
// CHECK-NEXT:         .wrapping_add(((((unsafe { utf8_text[0] }) as u32) as i32) as i64) as u64)
// CHECK-NEXT:         .wrapping_add(((((unsafe { utf8_text[1] }) as u32) as i32) as i64) as u64)
// CHECK-NEXT:         .wrapping_add((empty_struct.first as i64) as u64)
// CHECK-NEXT:         .wrapping_add((empty_array[0] as i64) as u64)
// CHECK-NEXT:         .wrapping_add((qualified_array[0] as i64) as u64)
// CHECK-NEXT:         .wrapping_add((qualified_array[2] as i64) as u64)
// CHECK-NEXT:         .wrapping_add(((((fixed_value as u16) as u32) as i32) as i64) as u64)
// CHECK-NEXT:         .wrapping_add(((((wide_value as u64) == 8589934591) as i32) as i64) as u64)
// CHECK-NEXT:         .wrapping_add((static_compound_value as i64) as u64)
// CHECK-NEXT:         .wrapping_add(7)
// CHECK-NEXT:         .wrapping_add((((11 as i32) + (13 as i32)) as i64) as u64)
// CHECK-NEXT:         .wrapping_add(1)
// CHECK-NEXT:         .wrapping_add(2)
// CHECK-NEXT:         .wrapping_add(3)
// CHECK-NEXT:         .wrapping_add(4) as u32) as i32;
// CHECK-NEXT:     let attribute_total: i32 = c23_nodiscard_value();
// CHECK-NEXT:     let preprocessor_total: i32 = 23 + 29 + 1 + 1 + 0 + 0 + 0 + 0
// CHECK-NEXT:         + (((unsafe { c23_embedded[0] }) as u32) as i32)
// CHECK-NEXT:         + (((unsafe { c23_embedded[1] }) as u32) as i32)
// CHECK-NEXT:         + (((unsafe { c23_embedded[2] }) as u32) as i32);
// CHECK-NEXT:     let type_total: i32 = ((null_value == std::ptr::null_mut::<()>()) as i32)
// CHECK-NEXT:         + ((null_pointer == std::ptr::null_mut::<i32>()) as i32) + (boolean_value as i32)
// CHECK-NEXT:         + (!false_value as i32) + unsafe { c23_thread_value };
// CHECK-NEXT:     let control_total: i32 = c23_unnamed_parameter(89 as i32, 97 as i32)
// CHECK-NEXT:         + c23_label_declaration(101 as i32) + c23_switch_fallthrough(1 as i32)
// CHECK-NEXT:         + unsafe { c23_relaxed_variadic(103 as i32, 107 as i32) };
// CHECK-NEXT:     let removal_total: i32 = ((202311 as i64) == (202311 as i64)) as i32;
// CHECK-NEXT:     if (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(c23_never_flag)) }) != 0 {
// CHECK-NEXT:         c23_never_return();
// CHECK-NEXT:     }
// CHECK-NEXT:     c23_label_before_brace();
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {} {} {} {}", language_total, attribute_total, preprocessor_total,
// CHECK-NEXT:         type_total, control_total, removal_total
// CHECK-NEXT:     );
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: static mut utf8_text: [u8; 3] = [206 as u8, 169 as u8, 0 as u8];
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
