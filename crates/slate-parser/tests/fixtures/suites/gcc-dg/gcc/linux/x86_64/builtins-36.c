/* Copyright (C) 2004 Free Software Foundation.

   Check sin, sinf, sinl, cos, cosf and cosl built-in functions
   eventually compile to sincos, sincosf and sincosl.

   Written by Uros Bizjak, 5th April 2004.  */

/* { dg-do compile } */
/* { dg-options "-O2 -ffast-math" } */

extern double sin(double);
extern float sinf(float);
extern long double sinl(long double);

extern double cos(double);
extern float cosf(float);
extern long double cosl(long double);


double test1(double x)
{
	double y1, y2;

	y1 = sin(x);
	y2 = cos(x);

	return y1 - y2;
}

float test1f(float x)
{
	float y1, y2;

	y1 = sinf(x);
	y2 = cosf(x);

	return y1 - y2;
}

long double test1l(long double x)
{
	long double y1, y2;

	y1 = sinl(x);
	y2 = cosl(x);

	return y1 - y2;
}

double test2(double x)
{
	return sin(x);
}

float test2f(float x)
{
	return sinf(x);
}

long double test2l(long double x)
{
	return sinl(x);
}

double test3(double x)
{
	return cos(x);
}

float test3f(float x)
{
	return cosf(x);
}

long double test3l(long double x)
{
	return cosl(x);
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
// DEFAULT-NEXT:     fn %[[VALUE_sin:[0-9]+]] @sin(%[[VALUE0:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_sinf:[0-9]+]] @sinf(%[[VALUE1:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_sinl:[0-9]+]] @sinl(%[[VALUE2:[0-9]+]] <unnamed>: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_cos:[0-9]+]] @cos(%[[VALUE3:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_cosf:[0-9]+]] @cosf(%[[VALUE4:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_cosl:[0-9]+]] @cosl(%[[VALUE5:[0-9]+]] <unnamed>: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test1:[0-9]+]] @test1(%[[VALUE_x:[0-9]+]] x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y1:[0-9]+]] y1: f64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_y2:[0-9]+]] y2: f64 [storage=automatic];
// DEFAULT-NEXT:         write<f64>(%[[VALUE_y1]], call<f64, signature=fn(f64) -> f64>(%[[VALUE_sin]], read<f64>(%[[VALUE_x]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_sin]], read<f64>(%[[VALUE_x]]));
// DEFAULT-NEXT:         write<f64>(%[[VALUE_y2]], call<f64, signature=fn(f64) -> f64>(%[[VALUE_cos]], read<f64>(%[[VALUE_x]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_cos]], read<f64>(%[[VALUE_x]]));
// DEFAULT-NEXT:         return sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE_y1]]), read<f64>(%[[VALUE_y2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test1f:[0-9]+]] @test1f(%[[VALUE_x_2:[0-9]+]] x: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y1_2:[0-9]+]] y1: f32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_y2_2:[0-9]+]] y2: f32 [storage=automatic];
// DEFAULT-NEXT:         write<f32>(%[[VALUE_y1_2]], call<f32, signature=fn(f32) -> f32>(%[[VALUE_sinf]], read<f32>(%[[VALUE_x_2]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%[[VALUE_sinf]], read<f32>(%[[VALUE_x_2]]));
// DEFAULT-NEXT:         write<f32>(%[[VALUE_y2_2]], call<f32, signature=fn(f32) -> f32>(%[[VALUE_cosf]], read<f32>(%[[VALUE_x_2]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%[[VALUE_cosf]], read<f32>(%[[VALUE_x_2]]));
// DEFAULT-NEXT:         return sub<f32, rounding=nearest_even, exceptions=observable, contract=fast>(read<f32>(%[[VALUE_y1_2]]), read<f32>(%[[VALUE_y2_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test1l:[0-9]+]] @test1l(%[[VALUE_x_3:[0-9]+]] x: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y1_3:[0-9]+]] y1: f80 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_y2_3:[0-9]+]] y2: f80 [storage=automatic];
// DEFAULT-NEXT:         write<f80>(%[[VALUE_y1_3]], call<f80, signature=fn(f80) -> f80>(%[[VALUE_sinl]], read<f80>(%[[VALUE_x_3]])));
// DEFAULT-NEXT:         call<f80, signature=fn(f80) -> f80>(%[[VALUE_sinl]], read<f80>(%[[VALUE_x_3]]));
// DEFAULT-NEXT:         write<f80>(%[[VALUE_y2_3]], call<f80, signature=fn(f80) -> f80>(%[[VALUE_cosl]], read<f80>(%[[VALUE_x_3]])));
// DEFAULT-NEXT:         call<f80, signature=fn(f80) -> f80>(%[[VALUE_cosl]], read<f80>(%[[VALUE_x_3]]));
// DEFAULT-NEXT:         return sub<f80, rounding=nearest_even, exceptions=observable, contract=fast>(read<f80>(%[[VALUE_y1_3]]), read<f80>(%[[VALUE_y2_3]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2:[0-9]+]] @test2(%[[VALUE_x_4:[0-9]+]] x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64) -> f64>(%[[VALUE_sin]], read<f64>(%[[VALUE_x_4]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2f:[0-9]+]] @test2f(%[[VALUE_x_5:[0-9]+]] x: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32) -> f32>(%[[VALUE_sinf]], read<f32>(%[[VALUE_x_5]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2l:[0-9]+]] @test2l(%[[VALUE_x_6:[0-9]+]] x: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80) -> f80>(%[[VALUE_sinl]], read<f80>(%[[VALUE_x_6]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test3:[0-9]+]] @test3(%[[VALUE_x_7:[0-9]+]] x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64) -> f64>(%[[VALUE_cos]], read<f64>(%[[VALUE_x_7]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test3f:[0-9]+]] @test3f(%[[VALUE_x_8:[0-9]+]] x: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32) -> f32>(%[[VALUE_cosf]], read<f32>(%[[VALUE_x_8]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test3l:[0-9]+]] @test3l(%[[VALUE_x_9:[0-9]+]] x: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80) -> f80>(%[[VALUE_cosl]], read<f80>(%[[VALUE_x_9]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
