/* PR c/102989 */
/* { dg-require-effective-target bitint } */
/* { dg-options "-O2 -std=c23 -pedantic-errors" } */

#if __BITINT_MAXWIDTH__ >= 192
__attribute__((noipa)) _BitInt(192)
tests192 (_Decimal128 d)
{
  return d;
}

__attribute__((noipa)) unsigned _BitInt(192)
testu192 (_Decimal128 d)
{
  return d;
}
#endif

#if __BITINT_MAXWIDTH__ >= 575
__attribute__((noipa)) _BitInt(575)
tests575 (_Decimal128 d)
{
  return d;
}

__attribute__((noipa)) unsigned _BitInt(575)
testu575 (_Decimal128 d)
{
  return d;
}
#endif

int
main ()
{
#if __BITINT_MAXWIDTH__ >= 192
  if (tests192 (0.DL) != 0wb
      || tests192 (0.9999999999999999999999999999999999DL) != 0wb
      || tests192 (7.999999999999999999999999999999999DL) != 7wb
      || tests192 (-0.DL) != 0wb
      || tests192 (-0.9999999999999999999999999999999999DL) != 0wb
      || tests192 (-1.DL) != -1wb
      || tests192 (-42.5DL) != -42wb
      || tests192 (-34242319854.45429439857871298745432e+27DL) != -34242319854454294398578712987454320000wb
      || tests192 (-213855087769445.9e+43DL) != -2138550877694459000000000000000000000000000000000000000000wb
      || tests192 (3138550867693340381917894711603833.0e+24DL) != 3138550867693340381917894711603833000000000000000000000000wb
      || tests192 (-3138550867693340381917894711603833.0e+24DL) != -3138550867693340381917894711603833000000000000000000000000wb)
    __builtin_abort ();
  if (tests192 (3138550867693340381917894711603834.0e+24DL) != 3138550867693340381917894711603833208051177722232017256447wb
      || tests192 (9999999999999999999999999999999999e+6111DL) != 3138550867693340381917894711603833208051177722232017256447wb
      || tests192 (-3138550867693340381917894711603834.0e+24DL) != -3138550867693340381917894711603833208051177722232017256447wb - 1wb
      || tests192 (-9999999999999999999999999999999999e+6111DL) != -3138550867693340381917894711603833208051177722232017256447wb - 1wb)
    __builtin_abort ();
  if (testu192 (0.DL) != 0uwb
      || testu192 (0.9999999999999999999999999999999999DL) != 0uwb
      || testu192 (-0.9999999999999999999999999999999999DL) != 0uwb
      || testu192 (-0.DL) != 0uwb
      || testu192 (-0.9999999999999999999999DL) != 0uwb
      || testu192 (-0.5DL) != 0uwb
      || testu192 (42.99999999999999999999999999999999DL) != 42uwb
      || testu192 (42.e+21DL) != 42000000000000000000000uwb
      || testu192 (34242319854.45429439857871298745432e+21DL) != 34242319854454294398578712987454uwb
      || testu192 (6277101735386680763835789423207666.0e+24DL) != 6277101735386680763835789423207666000000000000000000000000uwb)
    __builtin_abort ();
  if (testu192 (-1.DL) != 0uwb
      || testu192 (-42.5e+15DL) != 0uwb
      || testu192 (-9999999999999999999999999999999999e+6111DL) != 0uwb
      || testu192 (6277101735386680763835789423207667.0e+24DL) != 6277101735386680763835789423207666416102355444464034512895uwb
      || testu192 (9999999999999999999999999999999999e+6111DL) != 6277101735386680763835789423207666416102355444464034512895uwb)
    __builtin_abort ();
#endif
#if __BITINT_MAXWIDTH__ >= 575
  if (tests575 (0.DL) != 0wb
      || tests575 (0.999999999999999999999DL) != 0wb
      || tests575 (12.99999999999999999999999999999DL) != 12wb
      || tests575 (-0.0000000000DL) != 0wb
      || tests575 (-0.9999999999999999999999999999999999DL) != 0uwb
      || tests575 (-1.DL) != -1wb
      || tests575 (-89.5DL) != -89wb
      || tests575 (-34242319854.45429986754986758972345e+37DL) != -342423198544542998675498675897234500000000000000wb
      || tests575 (-518326003682761.2e+158DL) != -51832600368276120000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000wb
      || tests575 (6183260036827613351512563025491179.0e+139DL) != 61832600368276133515125630254911790000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000wb
      || tests575 (-6183260036827613351512563025491179.0e+139DL) != -61832600368276133515125630254911790000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000wb)
    __builtin_abort ();
  if (tests575 (618326003682761335151256302549118.0e+140DL) != 61832600368276133515125630254911797508782837275302959978515764023224306276632966792579100265310761247399417856504034834837841258576687802491886538775473291979151693037174783wb
      || tests575 (9999999999999999999999999999999999e+6111DL) != 61832600368276133515125630254911797508782837275302959978515764023224306276632966792579100265310761247399417856504034834837841258576687802491886538775473291979151693037174783wb
      || tests575 (-6183260036827613351512563025491180.0e+139DL) != -61832600368276133515125630254911797508782837275302959978515764023224306276632966792579100265310761247399417856504034834837841258576687802491886538775473291979151693037174783wb - 1wb
      || tests575 (-9999999999999999999999999999999999e+6111DL) != -61832600368276133515125630254911797508782837275302959978515764023224306276632966792579100265310761247399417856504034834837841258576687802491886538775473291979151693037174783wb - 1wb)
    __builtin_abort ();
  if (testu575 (0.DL) != 0uwb
      || testu575 (0.5555555555555555555555555555555555DL) != 0uwb
      || testu575 (-0.7777777777777777777777777777777777DL) != 0uwb
      || testu575 (-0.99DL) != 0uwb
      || testu575 (-0.00000000000DL) != 0uwb
      || testu575 (42.99999999999999999999999999999999DL) != 42uwb
      || testu575 (42.e+21DL) != 42000000000000000000000uwb
      || testu575 (94272319854.45429e+27DL) != 94272319854454290000000000000000000000uwb
      || testu575 (1236652007365522670302512605098235.0e+140DL) != 123665200736552267030251260509823500000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000uwb)
    __builtin_abort ();
  if (testu575 (-1.DL) != 0uwb
      || testu575 (-42.5e+15DL) != 0uwb
      || testu575 (-9999999999999999999999999999999999e+6111DL) != 0uwb
      || testu575 (1236652007365522670302512605098236.0e+140DL) != 123665200736552267030251260509823595017565674550605919957031528046448612553265933585158200530621522494798835713008069669675682517153375604983773077550946583958303386074349567uwb
      || testu575 (9999999999999999999999999999999999e+6111DL) != 123665200736552267030251260509823595017565674550605919957031528046448612553265933585158200530621522494798835713008069669675682517153375604983773077550946583958303386074349567uwb)
    __builtin_abort ();
#endif
}

