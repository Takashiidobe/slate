/* { dg-do compile } */

/* Constant float values of type double in <float.h> are suffixed with L
   and cast to double so they can be used within code that uses pragma
   FLOAT_CONST_DECIMAL64.  If they were not suffixed then use of the macro
   would have them interpreted as _Decimal64, leading to errors when used
   in expressions with other operands of type double.  */

#include <float.h>

extern double a, b, c, d;

void
foo ()
{
  _Pragma ("STDC FLOAT_CONST_DECIMAL64 ON")
  a = 0.1d * DBL_MAX;
  b = DBL_EPSILON * 10.0d;
  c = DBL_MIN * 200.0d;
}

// SLATE-FILECHECK-STD DEFAULT gnu99
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
// DEFAULT-NEXT:     extern %[[VALUE_a:[0-9]+]] a: f64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE_b:[0-9]+]] b: f64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE_c:[0-9]+]] c: f64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE_d:[0-9]+]] d: f64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<f64>(%[[VALUE_a]], mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(const<f64>(0.1), float_narrow<f64, reason=explicit, rounding=nearest_even, exceptions=observable>(const<f80>(1.79769313486231570815E+308))));
// DEFAULT-NEXT:         write<f64>(%[[VALUE_b]], mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(float_narrow<f64, reason=explicit, rounding=nearest_even, exceptions=observable>(const<f80>(2.22044604925031308085E-16)), const<f64>(10.0)));
// DEFAULT-NEXT:         write<f64>(%[[VALUE_c]], mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(float_narrow<f64, reason=explicit, rounding=nearest_even, exceptions=observable>(const<f80>(2.22507385850720138309E-308)), const<f64>(200.0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
