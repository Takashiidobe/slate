/* PR tree-optimization/126464 */
/* { dg-do run } */
/* { dg-options "-O2" } */
/* { dg-add-options ieee } */
/* { dg-skip-if "not IEEE float" { "pdp11-*-*" } } */

[[gnu::noipa]] double
foo (double x)
{
  float y = (float) x;

  if (y == -__builtin_inff ())
    return x * 0.5;
  return y;
}

[[gnu::noipa]] long double
bar (long double x)
{
  double y = (double) x;

  if (y == __builtin_inf ())
    return x * 0.5L;
  return y;
}

[[gnu::noipa]] double
baz (double x)
{
  float y = (float) x;

  if (y == __builtin_inff ())
    return x * 0.5;
  return y;
}

[[gnu::noipa]] long double
qux (long double x)
{
  double y = (double) x;

  if (y == -__builtin_inf ())
    return x * 0.5L;
  return y;
}

int
main ()
{
#if __DBL_MAX_10_EXP__ >= 301
  if (!__builtin_isinf ((double) 1e300)
      && __builtin_isinf ((float) 1e300)
      && (foo (-1e300) != -5e299
	  || baz (1e300) != 5e299))
    __builtin_abort ();
#endif

#if __LDBL_MAX_10_EXP__ >= 4001
  if (!__builtin_isinf ((long double) 1e4000L)
      && __builtin_isinf ((double) 1e4000L)
      && (bar (1e4000L) != 5e3999L
	  || qux (-1e4000L) != -5e3999L))
    __builtin_abort ();
#endif
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
// DEFAULT-NEXT:     fn %13 @__builtin_inff() -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %0 @foo(%1 x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %2 y: f32 [storage=automatic] = float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=ignore>(read<f64>(%1));
// DEFAULT-NEXT:         if eq<f32, exceptions=ignore>(read<f32>(%2), neg<f32>(call<f32, signature=fn() -> f32>(%13)))
// DEFAULT-NEXT:             return mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%1), const<f64>(0.5));
// DEFAULT-NEXT:         return float_widen<f64, reason=return>(read<f32>(%2));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @__builtin_inf() -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %3 @bar(%4 x: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %5 y: f64 [storage=automatic] = float_narrow<f64, reason=explicit, rounding=nearest_even, exceptions=ignore>(read<f80>(%4));
// DEFAULT-NEXT:         if eq<f64, exceptions=ignore>(read<f64>(%5), call<f64, signature=fn() -> f64>(%14))
// DEFAULT-NEXT:             return mul<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80>(%4), const<f80>(0.5));
// DEFAULT-NEXT:         return float_widen<f80, reason=return>(read<f64>(%5));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @baz(%7 x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %8 y: f32 [storage=automatic] = float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=ignore>(read<f64>(%7));
// DEFAULT-NEXT:         if eq<f32, exceptions=ignore>(read<f32>(%8), call<f32, signature=fn() -> f32>(%13))
// DEFAULT-NEXT:             return mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%7), const<f64>(0.5));
// DEFAULT-NEXT:         return float_widen<f64, reason=return>(read<f32>(%8));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @qux(%10 x: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %11 y: f64 [storage=automatic] = float_narrow<f64, reason=explicit, rounding=nearest_even, exceptions=ignore>(read<f80>(%10));
// DEFAULT-NEXT:         if eq<f64, exceptions=ignore>(read<f64>(%11), neg<f64>(call<f64, signature=fn() -> f64>(%14)))
// DEFAULT-NEXT:             return mul<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80>(%10), const<f80>(0.5));
// DEFAULT-NEXT:         return float_widen<f80, reason=return>(read<f64>(%11));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %12 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %16: bool [synthetic];
// DEFAULT-NEXT:         if logical_and<bool>(not<bool>(float_class<bool, test=infinite>(const<f64>(1e300))), float_class<bool, test=infinite>(float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=ignore>(const<f64>(1e300))))
// DEFAULT-NEXT:             let %17: bool [synthetic];
// DEFAULT-NEXT:             if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%0, neg<f64>(const<f64>(1e300))), neg<f64>(const<f64>(5e299)))
// DEFAULT-NEXT:                 write<bool>(%17, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%17, ne<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%6, const<f64>(1e300)), const<f64>(5e299)));
// DEFAULT-NEXT:             write<bool>(%16, read<bool>(%17));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%16, const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%16)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%15);
// DEFAULT-NEXT:         let %18: bool [synthetic];
// DEFAULT-NEXT:         if logical_and<bool>(not<bool>(float_class<bool, test=infinite>(const<f80>(9.99999999999999999997E+3999))), float_class<bool, test=infinite>(float_narrow<f64, reason=explicit, rounding=nearest_even, exceptions=ignore>(const<f80>(9.99999999999999999997E+3999))))
// DEFAULT-NEXT:             let %19: bool [synthetic];
// DEFAULT-NEXT:             if ne<f80, exceptions=ignore>(call<f80, signature=fn(f80) -> f80>(%3, const<f80>(9.99999999999999999997E+3999)), const<f80>(4.99999999999999999998E+3999))
// DEFAULT-NEXT:                 write<bool>(%19, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%19, ne<f80, exceptions=ignore>(call<f80, signature=fn(f80) -> f80>(%9, neg<f80>(const<f80>(9.99999999999999999997E+3999))), neg<f80>(const<f80>(4.99999999999999999998E+3999))));
// DEFAULT-NEXT:             write<bool>(%18, read<bool>(%19));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%18, const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%18)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%15);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