// SLATE-FILECHECK-STD DEFAULT c23
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
// DEFAULT-NEXT:     fn %0 @tests192(%1 d: d128) -> i192b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return float_to_int<i192b, reason=return, out_of_range=ub, exceptions=observable>(read<d128>(%1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %2 @testu192(%3 d: d128) -> u192b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return float_to_int<u192b, reason=return, out_of_range=ub, exceptions=observable>(read<d128>(%3));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @tests575(%5 d: d128) -> i575b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return float_to_int<i575b, reason=return, out_of_range=ub, exceptions=observable>(read<d128>(%5));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @testu575(%7 d: d128) -> u575b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return float_to_int<u575b, reason=return, out_of_range=ub, exceptions=observable>(read<d128>(%7));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %8 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %10: bool [synthetic];
// DEFAULT-NEXT:         if ne<i192b>(call<i192b, signature=fn(d128) -> i192b>(%0, const<d128>(0.)), widen<i192b, reason=usual_arith>(const<i2b>(0)))
// DEFAULT-NEXT:             write<bool>(%10, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%10, ne<i192b>(call<i192b, signature=fn(d128) -> i192b>(%0, const<d128>(0.9999999999999999999999999999999999)), widen<i192b, reason=usual_arith>(const<i2b>(0))));
// DEFAULT-NEXT:         let %11: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%10)
// DEFAULT-NEXT:             write<bool>(%11, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%11, ne<i192b>(call<i192b, signature=fn(d128) -> i192b>(%0, const<d128>(7.999999999999999999999999999999999)), widen<i192b, reason=usual_arith>(const<i4b>(7))));
// DEFAULT-NEXT:         let %12: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%11)
// DEFAULT-NEXT:             write<bool>(%12, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%12, ne<i192b>(call<i192b, signature=fn(d128) -> i192b>(%0, neg<d128>(const<d128>(0.))), widen<i192b, reason=usual_arith>(const<i2b>(0))));
// DEFAULT-NEXT:         let %13: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%12)
// DEFAULT-NEXT:             write<bool>(%13, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%13, ne<i192b>(call<i192b, signature=fn(d128) -> i192b>(%0, neg<d128>(const<d128>(0.9999999999999999999999999999999999))), widen<i192b, reason=usual_arith>(const<i2b>(0))));
// DEFAULT-NEXT:         let %14: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%13)
// DEFAULT-NEXT:             write<bool>(%14, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%14, ne<i192b>(call<i192b, signature=fn(d128) -> i192b>(%0, neg<d128>(const<d128>(1.))), widen<i192b, reason=usual_arith>(neg<i2b, overflow=ub>(const<i2b>(1)))));
// DEFAULT-NEXT:         let %15: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%14)
// DEFAULT-NEXT:             write<bool>(%15, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%15, ne<i192b>(call<i192b, signature=fn(d128) -> i192b>(%0, neg<d128>(const<d128>(42.5))), widen<i192b, reason=usual_arith>(neg<i7b, overflow=ub>(const<i7b>(42)))));
// DEFAULT-NEXT:         let %16: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%15)
// DEFAULT-NEXT:             write<bool>(%16, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%16, ne<i192b>(call<i192b, signature=fn(d128) -> i192b>(%0, neg<d128>(const<d128>(34242319854.45429439857871298745432e+27))), widen<i192b, reason=usual_arith>(neg<i126b, overflow=ub>(const<i126b>(34242319854454294398578712987454320000)))));
// DEFAULT-NEXT:         let %17: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%16)
// DEFAULT-NEXT:             write<bool>(%17, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%17, ne<i192b>(call<i192b, signature=fn(d128) -> i192b>(%0, neg<d128>(const<d128>(213855087769445.9e+43))), neg<i192b, overflow=ub>(const<i192b>(2138550877694459000000000000000000000000000000000000000000))));
// DEFAULT-NEXT:         let %18: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%17)
// DEFAULT-NEXT:             write<bool>(%18, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%18, ne<i192b>(call<i192b, signature=fn(d128) -> i192b>(%0, const<d128>(3138550867693340381917894711603833.0e+24)), const<i192b>(3138550867693340381917894711603833000000000000000000000000)));
// DEFAULT-NEXT:         let %19: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%18)
// DEFAULT-NEXT:             write<bool>(%19, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%19, ne<i192b>(call<i192b, signature=fn(d128) -> i192b>(%0, neg<d128>(const<d128>(3138550867693340381917894711603833.0e+24))), neg<i192b, overflow=ub>(const<i192b>(3138550867693340381917894711603833000000000000000000000000))));
// DEFAULT-NEXT:         if read<bool>(%19)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:         let %20: bool [synthetic];
// DEFAULT-NEXT:         if ne<i192b>(call<i192b, signature=fn(d128) -> i192b>(%0, const<d128>(3138550867693340381917894711603834.0e+24)), const<i192b>(3138550867693340381917894711603833208051177722232017256447))
// DEFAULT-NEXT:             write<bool>(%20, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%20, ne<i192b>(call<i192b, signature=fn(d128) -> i192b>(%0, const<d128>(9999999999999999999999999999999999e+6111)), const<i192b>(3138550867693340381917894711603833208051177722232017256447)));
// DEFAULT-NEXT:         let %21: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%20)
// DEFAULT-NEXT:             write<bool>(%21, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%21, ne<i192b>(call<i192b, signature=fn(d128) -> i192b>(%0, neg<d128>(const<d128>(3138550867693340381917894711603834.0e+24))), sub<i192b, overflow=ub>(neg<i192b, overflow=ub>(const<i192b>(3138550867693340381917894711603833208051177722232017256447)), widen<i192b, reason=usual_arith>(const<i2b>(1)))));
// DEFAULT-NEXT:         let %22: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%21)
// DEFAULT-NEXT:             write<bool>(%22, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%22, ne<i192b>(call<i192b, signature=fn(d128) -> i192b>(%0, neg<d128>(const<d128>(9999999999999999999999999999999999e+6111))), sub<i192b, overflow=ub>(neg<i192b, overflow=ub>(const<i192b>(3138550867693340381917894711603833208051177722232017256447)), widen<i192b, reason=usual_arith>(const<i2b>(1)))));
// DEFAULT-NEXT:         if read<bool>(%22)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:         let %23: bool [synthetic];
// DEFAULT-NEXT:         if ne<u192b>(call<u192b, signature=fn(d128) -> u192b>(%2, const<d128>(0.)), widen<u192b, reason=usual_arith>(const<u1b>(0)))
// DEFAULT-NEXT:             write<bool>(%23, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%23, ne<u192b>(call<u192b, signature=fn(d128) -> u192b>(%2, const<d128>(0.9999999999999999999999999999999999)), widen<u192b, reason=usual_arith>(const<u1b>(0))));
// DEFAULT-NEXT:         let %24: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%23)
// DEFAULT-NEXT:             write<bool>(%24, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%24, ne<u192b>(call<u192b, signature=fn(d128) -> u192b>(%2, neg<d128>(const<d128>(0.9999999999999999999999999999999999))), widen<u192b, reason=usual_arith>(const<u1b>(0))));
// DEFAULT-NEXT:         let %25: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%24)
// DEFAULT-NEXT:             write<bool>(%25, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%25, ne<u192b>(call<u192b, signature=fn(d128) -> u192b>(%2, neg<d128>(const<d128>(0.))), widen<u192b, reason=usual_arith>(const<u1b>(0))));
// DEFAULT-NEXT:         let %26: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%25)
// DEFAULT-NEXT:             write<bool>(%26, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%26, ne<u192b>(call<u192b, signature=fn(d128) -> u192b>(%2, neg<d128>(const<d128>(0.9999999999999999999999))), widen<u192b, reason=usual_arith>(const<u1b>(0))));
// DEFAULT-NEXT:         let %27: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%26)
// DEFAULT-NEXT:             write<bool>(%27, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%27, ne<u192b>(call<u192b, signature=fn(d128) -> u192b>(%2, neg<d128>(const<d128>(0.5))), widen<u192b, reason=usual_arith>(const<u1b>(0))));
// DEFAULT-NEXT:         let %28: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%27)
// DEFAULT-NEXT:             write<bool>(%28, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%28, ne<u192b>(call<u192b, signature=fn(d128) -> u192b>(%2, const<d128>(42.99999999999999999999999999999999)), widen<u192b, reason=usual_arith>(const<u6b>(42))));
// DEFAULT-NEXT:         let %29: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%28)
// DEFAULT-NEXT:             write<bool>(%29, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%29, ne<u192b>(call<u192b, signature=fn(d128) -> u192b>(%2, const<d128>(42.e+21)), widen<u192b, reason=usual_arith>(const<u76b>(42000000000000000000000))));
// DEFAULT-NEXT:         let %30: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%29)
// DEFAULT-NEXT:             write<bool>(%30, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%30, ne<u192b>(call<u192b, signature=fn(d128) -> u192b>(%2, const<d128>(34242319854.45429439857871298745432e+21)), widen<u192b, reason=usual_arith>(const<u105b>(34242319854454294398578712987454))));
// DEFAULT-NEXT:         let %31: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%30)
// DEFAULT-NEXT:             write<bool>(%31, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%31, ne<u192b>(call<u192b, signature=fn(d128) -> u192b>(%2, const<d128>(6277101735386680763835789423207666.0e+24)), const<u192b>(6277101735386680763835789423207666000000000000000000000000)));
// DEFAULT-NEXT:         if read<bool>(%31)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:         let %32: bool [synthetic];
// DEFAULT-NEXT:         if ne<u192b>(call<u192b, signature=fn(d128) -> u192b>(%2, neg<d128>(const<d128>(1.))), widen<u192b, reason=usual_arith>(const<u1b>(0)))
// DEFAULT-NEXT:             write<bool>(%32, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%32, ne<u192b>(call<u192b, signature=fn(d128) -> u192b>(%2, neg<d128>(const<d128>(42.5e+15))), widen<u192b, reason=usual_arith>(const<u1b>(0))));
// DEFAULT-NEXT:         let %33: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%32)
// DEFAULT-NEXT:             write<bool>(%33, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%33, ne<u192b>(call<u192b, signature=fn(d128) -> u192b>(%2, neg<d128>(const<d128>(9999999999999999999999999999999999e+6111))), widen<u192b, reason=usual_arith>(const<u1b>(0))));
// DEFAULT-NEXT:         let %34: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%33)
// DEFAULT-NEXT:             write<bool>(%34, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%34, ne<u192b>(call<u192b, signature=fn(d128) -> u192b>(%2, const<d128>(6277101735386680763835789423207667.0e+24)), const<u192b>(6277101735386680763835789423207666416102355444464034512895)));
// DEFAULT-NEXT:         let %35: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%34)
// DEFAULT-NEXT:             write<bool>(%35, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%35, ne<u192b>(call<u192b, signature=fn(d128) -> u192b>(%2, const<d128>(9999999999999999999999999999999999e+6111)), const<u192b>(6277101735386680763835789423207666416102355444464034512895)));
// DEFAULT-NEXT:         if read<bool>(%35)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:         let %36: bool [synthetic];
// DEFAULT-NEXT:         if ne<i575b>(call<i575b, signature=fn(d128) -> i575b>(%4, const<d128>(0.)), widen<i575b, reason=usual_arith>(const<i2b>(0)))
// DEFAULT-NEXT:             write<bool>(%36, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%36, ne<i575b>(call<i575b, signature=fn(d128) -> i575b>(%4, const<d128>(0.999999999999999999999)), widen<i575b, reason=usual_arith>(const<i2b>(0))));
// DEFAULT-NEXT:         let %37: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%36)
// DEFAULT-NEXT:             write<bool>(%37, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%37, ne<i575b>(call<i575b, signature=fn(d128) -> i575b>(%4, const<d128>(12.99999999999999999999999999999)), widen<i575b, reason=usual_arith>(const<i5b>(12))));
// DEFAULT-NEXT:         let %38: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%37)
// DEFAULT-NEXT:             write<bool>(%38, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%38, ne<i575b>(call<i575b, signature=fn(d128) -> i575b>(%4, neg<d128>(const<d128>(0.0000000000))), widen<i575b, reason=usual_arith>(const<i2b>(0))));
// DEFAULT-NEXT:         let %39: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%38)
// DEFAULT-NEXT:             write<bool>(%39, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%39, ne<i575b>(call<i575b, signature=fn(d128) -> i575b>(%4, neg<d128>(const<d128>(0.9999999999999999999999999999999999))), reinterpret<i575b, reason=usual_arith, fits=unknown>(widen<u575b, reason=usual_arith>(const<u1b>(0)))));
// DEFAULT-NEXT:         let %40: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%39)
// DEFAULT-NEXT:             write<bool>(%40, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%40, ne<i575b>(call<i575b, signature=fn(d128) -> i575b>(%4, neg<d128>(const<d128>(1.))), widen<i575b, reason=usual_arith>(neg<i2b, overflow=ub>(const<i2b>(1)))));
// DEFAULT-NEXT:         let %41: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%40)
// DEFAULT-NEXT:             write<bool>(%41, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%41, ne<i575b>(call<i575b, signature=fn(d128) -> i575b>(%4, neg<d128>(const<d128>(89.5))), widen<i575b, reason=usual_arith>(neg<i8b, overflow=ub>(const<i8b>(89)))));
// DEFAULT-NEXT:         let %42: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%41)
// DEFAULT-NEXT:             write<bool>(%42, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%42, ne<i575b>(call<i575b, signature=fn(d128) -> i575b>(%4, neg<d128>(const<d128>(34242319854.45429986754986758972345e+37))), widen<i575b, reason=usual_arith>(neg<i159b, overflow=ub>(const<i159b>(342423198544542998675498675897234500000000000000)))));
// DEFAULT-NEXT:         let %43: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%42)
// DEFAULT-NEXT:             write<bool>(%43, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%43, ne<i575b>(call<i575b, signature=fn(d128) -> i575b>(%4, neg<d128>(const<d128>(518326003682761.2e+158))), neg<i575b, overflow=ub>(const<i575b>(51832600368276120000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000))));
// DEFAULT-NEXT:         let %44: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%43)
// DEFAULT-NEXT:             write<bool>(%44, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%44, ne<i575b>(call<i575b, signature=fn(d128) -> i575b>(%4, const<d128>(6183260036827613351512563025491179.0e+139)), const<i575b>(61832600368276133515125630254911790000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000)));
// DEFAULT-NEXT:         let %45: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%44)
// DEFAULT-NEXT:             write<bool>(%45, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%45, ne<i575b>(call<i575b, signature=fn(d128) -> i575b>(%4, neg<d128>(const<d128>(6183260036827613351512563025491179.0e+139))), neg<i575b, overflow=ub>(const<i575b>(61832600368276133515125630254911790000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000))));
// DEFAULT-NEXT:         if read<bool>(%45)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:         let %46: bool [synthetic];
// DEFAULT-NEXT:         if ne<i575b>(call<i575b, signature=fn(d128) -> i575b>(%4, const<d128>(618326003682761335151256302549118.0e+140)), const<i575b>(61832600368276133515125630254911797508782837275302959978515764023224306276632966792579100265310761247399417856504034834837841258576687802491886538775473291979151693037174783))
// DEFAULT-NEXT:             write<bool>(%46, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%46, ne<i575b>(call<i575b, signature=fn(d128) -> i575b>(%4, const<d128>(9999999999999999999999999999999999e+6111)), const<i575b>(61832600368276133515125630254911797508782837275302959978515764023224306276632966792579100265310761247399417856504034834837841258576687802491886538775473291979151693037174783)));
// DEFAULT-NEXT:         let %47: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%46)
// DEFAULT-NEXT:             write<bool>(%47, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%47, ne<i575b>(call<i575b, signature=fn(d128) -> i575b>(%4, neg<d128>(const<d128>(6183260036827613351512563025491180.0e+139))), sub<i575b, overflow=ub>(neg<i575b, overflow=ub>(const<i575b>(61832600368276133515125630254911797508782837275302959978515764023224306276632966792579100265310761247399417856504034834837841258576687802491886538775473291979151693037174783)), widen<i575b, reason=usual_arith>(const<i2b>(1)))));
// DEFAULT-NEXT:         let %48: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%47)
// DEFAULT-NEXT:             write<bool>(%48, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%48, ne<i575b>(call<i575b, signature=fn(d128) -> i575b>(%4, neg<d128>(const<d128>(9999999999999999999999999999999999e+6111))), sub<i575b, overflow=ub>(neg<i575b, overflow=ub>(const<i575b>(61832600368276133515125630254911797508782837275302959978515764023224306276632966792579100265310761247399417856504034834837841258576687802491886538775473291979151693037174783)), widen<i575b, reason=usual_arith>(const<i2b>(1)))));
// DEFAULT-NEXT:         if read<bool>(%48)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:         let %49: bool [synthetic];
// DEFAULT-NEXT:         if ne<u575b>(call<u575b, signature=fn(d128) -> u575b>(%6, const<d128>(0.)), widen<u575b, reason=usual_arith>(const<u1b>(0)))
// DEFAULT-NEXT:             write<bool>(%49, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%49, ne<u575b>(call<u575b, signature=fn(d128) -> u575b>(%6, const<d128>(0.5555555555555555555555555555555555)), widen<u575b, reason=usual_arith>(const<u1b>(0))));
// DEFAULT-NEXT:         let %50: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%49)
// DEFAULT-NEXT:             write<bool>(%50, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%50, ne<u575b>(call<u575b, signature=fn(d128) -> u575b>(%6, neg<d128>(const<d128>(0.7777777777777777777777777777777777))), widen<u575b, reason=usual_arith>(const<u1b>(0))));
// DEFAULT-NEXT:         let %51: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%50)
// DEFAULT-NEXT:             write<bool>(%51, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%51, ne<u575b>(call<u575b, signature=fn(d128) -> u575b>(%6, neg<d128>(const<d128>(0.99))), widen<u575b, reason=usual_arith>(const<u1b>(0))));
// DEFAULT-NEXT:         let %52: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%51)
// DEFAULT-NEXT:             write<bool>(%52, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%52, ne<u575b>(call<u575b, signature=fn(d128) -> u575b>(%6, neg<d128>(const<d128>(0.00000000000))), widen<u575b, reason=usual_arith>(const<u1b>(0))));
// DEFAULT-NEXT:         let %53: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%52)
// DEFAULT-NEXT:             write<bool>(%53, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%53, ne<u575b>(call<u575b, signature=fn(d128) -> u575b>(%6, const<d128>(42.99999999999999999999999999999999)), widen<u575b, reason=usual_arith>(const<u6b>(42))));
// DEFAULT-NEXT:         let %54: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%53)
// DEFAULT-NEXT:             write<bool>(%54, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%54, ne<u575b>(call<u575b, signature=fn(d128) -> u575b>(%6, const<d128>(42.e+21)), widen<u575b, reason=usual_arith>(const<u76b>(42000000000000000000000))));
// DEFAULT-NEXT:         let %55: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%54)
// DEFAULT-NEXT:             write<bool>(%55, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%55, ne<u575b>(call<u575b, signature=fn(d128) -> u575b>(%6, const<d128>(94272319854.45429e+27)), widen<u575b, reason=usual_arith>(const<u127b>(94272319854454290000000000000000000000))));
// DEFAULT-NEXT:         let %56: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%55)
// DEFAULT-NEXT:             write<bool>(%56, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%56, ne<u575b>(call<u575b, signature=fn(d128) -> u575b>(%6, const<d128>(1236652007365522670302512605098235.0e+140)), const<u575b>(123665200736552267030251260509823500000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000)));
// DEFAULT-NEXT:         if read<bool>(%56)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:         let %57: bool [synthetic];
// DEFAULT-NEXT:         if ne<u575b>(call<u575b, signature=fn(d128) -> u575b>(%6, neg<d128>(const<d128>(1.))), widen<u575b, reason=usual_arith>(const<u1b>(0)))
// DEFAULT-NEXT:             write<bool>(%57, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%57, ne<u575b>(call<u575b, signature=fn(d128) -> u575b>(%6, neg<d128>(const<d128>(42.5e+15))), widen<u575b, reason=usual_arith>(const<u1b>(0))));
// DEFAULT-NEXT:         let %58: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%57)
// DEFAULT-NEXT:             write<bool>(%58, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%58, ne<u575b>(call<u575b, signature=fn(d128) -> u575b>(%6, neg<d128>(const<d128>(9999999999999999999999999999999999e+6111))), widen<u575b, reason=usual_arith>(const<u1b>(0))));
// DEFAULT-NEXT:         let %59: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%58)
// DEFAULT-NEXT:             write<bool>(%59, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%59, ne<u575b>(call<u575b, signature=fn(d128) -> u575b>(%6, const<d128>(1236652007365522670302512605098236.0e+140)), const<u575b>(123665200736552267030251260509823595017565674550605919957031528046448612553265933585158200530621522494798835713008069669675682517153375604983773077550946583958303386074349567)));
// DEFAULT-NEXT:         let %60: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%59)
// DEFAULT-NEXT:             write<bool>(%60, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%60, ne<u575b>(call<u575b, signature=fn(d128) -> u575b>(%6, const<d128>(9999999999999999999999999999999999e+6111)), const<u575b>(123665200736552267030251260509823595017565674550605919957031528046448612553265933585158200530621522494798835713008069669675682517153375604983773077550946583958303386074349567)));
// DEFAULT-NEXT:         if read<bool>(%60)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
