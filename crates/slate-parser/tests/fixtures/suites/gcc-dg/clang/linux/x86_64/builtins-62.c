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
// DEFAULT-NEXT:     fn %[[VALUE___builtin_sin:[0-9]+]] @__builtin_sin(%[[VALUE0:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_cos:[0-9]+]] @__builtin_cos(%[[VALUE1:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test1:[0-9]+]] @test1(%[[VALUE_x:[0-9]+]] x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_s:[0-9]+]] s: f64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_c:[0-9]+]] c: f64 [storage=automatic];
// DEFAULT-NEXT:         write<f64>(%[[VALUE_s]], call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_sin]], read<f64>(%[[VALUE_x]])));
// DEFAULT-NEXT:         write<f64>(%[[VALUE_c]], call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_cos]], read<f64>(%[[VALUE_x]])));
// DEFAULT-NEXT:         return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%[[VALUE_s]]), read<f64>(%[[VALUE_c]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2:[0-9]+]] @test2(%[[VALUE_x_2:[0-9]+]] x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_s_2:[0-9]+]] s: f64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_c_2:[0-9]+]] c: f64 [storage=automatic];
// DEFAULT-NEXT:         write<f64>(%[[VALUE_x_2]], mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%[[VALUE_x_2]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2))));
// DEFAULT-NEXT:         write<f64>(%[[VALUE_s_2]], call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_sin]], read<f64>(%[[VALUE_x_2]])));
// DEFAULT-NEXT:         write<f64>(%[[VALUE_c_2]], call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_cos]], read<f64>(%[[VALUE_x_2]])));
// DEFAULT-NEXT:         return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%[[VALUE_s_2]]), read<f64>(%[[VALUE_c_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test3:[0-9]+]] @test3(%[[VALUE_x_3:[0-9]+]] x: f64, %[[VALUE_b:[0-9]+]] b: i32) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_s_3:[0-9]+]] s: f64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_c_3:[0-9]+]] c: f64 [storage=automatic];
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_b]]), const<i32>(0))
// DEFAULT-NEXT:             write<f64>(%[[VALUE_x_3]], mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%[[VALUE_x_3]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2))));
// DEFAULT-NEXT:         write<f64>(%[[VALUE_s_3]], call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_sin]], read<f64>(%[[VALUE_x_3]])));
// DEFAULT-NEXT:         write<f64>(%[[VALUE_c_3]], call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_cos]], read<f64>(%[[VALUE_x_3]])));
// DEFAULT-NEXT:         return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%[[VALUE_s_3]]), read<f64>(%[[VALUE_c_3]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test4:[0-9]+]] @test4(%[[VALUE_x_4:[0-9]+]] x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_s_4:[0-9]+]] s: f64 [storage=automatic];
// DEFAULT-NEXT:         write<f64>(%[[VALUE_x_4]], mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%[[VALUE_x_4]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2))));
// DEFAULT-NEXT:         write<f64>(%[[VALUE_s_4]], call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_sin]], read<f64>(%[[VALUE_x_4]])));
// DEFAULT-NEXT:         return read<f64>(%[[VALUE_s_4]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
