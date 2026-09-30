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
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_powif:[0-9]+]] @__builtin_powif(%[[VALUE0:[0-9]+]] <unnamed>: f32, %[[VALUE1:[0-9]+]] <unnamed>: i32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_powif:[0-9]+]] @powif(%[[VALUE_x:[0-9]+]] x: f32, %[[VALUE_n:[0-9]+]] n: i32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32, i32) -> f32>(%[[VALUE___builtin_powif]], read<f32>(%[[VALUE_x]]), read<i32>(%[[VALUE_n]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_powi:[0-9]+]] @__builtin_powi(%[[VALUE2:[0-9]+]] <unnamed>: f64, %[[VALUE3:[0-9]+]] <unnamed>: i32) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_powi:[0-9]+]] @powi(%[[VALUE_x_2:[0-9]+]] x: f64, %[[VALUE_n_2:[0-9]+]] n: i32) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64, i32) -> f64>(%[[VALUE___builtin_powi]], read<f64>(%[[VALUE_x_2]]), read<i32>(%[[VALUE_n_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_powil:[0-9]+]] @__builtin_powil(%[[VALUE4:[0-9]+]] <unnamed>: f80, %[[VALUE5:[0-9]+]] <unnamed>: i32) -> f80 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_powil:[0-9]+]] @powil(%[[VALUE_x_3:[0-9]+]] x: f80, %[[VALUE_n_3:[0-9]+]] n: i32) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80, i32) -> f80>(%[[VALUE___builtin_powil]], read<f80>(%[[VALUE_x_3]]), read<i32>(%[[VALUE_n_3]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_powcif:[0-9]+]] @powcif(%[[VALUE_x_4:[0-9]+]] x: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32, i32) -> f32>(%[[VALUE___builtin_powif]], read<f32>(%[[VALUE_x_4]]), const<i32>(5));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_powci:[0-9]+]] @powci(%[[VALUE_x_5:[0-9]+]] x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64, i32) -> f64>(%[[VALUE___builtin_powi]], read<f64>(%[[VALUE_x_5]]), const<i32>(5));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_powcil:[0-9]+]] @powcil(%[[VALUE_x_6:[0-9]+]] x: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80, i32) -> f80>(%[[VALUE___builtin_powil]], read<f80>(%[[VALUE_x_6]]), const<i32>(5));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_powicf:[0-9]+]] @powicf(%[[VALUE_n_4:[0-9]+]] n: i32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32, i32) -> f32>(%[[VALUE___builtin_powif]], float_narrow<f32, reason=arg, rounding=nearest_even, exceptions=ignore>(const<f64>(2.0)), read<i32>(%[[VALUE_n_4]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_powic:[0-9]+]] @powic(%[[VALUE_n_5:[0-9]+]] n: i32) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64, i32) -> f64>(%[[VALUE___builtin_powi]], const<f64>(2.0), read<i32>(%[[VALUE_n_5]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_powicl:[0-9]+]] @powicl(%[[VALUE_n_6:[0-9]+]] n: i32) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80, i32) -> f80>(%[[VALUE___builtin_powil]], float_widen<f80, reason=arg>(const<f64>(2.0)), read<i32>(%[[VALUE_n_6]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, i32) -> f64>(%[[VALUE___builtin_powi]], const<f64>(1.0), const<i32>(5)), const<f64>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(float_widen<f64, reason=usual_arith>(call<f32, signature=fn(f32, i32) -> f32>(%[[VALUE___builtin_powif]], float_narrow<f32, reason=arg, rounding=nearest_even, exceptions=ignore>(const<f64>(1.0)), const<i32>(5))), const<f64>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(call<f80, signature=fn(f80, i32) -> f80>(%[[VALUE___builtin_powil]], float_widen<f80, reason=arg>(const<f64>(1.0)), const<i32>(5)), float_widen<f80, reason=usual_arith>(const<f64>(1.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_powci]], const<f64>(1.0)), const<f64>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(float_widen<f64, reason=usual_arith>(call<f32, signature=fn(f32) -> f32>(%[[VALUE_powcif]], float_narrow<f32, reason=arg, rounding=nearest_even, exceptions=ignore>(const<f64>(1.0)))), const<f64>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(call<f80, signature=fn(f80) -> f80>(%[[VALUE_powcil]], float_widen<f80, reason=arg>(const<f64>(1.0))), float_widen<f80, reason=usual_arith>(const<f64>(1.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, i32) -> f64>(%[[VALUE_powi]], const<f64>(1.0), neg<i32, overflow=ub>(const<i32>(5))), const<f64>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(float_widen<f64, reason=usual_arith>(call<f32, signature=fn(f32, i32) -> f32>(%[[VALUE_powif]], float_narrow<f32, reason=arg, rounding=nearest_even, exceptions=ignore>(const<f64>(1.0)), neg<i32, overflow=ub>(const<i32>(5)))), const<f64>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(call<f80, signature=fn(f80, i32) -> f80>(%[[VALUE_powil]], float_widen<f80, reason=arg>(const<f64>(1.0)), neg<i32, overflow=ub>(const<i32>(5))), float_widen<f80, reason=usual_arith>(const<f64>(1.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(i32) -> f64>(%[[VALUE_powic]], const<i32>(1)), const<f64>(2.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(float_widen<f64, reason=usual_arith>(call<f32, signature=fn(i32) -> f32>(%[[VALUE_powicf]], const<i32>(1))), const<f64>(2.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(call<f80, signature=fn(i32) -> f80>(%[[VALUE_powicl]], const<i32>(1)), float_widen<f80, reason=usual_arith>(const<f64>(2.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
