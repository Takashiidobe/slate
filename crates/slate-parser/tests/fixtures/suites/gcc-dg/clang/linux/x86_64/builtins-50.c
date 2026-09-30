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
// DEFAULT-NEXT:     fn %[[VALUE_copysign:[0-9]+]] @copysign(%[[VALUE0:[0-9]+]] <unnamed>: f64, %[[VALUE1:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_copysignf:[0-9]+]] @copysignf(%[[VALUE2:[0-9]+]] <unnamed>: f32, %[[VALUE3:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_fabs:[0-9]+]] @fabs(%[[VALUE4:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_fabsf:[0-9]+]] @fabsf(%[[VALUE5:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_test1:[0-9]+]] @test1(%[[VALUE_x:[0-9]+]] x: f64, %[[VALUE_y:[0-9]+]] y: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_copysign]], neg<f64>(read<f64>(%[[VALUE_x]])), read<f64>(%[[VALUE_y]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test1f:[0-9]+]] @test1f(%[[VALUE_x_2:[0-9]+]] x: f32, %[[VALUE_y_2:[0-9]+]] y: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_copysignf]], neg<f32>(read<f32>(%[[VALUE_x_2]])), read<f32>(%[[VALUE_y_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2:[0-9]+]] @test2(%[[VALUE_x_3:[0-9]+]] x: f64, %[[VALUE_y_3:[0-9]+]] y: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_copysign]], call<f64, signature=fn(f64) -> f64>(%[[VALUE_fabs]], read<f64>(%[[VALUE_x_3]])), read<f64>(%[[VALUE_y_3]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2f:[0-9]+]] @test2f(%[[VALUE_x_4:[0-9]+]] x: f32, %[[VALUE_y_4:[0-9]+]] y: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_copysignf]], call<f32, signature=fn(f32) -> f32>(%[[VALUE_fabsf]], read<f32>(%[[VALUE_x_4]])), read<f32>(%[[VALUE_y_4]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test3:[0-9]+]] @test3(%[[VALUE_x_5:[0-9]+]] x: f64, %[[VALUE_y_5:[0-9]+]] y: f64, %[[VALUE_z:[0-9]+]] z: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_copysign]], mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%[[VALUE_x_5]]), neg<f64>(read<f64>(%[[VALUE_y_5]]))), read<f64>(%[[VALUE_z]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test3f:[0-9]+]] @test3f(%[[VALUE_x_6:[0-9]+]] x: f32, %[[VALUE_y_6:[0-9]+]] y: f32, %[[VALUE_z_2:[0-9]+]] z: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_copysignf]], mul<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%[[VALUE_x_6]]), neg<f32>(read<f32>(%[[VALUE_y_6]]))), read<f32>(%[[VALUE_z_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test4:[0-9]+]] @test4(%[[VALUE_x_7:[0-9]+]] x: f64, %[[VALUE_y_7:[0-9]+]] y: f64, %[[VALUE_z_3:[0-9]+]] z: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_copysign]], div<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%[[VALUE_x_7]]), neg<f64>(read<f64>(%[[VALUE_y_7]]))), read<f64>(%[[VALUE_z_3]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test4f:[0-9]+]] @test4f(%[[VALUE_x_8:[0-9]+]] x: f32, %[[VALUE_y_8:[0-9]+]] y: f32, %[[VALUE_z_4:[0-9]+]] z: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_copysignf]], div<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%[[VALUE_x_8]]), neg<f32>(read<f32>(%[[VALUE_y_8]]))), read<f32>(%[[VALUE_z_4]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_test1]], const<f64>(3.0), const<f64>(2.0)), const<f64>(3.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_test1]], const<f64>(3.0), neg<f64>(const<f64>(2.0))), neg<f64>(const<f64>(3.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_test1]], neg<f64>(const<f64>(3.0)), const<f64>(2.0)), const<f64>(3.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_test1]], neg<f64>(const<f64>(3.0)), neg<f64>(const<f64>(2.0))), neg<f64>(const<f64>(3.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_test1f]], const<f32>(3.0), const<f32>(2.0)), const<f32>(3.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_test1f]], const<f32>(3.0), neg<f32>(const<f32>(2.0))), neg<f32>(const<f32>(3.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_test1f]], neg<f32>(const<f32>(3.0)), const<f32>(2.0)), const<f32>(3.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_test1f]], neg<f32>(const<f32>(3.0)), neg<f32>(const<f32>(2.0))), neg<f32>(const<f32>(3.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_test2]], const<f64>(3.0), const<f64>(2.0)), const<f64>(3.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_test2]], const<f64>(3.0), neg<f64>(const<f64>(2.0))), neg<f64>(const<f64>(3.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_test2]], neg<f64>(const<f64>(3.0)), const<f64>(2.0)), const<f64>(3.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_test2]], neg<f64>(const<f64>(3.0)), neg<f64>(const<f64>(2.0))), neg<f64>(const<f64>(3.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_test2f]], const<f32>(3.0), const<f32>(2.0)), const<f32>(3.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_test2f]], const<f32>(3.0), neg<f32>(const<f32>(2.0))), neg<f32>(const<f32>(3.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_test2f]], neg<f32>(const<f32>(3.0)), const<f32>(2.0)), const<f32>(3.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_test2f]], neg<f32>(const<f32>(3.0)), neg<f32>(const<f32>(2.0))), neg<f32>(const<f32>(3.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64, f64) -> f64>(%[[VALUE_test3]], const<f64>(2.0), const<f64>(3.0), const<f64>(4.0)), const<f64>(6.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64, f64) -> f64>(%[[VALUE_test3]], const<f64>(2.0), const<f64>(3.0), neg<f64>(const<f64>(4.0))), neg<f64>(const<f64>(6.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64, f64) -> f64>(%[[VALUE_test3]], const<f64>(2.0), neg<f64>(const<f64>(3.0)), const<f64>(4.0)), const<f64>(6.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64, f64) -> f64>(%[[VALUE_test3]], const<f64>(2.0), neg<f64>(const<f64>(3.0)), neg<f64>(const<f64>(4.0))), neg<f64>(const<f64>(6.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64, f64) -> f64>(%[[VALUE_test3]], neg<f64>(const<f64>(2.0)), const<f64>(3.0), const<f64>(4.0)), const<f64>(6.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64, f64) -> f64>(%[[VALUE_test3]], neg<f64>(const<f64>(2.0)), const<f64>(3.0), neg<f64>(const<f64>(4.0))), neg<f64>(const<f64>(6.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64, f64) -> f64>(%[[VALUE_test3]], neg<f64>(const<f64>(2.0)), neg<f64>(const<f64>(3.0)), const<f64>(4.0)), const<f64>(6.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64, f64) -> f64>(%[[VALUE_test3]], neg<f64>(const<f64>(2.0)), neg<f64>(const<f64>(3.0)), neg<f64>(const<f64>(4.0))), neg<f64>(const<f64>(6.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32, f32) -> f32>(%[[VALUE_test3f]], const<f32>(2.0), const<f32>(3.0), const<f32>(4.0)), const<f32>(6.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32, f32) -> f32>(%[[VALUE_test3f]], const<f32>(2.0), const<f32>(3.0), neg<f32>(const<f32>(4.0))), neg<f32>(const<f32>(6.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32, f32) -> f32>(%[[VALUE_test3f]], const<f32>(2.0), neg<f32>(const<f32>(3.0)), const<f32>(4.0)), const<f32>(6.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32, f32) -> f32>(%[[VALUE_test3f]], const<f32>(2.0), neg<f32>(const<f32>(3.0)), neg<f32>(const<f32>(4.0))), neg<f32>(const<f32>(6.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32, f32) -> f32>(%[[VALUE_test3f]], neg<f32>(const<f32>(2.0)), const<f32>(3.0), const<f32>(4.0)), const<f32>(6.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32, f32) -> f32>(%[[VALUE_test3f]], neg<f32>(const<f32>(2.0)), const<f32>(3.0), neg<f32>(const<f32>(4.0))), neg<f32>(const<f32>(6.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32, f32) -> f32>(%[[VALUE_test3f]], neg<f32>(const<f32>(2.0)), neg<f32>(const<f32>(3.0)), const<f32>(4.0)), const<f32>(6.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32, f32) -> f32>(%[[VALUE_test3f]], neg<f32>(const<f32>(2.0)), neg<f32>(const<f32>(3.0)), neg<f32>(const<f32>(4.0))), neg<f32>(const<f32>(6.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64, f64) -> f64>(%[[VALUE_test4]], const<f64>(8.0), const<f64>(2.0), const<f64>(3.0)), const<f64>(4.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64, f64) -> f64>(%[[VALUE_test4]], const<f64>(8.0), const<f64>(2.0), neg<f64>(const<f64>(3.0))), neg<f64>(const<f64>(4.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64, f64) -> f64>(%[[VALUE_test4]], const<f64>(8.0), neg<f64>(const<f64>(2.0)), const<f64>(3.0)), const<f64>(4.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64, f64) -> f64>(%[[VALUE_test4]], const<f64>(8.0), neg<f64>(const<f64>(2.0)), neg<f64>(const<f64>(3.0))), neg<f64>(const<f64>(4.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64, f64) -> f64>(%[[VALUE_test4]], neg<f64>(const<f64>(8.0)), const<f64>(2.0), const<f64>(3.0)), const<f64>(4.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64, f64) -> f64>(%[[VALUE_test4]], neg<f64>(const<f64>(8.0)), const<f64>(2.0), neg<f64>(const<f64>(3.0))), neg<f64>(const<f64>(4.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64, f64) -> f64>(%[[VALUE_test4]], neg<f64>(const<f64>(8.0)), neg<f64>(const<f64>(2.0)), const<f64>(3.0)), const<f64>(4.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64, f64) -> f64>(%[[VALUE_test4]], neg<f64>(const<f64>(8.0)), neg<f64>(const<f64>(2.0)), neg<f64>(const<f64>(3.0))), neg<f64>(const<f64>(4.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32, f32) -> f32>(%[[VALUE_test4f]], const<f32>(8.0), const<f32>(2.0), const<f32>(3.0)), const<f32>(4.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32, f32) -> f32>(%[[VALUE_test4f]], const<f32>(8.0), const<f32>(2.0), neg<f32>(const<f32>(3.0))), neg<f32>(const<f32>(4.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32, f32) -> f32>(%[[VALUE_test4f]], const<f32>(8.0), neg<f32>(const<f32>(2.0)), const<f32>(3.0)), const<f32>(4.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32, f32) -> f32>(%[[VALUE_test4f]], const<f32>(8.0), neg<f32>(const<f32>(2.0)), neg<f32>(const<f32>(3.0))), neg<f32>(const<f32>(4.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32, f32) -> f32>(%[[VALUE_test4f]], neg<f32>(const<f32>(8.0)), const<f32>(2.0), const<f32>(3.0)), const<f32>(4.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32, f32) -> f32>(%[[VALUE_test4f]], neg<f32>(const<f32>(8.0)), const<f32>(2.0), neg<f32>(const<f32>(3.0))), neg<f32>(const<f32>(4.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32, f32) -> f32>(%[[VALUE_test4f]], neg<f32>(const<f32>(8.0)), neg<f32>(const<f32>(2.0)), const<f32>(3.0)), const<f32>(4.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32, f32) -> f32>(%[[VALUE_test4f]], neg<f32>(const<f32>(8.0)), neg<f32>(const<f32>(2.0)), neg<f32>(const<f32>(3.0))), neg<f32>(const<f32>(4.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
