/* { dg-do run } */
/* { dg-options "-O2 -ffast-math" } */

extern double pow(double, double);
extern double fabs(double);
extern void abort(void);

double test2_1(double x)
{
  return pow(x,2.0);
}

double test2_2(double x)
{
  return pow(-x,2.0);
}

double test2_3(double x)
{
  return pow(fabs(x),2.0);
}

double test3_1(double x)
{
  return pow(x,3.0);
}

double test3_2(double x)
{
  return pow(-x,3.0);
}

double test3_3(double x)
{
  return pow(fabs(x),3.0);
}

double test6_1(double x)
{
  return pow(x,6.0);
}

double test6_2(double x)
{
  return pow(-x,6.0);
}

double test6_3(double x)
{
  return pow(fabs(x),6.0);
}


int main()
{
  if (test2_1(1.0) != 1.0)
    abort();
  if (test2_1(2.0) != 4.0)
    abort();
  if (test2_1(0.0) != 0.0)
    abort();
  if (test2_1(-1.0) != 1.0)
    abort();
  if (test2_1(-2.0) != 4.0)
    abort();

  if (test2_2(1.0) != 1.0)
    abort();
  if (test2_2(2.0) != 4.0)
    abort();
  if (test2_2(0.0) != 0.0)
    abort();
  if (test2_2(-1.0) != 1.0)
    abort();
  if (test2_2(-2.0) != 4.0)
    abort();

  if (test2_3(1.0) != 1.0)
    abort();
  if (test2_3(2.0) != 4.0)
    abort();
  if (test2_3(0.0) != 0.0)
    abort();
  if (test2_3(-1.0) != 1.0)
    abort();
  if (test2_3(2.0) != 4.0)
    abort();

  if (test3_1(1.0) != 1.0)
    abort();
  if (test3_1(2.0) != 8.0)
    abort();
  if (test3_1(0.0) != 0.0)
    abort();
  if (test3_1(-1.0) != -1.0)
    abort();
  if (test3_1(-2.0) != -8.0)
    abort();

  if (test3_2(1.0) != -1.0)
    abort();
  if (test3_2(2.0) != -8.0)
    abort();
  if (test3_2(0.0) != -0.0)
    abort();
  if (test3_2(-1.0) != 1.0)
    abort();
  if (test3_2(-2.0) != 8.0)
    abort();

  if (test3_3(1.0) != 1.0)
    abort();
  if (test3_3(2.0) != 8.0)
    abort();
  if (test3_3(0.0) != 0.0)
    abort();
  if (test3_3(-1.0) != 1.0)
    abort();
  if (test3_3(-2.0) != 8.0)
    abort();

  if (test6_1(1.0) != 1.0)
    abort();
  if (test6_1(2.0) != 64.0)
    abort();
  if (test6_1(0.0) != 0.0)
    abort();
  if (test6_1(-1.0) != 1.0)
    abort();
  if (test6_1(-2.0) != 64.0)
    abort();

  if (test6_2(1.0) != 1.0)
    abort();
  if (test6_2(2.0) != 64.0)
    abort();
  if (test6_2(0.0) != 0.0)
    abort();
  if (test6_2(-1.0) != 1.0)
    abort();
  if (test6_2(-2.0) != 64.0)
    abort();

  if (test6_3(1.0) != 1.0)
    abort();
  if (test6_3(2.0) != 64.0)
    abort();
  if (test6_3(0.0) != 0.0)
    abort();
  if (test6_3(-1.0) != 1.0)
    abort();
  if (test6_3(-2.0) != 64.0)
    abort();

  return 0;
}

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
// DEFAULT-NEXT:     fn %[[VALUE_pow:[0-9]+]] @pow(%[[VALUE0:[0-9]+]] <unnamed>: f64, %[[VALUE1:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fabs:[0-9]+]] @fabs(%[[VALUE2:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_test2_1:[0-9]+]] @test2_1(%[[VALUE_x:[0-9]+]] x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_pow]], read<f64>(%[[VALUE_x]]), const<f64>(2.0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2_2:[0-9]+]] @test2_2(%[[VALUE_x_2:[0-9]+]] x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_pow]], neg<f64>(read<f64>(%[[VALUE_x_2]])), const<f64>(2.0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2_3:[0-9]+]] @test2_3(%[[VALUE_x_3:[0-9]+]] x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_pow]], call<f64, signature=fn(f64) -> f64>(%[[VALUE_fabs]], read<f64>(%[[VALUE_x_3]])), const<f64>(2.0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test3_1:[0-9]+]] @test3_1(%[[VALUE_x_4:[0-9]+]] x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_pow]], read<f64>(%[[VALUE_x_4]]), const<f64>(3.0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test3_2:[0-9]+]] @test3_2(%[[VALUE_x_5:[0-9]+]] x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_pow]], neg<f64>(read<f64>(%[[VALUE_x_5]])), const<f64>(3.0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test3_3:[0-9]+]] @test3_3(%[[VALUE_x_6:[0-9]+]] x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_pow]], call<f64, signature=fn(f64) -> f64>(%[[VALUE_fabs]], read<f64>(%[[VALUE_x_6]])), const<f64>(3.0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test6_1:[0-9]+]] @test6_1(%[[VALUE_x_7:[0-9]+]] x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_pow]], read<f64>(%[[VALUE_x_7]]), const<f64>(6.0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test6_2:[0-9]+]] @test6_2(%[[VALUE_x_8:[0-9]+]] x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_pow]], neg<f64>(read<f64>(%[[VALUE_x_8]])), const<f64>(6.0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test6_3:[0-9]+]] @test6_3(%[[VALUE_x_9:[0-9]+]] x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_pow]], call<f64, signature=fn(f64) -> f64>(%[[VALUE_fabs]], read<f64>(%[[VALUE_x_9]])), const<f64>(6.0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_test2_1]], const<f64>(1.0)), const<f64>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_test2_1]], const<f64>(2.0)), const<f64>(4.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_test2_1]], const<f64>(0.0)), const<f64>(0.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_test2_1]], neg<f64>(const<f64>(1.0))), const<f64>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_test2_1]], neg<f64>(const<f64>(2.0))), const<f64>(4.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_test2_2]], const<f64>(1.0)), const<f64>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_test2_2]], const<f64>(2.0)), const<f64>(4.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_test2_2]], const<f64>(0.0)), const<f64>(0.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_test2_2]], neg<f64>(const<f64>(1.0))), const<f64>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_test2_2]], neg<f64>(const<f64>(2.0))), const<f64>(4.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_test2_3]], const<f64>(1.0)), const<f64>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_test2_3]], const<f64>(2.0)), const<f64>(4.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_test2_3]], const<f64>(0.0)), const<f64>(0.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_test2_3]], neg<f64>(const<f64>(1.0))), const<f64>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_test2_3]], const<f64>(2.0)), const<f64>(4.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_test3_1]], const<f64>(1.0)), const<f64>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_test3_1]], const<f64>(2.0)), const<f64>(8.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_test3_1]], const<f64>(0.0)), const<f64>(0.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_test3_1]], neg<f64>(const<f64>(1.0))), neg<f64>(const<f64>(1.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_test3_1]], neg<f64>(const<f64>(2.0))), neg<f64>(const<f64>(8.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_test3_2]], const<f64>(1.0)), neg<f64>(const<f64>(1.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_test3_2]], const<f64>(2.0)), neg<f64>(const<f64>(8.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_test3_2]], const<f64>(0.0)), neg<f64>(const<f64>(0.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_test3_2]], neg<f64>(const<f64>(1.0))), const<f64>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_test3_2]], neg<f64>(const<f64>(2.0))), const<f64>(8.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_test3_3]], const<f64>(1.0)), const<f64>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_test3_3]], const<f64>(2.0)), const<f64>(8.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_test3_3]], const<f64>(0.0)), const<f64>(0.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_test3_3]], neg<f64>(const<f64>(1.0))), const<f64>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_test3_3]], neg<f64>(const<f64>(2.0))), const<f64>(8.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_test6_1]], const<f64>(1.0)), const<f64>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_test6_1]], const<f64>(2.0)), const<f64>(64.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_test6_1]], const<f64>(0.0)), const<f64>(0.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_test6_1]], neg<f64>(const<f64>(1.0))), const<f64>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_test6_1]], neg<f64>(const<f64>(2.0))), const<f64>(64.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_test6_2]], const<f64>(1.0)), const<f64>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_test6_2]], const<f64>(2.0)), const<f64>(64.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_test6_2]], const<f64>(0.0)), const<f64>(0.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_test6_2]], neg<f64>(const<f64>(1.0))), const<f64>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_test6_2]], neg<f64>(const<f64>(2.0))), const<f64>(64.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_test6_3]], const<f64>(1.0)), const<f64>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_test6_3]], const<f64>(2.0)), const<f64>(64.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_test6_3]], const<f64>(0.0)), const<f64>(0.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_test6_3]], neg<f64>(const<f64>(1.0))), const<f64>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_test6_3]], neg<f64>(const<f64>(2.0))), const<f64>(64.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
