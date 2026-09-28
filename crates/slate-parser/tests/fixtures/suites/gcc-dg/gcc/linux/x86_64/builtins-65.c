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
// DEFAULT-NEXT:     fn %0 @ilogbf(%14 <unnamed>: f32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %1 @logbf(%15 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %2 @ilogb(%16 <unnamed>: f64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %3 @logb(%17 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %4 @ilogbl(%18 <unnamed>: f80) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %5 @logbl(%19 <unnamed>: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %6 @link_error() -> void [linkage=external];
// DEFAULT-NEXT:     fn %7 @testf(%8 x: f32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=observable>(call<f32, signature=fn(f32) -> f32>(%1, read<f32>(%8))), call<i32, signature=fn(f32) -> i32>(%0, read<f32>(%8)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @test(%10 x: f64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=observable>(call<f64, signature=fn(f64) -> f64>(%3, read<f64>(%10))), call<i32, signature=fn(f64) -> i32>(%2, read<f64>(%10)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @testl(%12 x: f80) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=observable>(call<f80, signature=fn(f80) -> f80>(%5, read<f80>(%12))), call<i32, signature=fn(f80) -> i32>(%4, read<f80>(%12)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(f32) -> void>(%7, const<f32>(2.0));
// DEFAULT-NEXT:         call<void, signature=fn(f64) -> void>(%9, const<f64>(2.0));
// DEFAULT-NEXT:         call<void, signature=fn(f80) -> void>(%11, const<f80>(2));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
