/* Copyright (C) 2002 Free Software Foundation.
  
   Test floating point negation produces the expected results.
  
   Written by Roger Sayle, 21st May 2002.  */

/* { dg-do run } */
/* { dg-options "-O2 -ffast-math" } */

extern void abort ();


double
dneg (double x)
{
  return -x;
}

double
dmult (double x)
{
  return -1.0 * x;
}

double
ddiv (double x)
{
  return x / -1.0;
}


float
fneg (float x)
{
  return -x;
}

float
fmult (float x)
{
  return -1.0f * x;
}

float
fdiv (float x)
{
  return x / -1.0f;
}


void
ftest(float src, float dst)
{
  if (fneg (src) != dst)
    abort ();

  if (src != fneg (dst))
    abort ();

  if (fmult (src) != dst)
    abort ();

  if (src != fmult (dst))
    abort ();

  if (fdiv (src) != dst)
    abort ();

  if (src != fdiv(dst))
    abort ();
}

void
dtest(double src, double dst)
{
  if (dneg (src) != dst)
    abort ();

  if (src != dneg (dst))
    abort ();

  if (dmult (src) != dst)
    abort ();

  if (src != dmult (dst))
    abort ();

  if (ddiv (src) != dst)
    abort ();

  if (src != ddiv(dst))
    abort ();
}


int
main ()
{
  ftest (1.0f, -1.0f);
  ftest (2.0f, -2.0f);
  ftest (-3.0f, 3.0f);
  ftest (0.0f, -0.0f);
  ftest (-0.0f, 0.0f);

  dtest (1.0, -1.0);
  dtest (2.0, -2.0);
  dtest (-3.0, 3.0);
  dtest (0.0, -0.0);
  dtest (-0.0, 0.0);

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
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @dneg(%2 x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return neg<f64>(read<f64>(%2));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @dmult(%4 x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(neg<f64>(const<f64>(1.0)), read<f64>(%4));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @ddiv(%6 x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return div<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%6), neg<f64>(const<f64>(1.0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @fneg(%8 x: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return neg<f32>(read<f32>(%8));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @fmult(%10 x: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return mul<f32, rounding=nearest_even, exceptions=observable, contract=fast>(neg<f32>(const<f32>(1.0)), read<f32>(%10));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @fdiv(%12 x: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return div<f32, rounding=nearest_even, exceptions=observable, contract=fast>(read<f32>(%12), neg<f32>(const<f32>(1.0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @ftest(%14 src: f32, %15 dst: f32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(f32) -> f32>(%7, read<f32>(%14)), read<f32>(%15))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(read<f32>(%14), call<f32, signature=fn(f32) -> f32>(%7, read<f32>(%15)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(f32) -> f32>(%9, read<f32>(%14)), read<f32>(%15))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(read<f32>(%14), call<f32, signature=fn(f32) -> f32>(%9, read<f32>(%15)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(f32) -> f32>(%11, read<f32>(%14)), read<f32>(%15))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(read<f32>(%14), call<f32, signature=fn(f32) -> f32>(%11, read<f32>(%15)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @dtest(%17 src: f64, %18 dst: f64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64) -> f64>(%1, read<f64>(%17)), read<f64>(%18))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(read<f64>(%17), call<f64, signature=fn(f64) -> f64>(%1, read<f64>(%18)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64) -> f64>(%3, read<f64>(%17)), read<f64>(%18))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(read<f64>(%17), call<f64, signature=fn(f64) -> f64>(%3, read<f64>(%18)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64) -> f64>(%5, read<f64>(%17)), read<f64>(%18))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(read<f64>(%17), call<f64, signature=fn(f64) -> f64>(%5, read<f64>(%18)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %19 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(f32, f32) -> void>(%13, const<f32>(1.0), neg<f32>(const<f32>(1.0)));
// DEFAULT-NEXT:         call<void, signature=fn(f32, f32) -> void>(%13, const<f32>(2.0), neg<f32>(const<f32>(2.0)));
// DEFAULT-NEXT:         call<void, signature=fn(f32, f32) -> void>(%13, neg<f32>(const<f32>(3.0)), const<f32>(3.0));
// DEFAULT-NEXT:         call<void, signature=fn(f32, f32) -> void>(%13, const<f32>(0.0), neg<f32>(const<f32>(0.0)));
// DEFAULT-NEXT:         call<void, signature=fn(f32, f32) -> void>(%13, neg<f32>(const<f32>(0.0)), const<f32>(0.0));
// DEFAULT-NEXT:         call<void, signature=fn(f64, f64) -> void>(%16, const<f64>(1.0), neg<f64>(const<f64>(1.0)));
// DEFAULT-NEXT:         call<void, signature=fn(f64, f64) -> void>(%16, const<f64>(2.0), neg<f64>(const<f64>(2.0)));
// DEFAULT-NEXT:         call<void, signature=fn(f64, f64) -> void>(%16, neg<f64>(const<f64>(3.0)), const<f64>(3.0));
// DEFAULT-NEXT:         call<void, signature=fn(f64, f64) -> void>(%16, const<f64>(0.0), neg<f64>(const<f64>(0.0)));
// DEFAULT-NEXT:         call<void, signature=fn(f64, f64) -> void>(%16, neg<f64>(const<f64>(0.0)), const<f64>(0.0));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
