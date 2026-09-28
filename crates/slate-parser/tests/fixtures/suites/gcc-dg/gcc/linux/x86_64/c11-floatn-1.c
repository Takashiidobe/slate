/* { dg-do compile } */
/* { dg-options "-std=c11 -pedantic-errors" } */
/* { dg-add-options float32 } */
/* { dg-add-options float64 } */
/* { dg-add-options float32x } */
/* { dg-require-effective-target float32 } */
/* { dg-require-effective-target float32x } */
/* { dg-require-effective-target float64 } */

_Float32 a		/* { dg-error "ISO C does not support the '_Float32' type before C23" } */
  = 1.0F32;		/* { dg-error "non-standard suffix on floating constant before C23" } */
_Float64 b		/* { dg-error "ISO C does not support the '_Float64' type before C23" } */
  = 1.0F64;		/* { dg-error "non-standard suffix on floating constant before C23" } */
_Float32x c		/* { dg-error "ISO C does not support the '_Float32x' type before C23" } */
  = 1.0F32x;		/* { dg-error "non-standard suffix on floating constant before C23" } */
__extension__ _Float32 d
  = 2.0F32;
__extension__ _Float64 e
  = 2.0F64;
__extension__ _Float32x f
  = 2.0F32x;
// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-STD DEFAULT c11

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
// DEFAULT-NEXT:     global %0 a: f32 [storage=static] = const<f32>(1.0) [linkage=external];
// DEFAULT-NEXT:     global %1 b: f64 [storage=static] = const<f64>(1.0) [linkage=external];
// DEFAULT-NEXT:     global %2 c: f64 [storage=static] = const<f64>(1.0) [linkage=external];
// DEFAULT-NEXT:     global %3 d: f32 [storage=static] = const<f32>(2.0) [linkage=external];
// DEFAULT-NEXT:     global %4 e: f64 [storage=static] = const<f64>(2.0) [linkage=external];
// DEFAULT-NEXT:     global %5 f: f64 [storage=static] = const<f64>(2.0) [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
