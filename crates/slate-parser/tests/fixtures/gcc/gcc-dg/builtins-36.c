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


// SLATE-FILECHECK-FLAVOR gcc
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
// DEFAULT-NEXT:     fn %0 @sin(%30 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %1 @sinf(%31 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %2 @sinl(%32 <unnamed>: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %3 @cos(%33 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %4 @cosf(%34 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %5 @cosl(%35 <unnamed>: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %6 @test1(%7 x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %8 y1: f64 [storage=automatic];
// DEFAULT-NEXT:         let %9 y2: f64 [storage=automatic];
// DEFAULT-NEXT:         write<f64>(%8, call<f64, signature=fn(f64) -> f64>(%0, read<f64>(%7)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%0, read<f64>(%7));
// DEFAULT-NEXT:         write<f64>(%9, call<f64, signature=fn(f64) -> f64>(%3, read<f64>(%7)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%3, read<f64>(%7));
// DEFAULT-NEXT:         return sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%8), read<f64>(%9));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @test1f(%11 x: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %12 y1: f32 [storage=automatic];
// DEFAULT-NEXT:         let %13 y2: f32 [storage=automatic];
// DEFAULT-NEXT:         write<f32>(%12, call<f32, signature=fn(f32) -> f32>(%1, read<f32>(%11)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%1, read<f32>(%11));
// DEFAULT-NEXT:         write<f32>(%13, call<f32, signature=fn(f32) -> f32>(%4, read<f32>(%11)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%4, read<f32>(%11));
// DEFAULT-NEXT:         return sub<f32, rounding=nearest_even, exceptions=observable, contract=fast>(read<f32>(%12), read<f32>(%13));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @test1l(%15 x: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %16 y1: f80 [storage=automatic];
// DEFAULT-NEXT:         let %17 y2: f80 [storage=automatic];
// DEFAULT-NEXT:         write<f80>(%16, call<f80, signature=fn(f80) -> f80>(%2, read<f80>(%15)));
// DEFAULT-NEXT:         call<f80, signature=fn(f80) -> f80>(%2, read<f80>(%15));
// DEFAULT-NEXT:         write<f80>(%17, call<f80, signature=fn(f80) -> f80>(%5, read<f80>(%15)));
// DEFAULT-NEXT:         call<f80, signature=fn(f80) -> f80>(%5, read<f80>(%15));
// DEFAULT-NEXT:         return sub<f80, rounding=nearest_even, exceptions=observable, contract=fast>(read<f80>(%16), read<f80>(%17));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %18 @test2(%19 x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64) -> f64>(%0, read<f64>(%19));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %20 @test2f(%21 x: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32) -> f32>(%1, read<f32>(%21));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %22 @test2l(%23 x: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80) -> f80>(%2, read<f80>(%23));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %24 @test3(%25 x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64) -> f64>(%3, read<f64>(%25));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %26 @test3f(%27 x: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32) -> f32>(%4, read<f32>(%27));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %28 @test3l(%29 x: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80) -> f80>(%5, read<f80>(%29));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
