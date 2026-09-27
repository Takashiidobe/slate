/* { dg-do compile } */
/* { dg-options "-O -ffinite-math-only -fdump-tree-optimized" } */
/* { dg-require-effective-target c99_runtime } */

double test1 (double x)
{
  double s, c;
  s = __builtin_sin (x);
  c = __builtin_cos (x);
  return s + c;
}

double test2 (double x)
{
  double s, c;
  x = x * 2;
  s = __builtin_sin (x);
  c = __builtin_cos (x);
  return s + c;
}

double test3 (double x, int b)
{
  double s, c;
  if (b)
    x = x * 2;
  s = __builtin_sin (x);
  c = __builtin_cos (x);
  return s + c;
}

double test4 (double x)
{
  double s;
  x = x * 2;
  s = __builtin_sin (x);
  return s;
}

/* { dg-final { scan-tree-dump-times "cexpi" 3 "optimized" } } */

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
// DEFAULT-NEXT:     fn %17 @__builtin_sin(%16 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %19 @__builtin_cos(%18 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %0 @test1(%1 x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %2 s: f64 [storage=automatic];
// DEFAULT-NEXT:         let %3 c: f64 [storage=automatic];
// DEFAULT-NEXT:         write<f64>(%2, call<f64, signature=fn(f64) -> f64>(%17, read<f64>(%1)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%17, read<f64>(%1));
// DEFAULT-NEXT:         write<f64>(%3, call<f64, signature=fn(f64) -> f64>(%19, read<f64>(%1)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%19, read<f64>(%1));
// DEFAULT-NEXT:         return add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%2), read<f64>(%3));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @test2(%5 x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %6 s: f64 [storage=automatic];
// DEFAULT-NEXT:         let %7 c: f64 [storage=automatic];
// DEFAULT-NEXT:         write<f64>(%5, mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%5), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2))));
// DEFAULT-NEXT:         write<f64>(%6, call<f64, signature=fn(f64) -> f64>(%17, read<f64>(%5)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%17, read<f64>(%5));
// DEFAULT-NEXT:         write<f64>(%7, call<f64, signature=fn(f64) -> f64>(%19, read<f64>(%5)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%19, read<f64>(%5));
// DEFAULT-NEXT:         return add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%6), read<f64>(%7));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @test3(%9 x: f64, %10 b: i32) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %11 s: f64 [storage=automatic];
// DEFAULT-NEXT:         let %12 c: f64 [storage=automatic];
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%10), const<i32>(0))
// DEFAULT-NEXT:             write<f64>(%9, mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%9), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2))));
// DEFAULT-NEXT:         write<f64>(%11, call<f64, signature=fn(f64) -> f64>(%17, read<f64>(%9)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%17, read<f64>(%9));
// DEFAULT-NEXT:         write<f64>(%12, call<f64, signature=fn(f64) -> f64>(%19, read<f64>(%9)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%19, read<f64>(%9));
// DEFAULT-NEXT:         return add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%11), read<f64>(%12));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @test4(%14 x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %15 s: f64 [storage=automatic];
// DEFAULT-NEXT:         write<f64>(%14, mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%14), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2))));
// DEFAULT-NEXT:         write<f64>(%15, call<f64, signature=fn(f64) -> f64>(%17, read<f64>(%14)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%17, read<f64>(%14));
// DEFAULT-NEXT:         return read<f64>(%15);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
