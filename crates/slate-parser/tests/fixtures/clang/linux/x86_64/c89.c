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
// DEFAULT-NEXT:     type @type[[TYPE___gnuc_va_list:[0-9]+]] __gnuc_va_list = va_list;
// DEFAULT-NEXT:     type @type[[TYPE_va_list:[0-9]+]] va_list = va_list;
// DEFAULT-NEXT:     type @type[[TYPE_wchar_t:[0-9]+]] wchar_t = i32;
// DEFAULT-NEXT:     type @type[[TYPE_va_list_2:[0-9]+]] va_list = va_list;
// DEFAULT-NEXT:     type @type[[TYPE_C89Color:[0-9]+]] C89Color = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_C89_RED:[0-9]+]] C89_RED = const<i32>(3);
// DEFAULT-NEXT:         %[[VALUE_C89_GREEN:[0-9]+]] C89_GREEN = const<i32>(5);
// DEFAULT-NEXT:         %[[VALUE_C89_BLUE:[0-9]+]] C89_BLUE = const<i32>(7);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_C89Point:[0-9]+]] C89Point = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:         field1 y: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_C89Bits:[0-9]+]] C89Bits = struct {
// DEFAULT-NEXT:         field0 low: u32 : 3;
// DEFAULT-NEXT:         field1 high: i32 : 4;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0], bit_offsets=[Some(0), Some(3)], bit_units=[(0, 1)], field_units=[Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type[[TYPE_C89Number:[0-9]+]] C89Number = union {
// DEFAULT-NEXT:         field0 integer: i32;
// DEFAULT-NEXT:         field1 real: f64;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE_C89SignedInt:[0-9]+]] C89SignedInt = i32;
// DEFAULT-NEXT:     type @type[[TYPE_C89BinaryOperation:[0-9]+]] C89BinaryOperation = ptr<fn(i32, i32) -> i32>;
// DEFAULT-NEXT:     global %[[VALUE_c89_external_value:[0-9]+]] c89_external_value: i32 [storage=static] = const<i32>(11) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c89_const_global:[0-9]+]] c89_const_global: i32 [storage=static] [const] = const<i32>(17) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_c89_volatile_global:[0-9]+]] c89_volatile_global: volatile i32 [storage=static] = const<i32>(19) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_c89_joined_value:[0-9]+]] c89_joined_value: i32 [storage=static] = const<i32>(23) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_calls:[0-9]+]] calls: i32 [storage=static] = const<i32>(0) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 88> [storage=static] = code_units<array<i8, 88>>([37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_c89_add:[0-9]+]] @c89_add(%[[VALUE_left:[0-9]+]] left: i32, %[[VALUE_right:[0-9]+]] right: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%[[VALUE_left]]), read<i32>(%[[VALUE_right]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_c89_store:[0-9]+]] @c89_store(%[[VALUE_destination:[0-9]+]] destination: ptr<void>, %[[VALUE_source:[0-9]+]] source: ptr<const void>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_output:[0-9]+]] output: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_input:[0-9]+]] input: ptr<const i32> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<i32>>(%[[VALUE_output]], pointer_cast<ptr<i32>, reason=explicit>(read<ptr<void>>(%[[VALUE_destination]])));
// DEFAULT-NEXT:         write<ptr<const i32>>(%[[VALUE_input]], pointer_cast<ptr<const i32>, reason=explicit>(read<ptr<const void>>(%[[VALUE_source]])));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%[[VALUE_output]])), read<i32>(deref(read<ptr<const i32>>(%[[VALUE_input]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_c89_variadic_sum:[0-9]+]] @c89_variadic_sum(%[[VALUE_count:[0-9]+]] count: i32, ...) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_arguments:[0-9]+]] arguments: va_list [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_index:[0-9]+]] index: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_total:[0-9]+]] total: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total]], const<i32>(0));
// DEFAULT-NEXT:         va_start(%[[VALUE_arguments]]);
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_index]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_index]]), read<i32>(%[[VALUE_count]]))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_index]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_index]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE3:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:                     let %[[VALUE4:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE3]]), va_arg<i32>(%[[VALUE_arguments]]));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE4]]));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         va_end(%[[VALUE_arguments]]);
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_c89_static_local:[0-9]+]] @c89_static_local() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_calls]]);
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE5]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_calls]], read<i32>(%[[VALUE6]]));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_calls]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_c89_control_flow:[0-9]+]] @c89_control_flow(%[[VALUE_value:[0-9]+]] value: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_result:[0-9]+]] result: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_index_2:[0-9]+]] index: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%[[VALUE_result]], const<i32>(0));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_index_2]], const<i32>(0));
// DEFAULT-NEXT:         while %[[VALUE7:[0-9]+]] lt<i32>(read<i32>(%[[VALUE_index_2]]), read<i32>(%[[VALUE_value]]))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if eq<i32>(read<i32>(%[[VALUE_index_2]]), const<i32>(1))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %[[VALUE8:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_index_2]]);
// DEFAULT-NEXT:                         let %[[VALUE9:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE8]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_index_2]], read<i32>(%[[VALUE9]]));
// DEFAULT-NEXT:                         continue %[[VALUE7]];
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 let %[[VALUE10:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_result]]);
// DEFAULT-NEXT:                 let %[[VALUE11:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE10]]), read<i32>(%[[VALUE_index_2]]));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_result]], read<i32>(%[[VALUE11]]));
// DEFAULT-NEXT:                 if gt<i32>(read<i32>(%[[VALUE_result]]), const<i32>(20))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         break %[[VALUE7]];
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 let %[[VALUE12:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_index_2]]);
// DEFAULT-NEXT:                 let %[[VALUE13:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE12]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_index_2]], read<i32>(%[[VALUE13]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         do %[[VALUE14:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE15:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_result]]);
// DEFAULT-NEXT:                 let %[[VALUE16:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE15]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_result]], read<i32>(%[[VALUE16]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while gt<i32>(read<i32>(%[[VALUE_result]]), const<i32>(6));
// DEFAULT-NEXT:         switch %[[VALUE17:[0-9]+]] read<i32>(%[[VALUE_value]])
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %[[VALUE17]] const<i32>(4):
// DEFAULT-NEXT:                     let %[[VALUE18:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_result]]);
// DEFAULT-NEXT:                     let %[[VALUE19:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE18]]), const<i32>(10));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_result]], read<i32>(%[[VALUE19]]));
// DEFAULT-NEXT:                 break %[[VALUE17]];
// DEFAULT-NEXT:                 default %[[VALUE17]]:
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_result]], const<i32>(0));
// DEFAULT-NEXT:                 break %[[VALUE17]];
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%[[VALUE_result]]), const<i32>(16))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 goto %[[VALUE_c89_done:[0-9]+]];
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         write<i32>(%[[VALUE_result]], neg<i32, overflow=ub>(const<i32>(1)));
// DEFAULT-NEXT:         label %[[VALUE_c89_done]] c89_done:
// DEFAULT-NEXT:             return read<i32>(%[[VALUE_result]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_automatic_value:[0-9]+]] automatic_value: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_register_value:[0-9]+]] register_value: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_signed_value:[0-9]+]] signed_value: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_unsigned_value:[0-9]+]] unsigned_value: u64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_signed_character:[0-9]+]] signed_character: i8 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_float_value:[0-9]+]] float_value: f32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_double_value:[0-9]+]] double_value: f64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_long_double_value:[0-9]+]] long_double_value: f80 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_wide_character:[0-9]+]] wide_character: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_color:[0-9]+]] color: @type[[TYPE_C89Color]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_point:[0-9]+]] point: @type[[TYPE_C89Point]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_bits:[0-9]+]] bits: @type[[TYPE_C89Bits]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_number:[0-9]+]] number: @type[[TYPE_C89Number]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_array:[0-9]+]] array: array<i32, 4> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_copied_value:[0-9]+]] copied_value: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_source_value:[0-9]+]] source_value: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_pointer:[0-9]+]] pointer: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_const_pointer:[0-9]+]] const_pointer: ptr<const i32> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_operation:[0-9]+]] operation: ptr<fn(i32, i32) -> i32> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_arithmetic:[0-9]+]] arithmetic: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_bitwise:[0-9]+]] bitwise: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_logical:[0-9]+]] logical: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_conditional:[0-9]+]] conditional: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_comma_value:[0-9]+]] comma_value: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_string_length:[0-9]+]] string_length: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_static_calls:[0-9]+]] static_calls: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_variadic_total:[0-9]+]] variadic_total: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_control_total:[0-9]+]] control_total: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_standard_macro:[0-9]+]] standard_macro: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%[[VALUE_automatic_value]], const<i32>(2));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_register_value]], const<i32>(3));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_signed_value]], neg<i32, overflow=ub>(const<i32>(5)));
// DEFAULT-NEXT:         write<u64>(%[[VALUE_unsigned_value]], const<u64>(29));
// DEFAULT-NEXT:         write<i8>(%[[VALUE_signed_character]], truncate<i8, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(7))));
// DEFAULT-NEXT:         write<f32>(%[[VALUE_float_value]], const<f32>(2.5));
// DEFAULT-NEXT:         write<f64>(%[[VALUE_double_value]], const<f64>(3.5));
// DEFAULT-NEXT:         write<f80>(%[[VALUE_long_double_value]], const<f80>(4.5));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_wide_character]], const<i32>(90));
// DEFAULT-NEXT:         write<@type[[TYPE_C89Color]]>(%[[VALUE_color]], int_to_enum<@type[[TYPE_C89Color]], reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(5))));
// DEFAULT-NEXT:         write<i32>(field0(%[[VALUE_point]]), const<i32>(31));
// DEFAULT-NEXT:         write<i32>(field1(%[[VALUE_point]]), const<i32>(37));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..1, bits=0..3>(%[[VALUE_bits]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(6)));
// DEFAULT-NEXT:         write<i32>(bitfield1<unit=0, bytes=0..1, bits=3..7>(%[[VALUE_bits]]), neg<i32, overflow=ub>(const<i32>(3)));
// DEFAULT-NEXT:         write<i32>(field0(%[[VALUE_number]]), const<i32>(41));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%[[VALUE_array]]), const<i32>(0))), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%[[VALUE_array]]), const<i32>(1))), const<i32>(2));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%[[VALUE_array]]), const<i32>(2))), const<i32>(3));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%[[VALUE_array]]), const<i32>(3))), const<i32>(4));
// DEFAULT-NEXT:         write<ptr<i32>>(%[[VALUE_pointer]], array_decay<ptr<i32>, length=Some(4)>(%[[VALUE_array]]));
// DEFAULT-NEXT:         write<ptr<const i32>>(%[[VALUE_const_pointer]], pointer_cast<ptr<const i32>, reason=assign>(read<ptr<i32>>(%[[VALUE_pointer]])));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_source_value]], const<i32>(43));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_copied_value]], const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ptr<const void>) -> void>(%[[VALUE_c89_store]], pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i32>>(%[[VALUE_copied_value]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i32>>(%[[VALUE_source_value]])));
// DEFAULT-NEXT:         write<ptr<fn(i32, i32) -> i32>>(%[[VALUE_operation]], function_decay<ptr<fn(i32, i32) -> i32>>(%[[VALUE_c89_add]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_arithmetic]], sub<i32, overflow=ub>(mul<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(%[[VALUE_automatic_value]]), read<i32>(%[[VALUE_register_value]])), const<i32>(4)), const<i32>(3)));
// DEFAULT-NEXT:         let %[[VALUE20:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_arithmetic]]);
// DEFAULT-NEXT:         let %[[VALUE21:[0-9]+]]: i32 [synthetic] = div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%[[VALUE20]]), const<i32>(17));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_arithmetic]], read<i32>(%[[VALUE21]]));
// DEFAULT-NEXT:         let %[[VALUE22:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_arithmetic]]);
// DEFAULT-NEXT:         let %[[VALUE23:[0-9]+]]: i32 [synthetic] = rem<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%[[VALUE22]]), const<i32>(3));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_arithmetic]], read<i32>(%[[VALUE23]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_bitwise]], xor<i32>(or<i32>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(5)), const<i32>(3)), const<i32>(2)));
// DEFAULT-NEXT:         let %[[VALUE24:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_bitwise]]);
// DEFAULT-NEXT:         let %[[VALUE25:[0-9]+]]: i32 [synthetic] = and<i32>(read<i32>(%[[VALUE24]]), const<i32>(31));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_bitwise]], read<i32>(%[[VALUE25]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_logical]], from_bool<i32, reason=assign>(logical_or<bool>(logical_and<bool>(lt<i32>(read<i32>(%[[VALUE_signed_value]]), const<i32>(0)), gt<u64>(read<u64>(%[[VALUE_unsigned_value]]), const<u64>(0))), ne<i32>(const<i32>(0), const<i32>(0)))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_conditional]], conditional<i32>(ne<i32>(read<i32>(%[[VALUE_logical]]), const<i32>(0)), const<i32>(47), const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE26:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_automatic_value]]);
// DEFAULT-NEXT:         let %[[VALUE27:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE26]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_automatic_value]], read<i32>(%[[VALUE27]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_comma_value]], add<i32, overflow=ub>(read<i32>(%[[VALUE_automatic_value]]), const<i32>(49)));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_string_length]], sub<i32, overflow=ub>(reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=always>(const<u64>(3))), const<i32>(1)));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_static_calls]], add<i32, overflow=ub>(mul<i32, overflow=ub>(call<i32, signature=fn() -> i32>(%[[VALUE_c89_static_local]]), const<i32>(10)), call<i32, signature=fn() -> i32>(%[[VALUE_c89_static_local]])));
// DEFAULT-NEXT:         add<i32, overflow=ub>(mul<i32, overflow=ub>(call<i32, signature=fn() -> i32>(%[[VALUE_c89_static_local]]), const<i32>(10)), call<i32, signature=fn() -> i32>(%[[VALUE_c89_static_local]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_variadic_total]], call<i32, signature=fn(i32, ...) -> i32>(%[[VALUE_c89_variadic_sum]], const<i32>(4), const<i32>(2), const<i32>(3), const<i32>(5), const<i32>(7)));
// DEFAULT-NEXT:         call<i32, signature=fn(i32, ...) -> i32>(%[[VALUE_c89_variadic_sum]], const<i32>(4), const<i32>(2), const<i32>(3), const<i32>(5), const<i32>(7));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_control_total]], call<i32, signature=fn(i32) -> i32>(%[[VALUE_c89_control_flow]], const<i32>(4)));
// DEFAULT-NEXT:         call<i32, signature=fn(i32) -> i32>(%[[VALUE_c89_control_flow]], const<i32>(4));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_standard_macro]], const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(88)>(%[[VALUE_str]])), read<i32>(%[[VALUE_c89_external_value]]), read<i32>(%[[VALUE_c89_const_global]]), read<i32, volatile>(%[[VALUE_c89_volatile_global]]), read<i32>(%[[VALUE_c89_joined_value]]), const<i32>(13), read<i32>(%[[VALUE_standard_macro]]), read<i32>(%[[VALUE_signed_value]]), reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(read<u64>(%[[VALUE_unsigned_value]]))), widen<i32, reason=explicit>(read<i8>(%[[VALUE_signed_character]])), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(read<f32>(%[[VALUE_float_value]])), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(read<f64>(%[[VALUE_double_value]])), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(read<f80>(%[[VALUE_long_double_value]])), read<i32>(%[[VALUE_wide_character]]), enum_to_int<u32, reason=promotion>(read<@type[[TYPE_C89Color]]>(%[[VALUE_color]])), add<i32, overflow=ub>(read<i32>(field0(%[[VALUE_point]])), read<i32>(field1(%[[VALUE_point]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..1, bits=0..3>(%[[VALUE_bits]]))), read<i32>(bitfield1<unit=0, bytes=0..1, bits=3..7>(%[[VALUE_bits]])), read<i32>(field0(%[[VALUE_number]])), add<i32, overflow=ub>(read<i32>(deref(ptr_offset<ptr<const i32>, subtract=false, element=i32, overflow=ub>(read<ptr<const i32>>(%[[VALUE_const_pointer]]), const<i32>(0)))), read<i32>(deref(ptr_offset<ptr<const i32>, subtract=false, element=i32, overflow=ub>(read<ptr<const i32>>(%[[VALUE_const_pointer]]), const<i32>(3))))), read<i32>(%[[VALUE_copied_value]]), call<i32, signature=fn(i32, i32) -> i32>(read<ptr<fn(i32, i32) -> i32>>(%[[VALUE_operation]]), const<i32>(53), const<i32>(6)), read<i32>(%[[VALUE_arithmetic]]), read<i32>(%[[VALUE_bitwise]]), read<i32>(%[[VALUE_logical]]), read<i32>(%[[VALUE_conditional]]), read<i32>(%[[VALUE_comma_value]]), read<i32>(%[[VALUE_string_length]]), read<i32>(%[[VALUE_static_calls]]), add<i32, overflow=ub>(read<i32>(%[[VALUE_variadic_total]]), read<i32>(%[[VALUE_control_total]])));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
