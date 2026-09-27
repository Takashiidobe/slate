/* { dg-do compile } */

/* N1150 6: Constants.
   C99 6.4.4.2: Floating constants.  */

_Decimal32 a = 1.1df;
_Decimal32 b = -.003DF;
_Decimal64 c = 11e-1dl;
_Decimal64 d = -.3DL;
_Decimal128 e = 000.3e0dl;
_Decimal128 f = 3000300030003e0DL;

// SLATE-FILECHECK-FLAVOR gcc
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
// DEFAULT-NEXT:     global %0 a: d32 [storage=static] = const<d32>(1.1) [linkage=external];
// DEFAULT-NEXT:     global %1 b: d32 [storage=static] = neg<d32>(const<d32>(.003)) [linkage=external];
// DEFAULT-NEXT:     global %2 c: d64 [storage=static] = float_narrow<d64, reason=assign, rounding=nearest_even, exceptions=ignore>(const<d128>(11e-1)) [linkage=external];
// DEFAULT-NEXT:     global %3 d: d64 [storage=static] = float_narrow<d64, reason=assign, rounding=nearest_even, exceptions=ignore>(neg<d128>(const<d128>(.3))) [linkage=external];
// DEFAULT-NEXT:     global %4 e: d128 [storage=static] = const<d128>(000.3e0) [linkage=external];
// DEFAULT-NEXT:     global %5 f: d128 [storage=static] = const<d128>(3000300030003e0) [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
