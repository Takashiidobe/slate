/* { dg-do run } */
/* { dg-options "-std=c11" } */
/* { dg-xfail-run-if "PR58757 -mieee is required to compare denormals" { alpha*-*-* } } */
/* { dg-skip-if "No subnormal support" { csky-*-* } { "-mhard-float" } } */

/* Test that the smallest positive value is not 0. This needs to be true
   even when denormals are not supported, so we do not pass any flag
   like -mieee.  */

#include <float.h>

int main(){
  volatile float f = FLT_TRUE_MIN;
  volatile double d = DBL_TRUE_MIN;
  volatile long double l = LDBL_TRUE_MIN;
  if (f == 0 || d == 0 || l == 0)
    __builtin_abort ();
  return 0;
}

// SLATE-FILECHECK-FLAVOR gcc
// SLATE-FILECHECK-STD DEFAULT c11
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
// DEFAULT-NEXT:     fn %4 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %0 @main(unprototyped) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %1 f: volatile f32 [storage=automatic] = const<f32>(1e-45);
// DEFAULT-NEXT:         let %2 d: volatile f64 [storage=automatic] = float_narrow<f64, reason=explicit, rounding=nearest_even, exceptions=observable>(const<f80>(4.94065645841246544177E-324));
// DEFAULT-NEXT:         let %3 l: volatile f80 [storage=automatic] = const<f80>(3.64519953188247460253E-4951);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(eq<f32, exceptions=observable>(read<f32, volatile>(%1), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0))), eq<f64, exceptions=observable>(read<f64, volatile>(%2), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)))), eq<f80, exceptions=observable>(read<f80, volatile>(%3), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
