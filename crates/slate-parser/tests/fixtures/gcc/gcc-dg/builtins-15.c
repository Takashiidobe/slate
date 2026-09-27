/* Derived from PR optimization/10764  */

/* { dg-do compile } */
/* { dg-options "-O2 -ffast-math" } */

double log(double x);

double ndtri(double y0)
{
  double x;

  x = log(y0);
  x = log(x);

  return x;
}


// SLATE-FILECHECK-FLAVOR gcc
// SLATE-FILECHECK-STD DEFAULT gnu23
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
// DEFAULT-NEXT:     fn %0 @log(%4 x: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %1 @ndtri(%2 y0: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %3 x: f64 [storage=automatic];
// DEFAULT-NEXT:         write<f64>(%3, call<f64, signature=fn(f64) -> f64>(%0, read<f64>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%0, read<f64>(%2));
// DEFAULT-NEXT:         write<f64>(%3, call<f64, signature=fn(f64) -> f64>(%0, read<f64>(%3)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%0, read<f64>(%3));
// DEFAULT-NEXT:         return read<f64>(%3);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
