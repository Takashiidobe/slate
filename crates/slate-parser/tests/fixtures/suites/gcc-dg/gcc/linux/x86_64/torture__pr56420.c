/* PR middle-end/56420 */
/* { dg-do run { target int128 } } */

extern void abort (void);

__attribute__((noinline, noclone)) __uint128_t
foo (__uint128_t x)
{
  return x * (((__uint128_t) -1) << 63);
}

__attribute__((noinline, noclone)) __uint128_t
bar (__uint128_t x)
{
  return x * (((__uint128_t) 1) << 63);
}

__attribute__((noinline, noclone)) __uint128_t
baz (__uint128_t x)
{
  return x * -(((__uint128_t) 1) << 62);
}

int
main ()
{
  if (foo (1) != (((__uint128_t) -1) << 63)
      || foo (8) != (((__uint128_t) -1) << 66))
    abort ();
  if (bar (1) != (((__uint128_t) 1) << 63)
      || bar (8) != (((__uint128_t) 1) << 66))
    abort ();
  if (baz (1) != -(((__uint128_t) 1) << 62)
      || baz (8) != ((-(((__uint128_t) 1) << 62)) << 3))
    abort ();
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
// DEFAULT-NEXT:     fn %1 @foo(%2 x: u128) -> u128 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return mul<u128, overflow=wrap>(read<u128>(%2), shl<u128, overflow=wrap, amount_out_of_range=ub>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), const<i32>(63)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @bar(%4 x: u128) -> u128 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return mul<u128, overflow=wrap>(read<u128>(%4), shl<u128, overflow=wrap, amount_out_of_range=ub>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(1))), const<i32>(63)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @baz(%6 x: u128) -> u128 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return mul<u128, overflow=wrap>(read<u128>(%6), neg<u128, overflow=wrap>(shl<u128, overflow=wrap, amount_out_of_range=ub>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(1))), const<i32>(62))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %8: bool [synthetic];
// DEFAULT-NEXT:         if ne<u128>(call<u128, signature=fn(u128) -> u128>(%1, reinterpret<u128, reason=arg, fits=unknown>(widen<i128, reason=arg>(const<i32>(1)))), shl<u128, overflow=wrap, amount_out_of_range=ub>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), const<i32>(63)))
// DEFAULT-NEXT:             write<bool>(%8, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%8, ne<u128>(call<u128, signature=fn(u128) -> u128>(%1, reinterpret<u128, reason=arg, fits=unknown>(widen<i128, reason=arg>(const<i32>(8)))), shl<u128, overflow=wrap, amount_out_of_range=ub>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), const<i32>(66))));
// DEFAULT-NEXT:         if read<bool>(%8)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %9: bool [synthetic];
// DEFAULT-NEXT:         if ne<u128>(call<u128, signature=fn(u128) -> u128>(%3, reinterpret<u128, reason=arg, fits=unknown>(widen<i128, reason=arg>(const<i32>(1)))), shl<u128, overflow=wrap, amount_out_of_range=ub>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(1))), const<i32>(63)))
// DEFAULT-NEXT:             write<bool>(%9, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%9, ne<u128>(call<u128, signature=fn(u128) -> u128>(%3, reinterpret<u128, reason=arg, fits=unknown>(widen<i128, reason=arg>(const<i32>(8)))), shl<u128, overflow=wrap, amount_out_of_range=ub>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(1))), const<i32>(66))));
// DEFAULT-NEXT:         if read<bool>(%9)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %10: bool [synthetic];
// DEFAULT-NEXT:         if ne<u128>(call<u128, signature=fn(u128) -> u128>(%5, reinterpret<u128, reason=arg, fits=unknown>(widen<i128, reason=arg>(const<i32>(1)))), neg<u128, overflow=wrap>(shl<u128, overflow=wrap, amount_out_of_range=ub>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(1))), const<i32>(62))))
// DEFAULT-NEXT:             write<bool>(%10, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%10, ne<u128>(call<u128, signature=fn(u128) -> u128>(%5, reinterpret<u128, reason=arg, fits=unknown>(widen<i128, reason=arg>(const<i32>(8)))), shl<u128, overflow=wrap, amount_out_of_range=ub>(neg<u128, overflow=wrap>(shl<u128, overflow=wrap, amount_out_of_range=ub>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(1))), const<i32>(62))), const<i32>(3))));
// DEFAULT-NEXT:         if read<bool>(%10)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
