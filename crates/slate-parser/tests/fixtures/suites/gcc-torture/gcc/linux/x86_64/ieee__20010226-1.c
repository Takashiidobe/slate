/* { dg-do run } */
#include <float.h>

void abort(void);

long double   dfrom = 1.1L;
long double   m1;
long double   m2;
unsigned long mant_long;

int main() {
  /* Some targets don't support a conforming long double type.  This is
     common with very small parts which set long double == float.   Look
     to see if the type has at least 32 bits of precision.  */
  if (LDBL_EPSILON > 0x1p-31L)
    return 0;

  m1        = dfrom / 2.0L;
  m2        = m1 * 4294967296.0L;
  mant_long = ((unsigned long)m2) & 0xffffffff;

  if (mant_long == 0x8ccccccc)
    return 0;
  else
    abort();
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
// DEFAULT-NEXT:     global %[[VALUE_dfrom:[0-9]+]] dfrom: f80 [storage=static] = const<f80>(1.10000000000000000002) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_m1:[0-9]+]] m1: f80 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_m2:[0-9]+]] m2: f80 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_mant_long:[0-9]+]] mant_long: u64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if gt<f80, exceptions=observable>(const<f80>(1.08420217248550443401E-19), const<f80>(4.65661287307739257813E-10))
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:         write<f80>(%[[VALUE_m1]], div<f80, rounding=nearest_even, exceptions=observable, contract=fast>(read<f80>(%[[VALUE_dfrom]]), const<f80>(2)));
// DEFAULT-NEXT:         write<f80>(%[[VALUE_m2]], mul<f80, rounding=nearest_even, exceptions=observable, contract=fast>(read<f80>(%[[VALUE_m1]]), const<f80>(4294967296)));
// DEFAULT-NEXT:         write<u64>(%[[VALUE_mant_long]], and<u64>(float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=observable>(read<f80>(%[[VALUE_m2]])), widen<u64, reason=usual_arith>(const<u32>(4294967295))));
// DEFAULT-NEXT:         if eq<u64>(read<u64>(%[[VALUE_mant_long]]), widen<u64, reason=usual_arith>(const<u32>(2362232012)))
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
