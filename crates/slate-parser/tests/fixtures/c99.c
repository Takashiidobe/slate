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
// DEFAULT-NEXT:     type @type0 size_t = u64;
// DEFAULT-NEXT:     type @type1 C99Pair = struct {
// DEFAULT-NEXT:         field0 first: i32;
// DEFAULT-NEXT:         field1 second: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type2 C99Flexible = struct {
// DEFAULT-NEXT:         field0 count: u64;
// DEFAULT-NEXT:         field1 values: array<i32, incomplete>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type3 C99TrailingComma = enum : u32 {
// DEFAULT-NEXT:         %0 C99_ENUM_VALUE = const<i32>(17);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type4 C99ConstInt = i32;
// DEFAULT-NEXT:     type @type5 C99VolatileInt = i32;
// DEFAULT-NEXT:     type @type6 C99RestrictedIntPointer = ptr<i32>;
// DEFAULT-NEXT:     global %13 c99_external_identifier_with_more_than_thirty_one_significant_characters: i32 [storage=static] = const<i32>(5) [linkage=external];
// DEFAULT-NEXT:     global %14 slash_comment_value: i32 [storage=static] = const<i32>(3) [linkage=internal];
// DEFAULT-NEXT:     global %117 .str117: array<i8, 88> [storage=static] = code_units<array<i8, 88>>([37, 115, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %118 .str118: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([109, 97, 105, 110, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @feclearexcept(%104 __excepts: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %1 @fetestexcept(%105 __excepts: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %3 @printf(%106 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %4 @malloc(%107 __size: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %5 @free(%108 __ptr: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %15 @c99_inline_square(%16 value: i32) -> i32 [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return mul<i32, overflow=ub>(read<i32>(%16), read<i32>(%16));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @sum3(%18 first: i32, %19 second: i32, %20 third: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(%18), read<i32>(%19)), read<i32>(%20));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %21 @c99_restrict_sum(%22 left: ptr<const i32> [restrict], %23 right: ptr<const i32> [restrict]) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(deref(read<ptr<const i32>>(%22))), read<i32>(deref(read<ptr<const i32>>(%23))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %24 @c99_qualified_array_sum(%25 values: ptr<i32> [restrict] [const] [array=static 3]) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%25), const<i32>(0)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%25), const<i32>(1))))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%25), const<i32>(2)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %26 @c99_vm_sum(%27 length: i32, %28 values: ptr<vla<i32, %109>>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %109: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%27)));
// DEFAULT-NEXT:         let %29 total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %110
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %30 index: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%30), read<i32>(%27))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %119: i32 [synthetic] = read<i32>(%30);
// DEFAULT-NEXT:                 let %120: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%119), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%30, read<i32>(%120));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %121: i32 [synthetic] = read<i32>(%29);
// DEFAULT-NEXT:                     let %122: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%121), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=None>(deref(read<ptr<vla<i32, %109>>>(%28))), read<i32>(%30)))));
// DEFAULT-NEXT:                     write<i32>(%29, read<i32>(%122));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return read<i32>(%29);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %31 @c99_thirty_two_parameters(%32 p01: i32, %33 p02: i32, %34 p03: i32, %35 p04: i32, %36 p05: i32, %37 p06: i32, %38 p07: i32, %39 p08: i32, %40 p09: i32, %41 p10: i32, %42 p11: i32, %43 p12: i32, %44 p13: i32, %45 p14: i32, %46 p15: i32, %47 p16: i32, %48 p17: i32, %49 p18: i32, %50 p19: i32, %51 p20: i32, %52 p21: i32, %53 p22: i32, %54 p23: i32, %55 p24: i32, %56 p25: i32, %57 p26: i32, %58 p27: i32, %59 p28: i32, %60 p29: i32, %61 p30: i32, %62 p31: i32, %63 p32: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(%32), read<i32>(%33)), read<i32>(%34)), read<i32>(%35)), read<i32>(%36)), read<i32>(%37)), read<i32>(%38)), read<i32>(%39)), read<i32>(%40)), read<i32>(%41)), read<i32>(%42)), read<i32>(%43)), read<i32>(%44)), read<i32>(%45)), read<i32>(%46)), read<i32>(%47)), read<i32>(%48)), read<i32>(%49)), read<i32>(%50)), read<i32>(%51)), read<i32>(%52)), read<i32>(%53)), read<i32>(%54)), read<i32>(%55)), read<i32>(%56)), read<i32>(%57)), read<i32>(%58)), read<i32>(%59)), read<i32>(%60)), read<i32>(%61)), read<i32>(%62)), read<i32>(%63));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %64 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %65 \u03b1: i32 [storage=automatic] = const<i32>(7);
// DEFAULT-NEXT:         let %66 boolean_value: bool [storage=automatic] = ne<i32, reason=assign>(const<i32>(4), const<i32>(0));
// DEFAULT-NEXT:         let %67 signed_long_long: i64 [storage=automatic] = neg<i64, overflow=ub>(const<i64>(9000000000));
// DEFAULT-NEXT:         let %68 unsigned_long_long: u64 [storage=automatic] = const<u64>(18000000000);
// DEFAULT-NEXT:         let %69 float_complex: complex<f32> [storage=automatic] = add<complex<f32>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(const<f32>(1.0), mul<complex<f32>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(const<f32>(2.0), aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = const<f32>(1.0))));
// DEFAULT-NEXT:         let %70 double_complex: complex<f64> [storage=automatic] = add<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(const<f64>(3.0), mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(const<f64>(4.0), complex_convert<complex<f64>, reason=usual_arith>(aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = const<f32>(1.0)))));
// DEFAULT-NEXT:         let %71 long_double_complex: complex<f80> [storage=automatic] = add<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(const<f80>(5), mul<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(const<f80>(6), complex_convert<complex<f80>, reason=usual_arith>(aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = const<f32>(1.0)))));
// DEFAULT-NEXT:         let %72 enhanced_arithmetic: i32 [storage=automatic] = add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(from_bool<i32, reason=promotion>(read<bool>(%66)), from_bool<i32, reason=promotion>(eq<i64>(read<i64>(%67), neg<i64, overflow=ub>(const<i64>(9000000000))))), from_bool<i32, reason=promotion>(eq<u64>(read<u64>(%68), const<u64>(18000000000)))), from_bool<i32, reason=promotion>(eq<complex<f32>, exceptions=ignore>(read<complex<f32>>(%69), add<complex<f32>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(const<f32>(1.0), mul<complex<f32>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(const<f32>(2.0), aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = const<f32>(1.0))))))), from_bool<i32, reason=promotion>(eq<complex<f64>, exceptions=ignore>(read<complex<f64>>(%70), add<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(const<f64>(3.0), mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(const<f64>(4.0), complex_convert<complex<f64>, reason=usual_arith>(aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = const<f32>(1.0)))))))), from_bool<i32, reason=promotion>(eq<complex<f80>, exceptions=ignore>(read<complex<f80>>(%71), add<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(const<f80>(5), mul<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(const<f80>(6), complex_convert<complex<f80>, reason=usual_arith>(aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = const<f32>(1.0))))))));
// DEFAULT-NEXT:         let %73 flexible_total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %74 flexible: ptr<@type2> [storage=automatic] = pointer_cast<ptr<@type2>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%4, add<u64, overflow=wrap>(const<u64>(8), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3))), const<u64>(4)))));
// DEFAULT-NEXT:         if eq<ptr<@type2>>(read<ptr<@type2>>(%74), null<ptr<@type2>>)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 return const<i32>(2);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         write<u64>(field0(deref(read<ptr<@type2>>(%74))), reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(3))));
// DEFAULT-NEXT:         for %111
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %75 index: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:             condition: lt<u64>(read<u64>(%75), read<u64>(field0(deref(read<ptr<@type2>>(%74)))))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %123: u64 [synthetic] = read<u64>(%75);
// DEFAULT-NEXT:                 let %124: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%123), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                 write<u64>(%75, read<u64>(%124));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=None>(field1(deref(read<ptr<@type2>>(%74)))), read<u64>(%75))), add<i32, overflow=ub>(reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(read<u64>(%75))), const<i32>(1)));
// DEFAULT-NEXT:                     let %125: i32 [synthetic] = read<i32>(%73);
// DEFAULT-NEXT:                     let %126: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%125), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=None>(field1(deref(read<ptr<@type2>>(%74)))), read<u64>(%75)))));
// DEFAULT-NEXT:                     write<i32>(%73, read<i32>(%126));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%5, pointer_cast<ptr<void>, reason=arg>(read<ptr<@type2>>(%74)));
// DEFAULT-NEXT:         let %76 length: i32 [storage=automatic] = const<i32>(3);
// DEFAULT-NEXT:         let %112: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%76)));
// DEFAULT-NEXT:         let %77 variable_length_array: vla<i32, %112> [storage=automatic];
// DEFAULT-NEXT:         for %113
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %78 index: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%78), read<i32>(%76))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %127: i32 [synthetic] = read<i32>(%78);
// DEFAULT-NEXT:                 let %128: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%127), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%78, read<i32>(%128));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=None>(%77), read<i32>(%78))), add<i32, overflow=ub>(read<i32>(%78), const<i32>(4)));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         let %79 vm_total: i32 [storage=automatic] = call<i32, signature=fn(i32, ptr<vla<i32, *>>) -> i32>(%26, read<i32>(%76), pointer_cast<ptr<vla<i32, *>>, reason=arg>(addr_of<ptr<vla<i32, %112>>>(%77)));
// DEFAULT-NEXT:         let %80 initializer_seed: i32 [storage=automatic] = const<i32>(19);
// DEFAULT-NEXT:         let %81 nonconstant_initializer: @type1 [storage=automatic] = aggregate<@type1, zero_fill=false>(field0 = read<i32>(%80), field1 = add<i32, overflow=ub>(read<i32>(%80), const<i32>(1)));
// DEFAULT-NEXT:         let %82 designated_initializer: @type1 [storage=automatic] = aggregate<@type1, zero_fill=false>(field0 = const<i32>(22), field1 = const<i32>(23));
// DEFAULT-NEXT:         let %83 designated_array: array<i32, 4> [storage=automatic] [align=16] = aggregate<array<i32, 4>, zero_fill=true>(index0 = const<i32>(27), index2 = const<i32>(29));
// DEFAULT-NEXT:         let %84 const_value: i32 [storage=automatic] [const] = const<i32>(31);
// DEFAULT-NEXT:         let %85 idempotent_const_value: i32 [storage=automatic] [const] = read<i32>(%84);
// DEFAULT-NEXT:         let %86 volatile_value: volatile i32 [storage=automatic] = const<i32>(37);
// DEFAULT-NEXT:         let %87 idempotent_volatile_value: volatile i32 [storage=automatic] = read<i32, volatile>(%86);
// DEFAULT-NEXT:         let %88 restricted_value: i32 [storage=automatic] = const<i32>(41);
// DEFAULT-NEXT:         let %89 restricted_pointer: ptr<i32> [storage=automatic] [restrict] = addr_of<ptr<i32>>(%88);
// DEFAULT-NEXT:         let %90 hexadecimal_float: f64 [storage=automatic] = const<f64>(3.0);
// DEFAULT-NEXT:         let %91 compound_pair: @type1 [storage=automatic] = copy<@type1, reason=assign>(read<@type1>(compound_literal %114 [storage=automatic] = aggregate<@type1, zero_fill=false>(field0 = const<i32>(43), field1 = const<i32>(47))));
// DEFAULT-NEXT:         let %92 compound_array_value: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(compound_literal %115 [storage=automatic] = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(51), index1 = const<i32>(53))), const<i32>(1))));
// DEFAULT-NEXT:         let %93 signed_quotient: i32 [storage=automatic] = div<i32, by_zero=ub, min_by_neg_one=ub>(neg<i32, overflow=ub>(const<i32>(7)), const<i32>(3));
// DEFAULT-NEXT:         let %94 signed_remainder: i32 [storage=automatic] = rem<i32, by_zero=ub, min_by_neg_one=ub>(neg<i32, overflow=ub>(const<i32>(7)), const<i32>(3));
// DEFAULT-NEXT:         let %95 mixed_order: i32 [storage=automatic] = const<i32>(59);
// DEFAULT-NEXT:         let %129: i32 [synthetic] = read<i32>(%95);
// DEFAULT-NEXT:         let %130: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%129), const<i32>(2));
// DEFAULT-NEXT:         write<i32>(%95, read<i32>(%130));
// DEFAULT-NEXT:         let %96 declaration_after_statement: i32 [storage=automatic] = const<i32>(61);
// DEFAULT-NEXT:         let %97 for_total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %116
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %98 index: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%98), const<i32>(3))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %131: i32 [synthetic] = read<i32>(%98);
// DEFAULT-NEXT:                 let %132: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%131), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%98, read<i32>(%132));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %133: i32 [synthetic] = read<i32>(%97);
// DEFAULT-NEXT:                     let %134: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%133), read<i32>(%98));
// DEFAULT-NEXT:                     write<i32>(%97, read<i32>(%134));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         let %99 qualified_values: array<i32, 3> [storage=automatic] = aggregate<array<i32, 3>, zero_fill=false>(index0 = const<i32>(2), index1 = const<i32>(3), index2 = const<i32>(5));
// DEFAULT-NEXT:         let %100 macro_total: i32 [storage=automatic] = call<i32, signature=fn(i32, i32, i32) -> i32>(%17, const<i32>(7), const<i32>(11), const<i32>(13));
// DEFAULT-NEXT:         let %101 translation_limit_total: i32 [storage=automatic] = call<i32, signature=fn(i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32) -> i32>(%31, const<i32>(1), const<i32>(1), const<i32>(1), const<i32>(1), const<i32>(1), const<i32>(1), const<i32>(1), const<i32>(1), const<i32>(1), const<i32>(1), const<i32>(1), const<i32>(1), const<i32>(1), const<i32>(1), const<i32>(1), const<i32>(1), const<i32>(1), const<i32>(1), const<i32>(1), const<i32>(1), const<i32>(1), const<i32>(1), const<i32>(1), const<i32>(1), const<i32>(1), const<i32>(1), const<i32>(1), const<i32>(1), const<i32>(1), const<i32>(1), const<i32>(1), const<i32>(1));
// DEFAULT-NEXT:         let %102 fenv_clear: i32 [storage=automatic] = call<i32, signature=fn(i32) -> i32>(%0, or<i32>(or<i32>(or<i32>(or<i32>(const<i32>(32), const<i32>(4)), const<i32>(16)), const<i32>(8)), const<i32>(1)));
// DEFAULT-NEXT:         let %103 fenv_flags: i32 [storage=automatic] = call<i32, signature=fn(i32) -> i32>(%1, or<i32>(or<i32>(or<i32>(or<i32>(const<i32>(32), const<i32>(4)), const<i32>(16)), const<i32>(8)), const<i32>(1)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%3, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(88)>(%117)), array_decay<ptr<i8>, length=Some(5)>(%118), read<i32>(%65), read<i32>(%13), read<i32>(%14), read<i32>(%72), read<i32>(%73), read<i32>(%79), add<i32, overflow=ub>(read<i32>(field0(%81)), read<i32>(field1(%81))), add<i32, overflow=ub>(read<i32>(field0(%82)), read<i32>(field1(%82))), add<i32, overflow=ub>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%83), const<i32>(0)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%83), const<i32>(2))))), read<i32>(%85), read<i32, volatile>(%87), read<i32>(deref(read<ptr<i32>>(%89))), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(read<f64>(%90)), add<i32, overflow=ub>(read<i32>(field0(%91)), read<i32>(field1(%91))), read<i32>(%92), read<i32>(%93), read<i32>(%94), read<i32>(%95), read<i32>(%96), read<i32>(%97), call<i32, signature=fn(i32) -> i32>(%15, const<i32>(8)), call<i32, signature=fn(ptr<i32>) -> i32>(%24, array_decay<ptr<i32>, length=Some(3)>(%99)), read<i32>(%100), read<i32>(%101), const<i32>(17), read<i32>(%102), read<i32>(%103), call<i32, signature=fn(ptr<const i32>, ptr<const i32>) -> i32>(%21, pointer_cast<ptr<const i32>, reason=arg>(addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(%99), const<i32>(0))))), pointer_cast<ptr<const i32>, reason=arg>(addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(%99), const<i32>(1)))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
