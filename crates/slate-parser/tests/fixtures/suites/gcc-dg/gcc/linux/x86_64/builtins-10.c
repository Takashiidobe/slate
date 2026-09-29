/* Copyright (C) 2003 Free Software Foundation.

   Check that constant folding of built-in math functions doesn't
   break anything and produces the expected results.

   Written by Roger Sayle, 2nd April 2003.  */

/* { dg-do link } */
/* { dg-options "-O2 -ffast-math" } */

extern void link_error(void);

extern double exp(double);
extern double log(double);
extern double sqrt(double);
extern double pow(double,double);
extern double fabs(double);

void test(double x)
{
  if (sqrt(pow(x,4.0)) != x*x)
    link_error ();

  if (pow(sqrt(x),4.0) != x*x)
    link_error ();

  if (pow(pow(x,4.0),0.25) != x)
    /* XFAIL.  PR41098.  */;
}

void test2(double x, double y, double z)
{
  if (sqrt(pow(x,y)) != pow(fabs(x),y*0.5))
    link_error ();

  if (log(pow(x,y)) != y*log(x))
    link_error ();

  if (pow(exp(x),y) != exp(x*y))
    link_error ();

  if (pow(sqrt(x),y) != pow(x,y*0.5))
    link_error ();

  if (pow(pow(fabs(x),y),z) != pow(fabs(x),y*z))
    link_error ();
}

int main()
{
  test (2.0);
  test2 (2.0, 3.0, 4.0);
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
// DEFAULT-NEXT:     fn %[[VALUE_link_error:[0-9]+]] @link_error() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_exp:[0-9]+]] @exp(%[[VALUE0:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_log:[0-9]+]] @log(%[[VALUE1:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_sqrt:[0-9]+]] @sqrt(%[[VALUE2:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_pow:[0-9]+]] @pow(%[[VALUE3:[0-9]+]] <unnamed>: f64, %[[VALUE4:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fabs:[0-9]+]] @fabs(%[[VALUE5:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_test:[0-9]+]] @test(%[[VALUE_x:[0-9]+]] x: f64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_sqrt]], call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_pow]], read<f64>(%[[VALUE_x]]), const<f64>(4.0))), mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE_x]]), read<f64>(%[[VALUE_x]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_pow]], call<f64, signature=fn(f64) -> f64>(%[[VALUE_sqrt]], read<f64>(%[[VALUE_x]])), const<f64>(4.0)), mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE_x]]), read<f64>(%[[VALUE_x]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_pow]], call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_pow]], read<f64>(%[[VALUE_x]]), const<f64>(4.0)), const<f64>(0.25)), read<f64>(%[[VALUE_x]]))
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2:[0-9]+]] @test2(%[[VALUE_x_2:[0-9]+]] x: f64, %[[VALUE_y:[0-9]+]] y: f64, %[[VALUE_z:[0-9]+]] z: f64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_sqrt]], call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_pow]], read<f64>(%[[VALUE_x_2]]), read<f64>(%[[VALUE_y]]))), call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_pow]], call<f64, signature=fn(f64) -> f64>(%[[VALUE_fabs]], read<f64>(%[[VALUE_x_2]])), mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE_y]]), const<f64>(0.5))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_log]], call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_pow]], read<f64>(%[[VALUE_x_2]]), read<f64>(%[[VALUE_y]]))), mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE_y]]), call<f64, signature=fn(f64) -> f64>(%[[VALUE_log]], read<f64>(%[[VALUE_x_2]]))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_pow]], call<f64, signature=fn(f64) -> f64>(%[[VALUE_exp]], read<f64>(%[[VALUE_x_2]])), read<f64>(%[[VALUE_y]])), call<f64, signature=fn(f64) -> f64>(%[[VALUE_exp]], mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE_x_2]]), read<f64>(%[[VALUE_y]]))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_pow]], call<f64, signature=fn(f64) -> f64>(%[[VALUE_sqrt]], read<f64>(%[[VALUE_x_2]])), read<f64>(%[[VALUE_y]])), call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_pow]], read<f64>(%[[VALUE_x_2]]), mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE_y]]), const<f64>(0.5))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_pow]], call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_pow]], call<f64, signature=fn(f64) -> f64>(%[[VALUE_fabs]], read<f64>(%[[VALUE_x_2]])), read<f64>(%[[VALUE_y]])), read<f64>(%[[VALUE_z]])), call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_pow]], call<f64, signature=fn(f64) -> f64>(%[[VALUE_fabs]], read<f64>(%[[VALUE_x_2]])), mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE_y]]), read<f64>(%[[VALUE_z]]))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(f64) -> void>(%[[VALUE_test]], const<f64>(2.0));
// DEFAULT-NEXT:         call<void, signature=fn(f64, f64, f64) -> void>(%[[VALUE_test2]], const<f64>(2.0), const<f64>(3.0), const<f64>(4.0));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
