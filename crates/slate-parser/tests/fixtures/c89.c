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
// DEFAULT-NEXT:     type @type2 wchar_t = i32;
// DEFAULT-NEXT:     type @type3 va_list = va_list;
// DEFAULT-NEXT:     type @type4 C89Color = enum : u32 {
// DEFAULT-NEXT:         %0 C89_RED = const<i32>(3);
// DEFAULT-NEXT:         %1 C89_GREEN = const<i32>(5);
// DEFAULT-NEXT:         %2 C89_BLUE = const<i32>(7);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type5 C89Point = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:         field1 y: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type6 C89Bits = struct {
// DEFAULT-NEXT:         field0 low: u32 : 3;
// DEFAULT-NEXT:         field1 high: i32 : 4;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0], bit_offsets=[Some(0), Some(3)], bit_units=[(0, 1)], field_units=[Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type7 C89Number = union {
// DEFAULT-NEXT:         field0 integer: i32;
// DEFAULT-NEXT:         field1 real: f64;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type8 C89SignedInt = i32;
// DEFAULT-NEXT:     type @type9 C89BinaryOperation = ptr<fn(i32, i32) -> i32>;
// DEFAULT-NEXT:     global %13 c89_external_value: i32 [storage=static] = const<i32>(11) [linkage=external];
// DEFAULT-NEXT:     global %14 c89_const_global: i32 [storage=static] [const] = const<i32>(17) [linkage=internal];
// DEFAULT-NEXT:     global %15 c89_volatile_global: volatile i32 [storage=static] = const<i32>(19) [linkage=internal];
// DEFAULT-NEXT:     global %16 c89_joined_value: i32 [storage=static] = const<i32>(23) [linkage=internal];
// DEFAULT-NEXT:     global %31 calls: i32 [storage=static] = const<i32>(0) [linkage=internal];
// DEFAULT-NEXT:     global %76 .str76: array<i8, 88> [storage=static] = code_units<array<i8, 88>>([37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %3 @printf(%67 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %17 @c89_add(%19 left: i32, %20 right: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%19), read<i32>(%20));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %18 @c89_store(%21 destination: ptr<void>, %22 source: ptr<const void>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %23 output: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         let %24 input: ptr<const i32> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<i32>>(%23, pointer_cast<ptr<i32>, reason=explicit>(read<ptr<void>>(%21)));
// DEFAULT-NEXT:         write<ptr<const i32>>(%24, pointer_cast<ptr<const i32>, reason=explicit>(read<ptr<const void>>(%22)));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%23)), read<i32>(deref(read<ptr<const i32>>(%24))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %25 @c89_variadic_sum(%26 count: i32, ...) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %27 arguments: va_list [storage=automatic];
// DEFAULT-NEXT:         let %28 index: i32 [storage=automatic];
// DEFAULT-NEXT:         let %29 total: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%29, const<i32>(0));
// DEFAULT-NEXT:         va_start(%27);
// DEFAULT-NEXT:         for %72
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%28, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%28), read<i32>(%26))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %77: i32 [synthetic] = read<i32>(%28);
// DEFAULT-NEXT:                 let %78: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%77), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%28, read<i32>(%78));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %79: i32 [synthetic] = read<i32>(%29);
// DEFAULT-NEXT:                     let %80: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%79), va_arg<i32>(%27));
// DEFAULT-NEXT:                     write<i32>(%29, read<i32>(%80));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         va_end(%27);
// DEFAULT-NEXT:         return read<i32>(%29);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %30 @c89_static_local() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %81: i32 [synthetic] = read<i32>(%31);
// DEFAULT-NEXT:         let %82: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%81), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%31, read<i32>(%82));
// DEFAULT-NEXT:         return read<i32>(%31);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %32 @c89_control_flow(%34 value: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %35 result: i32 [storage=automatic];
// DEFAULT-NEXT:         let %36 index: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%35, const<i32>(0));
// DEFAULT-NEXT:         write<i32>(%36, const<i32>(0));
// DEFAULT-NEXT:         while %73 lt<i32>(read<i32>(%36), read<i32>(%34))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if eq<i32>(read<i32>(%36), const<i32>(1))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %83: i32 [synthetic] = read<i32>(%36);
// DEFAULT-NEXT:                         let %84: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%83), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%36, read<i32>(%84));
// DEFAULT-NEXT:                         continue %73;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 let %85: i32 [synthetic] = read<i32>(%35);
// DEFAULT-NEXT:                 let %86: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%85), read<i32>(%36));
// DEFAULT-NEXT:                 write<i32>(%35, read<i32>(%86));
// DEFAULT-NEXT:                 if gt<i32>(read<i32>(%35), const<i32>(20))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         break %73;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 let %87: i32 [synthetic] = read<i32>(%36);
// DEFAULT-NEXT:                 let %88: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%87), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%36, read<i32>(%88));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         do %74
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %89: i32 [synthetic] = read<i32>(%35);
// DEFAULT-NEXT:                 let %90: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%89), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%35, read<i32>(%90));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while gt<i32>(read<i32>(%35), const<i32>(6));
// DEFAULT-NEXT:         switch %75 read<i32>(%34)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %75 const<i32>(4):
// DEFAULT-NEXT:                     let %91: i32 [synthetic] = read<i32>(%35);
// DEFAULT-NEXT:                     let %92: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%91), const<i32>(10));
// DEFAULT-NEXT:                     write<i32>(%35, read<i32>(%92));
// DEFAULT-NEXT:                 break %75;
// DEFAULT-NEXT:                 default %75:
// DEFAULT-NEXT:                     write<i32>(%35, const<i32>(0));
// DEFAULT-NEXT:                 break %75;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%35), const<i32>(16))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 goto %33;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         write<i32>(%35, neg<i32, overflow=ub>(const<i32>(1)));
// DEFAULT-NEXT:         label %33 c89_done:
// DEFAULT-NEXT:             return read<i32>(%35);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %37 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %38 automatic_value: i32 [storage=automatic];
// DEFAULT-NEXT:         let %39 register_value: i32 [storage=automatic];
// DEFAULT-NEXT:         let %40 signed_value: i32 [storage=automatic];
// DEFAULT-NEXT:         let %41 unsigned_value: u64 [storage=automatic];
// DEFAULT-NEXT:         let %42 signed_character: i8 [storage=automatic];
// DEFAULT-NEXT:         let %43 float_value: f32 [storage=automatic];
// DEFAULT-NEXT:         let %44 double_value: f64 [storage=automatic];
// DEFAULT-NEXT:         let %45 long_double_value: f80 [storage=automatic];
// DEFAULT-NEXT:         let %46 wide_character: i32 [storage=automatic];
// DEFAULT-NEXT:         let %47 color: @type4 [storage=automatic];
// DEFAULT-NEXT:         let %48 point: @type5 [storage=automatic];
// DEFAULT-NEXT:         let %49 bits: @type6 [storage=automatic];
// DEFAULT-NEXT:         let %50 number: @type7 [storage=automatic];
// DEFAULT-NEXT:         let %51 array: array<i32, 4> [storage=automatic];
// DEFAULT-NEXT:         let %52 copied_value: i32 [storage=automatic];
// DEFAULT-NEXT:         let %53 source_value: i32 [storage=automatic];
// DEFAULT-NEXT:         let %54 pointer: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         let %55 const_pointer: ptr<const i32> [storage=automatic];
// DEFAULT-NEXT:         let %56 operation: ptr<fn(i32, i32) -> i32> [storage=automatic];
// DEFAULT-NEXT:         let %57 arithmetic: i32 [storage=automatic];
// DEFAULT-NEXT:         let %58 bitwise: i32 [storage=automatic];
// DEFAULT-NEXT:         let %59 logical: i32 [storage=automatic];
// DEFAULT-NEXT:         let %60 conditional: i32 [storage=automatic];
// DEFAULT-NEXT:         let %61 comma_value: i32 [storage=automatic];
// DEFAULT-NEXT:         let %62 string_length: i32 [storage=automatic];
// DEFAULT-NEXT:         let %63 static_calls: i32 [storage=automatic];
// DEFAULT-NEXT:         let %64 variadic_total: i32 [storage=automatic];
// DEFAULT-NEXT:         let %65 control_total: i32 [storage=automatic];
// DEFAULT-NEXT:         let %66 standard_macro: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%38, const<i32>(2));
// DEFAULT-NEXT:         write<i32>(%39, const<i32>(3));
// DEFAULT-NEXT:         write<i32>(%40, neg<i32, overflow=ub>(const<i32>(5)));
// DEFAULT-NEXT:         write<u64>(%41, const<u64>(29));
// DEFAULT-NEXT:         write<i8>(%42, truncate<i8, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(7))));
// DEFAULT-NEXT:         write<f32>(%43, const<f32>(2.5));
// DEFAULT-NEXT:         write<f64>(%44, const<f64>(3.5));
// DEFAULT-NEXT:         write<f80>(%45, const<f80>(4.5));
// DEFAULT-NEXT:         write<i32>(%46, const<i32>(90));
// DEFAULT-NEXT:         write<@type4>(%47, int_to_enum<@type4, reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(5))));
// DEFAULT-NEXT:         write<i32>(field0(%48), const<i32>(31));
// DEFAULT-NEXT:         write<i32>(field1(%48), const<i32>(37));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..1, bits=0..3>(%49), reinterpret<u32, reason=assign, fits=always>(const<i32>(6)));
// DEFAULT-NEXT:         write<i32>(bitfield1<unit=0, bytes=0..1, bits=3..7>(%49), neg<i32, overflow=ub>(const<i32>(3)));
// DEFAULT-NEXT:         write<i32>(field0(%50), const<i32>(41));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%51), const<i32>(0))), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%51), const<i32>(1))), const<i32>(2));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%51), const<i32>(2))), const<i32>(3));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%51), const<i32>(3))), const<i32>(4));
// DEFAULT-NEXT:         write<ptr<i32>>(%54, array_decay<ptr<i32>, length=Some(4)>(%51));
// DEFAULT-NEXT:         write<ptr<const i32>>(%55, pointer_cast<ptr<const i32>, reason=assign>(read<ptr<i32>>(%54)));
// DEFAULT-NEXT:         write<i32>(%53, const<i32>(43));
// DEFAULT-NEXT:         write<i32>(%52, const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ptr<const void>) -> void>(%18, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i32>>(%52)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i32>>(%53)));
// DEFAULT-NEXT:         write<ptr<fn(i32, i32) -> i32>>(%56, function_decay<ptr<fn(i32, i32) -> i32>>(%17));
// DEFAULT-NEXT:         write<i32>(%57, sub<i32, overflow=ub>(mul<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(%38), read<i32>(%39)), const<i32>(4)), const<i32>(3)));
// DEFAULT-NEXT:         let %93: i32 [synthetic] = read<i32>(%57);
// DEFAULT-NEXT:         let %94: i32 [synthetic] = div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%93), const<i32>(17));
// DEFAULT-NEXT:         write<i32>(%57, read<i32>(%94));
// DEFAULT-NEXT:         let %95: i32 [synthetic] = read<i32>(%57);
// DEFAULT-NEXT:         let %96: i32 [synthetic] = rem<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%95), const<i32>(3));
// DEFAULT-NEXT:         write<i32>(%57, read<i32>(%96));
// DEFAULT-NEXT:         write<i32>(%58, xor<i32>(or<i32>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(5)), const<i32>(3)), const<i32>(2)));
// DEFAULT-NEXT:         let %97: i32 [synthetic] = read<i32>(%58);
// DEFAULT-NEXT:         let %98: i32 [synthetic] = and<i32>(read<i32>(%97), const<i32>(31));
// DEFAULT-NEXT:         write<i32>(%58, read<i32>(%98));
// DEFAULT-NEXT:         write<i32>(%59, from_bool<i32, reason=assign>(logical_or<bool>(logical_and<bool>(lt<i32>(read<i32>(%40), const<i32>(0)), gt<u64>(read<u64>(%41), const<u64>(0))), ne<i32>(const<i32>(0), const<i32>(0)))));
// DEFAULT-NEXT:         write<i32>(%60, conditional<i32>(ne<i32>(read<i32>(%59), const<i32>(0)), const<i32>(47), const<i32>(0)));
// DEFAULT-NEXT:         let %99: i32 [synthetic] = read<i32>(%38);
// DEFAULT-NEXT:         let %100: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%99), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%38, read<i32>(%100));
// DEFAULT-NEXT:         write<i32>(%61, add<i32, overflow=ub>(read<i32>(%38), const<i32>(49)));
// DEFAULT-NEXT:         write<i32>(%62, sub<i32, overflow=ub>(reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=always>(const<u64>(3))), const<i32>(1)));
// DEFAULT-NEXT:         write<i32>(%63, add<i32, overflow=ub>(mul<i32, overflow=ub>(call<i32, signature=fn() -> i32>(%30), const<i32>(10)), call<i32, signature=fn() -> i32>(%30)));
// DEFAULT-NEXT:         add<i32, overflow=ub>(mul<i32, overflow=ub>(call<i32, signature=fn() -> i32>(%30), const<i32>(10)), call<i32, signature=fn() -> i32>(%30));
// DEFAULT-NEXT:         write<i32>(%64, call<i32, signature=fn(i32, ...) -> i32>(%25, const<i32>(4), const<i32>(2), const<i32>(3), const<i32>(5), const<i32>(7)));
// DEFAULT-NEXT:         call<i32, signature=fn(i32, ...) -> i32>(%25, const<i32>(4), const<i32>(2), const<i32>(3), const<i32>(5), const<i32>(7));
// DEFAULT-NEXT:         write<i32>(%65, call<i32, signature=fn(i32) -> i32>(%32, const<i32>(4)));
// DEFAULT-NEXT:         call<i32, signature=fn(i32) -> i32>(%32, const<i32>(4));
// DEFAULT-NEXT:         write<i32>(%66, const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%3, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(88)>(%76)), read<i32>(%13), read<i32>(%14), read<i32, volatile>(%15), read<i32>(%16), const<i32>(13), read<i32>(%66), read<i32>(%40), reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(read<u64>(%41))), widen<i32, reason=explicit>(read<i8>(%42)), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(read<f32>(%43)), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(read<f64>(%44)), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(read<f80>(%45)), read<i32>(%46), enum_to_int<u32, reason=promotion>(read<@type4>(%47)), add<i32, overflow=ub>(read<i32>(field0(%48)), read<i32>(field1(%48))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..1, bits=0..3>(%49))), read<i32>(bitfield1<unit=0, bytes=0..1, bits=3..7>(%49)), read<i32>(field0(%50)), add<i32, overflow=ub>(read<i32>(deref(ptr_offset<ptr<const i32>, subtract=false, element=i32, overflow=ub>(read<ptr<const i32>>(%55), const<i32>(0)))), read<i32>(deref(ptr_offset<ptr<const i32>, subtract=false, element=i32, overflow=ub>(read<ptr<const i32>>(%55), const<i32>(3))))), read<i32>(%52), call<i32, signature=fn(i32, i32) -> i32>(read<ptr<fn(i32, i32) -> i32>>(%56), const<i32>(53), const<i32>(6)), read<i32>(%57), read<i32>(%58), read<i32>(%59), read<i32>(%60), read<i32>(%61), read<i32>(%62), read<i32>(%63), add<i32, overflow=ub>(read<i32>(%64), read<i32>(%65)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
