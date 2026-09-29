#include <complex.h>
#include <fenv.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define C99_SUM3(first, ...) sum3(first, __VA_ARGS__)

struct C99Pair {
  int first;
  int second;
};

struct C99Flexible {
  size_t count;
  int    values[];
};

enum C99TrailingComma {
  C99_ENUM_VALUE = 17,
};

typedef const int    C99ConstInt;
typedef volatile int C99VolatileInt;
typedef int *restrict C99RestrictedIntPointer;

int c99_external_identifier_with_more_than_thirty_one_significant_characters =
    5;
static int slash_comment_value = 3; //

static inline int c99_inline_square(int value) { return value * value; }

static int sum3(int first, int second, int third) {
  return first + second + third;
}

static int c99_restrict_sum(const int *restrict left,
                            const int *restrict right) {
  return *left + *right;
}

static int c99_qualified_array_sum(int values[static const restrict 3]) {
  return values[0] + values[1] + values[2];
}

static int c99_vm_sum(int length, int (*values)[length]) {
  int total = 0;
  for (int index = 0; index < length; ++index) {
    total += (*values)[index];
  }
  return total;
}

static int c99_thirty_two_parameters(
    int p01, int p02, int p03, int p04, int p05, int p06, int p07, int p08,
    int p09, int p10, int p11, int p12, int p13, int p14, int p15, int p16,
    int p17, int p18, int p19, int p20, int p21, int p22, int p23, int p24,
    int p25, int p26, int p27, int p28, int p29, int p30, int p31, int p32) {
  return p01 + p02 + p03 + p04 + p05 + p06 + p07 + p08 + p09 + p10 + p11 + p12 +
         p13 + p14 + p15 + p16 + p17 + p18 + p19 + p20 + p21 + p22 + p23 + p24 +
         p25 + p26 + p27 + p28 + p29 + p30 + p31 + p32;
}

