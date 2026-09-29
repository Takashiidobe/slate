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


// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-unknown-linux-gnu" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f80;
// DEFAULT-NEXT:         storage bool [size=1, align=1];
// DEFAULT-NEXT:         storage i8, u8 [size=1, align=1];
// DEFAULT-NEXT:         storage i16, u16 [size=2, align=2];
// DEFAULT-NEXT:         storage i32, u32 [size=4, align=4];
// DEFAULT-NEXT:         storage i64, u64 [size=8, align=8];
// DEFAULT-NEXT:         storage i128, u128 [size=16, align=16];
// DEFAULT-NEXT:         storage bf16 [size=2, align=2];
// DEFAULT-NEXT:         storage f16 [size=2, align=2];
// DEFAULT-NEXT:         storage f32 [size=4, align=4];
// DEFAULT-NEXT:         storage f64 [size=8, align=8];
// DEFAULT-NEXT:         storage f80 [size=16, align=16];
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     type @type[[TYPE___gnuc_va_list:[0-9]+]] __gnuc_va_list = va_list;
// DEFAULT-NEXT:     type @type[[TYPE_va_list:[0-9]+]] va_list = va_list;
// DEFAULT-NEXT:     type @type[[TYPE_nullptr_t:[0-9]+]] nullptr_t = ptr<void>;
// DEFAULT-NEXT:     type @type[[TYPE_va_list_2:[0-9]+]] va_list = va_list;
// DEFAULT-NEXT:     type @type[[TYPE_char8_t:[0-9]+]] char8_t = u8;
// DEFAULT-NEXT:     type @type[[TYPE_C23Fixed:[0-9]+]] C23Fixed = enum : u16 {
// DEFAULT-NEXT:         %[[VALUE_C23_FIXED_FIRST:[0-9]+]] C23_FIXED_FIRST = const<@type[[TYPE_C23Fixed]]>(31);
// DEFAULT-NEXT:         %[[VALUE_C23_FIXED_SECOND:[0-9]+]] C23_FIXED_SECOND = const<@type[[TYPE_C23Fixed]]>(37);
// DEFAULT-NEXT:     } [size=2, align=2];
// DEFAULT-NEXT:     type @type[[TYPE_C23Wide:[0-9]+]] C23Wide = enum : u64 {
// DEFAULT-NEXT:         %[[VALUE_C23_FIXED_FIRST]] C23_WIDE_VALUE = const<@type[[TYPE_C23Wide]]>(8589934591);
// DEFAULT-NEXT:     } [size=8, align=8];
// DEFAULT-NEXT:     type @type[[TYPE_C23Empty:[0-9]+]] C23Empty = struct {
// DEFAULT-NEXT:         field0 first: i32;
// DEFAULT-NEXT:         field1 second: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_C23Array:[0-9]+]] C23Array = array<i32, 3>;
// DEFAULT-NEXT:     global %[[VALUE_c23_embedded:[0-9]+]] c23_embedded: array<u8, 3> [storage=static] [const] = aggregate<array<u8, 3>, zero_fill=false>(index0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(67))), index1 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(50))), index2 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(51)))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_c23_deprecated_value:[0-9]+]] c23_deprecated_value: i32 [storage=static] = const<i32>(41) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_c23_maybe_unused_value:[0-9]+]] c23_maybe_unused_value: i32 [storage=static] = const<i32>(43) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_c23_thread_value:[0-9]+]] c23_thread_value: i32 [storage=thread] = const<i32>(53) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_c23_never_flag:[0-9]+]] c23_never_flag: volatile i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_c23_file_constant:[0-9]+]] c23_file_constant: i32 [storage=static] [const] [constexpr] = const<i32>(59) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_utf8_text:[0-9]+]] utf8_text: array<u8, 3> [storage=static] [const] = code_units<array<u8, 3>>([206, 169, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 19> [storage=static] = code_units<array<i8, 19>>([37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE___status:[0-9]+]] __status: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_c23_nodiscard_value:[0-9]+]] @c23_nodiscard_value() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(47);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_c23_never_return:[0-9]+]] @c23_never_return() -> void [linkage=internal] [noreturn] [fallthrough=ub] {
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(99));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_c23_unnamed_parameter:[0-9]+]] @c23_unnamed_parameter(%[[VALUE0:[0-9]+]] <unnamed>: i32, %[[VALUE_value:[0-9]+]] value: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_value]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_c23_label_declaration:[0-9]+]] @c23_label_declaration(%[[VALUE_value_2:[0-9]+]] value: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         goto %[[VALUE_c23_label:[0-9]+]];
// DEFAULT-NEXT:         label %[[VALUE_c23_label]] c23_label:
// DEFAULT-NEXT:             let %[[VALUE_result:[0-9]+]] result: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%[[VALUE_value_2]]), const<i32>(1));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_result]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_c23_label_before_brace:[0-9]+]] @c23_label_before_brace() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         goto %[[VALUE_c23_end:[0-9]+]];
// DEFAULT-NEXT:         label %[[VALUE_c23_end]] c23_end:
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_c23_switch_fallthrough:[0-9]+]] @c23_switch_fallthrough(%[[VALUE_value_3:[0-9]+]] value: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_result_2:[0-9]+]] result: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         switch %[[VALUE1:[0-9]+]] read<i32>(%[[VALUE_value_3]])
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(1):
// DEFAULT-NEXT:                     let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_result_2]]);
// DEFAULT-NEXT:                     let %[[VALUE3:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE2]]), const<i32>(3));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_result_2]], read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:                 case %[[VALUE1]] const<i32>(2):
// DEFAULT-NEXT:                     let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_result_2]]);
// DEFAULT-NEXT:                     let %[[VALUE5:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE4]]), const<i32>(5));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_result_2]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:                 break %[[VALUE1]];
// DEFAULT-NEXT:                 default %[[VALUE1]]:
// DEFAULT-NEXT:                     break %[[VALUE1]];
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_result_2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_c23_relaxed_variadic:[0-9]+]] @c23_relaxed_variadic(...) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_arguments:[0-9]+]] arguments: va_list [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_first:[0-9]+]] first: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_second:[0-9]+]] second: i32 [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_arguments]]);
// DEFAULT-NEXT:         write<i32>(%[[VALUE_first]], va_arg<i32>(%[[VALUE_arguments]]));
// DEFAULT-NEXT:         va_arg<i32>(%[[VALUE_arguments]]);
// DEFAULT-NEXT:         write<i32>(%[[VALUE_second]], va_arg<i32>(%[[VALUE_arguments]]));
// DEFAULT-NEXT:         va_arg<i32>(%[[VALUE_arguments]]);
// DEFAULT-NEXT:         va_end(%[[VALUE_arguments]]);
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%[[VALUE_first]]), read<i32>(%[[VALUE_second]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_local_constant:[0-9]+]] local_constant: i32 [storage=automatic] [const] [constexpr] = const<i32>(61);
// DEFAULT-NEXT:         let %[[VALUE_aligned_value:[0-9]+]] aligned_value: i32 [storage=automatic] [align=32] = const<i32>(3);
// DEFAULT-NEXT:         let %[[VALUE_inferred_value:[0-9]+]] inferred_value: i32 [storage=automatic] = const<i32>(67);
// DEFAULT-NEXT:         let %[[VALUE_same_type_value:[0-9]+]] same_type_value: i32 [storage=automatic] = const<i32>(71);
// DEFAULT-NEXT:         let %[[VALUE_qualified_value:[0-9]+]] qualified_value: i32 [storage=automatic] [const] = const<i32>(73);
// DEFAULT-NEXT:         let %[[VALUE_unqualified_value:[0-9]+]] unqualified_value: i32 [storage=automatic] = const<i32>(79);
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]] \u03b1: i32 [storage=automatic] = const<i32>(5);
// DEFAULT-NEXT:         let %[[VALUE_signed_precise:[0-9]+]] signed_precise: i17b [storage=automatic] = truncate<i17b, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(12345)));
// DEFAULT-NEXT:         let %[[VALUE_unsigned_precise:[0-9]+]] unsigned_precise: u17b [storage=automatic] = const<u17b>(100000);
// DEFAULT-NEXT:         let %[[VALUE_binary_value:[0-9]+]] binary_value: i32 [storage=automatic] = const<i32>(165);
// DEFAULT-NEXT:         let %[[VALUE_utf8_character:[0-9]+]] utf8_character: u8 [storage=automatic] = const<u8>(90);
// DEFAULT-NEXT:         let %[[VALUE_empty_struct:[0-9]+]] empty_struct: @type[[TYPE_C23Empty]] [storage=automatic] = aggregate<@type[[TYPE_C23Empty]], zero_fill=true>();
// DEFAULT-NEXT:         let %[[VALUE_empty_array:[0-9]+]] empty_array: array<i32, 3> [storage=automatic] = aggregate<array<i32, 3>, zero_fill=true>();
// DEFAULT-NEXT:         let %[[VALUE_qualified_array:[0-9]+]] qualified_array: array<i32, 3> [storage=automatic] [const] = aggregate<array<i32, 3>, zero_fill=false>(index0 = const<i32>(2), index1 = const<i32>(3), index2 = const<i32>(5));
// DEFAULT-NEXT:         let %[[VALUE_fixed_value:[0-9]+]] fixed_value: @type[[TYPE_C23Fixed]] [storage=automatic] = const<@type[[TYPE_C23Fixed]]>(37);
// DEFAULT-NEXT:         let %[[VALUE_wide_value:[0-9]+]] wide_value: @type[[TYPE_C23Wide]] [storage=automatic] = const<@type[[TYPE_C23Wide]]>(8589934591);
// DEFAULT-NEXT:         let %[[VALUE_null_value:[0-9]+]] null_value: ptr<void> [storage=automatic] = null<ptr<void>>;
// DEFAULT-NEXT:         let %[[VALUE_null_pointer:[0-9]+]] null_pointer: ptr<i32> [storage=automatic] = null<ptr<i32>>;
// DEFAULT-NEXT:         let %[[VALUE_boolean_value:[0-9]+]] boolean_value: bool [storage=automatic] = const<bool>(true);
// DEFAULT-NEXT:         let %[[VALUE_false_value:[0-9]+]] false_value: bool [storage=automatic] = const<bool>(false);
// DEFAULT-NEXT:         let %[[VALUE_static_compound_value:[0-9]+]] static_compound_value: i32 [storage=automatic] = const<i32>(83);
// DEFAULT-NEXT:         let %[[VALUE_language_total:[0-9]+]] language_total: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_attribute_total:[0-9]+]] attribute_total: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_preprocessor_total:[0-9]+]] preprocessor_total: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_type_total:[0-9]+]] type_total: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_control_total:[0-9]+]] control_total: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_removal_total:[0-9]+]] removal_total: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%[[VALUE_language_total]], reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(%[[VALUE_c23_file_constant]]), read<i32>(%[[VALUE_local_constant]])), read<i32>(%[[VALUE_inferred_value]])), read<i32>(%[[VALUE_same_type_value]])), read<i32>(%[[VALUE_unqualified_value]])), widen<i32, reason=explicit>(read<i17b>(%[[VALUE_signed_precise]]))), reinterpret<i32, reason=explicit, fits=unknown>(widen<u32, reason=explicit>(read<u17b>(%[[VALUE_unsigned_precise]])))), read<i32>(%[[VALUE_binary_value]])), read<i32>(%[[VALUE_aligned_value]])))), const<u64>(4)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE6]])))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_utf8_character]])))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<const u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<const u8>, length=Some(3)>(%[[VALUE_utf8_text]]), const<i32>(0))))))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<const u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<const u8>, length=Some(3)>(%[[VALUE_utf8_text]]), const<i32>(1))))))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(field0(%[[VALUE_empty_struct]]))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(%[[VALUE_empty_array]]), const<i32>(0))))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(deref(ptr_offset<ptr<const i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<const i32>, length=Some(3)>(%[[VALUE_qualified_array]]), const<i32>(0))))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(deref(ptr_offset<ptr<const i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<const i32>, length=Some(3)>(%[[VALUE_qualified_array]]), const<i32>(2))))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(enum_to_int<u16, reason=promotion>(read<@type[[TYPE_C23Fixed]]>(%[[VALUE_fixed_value]]))))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(from_bool<i32, reason=explicit>(eq<u64>(enum_to_int<u64, reason=promotion>(read<@type[[TYPE_C23Wide]]>(%[[VALUE_wide_value]])), enum_to_int<u64, reason=promotion>(const<@type[[TYPE_C23Wide]]>(8589934591))))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_static_compound_value]])))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(7)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(add<i32, overflow=ub>(const<i32>(11), const<i32>(13))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_attribute_total]], call<i32, signature=fn() -> i32>(%[[VALUE_c23_nodiscard_value]]));
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%[[VALUE_c23_nodiscard_value]]);
// DEFAULT-NEXT:         write<i32>(%[[VALUE_preprocessor_total]], add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(const<i32>(23), const<i32>(29)), const<i32>(1)), const<i32>(1)), const<i32>(0)), const<i32>(0)), const<i32>(0)), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<const u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<const u8>, length=Some(3)>(%[[VALUE_c23_embedded]]), const<i32>(0))))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<const u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<const u8>, length=Some(3)>(%[[VALUE_c23_embedded]]), const<i32>(1))))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<const u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<const u8>, length=Some(3)>(%[[VALUE_c23_embedded]]), const<i32>(2))))))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_type_total]], add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(eq<ptr<void>>(read<ptr<void>>(%[[VALUE_null_value]]), null<ptr<void>>)), from_bool<i32, reason=promotion>(eq<ptr<i32>>(read<ptr<i32>>(%[[VALUE_null_pointer]]), null<ptr<i32>>))), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_boolean_value]]))), from_bool<i32, reason=promotion>(not<bool>(read<bool>(%[[VALUE_false_value]])))), read<i32>(%[[VALUE_c23_thread_value]])));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_control_total]], add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(call<i32, signature=fn(i32, i32) -> i32>(%[[VALUE_c23_unnamed_parameter]], const<i32>(89), const<i32>(97)), call<i32, signature=fn(i32) -> i32>(%[[VALUE_c23_label_declaration]], const<i32>(101))), call<i32, signature=fn(i32) -> i32>(%[[VALUE_c23_switch_fallthrough]], const<i32>(1))), call<i32, signature=fn(...) -> i32>(%[[VALUE_c23_relaxed_variadic]], const<i32>(103), const<i32>(107))));
// DEFAULT-NEXT:         add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(call<i32, signature=fn(i32, i32) -> i32>(%[[VALUE_c23_unnamed_parameter]], const<i32>(89), const<i32>(97)), call<i32, signature=fn(i32) -> i32>(%[[VALUE_c23_label_declaration]], const<i32>(101))), call<i32, signature=fn(i32) -> i32>(%[[VALUE_c23_switch_fallthrough]], const<i32>(1))), call<i32, signature=fn(...) -> i32>(%[[VALUE_c23_relaxed_variadic]], const<i32>(103), const<i32>(107)));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_removal_total]], from_bool<i32, reason=assign>(eq<i64>(const<i64>(202311), const<i64>(202311))));
// DEFAULT-NEXT:         if ne<i32>(read<i32, volatile>(%[[VALUE_c23_never_flag]]), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_c23_never_return]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_c23_label_before_brace]]);
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(19)>(%[[VALUE_str]])), read<i32>(%[[VALUE_language_total]]), read<i32>(%[[VALUE_attribute_total]]), read<i32>(%[[VALUE_preprocessor_total]]), read<i32>(%[[VALUE_type_total]]), read<i32>(%[[VALUE_control_total]]), read<i32>(%[[VALUE_removal_total]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
