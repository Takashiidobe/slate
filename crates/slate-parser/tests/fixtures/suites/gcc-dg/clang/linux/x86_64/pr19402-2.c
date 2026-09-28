/* { dg-do run } */
/* { dg-options "-fno-inline -Os" } */
/* { dg-skip-if "long double support is incomplete" { "avr-*-*" } } */

void abort(void);


float powif(float x, int n)
{
  return __builtin_powif(x, n);
}

double powi(double x, int n)
{
  return __builtin_powi(x, n);
}

long double powil(long double x, int n)
{
  return __builtin_powil(x, n);
}


float powcif(float x)
{
  return __builtin_powif(x, 5);
}

double powci(double x)
{
  return __builtin_powi(x, 5);
}

long double powcil(long double x)
{
  return __builtin_powil(x, 5);
}


float powicf(int n)
{
  return __builtin_powif(2.0, n);
}

double powic(int n)
{
  return __builtin_powi(2.0, n);
}

long double powicl(int n)
{
  return __builtin_powil(2.0, n);
}


int main()
{
  if (__builtin_powi(1.0, 5) != 1.0)
    abort();
  if (__builtin_powif(1.0, 5) != 1.0)
    abort();
  if (__builtin_powil(1.0, 5) != 1.0)
    abort();
  if (powci(1.0) != 1.0)
    abort();
  if (powcif(1.0) != 1.0)
    abort();
  if (powcil(1.0) != 1.0)
    abort();
  if (powi(1.0, -5) != 1.0)
    abort();
  if (powif(1.0, -5) != 1.0)
    abort();
  if (powil(1.0, -5) != 1.0)
    abort();
  if (powic(1) != 2.0)
    abort();
  if (powicf(1) != 2.0)
    abort();
  if (powicl(1) != 2.0)
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
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %25 @__builtin_powif(%23 <unnamed>: f32, %24 <unnamed>: i32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %1 @powif(%2 x: f32, %3 n: i32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32, i32) -> f32>(%25, read<f32>(%2), read<i32>(%3));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %28 @__builtin_powi(%26 <unnamed>: f64, %27 <unnamed>: i32) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %4 @powi(%5 x: f64, %6 n: i32) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64, i32) -> f64>(%28, read<f64>(%5), read<i32>(%6));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %31 @__builtin_powil(%29 <unnamed>: f80, %30 <unnamed>: i32) -> f80 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %7 @powil(%8 x: f80, %9 n: i32) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80, i32) -> f80>(%31, read<f80>(%8), read<i32>(%9));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @powcif(%11 x: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32, i32) -> f32>(%25, read<f32>(%11), const<i32>(5));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @powci(%13 x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64, i32) -> f64>(%28, read<f64>(%13), const<i32>(5));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @powcil(%15 x: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80, i32) -> f80>(%31, read<f80>(%15), const<i32>(5));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @powicf(%17 n: i32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32, i32) -> f32>(%25, float_narrow<f32, reason=arg, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0)), read<i32>(%17));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %18 @powic(%19 n: i32) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64, i32) -> f64>(%28, const<f64>(2.0), read<i32>(%19));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %20 @powicl(%21 n: i32) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80, i32) -> f80>(%31, float_widen<f80, reason=arg>(const<f64>(2.0)), read<i32>(%21));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %22 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, i32) -> f64>(%28, const<f64>(1.0), const<i32>(5)), const<f64>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(float_widen<f64, reason=usual_arith>(call<f32, signature=fn(f32, i32) -> f32>(%25, float_narrow<f32, reason=arg, rounding=nearest_even, exceptions=ignore>(const<f64>(1.0)), const<i32>(5))), const<f64>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(call<f80, signature=fn(f80, i32) -> f80>(%31, float_widen<f80, reason=arg>(const<f64>(1.0)), const<i32>(5)), float_widen<f80, reason=usual_arith>(const<f64>(1.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%12, const<f64>(1.0)), const<f64>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(float_widen<f64, reason=usual_arith>(call<f32, signature=fn(f32) -> f32>(%10, float_narrow<f32, reason=arg, rounding=nearest_even, exceptions=ignore>(const<f64>(1.0)))), const<f64>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(call<f80, signature=fn(f80) -> f80>(%14, float_widen<f80, reason=arg>(const<f64>(1.0))), float_widen<f80, reason=usual_arith>(const<f64>(1.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, i32) -> f64>(%4, const<f64>(1.0), neg<i32, overflow=ub>(const<i32>(5))), const<f64>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(float_widen<f64, reason=usual_arith>(call<f32, signature=fn(f32, i32) -> f32>(%1, float_narrow<f32, reason=arg, rounding=nearest_even, exceptions=ignore>(const<f64>(1.0)), neg<i32, overflow=ub>(const<i32>(5)))), const<f64>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(call<f80, signature=fn(f80, i32) -> f80>(%7, float_widen<f80, reason=arg>(const<f64>(1.0)), neg<i32, overflow=ub>(const<i32>(5))), float_widen<f80, reason=usual_arith>(const<f64>(1.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(i32) -> f64>(%18, const<i32>(1)), const<f64>(2.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(float_widen<f64, reason=usual_arith>(call<f32, signature=fn(i32) -> f32>(%16, const<i32>(1))), const<f64>(2.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(call<f80, signature=fn(i32) -> f80>(%20, const<i32>(1)), float_widen<f80, reason=usual_arith>(const<f64>(2.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