int main(void) {
  int                \u03b1                = 7;
  _Bool              boolean_value         = 4;
  long long          signed_long_long      = -9000000000LL;
  unsigned long long unsigned_long_long    = 18000000000ULL;
  float _Complex float_complex             = 1.0f + 2.0f * I;
  double _Complex double_complex           = 3.0 + 4.0 * I;
  long double _Complex long_double_complex = 5.0L + 6.0L * I;
  int enhanced_arithmetic =
      boolean_value + (signed_long_long == -9000000000LL) +
      (unsigned_long_long == 18000000000ULL) +
      (float_complex == 1.0f + 2.0f * I) + (double_complex == 3.0 + 4.0 * I) +
      (long_double_complex == 5.0L + 6.0L * I);

  int                 flexible_total = 0;
  struct C99Flexible *flexible =
      malloc(sizeof(*flexible) + 3 * sizeof(flexible->values[0]));
  if (flexible == NULL) {
    return 2;
  }
  flexible->count = 3;
  for (size_t index = 0; index < flexible->count; ++index) {
    flexible->values[index]  = (int)index + 1;
    flexible_total          += flexible->values[index];
  }
  free(flexible);

  int length = 3;
  int variable_length_array[length];
  for (int index = 0; index < length; ++index) {
    variable_length_array[index] = index + 4;
  }
  int vm_total = c99_vm_sum(length, &variable_length_array);

  int            initializer_seed        = 19;
  struct C99Pair nonconstant_initializer = {initializer_seed,
                                            initializer_seed + 1};
  struct C99Pair designated_initializer  = {.second = 23, .first = 22};
  int            designated_array[4]     = {[2] = 29, [0] = 27};

  C99ConstInt             const_value                 = 31;
  const C99ConstInt       idempotent_const_value      = const_value;
  C99VolatileInt          volatile_value              = 37;
  volatile C99VolatileInt idempotent_volatile_value   = volatile_value;
  int                     restricted_value            = 41;
  C99RestrictedIntPointer restrict restricted_pointer = &restricted_value;

  double         hexadecimal_float    = 0x1.8p+1;
  struct C99Pair compound_pair        = (struct C99Pair){43, 47};
  int            compound_array_value = ((int[]){51, 53})[1];
  int            signed_quotient      = -7 / 3;
  int            signed_remainder     = -7 % 3;

  int mixed_order                  = 59;
  mixed_order                     += 2;
  int declaration_after_statement  = 61;

  int for_total = 0;
  for (int index = 0; index < 3; ++index) {
    for_total += index;
  }

  int qualified_values[3] = {2, 3, 5};
  int macro_total         = C99_SUM3(7, 11, 13);
  int translation_limit_total =
      c99_thirty_two_parameters(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                                1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);

  int fenv_clear = feclearexcept(FE_ALL_EXCEPT);
  int fenv_flags = fetestexcept(FE_ALL_EXCEPT);

  printf(
      "%s %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d "
      "%d %d %d %d %d %d %d %d %d\n",
      __func__, \u03b1,
      c99_external_identifier_with_more_than_thirty_one_significant_characters,
      slash_comment_value, enhanced_arithmetic, flexible_total, vm_total,
      nonconstant_initializer.first + nonconstant_initializer.second,
      designated_initializer.first + designated_initializer.second,
      designated_array[0] + designated_array[2], idempotent_const_value,
      idempotent_volatile_value, *restricted_pointer, (int)hexadecimal_float,
      compound_pair.first + compound_pair.second, compound_array_value,
      signed_quotient, signed_remainder, mixed_order,
      declaration_after_statement, for_total, c99_inline_square(8),
      c99_qualified_array_sum(qualified_values), macro_total,
      translation_limit_total, C99_ENUM_VALUE, fenv_clear, fenv_flags,
      c99_restrict_sum(&qualified_values[0], &qualified_values[1]));
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
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE_C99Pair:[0-9]+]] C99Pair = struct {
// DEFAULT-NEXT:         field0 first: i32;
// DEFAULT-NEXT:         field1 second: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_C99Flexible:[0-9]+]] C99Flexible = struct {
// DEFAULT-NEXT:         field0 count: u64;
// DEFAULT-NEXT:         field1 values: array<i32, incomplete>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_C99TrailingComma:[0-9]+]] C99TrailingComma = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_C99_ENUM_VALUE:[0-9]+]] C99_ENUM_VALUE = const<i32>(17);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_C99ConstInt:[0-9]+]] C99ConstInt = i32;
// DEFAULT-NEXT:     type @type[[TYPE_C99VolatileInt:[0-9]+]] C99VolatileInt = i32;
// DEFAULT-NEXT:     type @type[[TYPE_C99RestrictedIntPointer:[0-9]+]] C99RestrictedIntPointer = ptr<i32>;
// DEFAULT-NEXT:     global %[[VALUE_c99_external_identifier_with_more_than_thirty_one_significant_characters:[0-9]+]] c99_external_identifier_with_more_than_thirty_one_significant_characters: i32 [storage=static] = const<i32>(5) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_slash_comment_value:[0-9]+]] slash_comment_value: i32 [storage=static] = const<i32>(3) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 88> [storage=static] = code_units<array<i8, 88>>([37, 115, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([109, 97, 105, 110, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_feclearexcept:[0-9]+]] @feclearexcept(%[[VALUE___excepts:[0-9]+]] __excepts: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fetestexcept:[0-9]+]] @fetestexcept(%[[VALUE___excepts_2:[0-9]+]] __excepts: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_malloc:[0-9]+]] @malloc(%[[VALUE___size:[0-9]+]] __size: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_free:[0-9]+]] @free(%[[VALUE___ptr:[0-9]+]] __ptr: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_c99_inline_square:[0-9]+]] @c99_inline_square(%[[VALUE_value:[0-9]+]] value: i32) -> i32 [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return mul<i32, overflow=ub>(read<i32>(%[[VALUE_value]]), read<i32>(%[[VALUE_value]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_sum3:[0-9]+]] @sum3(%[[VALUE_first:[0-9]+]] first: i32, %[[VALUE_second:[0-9]+]] second: i32, %[[VALUE_third:[0-9]+]] third: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(%[[VALUE_first]]), read<i32>(%[[VALUE_second]])), read<i32>(%[[VALUE_third]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_c99_restrict_sum:[0-9]+]] @c99_restrict_sum(%[[VALUE_left:[0-9]+]] left: ptr<const i32> [restrict], %[[VALUE_right:[0-9]+]] right: ptr<const i32> [restrict]) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(deref(read<ptr<const i32>>(%[[VALUE_left]]))), read<i32>(deref(read<ptr<const i32>>(%[[VALUE_right]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_c99_qualified_array_sum:[0-9]+]] @c99_qualified_array_sum(%[[VALUE_values:[0-9]+]] values: ptr<i32> [restrict] [const] [array=static 3]) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_values]]), const<i32>(0)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_values]]), const<i32>(1))))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_values]]), const<i32>(2)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_c99_vm_sum:[0-9]+]] @c99_vm_sum(%[[VALUE_length:[0-9]+]] length: i32, %[[VALUE_values_2:[0-9]+]] values: ptr<vla<i32, %[[VALUE0:[0-9]+]]>>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE0]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_length]])));
// DEFAULT-NEXT:         let %[[VALUE_total:[0-9]+]] total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %[[VALUE1:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_index:[0-9]+]] index: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_index]]), read<i32>(%[[VALUE_length]]))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_index]]);
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE2]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_index]], read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:                     let %[[VALUE5:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE4]]), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=None>(deref(read<ptr<vla<i32, %[[VALUE0]]>>>(%[[VALUE_values_2]]))), read<i32>(%[[VALUE_index]])))));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_c99_thirty_two_parameters:[0-9]+]]
// DEFAULT-SAME: @c99_thirty_two_parameters(%[[VALUE_p01:[0-9]+]] p01: i32,
// DEFAULT-SAME: %[[VALUE_p02:[0-9]+]] p02: i32,
// DEFAULT-SAME: %[[VALUE_p03:[0-9]+]] p03: i32,
// DEFAULT-SAME: %[[VALUE_p04:[0-9]+]] p04: i32,
// DEFAULT-SAME: %[[VALUE_p05:[0-9]+]] p05: i32,
// DEFAULT-SAME: %[[VALUE_p06:[0-9]+]] p06: i32,
// DEFAULT-SAME: %[[VALUE_p07:[0-9]+]] p07: i32,
// DEFAULT-SAME: %[[VALUE_p08:[0-9]+]] p08: i32,
// DEFAULT-SAME: %[[VALUE_p09:[0-9]+]] p09: i32,
// DEFAULT-SAME: %[[VALUE_p10:[0-9]+]] p10: i32,
// DEFAULT-SAME: %[[VALUE_p11:[0-9]+]] p11: i32,
// DEFAULT-SAME: %[[VALUE_p12:[0-9]+]] p12: i32,
// DEFAULT-SAME: %[[VALUE_p13:[0-9]+]] p13: i32,
// DEFAULT-SAME: %[[VALUE_p14:[0-9]+]] p14: i32,
// DEFAULT-SAME: %[[VALUE_p15:[0-9]+]] p15: i32,
// DEFAULT-SAME: %[[VALUE_p16:[0-9]+]] p16: i32,
// DEFAULT-SAME: %[[VALUE_p17:[0-9]+]] p17: i32,
// DEFAULT-SAME: %[[VALUE_p18:[0-9]+]] p18: i32,
// DEFAULT-SAME: %[[VALUE_p19:[0-9]+]] p19: i32,
// DEFAULT-SAME: %[[VALUE_p20:[0-9]+]] p20: i32,
// DEFAULT-SAME: %[[VALUE_p21:[0-9]+]] p21: i32,
// DEFAULT-SAME: %[[VALUE_p22:[0-9]+]] p22: i32,
// DEFAULT-SAME: %[[VALUE_p23:[0-9]+]] p23: i32,
// DEFAULT-SAME: %[[VALUE_p24:[0-9]+]] p24: i32,
// DEFAULT-SAME: %[[VALUE_p25:[0-9]+]] p25: i32,
// DEFAULT-SAME: %[[VALUE_p26:[0-9]+]] p26: i32,
// DEFAULT-SAME: %[[VALUE_p27:[0-9]+]] p27: i32,
// DEFAULT-SAME: %[[VALUE_p28:[0-9]+]] p28: i32,
// DEFAULT-SAME: %[[VALUE_p29:[0-9]+]] p29: i32,
// DEFAULT-SAME: %[[VALUE_p30:[0-9]+]] p30: i32,
// DEFAULT-SAME: %[[VALUE_p31:[0-9]+]] p31: i32,
// DEFAULT-SAME: %[[VALUE_p32:[0-9]+]] p32: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32,
// DEFAULT-SAME: overflow=ub>(read<i32>(%[[VALUE_p01]]),
// DEFAULT-SAME: read<i32>(%[[VALUE_p02]])),
// DEFAULT-SAME: read<i32>(%[[VALUE_p03]])),
// DEFAULT-SAME: read<i32>(%[[VALUE_p04]])),
// DEFAULT-SAME: read<i32>(%[[VALUE_p05]])),
// DEFAULT-SAME: read<i32>(%[[VALUE_p06]])),
// DEFAULT-SAME: read<i32>(%[[VALUE_p07]])),
// DEFAULT-SAME: read<i32>(%[[VALUE_p08]])),
// DEFAULT-SAME: read<i32>(%[[VALUE_p09]])),
// DEFAULT-SAME: read<i32>(%[[VALUE_p10]])),
// DEFAULT-SAME: read<i32>(%[[VALUE_p11]])),
// DEFAULT-SAME: read<i32>(%[[VALUE_p12]])),
// DEFAULT-SAME: read<i32>(%[[VALUE_p13]])),
// DEFAULT-SAME: read<i32>(%[[VALUE_p14]])),
// DEFAULT-SAME: read<i32>(%[[VALUE_p15]])),
// DEFAULT-SAME: read<i32>(%[[VALUE_p16]])),
// DEFAULT-SAME: read<i32>(%[[VALUE_p17]])),
// DEFAULT-SAME: read<i32>(%[[VALUE_p18]])),
// DEFAULT-SAME: read<i32>(%[[VALUE_p19]])),
// DEFAULT-SAME: read<i32>(%[[VALUE_p20]])),
// DEFAULT-SAME: read<i32>(%[[VALUE_p21]])),
// DEFAULT-SAME: read<i32>(%[[VALUE_p22]])),
// DEFAULT-SAME: read<i32>(%[[VALUE_p23]])),
// DEFAULT-SAME: read<i32>(%[[VALUE_p24]])),
// DEFAULT-SAME: read<i32>(%[[VALUE_p25]])),
// DEFAULT-SAME: read<i32>(%[[VALUE_p26]])),
// DEFAULT-SAME: read<i32>(%[[VALUE_p27]])),
// DEFAULT-SAME: read<i32>(%[[VALUE_p28]])),
// DEFAULT-SAME: read<i32>(%[[VALUE_p29]])),
// DEFAULT-SAME: read<i32>(%[[VALUE_p30]])),
// DEFAULT-SAME: read<i32>(%[[VALUE_p31]])),
// DEFAULT-SAME: read<i32>(%[[VALUE_p32]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]] \u03b1: i32 [storage=automatic] = const<i32>(7);
// DEFAULT-NEXT:         let %[[VALUE_boolean_value:[0-9]+]] boolean_value: bool [storage=automatic] = ne<i32, reason=assign>(const<i32>(4), const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE_signed_long_long:[0-9]+]] signed_long_long: i64 [storage=automatic] = neg<i64, overflow=ub>(const<i64>(9000000000));
// DEFAULT-NEXT:         let %[[VALUE_unsigned_long_long:[0-9]+]] unsigned_long_long: u64 [storage=automatic] = const<u64>(18000000000);
// DEFAULT-NEXT:         let %[[VALUE_float_complex:[0-9]+]] float_complex: complex<f32> [storage=automatic] = add<complex<f32>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(const<f32>(1.0), mul<complex<f32>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(const<f32>(2.0), aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = const<f32>(1.0))));
// DEFAULT-NEXT:         let %[[VALUE_double_complex:[0-9]+]] double_complex: complex<f64> [storage=automatic] = add<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(const<f64>(3.0), mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(const<f64>(4.0), complex_convert<complex<f64>, reason=usual_arith>(aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = const<f32>(1.0)))));
// DEFAULT-NEXT:         let %[[VALUE_long_double_complex:[0-9]+]] long_double_complex: complex<f80> [storage=automatic] = add<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(const<f80>(5), mul<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(const<f80>(6), complex_convert<complex<f80>, reason=usual_arith>(aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = const<f32>(1.0)))));
// DEFAULT-NEXT:         let %[[VALUE_enhanced_arithmetic:[0-9]+]] enhanced_arithmetic: i32 [storage=automatic] = add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32,
// DEFAULT-SAME: overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool>(%[[VALUE_boolean_value]])), from_bool<i32,
// DEFAULT-SAME: reason=promotion>(eq<i64>(read<i64>(%[[VALUE_signed_long_long]]), neg<i64, overflow=ub>(const<i64>(9000000000))))), from_bool<i32,
// DEFAULT-SAME: reason=promotion>(eq<u64>(read<u64>(%[[VALUE_unsigned_long_long]]), const<u64>(18000000000)))), from_bool<i32, reason=promotion>(eq<complex<f32>,
// DEFAULT-SAME: exceptions=ignore>(read<complex<f32>>(%[[VALUE_float_complex]]), add<complex<f32>, complex=true, rounding=nearest_even, exceptions=ignore,
// DEFAULT-SAME: range=full>(const<f32>(1.0), mul<complex<f32>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(const<f32>(2.0), aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = const<f32>(1.0))))))), from_bool<i32, reason=promotion>(eq<complex<f64>,
// DEFAULT-SAME: exceptions=ignore>(read<complex<f64>>(%[[VALUE_double_complex]]), add<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore,
// DEFAULT-SAME: range=full>(const<f64>(3.0), mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(const<f64>(4.0), complex_convert<complex<f64>, reason=usual_arith>(aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = const<f32>(1.0)))))))), from_bool<i32, reason=promotion>(eq<complex<f80>,
// DEFAULT-SAME: exceptions=ignore>(read<complex<f80>>(%[[VALUE_long_double_complex]]), add<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore,
// DEFAULT-SAME: range=full>(const<f80>(5), mul<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(const<f80>(6), complex_convert<complex<f80>, reason=usual_arith>(aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = const<f32>(1.0))))))));
// DEFAULT-NEXT:         let %[[VALUE_flexible_total:[0-9]+]] flexible_total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_flexible:[0-9]+]] flexible: ptr<@type[[TYPE_C99Flexible]]> [storage=automatic] = pointer_cast<ptr<@type[[TYPE_C99Flexible]]>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_malloc]], add<u64, overflow=wrap>(const<u64>(8), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3))), const<u64>(4)))));
// DEFAULT-NEXT:         if eq<ptr<@type[[TYPE_C99Flexible]]>>(read<ptr<@type[[TYPE_C99Flexible]]>>(%[[VALUE_flexible]]), null<ptr<@type[[TYPE_C99Flexible]]>>)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 return const<i32>(2);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         write<u64>(field0(deref(read<ptr<@type[[TYPE_C99Flexible]]>>(%[[VALUE_flexible]]))), reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(3))));
// DEFAULT-NEXT:         for %[[VALUE7:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_index_2:[0-9]+]] index: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:             condition: lt<u64>(read<u64>(%[[VALUE_index_2]]), read<u64>(field0(deref(read<ptr<@type[[TYPE_C99Flexible]]>>(%[[VALUE_flexible]])))))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE8:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_index_2]]);
// DEFAULT-NEXT:                 let %[[VALUE9:[0-9]+]]: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%[[VALUE8]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                 write<u64>(%[[VALUE_index_2]], read<u64>(%[[VALUE9]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=None>(field1(deref(read<ptr<@type[[TYPE_C99Flexible]]>>(%[[VALUE_flexible]])))), read<u64>(%[[VALUE_index_2]]))), add<i32, overflow=ub>(reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(read<u64>(%[[VALUE_index_2]]))), const<i32>(1)));
// DEFAULT-NEXT:                     let %[[VALUE10:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_flexible_total]]);
// DEFAULT-NEXT:                     let %[[VALUE11:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE10]]), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=None>(field1(deref(read<ptr<@type[[TYPE_C99Flexible]]>>(%[[VALUE_flexible]])))), read<u64>(%[[VALUE_index_2]])))));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_flexible_total]], read<i32>(%[[VALUE11]]));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_free]], pointer_cast<ptr<void>, reason=arg>(read<ptr<@type[[TYPE_C99Flexible]]>>(%[[VALUE_flexible]])));
// DEFAULT-NEXT:         let %[[VALUE_length_2:[0-9]+]] length: i32 [storage=automatic] = const<i32>(3);
// DEFAULT-NEXT:         let %[[VALUE12:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_length_2]])));
// DEFAULT-NEXT:         let %[[VALUE_variable_length_array:[0-9]+]] variable_length_array: vla<i32, %[[VALUE12]]> [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE13:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_index_3:[0-9]+]] index: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_index_3]]), read<i32>(%[[VALUE_length_2]]))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE14:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_index_3]]);
// DEFAULT-NEXT:                 let %[[VALUE15:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE14]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_index_3]], read<i32>(%[[VALUE15]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=None>(%[[VALUE_variable_length_array]]), read<i32>(%[[VALUE_index_3]]))), add<i32, overflow=ub>(read<i32>(%[[VALUE_index_3]]), const<i32>(4)));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         let %[[VALUE_vm_total:[0-9]+]] vm_total: i32 [storage=automatic] = call<i32, signature=fn(i32, ptr<vla<i32, *>>) -> i32>(%[[VALUE_c99_vm_sum]], read<i32>(%[[VALUE_length_2]]), pointer_cast<ptr<vla<i32, *>>, reason=arg>(addr_of<ptr<vla<i32, %[[VALUE12]]>>>(%[[VALUE_variable_length_array]])));
// DEFAULT-NEXT:         let %[[VALUE_initializer_seed:[0-9]+]] initializer_seed: i32 [storage=automatic] = const<i32>(19);
// DEFAULT-NEXT:         let %[[VALUE_nonconstant_initializer:[0-9]+]] nonconstant_initializer: @type[[TYPE_C99Pair]] [storage=automatic] = aggregate<@type[[TYPE_C99Pair]], zero_fill=false>(field0 = read<i32>(%[[VALUE_initializer_seed]]), field1 = add<i32, overflow=ub>(read<i32>(%[[VALUE_initializer_seed]]), const<i32>(1)));
// DEFAULT-NEXT:         let %[[VALUE_designated_initializer:[0-9]+]] designated_initializer: @type[[TYPE_C99Pair]] [storage=automatic] = aggregate<@type[[TYPE_C99Pair]], zero_fill=false>(field0 = const<i32>(22), field1 = const<i32>(23));
// DEFAULT-NEXT:         let %[[VALUE_designated_array:[0-9]+]] designated_array: array<i32, 4> [storage=automatic] [align=16] = aggregate<array<i32, 4>, zero_fill=true>(index0 = const<i32>(27), index2 = const<i32>(29));
// DEFAULT-NEXT:         let %[[VALUE_const_value:[0-9]+]] const_value: i32 [storage=automatic] [const] = const<i32>(31);
// DEFAULT-NEXT:         let %[[VALUE_idempotent_const_value:[0-9]+]] idempotent_const_value: i32 [storage=automatic] [const] = read<i32>(%[[VALUE_const_value]]);
// DEFAULT-NEXT:         let %[[VALUE_volatile_value:[0-9]+]] volatile_value: volatile i32 [storage=automatic] = const<i32>(37);
// DEFAULT-NEXT:         let %[[VALUE_idempotent_volatile_value:[0-9]+]] idempotent_volatile_value: volatile i32 [storage=automatic] = read<i32, volatile>(%[[VALUE_volatile_value]]);
// DEFAULT-NEXT:         let %[[VALUE_restricted_value:[0-9]+]] restricted_value: i32 [storage=automatic] = const<i32>(41);
// DEFAULT-NEXT:         let %[[VALUE_restricted_pointer:[0-9]+]] restricted_pointer: ptr<i32> [storage=automatic] [restrict] = addr_of<ptr<i32>>(%[[VALUE_restricted_value]]);
// DEFAULT-NEXT:         let %[[VALUE_hexadecimal_float:[0-9]+]] hexadecimal_float: f64 [storage=automatic] = const<f64>(3.0);
// DEFAULT-NEXT:         let %[[VALUE_compound_pair:[0-9]+]] compound_pair: @type[[TYPE_C99Pair]] [storage=automatic] = copy<@type[[TYPE_C99Pair]], reason=assign>(read<@type[[TYPE_C99Pair]]>(compound_literal %[[VALUE16:[0-9]+]] [storage=automatic] = aggregate<@type[[TYPE_C99Pair]], zero_fill=false>(field0 = const<i32>(43), field1 = const<i32>(47))));
// DEFAULT-NEXT:         let %[[VALUE_compound_array_value:[0-9]+]] compound_array_value: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(compound_literal %[[VALUE17:[0-9]+]] [storage=automatic] = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(51), index1 = const<i32>(53))), const<i32>(1))));
// DEFAULT-NEXT:         let %[[VALUE_signed_quotient:[0-9]+]] signed_quotient: i32 [storage=automatic] = div<i32, by_zero=ub, min_by_neg_one=ub>(neg<i32, overflow=ub>(const<i32>(7)), const<i32>(3));
// DEFAULT-NEXT:         let %[[VALUE_signed_remainder:[0-9]+]] signed_remainder: i32 [storage=automatic] = rem<i32, by_zero=ub, min_by_neg_one=ub>(neg<i32, overflow=ub>(const<i32>(7)), const<i32>(3));
// DEFAULT-NEXT:         let %[[VALUE_mixed_order:[0-9]+]] mixed_order: i32 [storage=automatic] = const<i32>(59);
// DEFAULT-NEXT:         let %[[VALUE18:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_mixed_order]]);
// DEFAULT-NEXT:         let %[[VALUE19:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE18]]), const<i32>(2));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_mixed_order]], read<i32>(%[[VALUE19]]));
// DEFAULT-NEXT:         let %[[VALUE_declaration_after_statement:[0-9]+]] declaration_after_statement: i32 [storage=automatic] = const<i32>(61);
// DEFAULT-NEXT:         let %[[VALUE_for_total:[0-9]+]] for_total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %[[VALUE20:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_index_4:[0-9]+]] index: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_index_4]]), const<i32>(3))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE21:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_index_4]]);
// DEFAULT-NEXT:                 let %[[VALUE22:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE21]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_index_4]], read<i32>(%[[VALUE22]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE23:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_for_total]]);
// DEFAULT-NEXT:                     let %[[VALUE24:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE23]]), read<i32>(%[[VALUE_index_4]]));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_for_total]], read<i32>(%[[VALUE24]]));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         let %[[VALUE_qualified_values:[0-9]+]] qualified_values: array<i32, 3> [storage=automatic] = aggregate<array<i32, 3>, zero_fill=false>(index0 = const<i32>(2), index1 = const<i32>(3), index2 = const<i32>(5));
// DEFAULT-NEXT:         let %[[VALUE_macro_total:[0-9]+]] macro_total: i32 [storage=automatic] = call<i32, signature=fn(i32, i32, i32) -> i32>(%[[VALUE_sum3]], const<i32>(7), const<i32>(11), const<i32>(13));
// DEFAULT-NEXT:         let %[[VALUE_translation_limit_total:[0-9]+]] translation_limit_total: i32 [storage=automatic] = call<i32, signature=fn(i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32) -> i32>(%[[VALUE_c99_thirty_two_parameters]], const<i32>(1), const<i32>(1), const<i32>(1), const<i32>(1), const<i32>(1), const<i32>(1), const<i32>(1), const<i32>(1), const<i32>(1), const<i32>(1), const<i32>(1), const<i32>(1), const<i32>(1), const<i32>(1), const<i32>(1), const<i32>(1), const<i32>(1), const<i32>(1), const<i32>(1), const<i32>(1), const<i32>(1), const<i32>(1), const<i32>(1), const<i32>(1), const<i32>(1), const<i32>(1), const<i32>(1), const<i32>(1), const<i32>(1), const<i32>(1), const<i32>(1), const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE_fenv_clear:[0-9]+]] fenv_clear: i32 [storage=automatic] = call<i32, signature=fn(i32) -> i32>(%[[VALUE_feclearexcept]], or<i32>(or<i32>(or<i32>(or<i32>(const<i32>(32), const<i32>(4)), const<i32>(16)), const<i32>(8)), const<i32>(1)));
// DEFAULT-NEXT:         let %[[VALUE_fenv_flags:[0-9]+]] fenv_flags: i32 [storage=automatic] = call<i32, signature=fn(i32) -> i32>(%[[VALUE_fetestexcept]], or<i32>(or<i32>(or<i32>(or<i32>(const<i32>(32), const<i32>(4)), const<i32>(16)), const<i32>(8)), const<i32>(1)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>,
// DEFAULT-SAME: length=Some(88)>(%[[VALUE_str]])), array_decay<ptr<i8>,
// DEFAULT-SAME: length=Some(5)>(%[[VALUE_str_2]]),
// DEFAULT-SAME: read<i32>(%[[VALUE6]]),
// DEFAULT-SAME: read<i32>(%[[VALUE_c99_external_identifier_with_more_than_thirty_one_significant_characters]]),
// DEFAULT-SAME: read<i32>(%[[VALUE_slash_comment_value]]),
// DEFAULT-SAME: read<i32>(%[[VALUE_enhanced_arithmetic]]),
// DEFAULT-SAME: read<i32>(%[[VALUE_flexible_total]]),
// DEFAULT-SAME: read<i32>(%[[VALUE_vm_total]]), add<i32,
// DEFAULT-SAME: overflow=ub>(read<i32>(field0(%[[VALUE_nonconstant_initializer]])),
// DEFAULT-SAME: read<i32>(field1(%[[VALUE_nonconstant_initializer]]))), add<i32,
// DEFAULT-SAME: overflow=ub>(read<i32>(field0(%[[VALUE_designated_initializer]])),
// DEFAULT-SAME: read<i32>(field1(%[[VALUE_designated_initializer]]))), add<i32, overflow=ub>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32,
// DEFAULT-SAME: overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%[[VALUE_designated_array]]), const<i32>(0)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false,
// DEFAULT-SAME: element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%[[VALUE_designated_array]]), const<i32>(2))))),
// DEFAULT-SAME: read<i32>(%[[VALUE_idempotent_const_value]]), read<i32,
// DEFAULT-SAME: volatile>(%[[VALUE_idempotent_volatile_value]]),
// DEFAULT-SAME: read<i32>(deref(read<ptr<i32>>(%[[VALUE_restricted_pointer]]))), float_to_int<i32, reason=explicit, out_of_range=ub,
// DEFAULT-SAME: exceptions=ignore>(read<f64>(%[[VALUE_hexadecimal_float]])), add<i32,
// DEFAULT-SAME: overflow=ub>(read<i32>(field0(%[[VALUE_compound_pair]])),
// DEFAULT-SAME: read<i32>(field1(%[[VALUE_compound_pair]]))),
// DEFAULT-SAME: read<i32>(%[[VALUE_compound_array_value]]),
// DEFAULT-SAME: read<i32>(%[[VALUE_signed_quotient]]),
// DEFAULT-SAME: read<i32>(%[[VALUE_signed_remainder]]),
// DEFAULT-SAME: read<i32>(%[[VALUE_mixed_order]]),
// DEFAULT-SAME: read<i32>(%[[VALUE_declaration_after_statement]]),
// DEFAULT-SAME: read<i32>(%[[VALUE_for_total]]), call<i32, signature=fn(i32) ->
// DEFAULT-SAME: i32>(%[[VALUE_c99_inline_square]], const<i32>(8)), call<i32, signature=fn(ptr<i32>) ->
// DEFAULT-SAME: i32>(%[[VALUE_c99_qualified_array_sum]], array_decay<ptr<i32>,
// DEFAULT-SAME: length=Some(3)>(%[[VALUE_qualified_values]])),
// DEFAULT-SAME: read<i32>(%[[VALUE_macro_total]]),
// DEFAULT-SAME: read<i32>(%[[VALUE_translation_limit_total]]), const<i32>(17),
// DEFAULT-SAME: read<i32>(%[[VALUE_fenv_clear]]),
// DEFAULT-SAME: read<i32>(%[[VALUE_fenv_flags]]), call<i32, signature=fn(ptr<const i32>, ptr<const i32>) ->
// DEFAULT-SAME: i32>(%[[VALUE_c99_restrict_sum]], pointer_cast<ptr<const i32>, reason=arg>(addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32,
// DEFAULT-SAME: overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(%[[VALUE_qualified_values]]), const<i32>(0))))), pointer_cast<ptr<const i32>,
// DEFAULT-SAME: reason=arg>(addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>,
// DEFAULT-SAME: length=Some(3)>(%[[VALUE_qualified_values]]), const<i32>(1)))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
