/* { dg-do link } */
/* { dg-options "-O2 -ffast-math" } */
/* { dg-require-effective-target c99_runtime } */

extern int ilogbf (float);
extern float logbf (float);
extern int ilogb (double);
extern double logb (double);
extern int ilogbl (long double);
extern long double logbl (long double);

extern void link_error(void);

void testf(float x)
{
  if ((int) logbf (x) != ilogbf (x))
    link_error ();
}

void test(double x)
{
  if ((int) logb (x) != ilogb (x))
    link_error ();
}

void testl(long double x)
{
  if ((int) logbl (x) != ilogbl (x))
    link_error ();
}

int main()
{
  testf (2.0f);
  test (2.0);
  testl (2.0l);

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
// DEFAULT-NEXT:     fn %[[VALUE_ilogbf:[0-9]+]] @ilogbf(%[[VALUE0:[0-9]+]] <unnamed>: f32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_logbf:[0-9]+]] @logbf(%[[VALUE1:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_ilogb:[0-9]+]] @ilogb(%[[VALUE2:[0-9]+]] <unnamed>: f64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_logb:[0-9]+]] @logb(%[[VALUE3:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_ilogbl:[0-9]+]] @ilogbl(%[[VALUE4:[0-9]+]] <unnamed>: f80) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_logbl:[0-9]+]] @logbl(%[[VALUE5:[0-9]+]] <unnamed>: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_link_error:[0-9]+]] @link_error() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_testf:[0-9]+]] @testf(%[[VALUE_x:[0-9]+]] x: f32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=observable>(call<f32, signature=fn(f32) -> f32>(%[[VALUE_logbf]], read<f32>(%[[VALUE_x]]))), call<i32, signature=fn(f32) -> i32>(%[[VALUE_ilogbf]], read<f32>(%[[VALUE_x]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test:[0-9]+]] @test(%[[VALUE_x_2:[0-9]+]] x: f64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=observable>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_logb]], read<f64>(%[[VALUE_x_2]]))), call<i32, signature=fn(f64) -> i32>(%[[VALUE_ilogb]], read<f64>(%[[VALUE_x_2]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testl:[0-9]+]] @testl(%[[VALUE_x_3:[0-9]+]] x: f80) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=observable>(call<f80, signature=fn(f80) -> f80>(%[[VALUE_logbl]], read<f80>(%[[VALUE_x_3]]))), call<i32, signature=fn(f80) -> i32>(%[[VALUE_ilogbl]], read<f80>(%[[VALUE_x_3]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(f32) -> void>(%[[VALUE_testf]], const<f32>(2.0));
// DEFAULT-NEXT:         call<void, signature=fn(f64) -> void>(%[[VALUE_test]], const<f64>(2.0));
// DEFAULT-NEXT:         call<void, signature=fn(f80) -> void>(%[[VALUE_testl]], const<f80>(2));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
