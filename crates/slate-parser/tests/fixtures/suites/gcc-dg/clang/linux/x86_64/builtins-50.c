/* { dg-do run } */
/* { dg-options "-O2" } */

extern double copysign(double,double);
extern float copysignf(float,float);
extern double fabs(double);
extern float fabsf(float);
extern void abort(void);


double test1(double x, double y)
{
  return copysign(-x,y);
}

float test1f(float x, float y)
{
  return copysignf(-x,y);
}

double test2(double x, double y)
{
  return copysign(fabs(x),y);
}

float test2f(float x, float y)
{
  return copysignf(fabsf(x),y);
}

double test3(double x, double y, double z)
{
  return copysign(x*-y,z);
}

float test3f(float x, float y, float z)
{
  return copysignf(x*-y,z);
}

double test4(double x, double y, double z)
{
  return copysign(x/-y,z);
}

float test4f(float x, float y, float z)
{
  return copysignf(x/-y,z);
}

int main()
{
  if (test1(3.0,2.0) != 3.0)
    abort();
  if (test1(3.0,-2.0) != -3.0)
    abort();
  if (test1(-3.0,2.0) != 3.0)
    abort();
  if (test1(-3.0,-2.0) != -3.0)
    abort();

  if (test1f(3.0f,2.0f) != 3.0f)
    abort();
  if (test1f(3.0f,-2.0f) != -3.0f)
    abort();
  if (test1f(-3.0f,2.0f) != 3.0f)
    abort();
  if (test1f(-3.0f,-2.0f) != -3.0f)
    abort();

  if (test2(3.0,2.0) != 3.0)
    abort();
  if (test2(3.0,-2.0) != -3.0)
    abort();
  if (test2(-3.0,2.0) != 3.0)
    abort();
  if (test2(-3.0,-2.0) != -3.0)
    abort();

  if (test2f(3.0f,2.0f) != 3.0f)
    abort();
  if (test2f(3.0f,-2.0f) != -3.0f)
    abort();
  if (test2f(-3.0f,2.0f) != 3.0f)
    abort();
  if (test2f(-3.0f,-2.0f) != -3.0f)
    abort();

  if (test3(2.0,3.0,4.0) != 6.0)
    abort();
  if (test3(2.0,3.0,-4.0) != -6.0)
    abort();
  if (test3(2.0,-3.0,4.0) != 6.0)
    abort();
  if (test3(2.0,-3.0,-4.0) != -6.0)
    abort();
  if (test3(-2.0,3.0,4.0) != 6.0)
    abort();
  if (test3(-2.0,3.0,-4.0) != -6.0)
    abort();
  if (test3(-2.0,-3.0,4.0) != 6.0)
    abort();
  if (test3(-2.0,-3.0,-4.0) != -6.0)
    abort();

  if (test3f(2.0f,3.0f,4.0f) != 6.0f)
    abort();
  if (test3f(2.0f,3.0f,-4.0f) != -6.0f)
    abort();
  if (test3f(2.0f,-3.0f,4.0f) != 6.0f)
    abort();
  if (test3f(2.0f,-3.0f,-4.0f) != -6.0f)
    abort();
  if (test3f(-2.0f,3.0f,4.0f) != 6.0f)
    abort();
  if (test3f(-2.0f,3.0f,-4.0f) != -6.0f)
    abort();
  if (test3f(-2.0f,-3.0f,4.0f) != 6.0f)
    abort();
  if (test3f(-2.0f,-3.0f,-4.0f) != -6.0f)
    abort();

  if (test4(8.0,2.0,3.0) != 4.0)
    abort();
  if (test4(8.0,2.0,-3.0) != -4.0)
    abort();
  if (test4(8.0,-2.0,3.0) != 4.0)
    abort();
  if (test4(8.0,-2.0,-3.0) != -4.0)
    abort();
  if (test4(-8.0,2.0,3.0) != 4.0)
    abort();
  if (test4(-8.0,2.0,-3.0) != -4.0)
    abort();
  if (test4(-8.0,-2.0,3.0) != 4.0)
    abort();
  if (test4(-8.0,-2.0,-3.0) != -4.0)
    abort();

  if (test4f(8.0f,2.0f,3.0f) != 4.0f)
    abort();
  if (test4f(8.0f,2.0f,-3.0f) != -4.0f)
    abort();
  if (test4f(8.0f,-2.0f,3.0f) != 4.0f)
    abort();
  if (test4f(8.0f,-2.0f,-3.0f) != -4.0f)
    abort();
  if (test4f(-8.0f,2.0f,3.0f) != 4.0f)
    abort();
  if (test4f(-8.0f,2.0f,-3.0f) != -4.0f)
    abort();
  if (test4f(-8.0f,-2.0f,3.0f) != 4.0f)
    abort();
  if (test4f(-8.0f,-2.0f,-3.0f) != -4.0f)
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
// DEFAULT-NEXT:     fn %0 @copysign(%34 <unnamed>: f64, %35 <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %1 @copysignf(%36 <unnamed>: f32, %37 <unnamed>: f32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %2 @fabs(%38 <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %3 @fabsf(%39 <unnamed>: f32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %4 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %5 @test1(%6 x: f64, %7 y: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64, f64) -> f64>(%0, neg<f64>(read<f64>(%6)), read<f64>(%7));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @test1f(%9 x: f32, %10 y: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32, f32) -> f32>(%1, neg<f32>(read<f32>(%9)), read<f32>(%10));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @test2(%12 x: f64, %13 y: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64, f64) -> f64>(%0, call<f64, signature=fn(f64) -> f64>(%2, read<f64>(%12)), read<f64>(%13));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @test2f(%15 x: f32, %16 y: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32, f32) -> f32>(%1, call<f32, signature=fn(f32) -> f32>(%3, read<f32>(%15)), read<f32>(%16));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @test3(%18 x: f64, %19 y: f64, %20 z: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64, f64) -> f64>(%0, mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%18), neg<f64>(read<f64>(%19))), read<f64>(%20));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %21 @test3f(%22 x: f32, %23 y: f32, %24 z: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32, f32) -> f32>(%1, mul<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%22), neg<f32>(read<f32>(%23))), read<f32>(%24));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %25 @test4(%26 x: f64, %27 y: f64, %28 z: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64, f64) -> f64>(%0, div<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%26), neg<f64>(read<f64>(%27))), read<f64>(%28));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %29 @test4f(%30 x: f32, %31 y: f32, %32 z: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32, f32) -> f32>(%1, div<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%30), neg<f32>(read<f32>(%31))), read<f32>(%32));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %33 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%5, const<f64>(3.0), const<f64>(2.0)), const<f64>(3.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%5, const<f64>(3.0), neg<f64>(const<f64>(2.0))), neg<f64>(const<f64>(3.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%5, neg<f64>(const<f64>(3.0)), const<f64>(2.0)), const<f64>(3.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%5, neg<f64>(const<f64>(3.0)), neg<f64>(const<f64>(2.0))), neg<f64>(const<f64>(3.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%8, const<f32>(3.0), const<f32>(2.0)), const<f32>(3.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%8, const<f32>(3.0), neg<f32>(const<f32>(2.0))), neg<f32>(const<f32>(3.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%8, neg<f32>(const<f32>(3.0)), const<f32>(2.0)), const<f32>(3.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%8, neg<f32>(const<f32>(3.0)), neg<f32>(const<f32>(2.0))), neg<f32>(const<f32>(3.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%11, const<f64>(3.0), const<f64>(2.0)), const<f64>(3.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%11, const<f64>(3.0), neg<f64>(const<f64>(2.0))), neg<f64>(const<f64>(3.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%11, neg<f64>(const<f64>(3.0)), const<f64>(2.0)), const<f64>(3.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%11, neg<f64>(const<f64>(3.0)), neg<f64>(const<f64>(2.0))), neg<f64>(const<f64>(3.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%14, const<f32>(3.0), const<f32>(2.0)), const<f32>(3.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%14, const<f32>(3.0), neg<f32>(const<f32>(2.0))), neg<f32>(const<f32>(3.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%14, neg<f32>(const<f32>(3.0)), const<f32>(2.0)), const<f32>(3.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%14, neg<f32>(const<f32>(3.0)), neg<f32>(const<f32>(2.0))), neg<f32>(const<f32>(3.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64, f64) -> f64>(%17, const<f64>(2.0), const<f64>(3.0), const<f64>(4.0)), const<f64>(6.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64, f64) -> f64>(%17, const<f64>(2.0), const<f64>(3.0), neg<f64>(const<f64>(4.0))), neg<f64>(const<f64>(6.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64, f64) -> f64>(%17, const<f64>(2.0), neg<f64>(const<f64>(3.0)), const<f64>(4.0)), const<f64>(6.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64, f64) -> f64>(%17, const<f64>(2.0), neg<f64>(const<f64>(3.0)), neg<f64>(const<f64>(4.0))), neg<f64>(const<f64>(6.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64, f64) -> f64>(%17, neg<f64>(const<f64>(2.0)), const<f64>(3.0), const<f64>(4.0)), const<f64>(6.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64, f64) -> f64>(%17, neg<f64>(const<f64>(2.0)), const<f64>(3.0), neg<f64>(const<f64>(4.0))), neg<f64>(const<f64>(6.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64, f64) -> f64>(%17, neg<f64>(const<f64>(2.0)), neg<f64>(const<f64>(3.0)), const<f64>(4.0)), const<f64>(6.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64, f64) -> f64>(%17, neg<f64>(const<f64>(2.0)), neg<f64>(const<f64>(3.0)), neg<f64>(const<f64>(4.0))), neg<f64>(const<f64>(6.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32, f32) -> f32>(%21, const<f32>(2.0), const<f32>(3.0), const<f32>(4.0)), const<f32>(6.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32, f32) -> f32>(%21, const<f32>(2.0), const<f32>(3.0), neg<f32>(const<f32>(4.0))), neg<f32>(const<f32>(6.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32, f32) -> f32>(%21, const<f32>(2.0), neg<f32>(const<f32>(3.0)), const<f32>(4.0)), const<f32>(6.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32, f32) -> f32>(%21, const<f32>(2.0), neg<f32>(const<f32>(3.0)), neg<f32>(const<f32>(4.0))), neg<f32>(const<f32>(6.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32, f32) -> f32>(%21, neg<f32>(const<f32>(2.0)), const<f32>(3.0), const<f32>(4.0)), const<f32>(6.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32, f32) -> f32>(%21, neg<f32>(const<f32>(2.0)), const<f32>(3.0), neg<f32>(const<f32>(4.0))), neg<f32>(const<f32>(6.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32, f32) -> f32>(%21, neg<f32>(const<f32>(2.0)), neg<f32>(const<f32>(3.0)), const<f32>(4.0)), const<f32>(6.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32, f32) -> f32>(%21, neg<f32>(const<f32>(2.0)), neg<f32>(const<f32>(3.0)), neg<f32>(const<f32>(4.0))), neg<f32>(const<f32>(6.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64, f64) -> f64>(%25, const<f64>(8.0), const<f64>(2.0), const<f64>(3.0)), const<f64>(4.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64, f64) -> f64>(%25, const<f64>(8.0), const<f64>(2.0), neg<f64>(const<f64>(3.0))), neg<f64>(const<f64>(4.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64, f64) -> f64>(%25, const<f64>(8.0), neg<f64>(const<f64>(2.0)), const<f64>(3.0)), const<f64>(4.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64, f64) -> f64>(%25, const<f64>(8.0), neg<f64>(const<f64>(2.0)), neg<f64>(const<f64>(3.0))), neg<f64>(const<f64>(4.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64, f64) -> f64>(%25, neg<f64>(const<f64>(8.0)), const<f64>(2.0), const<f64>(3.0)), const<f64>(4.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64, f64) -> f64>(%25, neg<f64>(const<f64>(8.0)), const<f64>(2.0), neg<f64>(const<f64>(3.0))), neg<f64>(const<f64>(4.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64, f64) -> f64>(%25, neg<f64>(const<f64>(8.0)), neg<f64>(const<f64>(2.0)), const<f64>(3.0)), const<f64>(4.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64, f64) -> f64>(%25, neg<f64>(const<f64>(8.0)), neg<f64>(const<f64>(2.0)), neg<f64>(const<f64>(3.0))), neg<f64>(const<f64>(4.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32, f32) -> f32>(%29, const<f32>(8.0), const<f32>(2.0), const<f32>(3.0)), const<f32>(4.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32, f32) -> f32>(%29, const<f32>(8.0), const<f32>(2.0), neg<f32>(const<f32>(3.0))), neg<f32>(const<f32>(4.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32, f32) -> f32>(%29, const<f32>(8.0), neg<f32>(const<f32>(2.0)), const<f32>(3.0)), const<f32>(4.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32, f32) -> f32>(%29, const<f32>(8.0), neg<f32>(const<f32>(2.0)), neg<f32>(const<f32>(3.0))), neg<f32>(const<f32>(4.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32, f32) -> f32>(%29, neg<f32>(const<f32>(8.0)), const<f32>(2.0), const<f32>(3.0)), const<f32>(4.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32, f32) -> f32>(%29, neg<f32>(const<f32>(8.0)), const<f32>(2.0), neg<f32>(const<f32>(3.0))), neg<f32>(const<f32>(4.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32, f32) -> f32>(%29, neg<f32>(const<f32>(8.0)), neg<f32>(const<f32>(2.0)), const<f32>(3.0)), const<f32>(4.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32, f32) -> f32>(%29, neg<f32>(const<f32>(8.0)), neg<f32>(const<f32>(2.0)), neg<f32>(const<f32>(3.0))), neg<f32>(const<f32>(4.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
