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
// DEFAULT-NEXT:     fn %[[VALUE_tests192:[0-9]+]] @tests192(%[[VALUE_d:[0-9]+]] d: d128) -> i192b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return float_to_int<i192b, reason=return, out_of_range=ub, exceptions=observable>(read<d128>(%[[VALUE_d]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testu192:[0-9]+]] @testu192(%[[VALUE_d_2:[0-9]+]] d: d128) -> u192b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return float_to_int<u192b, reason=return, out_of_range=ub, exceptions=observable>(read<d128>(%[[VALUE_d_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_tests575:[0-9]+]] @tests575(%[[VALUE_d_3:[0-9]+]] d: d128) -> i575b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return float_to_int<i575b, reason=return, out_of_range=ub, exceptions=observable>(read<d128>(%[[VALUE_d_3]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testu575:[0-9]+]] @testu575(%[[VALUE_d_4:[0-9]+]] d: d128) -> u575b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return float_to_int<u575b, reason=return, out_of_range=ub, exceptions=observable>(read<d128>(%[[VALUE_d_4]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<i192b>(call<i192b, signature=fn(d128) -> i192b>(%[[VALUE_tests192]], const<d128>(0.)), widen<i192b, reason=usual_arith>(const<i2b>(0)))
// DEFAULT-NEXT:             write<bool>(%[[VALUE0]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE0]], ne<i192b>(call<i192b, signature=fn(d128) -> i192b>(%[[VALUE_tests192]], const<d128>(0.9999999999999999999999999999999999)), widen<i192b, reason=usual_arith>(const<i2b>(0))));
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE0]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE1]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE1]], ne<i192b>(call<i192b, signature=fn(d128) -> i192b>(%[[VALUE_tests192]], const<d128>(7.999999999999999999999999999999999)), widen<i192b, reason=usual_arith>(const<i4b>(7))));
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE1]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE2]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE2]], ne<i192b>(call<i192b, signature=fn(d128) -> i192b>(%[[VALUE_tests192]], neg<d128>(const<d128>(0.))), widen<i192b, reason=usual_arith>(const<i2b>(0))));
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE2]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE3]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE3]], ne<i192b>(call<i192b, signature=fn(d128) -> i192b>(%[[VALUE_tests192]], neg<d128>(const<d128>(0.9999999999999999999999999999999999))), widen<i192b, reason=usual_arith>(const<i2b>(0))));
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE3]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE4]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE4]], ne<i192b>(call<i192b, signature=fn(d128) -> i192b>(%[[VALUE_tests192]], neg<d128>(const<d128>(1.))), widen<i192b, reason=usual_arith>(neg<i2b, overflow=ub>(const<i2b>(1)))));
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE4]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE5]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE5]], ne<i192b>(call<i192b, signature=fn(d128) -> i192b>(%[[VALUE_tests192]], neg<d128>(const<d128>(42.5))), widen<i192b, reason=usual_arith>(neg<i7b, overflow=ub>(const<i7b>(42)))));
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE5]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE6]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE6]], ne<i192b>(call<i192b, signature=fn(d128) -> i192b>(%[[VALUE_tests192]], neg<d128>(const<d128>(34242319854.45429439857871298745432e+27))), widen<i192b, reason=usual_arith>(neg<i126b, overflow=ub>(const<i126b>(34242319854454294398578712987454320000)))));
// DEFAULT-NEXT:         let %[[VALUE7:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE6]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE7]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE7]], ne<i192b>(call<i192b, signature=fn(d128) -> i192b>(%[[VALUE_tests192]], neg<d128>(const<d128>(213855087769445.9e+43))), neg<i192b, overflow=ub>(const<i192b>(2138550877694459000000000000000000000000000000000000000000))));
// DEFAULT-NEXT:         let %[[VALUE8:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE7]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE8]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE8]], ne<i192b>(call<i192b, signature=fn(d128) -> i192b>(%[[VALUE_tests192]], const<d128>(3138550867693340381917894711603833.0e+24)), const<i192b>(3138550867693340381917894711603833000000000000000000000000)));
// DEFAULT-NEXT:         let %[[VALUE9:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE8]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE9]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE9]], ne<i192b>(call<i192b, signature=fn(d128) -> i192b>(%[[VALUE_tests192]], neg<d128>(const<d128>(3138550867693340381917894711603833.0e+24))), neg<i192b, overflow=ub>(const<i192b>(3138550867693340381917894711603833000000000000000000000000))));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE9]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         let %[[VALUE10:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<i192b>(call<i192b, signature=fn(d128) -> i192b>(%[[VALUE_tests192]], const<d128>(3138550867693340381917894711603834.0e+24)), const<i192b>(3138550867693340381917894711603833208051177722232017256447))
// DEFAULT-NEXT:             write<bool>(%[[VALUE10]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE10]], ne<i192b>(call<i192b, signature=fn(d128) -> i192b>(%[[VALUE_tests192]], const<d128>(9999999999999999999999999999999999e+6111)), const<i192b>(3138550867693340381917894711603833208051177722232017256447)));
// DEFAULT-NEXT:         let %[[VALUE11:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE10]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE11]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE11]], ne<i192b>(call<i192b, signature=fn(d128) -> i192b>(%[[VALUE_tests192]], neg<d128>(const<d128>(3138550867693340381917894711603834.0e+24))), sub<i192b, overflow=ub>(neg<i192b, overflow=ub>(const<i192b>(3138550867693340381917894711603833208051177722232017256447)), widen<i192b, reason=usual_arith>(const<i2b>(1)))));
// DEFAULT-NEXT:         let %[[VALUE12:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE11]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE12]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE12]], ne<i192b>(call<i192b, signature=fn(d128) -> i192b>(%[[VALUE_tests192]], neg<d128>(const<d128>(9999999999999999999999999999999999e+6111))), sub<i192b, overflow=ub>(neg<i192b, overflow=ub>(const<i192b>(3138550867693340381917894711603833208051177722232017256447)), widen<i192b, reason=usual_arith>(const<i2b>(1)))));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE12]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         let %[[VALUE13:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<u192b>(call<u192b, signature=fn(d128) -> u192b>(%[[VALUE_testu192]], const<d128>(0.)), widen<u192b, reason=usual_arith>(const<u1b>(0)))
// DEFAULT-NEXT:             write<bool>(%[[VALUE13]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE13]], ne<u192b>(call<u192b, signature=fn(d128) -> u192b>(%[[VALUE_testu192]], const<d128>(0.9999999999999999999999999999999999)), widen<u192b, reason=usual_arith>(const<u1b>(0))));
// DEFAULT-NEXT:         let %[[VALUE14:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE13]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE14]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE14]], ne<u192b>(call<u192b, signature=fn(d128) -> u192b>(%[[VALUE_testu192]], neg<d128>(const<d128>(0.9999999999999999999999999999999999))), widen<u192b, reason=usual_arith>(const<u1b>(0))));
// DEFAULT-NEXT:         let %[[VALUE15:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE14]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE15]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE15]], ne<u192b>(call<u192b, signature=fn(d128) -> u192b>(%[[VALUE_testu192]], neg<d128>(const<d128>(0.))), widen<u192b, reason=usual_arith>(const<u1b>(0))));
// DEFAULT-NEXT:         let %[[VALUE16:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE15]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE16]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE16]], ne<u192b>(call<u192b, signature=fn(d128) -> u192b>(%[[VALUE_testu192]], neg<d128>(const<d128>(0.9999999999999999999999))), widen<u192b, reason=usual_arith>(const<u1b>(0))));
// DEFAULT-NEXT:         let %[[VALUE17:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE16]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE17]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE17]], ne<u192b>(call<u192b, signature=fn(d128) -> u192b>(%[[VALUE_testu192]], neg<d128>(const<d128>(0.5))), widen<u192b, reason=usual_arith>(const<u1b>(0))));
// DEFAULT-NEXT:         let %[[VALUE18:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE17]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE18]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE18]], ne<u192b>(call<u192b, signature=fn(d128) -> u192b>(%[[VALUE_testu192]], const<d128>(42.99999999999999999999999999999999)), widen<u192b, reason=usual_arith>(const<u6b>(42))));
// DEFAULT-NEXT:         let %[[VALUE19:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE18]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE19]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE19]], ne<u192b>(call<u192b, signature=fn(d128) -> u192b>(%[[VALUE_testu192]], const<d128>(42.e+21)), widen<u192b, reason=usual_arith>(const<u76b>(42000000000000000000000))));
// DEFAULT-NEXT:         let %[[VALUE20:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE19]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE20]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE20]], ne<u192b>(call<u192b, signature=fn(d128) -> u192b>(%[[VALUE_testu192]], const<d128>(34242319854.45429439857871298745432e+21)), widen<u192b, reason=usual_arith>(const<u105b>(34242319854454294398578712987454))));
// DEFAULT-NEXT:         let %[[VALUE21:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE20]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE21]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE21]], ne<u192b>(call<u192b, signature=fn(d128) -> u192b>(%[[VALUE_testu192]], const<d128>(6277101735386680763835789423207666.0e+24)), const<u192b>(6277101735386680763835789423207666000000000000000000000000)));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE21]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         let %[[VALUE22:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<u192b>(call<u192b, signature=fn(d128) -> u192b>(%[[VALUE_testu192]], neg<d128>(const<d128>(1.))), widen<u192b, reason=usual_arith>(const<u1b>(0)))
// DEFAULT-NEXT:             write<bool>(%[[VALUE22]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE22]], ne<u192b>(call<u192b, signature=fn(d128) -> u192b>(%[[VALUE_testu192]], neg<d128>(const<d128>(42.5e+15))), widen<u192b, reason=usual_arith>(const<u1b>(0))));
// DEFAULT-NEXT:         let %[[VALUE23:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE22]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE23]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE23]], ne<u192b>(call<u192b, signature=fn(d128) -> u192b>(%[[VALUE_testu192]], neg<d128>(const<d128>(9999999999999999999999999999999999e+6111))), widen<u192b, reason=usual_arith>(const<u1b>(0))));
// DEFAULT-NEXT:         let %[[VALUE24:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE23]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE24]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE24]], ne<u192b>(call<u192b, signature=fn(d128) -> u192b>(%[[VALUE_testu192]], const<d128>(6277101735386680763835789423207667.0e+24)), const<u192b>(6277101735386680763835789423207666416102355444464034512895)));
// DEFAULT-NEXT:         let %[[VALUE25:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE24]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE25]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE25]], ne<u192b>(call<u192b, signature=fn(d128) -> u192b>(%[[VALUE_testu192]], const<d128>(9999999999999999999999999999999999e+6111)), const<u192b>(6277101735386680763835789423207666416102355444464034512895)));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE25]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         let %[[VALUE26:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<i575b>(call<i575b, signature=fn(d128) -> i575b>(%[[VALUE_tests575]], const<d128>(0.)), widen<i575b, reason=usual_arith>(const<i2b>(0)))
// DEFAULT-NEXT:             write<bool>(%[[VALUE26]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE26]], ne<i575b>(call<i575b, signature=fn(d128) -> i575b>(%[[VALUE_tests575]], const<d128>(0.999999999999999999999)), widen<i575b, reason=usual_arith>(const<i2b>(0))));
// DEFAULT-NEXT:         let %[[VALUE27:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE26]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE27]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE27]], ne<i575b>(call<i575b, signature=fn(d128) -> i575b>(%[[VALUE_tests575]], const<d128>(12.99999999999999999999999999999)), widen<i575b, reason=usual_arith>(const<i5b>(12))));
// DEFAULT-NEXT:         let %[[VALUE28:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE27]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE28]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE28]], ne<i575b>(call<i575b, signature=fn(d128) -> i575b>(%[[VALUE_tests575]], neg<d128>(const<d128>(0.0000000000))), widen<i575b, reason=usual_arith>(const<i2b>(0))));
// DEFAULT-NEXT:         let %[[VALUE29:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE28]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE29]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE29]], ne<i575b>(call<i575b, signature=fn(d128) -> i575b>(%[[VALUE_tests575]], neg<d128>(const<d128>(0.9999999999999999999999999999999999))), reinterpret<i575b, reason=usual_arith, fits=unknown>(widen<u575b, reason=usual_arith>(const<u1b>(0)))));
// DEFAULT-NEXT:         let %[[VALUE30:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE29]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE30]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE30]], ne<i575b>(call<i575b, signature=fn(d128) -> i575b>(%[[VALUE_tests575]], neg<d128>(const<d128>(1.))), widen<i575b, reason=usual_arith>(neg<i2b, overflow=ub>(const<i2b>(1)))));
// DEFAULT-NEXT:         let %[[VALUE31:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE30]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE31]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE31]], ne<i575b>(call<i575b, signature=fn(d128) -> i575b>(%[[VALUE_tests575]], neg<d128>(const<d128>(89.5))), widen<i575b, reason=usual_arith>(neg<i8b, overflow=ub>(const<i8b>(89)))));
// DEFAULT-NEXT:         let %[[VALUE32:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE31]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE32]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE32]], ne<i575b>(call<i575b, signature=fn(d128) -> i575b>(%[[VALUE_tests575]], neg<d128>(const<d128>(34242319854.45429986754986758972345e+37))), widen<i575b, reason=usual_arith>(neg<i159b, overflow=ub>(const<i159b>(342423198544542998675498675897234500000000000000)))));
// DEFAULT-NEXT:         let %[[VALUE33:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE32]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE33]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE33]], ne<i575b>(call<i575b, signature=fn(d128) -> i575b>(%[[VALUE_tests575]], neg<d128>(const<d128>(518326003682761.2e+158))), neg<i575b, overflow=ub>(const<i575b>(51832600368276120000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000))));
// DEFAULT-NEXT:         let %[[VALUE34:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE33]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE34]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE34]], ne<i575b>(call<i575b, signature=fn(d128) -> i575b>(%[[VALUE_tests575]], const<d128>(6183260036827613351512563025491179.0e+139)), const<i575b>(61832600368276133515125630254911790000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000)));
// DEFAULT-NEXT:         let %[[VALUE35:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE34]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE35]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE35]], ne<i575b>(call<i575b, signature=fn(d128) -> i575b>(%[[VALUE_tests575]], neg<d128>(const<d128>(6183260036827613351512563025491179.0e+139))), neg<i575b, overflow=ub>(const<i575b>(61832600368276133515125630254911790000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000))));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE35]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         let %[[VALUE36:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<i575b>(call<i575b, signature=fn(d128) -> i575b>(%[[VALUE_tests575]], const<d128>(618326003682761335151256302549118.0e+140)), const<i575b>(61832600368276133515125630254911797508782837275302959978515764023224306276632966792579100265310761247399417856504034834837841258576687802491886538775473291979151693037174783))
// DEFAULT-NEXT:             write<bool>(%[[VALUE36]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE36]], ne<i575b>(call<i575b, signature=fn(d128) -> i575b>(%[[VALUE_tests575]], const<d128>(9999999999999999999999999999999999e+6111)), const<i575b>(61832600368276133515125630254911797508782837275302959978515764023224306276632966792579100265310761247399417856504034834837841258576687802491886538775473291979151693037174783)));
// DEFAULT-NEXT:         let %[[VALUE37:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE36]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE37]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE37]], ne<i575b>(call<i575b, signature=fn(d128) -> i575b>(%[[VALUE_tests575]], neg<d128>(const<d128>(6183260036827613351512563025491180.0e+139))), sub<i575b, overflow=ub>(neg<i575b, overflow=ub>(const<i575b>(61832600368276133515125630254911797508782837275302959978515764023224306276632966792579100265310761247399417856504034834837841258576687802491886538775473291979151693037174783)), widen<i575b, reason=usual_arith>(const<i2b>(1)))));
// DEFAULT-NEXT:         let %[[VALUE38:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE37]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE38]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE38]], ne<i575b>(call<i575b, signature=fn(d128) -> i575b>(%[[VALUE_tests575]], neg<d128>(const<d128>(9999999999999999999999999999999999e+6111))), sub<i575b, overflow=ub>(neg<i575b, overflow=ub>(const<i575b>(61832600368276133515125630254911797508782837275302959978515764023224306276632966792579100265310761247399417856504034834837841258576687802491886538775473291979151693037174783)), widen<i575b, reason=usual_arith>(const<i2b>(1)))));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE38]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         let %[[VALUE39:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<u575b>(call<u575b, signature=fn(d128) -> u575b>(%[[VALUE_testu575]], const<d128>(0.)), widen<u575b, reason=usual_arith>(const<u1b>(0)))
// DEFAULT-NEXT:             write<bool>(%[[VALUE39]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE39]], ne<u575b>(call<u575b, signature=fn(d128) -> u575b>(%[[VALUE_testu575]], const<d128>(0.5555555555555555555555555555555555)), widen<u575b, reason=usual_arith>(const<u1b>(0))));
// DEFAULT-NEXT:         let %[[VALUE40:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE39]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE40]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE40]], ne<u575b>(call<u575b, signature=fn(d128) -> u575b>(%[[VALUE_testu575]], neg<d128>(const<d128>(0.7777777777777777777777777777777777))), widen<u575b, reason=usual_arith>(const<u1b>(0))));
// DEFAULT-NEXT:         let %[[VALUE41:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE40]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE41]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE41]], ne<u575b>(call<u575b, signature=fn(d128) -> u575b>(%[[VALUE_testu575]], neg<d128>(const<d128>(0.99))), widen<u575b, reason=usual_arith>(const<u1b>(0))));
// DEFAULT-NEXT:         let %[[VALUE42:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE41]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE42]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE42]], ne<u575b>(call<u575b, signature=fn(d128) -> u575b>(%[[VALUE_testu575]], neg<d128>(const<d128>(0.00000000000))), widen<u575b, reason=usual_arith>(const<u1b>(0))));
// DEFAULT-NEXT:         let %[[VALUE43:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE42]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE43]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE43]], ne<u575b>(call<u575b, signature=fn(d128) -> u575b>(%[[VALUE_testu575]], const<d128>(42.99999999999999999999999999999999)), widen<u575b, reason=usual_arith>(const<u6b>(42))));
// DEFAULT-NEXT:         let %[[VALUE44:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE43]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE44]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE44]], ne<u575b>(call<u575b, signature=fn(d128) -> u575b>(%[[VALUE_testu575]], const<d128>(42.e+21)), widen<u575b, reason=usual_arith>(const<u76b>(42000000000000000000000))));
// DEFAULT-NEXT:         let %[[VALUE45:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE44]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE45]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE45]], ne<u575b>(call<u575b, signature=fn(d128) -> u575b>(%[[VALUE_testu575]], const<d128>(94272319854.45429e+27)), widen<u575b, reason=usual_arith>(const<u127b>(94272319854454290000000000000000000000))));
// DEFAULT-NEXT:         let %[[VALUE46:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE45]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE46]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE46]], ne<u575b>(call<u575b, signature=fn(d128) -> u575b>(%[[VALUE_testu575]], const<d128>(1236652007365522670302512605098235.0e+140)), const<u575b>(123665200736552267030251260509823500000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000)));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE46]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         let %[[VALUE47:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<u575b>(call<u575b, signature=fn(d128) -> u575b>(%[[VALUE_testu575]], neg<d128>(const<d128>(1.))), widen<u575b, reason=usual_arith>(const<u1b>(0)))
// DEFAULT-NEXT:             write<bool>(%[[VALUE47]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE47]], ne<u575b>(call<u575b, signature=fn(d128) -> u575b>(%[[VALUE_testu575]], neg<d128>(const<d128>(42.5e+15))), widen<u575b, reason=usual_arith>(const<u1b>(0))));
// DEFAULT-NEXT:         let %[[VALUE48:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE47]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE48]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE48]], ne<u575b>(call<u575b, signature=fn(d128) -> u575b>(%[[VALUE_testu575]], neg<d128>(const<d128>(9999999999999999999999999999999999e+6111))), widen<u575b, reason=usual_arith>(const<u1b>(0))));
// DEFAULT-NEXT:         let %[[VALUE49:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE48]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE49]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE49]], ne<u575b>(call<u575b, signature=fn(d128) -> u575b>(%[[VALUE_testu575]], const<d128>(1236652007365522670302512605098236.0e+140)), const<u575b>(123665200736552267030251260509823595017565674550605919957031528046448612553265933585158200530621522494798835713008069669675682517153375604983773077550946583958303386074349567)));
// DEFAULT-NEXT:         let %[[VALUE50:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE49]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE50]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE50]], ne<u575b>(call<u575b, signature=fn(d128) -> u575b>(%[[VALUE_testu575]], const<d128>(9999999999999999999999999999999999e+6111)), const<u575b>(123665200736552267030251260509823595017565674550605919957031528046448612553265933585158200530621522494798835713008069669675682517153375604983773077550946583958303386074349567)));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE50]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
