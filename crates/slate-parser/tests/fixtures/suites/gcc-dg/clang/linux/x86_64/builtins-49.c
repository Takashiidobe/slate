/* { dg-do run } */
/* { dg-options "-O2" } */

extern double fabs(double);
extern float fabsf(float);
extern void abort(void);


double test1(double x)
{
  return fabs(-x);
}

float test1f(float x)
{
  return fabsf(-x);
}

double test2(double x)
{
  return fabs(fabs(x));
}

float test2f(float x)
{
  return fabsf(fabsf(x));
}

double test3(double x, double y)
{
  return fabs(x*-y);
}

float test3f(float x, float y)
{
  return fabsf(x*-y);
}

double test4(double x, double y)
{
  return fabs(x/-y);
}

float test4f(float x, float y)
{
  return fabsf(x/-y);
}

int main()
{
  if (test1(1.0) != 1.0)
    abort();
  if (test1(2.0) != 2.0)
    abort();
  if (test1(0.0) != 0.0)
    abort();
  if (test1(-1.0) != 1.0)
    abort();
  if (test1(-2.0) != 2.0)
    abort();

  if (test1f(1.0f) != 1.0f)
    abort();
  if (test1f(2.0f) != 2.0f)
    abort();
  if (test1f(0.0f) != 0.0f)
    abort();
  if (test1f(-1.0f) != 1.0f)
    abort();
  if (test1f(-2.0f) != 2.0f)
    abort();

  if (test2(1.0) != 1.0)
    abort();
  if (test2(2.0) != 2.0)
    abort();
  if (test2(0.0) != 0.0)
    abort();
  if (test2(-1.0) != 1.0)
    abort();
  if (test2(-2.0) != 2.0)
    abort();

  if (test2f(1.0f) != 1.0f)
    abort();
  if (test2f(2.0f) != 2.0f)
    abort();
  if (test2f(0.0f) != 0.0f)
    abort();
  if (test2f(-1.0f) != 1.0f)
    abort();
  if (test2f(-2.0f) != 2.0f)
    abort();

  if (test3(1.0,1.0) != 1.0)
    abort();
  if (test3(1.0,-1.0) != 1.0)
    abort();
  if (test3(1.0,2.0) != 2.0)
    abort();
  if (test3(1.0,-2.0) != 2.0)
    abort();
  if (test3(2.0,1.0) != 2.0)
    abort();
  if (test3(2.0,-1.0) != 2.0)
    abort();
  if (test3(2.0,2.0) != 4.0)
    abort();
  if (test3(2.0,-2.0) != 4.0)
    abort();
  if (test3(-2.0,1.0) != 2.0)
    abort();
  if (test3(-2.0,-1.0) != 2.0)
    abort();
  if (test3(-2.0,2.0) != 4.0)
    abort();
  if (test3(-2.0,-2.0) != 4.0)
    abort();

  if (test3f(1.0f,1.0f) != 1.0f)
    abort();
  if (test3f(1.0f,-1.0f) != 1.0f)
    abort();
  if (test3f(1.0f,2.0f) != 2.0f)
    abort();
  if (test3f(1.0f,-2.0f) != 2.0f)
    abort();
  if (test3f(2.0f,1.0f) != 2.0f)
    abort();
  if (test3f(2.0f,-1.0f) != 2.0f)
    abort();
  if (test3f(2.0f,2.0f) != 4.0f)
    abort();
  if (test3f(2.0f,-2.0f) != 4.0f)
    abort();
  if (test3f(-2.0f,1.0f) != 2.0f)
    abort();
  if (test3f(-2.0f,-1.0f) != 2.0f)
    abort();
  if (test3f(-2.0f,2.0f) != 4.0f)
    abort();
  if (test3f(-2.0f,-2.0f) != 4.0f)
    abort();

  if (test4(1.0,1.0) != 1.0)
    abort();
  if (test4(1.0,-1.0) != 1.0)
    abort();
  if (test4(-1.0,1.0) != 1.0)
    abort();
  if (test4(-1.0,-1.0) != 1.0)
    abort();
  if (test4(6.0,3.0) != 2.0)
    abort();
  if (test4(6.0,-3.0) != 2.0)
    abort();
  if (test4(-6.0,3.0) != 2.0)
    abort();
  if (test4(-6.0,-3.0) != 2.0)
    abort();

  if (test4f(1.0f,1.0f) != 1.0f)
    abort();
  if (test4f(1.0f,-1.0f) != 1.0f)
    abort();
  if (test4f(-1.0f,1.0f) != 1.0f)
    abort();
  if (test4f(-1.0f,-1.0f) != 1.0f)
    abort();
  if (test4f(6.0f,3.0f) != 2.0f)
    abort();
  if (test4f(6.0f,-3.0f) != 2.0f)
    abort();
  if (test4f(-6.0f,3.0f) != 2.0f)
    abort();
  if (test4f(-6.0f,-3.0f) != 2.0f)
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
// DEFAULT-NEXT:     fn %0 @fabs(%24 <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %1 @fabsf(%25 <unnamed>: f32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %2 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %3 @test1(%4 x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64) -> f64>(%0, neg<f64>(read<f64>(%4)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @test1f(%6 x: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32) -> f32>(%1, neg<f32>(read<f32>(%6)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @test2(%8 x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64) -> f64>(%0, call<f64, signature=fn(f64) -> f64>(%0, read<f64>(%8)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @test2f(%10 x: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32) -> f32>(%1, call<f32, signature=fn(f32) -> f32>(%1, read<f32>(%10)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @test3(%12 x: f64, %13 y: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64) -> f64>(%0, mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%12), neg<f64>(read<f64>(%13))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @test3f(%15 x: f32, %16 y: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32) -> f32>(%1, mul<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%15), neg<f32>(read<f32>(%16))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @test4(%18 x: f64, %19 y: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64) -> f64>(%0, div<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%18), neg<f64>(read<f64>(%19))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %20 @test4f(%21 x: f32, %22 y: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32) -> f32>(%1, div<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%21), neg<f32>(read<f32>(%22))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %23 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%3, const<f64>(1.0)), const<f64>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%3, const<f64>(2.0)), const<f64>(2.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%3, const<f64>(0.0)), const<f64>(0.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%3, neg<f64>(const<f64>(1.0))), const<f64>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%3, neg<f64>(const<f64>(2.0))), const<f64>(2.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32) -> f32>(%5, const<f32>(1.0)), const<f32>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32) -> f32>(%5, const<f32>(2.0)), const<f32>(2.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32) -> f32>(%5, const<f32>(0.0)), const<f32>(0.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32) -> f32>(%5, neg<f32>(const<f32>(1.0))), const<f32>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32) -> f32>(%5, neg<f32>(const<f32>(2.0))), const<f32>(2.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%7, const<f64>(1.0)), const<f64>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%7, const<f64>(2.0)), const<f64>(2.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%7, const<f64>(0.0)), const<f64>(0.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%7, neg<f64>(const<f64>(1.0))), const<f64>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%7, neg<f64>(const<f64>(2.0))), const<f64>(2.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32) -> f32>(%9, const<f32>(1.0)), const<f32>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32) -> f32>(%9, const<f32>(2.0)), const<f32>(2.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32) -> f32>(%9, const<f32>(0.0)), const<f32>(0.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32) -> f32>(%9, neg<f32>(const<f32>(1.0))), const<f32>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32) -> f32>(%9, neg<f32>(const<f32>(2.0))), const<f32>(2.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%11, const<f64>(1.0), const<f64>(1.0)), const<f64>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%11, const<f64>(1.0), neg<f64>(const<f64>(1.0))), const<f64>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%11, const<f64>(1.0), const<f64>(2.0)), const<f64>(2.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%11, const<f64>(1.0), neg<f64>(const<f64>(2.0))), const<f64>(2.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%11, const<f64>(2.0), const<f64>(1.0)), const<f64>(2.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%11, const<f64>(2.0), neg<f64>(const<f64>(1.0))), const<f64>(2.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%11, const<f64>(2.0), const<f64>(2.0)), const<f64>(4.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%11, const<f64>(2.0), neg<f64>(const<f64>(2.0))), const<f64>(4.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%11, neg<f64>(const<f64>(2.0)), const<f64>(1.0)), const<f64>(2.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%11, neg<f64>(const<f64>(2.0)), neg<f64>(const<f64>(1.0))), const<f64>(2.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%11, neg<f64>(const<f64>(2.0)), const<f64>(2.0)), const<f64>(4.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%11, neg<f64>(const<f64>(2.0)), neg<f64>(const<f64>(2.0))), const<f64>(4.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%14, const<f32>(1.0), const<f32>(1.0)), const<f32>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%14, const<f32>(1.0), neg<f32>(const<f32>(1.0))), const<f32>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%14, const<f32>(1.0), const<f32>(2.0)), const<f32>(2.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%14, const<f32>(1.0), neg<f32>(const<f32>(2.0))), const<f32>(2.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%14, const<f32>(2.0), const<f32>(1.0)), const<f32>(2.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%14, const<f32>(2.0), neg<f32>(const<f32>(1.0))), const<f32>(2.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%14, const<f32>(2.0), const<f32>(2.0)), const<f32>(4.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%14, const<f32>(2.0), neg<f32>(const<f32>(2.0))), const<f32>(4.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%14, neg<f32>(const<f32>(2.0)), const<f32>(1.0)), const<f32>(2.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%14, neg<f32>(const<f32>(2.0)), neg<f32>(const<f32>(1.0))), const<f32>(2.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%14, neg<f32>(const<f32>(2.0)), const<f32>(2.0)), const<f32>(4.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%14, neg<f32>(const<f32>(2.0)), neg<f32>(const<f32>(2.0))), const<f32>(4.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%17, const<f64>(1.0), const<f64>(1.0)), const<f64>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%17, const<f64>(1.0), neg<f64>(const<f64>(1.0))), const<f64>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%17, neg<f64>(const<f64>(1.0)), const<f64>(1.0)), const<f64>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%17, neg<f64>(const<f64>(1.0)), neg<f64>(const<f64>(1.0))), const<f64>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%17, const<f64>(6.0), const<f64>(3.0)), const<f64>(2.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%17, const<f64>(6.0), neg<f64>(const<f64>(3.0))), const<f64>(2.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%17, neg<f64>(const<f64>(6.0)), const<f64>(3.0)), const<f64>(2.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%17, neg<f64>(const<f64>(6.0)), neg<f64>(const<f64>(3.0))), const<f64>(2.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%20, const<f32>(1.0), const<f32>(1.0)), const<f32>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%20, const<f32>(1.0), neg<f32>(const<f32>(1.0))), const<f32>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%20, neg<f32>(const<f32>(1.0)), const<f32>(1.0)), const<f32>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%20, neg<f32>(const<f32>(1.0)), neg<f32>(const<f32>(1.0))), const<f32>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%20, const<f32>(6.0), const<f32>(3.0)), const<f32>(2.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%20, const<f32>(6.0), neg<f32>(const<f32>(3.0))), const<f32>(2.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%20, neg<f32>(const<f32>(6.0)), const<f32>(3.0)), const<f32>(2.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%20, neg<f32>(const<f32>(6.0)), neg<f32>(const<f32>(3.0))), const<f32>(2.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
