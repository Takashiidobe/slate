// SLATE-FILECHECK-DEFINES DEFAULT

/* PR optimization/9768 */
/* Originator: Randolph Chung <tausq@debian.org> */

inline int fixfloor (long x)
{
  if (x >= 0)
    return (x >> 16);
  else
    return ~((~x) >> 16);
}

inline int fixtoi (long x)
{
  return fixfloor(x) + ((x & 0x8000) >> 15);
}

int foo(long x, long y)
{
  return fixtoi(x*y);
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
// DEFAULT-NEXT:     fn %0 @fixfloor(%1 x: i64) -> i32 [linkage=external] [inline=hint] [definition=inline_only] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ge<i64>(read<i64>(%1), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             return truncate<i32, reason=return, fits=unknown>(shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%1), const<i32>(16)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             return truncate<i32, reason=return, fits=unknown>(not<i64>(shr<i64, amount_out_of_range=ub, fill=sign_extend>(not<i64>(read<i64>(%1)), const<i32>(16))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %2 @fixtoi(%3 x: i64) -> i32 [linkage=external] [inline=hint] [definition=inline_only] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i32, reason=return, fits=unknown>(add<i64, overflow=ub>(widen<i64, reason=usual_arith>(call<i32, signature=fn(i64) -> i32>(%0, read<i64>(%3))), shr<i64, amount_out_of_range=ub, fill=sign_extend>(and<i64>(read<i64>(%3), widen<i64, reason=usual_arith>(const<i32>(32768))), const<i32>(15))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @foo(%5 x: i64, %6 y: i64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i32, signature=fn(i64) -> i32>(%2, mul<i64, overflow=ub>(read<i64>(%5), read<i64>(%6)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
