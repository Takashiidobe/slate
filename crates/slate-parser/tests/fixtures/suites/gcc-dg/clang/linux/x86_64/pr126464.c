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
// DEFAULT-NEXT:     fn %[[VALUE___builtin_inff:[0-9]+]] @__builtin_inff() -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_x:[0-9]+]] x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y:[0-9]+]] y: f32 [storage=automatic] = float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=ignore>(read<f64>(%[[VALUE_x]]));
// DEFAULT-NEXT:         if eq<f32, exceptions=ignore>(read<f32>(%[[VALUE_y]]), neg<f32>(call<f32, signature=fn() -> f32>(%[[VALUE___builtin_inff]])))
// DEFAULT-NEXT:             return mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%[[VALUE_x]]), const<f64>(0.5));
// DEFAULT-NEXT:         return float_widen<f64, reason=return>(read<f32>(%[[VALUE_y]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_inf:[0-9]+]] @__builtin_inf() -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_x_2:[0-9]+]] x: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y_2:[0-9]+]] y: f64 [storage=automatic] = float_narrow<f64, reason=explicit, rounding=nearest_even, exceptions=ignore>(read<f80>(%[[VALUE_x_2]]));
// DEFAULT-NEXT:         if eq<f64, exceptions=ignore>(read<f64>(%[[VALUE_y_2]]), call<f64, signature=fn() -> f64>(%[[VALUE___builtin_inf]]))
// DEFAULT-NEXT:             return mul<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80>(%[[VALUE_x_2]]), const<f80>(0.5));
// DEFAULT-NEXT:         return float_widen<f80, reason=return>(read<f64>(%[[VALUE_y_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_baz:[0-9]+]] @baz(%[[VALUE_x_3:[0-9]+]] x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y_3:[0-9]+]] y: f32 [storage=automatic] = float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=ignore>(read<f64>(%[[VALUE_x_3]]));
// DEFAULT-NEXT:         if eq<f32, exceptions=ignore>(read<f32>(%[[VALUE_y_3]]), call<f32, signature=fn() -> f32>(%[[VALUE___builtin_inff]]))
// DEFAULT-NEXT:             return mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%[[VALUE_x_3]]), const<f64>(0.5));
// DEFAULT-NEXT:         return float_widen<f64, reason=return>(read<f32>(%[[VALUE_y_3]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_qux:[0-9]+]] @qux(%[[VALUE_x_4:[0-9]+]] x: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y_4:[0-9]+]] y: f64 [storage=automatic] = float_narrow<f64, reason=explicit, rounding=nearest_even, exceptions=ignore>(read<f80>(%[[VALUE_x_4]]));
// DEFAULT-NEXT:         if eq<f64, exceptions=ignore>(read<f64>(%[[VALUE_y_4]]), neg<f64>(call<f64, signature=fn() -> f64>(%[[VALUE___builtin_inf]])))
// DEFAULT-NEXT:             return mul<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80>(%[[VALUE_x_4]]), const<f80>(0.5));
// DEFAULT-NEXT:         return float_widen<f80, reason=return>(read<f64>(%[[VALUE_y_4]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if logical_and<bool>(not<bool>(float_class<bool, test=infinite>(const<f64>(1e300))), float_class<bool, test=infinite>(float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=ignore>(const<f64>(1e300))))
// DEFAULT-NEXT:             let %[[VALUE1:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_foo]], neg<f64>(const<f64>(1e300))), neg<f64>(const<f64>(5e299)))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE1]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE1]], ne<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_baz]], const<f64>(1e300)), const<f64>(5e299)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE0]], read<bool>(%[[VALUE1]]));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE0]], const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE0]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if logical_and<bool>(not<bool>(float_class<bool, test=infinite>(const<f80>(9.99999999999999999997E+3999))), float_class<bool, test=infinite>(float_narrow<f64, reason=explicit, rounding=nearest_even, exceptions=ignore>(const<f80>(9.99999999999999999997E+3999))))
// DEFAULT-NEXT:             let %[[VALUE3:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<f80, exceptions=ignore>(call<f80, signature=fn(f80) -> f80>(%[[VALUE_bar]], const<f80>(9.99999999999999999997E+3999)), const<f80>(4.99999999999999999998E+3999))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE3]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE3]], ne<f80, exceptions=ignore>(call<f80, signature=fn(f80) -> f80>(%[[VALUE_qux]], neg<f80>(const<f80>(9.99999999999999999997E+3999))), neg<f80>(const<f80>(4.99999999999999999998E+3999))));
// DEFAULT-NEXT:             write<bool>(%[[VALUE2]], read<bool>(%[[VALUE3]]));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE2]], const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE2]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
