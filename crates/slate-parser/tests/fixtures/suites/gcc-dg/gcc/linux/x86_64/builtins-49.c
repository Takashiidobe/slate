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
// DEFAULT-NEXT:     fn %[[VALUE_fabs:[0-9]+]] @fabs(%[[VALUE0:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_fabsf:[0-9]+]] @fabsf(%[[VALUE1:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_test1:[0-9]+]] @test1(%[[VALUE_x:[0-9]+]] x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64) -> f64>(%[[VALUE_fabs]], neg<f64>(read<f64>(%[[VALUE_x]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test1f:[0-9]+]] @test1f(%[[VALUE_x_2:[0-9]+]] x: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32) -> f32>(%[[VALUE_fabsf]], neg<f32>(read<f32>(%[[VALUE_x_2]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2:[0-9]+]] @test2(%[[VALUE_x_3:[0-9]+]] x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64) -> f64>(%[[VALUE_fabs]], call<f64, signature=fn(f64) -> f64>(%[[VALUE_fabs]], read<f64>(%[[VALUE_x_3]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2f:[0-9]+]] @test2f(%[[VALUE_x_4:[0-9]+]] x: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32) -> f32>(%[[VALUE_fabsf]], call<f32, signature=fn(f32) -> f32>(%[[VALUE_fabsf]], read<f32>(%[[VALUE_x_4]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test3:[0-9]+]] @test3(%[[VALUE_x_5:[0-9]+]] x: f64, %[[VALUE_y:[0-9]+]] y: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64) -> f64>(%[[VALUE_fabs]], mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE_x_5]]), neg<f64>(read<f64>(%[[VALUE_y]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test3f:[0-9]+]] @test3f(%[[VALUE_x_6:[0-9]+]] x: f32, %[[VALUE_y_2:[0-9]+]] y: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32) -> f32>(%[[VALUE_fabsf]], mul<f32, rounding=nearest_even, exceptions=observable, contract=fast>(read<f32>(%[[VALUE_x_6]]), neg<f32>(read<f32>(%[[VALUE_y_2]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test4:[0-9]+]] @test4(%[[VALUE_x_7:[0-9]+]] x: f64, %[[VALUE_y_3:[0-9]+]] y: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64) -> f64>(%[[VALUE_fabs]], div<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE_x_7]]), neg<f64>(read<f64>(%[[VALUE_y_3]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test4f:[0-9]+]] @test4f(%[[VALUE_x_8:[0-9]+]] x: f32, %[[VALUE_y_4:[0-9]+]] y: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32) -> f32>(%[[VALUE_fabsf]], div<f32, rounding=nearest_even, exceptions=observable, contract=fast>(read<f32>(%[[VALUE_x_8]]), neg<f32>(read<f32>(%[[VALUE_y_4]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_test1]], const<f64>(1.0)), const<f64>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_test1]], const<f64>(2.0)), const<f64>(2.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_test1]], const<f64>(0.0)), const<f64>(0.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_test1]], neg<f64>(const<f64>(1.0))), const<f64>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_test1]], neg<f64>(const<f64>(2.0))), const<f64>(2.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(f32) -> f32>(%[[VALUE_test1f]], const<f32>(1.0)), const<f32>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(f32) -> f32>(%[[VALUE_test1f]], const<f32>(2.0)), const<f32>(2.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(f32) -> f32>(%[[VALUE_test1f]], const<f32>(0.0)), const<f32>(0.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(f32) -> f32>(%[[VALUE_test1f]], neg<f32>(const<f32>(1.0))), const<f32>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(f32) -> f32>(%[[VALUE_test1f]], neg<f32>(const<f32>(2.0))), const<f32>(2.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_test2]], const<f64>(1.0)), const<f64>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_test2]], const<f64>(2.0)), const<f64>(2.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_test2]], const<f64>(0.0)), const<f64>(0.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_test2]], neg<f64>(const<f64>(1.0))), const<f64>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_test2]], neg<f64>(const<f64>(2.0))), const<f64>(2.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(f32) -> f32>(%[[VALUE_test2f]], const<f32>(1.0)), const<f32>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(f32) -> f32>(%[[VALUE_test2f]], const<f32>(2.0)), const<f32>(2.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(f32) -> f32>(%[[VALUE_test2f]], const<f32>(0.0)), const<f32>(0.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(f32) -> f32>(%[[VALUE_test2f]], neg<f32>(const<f32>(1.0))), const<f32>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(f32) -> f32>(%[[VALUE_test2f]], neg<f32>(const<f32>(2.0))), const<f32>(2.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_test3]], const<f64>(1.0), const<f64>(1.0)), const<f64>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_test3]], const<f64>(1.0), neg<f64>(const<f64>(1.0))), const<f64>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_test3]], const<f64>(1.0), const<f64>(2.0)), const<f64>(2.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_test3]], const<f64>(1.0), neg<f64>(const<f64>(2.0))), const<f64>(2.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_test3]], const<f64>(2.0), const<f64>(1.0)), const<f64>(2.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_test3]], const<f64>(2.0), neg<f64>(const<f64>(1.0))), const<f64>(2.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_test3]], const<f64>(2.0), const<f64>(2.0)), const<f64>(4.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_test3]], const<f64>(2.0), neg<f64>(const<f64>(2.0))), const<f64>(4.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_test3]], neg<f64>(const<f64>(2.0)), const<f64>(1.0)), const<f64>(2.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_test3]], neg<f64>(const<f64>(2.0)), neg<f64>(const<f64>(1.0))), const<f64>(2.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_test3]], neg<f64>(const<f64>(2.0)), const<f64>(2.0)), const<f64>(4.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_test3]], neg<f64>(const<f64>(2.0)), neg<f64>(const<f64>(2.0))), const<f64>(4.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_test3f]], const<f32>(1.0), const<f32>(1.0)), const<f32>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_test3f]], const<f32>(1.0), neg<f32>(const<f32>(1.0))), const<f32>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_test3f]], const<f32>(1.0), const<f32>(2.0)), const<f32>(2.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_test3f]], const<f32>(1.0), neg<f32>(const<f32>(2.0))), const<f32>(2.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_test3f]], const<f32>(2.0), const<f32>(1.0)), const<f32>(2.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_test3f]], const<f32>(2.0), neg<f32>(const<f32>(1.0))), const<f32>(2.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_test3f]], const<f32>(2.0), const<f32>(2.0)), const<f32>(4.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_test3f]], const<f32>(2.0), neg<f32>(const<f32>(2.0))), const<f32>(4.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_test3f]], neg<f32>(const<f32>(2.0)), const<f32>(1.0)), const<f32>(2.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_test3f]], neg<f32>(const<f32>(2.0)), neg<f32>(const<f32>(1.0))), const<f32>(2.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_test3f]], neg<f32>(const<f32>(2.0)), const<f32>(2.0)), const<f32>(4.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_test3f]], neg<f32>(const<f32>(2.0)), neg<f32>(const<f32>(2.0))), const<f32>(4.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_test4]], const<f64>(1.0), const<f64>(1.0)), const<f64>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_test4]], const<f64>(1.0), neg<f64>(const<f64>(1.0))), const<f64>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_test4]], neg<f64>(const<f64>(1.0)), const<f64>(1.0)), const<f64>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_test4]], neg<f64>(const<f64>(1.0)), neg<f64>(const<f64>(1.0))), const<f64>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_test4]], const<f64>(6.0), const<f64>(3.0)), const<f64>(2.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_test4]], const<f64>(6.0), neg<f64>(const<f64>(3.0))), const<f64>(2.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_test4]], neg<f64>(const<f64>(6.0)), const<f64>(3.0)), const<f64>(2.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_test4]], neg<f64>(const<f64>(6.0)), neg<f64>(const<f64>(3.0))), const<f64>(2.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_test4f]], const<f32>(1.0), const<f32>(1.0)), const<f32>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_test4f]], const<f32>(1.0), neg<f32>(const<f32>(1.0))), const<f32>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_test4f]], neg<f32>(const<f32>(1.0)), const<f32>(1.0)), const<f32>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_test4f]], neg<f32>(const<f32>(1.0)), neg<f32>(const<f32>(1.0))), const<f32>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_test4f]], const<f32>(6.0), const<f32>(3.0)), const<f32>(2.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_test4f]], const<f32>(6.0), neg<f32>(const<f32>(3.0))), const<f32>(2.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_test4f]], neg<f32>(const<f32>(6.0)), const<f32>(3.0)), const<f32>(2.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_test4f]], neg<f32>(const<f32>(6.0)), neg<f32>(const<f32>(3.0))), const<f32>(2.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
