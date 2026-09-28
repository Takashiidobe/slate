// SLATE-FILECHECK-DEFINES DEFAULT

/* PR optimization/10024 */
extern int *allegro_errno;
typedef long fixed;
extern inline int
fixfloor (fixed x)
{
  if (x >= 0)
    return (x >> 16);
  else
    return ~((~x) >> 16);
}
extern inline int
fixtoi (fixed x)
{
  return fixfloor (x) + ((x & 0x8000) >> 15);
}
extern inline fixed
ftofix (double x)
{
  if (x > 32767.0)
    {
      *allegro_errno = 34;
      return 0x7FFFFFFF;
    }
  if (x < -32767.0)
    {
      *allegro_errno = 34;
      return -0x7FFFFFFF;
    }
  return (long) (x * 65536.0 + (x < 0 ? -0.5 : 0.5));
}
extern inline double
fixtof (fixed x)
{
  return (double) x / 65536.0;
}
extern inline fixed
fixdiv (fixed x, fixed y)
{
  if (y == 0)
    {
      *allegro_errno = 34;
      return (x < 0) ? -0x7FFFFFFF : 0x7FFFFFFF;
    }
  else
    return ftofix (fixtof (x) / fixtof (y));
}
extern inline fixed
itofix (int x)
{
  return x << 16;
}

int
foo (int n)
{
  return fixtoi (fixdiv (itofix (512), itofix (n)));
}

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
// DEFAULT-NEXT:     type @type0 fixed = i64;
// DEFAULT-NEXT:     extern %0 allegro_errno: ptr<i32> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %2 @fixfloor(%3 x: i64) -> i32 [linkage=external] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ge<i64>(read<i64>(%3), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             return truncate<i32, reason=return, fits=unknown>(shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%3), const<i32>(16)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             return truncate<i32, reason=return, fits=unknown>(not<i64>(shr<i64, amount_out_of_range=ub, fill=sign_extend>(not<i64>(read<i64>(%3)), const<i32>(16))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @fixtoi(%5 x: i64) -> i32 [linkage=external] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i32, reason=return, fits=unknown>(add<i64, overflow=ub>(widen<i64, reason=usual_arith>(call<i32, signature=fn(i64) -> i32>(%2, read<i64>(%5))), shr<i64, amount_out_of_range=ub, fill=sign_extend>(and<i64>(read<i64>(%5), widen<i64, reason=usual_arith>(const<i32>(32768))), const<i32>(15))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @ftofix(%7 x: f64) -> i64 [linkage=external] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if gt<f64, exceptions=ignore>(read<f64>(%7), const<f64>(32767.0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<i32>(deref(read<ptr<i32>>(%0)), const<i32>(34));
// DEFAULT-NEXT:                 return widen<i64, reason=return>(const<i32>(2147483647));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         if lt<f64, exceptions=ignore>(read<f64>(%7), neg<f64>(const<f64>(32767.0)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<i32>(deref(read<ptr<i32>>(%0)), const<i32>(34));
// DEFAULT-NEXT:                 return widen<i64, reason=return>(neg<i32, overflow=ub>(const<i32>(2147483647)));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=ignore>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%7), const<f64>(65536.0)), conditional<f64>(lt<f64, exceptions=ignore>(read<f64>(%7), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0))), neg<f64>(const<f64>(0.5)), const<f64>(0.5))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @fixtof(%9 x: i64) -> f64 [linkage=external] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return div<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(read<i64>(%9)), const<f64>(65536.0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @fixdiv(%11 x: i64, %12 y: i64) -> i64 [linkage=external] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if eq<i64>(read<i64>(%12), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<i32>(deref(read<ptr<i32>>(%0)), const<i32>(34));
// DEFAULT-NEXT:                 return widen<i64, reason=return>(conditional<i32>(lt<i64>(read<i64>(%11), widen<i64, reason=usual_arith>(const<i32>(0))), neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(2147483647)));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             return call<i64, signature=fn(f64) -> i64>(%6, div<f64, rounding=nearest_even, exceptions=ignore, contract=on>(call<f64, signature=fn(i64) -> f64>(%8, read<i64>(%11)), call<f64, signature=fn(i64) -> f64>(%8, read<i64>(%12))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @itofix(%14 x: i32) -> i64 [linkage=external] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return widen<i64, reason=return>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i32>(%14), const<i32>(16)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @foo(%16 n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i32, signature=fn(i64) -> i32>(%4, call<i64, signature=fn(i64, i64) -> i64>(%10, call<i64, signature=fn(i32) -> i64>(%13, const<i32>(512)), call<i64, signature=fn(i32) -> i64>(%13, read<i32>(%16))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
