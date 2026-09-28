/* Test static initializer folding of implicit conversions to floating point
   types, even with -frounding-math and related options.  Bug 103031.  */
/* { dg-do compile } */
/* { dg-options "-frounding-math -ftrapping-math -fsignaling-nans" } */

float f1 = -1ULL;
float f2 = __DBL_MAX__;
float f3 = __DBL_MIN__;
float f4 = 0.1;
float f5 = __builtin_nans ("");
double d1 = -1ULL;

// SLATE-FILECHECK-STD DEFAULT gnu23
// SLATE-FILECHECK-ARGS -frounding-math -ftrapping-math
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
// DEFAULT-NEXT:     global %0 f1: f32 [storage=static] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(neg<u64, overflow=wrap>(const<u64>(1))) [linkage=external];
// DEFAULT-NEXT:     global %1 f2: f32 [storage=static] = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(float_narrow<f64, reason=explicit, rounding=nearest_even, exceptions=ignore>(const<f80>(1.79769313486231570815E+308))) [linkage=external];
// DEFAULT-NEXT:     global %2 f3: f32 [storage=static] = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(float_narrow<f64, reason=explicit, rounding=nearest_even, exceptions=ignore>(const<f80>(2.22507385850720138309E-308))) [linkage=external];
// DEFAULT-NEXT:     global %3 f4: f32 [storage=static] = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(0.1)) [linkage=external];
// DEFAULT-NEXT:     global %8 .str8: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %4 f5: f32 [storage=static] = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(call<f64, signature=fn(ptr<const i8>) -> f64>(%7, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%8)))) [linkage=external];
// DEFAULT-NEXT:     global %5 d1: f64 [storage=static] = int_to_float<f64, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(neg<u64, overflow=wrap>(const<u64>(1))) [linkage=external];
// DEFAULT-NEXT:     fn %7 @__builtin_nans(%6 <unnamed>: ptr<const i8>) -> f64 [linkage=external] [memory=read];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
