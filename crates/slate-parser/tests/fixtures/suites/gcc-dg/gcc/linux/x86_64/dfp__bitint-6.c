/* PR c/102989 */
/* { dg-require-effective-target bitint } */
/* { dg-options "-O2 -std=c23 -pedantic-errors" } */

#if __BITINT_MAXWIDTH__ >= 192
__attribute__((noipa)) _Decimal128
tests192 (_BitInt(192) b)
{
  return b;
}

__attribute__((noipa)) _Decimal128
testu192 (unsigned _BitInt(192) b)
{
  return b;
}
#endif

#if __BITINT_MAXWIDTH__ >= 575
__attribute__((noipa)) _Decimal128
tests575 (_BitInt(575) b)
{
  return b;
}

__attribute__((noipa)) _Decimal128
testu575 (unsigned _BitInt(575) b)
{
  return b;
}
#endif

int
main ()
{
  _Decimal128 a, b;
#define CHECK(x, y) (a = (x), b = (y), a != (y) || __builtin_memcmp (&a, &b, sizeof (a)))
#if __BITINT_MAXWIDTH__ >= 192
  if (CHECK (tests192 (0wb), 0.DL)
      || CHECK (tests192 (7wb), 7.DL)
      || CHECK (tests192 (-42wb), -42.DL)
      || CHECK (tests192 (-777777777wb), -777777777.DL)
      || CHECK (tests192 (-12345678912345wb), -12345678912345.DL)
      || CHECK (tests192 (123456789123456789wb), 123456789123456789.DL)
      || CHECK (tests192 (777777777777777777777777777wb), 777777777777777777777777777.DL)
      || CHECK (tests192 (9999999999999999999999999900000000wb), 9999999999999999999999999900000000.DL)
      || CHECK (tests192 (-9999999999999999999999999999999999wb), -9999999999999999999999999999999999.DL)
      || CHECK (tests192 (-99999999999999999999999999999999994wb), -9999999999999999999999999999999999.e+1DL)
      || CHECK (tests192 (99999999999999999999999999999999995wb), 1000000000000000000000000000000000.e+2DL)
      || CHECK (tests192 (999999999999999999999999999999999900wb), 9999999999999999999999999999999999.e+2DL)
      || CHECK (tests192 (999999999999999999999999999999999949wb), 9999999999999999999999999999999999.e+2DL)
      || CHECK (tests192 (-9999999999999999999999999999999999000wb), -9999999999999999999999999999999999.e+3DL)
      || CHECK (tests192 (9999999999999999999999999999999999499wb), 9999999999999999999999999999999999.e+3DL)
      || CHECK (tests192 (34242319854454290000000000000000000000wb), 3424231985445429000000000000000000e+4DL)
      || CHECK (tests192 (34242319854454294983573424983275760000wb), 3424231985445429498357342498327576e+4DL)
      || CHECK (tests192 (999999999999999999999999999999999900000wb), 9999999999999999999999999999999999.e+5DL)
      || CHECK (tests192 (999999999999999999999999999999999949999wb), 9999999999999999999999999999999999.e+5DL)
      || CHECK (tests192 (-9999999999999999999999999999999999000000wb), -9999999999999999999999999999999999.e+6DL)
      || CHECK (tests192 (-9999999999999999999999999999999999499999wb), -9999999999999999999999999999999999.e+6DL)
      || CHECK (tests192 (123456789012345678901234567890123400000000wb), 1234567890123456789012345678901234.e+8DL)
      || CHECK (tests192 (999999999999999999999999999999999900000000000000000000000wb), 9999999999999999999999999999999999.e+23DL)
      || CHECK (tests192 (999999999999999999999999999999999949999999999999999999999wb), 9999999999999999999999999999999999.e+23DL)
      || CHECK (tests192 (-999999999999999999999999999999999900000000000000000000000wb), -9999999999999999999999999999999999.e+23DL)
      || CHECK (tests192 (-2138550877694459381917894711603833000000000000000000000000wb), -2138550877694459381917894711603833e+24DL)
      || CHECK (tests192 (-2138550877694459381917894711603833500000000000000000000000wb), -2138550877694459381917894711603834e+24DL)
      || CHECK (tests192 (-2138550877694459381917894711603833499999999999999999999999wb), -2138550877694459381917894711603833e+24DL)
      || CHECK (tests192 (-2138550877694459381917894711603833999999999999999999999999wb), -2138550877694459381917894711603834e+24DL)
      || CHECK (tests192 (-2138550877694459381917894711603832000000000000000000000000wb), -2138550877694459381917894711603832e+24DL)
      || CHECK (tests192 (-2138550877694459381917894711603832500000000000000000000000wb), -2138550877694459381917894711603832e+24DL)
      || CHECK (tests192 (-2138550877694459381917894711603832500000000000000000000001wb), -2138550877694459381917894711603833e+24DL)
      || CHECK (tests192 (3138550867693340381917894711603833000000000000000000000000wb), 3138550867693340381917894711603833e+24DL)
      || CHECK (tests192 (3138550867693340381917894711603833208051177722232017256447wb), 3138550867693340381917894711603833e+24DL)
      || CHECK (tests192 (-3138550867693340381917894711603833000000000000000000000000wb), -3138550867693340381917894711603833e+24DL)
      || CHECK (tests192 (-3138550867693340381917894711603833208051177722232017256447wb - 1wb), -3138550867693340381917894711603833e+24DL))
    __builtin_abort ();
  if (CHECK (testu192 (0uwb), 0.DL)
      || CHECK (testu192 (7uwb), 7.DL)
      || CHECK (testu192 (42uwb), 42.DL)
      || CHECK (testu192 (777777777uwb), 777777777.DL)
      || CHECK (testu192 (99999999999999999999999999000uwb), 99999999999999999999999999000.DL)
      || CHECK (testu192 (999999999999999999999999999999999900uwb), 9999999999999999999999999999999999.e+2DL)
      || CHECK (testu192 (9999999999999999999999999999999999000uwb), 9999999999999999999999999999999999.e+3DL)
      || CHECK (testu192 (99999999999999999999999999999999994999uwb), 9999999999999999999999999999999999.e+4DL)
      || CHECK (testu192 (999999999999999999999999999999999900000uwb), 9999999999999999999999999999999999.e+5DL)
      || CHECK (testu192 (9999999999999999999999999999999999000000uwb), 9999999999999999999999999999999999.e+6DL)
      || CHECK (testu192 (123456789012345600000000uwb), 123456789012345600000000.DL)
      || CHECK (testu192 (34242319854454290000000000000000000000uwb), 3424231985445429000000000000000000e+4DL)
      || CHECK (testu192 (999999999999999999999999999999999900000000000000000000000uwb), 9999999999999999999999999999999999.e+23DL)
      || CHECK (testu192 (6189354365465174593875438957438959000000000000000000000000uwb), 6189354365465174593875438957438959e+24DL)
      || CHECK (testu192 (6189354365465174593875438957438959500000000000000000000000uwb), 6189354365465174593875438957438960e+24DL)
      || CHECK (testu192 (6189354365465174593875438957438959499999999999999999999999uwb), 6189354365465174593875438957438959e+24DL)
      || CHECK (testu192 (6189354365465174593875438957438959999999999999999999999999uwb), 6189354365465174593875438957438960e+24DL)
      || CHECK (testu192 (6189354365465174593875438957438958000000000000000000000000uwb), 6189354365465174593875438957438958e+24DL)
      || CHECK (testu192 (6189354365465174593875438957438958500000000000000000000000uwb), 6189354365465174593875438957438958e+24DL)
      || CHECK (testu192 (6189354365465174593875438957438958500000000000000000000001uwb), 6189354365465174593875438957438959e+24DL)
      || CHECK (testu192 (6277101735386680763835789423207666000000000000000000000000uwb), 6277101735386680763835789423207666e+24DL)
      || CHECK (testu192 (6277101735386680763835789423207666416102355444464034512895uwb), 6277101735386680763835789423207666e+24DL))
    __builtin_abort ();
#endif
#if __BITINT_MAXWIDTH__ >= 575
  if (CHECK (tests575 (0wb), 0.DL)
      || CHECK (tests575 (7wb), 7.DL)
      || CHECK (tests575 (-42wb), -42.DL)
      || CHECK (tests575 (-444444444wb), -444444444.DL)
      || CHECK (tests575 (-3333333333333333wb), -3333333333333333.DL)
      || CHECK (tests575 (99999999999999999999999999000wb), 99999999999999999999999999000.DL)
      || CHECK (tests575 (-9999999999999999999999999999999999wb), -9999999999999999999999999999999999.DL)
      || CHECK (tests575 (999999999999999999999999999999999900wb), 9999999999999999999999999999999999.e+2DL)
      || CHECK (tests575 (-9999999999999999999999999999999999000wb), -9999999999999999999999999999999999.e+3DL)
      || CHECK (tests575 (999999999999999999999999999999999900000wb), 9999999999999999999999999999999999.e+5DL)
      || CHECK (tests575 (-99999999999999999999999999999999990000000wb), -9999999999999999999999999999999999.e+7DL)
      || CHECK (tests575 (1234567890123456000000000wb), 1234567890123456000000000.DL)
      || CHECK (tests575 (3424231985445429000000000000000000000000wb), 3424231985445429000000000000000000e+6DL)
      || CHECK (tests575 (99999999999999999999999999999999990000000000000000000000000000000000000000wb), 9999999999999999999999999999999999.e+40DL)
      || CHECK (tests575 (-9999999999999999999999999999999999000000000000000000000000000000000000000000000000000000000000000wb), -9999999999999999999999999999999999.e+63DL)
      || CHECK (tests575 (-213855087769441389758947543987475900000000000000000000000000000000000000000000000000000000000000000000000000000000000000wb), -2138550877694413897589475439874759e+86DL)
      || CHECK (tests575 (-213855087769441389758947543987475950000000000000000000000000000000000000000000000000000000000000000000000000000000000000wb), -2138550877694413897589475439874760e+86DL)
      || CHECK (tests575 (-213855087769441389758947543987475949999999999999999999999999999999999999999999999999999999999999999999999999999999999999wb), -2138550877694413897589475439874759e+86DL)
      || CHECK (tests575 (-213855087769441389758947543987475999999999999999999999999999999999999999999999999999999999999999999999999999999999999999wb), -2138550877694413897589475439874760e+86DL)
      || CHECK (tests575 (-213855087769441389758947543987475800000000000000000000000000000000000000000000000000000000000000000000000000000000000000wb), -2138550877694413897589475439874758e+86DL)
      || CHECK (tests575 (-213855087769441389758947543987475850000000000000000000000000000000000000000000000000000000000000000000000000000000000000wb), -2138550877694413897589475439874758e+86DL)
      || CHECK (tests575 (-213855087769441389758947543987475850000000000000000000000000000000000000000000000000000000000000000000000000000000000001wb), -2138550877694413897589475439874759e+86DL)
      || CHECK (tests575 (61832600368276133515125630254911790000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000wb), 6183260036827613351512563025491179e+139DL)
      || CHECK (tests575 (61832600368276133515125630254911797508782837275302959978515764023224306276632966792579100265310761247399417856504034834837841258576687802491886538775473291979151693037174783wb), 6183260036827613351512563025491180e+139DL)
      || CHECK (tests575 (-61832600368276133515125630254911790000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000wb), -6183260036827613351512563025491179e+139DL)
      || CHECK (tests575 (-61832600368276133515125630254911797508782837275302959978515764023224306276632966792579100265310761247399417856504034834837841258576687802491886538775473291979151693037174783wb - 1wb), -6183260036827613351512563025491180e+139DL))
    __builtin_abort ();
  if (CHECK (testu575 (0uwb), 0.DL)
      || CHECK (testu575 (17uwb), 17.DL)
      || CHECK (testu575 (420uwb), 420.DL)
      || CHECK (testu575 (888888888uwb), 888888888.DL)
      || CHECK (testu575 (9999999999999000uwb), 9999999999999000.DL)
      || CHECK (testu575 (99999999999999999999999999999999990000000uwb), 9999999999999999999999999999999999.e+7DL)
      || CHECK (testu575 (9999999999999999999999999999999999000000000uwb), 9999999999999999999999999999999999.e+9DL)
      || CHECK (testu575 (99999999999999999999999999999999990000000000000uwb), 9999999999999999999999999999999999.e+13DL)
      || CHECK (testu575 (9999999999999999999999999999999999000000000000000uwb), 9999999999999999999999999999999999.e+15DL)
      || CHECK (testu575 (1234567890123456000000000000000000uwb), 1234567890123456000000000000000000.DL)
      || CHECK (testu575 (34242319854454290000000000000000000000uwb), 3424231985445429000000000000000000e+4DL)
      || CHECK (testu575 (9999999999999999999999999999999999000000000000000000000000000000000uwb), 9999999999999999999999999999999999.e+33DL)
      || CHECK (testu575 (618935436546517949837539847534981700000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000uwb), 6189354365465179498375398475349817e+104DL)
      || CHECK (testu575 (618935436546517949837539847534981750000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000uwb), 6189354365465179498375398475349818e+104DL)
      || CHECK (testu575 (618935436546517949837539847534981749999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999uwb), 6189354365465179498375398475349817e+104DL)
      || CHECK (testu575 (618935436546517949837539847534981799999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999uwb), 6189354365465179498375398475349818e+104DL)
      || CHECK (testu575 (618935436546517949837539847534981800000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000uwb), 6189354365465179498375398475349818e+104DL)
      || CHECK (testu575 (618935436546517949837539847534981850000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000uwb), 6189354365465179498375398475349818e+104DL)
      || CHECK (testu575 (618935436546517949837539847534981850000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001uwb), 6189354365465179498375398475349819e+104DL)
      || CHECK (testu575 (99999999999999999999999999999999990000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000uwb), 9999999999999999999999999999999999.e+139DL)
      || CHECK (testu575 (123665200736552267030251260509823500000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000uwb), 1236652007365522670302512605098235e+140DL)
      || CHECK (testu575 (123665200736552267030251260509823549999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999uwb), 1236652007365522670302512605098235e+140DL)
      || CHECK (testu575 (123665200736552267030251260509823550000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000uwb), 1236652007365522670302512605098236e+140DL)
      || CHECK (testu575 (123665200736552267030251260509823550000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001uwb), 1236652007365522670302512605098236e+140DL)
      || CHECK (testu575 (123665200736552267030251260509823550000000000000000000000000000000000000001000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000uwb), 1236652007365522670302512605098236e+140DL)
      || CHECK (testu575 (123665200736552267030251260509823595017565674550605919957031528046448612553265933585158200530621522494798835713008069669675682517153375604983773077550946583958303386074349567uwb), 1236652007365522670302512605098236e+140DL))
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
// DEFAULT-NEXT:     fn %0 @tests192(%1 b: i192b) -> d128 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return int_to_float<d128, reason=return, exact=false, rounding=nearest_even, exceptions=observable>(read<i192b>(%1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %2 @testu192(%3 b: u192b) -> d128 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return int_to_float<d128, reason=return, exact=false, rounding=nearest_even, exceptions=observable>(read<u192b>(%3));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @tests575(%5 b: i575b) -> d128 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return int_to_float<d128, reason=return, exact=false, rounding=nearest_even, exceptions=observable>(read<i575b>(%5));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @testu575(%7 b: u575b) -> d128 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return int_to_float<d128, reason=return, exact=false, rounding=nearest_even, exceptions=observable>(read<u575b>(%7));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @__builtin_memcmp(%11 <unnamed>: ptr<const void>, %12 <unnamed>: ptr<const void>, %13 <unnamed>: u64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %15 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %8 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %9 a: d128 [storage=automatic];
// DEFAULT-NEXT:         let %10 b: d128 [storage=automatic];
// DEFAULT-NEXT:         write<d128>(%9, call<d128, signature=fn(i192b) -> d128>(%0, widen<i192b, reason=arg>(const<i2b>(0))));
// DEFAULT-NEXT:         call<d128, signature=fn(i192b) -> d128>(%0, widen<i192b, reason=arg>(const<i2b>(0)));
// DEFAULT-NEXT:         write<d128>(%10, const<d128>(0.));
// DEFAULT-NEXT:         let %16: bool [synthetic];
// DEFAULT-NEXT:         if ne<d128, exceptions=observable>(read<d128>(%9), const<d128>(0.))
// DEFAULT-NEXT:             write<bool>(%16, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%16, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:         let %17: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%16)
// DEFAULT-NEXT:             write<bool>(%17, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(i192b) -> d128>(%0, widen<i192b, reason=arg>(const<i4b>(7))));
// DEFAULT-NEXT:             call<d128, signature=fn(i192b) -> d128>(%0, widen<i192b, reason=arg>(const<i4b>(7)));
// DEFAULT-NEXT:             write<d128>(%10, const<d128>(7.));
// DEFAULT-NEXT:             let %18: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), const<d128>(7.))
// DEFAULT-NEXT:                 write<bool>(%18, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%18, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%17, read<bool>(%18));
// DEFAULT-NEXT:         let %19: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%17)
// DEFAULT-NEXT:             write<bool>(%19, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(i192b) -> d128>(%0, widen<i192b, reason=arg>(neg<i7b, overflow=ub>(const<i7b>(42)))));
// DEFAULT-NEXT:             call<d128, signature=fn(i192b) -> d128>(%0, widen<i192b, reason=arg>(neg<i7b, overflow=ub>(const<i7b>(42))));
// DEFAULT-NEXT:             write<d128>(%10, neg<d128>(const<d128>(42.)));
// DEFAULT-NEXT:             let %20: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), neg<d128>(const<d128>(42.)))
// DEFAULT-NEXT:                 write<bool>(%20, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%20, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%19, read<bool>(%20));
// DEFAULT-NEXT:         let %21: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%19)
// DEFAULT-NEXT:             write<bool>(%21, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(i192b) -> d128>(%0, widen<i192b, reason=arg>(neg<i31b, overflow=ub>(const<i31b>(777777777)))));
// DEFAULT-NEXT:             call<d128, signature=fn(i192b) -> d128>(%0, widen<i192b, reason=arg>(neg<i31b, overflow=ub>(const<i31b>(777777777))));
// DEFAULT-NEXT:             write<d128>(%10, neg<d128>(const<d128>(777777777.)));
// DEFAULT-NEXT:             let %22: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), neg<d128>(const<d128>(777777777.)))
// DEFAULT-NEXT:                 write<bool>(%22, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%22, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%21, read<bool>(%22));
// DEFAULT-NEXT:         let %23: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%21)
// DEFAULT-NEXT:             write<bool>(%23, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(i192b) -> d128>(%0, widen<i192b, reason=arg>(neg<i45b, overflow=ub>(const<i45b>(12345678912345)))));
// DEFAULT-NEXT:             call<d128, signature=fn(i192b) -> d128>(%0, widen<i192b, reason=arg>(neg<i45b, overflow=ub>(const<i45b>(12345678912345))));
// DEFAULT-NEXT:             write<d128>(%10, neg<d128>(const<d128>(12345678912345.)));
// DEFAULT-NEXT:             let %24: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), neg<d128>(const<d128>(12345678912345.)))
// DEFAULT-NEXT:                 write<bool>(%24, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%24, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%23, read<bool>(%24));
// DEFAULT-NEXT:         let %25: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%23)
// DEFAULT-NEXT:             write<bool>(%25, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(i192b) -> d128>(%0, widen<i192b, reason=arg>(const<i58b>(123456789123456789))));
// DEFAULT-NEXT:             call<d128, signature=fn(i192b) -> d128>(%0, widen<i192b, reason=arg>(const<i58b>(123456789123456789)));
// DEFAULT-NEXT:             write<d128>(%10, const<d128>(123456789123456789.));
// DEFAULT-NEXT:             let %26: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), const<d128>(123456789123456789.))
// DEFAULT-NEXT:                 write<bool>(%26, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%26, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%25, read<bool>(%26));
// DEFAULT-NEXT:         let %27: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%25)
// DEFAULT-NEXT:             write<bool>(%27, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(i192b) -> d128>(%0, widen<i192b, reason=arg>(const<i91b>(777777777777777777777777777))));
// DEFAULT-NEXT:             call<d128, signature=fn(i192b) -> d128>(%0, widen<i192b, reason=arg>(const<i91b>(777777777777777777777777777)));
// DEFAULT-NEXT:             write<d128>(%10, const<d128>(777777777777777777777777777.));
// DEFAULT-NEXT:             let %28: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), const<d128>(777777777777777777777777777.))
// DEFAULT-NEXT:                 write<bool>(%28, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%28, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%27, read<bool>(%28));
// DEFAULT-NEXT:         let %29: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%27)
// DEFAULT-NEXT:             write<bool>(%29, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(i192b) -> d128>(%0, widen<i192b, reason=arg>(const<i114b>(9999999999999999999999999900000000))));
// DEFAULT-NEXT:             call<d128, signature=fn(i192b) -> d128>(%0, widen<i192b, reason=arg>(const<i114b>(9999999999999999999999999900000000)));
// DEFAULT-NEXT:             write<d128>(%10, const<d128>(9999999999999999999999999900000000.));
// DEFAULT-NEXT:             let %30: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), const<d128>(9999999999999999999999999900000000.))
// DEFAULT-NEXT:                 write<bool>(%30, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%30, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%29, read<bool>(%30));
// DEFAULT-NEXT:         let %31: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%29)
// DEFAULT-NEXT:             write<bool>(%31, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(i192b) -> d128>(%0, widen<i192b, reason=arg>(neg<i114b, overflow=ub>(const<i114b>(9999999999999999999999999999999999)))));
// DEFAULT-NEXT:             call<d128, signature=fn(i192b) -> d128>(%0, widen<i192b, reason=arg>(neg<i114b, overflow=ub>(const<i114b>(9999999999999999999999999999999999))));
// DEFAULT-NEXT:             write<d128>(%10, neg<d128>(const<d128>(9999999999999999999999999999999999.)));
// DEFAULT-NEXT:             let %32: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), neg<d128>(const<d128>(9999999999999999999999999999999999.)))
// DEFAULT-NEXT:                 write<bool>(%32, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%32, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%31, read<bool>(%32));
// DEFAULT-NEXT:         let %33: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%31)
// DEFAULT-NEXT:             write<bool>(%33, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(i192b) -> d128>(%0, widen<i192b, reason=arg>(neg<i118b, overflow=ub>(const<i118b>(99999999999999999999999999999999994)))));
// DEFAULT-NEXT:             call<d128, signature=fn(i192b) -> d128>(%0, widen<i192b, reason=arg>(neg<i118b, overflow=ub>(const<i118b>(99999999999999999999999999999999994))));
// DEFAULT-NEXT:             write<d128>(%10, neg<d128>(const<d128>(9999999999999999999999999999999999.e+1)));
// DEFAULT-NEXT:             let %34: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), neg<d128>(const<d128>(9999999999999999999999999999999999.e+1)))
// DEFAULT-NEXT:                 write<bool>(%34, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%34, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%33, read<bool>(%34));
// DEFAULT-NEXT:         let %35: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%33)
// DEFAULT-NEXT:             write<bool>(%35, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(i192b) -> d128>(%0, widen<i192b, reason=arg>(const<i118b>(99999999999999999999999999999999995))));
// DEFAULT-NEXT:             call<d128, signature=fn(i192b) -> d128>(%0, widen<i192b, reason=arg>(const<i118b>(99999999999999999999999999999999995)));
// DEFAULT-NEXT:             write<d128>(%10, const<d128>(1000000000000000000000000000000000.e+2));
// DEFAULT-NEXT:             let %36: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), const<d128>(1000000000000000000000000000000000.e+2))
// DEFAULT-NEXT:                 write<bool>(%36, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%36, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%35, read<bool>(%36));
// DEFAULT-NEXT:         let %37: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%35)
// DEFAULT-NEXT:             write<bool>(%37, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(i192b) -> d128>(%0, widen<i192b, reason=arg>(const<i121b>(999999999999999999999999999999999900))));
// DEFAULT-NEXT:             call<d128, signature=fn(i192b) -> d128>(%0, widen<i192b, reason=arg>(const<i121b>(999999999999999999999999999999999900)));
// DEFAULT-NEXT:             write<d128>(%10, const<d128>(9999999999999999999999999999999999.e+2));
// DEFAULT-NEXT:             let %38: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), const<d128>(9999999999999999999999999999999999.e+2))
// DEFAULT-NEXT:                 write<bool>(%38, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%38, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%37, read<bool>(%38));
// DEFAULT-NEXT:         let %39: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%37)
// DEFAULT-NEXT:             write<bool>(%39, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(i192b) -> d128>(%0, widen<i192b, reason=arg>(const<i121b>(999999999999999999999999999999999949))));
// DEFAULT-NEXT:             call<d128, signature=fn(i192b) -> d128>(%0, widen<i192b, reason=arg>(const<i121b>(999999999999999999999999999999999949)));
// DEFAULT-NEXT:             write<d128>(%10, const<d128>(9999999999999999999999999999999999.e+2));
// DEFAULT-NEXT:             let %40: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), const<d128>(9999999999999999999999999999999999.e+2))
// DEFAULT-NEXT:                 write<bool>(%40, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%40, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%39, read<bool>(%40));
// DEFAULT-NEXT:         let %41: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%39)
// DEFAULT-NEXT:             write<bool>(%41, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(i192b) -> d128>(%0, widen<i192b, reason=arg>(neg<i124b, overflow=ub>(const<i124b>(9999999999999999999999999999999999000)))));
// DEFAULT-NEXT:             call<d128, signature=fn(i192b) -> d128>(%0, widen<i192b, reason=arg>(neg<i124b, overflow=ub>(const<i124b>(9999999999999999999999999999999999000))));
// DEFAULT-NEXT:             write<d128>(%10, neg<d128>(const<d128>(9999999999999999999999999999999999.e+3)));
// DEFAULT-NEXT:             let %42: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), neg<d128>(const<d128>(9999999999999999999999999999999999.e+3)))
// DEFAULT-NEXT:                 write<bool>(%42, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%42, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%41, read<bool>(%42));
// DEFAULT-NEXT:         let %43: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%41)
// DEFAULT-NEXT:             write<bool>(%43, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(i192b) -> d128>(%0, widen<i192b, reason=arg>(const<i124b>(9999999999999999999999999999999999499))));
// DEFAULT-NEXT:             call<d128, signature=fn(i192b) -> d128>(%0, widen<i192b, reason=arg>(const<i124b>(9999999999999999999999999999999999499)));
// DEFAULT-NEXT:             write<d128>(%10, const<d128>(9999999999999999999999999999999999.e+3));
// DEFAULT-NEXT:             let %44: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), const<d128>(9999999999999999999999999999999999.e+3))
// DEFAULT-NEXT:                 write<bool>(%44, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%44, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%43, read<bool>(%44));
// DEFAULT-NEXT:         let %45: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%43)
// DEFAULT-NEXT:             write<bool>(%45, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(i192b) -> d128>(%0, widen<i192b, reason=arg>(const<i126b>(34242319854454290000000000000000000000))));
// DEFAULT-NEXT:             call<d128, signature=fn(i192b) -> d128>(%0, widen<i192b, reason=arg>(const<i126b>(34242319854454290000000000000000000000)));
// DEFAULT-NEXT:             write<d128>(%10, const<d128>(3424231985445429000000000000000000e+4));
// DEFAULT-NEXT:             let %46: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), const<d128>(3424231985445429000000000000000000e+4))
// DEFAULT-NEXT:                 write<bool>(%46, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%46, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%45, read<bool>(%46));
// DEFAULT-NEXT:         let %47: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%45)
// DEFAULT-NEXT:             write<bool>(%47, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(i192b) -> d128>(%0, widen<i192b, reason=arg>(const<i126b>(34242319854454294983573424983275760000))));
// DEFAULT-NEXT:             call<d128, signature=fn(i192b) -> d128>(%0, widen<i192b, reason=arg>(const<i126b>(34242319854454294983573424983275760000)));
// DEFAULT-NEXT:             write<d128>(%10, const<d128>(3424231985445429498357342498327576e+4));
// DEFAULT-NEXT:             let %48: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), const<d128>(3424231985445429498357342498327576e+4))
// DEFAULT-NEXT:                 write<bool>(%48, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%48, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%47, read<bool>(%48));
// DEFAULT-NEXT:         let %49: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%47)
// DEFAULT-NEXT:             write<bool>(%49, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(i192b) -> d128>(%0, widen<i192b, reason=arg>(const<i131b>(999999999999999999999999999999999900000))));
// DEFAULT-NEXT:             call<d128, signature=fn(i192b) -> d128>(%0, widen<i192b, reason=arg>(const<i131b>(999999999999999999999999999999999900000)));
// DEFAULT-NEXT:             write<d128>(%10, const<d128>(9999999999999999999999999999999999.e+5));
// DEFAULT-NEXT:             let %50: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), const<d128>(9999999999999999999999999999999999.e+5))
// DEFAULT-NEXT:                 write<bool>(%50, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%50, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%49, read<bool>(%50));
// DEFAULT-NEXT:         let %51: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%49)
// DEFAULT-NEXT:             write<bool>(%51, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(i192b) -> d128>(%0, widen<i192b, reason=arg>(const<i131b>(999999999999999999999999999999999949999))));
// DEFAULT-NEXT:             call<d128, signature=fn(i192b) -> d128>(%0, widen<i192b, reason=arg>(const<i131b>(999999999999999999999999999999999949999)));
// DEFAULT-NEXT:             write<d128>(%10, const<d128>(9999999999999999999999999999999999.e+5));
// DEFAULT-NEXT:             let %52: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), const<d128>(9999999999999999999999999999999999.e+5))
// DEFAULT-NEXT:                 write<bool>(%52, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%52, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%51, read<bool>(%52));
// DEFAULT-NEXT:         let %53: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%51)
// DEFAULT-NEXT:             write<bool>(%53, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(i192b) -> d128>(%0, widen<i192b, reason=arg>(neg<i134b, overflow=ub>(const<i134b>(9999999999999999999999999999999999000000)))));
// DEFAULT-NEXT:             call<d128, signature=fn(i192b) -> d128>(%0, widen<i192b, reason=arg>(neg<i134b, overflow=ub>(const<i134b>(9999999999999999999999999999999999000000))));
// DEFAULT-NEXT:             write<d128>(%10, neg<d128>(const<d128>(9999999999999999999999999999999999.e+6)));
// DEFAULT-NEXT:             let %54: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), neg<d128>(const<d128>(9999999999999999999999999999999999.e+6)))
// DEFAULT-NEXT:                 write<bool>(%54, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%54, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%53, read<bool>(%54));
// DEFAULT-NEXT:         let %55: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%53)
// DEFAULT-NEXT:             write<bool>(%55, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(i192b) -> d128>(%0, widen<i192b, reason=arg>(neg<i134b, overflow=ub>(const<i134b>(9999999999999999999999999999999999499999)))));
// DEFAULT-NEXT:             call<d128, signature=fn(i192b) -> d128>(%0, widen<i192b, reason=arg>(neg<i134b, overflow=ub>(const<i134b>(9999999999999999999999999999999999499999))));
// DEFAULT-NEXT:             write<d128>(%10, neg<d128>(const<d128>(9999999999999999999999999999999999.e+6)));
// DEFAULT-NEXT:             let %56: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), neg<d128>(const<d128>(9999999999999999999999999999999999.e+6)))
// DEFAULT-NEXT:                 write<bool>(%56, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%56, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%55, read<bool>(%56));
// DEFAULT-NEXT:         let %57: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%55)
// DEFAULT-NEXT:             write<bool>(%57, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(i192b) -> d128>(%0, widen<i192b, reason=arg>(const<i138b>(123456789012345678901234567890123400000000))));
// DEFAULT-NEXT:             call<d128, signature=fn(i192b) -> d128>(%0, widen<i192b, reason=arg>(const<i138b>(123456789012345678901234567890123400000000)));
// DEFAULT-NEXT:             write<d128>(%10, const<d128>(1234567890123456789012345678901234.e+8));
// DEFAULT-NEXT:             let %58: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), const<d128>(1234567890123456789012345678901234.e+8))
// DEFAULT-NEXT:                 write<bool>(%58, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%58, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%57, read<bool>(%58));
// DEFAULT-NEXT:         let %59: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%57)
// DEFAULT-NEXT:             write<bool>(%59, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(i192b) -> d128>(%0, widen<i192b, reason=arg>(const<i191b>(999999999999999999999999999999999900000000000000000000000))));
// DEFAULT-NEXT:             call<d128, signature=fn(i192b) -> d128>(%0, widen<i192b, reason=arg>(const<i191b>(999999999999999999999999999999999900000000000000000000000)));
// DEFAULT-NEXT:             write<d128>(%10, const<d128>(9999999999999999999999999999999999.e+23));
// DEFAULT-NEXT:             let %60: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), const<d128>(9999999999999999999999999999999999.e+23))
// DEFAULT-NEXT:                 write<bool>(%60, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%60, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%59, read<bool>(%60));
// DEFAULT-NEXT:         let %61: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%59)
// DEFAULT-NEXT:             write<bool>(%61, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(i192b) -> d128>(%0, widen<i192b, reason=arg>(const<i191b>(999999999999999999999999999999999949999999999999999999999))));
// DEFAULT-NEXT:             call<d128, signature=fn(i192b) -> d128>(%0, widen<i192b, reason=arg>(const<i191b>(999999999999999999999999999999999949999999999999999999999)));
// DEFAULT-NEXT:             write<d128>(%10, const<d128>(9999999999999999999999999999999999.e+23));
// DEFAULT-NEXT:             let %62: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), const<d128>(9999999999999999999999999999999999.e+23))
// DEFAULT-NEXT:                 write<bool>(%62, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%62, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%61, read<bool>(%62));
// DEFAULT-NEXT:         let %63: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%61)
// DEFAULT-NEXT:             write<bool>(%63, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(i192b) -> d128>(%0, widen<i192b, reason=arg>(neg<i191b, overflow=ub>(const<i191b>(999999999999999999999999999999999900000000000000000000000)))));
// DEFAULT-NEXT:             call<d128, signature=fn(i192b) -> d128>(%0, widen<i192b, reason=arg>(neg<i191b, overflow=ub>(const<i191b>(999999999999999999999999999999999900000000000000000000000))));
// DEFAULT-NEXT:             write<d128>(%10, neg<d128>(const<d128>(9999999999999999999999999999999999.e+23)));
// DEFAULT-NEXT:             let %64: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), neg<d128>(const<d128>(9999999999999999999999999999999999.e+23)))
// DEFAULT-NEXT:                 write<bool>(%64, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%64, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%63, read<bool>(%64));
// DEFAULT-NEXT:         let %65: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%63)
// DEFAULT-NEXT:             write<bool>(%65, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(i192b) -> d128>(%0, neg<i192b, overflow=ub>(const<i192b>(2138550877694459381917894711603833000000000000000000000000))));
// DEFAULT-NEXT:             call<d128, signature=fn(i192b) -> d128>(%0, neg<i192b, overflow=ub>(const<i192b>(2138550877694459381917894711603833000000000000000000000000)));
// DEFAULT-NEXT:             write<d128>(%10, neg<d128>(const<d128>(2138550877694459381917894711603833e+24)));
// DEFAULT-NEXT:             let %66: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), neg<d128>(const<d128>(2138550877694459381917894711603833e+24)))
// DEFAULT-NEXT:                 write<bool>(%66, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%66, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%65, read<bool>(%66));
// DEFAULT-NEXT:         let %67: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%65)
// DEFAULT-NEXT:             write<bool>(%67, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(i192b) -> d128>(%0, neg<i192b, overflow=ub>(const<i192b>(2138550877694459381917894711603833500000000000000000000000))));
// DEFAULT-NEXT:             call<d128, signature=fn(i192b) -> d128>(%0, neg<i192b, overflow=ub>(const<i192b>(2138550877694459381917894711603833500000000000000000000000)));
// DEFAULT-NEXT:             write<d128>(%10, neg<d128>(const<d128>(2138550877694459381917894711603834e+24)));
// DEFAULT-NEXT:             let %68: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), neg<d128>(const<d128>(2138550877694459381917894711603834e+24)))
// DEFAULT-NEXT:                 write<bool>(%68, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%68, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%67, read<bool>(%68));
// DEFAULT-NEXT:         let %69: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%67)
// DEFAULT-NEXT:             write<bool>(%69, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(i192b) -> d128>(%0, neg<i192b, overflow=ub>(const<i192b>(2138550877694459381917894711603833499999999999999999999999))));
// DEFAULT-NEXT:             call<d128, signature=fn(i192b) -> d128>(%0, neg<i192b, overflow=ub>(const<i192b>(2138550877694459381917894711603833499999999999999999999999)));
// DEFAULT-NEXT:             write<d128>(%10, neg<d128>(const<d128>(2138550877694459381917894711603833e+24)));
// DEFAULT-NEXT:             let %70: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), neg<d128>(const<d128>(2138550877694459381917894711603833e+24)))
// DEFAULT-NEXT:                 write<bool>(%70, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%70, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%69, read<bool>(%70));
// DEFAULT-NEXT:         let %71: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%69)
// DEFAULT-NEXT:             write<bool>(%71, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(i192b) -> d128>(%0, neg<i192b, overflow=ub>(const<i192b>(2138550877694459381917894711603833999999999999999999999999))));
// DEFAULT-NEXT:             call<d128, signature=fn(i192b) -> d128>(%0, neg<i192b, overflow=ub>(const<i192b>(2138550877694459381917894711603833999999999999999999999999)));
// DEFAULT-NEXT:             write<d128>(%10, neg<d128>(const<d128>(2138550877694459381917894711603834e+24)));
// DEFAULT-NEXT:             let %72: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), neg<d128>(const<d128>(2138550877694459381917894711603834e+24)))
// DEFAULT-NEXT:                 write<bool>(%72, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%72, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%71, read<bool>(%72));
// DEFAULT-NEXT:         let %73: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%71)
// DEFAULT-NEXT:             write<bool>(%73, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(i192b) -> d128>(%0, neg<i192b, overflow=ub>(const<i192b>(2138550877694459381917894711603832000000000000000000000000))));
// DEFAULT-NEXT:             call<d128, signature=fn(i192b) -> d128>(%0, neg<i192b, overflow=ub>(const<i192b>(2138550877694459381917894711603832000000000000000000000000)));
// DEFAULT-NEXT:             write<d128>(%10, neg<d128>(const<d128>(2138550877694459381917894711603832e+24)));
// DEFAULT-NEXT:             let %74: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), neg<d128>(const<d128>(2138550877694459381917894711603832e+24)))
// DEFAULT-NEXT:                 write<bool>(%74, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%74, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%73, read<bool>(%74));
// DEFAULT-NEXT:         let %75: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%73)
// DEFAULT-NEXT:             write<bool>(%75, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(i192b) -> d128>(%0, neg<i192b, overflow=ub>(const<i192b>(2138550877694459381917894711603832500000000000000000000000))));
// DEFAULT-NEXT:             call<d128, signature=fn(i192b) -> d128>(%0, neg<i192b, overflow=ub>(const<i192b>(2138550877694459381917894711603832500000000000000000000000)));
// DEFAULT-NEXT:             write<d128>(%10, neg<d128>(const<d128>(2138550877694459381917894711603832e+24)));
// DEFAULT-NEXT:             let %76: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), neg<d128>(const<d128>(2138550877694459381917894711603832e+24)))
// DEFAULT-NEXT:                 write<bool>(%76, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%76, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%75, read<bool>(%76));
// DEFAULT-NEXT:         let %77: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%75)
// DEFAULT-NEXT:             write<bool>(%77, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(i192b) -> d128>(%0, neg<i192b, overflow=ub>(const<i192b>(2138550877694459381917894711603832500000000000000000000001))));
// DEFAULT-NEXT:             call<d128, signature=fn(i192b) -> d128>(%0, neg<i192b, overflow=ub>(const<i192b>(2138550877694459381917894711603832500000000000000000000001)));
// DEFAULT-NEXT:             write<d128>(%10, neg<d128>(const<d128>(2138550877694459381917894711603833e+24)));
// DEFAULT-NEXT:             let %78: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), neg<d128>(const<d128>(2138550877694459381917894711603833e+24)))
// DEFAULT-NEXT:                 write<bool>(%78, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%78, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%77, read<bool>(%78));
// DEFAULT-NEXT:         let %79: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%77)
// DEFAULT-NEXT:             write<bool>(%79, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(i192b) -> d128>(%0, const<i192b>(3138550867693340381917894711603833000000000000000000000000)));
// DEFAULT-NEXT:             call<d128, signature=fn(i192b) -> d128>(%0, const<i192b>(3138550867693340381917894711603833000000000000000000000000));
// DEFAULT-NEXT:             write<d128>(%10, const<d128>(3138550867693340381917894711603833e+24));
// DEFAULT-NEXT:             let %80: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), const<d128>(3138550867693340381917894711603833e+24))
// DEFAULT-NEXT:                 write<bool>(%80, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%80, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%79, read<bool>(%80));
// DEFAULT-NEXT:         let %81: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%79)
// DEFAULT-NEXT:             write<bool>(%81, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(i192b) -> d128>(%0, const<i192b>(3138550867693340381917894711603833208051177722232017256447)));
// DEFAULT-NEXT:             call<d128, signature=fn(i192b) -> d128>(%0, const<i192b>(3138550867693340381917894711603833208051177722232017256447));
// DEFAULT-NEXT:             write<d128>(%10, const<d128>(3138550867693340381917894711603833e+24));
// DEFAULT-NEXT:             let %82: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), const<d128>(3138550867693340381917894711603833e+24))
// DEFAULT-NEXT:                 write<bool>(%82, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%82, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%81, read<bool>(%82));
// DEFAULT-NEXT:         let %83: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%81)
// DEFAULT-NEXT:             write<bool>(%83, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(i192b) -> d128>(%0, neg<i192b, overflow=ub>(const<i192b>(3138550867693340381917894711603833000000000000000000000000))));
// DEFAULT-NEXT:             call<d128, signature=fn(i192b) -> d128>(%0, neg<i192b, overflow=ub>(const<i192b>(3138550867693340381917894711603833000000000000000000000000)));
// DEFAULT-NEXT:             write<d128>(%10, neg<d128>(const<d128>(3138550867693340381917894711603833e+24)));
// DEFAULT-NEXT:             let %84: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), neg<d128>(const<d128>(3138550867693340381917894711603833e+24)))
// DEFAULT-NEXT:                 write<bool>(%84, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%84, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%83, read<bool>(%84));
// DEFAULT-NEXT:         let %85: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%83)
// DEFAULT-NEXT:             write<bool>(%85, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(i192b) -> d128>(%0, sub<i192b, overflow=ub>(neg<i192b, overflow=ub>(const<i192b>(3138550867693340381917894711603833208051177722232017256447)), widen<i192b, reason=usual_arith>(const<i2b>(1)))));
// DEFAULT-NEXT:             call<d128, signature=fn(i192b) -> d128>(%0, sub<i192b, overflow=ub>(neg<i192b, overflow=ub>(const<i192b>(3138550867693340381917894711603833208051177722232017256447)), widen<i192b, reason=usual_arith>(const<i2b>(1))));
// DEFAULT-NEXT:             write<d128>(%10, neg<d128>(const<d128>(3138550867693340381917894711603833e+24)));
// DEFAULT-NEXT:             let %86: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), neg<d128>(const<d128>(3138550867693340381917894711603833e+24)))
// DEFAULT-NEXT:                 write<bool>(%86, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%86, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%85, read<bool>(%86));
// DEFAULT-NEXT:         if read<bool>(%85)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%15);
// DEFAULT-NEXT:         write<d128>(%9, call<d128, signature=fn(u192b) -> d128>(%2, widen<u192b, reason=arg>(const<u1b>(0))));
// DEFAULT-NEXT:         call<d128, signature=fn(u192b) -> d128>(%2, widen<u192b, reason=arg>(const<u1b>(0)));
// DEFAULT-NEXT:         write<d128>(%10, const<d128>(0.));
// DEFAULT-NEXT:         let %87: bool [synthetic];
// DEFAULT-NEXT:         if ne<d128, exceptions=observable>(read<d128>(%9), const<d128>(0.))
// DEFAULT-NEXT:             write<bool>(%87, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%87, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:         let %88: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%87)
// DEFAULT-NEXT:             write<bool>(%88, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(u192b) -> d128>(%2, widen<u192b, reason=arg>(const<u3b>(7))));
// DEFAULT-NEXT:             call<d128, signature=fn(u192b) -> d128>(%2, widen<u192b, reason=arg>(const<u3b>(7)));
// DEFAULT-NEXT:             write<d128>(%10, const<d128>(7.));
// DEFAULT-NEXT:             let %89: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), const<d128>(7.))
// DEFAULT-NEXT:                 write<bool>(%89, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%89, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%88, read<bool>(%89));
// DEFAULT-NEXT:         let %90: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%88)
// DEFAULT-NEXT:             write<bool>(%90, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(u192b) -> d128>(%2, widen<u192b, reason=arg>(const<u6b>(42))));
// DEFAULT-NEXT:             call<d128, signature=fn(u192b) -> d128>(%2, widen<u192b, reason=arg>(const<u6b>(42)));
// DEFAULT-NEXT:             write<d128>(%10, const<d128>(42.));
// DEFAULT-NEXT:             let %91: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), const<d128>(42.))
// DEFAULT-NEXT:                 write<bool>(%91, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%91, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%90, read<bool>(%91));
// DEFAULT-NEXT:         let %92: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%90)
// DEFAULT-NEXT:             write<bool>(%92, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(u192b) -> d128>(%2, widen<u192b, reason=arg>(const<u30b>(777777777))));
// DEFAULT-NEXT:             call<d128, signature=fn(u192b) -> d128>(%2, widen<u192b, reason=arg>(const<u30b>(777777777)));
// DEFAULT-NEXT:             write<d128>(%10, const<d128>(777777777.));
// DEFAULT-NEXT:             let %93: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), const<d128>(777777777.))
// DEFAULT-NEXT:                 write<bool>(%93, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%93, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%92, read<bool>(%93));
// DEFAULT-NEXT:         let %94: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%92)
// DEFAULT-NEXT:             write<bool>(%94, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(u192b) -> d128>(%2, widen<u192b, reason=arg>(const<u97b>(99999999999999999999999999000))));
// DEFAULT-NEXT:             call<d128, signature=fn(u192b) -> d128>(%2, widen<u192b, reason=arg>(const<u97b>(99999999999999999999999999000)));
// DEFAULT-NEXT:             write<d128>(%10, const<d128>(99999999999999999999999999000.));
// DEFAULT-NEXT:             let %95: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), const<d128>(99999999999999999999999999000.))
// DEFAULT-NEXT:                 write<bool>(%95, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%95, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%94, read<bool>(%95));
// DEFAULT-NEXT:         let %96: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%94)
// DEFAULT-NEXT:             write<bool>(%96, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(u192b) -> d128>(%2, widen<u192b, reason=arg>(const<u120b>(999999999999999999999999999999999900))));
// DEFAULT-NEXT:             call<d128, signature=fn(u192b) -> d128>(%2, widen<u192b, reason=arg>(const<u120b>(999999999999999999999999999999999900)));
// DEFAULT-NEXT:             write<d128>(%10, const<d128>(9999999999999999999999999999999999.e+2));
// DEFAULT-NEXT:             let %97: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), const<d128>(9999999999999999999999999999999999.e+2))
// DEFAULT-NEXT:                 write<bool>(%97, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%97, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%96, read<bool>(%97));
// DEFAULT-NEXT:         let %98: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%96)
// DEFAULT-NEXT:             write<bool>(%98, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(u192b) -> d128>(%2, widen<u192b, reason=arg>(const<u123b>(9999999999999999999999999999999999000))));
// DEFAULT-NEXT:             call<d128, signature=fn(u192b) -> d128>(%2, widen<u192b, reason=arg>(const<u123b>(9999999999999999999999999999999999000)));
// DEFAULT-NEXT:             write<d128>(%10, const<d128>(9999999999999999999999999999999999.e+3));
// DEFAULT-NEXT:             let %99: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), const<d128>(9999999999999999999999999999999999.e+3))
// DEFAULT-NEXT:                 write<bool>(%99, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%99, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%98, read<bool>(%99));
// DEFAULT-NEXT:         let %100: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%98)
// DEFAULT-NEXT:             write<bool>(%100, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(u192b) -> d128>(%2, widen<u192b, reason=arg>(const<u127b>(99999999999999999999999999999999994999))));
// DEFAULT-NEXT:             call<d128, signature=fn(u192b) -> d128>(%2, widen<u192b, reason=arg>(const<u127b>(99999999999999999999999999999999994999)));
// DEFAULT-NEXT:             write<d128>(%10, const<d128>(9999999999999999999999999999999999.e+4));
// DEFAULT-NEXT:             let %101: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), const<d128>(9999999999999999999999999999999999.e+4))
// DEFAULT-NEXT:                 write<bool>(%101, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%101, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%100, read<bool>(%101));
// DEFAULT-NEXT:         let %102: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%100)
// DEFAULT-NEXT:             write<bool>(%102, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(u192b) -> d128>(%2, widen<u192b, reason=arg>(const<u130b>(999999999999999999999999999999999900000))));
// DEFAULT-NEXT:             call<d128, signature=fn(u192b) -> d128>(%2, widen<u192b, reason=arg>(const<u130b>(999999999999999999999999999999999900000)));
// DEFAULT-NEXT:             write<d128>(%10, const<d128>(9999999999999999999999999999999999.e+5));
// DEFAULT-NEXT:             let %103: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), const<d128>(9999999999999999999999999999999999.e+5))
// DEFAULT-NEXT:                 write<bool>(%103, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%103, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%102, read<bool>(%103));
// DEFAULT-NEXT:         let %104: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%102)
// DEFAULT-NEXT:             write<bool>(%104, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(u192b) -> d128>(%2, widen<u192b, reason=arg>(const<u133b>(9999999999999999999999999999999999000000))));
// DEFAULT-NEXT:             call<d128, signature=fn(u192b) -> d128>(%2, widen<u192b, reason=arg>(const<u133b>(9999999999999999999999999999999999000000)));
// DEFAULT-NEXT:             write<d128>(%10, const<d128>(9999999999999999999999999999999999.e+6));
// DEFAULT-NEXT:             let %105: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), const<d128>(9999999999999999999999999999999999.e+6))
// DEFAULT-NEXT:                 write<bool>(%105, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%105, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%104, read<bool>(%105));
// DEFAULT-NEXT:         let %106: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%104)
// DEFAULT-NEXT:             write<bool>(%106, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(u192b) -> d128>(%2, widen<u192b, reason=arg>(const<u77b>(123456789012345600000000))));
// DEFAULT-NEXT:             call<d128, signature=fn(u192b) -> d128>(%2, widen<u192b, reason=arg>(const<u77b>(123456789012345600000000)));
// DEFAULT-NEXT:             write<d128>(%10, const<d128>(123456789012345600000000.));
// DEFAULT-NEXT:             let %107: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), const<d128>(123456789012345600000000.))
// DEFAULT-NEXT:                 write<bool>(%107, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%107, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%106, read<bool>(%107));
// DEFAULT-NEXT:         let %108: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%106)
// DEFAULT-NEXT:             write<bool>(%108, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(u192b) -> d128>(%2, widen<u192b, reason=arg>(const<u125b>(34242319854454290000000000000000000000))));
// DEFAULT-NEXT:             call<d128, signature=fn(u192b) -> d128>(%2, widen<u192b, reason=arg>(const<u125b>(34242319854454290000000000000000000000)));
// DEFAULT-NEXT:             write<d128>(%10, const<d128>(3424231985445429000000000000000000e+4));
// DEFAULT-NEXT:             let %109: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), const<d128>(3424231985445429000000000000000000e+4))
// DEFAULT-NEXT:                 write<bool>(%109, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%109, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%108, read<bool>(%109));
// DEFAULT-NEXT:         let %110: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%108)
// DEFAULT-NEXT:             write<bool>(%110, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(u192b) -> d128>(%2, widen<u192b, reason=arg>(const<u190b>(999999999999999999999999999999999900000000000000000000000))));
// DEFAULT-NEXT:             call<d128, signature=fn(u192b) -> d128>(%2, widen<u192b, reason=arg>(const<u190b>(999999999999999999999999999999999900000000000000000000000)));
// DEFAULT-NEXT:             write<d128>(%10, const<d128>(9999999999999999999999999999999999.e+23));
// DEFAULT-NEXT:             let %111: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), const<d128>(9999999999999999999999999999999999.e+23))
// DEFAULT-NEXT:                 write<bool>(%111, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%111, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%110, read<bool>(%111));
// DEFAULT-NEXT:         let %112: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%110)
// DEFAULT-NEXT:             write<bool>(%112, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(u192b) -> d128>(%2, const<u192b>(6189354365465174593875438957438959000000000000000000000000)));
// DEFAULT-NEXT:             call<d128, signature=fn(u192b) -> d128>(%2, const<u192b>(6189354365465174593875438957438959000000000000000000000000));
// DEFAULT-NEXT:             write<d128>(%10, const<d128>(6189354365465174593875438957438959e+24));
// DEFAULT-NEXT:             let %113: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), const<d128>(6189354365465174593875438957438959e+24))
// DEFAULT-NEXT:                 write<bool>(%113, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%113, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%112, read<bool>(%113));
// DEFAULT-NEXT:         let %114: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%112)
// DEFAULT-NEXT:             write<bool>(%114, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(u192b) -> d128>(%2, const<u192b>(6189354365465174593875438957438959500000000000000000000000)));
// DEFAULT-NEXT:             call<d128, signature=fn(u192b) -> d128>(%2, const<u192b>(6189354365465174593875438957438959500000000000000000000000));
// DEFAULT-NEXT:             write<d128>(%10, const<d128>(6189354365465174593875438957438960e+24));
// DEFAULT-NEXT:             let %115: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), const<d128>(6189354365465174593875438957438960e+24))
// DEFAULT-NEXT:                 write<bool>(%115, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%115, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%114, read<bool>(%115));
// DEFAULT-NEXT:         let %116: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%114)
// DEFAULT-NEXT:             write<bool>(%116, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(u192b) -> d128>(%2, const<u192b>(6189354365465174593875438957438959499999999999999999999999)));
// DEFAULT-NEXT:             call<d128, signature=fn(u192b) -> d128>(%2, const<u192b>(6189354365465174593875438957438959499999999999999999999999));
// DEFAULT-NEXT:             write<d128>(%10, const<d128>(6189354365465174593875438957438959e+24));
// DEFAULT-NEXT:             let %117: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), const<d128>(6189354365465174593875438957438959e+24))
// DEFAULT-NEXT:                 write<bool>(%117, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%117, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%116, read<bool>(%117));
// DEFAULT-NEXT:         let %118: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%116)
// DEFAULT-NEXT:             write<bool>(%118, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(u192b) -> d128>(%2, const<u192b>(6189354365465174593875438957438959999999999999999999999999)));
// DEFAULT-NEXT:             call<d128, signature=fn(u192b) -> d128>(%2, const<u192b>(6189354365465174593875438957438959999999999999999999999999));
// DEFAULT-NEXT:             write<d128>(%10, const<d128>(6189354365465174593875438957438960e+24));
// DEFAULT-NEXT:             let %119: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), const<d128>(6189354365465174593875438957438960e+24))
// DEFAULT-NEXT:                 write<bool>(%119, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%119, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%118, read<bool>(%119));
// DEFAULT-NEXT:         let %120: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%118)
// DEFAULT-NEXT:             write<bool>(%120, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(u192b) -> d128>(%2, const<u192b>(6189354365465174593875438957438958000000000000000000000000)));
// DEFAULT-NEXT:             call<d128, signature=fn(u192b) -> d128>(%2, const<u192b>(6189354365465174593875438957438958000000000000000000000000));
// DEFAULT-NEXT:             write<d128>(%10, const<d128>(6189354365465174593875438957438958e+24));
// DEFAULT-NEXT:             let %121: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), const<d128>(6189354365465174593875438957438958e+24))
// DEFAULT-NEXT:                 write<bool>(%121, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%121, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%120, read<bool>(%121));
// DEFAULT-NEXT:         let %122: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%120)
// DEFAULT-NEXT:             write<bool>(%122, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(u192b) -> d128>(%2, const<u192b>(6189354365465174593875438957438958500000000000000000000000)));
// DEFAULT-NEXT:             call<d128, signature=fn(u192b) -> d128>(%2, const<u192b>(6189354365465174593875438957438958500000000000000000000000));
// DEFAULT-NEXT:             write<d128>(%10, const<d128>(6189354365465174593875438957438958e+24));
// DEFAULT-NEXT:             let %123: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), const<d128>(6189354365465174593875438957438958e+24))
// DEFAULT-NEXT:                 write<bool>(%123, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%123, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%122, read<bool>(%123));
// DEFAULT-NEXT:         let %124: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%122)
// DEFAULT-NEXT:             write<bool>(%124, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(u192b) -> d128>(%2, const<u192b>(6189354365465174593875438957438958500000000000000000000001)));
// DEFAULT-NEXT:             call<d128, signature=fn(u192b) -> d128>(%2, const<u192b>(6189354365465174593875438957438958500000000000000000000001));
// DEFAULT-NEXT:             write<d128>(%10, const<d128>(6189354365465174593875438957438959e+24));
// DEFAULT-NEXT:             let %125: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), const<d128>(6189354365465174593875438957438959e+24))
// DEFAULT-NEXT:                 write<bool>(%125, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%125, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%124, read<bool>(%125));
// DEFAULT-NEXT:         let %126: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%124)
// DEFAULT-NEXT:             write<bool>(%126, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(u192b) -> d128>(%2, const<u192b>(6277101735386680763835789423207666000000000000000000000000)));
// DEFAULT-NEXT:             call<d128, signature=fn(u192b) -> d128>(%2, const<u192b>(6277101735386680763835789423207666000000000000000000000000));
// DEFAULT-NEXT:             write<d128>(%10, const<d128>(6277101735386680763835789423207666e+24));
// DEFAULT-NEXT:             let %127: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), const<d128>(6277101735386680763835789423207666e+24))
// DEFAULT-NEXT:                 write<bool>(%127, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%127, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%126, read<bool>(%127));
// DEFAULT-NEXT:         let %128: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%126)
// DEFAULT-NEXT:             write<bool>(%128, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(u192b) -> d128>(%2, const<u192b>(6277101735386680763835789423207666416102355444464034512895)));
// DEFAULT-NEXT:             call<d128, signature=fn(u192b) -> d128>(%2, const<u192b>(6277101735386680763835789423207666416102355444464034512895));
// DEFAULT-NEXT:             write<d128>(%10, const<d128>(6277101735386680763835789423207666e+24));
// DEFAULT-NEXT:             let %129: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), const<d128>(6277101735386680763835789423207666e+24))
// DEFAULT-NEXT:                 write<bool>(%129, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%129, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%128, read<bool>(%129));
// DEFAULT-NEXT:         if read<bool>(%128)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%15);
// DEFAULT-NEXT:         write<d128>(%9, call<d128, signature=fn(i575b) -> d128>(%4, widen<i575b, reason=arg>(const<i2b>(0))));
// DEFAULT-NEXT:         call<d128, signature=fn(i575b) -> d128>(%4, widen<i575b, reason=arg>(const<i2b>(0)));
// DEFAULT-NEXT:         write<d128>(%10, const<d128>(0.));
// DEFAULT-NEXT:         let %130: bool [synthetic];
// DEFAULT-NEXT:         if ne<d128, exceptions=observable>(read<d128>(%9), const<d128>(0.))
// DEFAULT-NEXT:             write<bool>(%130, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%130, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:         let %131: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%130)
// DEFAULT-NEXT:             write<bool>(%131, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(i575b) -> d128>(%4, widen<i575b, reason=arg>(const<i4b>(7))));
// DEFAULT-NEXT:             call<d128, signature=fn(i575b) -> d128>(%4, widen<i575b, reason=arg>(const<i4b>(7)));
// DEFAULT-NEXT:             write<d128>(%10, const<d128>(7.));
// DEFAULT-NEXT:             let %132: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), const<d128>(7.))
// DEFAULT-NEXT:                 write<bool>(%132, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%132, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%131, read<bool>(%132));
// DEFAULT-NEXT:         let %133: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%131)
// DEFAULT-NEXT:             write<bool>(%133, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(i575b) -> d128>(%4, widen<i575b, reason=arg>(neg<i7b, overflow=ub>(const<i7b>(42)))));
// DEFAULT-NEXT:             call<d128, signature=fn(i575b) -> d128>(%4, widen<i575b, reason=arg>(neg<i7b, overflow=ub>(const<i7b>(42))));
// DEFAULT-NEXT:             write<d128>(%10, neg<d128>(const<d128>(42.)));
// DEFAULT-NEXT:             let %134: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), neg<d128>(const<d128>(42.)))
// DEFAULT-NEXT:                 write<bool>(%134, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%134, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%133, read<bool>(%134));
// DEFAULT-NEXT:         let %135: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%133)
// DEFAULT-NEXT:             write<bool>(%135, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(i575b) -> d128>(%4, widen<i575b, reason=arg>(neg<i30b, overflow=ub>(const<i30b>(444444444)))));
// DEFAULT-NEXT:             call<d128, signature=fn(i575b) -> d128>(%4, widen<i575b, reason=arg>(neg<i30b, overflow=ub>(const<i30b>(444444444))));
// DEFAULT-NEXT:             write<d128>(%10, neg<d128>(const<d128>(444444444.)));
// DEFAULT-NEXT:             let %136: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), neg<d128>(const<d128>(444444444.)))
// DEFAULT-NEXT:                 write<bool>(%136, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%136, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%135, read<bool>(%136));
// DEFAULT-NEXT:         let %137: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%135)
// DEFAULT-NEXT:             write<bool>(%137, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(i575b) -> d128>(%4, widen<i575b, reason=arg>(neg<i53b, overflow=ub>(const<i53b>(3333333333333333)))));
// DEFAULT-NEXT:             call<d128, signature=fn(i575b) -> d128>(%4, widen<i575b, reason=arg>(neg<i53b, overflow=ub>(const<i53b>(3333333333333333))));
// DEFAULT-NEXT:             write<d128>(%10, neg<d128>(const<d128>(3333333333333333.)));
// DEFAULT-NEXT:             let %138: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), neg<d128>(const<d128>(3333333333333333.)))
// DEFAULT-NEXT:                 write<bool>(%138, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%138, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%137, read<bool>(%138));
// DEFAULT-NEXT:         let %139: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%137)
// DEFAULT-NEXT:             write<bool>(%139, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(i575b) -> d128>(%4, widen<i575b, reason=arg>(const<i98b>(99999999999999999999999999000))));
// DEFAULT-NEXT:             call<d128, signature=fn(i575b) -> d128>(%4, widen<i575b, reason=arg>(const<i98b>(99999999999999999999999999000)));
// DEFAULT-NEXT:             write<d128>(%10, const<d128>(99999999999999999999999999000.));
// DEFAULT-NEXT:             let %140: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), const<d128>(99999999999999999999999999000.))
// DEFAULT-NEXT:                 write<bool>(%140, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%140, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%139, read<bool>(%140));
// DEFAULT-NEXT:         let %141: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%139)
// DEFAULT-NEXT:             write<bool>(%141, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(i575b) -> d128>(%4, widen<i575b, reason=arg>(neg<i114b, overflow=ub>(const<i114b>(9999999999999999999999999999999999)))));
// DEFAULT-NEXT:             call<d128, signature=fn(i575b) -> d128>(%4, widen<i575b, reason=arg>(neg<i114b, overflow=ub>(const<i114b>(9999999999999999999999999999999999))));
// DEFAULT-NEXT:             write<d128>(%10, neg<d128>(const<d128>(9999999999999999999999999999999999.)));
// DEFAULT-NEXT:             let %142: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), neg<d128>(const<d128>(9999999999999999999999999999999999.)))
// DEFAULT-NEXT:                 write<bool>(%142, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%142, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%141, read<bool>(%142));
// DEFAULT-NEXT:         let %143: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%141)
// DEFAULT-NEXT:             write<bool>(%143, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(i575b) -> d128>(%4, widen<i575b, reason=arg>(const<i121b>(999999999999999999999999999999999900))));
// DEFAULT-NEXT:             call<d128, signature=fn(i575b) -> d128>(%4, widen<i575b, reason=arg>(const<i121b>(999999999999999999999999999999999900)));
// DEFAULT-NEXT:             write<d128>(%10, const<d128>(9999999999999999999999999999999999.e+2));
// DEFAULT-NEXT:             let %144: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), const<d128>(9999999999999999999999999999999999.e+2))
// DEFAULT-NEXT:                 write<bool>(%144, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%144, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%143, read<bool>(%144));
// DEFAULT-NEXT:         let %145: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%143)
// DEFAULT-NEXT:             write<bool>(%145, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(i575b) -> d128>(%4, widen<i575b, reason=arg>(neg<i124b, overflow=ub>(const<i124b>(9999999999999999999999999999999999000)))));
// DEFAULT-NEXT:             call<d128, signature=fn(i575b) -> d128>(%4, widen<i575b, reason=arg>(neg<i124b, overflow=ub>(const<i124b>(9999999999999999999999999999999999000))));
// DEFAULT-NEXT:             write<d128>(%10, neg<d128>(const<d128>(9999999999999999999999999999999999.e+3)));
// DEFAULT-NEXT:             let %146: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), neg<d128>(const<d128>(9999999999999999999999999999999999.e+3)))
// DEFAULT-NEXT:                 write<bool>(%146, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%146, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%145, read<bool>(%146));
// DEFAULT-NEXT:         let %147: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%145)
// DEFAULT-NEXT:             write<bool>(%147, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(i575b) -> d128>(%4, widen<i575b, reason=arg>(const<i131b>(999999999999999999999999999999999900000))));
// DEFAULT-NEXT:             call<d128, signature=fn(i575b) -> d128>(%4, widen<i575b, reason=arg>(const<i131b>(999999999999999999999999999999999900000)));
// DEFAULT-NEXT:             write<d128>(%10, const<d128>(9999999999999999999999999999999999.e+5));
// DEFAULT-NEXT:             let %148: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), const<d128>(9999999999999999999999999999999999.e+5))
// DEFAULT-NEXT:                 write<bool>(%148, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%148, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%147, read<bool>(%148));
// DEFAULT-NEXT:         let %149: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%147)
// DEFAULT-NEXT:             write<bool>(%149, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(i575b) -> d128>(%4, widen<i575b, reason=arg>(neg<i138b, overflow=ub>(const<i138b>(99999999999999999999999999999999990000000)))));
// DEFAULT-NEXT:             call<d128, signature=fn(i575b) -> d128>(%4, widen<i575b, reason=arg>(neg<i138b, overflow=ub>(const<i138b>(99999999999999999999999999999999990000000))));
// DEFAULT-NEXT:             write<d128>(%10, neg<d128>(const<d128>(9999999999999999999999999999999999.e+7)));
// DEFAULT-NEXT:             let %150: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), neg<d128>(const<d128>(9999999999999999999999999999999999.e+7)))
// DEFAULT-NEXT:                 write<bool>(%150, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%150, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%149, read<bool>(%150));
// DEFAULT-NEXT:         let %151: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%149)
// DEFAULT-NEXT:             write<bool>(%151, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(i575b) -> d128>(%4, widen<i575b, reason=arg>(const<i82b>(1234567890123456000000000))));
// DEFAULT-NEXT:             call<d128, signature=fn(i575b) -> d128>(%4, widen<i575b, reason=arg>(const<i82b>(1234567890123456000000000)));
// DEFAULT-NEXT:             write<d128>(%10, const<d128>(1234567890123456000000000.));
// DEFAULT-NEXT:             let %152: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), const<d128>(1234567890123456000000000.))
// DEFAULT-NEXT:                 write<bool>(%152, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%152, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%151, read<bool>(%152));
// DEFAULT-NEXT:         let %153: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%151)
// DEFAULT-NEXT:             write<bool>(%153, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(i575b) -> d128>(%4, widen<i575b, reason=arg>(const<i133b>(3424231985445429000000000000000000000000))));
// DEFAULT-NEXT:             call<d128, signature=fn(i575b) -> d128>(%4, widen<i575b, reason=arg>(const<i133b>(3424231985445429000000000000000000000000)));
// DEFAULT-NEXT:             write<d128>(%10, const<d128>(3424231985445429000000000000000000e+6));
// DEFAULT-NEXT:             let %154: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), const<d128>(3424231985445429000000000000000000e+6))
// DEFAULT-NEXT:                 write<bool>(%154, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%154, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%153, read<bool>(%154));
// DEFAULT-NEXT:         let %155: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%153)
// DEFAULT-NEXT:             write<bool>(%155, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(i575b) -> d128>(%4, widen<i575b, reason=arg>(const<i247b>(99999999999999999999999999999999990000000000000000000000000000000000000000))));
// DEFAULT-NEXT:             call<d128, signature=fn(i575b) -> d128>(%4, widen<i575b, reason=arg>(const<i247b>(99999999999999999999999999999999990000000000000000000000000000000000000000)));
// DEFAULT-NEXT:             write<d128>(%10, const<d128>(9999999999999999999999999999999999.e+40));
// DEFAULT-NEXT:             let %156: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), const<d128>(9999999999999999999999999999999999.e+40))
// DEFAULT-NEXT:                 write<bool>(%156, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%156, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%155, read<bool>(%156));
// DEFAULT-NEXT:         let %157: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%155)
// DEFAULT-NEXT:             write<bool>(%157, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(i575b) -> d128>(%4, widen<i575b, reason=arg>(neg<i324b, overflow=ub>(const<i324b>(9999999999999999999999999999999999000000000000000000000000000000000000000000000000000000000000000)))));
// DEFAULT-NEXT:             call<d128, signature=fn(i575b) -> d128>(%4, widen<i575b, reason=arg>(neg<i324b, overflow=ub>(const<i324b>(9999999999999999999999999999999999000000000000000000000000000000000000000000000000000000000000000))));
// DEFAULT-NEXT:             write<d128>(%10, neg<d128>(const<d128>(9999999999999999999999999999999999.e+63)));
// DEFAULT-NEXT:             let %158: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), neg<d128>(const<d128>(9999999999999999999999999999999999.e+63)))
// DEFAULT-NEXT:                 write<bool>(%158, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%158, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%157, read<bool>(%158));
// DEFAULT-NEXT:         let %159: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%157)
// DEFAULT-NEXT:             write<bool>(%159, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(i575b) -> d128>(%4, widen<i575b, reason=arg>(neg<i398b, overflow=ub>(const<i398b>(213855087769441389758947543987475900000000000000000000000000000000000000000000000000000000000000000000000000000000000000)))));
// DEFAULT-NEXT:             call<d128, signature=fn(i575b) -> d128>(%4, widen<i575b, reason=arg>(neg<i398b, overflow=ub>(const<i398b>(213855087769441389758947543987475900000000000000000000000000000000000000000000000000000000000000000000000000000000000000))));
// DEFAULT-NEXT:             write<d128>(%10, neg<d128>(const<d128>(2138550877694413897589475439874759e+86)));
// DEFAULT-NEXT:             let %160: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), neg<d128>(const<d128>(2138550877694413897589475439874759e+86)))
// DEFAULT-NEXT:                 write<bool>(%160, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%160, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%159, read<bool>(%160));
// DEFAULT-NEXT:         let %161: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%159)
// DEFAULT-NEXT:             write<bool>(%161, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(i575b) -> d128>(%4, widen<i575b, reason=arg>(neg<i398b, overflow=ub>(const<i398b>(213855087769441389758947543987475950000000000000000000000000000000000000000000000000000000000000000000000000000000000000)))));
// DEFAULT-NEXT:             call<d128, signature=fn(i575b) -> d128>(%4, widen<i575b, reason=arg>(neg<i398b, overflow=ub>(const<i398b>(213855087769441389758947543987475950000000000000000000000000000000000000000000000000000000000000000000000000000000000000))));
// DEFAULT-NEXT:             write<d128>(%10, neg<d128>(const<d128>(2138550877694413897589475439874760e+86)));
// DEFAULT-NEXT:             let %162: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), neg<d128>(const<d128>(2138550877694413897589475439874760e+86)))
// DEFAULT-NEXT:                 write<bool>(%162, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%162, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%161, read<bool>(%162));
// DEFAULT-NEXT:         let %163: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%161)
// DEFAULT-NEXT:             write<bool>(%163, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(i575b) -> d128>(%4, widen<i575b, reason=arg>(neg<i398b, overflow=ub>(const<i398b>(213855087769441389758947543987475949999999999999999999999999999999999999999999999999999999999999999999999999999999999999)))));
// DEFAULT-NEXT:             call<d128, signature=fn(i575b) -> d128>(%4, widen<i575b, reason=arg>(neg<i398b, overflow=ub>(const<i398b>(213855087769441389758947543987475949999999999999999999999999999999999999999999999999999999999999999999999999999999999999))));
// DEFAULT-NEXT:             write<d128>(%10, neg<d128>(const<d128>(2138550877694413897589475439874759e+86)));
// DEFAULT-NEXT:             let %164: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), neg<d128>(const<d128>(2138550877694413897589475439874759e+86)))
// DEFAULT-NEXT:                 write<bool>(%164, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%164, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%163, read<bool>(%164));
// DEFAULT-NEXT:         let %165: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%163)
// DEFAULT-NEXT:             write<bool>(%165, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(i575b) -> d128>(%4, widen<i575b, reason=arg>(neg<i398b, overflow=ub>(const<i398b>(213855087769441389758947543987475999999999999999999999999999999999999999999999999999999999999999999999999999999999999999)))));
// DEFAULT-NEXT:             call<d128, signature=fn(i575b) -> d128>(%4, widen<i575b, reason=arg>(neg<i398b, overflow=ub>(const<i398b>(213855087769441389758947543987475999999999999999999999999999999999999999999999999999999999999999999999999999999999999999))));
// DEFAULT-NEXT:             write<d128>(%10, neg<d128>(const<d128>(2138550877694413897589475439874760e+86)));
// DEFAULT-NEXT:             let %166: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), neg<d128>(const<d128>(2138550877694413897589475439874760e+86)))
// DEFAULT-NEXT:                 write<bool>(%166, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%166, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%165, read<bool>(%166));
// DEFAULT-NEXT:         let %167: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%165)
// DEFAULT-NEXT:             write<bool>(%167, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(i575b) -> d128>(%4, widen<i575b, reason=arg>(neg<i398b, overflow=ub>(const<i398b>(213855087769441389758947543987475800000000000000000000000000000000000000000000000000000000000000000000000000000000000000)))));
// DEFAULT-NEXT:             call<d128, signature=fn(i575b) -> d128>(%4, widen<i575b, reason=arg>(neg<i398b, overflow=ub>(const<i398b>(213855087769441389758947543987475800000000000000000000000000000000000000000000000000000000000000000000000000000000000000))));
// DEFAULT-NEXT:             write<d128>(%10, neg<d128>(const<d128>(2138550877694413897589475439874758e+86)));
// DEFAULT-NEXT:             let %168: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), neg<d128>(const<d128>(2138550877694413897589475439874758e+86)))
// DEFAULT-NEXT:                 write<bool>(%168, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%168, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%167, read<bool>(%168));
// DEFAULT-NEXT:         let %169: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%167)
// DEFAULT-NEXT:             write<bool>(%169, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(i575b) -> d128>(%4, widen<i575b, reason=arg>(neg<i398b, overflow=ub>(const<i398b>(213855087769441389758947543987475850000000000000000000000000000000000000000000000000000000000000000000000000000000000000)))));
// DEFAULT-NEXT:             call<d128, signature=fn(i575b) -> d128>(%4, widen<i575b, reason=arg>(neg<i398b, overflow=ub>(const<i398b>(213855087769441389758947543987475850000000000000000000000000000000000000000000000000000000000000000000000000000000000000))));
// DEFAULT-NEXT:             write<d128>(%10, neg<d128>(const<d128>(2138550877694413897589475439874758e+86)));
// DEFAULT-NEXT:             let %170: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), neg<d128>(const<d128>(2138550877694413897589475439874758e+86)))
// DEFAULT-NEXT:                 write<bool>(%170, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%170, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%169, read<bool>(%170));
// DEFAULT-NEXT:         let %171: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%169)
// DEFAULT-NEXT:             write<bool>(%171, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(i575b) -> d128>(%4, widen<i575b, reason=arg>(neg<i398b, overflow=ub>(const<i398b>(213855087769441389758947543987475850000000000000000000000000000000000000000000000000000000000000000000000000000000000001)))));
// DEFAULT-NEXT:             call<d128, signature=fn(i575b) -> d128>(%4, widen<i575b, reason=arg>(neg<i398b, overflow=ub>(const<i398b>(213855087769441389758947543987475850000000000000000000000000000000000000000000000000000000000000000000000000000000000001))));
// DEFAULT-NEXT:             write<d128>(%10, neg<d128>(const<d128>(2138550877694413897589475439874759e+86)));
// DEFAULT-NEXT:             let %172: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), neg<d128>(const<d128>(2138550877694413897589475439874759e+86)))
// DEFAULT-NEXT:                 write<bool>(%172, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%172, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%171, read<bool>(%172));
// DEFAULT-NEXT:         let %173: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%171)
// DEFAULT-NEXT:             write<bool>(%173, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(i575b) -> d128>(%4, const<i575b>(61832600368276133515125630254911790000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000)));
// DEFAULT-NEXT:             call<d128, signature=fn(i575b) -> d128>(%4, const<i575b>(61832600368276133515125630254911790000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000));
// DEFAULT-NEXT:             write<d128>(%10, const<d128>(6183260036827613351512563025491179e+139));
// DEFAULT-NEXT:             let %174: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), const<d128>(6183260036827613351512563025491179e+139))
// DEFAULT-NEXT:                 write<bool>(%174, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%174, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%173, read<bool>(%174));
// DEFAULT-NEXT:         let %175: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%173)
// DEFAULT-NEXT:             write<bool>(%175, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(i575b) -> d128>(%4, const<i575b>(61832600368276133515125630254911797508782837275302959978515764023224306276632966792579100265310761247399417856504034834837841258576687802491886538775473291979151693037174783)));
// DEFAULT-NEXT:             call<d128, signature=fn(i575b) -> d128>(%4, const<i575b>(61832600368276133515125630254911797508782837275302959978515764023224306276632966792579100265310761247399417856504034834837841258576687802491886538775473291979151693037174783));
// DEFAULT-NEXT:             write<d128>(%10, const<d128>(6183260036827613351512563025491180e+139));
// DEFAULT-NEXT:             let %176: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), const<d128>(6183260036827613351512563025491180e+139))
// DEFAULT-NEXT:                 write<bool>(%176, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%176, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%175, read<bool>(%176));
// DEFAULT-NEXT:         let %177: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%175)
// DEFAULT-NEXT:             write<bool>(%177, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(i575b) -> d128>(%4, neg<i575b, overflow=ub>(const<i575b>(61832600368276133515125630254911790000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000))));
// DEFAULT-NEXT:             call<d128, signature=fn(i575b) -> d128>(%4, neg<i575b, overflow=ub>(const<i575b>(61832600368276133515125630254911790000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000)));
// DEFAULT-NEXT:             write<d128>(%10, neg<d128>(const<d128>(6183260036827613351512563025491179e+139)));
// DEFAULT-NEXT:             let %178: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), neg<d128>(const<d128>(6183260036827613351512563025491179e+139)))
// DEFAULT-NEXT:                 write<bool>(%178, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%178, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%177, read<bool>(%178));
// DEFAULT-NEXT:         let %179: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%177)
// DEFAULT-NEXT:             write<bool>(%179, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(i575b) -> d128>(%4, sub<i575b, overflow=ub>(neg<i575b, overflow=ub>(const<i575b>(61832600368276133515125630254911797508782837275302959978515764023224306276632966792579100265310761247399417856504034834837841258576687802491886538775473291979151693037174783)), widen<i575b, reason=usual_arith>(const<i2b>(1)))));
// DEFAULT-NEXT:             call<d128, signature=fn(i575b) -> d128>(%4, sub<i575b, overflow=ub>(neg<i575b, overflow=ub>(const<i575b>(61832600368276133515125630254911797508782837275302959978515764023224306276632966792579100265310761247399417856504034834837841258576687802491886538775473291979151693037174783)), widen<i575b, reason=usual_arith>(const<i2b>(1))));
// DEFAULT-NEXT:             write<d128>(%10, neg<d128>(const<d128>(6183260036827613351512563025491180e+139)));
// DEFAULT-NEXT:             let %180: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), neg<d128>(const<d128>(6183260036827613351512563025491180e+139)))
// DEFAULT-NEXT:                 write<bool>(%180, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%180, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%179, read<bool>(%180));
// DEFAULT-NEXT:         if read<bool>(%179)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%15);
// DEFAULT-NEXT:         write<d128>(%9, call<d128, signature=fn(u575b) -> d128>(%6, widen<u575b, reason=arg>(const<u1b>(0))));
// DEFAULT-NEXT:         call<d128, signature=fn(u575b) -> d128>(%6, widen<u575b, reason=arg>(const<u1b>(0)));
// DEFAULT-NEXT:         write<d128>(%10, const<d128>(0.));
// DEFAULT-NEXT:         let %181: bool [synthetic];
// DEFAULT-NEXT:         if ne<d128, exceptions=observable>(read<d128>(%9), const<d128>(0.))
// DEFAULT-NEXT:             write<bool>(%181, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%181, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:         let %182: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%181)
// DEFAULT-NEXT:             write<bool>(%182, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(u575b) -> d128>(%6, widen<u575b, reason=arg>(const<u5b>(17))));
// DEFAULT-NEXT:             call<d128, signature=fn(u575b) -> d128>(%6, widen<u575b, reason=arg>(const<u5b>(17)));
// DEFAULT-NEXT:             write<d128>(%10, const<d128>(17.));
// DEFAULT-NEXT:             let %183: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), const<d128>(17.))
// DEFAULT-NEXT:                 write<bool>(%183, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%183, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%182, read<bool>(%183));
// DEFAULT-NEXT:         let %184: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%182)
// DEFAULT-NEXT:             write<bool>(%184, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(u575b) -> d128>(%6, widen<u575b, reason=arg>(const<u9b>(420))));
// DEFAULT-NEXT:             call<d128, signature=fn(u575b) -> d128>(%6, widen<u575b, reason=arg>(const<u9b>(420)));
// DEFAULT-NEXT:             write<d128>(%10, const<d128>(420.));
// DEFAULT-NEXT:             let %185: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), const<d128>(420.))
// DEFAULT-NEXT:                 write<bool>(%185, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%185, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%184, read<bool>(%185));
// DEFAULT-NEXT:         let %186: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%184)
// DEFAULT-NEXT:             write<bool>(%186, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(u575b) -> d128>(%6, widen<u575b, reason=arg>(const<u30b>(888888888))));
// DEFAULT-NEXT:             call<d128, signature=fn(u575b) -> d128>(%6, widen<u575b, reason=arg>(const<u30b>(888888888)));
// DEFAULT-NEXT:             write<d128>(%10, const<d128>(888888888.));
// DEFAULT-NEXT:             let %187: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), const<d128>(888888888.))
// DEFAULT-NEXT:                 write<bool>(%187, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%187, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%186, read<bool>(%187));
// DEFAULT-NEXT:         let %188: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%186)
// DEFAULT-NEXT:             write<bool>(%188, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(u575b) -> d128>(%6, widen<u575b, reason=arg>(const<u54b>(9999999999999000))));
// DEFAULT-NEXT:             call<d128, signature=fn(u575b) -> d128>(%6, widen<u575b, reason=arg>(const<u54b>(9999999999999000)));
// DEFAULT-NEXT:             write<d128>(%10, const<d128>(9999999999999000.));
// DEFAULT-NEXT:             let %189: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), const<d128>(9999999999999000.))
// DEFAULT-NEXT:                 write<bool>(%189, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%189, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%188, read<bool>(%189));
// DEFAULT-NEXT:         let %190: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%188)
// DEFAULT-NEXT:             write<bool>(%190, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(u575b) -> d128>(%6, widen<u575b, reason=arg>(const<u137b>(99999999999999999999999999999999990000000))));
// DEFAULT-NEXT:             call<d128, signature=fn(u575b) -> d128>(%6, widen<u575b, reason=arg>(const<u137b>(99999999999999999999999999999999990000000)));
// DEFAULT-NEXT:             write<d128>(%10, const<d128>(9999999999999999999999999999999999.e+7));
// DEFAULT-NEXT:             let %191: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), const<d128>(9999999999999999999999999999999999.e+7))
// DEFAULT-NEXT:                 write<bool>(%191, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%191, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%190, read<bool>(%191));
// DEFAULT-NEXT:         let %192: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%190)
// DEFAULT-NEXT:             write<bool>(%192, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(u575b) -> d128>(%6, widen<u575b, reason=arg>(const<u143b>(9999999999999999999999999999999999000000000))));
// DEFAULT-NEXT:             call<d128, signature=fn(u575b) -> d128>(%6, widen<u575b, reason=arg>(const<u143b>(9999999999999999999999999999999999000000000)));
// DEFAULT-NEXT:             write<d128>(%10, const<d128>(9999999999999999999999999999999999.e+9));
// DEFAULT-NEXT:             let %193: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), const<d128>(9999999999999999999999999999999999.e+9))
// DEFAULT-NEXT:                 write<bool>(%193, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%193, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%192, read<bool>(%193));
// DEFAULT-NEXT:         let %194: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%192)
// DEFAULT-NEXT:             write<bool>(%194, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(u575b) -> d128>(%6, widen<u575b, reason=arg>(const<u157b>(99999999999999999999999999999999990000000000000))));
// DEFAULT-NEXT:             call<d128, signature=fn(u575b) -> d128>(%6, widen<u575b, reason=arg>(const<u157b>(99999999999999999999999999999999990000000000000)));
// DEFAULT-NEXT:             write<d128>(%10, const<d128>(9999999999999999999999999999999999.e+13));
// DEFAULT-NEXT:             let %195: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), const<d128>(9999999999999999999999999999999999.e+13))
// DEFAULT-NEXT:                 write<bool>(%195, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%195, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%194, read<bool>(%195));
// DEFAULT-NEXT:         let %196: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%194)
// DEFAULT-NEXT:             write<bool>(%196, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(u575b) -> d128>(%6, widen<u575b, reason=arg>(const<u163b>(9999999999999999999999999999999999000000000000000))));
// DEFAULT-NEXT:             call<d128, signature=fn(u575b) -> d128>(%6, widen<u575b, reason=arg>(const<u163b>(9999999999999999999999999999999999000000000000000)));
// DEFAULT-NEXT:             write<d128>(%10, const<d128>(9999999999999999999999999999999999.e+15));
// DEFAULT-NEXT:             let %197: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), const<d128>(9999999999999999999999999999999999.e+15))
// DEFAULT-NEXT:                 write<bool>(%197, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%197, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%196, read<bool>(%197));
// DEFAULT-NEXT:         let %198: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%196)
// DEFAULT-NEXT:             write<bool>(%198, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(u575b) -> d128>(%6, widen<u575b, reason=arg>(const<u110b>(1234567890123456000000000000000000))));
// DEFAULT-NEXT:             call<d128, signature=fn(u575b) -> d128>(%6, widen<u575b, reason=arg>(const<u110b>(1234567890123456000000000000000000)));
// DEFAULT-NEXT:             write<d128>(%10, const<d128>(1234567890123456000000000000000000.));
// DEFAULT-NEXT:             let %199: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), const<d128>(1234567890123456000000000000000000.))
// DEFAULT-NEXT:                 write<bool>(%199, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%199, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%198, read<bool>(%199));
// DEFAULT-NEXT:         let %200: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%198)
// DEFAULT-NEXT:             write<bool>(%200, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(u575b) -> d128>(%6, widen<u575b, reason=arg>(const<u125b>(34242319854454290000000000000000000000))));
// DEFAULT-NEXT:             call<d128, signature=fn(u575b) -> d128>(%6, widen<u575b, reason=arg>(const<u125b>(34242319854454290000000000000000000000)));
// DEFAULT-NEXT:             write<d128>(%10, const<d128>(3424231985445429000000000000000000e+4));
// DEFAULT-NEXT:             let %201: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), const<d128>(3424231985445429000000000000000000e+4))
// DEFAULT-NEXT:                 write<bool>(%201, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%201, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%200, read<bool>(%201));
// DEFAULT-NEXT:         let %202: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%200)
// DEFAULT-NEXT:             write<bool>(%202, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(u575b) -> d128>(%6, widen<u575b, reason=arg>(const<u223b>(9999999999999999999999999999999999000000000000000000000000000000000))));
// DEFAULT-NEXT:             call<d128, signature=fn(u575b) -> d128>(%6, widen<u575b, reason=arg>(const<u223b>(9999999999999999999999999999999999000000000000000000000000000000000)));
// DEFAULT-NEXT:             write<d128>(%10, const<d128>(9999999999999999999999999999999999.e+33));
// DEFAULT-NEXT:             let %203: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), const<d128>(9999999999999999999999999999999999.e+33))
// DEFAULT-NEXT:                 write<bool>(%203, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%203, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%202, read<bool>(%203));
// DEFAULT-NEXT:         let %204: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%202)
// DEFAULT-NEXT:             write<bool>(%204, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(u575b) -> d128>(%6, widen<u575b, reason=arg>(const<u458b>(618935436546517949837539847534981700000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000))));
// DEFAULT-NEXT:             call<d128, signature=fn(u575b) -> d128>(%6, widen<u575b, reason=arg>(const<u458b>(618935436546517949837539847534981700000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000)));
// DEFAULT-NEXT:             write<d128>(%10, const<d128>(6189354365465179498375398475349817e+104));
// DEFAULT-NEXT:             let %205: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), const<d128>(6189354365465179498375398475349817e+104))
// DEFAULT-NEXT:                 write<bool>(%205, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%205, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%204, read<bool>(%205));
// DEFAULT-NEXT:         let %206: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%204)
// DEFAULT-NEXT:             write<bool>(%206, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(u575b) -> d128>(%6, widen<u575b, reason=arg>(const<u458b>(618935436546517949837539847534981750000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000))));
// DEFAULT-NEXT:             call<d128, signature=fn(u575b) -> d128>(%6, widen<u575b, reason=arg>(const<u458b>(618935436546517949837539847534981750000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000)));
// DEFAULT-NEXT:             write<d128>(%10, const<d128>(6189354365465179498375398475349818e+104));
// DEFAULT-NEXT:             let %207: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), const<d128>(6189354365465179498375398475349818e+104))
// DEFAULT-NEXT:                 write<bool>(%207, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%207, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%206, read<bool>(%207));
// DEFAULT-NEXT:         let %208: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%206)
// DEFAULT-NEXT:             write<bool>(%208, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(u575b) -> d128>(%6, widen<u575b, reason=arg>(const<u458b>(618935436546517949837539847534981749999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999))));
// DEFAULT-NEXT:             call<d128, signature=fn(u575b) -> d128>(%6, widen<u575b, reason=arg>(const<u458b>(618935436546517949837539847534981749999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999)));
// DEFAULT-NEXT:             write<d128>(%10, const<d128>(6189354365465179498375398475349817e+104));
// DEFAULT-NEXT:             let %209: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), const<d128>(6189354365465179498375398475349817e+104))
// DEFAULT-NEXT:                 write<bool>(%209, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%209, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%208, read<bool>(%209));
// DEFAULT-NEXT:         let %210: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%208)
// DEFAULT-NEXT:             write<bool>(%210, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(u575b) -> d128>(%6, widen<u575b, reason=arg>(const<u458b>(618935436546517949837539847534981799999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999))));
// DEFAULT-NEXT:             call<d128, signature=fn(u575b) -> d128>(%6, widen<u575b, reason=arg>(const<u458b>(618935436546517949837539847534981799999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999)));
// DEFAULT-NEXT:             write<d128>(%10, const<d128>(6189354365465179498375398475349818e+104));
// DEFAULT-NEXT:             let %211: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), const<d128>(6189354365465179498375398475349818e+104))
// DEFAULT-NEXT:                 write<bool>(%211, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%211, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%210, read<bool>(%211));
// DEFAULT-NEXT:         let %212: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%210)
// DEFAULT-NEXT:             write<bool>(%212, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(u575b) -> d128>(%6, widen<u575b, reason=arg>(const<u458b>(618935436546517949837539847534981800000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000))));
// DEFAULT-NEXT:             call<d128, signature=fn(u575b) -> d128>(%6, widen<u575b, reason=arg>(const<u458b>(618935436546517949837539847534981800000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000)));
// DEFAULT-NEXT:             write<d128>(%10, const<d128>(6189354365465179498375398475349818e+104));
// DEFAULT-NEXT:             let %213: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), const<d128>(6189354365465179498375398475349818e+104))
// DEFAULT-NEXT:                 write<bool>(%213, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%213, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%212, read<bool>(%213));
// DEFAULT-NEXT:         let %214: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%212)
// DEFAULT-NEXT:             write<bool>(%214, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(u575b) -> d128>(%6, widen<u575b, reason=arg>(const<u458b>(618935436546517949837539847534981850000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000))));
// DEFAULT-NEXT:             call<d128, signature=fn(u575b) -> d128>(%6, widen<u575b, reason=arg>(const<u458b>(618935436546517949837539847534981850000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000)));
// DEFAULT-NEXT:             write<d128>(%10, const<d128>(6189354365465179498375398475349818e+104));
// DEFAULT-NEXT:             let %215: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), const<d128>(6189354365465179498375398475349818e+104))
// DEFAULT-NEXT:                 write<bool>(%215, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%215, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%214, read<bool>(%215));
// DEFAULT-NEXT:         let %216: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%214)
// DEFAULT-NEXT:             write<bool>(%216, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(u575b) -> d128>(%6, widen<u575b, reason=arg>(const<u458b>(618935436546517949837539847534981850000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001))));
// DEFAULT-NEXT:             call<d128, signature=fn(u575b) -> d128>(%6, widen<u575b, reason=arg>(const<u458b>(618935436546517949837539847534981850000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001)));
// DEFAULT-NEXT:             write<d128>(%10, const<d128>(6189354365465179498375398475349819e+104));
// DEFAULT-NEXT:             let %217: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), const<d128>(6189354365465179498375398475349819e+104))
// DEFAULT-NEXT:                 write<bool>(%217, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%217, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%216, read<bool>(%217));
// DEFAULT-NEXT:         let %218: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%216)
// DEFAULT-NEXT:             write<bool>(%218, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(u575b) -> d128>(%6, const<u575b>(99999999999999999999999999999999990000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000)));
// DEFAULT-NEXT:             call<d128, signature=fn(u575b) -> d128>(%6, const<u575b>(99999999999999999999999999999999990000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000));
// DEFAULT-NEXT:             write<d128>(%10, const<d128>(9999999999999999999999999999999999.e+139));
// DEFAULT-NEXT:             let %219: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), const<d128>(9999999999999999999999999999999999.e+139))
// DEFAULT-NEXT:                 write<bool>(%219, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%219, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%218, read<bool>(%219));
// DEFAULT-NEXT:         let %220: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%218)
// DEFAULT-NEXT:             write<bool>(%220, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(u575b) -> d128>(%6, const<u575b>(123665200736552267030251260509823500000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000)));
// DEFAULT-NEXT:             call<d128, signature=fn(u575b) -> d128>(%6, const<u575b>(123665200736552267030251260509823500000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000));
// DEFAULT-NEXT:             write<d128>(%10, const<d128>(1236652007365522670302512605098235e+140));
// DEFAULT-NEXT:             let %221: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), const<d128>(1236652007365522670302512605098235e+140))
// DEFAULT-NEXT:                 write<bool>(%221, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%221, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%220, read<bool>(%221));
// DEFAULT-NEXT:         let %222: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%220)
// DEFAULT-NEXT:             write<bool>(%222, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(u575b) -> d128>(%6, const<u575b>(123665200736552267030251260509823549999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999)));
// DEFAULT-NEXT:             call<d128, signature=fn(u575b) -> d128>(%6, const<u575b>(123665200736552267030251260509823549999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999));
// DEFAULT-NEXT:             write<d128>(%10, const<d128>(1236652007365522670302512605098235e+140));
// DEFAULT-NEXT:             let %223: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), const<d128>(1236652007365522670302512605098235e+140))
// DEFAULT-NEXT:                 write<bool>(%223, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%223, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%222, read<bool>(%223));
// DEFAULT-NEXT:         let %224: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%222)
// DEFAULT-NEXT:             write<bool>(%224, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(u575b) -> d128>(%6, const<u575b>(123665200736552267030251260509823550000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000)));
// DEFAULT-NEXT:             call<d128, signature=fn(u575b) -> d128>(%6, const<u575b>(123665200736552267030251260509823550000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000));
// DEFAULT-NEXT:             write<d128>(%10, const<d128>(1236652007365522670302512605098236e+140));
// DEFAULT-NEXT:             let %225: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), const<d128>(1236652007365522670302512605098236e+140))
// DEFAULT-NEXT:                 write<bool>(%225, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%225, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%224, read<bool>(%225));
// DEFAULT-NEXT:         let %226: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%224)
// DEFAULT-NEXT:             write<bool>(%226, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(u575b) -> d128>(%6, const<u575b>(123665200736552267030251260509823550000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001)));
// DEFAULT-NEXT:             call<d128, signature=fn(u575b) -> d128>(%6, const<u575b>(123665200736552267030251260509823550000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001));
// DEFAULT-NEXT:             write<d128>(%10, const<d128>(1236652007365522670302512605098236e+140));
// DEFAULT-NEXT:             let %227: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), const<d128>(1236652007365522670302512605098236e+140))
// DEFAULT-NEXT:                 write<bool>(%227, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%227, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%226, read<bool>(%227));
// DEFAULT-NEXT:         let %228: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%226)
// DEFAULT-NEXT:             write<bool>(%228, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(u575b) -> d128>(%6, const<u575b>(123665200736552267030251260509823550000000000000000000000000000000000000001000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000)));
// DEFAULT-NEXT:             call<d128, signature=fn(u575b) -> d128>(%6, const<u575b>(123665200736552267030251260509823550000000000000000000000000000000000000001000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000));
// DEFAULT-NEXT:             write<d128>(%10, const<d128>(1236652007365522670302512605098236e+140));
// DEFAULT-NEXT:             let %229: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), const<d128>(1236652007365522670302512605098236e+140))
// DEFAULT-NEXT:                 write<bool>(%229, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%229, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%228, read<bool>(%229));
// DEFAULT-NEXT:         let %230: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%228)
// DEFAULT-NEXT:             write<bool>(%230, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%9, call<d128, signature=fn(u575b) -> d128>(%6, const<u575b>(123665200736552267030251260509823595017565674550605919957031528046448612553265933585158200530621522494798835713008069669675682517153375604983773077550946583958303386074349567)));
// DEFAULT-NEXT:             call<d128, signature=fn(u575b) -> d128>(%6, const<u575b>(123665200736552267030251260509823595017565674550605919957031528046448612553265933585158200530621522494798835713008069669675682517153375604983773077550946583958303386074349567));
// DEFAULT-NEXT:             write<d128>(%10, const<d128>(1236652007365522670302512605098236e+140));
// DEFAULT-NEXT:             let %231: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%9), const<d128>(1236652007365522670302512605098236e+140))
// DEFAULT-NEXT:                 write<bool>(%231, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%231, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%14, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%9)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%10)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%230, read<bool>(%231));
// DEFAULT-NEXT:         if read<bool>(%230)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%15);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
