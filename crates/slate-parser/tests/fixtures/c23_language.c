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
// DEFAULT-NEXT:     type @type0 __gnuc_va_list = va_list;
// DEFAULT-NEXT:     type @type1 va_list = va_list;
// DEFAULT-NEXT:     type @type2 nullptr_t = ptr<void>;
// DEFAULT-NEXT:     type @type3 va_list = va_list;
// DEFAULT-NEXT:     type @type4 char8_t = u8;
// DEFAULT-NEXT:     type @type5 C23Fixed = enum : u16 {
// DEFAULT-NEXT:         %0 C23_FIXED_FIRST = const<@type5>(31);
// DEFAULT-NEXT:         %1 C23_FIXED_SECOND = const<@type5>(37);
// DEFAULT-NEXT:     } [size=2, align=2];
// DEFAULT-NEXT:     type @type6 C23Wide = enum : u64 {
// DEFAULT-NEXT:         %0 C23_WIDE_VALUE = const<@type6>(8589934591);
// DEFAULT-NEXT:     } [size=8, align=8];
// DEFAULT-NEXT:     type @type7 C23Empty = struct {
// DEFAULT-NEXT:         field0 first: i32;
// DEFAULT-NEXT:         field1 second: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type8 C23Array = array<i32, 3>;
// DEFAULT-NEXT:     global %6 c23_embedded: array<u8, 3> [storage=static] [const] = aggregate<array<u8, 3>, zero_fill=false>(index0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(67))), index1 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(50))), index2 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(51)))) [linkage=internal];
// DEFAULT-NEXT:     global %14 c23_deprecated_value: i32 [storage=static] = const<i32>(41) [linkage=internal];
// DEFAULT-NEXT:     global %15 c23_maybe_unused_value: i32 [storage=static] = const<i32>(43) [linkage=internal];
// DEFAULT-NEXT:     global %18 c23_thread_value: i32 [storage=thread] = const<i32>(53) [linkage=internal];
// DEFAULT-NEXT:     global %19 c23_never_flag: volatile i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %20 c23_file_constant: i32 [storage=static] [const] [constexpr] = const<i32>(59) [linkage=internal];
// DEFAULT-NEXT:     global %48 utf8_text: array<u8, 3> [storage=static] [const] = code_units<array<u8, 3>>([206, 169, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %69 .str69: array<i8, 19> [storage=static] = code_units<array<i8, 19>>([37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %3 @printf(%65 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %4 @exit(%66 __status: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %16 @c23_nodiscard_value() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(47);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @c23_never_return() -> void [linkage=internal] [noreturn] [fallthrough=ub] {
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%4, const<i32>(99));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %21 @c23_unnamed_parameter(%67 <unnamed>: i32, %22 value: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(%22);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %23 @c23_label_declaration(%25 value: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         goto %24;
// DEFAULT-NEXT:         label %24 c23_label:
// DEFAULT-NEXT:             let %26 result: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%25), const<i32>(1));
// DEFAULT-NEXT:         return read<i32>(%26);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %27 @c23_label_before_brace() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         goto %28;
// DEFAULT-NEXT:         label %28 c23_end:
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %29 @c23_switch_fallthrough(%30 value: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %31 result: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         switch %68 read<i32>(%30)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %68 const<i32>(1):
// DEFAULT-NEXT:                     let %70: i32 [synthetic] = read<i32>(%31);
// DEFAULT-NEXT:                     let %71: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%70), const<i32>(3));
// DEFAULT-NEXT:                     write<i32>(%31, read<i32>(%71));
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:                 case %68 const<i32>(2):
// DEFAULT-NEXT:                     let %72: i32 [synthetic] = read<i32>(%31);
// DEFAULT-NEXT:                     let %73: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%72), const<i32>(5));
// DEFAULT-NEXT:                     write<i32>(%31, read<i32>(%73));
// DEFAULT-NEXT:                 break %68;
// DEFAULT-NEXT:                 default %68:
// DEFAULT-NEXT:                     break %68;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return read<i32>(%31);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %32 @c23_relaxed_variadic(...) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %33 arguments: va_list [storage=automatic];
// DEFAULT-NEXT:         let %34 first: i32 [storage=automatic];
// DEFAULT-NEXT:         let %35 second: i32 [storage=automatic];
// DEFAULT-NEXT:         va_start(%33);
// DEFAULT-NEXT:         write<i32>(%34, va_arg<i32>(%33));
// DEFAULT-NEXT:         va_arg<i32>(%33);
// DEFAULT-NEXT:         write<i32>(%35, va_arg<i32>(%33));
// DEFAULT-NEXT:         va_arg<i32>(%33);
// DEFAULT-NEXT:         va_end(%33);
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%34), read<i32>(%35));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %36 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %37 local_constant: i32 [storage=automatic] [const] [constexpr] = const<i32>(61);
// DEFAULT-NEXT:         let %38 aligned_value: i32 [storage=automatic] [align=32] = const<i32>(3);
// DEFAULT-NEXT:         let %39 inferred_value: i32 [storage=automatic] = const<i32>(67);
// DEFAULT-NEXT:         let %40 same_type_value: i32 [storage=automatic] = const<i32>(71);
// DEFAULT-NEXT:         let %41 qualified_value: i32 [storage=automatic] [const] = const<i32>(73);
// DEFAULT-NEXT:         let %42 unqualified_value: i32 [storage=automatic] = const<i32>(79);
// DEFAULT-NEXT:         let %43 \u03b1: i32 [storage=automatic] = const<i32>(5);
// DEFAULT-NEXT:         let %44 signed_precise: i17b [storage=automatic] = truncate<i17b, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(12345)));
// DEFAULT-NEXT:         let %45 unsigned_precise: u17b [storage=automatic] = const<u17b>(100000);
// DEFAULT-NEXT:         let %46 binary_value: i32 [storage=automatic] = const<i32>(165);
// DEFAULT-NEXT:         let %47 utf8_character: u8 [storage=automatic] = const<u8>(90);
// DEFAULT-NEXT:         let %49 empty_struct: @type7 [storage=automatic] = aggregate<@type7, zero_fill=true>();
// DEFAULT-NEXT:         let %50 empty_array: array<i32, 3> [storage=automatic] = aggregate<array<i32, 3>, zero_fill=true>();
// DEFAULT-NEXT:         let %51 qualified_array: array<i32, 3> [storage=automatic] [const] = aggregate<array<i32, 3>, zero_fill=false>(index0 = const<i32>(2), index1 = const<i32>(3), index2 = const<i32>(5));
// DEFAULT-NEXT:         let %52 fixed_value: @type5 [storage=automatic] = const<@type5>(37);
// DEFAULT-NEXT:         let %53 wide_value: @type6 [storage=automatic] = const<@type6>(8589934591);
// DEFAULT-NEXT:         let %54 null_value: ptr<void> [storage=automatic] = null<ptr<void>>;
// DEFAULT-NEXT:         let %55 null_pointer: ptr<i32> [storage=automatic] = null<ptr<i32>>;
// DEFAULT-NEXT:         let %56 boolean_value: bool [storage=automatic] = const<bool>(true);
// DEFAULT-NEXT:         let %57 false_value: bool [storage=automatic] = const<bool>(false);
// DEFAULT-NEXT:         let %58 static_compound_value: i32 [storage=automatic] = const<i32>(83);
// DEFAULT-NEXT:         let %59 language_total: i32 [storage=automatic];
// DEFAULT-NEXT:         let %60 attribute_total: i32 [storage=automatic];
// DEFAULT-NEXT:         let %61 preprocessor_total: i32 [storage=automatic];
// DEFAULT-NEXT:         let %62 type_total: i32 [storage=automatic];
// DEFAULT-NEXT:         let %63 control_total: i32 [storage=automatic];
// DEFAULT-NEXT:         let %64 removal_total: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%59, reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(%20), read<i32>(%37)), read<i32>(%39)), read<i32>(%40)), read<i32>(%42)), widen<i32, reason=explicit>(read<i17b>(%44))), reinterpret<i32, reason=explicit, fits=unknown>(widen<u32, reason=explicit>(read<u17b>(%45)))), read<i32>(%46)), read<i32>(%38)))), const<u64>(4)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%43)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%47)))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<const u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<const u8>, length=Some(3)>(%48), const<i32>(0))))))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<const u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<const u8>, length=Some(3)>(%48), const<i32>(1))))))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(field0(%49))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(%50), const<i32>(0))))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(deref(ptr_offset<ptr<const i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<const i32>, length=Some(3)>(%51), const<i32>(0))))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(deref(ptr_offset<ptr<const i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<const i32>, length=Some(3)>(%51), const<i32>(2))))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(enum_to_int<u16, reason=promotion>(read<@type5>(%52))))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(from_bool<i32, reason=explicit>(eq<u64>(enum_to_int<u64, reason=promotion>(read<@type6>(%53)), enum_to_int<u64, reason=promotion>(const<@type6>(8589934591))))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%58)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(7)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(add<i32, overflow=ub>(const<i32>(11), const<i32>(13))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))));
// DEFAULT-NEXT:         write<i32>(%60, call<i32, signature=fn() -> i32>(%16));
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%16);
// DEFAULT-NEXT:         write<i32>(%61, add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(const<i32>(23), const<i32>(29)), const<i32>(1)), const<i32>(1)), const<i32>(0)), const<i32>(0)), const<i32>(0)), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<const u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<const u8>, length=Some(3)>(%6), const<i32>(0))))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<const u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<const u8>, length=Some(3)>(%6), const<i32>(1))))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<const u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<const u8>, length=Some(3)>(%6), const<i32>(2))))))));
// DEFAULT-NEXT:         write<i32>(%62, add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(eq<ptr<void>>(read<ptr<void>>(%54), null<ptr<void>>)), from_bool<i32, reason=promotion>(eq<ptr<i32>>(read<ptr<i32>>(%55), null<ptr<i32>>))), from_bool<i32, reason=promotion>(read<bool>(%56))), from_bool<i32, reason=promotion>(not<bool>(read<bool>(%57)))), read<i32>(%18)));
// DEFAULT-NEXT:         write<i32>(%63, add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(call<i32, signature=fn(i32, i32) -> i32>(%21, const<i32>(89), const<i32>(97)), call<i32, signature=fn(i32) -> i32>(%23, const<i32>(101))), call<i32, signature=fn(i32) -> i32>(%29, const<i32>(1))), call<i32, signature=fn(...) -> i32>(%32, const<i32>(103), const<i32>(107))));
// DEFAULT-NEXT:         add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(call<i32, signature=fn(i32, i32) -> i32>(%21, const<i32>(89), const<i32>(97)), call<i32, signature=fn(i32) -> i32>(%23, const<i32>(101))), call<i32, signature=fn(i32) -> i32>(%29, const<i32>(1))), call<i32, signature=fn(...) -> i32>(%32, const<i32>(103), const<i32>(107)));
// DEFAULT-NEXT:         write<i32>(%64, from_bool<i32, reason=assign>(eq<i64>(const<i64>(202311), const<i64>(202311))));
// DEFAULT-NEXT:         if ne<i32>(read<i32, volatile>(%19), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%17);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%27);
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%3, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(19)>(%69)), read<i32>(%59), read<i32>(%60), read<i32>(%61), read<i32>(%62), read<i32>(%63), read<i32>(%64));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
