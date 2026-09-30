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
// DEFAULT-NEXT:     fn %[[VALUE_tests192:[0-9]+]] @tests192(%[[VALUE_b:[0-9]+]] b: i192b) -> d128 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return int_to_float<d128, reason=return, exact=false, rounding=nearest_even, exceptions=observable>(read<i192b>(%[[VALUE_b]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testu192:[0-9]+]] @testu192(%[[VALUE_b_2:[0-9]+]] b: u192b) -> d128 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return int_to_float<d128, reason=return, exact=false, rounding=nearest_even, exceptions=observable>(read<u192b>(%[[VALUE_b_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_tests575:[0-9]+]] @tests575(%[[VALUE_b_3:[0-9]+]] b: i575b) -> d128 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return int_to_float<d128, reason=return, exact=false, rounding=nearest_even, exceptions=observable>(read<i575b>(%[[VALUE_b_3]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testu575:[0-9]+]] @testu575(%[[VALUE_b_4:[0-9]+]] b: u575b) -> d128 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return int_to_float<d128, reason=return, exact=false, rounding=nearest_even, exceptions=observable>(read<u575b>(%[[VALUE_b_4]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_memcmp:[0-9]+]] @__builtin_memcmp(%[[VALUE0:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE1:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE2:[0-9]+]] <unnamed>: u64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: d128 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_b_5:[0-9]+]] b: d128 [storage=automatic];
// DEFAULT-NEXT:         write<d128>(%[[VALUE_a]], call<d128, signature=fn(i192b) -> d128>(%[[VALUE_tests192]], widen<i192b, reason=arg>(const<i2b>(0))));
// DEFAULT-NEXT:         write<d128>(%[[VALUE_b_5]], const<d128>(0.));
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), const<d128>(0.))
// DEFAULT-NEXT:             write<bool>(%[[VALUE3]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE3]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE3]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE4]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(i192b) -> d128>(%[[VALUE_tests192]], widen<i192b, reason=arg>(const<i4b>(7))));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], const<d128>(7.));
// DEFAULT-NEXT:             let %[[VALUE5:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), const<d128>(7.))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE5]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE5]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE4]], read<bool>(%[[VALUE5]]));
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE4]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE6]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(i192b) -> d128>(%[[VALUE_tests192]], widen<i192b, reason=arg>(neg<i7b, overflow=ub>(const<i7b>(42)))));
// DEFAULT-NEXT:             let %[[VALUE7:[0-9]+]]: d128 [synthetic] = neg<d128>(const<d128>(42.));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], read<d128>(%[[VALUE7]]));
// DEFAULT-NEXT:             let %[[VALUE8:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), neg<d128>(const<d128>(42.)))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE8]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE8]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE6]], read<bool>(%[[VALUE8]]));
// DEFAULT-NEXT:         let %[[VALUE9:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE6]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE9]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(i192b) -> d128>(%[[VALUE_tests192]], widen<i192b, reason=arg>(neg<i31b, overflow=ub>(const<i31b>(777777777)))));
// DEFAULT-NEXT:             let %[[VALUE10:[0-9]+]]: d128 [synthetic] = neg<d128>(const<d128>(777777777.));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], read<d128>(%[[VALUE10]]));
// DEFAULT-NEXT:             let %[[VALUE11:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), neg<d128>(const<d128>(777777777.)))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE11]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE11]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE9]], read<bool>(%[[VALUE11]]));
// DEFAULT-NEXT:         let %[[VALUE12:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE9]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE12]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(i192b) -> d128>(%[[VALUE_tests192]], widen<i192b, reason=arg>(neg<i45b, overflow=ub>(const<i45b>(12345678912345)))));
// DEFAULT-NEXT:             let %[[VALUE13:[0-9]+]]: d128 [synthetic] = neg<d128>(const<d128>(12345678912345.));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], read<d128>(%[[VALUE13]]));
// DEFAULT-NEXT:             let %[[VALUE14:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), neg<d128>(const<d128>(12345678912345.)))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE14]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE14]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE12]], read<bool>(%[[VALUE14]]));
// DEFAULT-NEXT:         let %[[VALUE15:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE12]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE15]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(i192b) -> d128>(%[[VALUE_tests192]], widen<i192b, reason=arg>(const<i58b>(123456789123456789))));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], const<d128>(123456789123456789.));
// DEFAULT-NEXT:             let %[[VALUE16:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), const<d128>(123456789123456789.))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE16]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE16]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE15]], read<bool>(%[[VALUE16]]));
// DEFAULT-NEXT:         let %[[VALUE17:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE15]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE17]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(i192b) -> d128>(%[[VALUE_tests192]], widen<i192b, reason=arg>(const<i91b>(777777777777777777777777777))));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], const<d128>(777777777777777777777777777.));
// DEFAULT-NEXT:             let %[[VALUE18:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), const<d128>(777777777777777777777777777.))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE18]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE18]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE17]], read<bool>(%[[VALUE18]]));
// DEFAULT-NEXT:         let %[[VALUE19:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE17]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE19]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(i192b) -> d128>(%[[VALUE_tests192]], widen<i192b, reason=arg>(const<i114b>(9999999999999999999999999900000000))));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], const<d128>(9999999999999999999999999900000000.));
// DEFAULT-NEXT:             let %[[VALUE20:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), const<d128>(9999999999999999999999999900000000.))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE20]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE20]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE19]], read<bool>(%[[VALUE20]]));
// DEFAULT-NEXT:         let %[[VALUE21:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE19]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE21]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(i192b) -> d128>(%[[VALUE_tests192]], widen<i192b, reason=arg>(neg<i114b, overflow=ub>(const<i114b>(9999999999999999999999999999999999)))));
// DEFAULT-NEXT:             let %[[VALUE22:[0-9]+]]: d128 [synthetic] = neg<d128>(const<d128>(9999999999999999999999999999999999.));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], read<d128>(%[[VALUE22]]));
// DEFAULT-NEXT:             let %[[VALUE23:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), neg<d128>(const<d128>(9999999999999999999999999999999999.)))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE23]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE23]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE21]], read<bool>(%[[VALUE23]]));
// DEFAULT-NEXT:         let %[[VALUE24:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE21]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE24]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(i192b) -> d128>(%[[VALUE_tests192]], widen<i192b, reason=arg>(neg<i118b, overflow=ub>(const<i118b>(99999999999999999999999999999999994)))));
// DEFAULT-NEXT:             let %[[VALUE25:[0-9]+]]: d128 [synthetic] = neg<d128>(const<d128>(9999999999999999999999999999999999.e+1));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], read<d128>(%[[VALUE25]]));
// DEFAULT-NEXT:             let %[[VALUE26:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), neg<d128>(const<d128>(9999999999999999999999999999999999.e+1)))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE26]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE26]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE24]], read<bool>(%[[VALUE26]]));
// DEFAULT-NEXT:         let %[[VALUE27:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE24]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE27]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(i192b) -> d128>(%[[VALUE_tests192]], widen<i192b, reason=arg>(const<i118b>(99999999999999999999999999999999995))));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], const<d128>(1000000000000000000000000000000000.e+2));
// DEFAULT-NEXT:             let %[[VALUE28:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), const<d128>(1000000000000000000000000000000000.e+2))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE28]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE28]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE27]], read<bool>(%[[VALUE28]]));
// DEFAULT-NEXT:         let %[[VALUE29:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE27]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE29]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(i192b) -> d128>(%[[VALUE_tests192]], widen<i192b, reason=arg>(const<i121b>(999999999999999999999999999999999900))));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], const<d128>(9999999999999999999999999999999999.e+2));
// DEFAULT-NEXT:             let %[[VALUE30:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), const<d128>(9999999999999999999999999999999999.e+2))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE30]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE30]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE29]], read<bool>(%[[VALUE30]]));
// DEFAULT-NEXT:         let %[[VALUE31:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE29]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE31]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(i192b) -> d128>(%[[VALUE_tests192]], widen<i192b, reason=arg>(const<i121b>(999999999999999999999999999999999949))));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], const<d128>(9999999999999999999999999999999999.e+2));
// DEFAULT-NEXT:             let %[[VALUE32:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), const<d128>(9999999999999999999999999999999999.e+2))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE32]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE32]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE31]], read<bool>(%[[VALUE32]]));
// DEFAULT-NEXT:         let %[[VALUE33:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE31]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE33]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(i192b) -> d128>(%[[VALUE_tests192]], widen<i192b, reason=arg>(neg<i124b, overflow=ub>(const<i124b>(9999999999999999999999999999999999000)))));
// DEFAULT-NEXT:             let %[[VALUE34:[0-9]+]]: d128 [synthetic] = neg<d128>(const<d128>(9999999999999999999999999999999999.e+3));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], read<d128>(%[[VALUE34]]));
// DEFAULT-NEXT:             let %[[VALUE35:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), neg<d128>(const<d128>(9999999999999999999999999999999999.e+3)))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE35]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE35]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE33]], read<bool>(%[[VALUE35]]));
// DEFAULT-NEXT:         let %[[VALUE36:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE33]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE36]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(i192b) -> d128>(%[[VALUE_tests192]], widen<i192b, reason=arg>(const<i124b>(9999999999999999999999999999999999499))));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], const<d128>(9999999999999999999999999999999999.e+3));
// DEFAULT-NEXT:             let %[[VALUE37:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), const<d128>(9999999999999999999999999999999999.e+3))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE37]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE37]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE36]], read<bool>(%[[VALUE37]]));
// DEFAULT-NEXT:         let %[[VALUE38:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE36]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE38]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(i192b) -> d128>(%[[VALUE_tests192]], widen<i192b, reason=arg>(const<i126b>(34242319854454290000000000000000000000))));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], const<d128>(3424231985445429000000000000000000e+4));
// DEFAULT-NEXT:             let %[[VALUE39:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), const<d128>(3424231985445429000000000000000000e+4))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE39]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE39]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE38]], read<bool>(%[[VALUE39]]));
// DEFAULT-NEXT:         let %[[VALUE40:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE38]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE40]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(i192b) -> d128>(%[[VALUE_tests192]], widen<i192b, reason=arg>(const<i126b>(34242319854454294983573424983275760000))));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], const<d128>(3424231985445429498357342498327576e+4));
// DEFAULT-NEXT:             let %[[VALUE41:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), const<d128>(3424231985445429498357342498327576e+4))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE41]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE41]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE40]], read<bool>(%[[VALUE41]]));
// DEFAULT-NEXT:         let %[[VALUE42:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE40]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE42]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(i192b) -> d128>(%[[VALUE_tests192]], widen<i192b, reason=arg>(const<i131b>(999999999999999999999999999999999900000))));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], const<d128>(9999999999999999999999999999999999.e+5));
// DEFAULT-NEXT:             let %[[VALUE43:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), const<d128>(9999999999999999999999999999999999.e+5))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE43]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE43]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE42]], read<bool>(%[[VALUE43]]));
// DEFAULT-NEXT:         let %[[VALUE44:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE42]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE44]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(i192b) -> d128>(%[[VALUE_tests192]], widen<i192b, reason=arg>(const<i131b>(999999999999999999999999999999999949999))));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], const<d128>(9999999999999999999999999999999999.e+5));
// DEFAULT-NEXT:             let %[[VALUE45:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), const<d128>(9999999999999999999999999999999999.e+5))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE45]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE45]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE44]], read<bool>(%[[VALUE45]]));
// DEFAULT-NEXT:         let %[[VALUE46:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE44]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE46]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(i192b) -> d128>(%[[VALUE_tests192]], widen<i192b, reason=arg>(neg<i134b, overflow=ub>(const<i134b>(9999999999999999999999999999999999000000)))));
// DEFAULT-NEXT:             let %[[VALUE47:[0-9]+]]: d128 [synthetic] = neg<d128>(const<d128>(9999999999999999999999999999999999.e+6));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], read<d128>(%[[VALUE47]]));
// DEFAULT-NEXT:             let %[[VALUE48:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), neg<d128>(const<d128>(9999999999999999999999999999999999.e+6)))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE48]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE48]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE46]], read<bool>(%[[VALUE48]]));
// DEFAULT-NEXT:         let %[[VALUE49:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE46]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE49]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(i192b) -> d128>(%[[VALUE_tests192]], widen<i192b, reason=arg>(neg<i134b, overflow=ub>(const<i134b>(9999999999999999999999999999999999499999)))));
// DEFAULT-NEXT:             let %[[VALUE50:[0-9]+]]: d128 [synthetic] = neg<d128>(const<d128>(9999999999999999999999999999999999.e+6));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], read<d128>(%[[VALUE50]]));
// DEFAULT-NEXT:             let %[[VALUE51:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), neg<d128>(const<d128>(9999999999999999999999999999999999.e+6)))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE51]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE51]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE49]], read<bool>(%[[VALUE51]]));
// DEFAULT-NEXT:         let %[[VALUE52:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE49]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE52]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(i192b) -> d128>(%[[VALUE_tests192]], widen<i192b, reason=arg>(const<i138b>(123456789012345678901234567890123400000000))));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], const<d128>(1234567890123456789012345678901234.e+8));
// DEFAULT-NEXT:             let %[[VALUE53:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), const<d128>(1234567890123456789012345678901234.e+8))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE53]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE53]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE52]], read<bool>(%[[VALUE53]]));
// DEFAULT-NEXT:         let %[[VALUE54:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE52]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE54]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(i192b) -> d128>(%[[VALUE_tests192]], widen<i192b, reason=arg>(const<i191b>(999999999999999999999999999999999900000000000000000000000))));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], const<d128>(9999999999999999999999999999999999.e+23));
// DEFAULT-NEXT:             let %[[VALUE55:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), const<d128>(9999999999999999999999999999999999.e+23))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE55]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE55]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE54]], read<bool>(%[[VALUE55]]));
// DEFAULT-NEXT:         let %[[VALUE56:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE54]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE56]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(i192b) -> d128>(%[[VALUE_tests192]], widen<i192b, reason=arg>(const<i191b>(999999999999999999999999999999999949999999999999999999999))));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], const<d128>(9999999999999999999999999999999999.e+23));
// DEFAULT-NEXT:             let %[[VALUE57:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), const<d128>(9999999999999999999999999999999999.e+23))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE57]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE57]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE56]], read<bool>(%[[VALUE57]]));
// DEFAULT-NEXT:         let %[[VALUE58:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE56]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE58]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(i192b) -> d128>(%[[VALUE_tests192]], widen<i192b, reason=arg>(neg<i191b, overflow=ub>(const<i191b>(999999999999999999999999999999999900000000000000000000000)))));
// DEFAULT-NEXT:             let %[[VALUE59:[0-9]+]]: d128 [synthetic] = neg<d128>(const<d128>(9999999999999999999999999999999999.e+23));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], read<d128>(%[[VALUE59]]));
// DEFAULT-NEXT:             let %[[VALUE60:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), neg<d128>(const<d128>(9999999999999999999999999999999999.e+23)))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE60]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE60]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE58]], read<bool>(%[[VALUE60]]));
// DEFAULT-NEXT:         let %[[VALUE61:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE58]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE61]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(i192b) -> d128>(%[[VALUE_tests192]], neg<i192b, overflow=ub>(const<i192b>(2138550877694459381917894711603833000000000000000000000000))));
// DEFAULT-NEXT:             let %[[VALUE62:[0-9]+]]: d128 [synthetic] = neg<d128>(const<d128>(2138550877694459381917894711603833e+24));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], read<d128>(%[[VALUE62]]));
// DEFAULT-NEXT:             let %[[VALUE63:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), neg<d128>(const<d128>(2138550877694459381917894711603833e+24)))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE63]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE63]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE61]], read<bool>(%[[VALUE63]]));
// DEFAULT-NEXT:         let %[[VALUE64:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE61]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE64]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(i192b) -> d128>(%[[VALUE_tests192]], neg<i192b, overflow=ub>(const<i192b>(2138550877694459381917894711603833500000000000000000000000))));
// DEFAULT-NEXT:             let %[[VALUE65:[0-9]+]]: d128 [synthetic] = neg<d128>(const<d128>(2138550877694459381917894711603834e+24));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], read<d128>(%[[VALUE65]]));
// DEFAULT-NEXT:             let %[[VALUE66:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), neg<d128>(const<d128>(2138550877694459381917894711603834e+24)))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE66]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE66]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE64]], read<bool>(%[[VALUE66]]));
// DEFAULT-NEXT:         let %[[VALUE67:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE64]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE67]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(i192b) -> d128>(%[[VALUE_tests192]], neg<i192b, overflow=ub>(const<i192b>(2138550877694459381917894711603833499999999999999999999999))));
// DEFAULT-NEXT:             let %[[VALUE68:[0-9]+]]: d128 [synthetic] = neg<d128>(const<d128>(2138550877694459381917894711603833e+24));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], read<d128>(%[[VALUE68]]));
// DEFAULT-NEXT:             let %[[VALUE69:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), neg<d128>(const<d128>(2138550877694459381917894711603833e+24)))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE69]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE69]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE67]], read<bool>(%[[VALUE69]]));
// DEFAULT-NEXT:         let %[[VALUE70:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE67]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE70]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(i192b) -> d128>(%[[VALUE_tests192]], neg<i192b, overflow=ub>(const<i192b>(2138550877694459381917894711603833999999999999999999999999))));
// DEFAULT-NEXT:             let %[[VALUE71:[0-9]+]]: d128 [synthetic] = neg<d128>(const<d128>(2138550877694459381917894711603834e+24));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], read<d128>(%[[VALUE71]]));
// DEFAULT-NEXT:             let %[[VALUE72:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), neg<d128>(const<d128>(2138550877694459381917894711603834e+24)))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE72]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE72]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE70]], read<bool>(%[[VALUE72]]));
// DEFAULT-NEXT:         let %[[VALUE73:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE70]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE73]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(i192b) -> d128>(%[[VALUE_tests192]], neg<i192b, overflow=ub>(const<i192b>(2138550877694459381917894711603832000000000000000000000000))));
// DEFAULT-NEXT:             let %[[VALUE74:[0-9]+]]: d128 [synthetic] = neg<d128>(const<d128>(2138550877694459381917894711603832e+24));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], read<d128>(%[[VALUE74]]));
// DEFAULT-NEXT:             let %[[VALUE75:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), neg<d128>(const<d128>(2138550877694459381917894711603832e+24)))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE75]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE75]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE73]], read<bool>(%[[VALUE75]]));
// DEFAULT-NEXT:         let %[[VALUE76:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE73]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE76]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(i192b) -> d128>(%[[VALUE_tests192]], neg<i192b, overflow=ub>(const<i192b>(2138550877694459381917894711603832500000000000000000000000))));
// DEFAULT-NEXT:             let %[[VALUE77:[0-9]+]]: d128 [synthetic] = neg<d128>(const<d128>(2138550877694459381917894711603832e+24));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], read<d128>(%[[VALUE77]]));
// DEFAULT-NEXT:             let %[[VALUE78:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), neg<d128>(const<d128>(2138550877694459381917894711603832e+24)))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE78]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE78]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE76]], read<bool>(%[[VALUE78]]));
// DEFAULT-NEXT:         let %[[VALUE79:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE76]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE79]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(i192b) -> d128>(%[[VALUE_tests192]], neg<i192b, overflow=ub>(const<i192b>(2138550877694459381917894711603832500000000000000000000001))));
// DEFAULT-NEXT:             let %[[VALUE80:[0-9]+]]: d128 [synthetic] = neg<d128>(const<d128>(2138550877694459381917894711603833e+24));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], read<d128>(%[[VALUE80]]));
// DEFAULT-NEXT:             let %[[VALUE81:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), neg<d128>(const<d128>(2138550877694459381917894711603833e+24)))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE81]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE81]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE79]], read<bool>(%[[VALUE81]]));
// DEFAULT-NEXT:         let %[[VALUE82:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE79]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE82]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(i192b) -> d128>(%[[VALUE_tests192]], const<i192b>(3138550867693340381917894711603833000000000000000000000000)));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], const<d128>(3138550867693340381917894711603833e+24));
// DEFAULT-NEXT:             let %[[VALUE83:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), const<d128>(3138550867693340381917894711603833e+24))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE83]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE83]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE82]], read<bool>(%[[VALUE83]]));
// DEFAULT-NEXT:         let %[[VALUE84:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE82]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE84]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(i192b) -> d128>(%[[VALUE_tests192]], const<i192b>(3138550867693340381917894711603833208051177722232017256447)));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], const<d128>(3138550867693340381917894711603833e+24));
// DEFAULT-NEXT:             let %[[VALUE85:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), const<d128>(3138550867693340381917894711603833e+24))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE85]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE85]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE84]], read<bool>(%[[VALUE85]]));
// DEFAULT-NEXT:         let %[[VALUE86:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE84]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE86]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(i192b) -> d128>(%[[VALUE_tests192]], neg<i192b, overflow=ub>(const<i192b>(3138550867693340381917894711603833000000000000000000000000))));
// DEFAULT-NEXT:             let %[[VALUE87:[0-9]+]]: d128 [synthetic] = neg<d128>(const<d128>(3138550867693340381917894711603833e+24));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], read<d128>(%[[VALUE87]]));
// DEFAULT-NEXT:             let %[[VALUE88:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), neg<d128>(const<d128>(3138550867693340381917894711603833e+24)))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE88]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE88]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE86]], read<bool>(%[[VALUE88]]));
// DEFAULT-NEXT:         let %[[VALUE89:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE86]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE89]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(i192b) -> d128>(%[[VALUE_tests192]], sub<i192b, overflow=ub>(neg<i192b, overflow=ub>(const<i192b>(3138550867693340381917894711603833208051177722232017256447)), widen<i192b, reason=usual_arith>(const<i2b>(1)))));
// DEFAULT-NEXT:             let %[[VALUE90:[0-9]+]]: d128 [synthetic] = neg<d128>(const<d128>(3138550867693340381917894711603833e+24));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], read<d128>(%[[VALUE90]]));
// DEFAULT-NEXT:             let %[[VALUE91:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), neg<d128>(const<d128>(3138550867693340381917894711603833e+24)))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE91]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE91]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE89]], read<bool>(%[[VALUE91]]));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE89]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         write<d128>(%[[VALUE_a]], call<d128, signature=fn(u192b) -> d128>(%[[VALUE_testu192]], widen<u192b, reason=arg>(const<u1b>(0))));
// DEFAULT-NEXT:         write<d128>(%[[VALUE_b_5]], const<d128>(0.));
// DEFAULT-NEXT:         let %[[VALUE92:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), const<d128>(0.))
// DEFAULT-NEXT:             write<bool>(%[[VALUE92]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE92]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE93:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE92]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE93]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(u192b) -> d128>(%[[VALUE_testu192]], widen<u192b, reason=arg>(const<u3b>(7))));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], const<d128>(7.));
// DEFAULT-NEXT:             let %[[VALUE94:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), const<d128>(7.))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE94]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE94]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE93]], read<bool>(%[[VALUE94]]));
// DEFAULT-NEXT:         let %[[VALUE95:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE93]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE95]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(u192b) -> d128>(%[[VALUE_testu192]], widen<u192b, reason=arg>(const<u6b>(42))));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], const<d128>(42.));
// DEFAULT-NEXT:             let %[[VALUE96:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), const<d128>(42.))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE96]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE96]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE95]], read<bool>(%[[VALUE96]]));
// DEFAULT-NEXT:         let %[[VALUE97:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE95]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE97]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(u192b) -> d128>(%[[VALUE_testu192]], widen<u192b, reason=arg>(const<u30b>(777777777))));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], const<d128>(777777777.));
// DEFAULT-NEXT:             let %[[VALUE98:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), const<d128>(777777777.))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE98]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE98]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE97]], read<bool>(%[[VALUE98]]));
// DEFAULT-NEXT:         let %[[VALUE99:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE97]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE99]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(u192b) -> d128>(%[[VALUE_testu192]], widen<u192b, reason=arg>(const<u97b>(99999999999999999999999999000))));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], const<d128>(99999999999999999999999999000.));
// DEFAULT-NEXT:             let %[[VALUE100:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), const<d128>(99999999999999999999999999000.))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE100]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE100]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE99]], read<bool>(%[[VALUE100]]));
// DEFAULT-NEXT:         let %[[VALUE101:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE99]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE101]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(u192b) -> d128>(%[[VALUE_testu192]], widen<u192b, reason=arg>(const<u120b>(999999999999999999999999999999999900))));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], const<d128>(9999999999999999999999999999999999.e+2));
// DEFAULT-NEXT:             let %[[VALUE102:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), const<d128>(9999999999999999999999999999999999.e+2))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE102]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE102]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE101]], read<bool>(%[[VALUE102]]));
// DEFAULT-NEXT:         let %[[VALUE103:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE101]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE103]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(u192b) -> d128>(%[[VALUE_testu192]], widen<u192b, reason=arg>(const<u123b>(9999999999999999999999999999999999000))));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], const<d128>(9999999999999999999999999999999999.e+3));
// DEFAULT-NEXT:             let %[[VALUE104:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), const<d128>(9999999999999999999999999999999999.e+3))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE104]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE104]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE103]], read<bool>(%[[VALUE104]]));
// DEFAULT-NEXT:         let %[[VALUE105:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE103]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE105]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(u192b) -> d128>(%[[VALUE_testu192]], widen<u192b, reason=arg>(const<u127b>(99999999999999999999999999999999994999))));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], const<d128>(9999999999999999999999999999999999.e+4));
// DEFAULT-NEXT:             let %[[VALUE106:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), const<d128>(9999999999999999999999999999999999.e+4))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE106]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE106]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE105]], read<bool>(%[[VALUE106]]));
// DEFAULT-NEXT:         let %[[VALUE107:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE105]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE107]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(u192b) -> d128>(%[[VALUE_testu192]], widen<u192b, reason=arg>(const<u130b>(999999999999999999999999999999999900000))));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], const<d128>(9999999999999999999999999999999999.e+5));
// DEFAULT-NEXT:             let %[[VALUE108:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), const<d128>(9999999999999999999999999999999999.e+5))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE108]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE108]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE107]], read<bool>(%[[VALUE108]]));
// DEFAULT-NEXT:         let %[[VALUE109:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE107]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE109]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(u192b) -> d128>(%[[VALUE_testu192]], widen<u192b, reason=arg>(const<u133b>(9999999999999999999999999999999999000000))));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], const<d128>(9999999999999999999999999999999999.e+6));
// DEFAULT-NEXT:             let %[[VALUE110:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), const<d128>(9999999999999999999999999999999999.e+6))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE110]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE110]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE109]], read<bool>(%[[VALUE110]]));
// DEFAULT-NEXT:         let %[[VALUE111:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE109]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE111]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(u192b) -> d128>(%[[VALUE_testu192]], widen<u192b, reason=arg>(const<u77b>(123456789012345600000000))));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], const<d128>(123456789012345600000000.));
// DEFAULT-NEXT:             let %[[VALUE112:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), const<d128>(123456789012345600000000.))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE112]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE112]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE111]], read<bool>(%[[VALUE112]]));
// DEFAULT-NEXT:         let %[[VALUE113:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE111]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE113]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(u192b) -> d128>(%[[VALUE_testu192]], widen<u192b, reason=arg>(const<u125b>(34242319854454290000000000000000000000))));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], const<d128>(3424231985445429000000000000000000e+4));
// DEFAULT-NEXT:             let %[[VALUE114:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), const<d128>(3424231985445429000000000000000000e+4))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE114]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE114]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE113]], read<bool>(%[[VALUE114]]));
// DEFAULT-NEXT:         let %[[VALUE115:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE113]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE115]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(u192b) -> d128>(%[[VALUE_testu192]], widen<u192b, reason=arg>(const<u190b>(999999999999999999999999999999999900000000000000000000000))));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], const<d128>(9999999999999999999999999999999999.e+23));
// DEFAULT-NEXT:             let %[[VALUE116:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), const<d128>(9999999999999999999999999999999999.e+23))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE116]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE116]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE115]], read<bool>(%[[VALUE116]]));
// DEFAULT-NEXT:         let %[[VALUE117:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE115]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE117]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(u192b) -> d128>(%[[VALUE_testu192]], const<u192b>(6189354365465174593875438957438959000000000000000000000000)));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], const<d128>(6189354365465174593875438957438959e+24));
// DEFAULT-NEXT:             let %[[VALUE118:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), const<d128>(6189354365465174593875438957438959e+24))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE118]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE118]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE117]], read<bool>(%[[VALUE118]]));
// DEFAULT-NEXT:         let %[[VALUE119:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE117]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE119]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(u192b) -> d128>(%[[VALUE_testu192]], const<u192b>(6189354365465174593875438957438959500000000000000000000000)));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], const<d128>(6189354365465174593875438957438960e+24));
// DEFAULT-NEXT:             let %[[VALUE120:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), const<d128>(6189354365465174593875438957438960e+24))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE120]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE120]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE119]], read<bool>(%[[VALUE120]]));
// DEFAULT-NEXT:         let %[[VALUE121:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE119]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE121]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(u192b) -> d128>(%[[VALUE_testu192]], const<u192b>(6189354365465174593875438957438959499999999999999999999999)));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], const<d128>(6189354365465174593875438957438959e+24));
// DEFAULT-NEXT:             let %[[VALUE122:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), const<d128>(6189354365465174593875438957438959e+24))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE122]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE122]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE121]], read<bool>(%[[VALUE122]]));
// DEFAULT-NEXT:         let %[[VALUE123:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE121]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE123]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(u192b) -> d128>(%[[VALUE_testu192]], const<u192b>(6189354365465174593875438957438959999999999999999999999999)));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], const<d128>(6189354365465174593875438957438960e+24));
// DEFAULT-NEXT:             let %[[VALUE124:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), const<d128>(6189354365465174593875438957438960e+24))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE124]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE124]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE123]], read<bool>(%[[VALUE124]]));
// DEFAULT-NEXT:         let %[[VALUE125:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE123]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE125]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(u192b) -> d128>(%[[VALUE_testu192]], const<u192b>(6189354365465174593875438957438958000000000000000000000000)));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], const<d128>(6189354365465174593875438957438958e+24));
// DEFAULT-NEXT:             let %[[VALUE126:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), const<d128>(6189354365465174593875438957438958e+24))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE126]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE126]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE125]], read<bool>(%[[VALUE126]]));
// DEFAULT-NEXT:         let %[[VALUE127:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE125]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE127]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(u192b) -> d128>(%[[VALUE_testu192]], const<u192b>(6189354365465174593875438957438958500000000000000000000000)));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], const<d128>(6189354365465174593875438957438958e+24));
// DEFAULT-NEXT:             let %[[VALUE128:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), const<d128>(6189354365465174593875438957438958e+24))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE128]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE128]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE127]], read<bool>(%[[VALUE128]]));
// DEFAULT-NEXT:         let %[[VALUE129:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE127]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE129]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(u192b) -> d128>(%[[VALUE_testu192]], const<u192b>(6189354365465174593875438957438958500000000000000000000001)));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], const<d128>(6189354365465174593875438957438959e+24));
// DEFAULT-NEXT:             let %[[VALUE130:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), const<d128>(6189354365465174593875438957438959e+24))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE130]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE130]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE129]], read<bool>(%[[VALUE130]]));
// DEFAULT-NEXT:         let %[[VALUE131:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE129]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE131]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(u192b) -> d128>(%[[VALUE_testu192]], const<u192b>(6277101735386680763835789423207666000000000000000000000000)));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], const<d128>(6277101735386680763835789423207666e+24));
// DEFAULT-NEXT:             let %[[VALUE132:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), const<d128>(6277101735386680763835789423207666e+24))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE132]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE132]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE131]], read<bool>(%[[VALUE132]]));
// DEFAULT-NEXT:         let %[[VALUE133:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE131]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE133]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(u192b) -> d128>(%[[VALUE_testu192]], const<u192b>(6277101735386680763835789423207666416102355444464034512895)));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], const<d128>(6277101735386680763835789423207666e+24));
// DEFAULT-NEXT:             let %[[VALUE134:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), const<d128>(6277101735386680763835789423207666e+24))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE134]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE134]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE133]], read<bool>(%[[VALUE134]]));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE133]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         write<d128>(%[[VALUE_a]], call<d128, signature=fn(i575b) -> d128>(%[[VALUE_tests575]], widen<i575b, reason=arg>(const<i2b>(0))));
// DEFAULT-NEXT:         write<d128>(%[[VALUE_b_5]], const<d128>(0.));
// DEFAULT-NEXT:         let %[[VALUE135:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), const<d128>(0.))
// DEFAULT-NEXT:             write<bool>(%[[VALUE135]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE135]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE136:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE135]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE136]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(i575b) -> d128>(%[[VALUE_tests575]], widen<i575b, reason=arg>(const<i4b>(7))));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], const<d128>(7.));
// DEFAULT-NEXT:             let %[[VALUE137:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), const<d128>(7.))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE137]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE137]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE136]], read<bool>(%[[VALUE137]]));
// DEFAULT-NEXT:         let %[[VALUE138:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE136]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE138]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(i575b) -> d128>(%[[VALUE_tests575]], widen<i575b, reason=arg>(neg<i7b, overflow=ub>(const<i7b>(42)))));
// DEFAULT-NEXT:             let %[[VALUE139:[0-9]+]]: d128 [synthetic] = neg<d128>(const<d128>(42.));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], read<d128>(%[[VALUE139]]));
// DEFAULT-NEXT:             let %[[VALUE140:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), neg<d128>(const<d128>(42.)))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE140]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE140]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE138]], read<bool>(%[[VALUE140]]));
// DEFAULT-NEXT:         let %[[VALUE141:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE138]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE141]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(i575b) -> d128>(%[[VALUE_tests575]], widen<i575b, reason=arg>(neg<i30b, overflow=ub>(const<i30b>(444444444)))));
// DEFAULT-NEXT:             let %[[VALUE142:[0-9]+]]: d128 [synthetic] = neg<d128>(const<d128>(444444444.));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], read<d128>(%[[VALUE142]]));
// DEFAULT-NEXT:             let %[[VALUE143:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), neg<d128>(const<d128>(444444444.)))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE143]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE143]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE141]], read<bool>(%[[VALUE143]]));
// DEFAULT-NEXT:         let %[[VALUE144:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE141]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE144]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(i575b) -> d128>(%[[VALUE_tests575]], widen<i575b, reason=arg>(neg<i53b, overflow=ub>(const<i53b>(3333333333333333)))));
// DEFAULT-NEXT:             let %[[VALUE145:[0-9]+]]: d128 [synthetic] = neg<d128>(const<d128>(3333333333333333.));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], read<d128>(%[[VALUE145]]));
// DEFAULT-NEXT:             let %[[VALUE146:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), neg<d128>(const<d128>(3333333333333333.)))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE146]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE146]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE144]], read<bool>(%[[VALUE146]]));
// DEFAULT-NEXT:         let %[[VALUE147:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE144]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE147]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(i575b) -> d128>(%[[VALUE_tests575]], widen<i575b, reason=arg>(const<i98b>(99999999999999999999999999000))));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], const<d128>(99999999999999999999999999000.));
// DEFAULT-NEXT:             let %[[VALUE148:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), const<d128>(99999999999999999999999999000.))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE148]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE148]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE147]], read<bool>(%[[VALUE148]]));
// DEFAULT-NEXT:         let %[[VALUE149:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE147]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE149]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(i575b) -> d128>(%[[VALUE_tests575]], widen<i575b, reason=arg>(neg<i114b, overflow=ub>(const<i114b>(9999999999999999999999999999999999)))));
// DEFAULT-NEXT:             let %[[VALUE150:[0-9]+]]: d128 [synthetic] = neg<d128>(const<d128>(9999999999999999999999999999999999.));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], read<d128>(%[[VALUE150]]));
// DEFAULT-NEXT:             let %[[VALUE151:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), neg<d128>(const<d128>(9999999999999999999999999999999999.)))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE151]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE151]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE149]], read<bool>(%[[VALUE151]]));
// DEFAULT-NEXT:         let %[[VALUE152:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE149]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE152]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(i575b) -> d128>(%[[VALUE_tests575]], widen<i575b, reason=arg>(const<i121b>(999999999999999999999999999999999900))));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], const<d128>(9999999999999999999999999999999999.e+2));
// DEFAULT-NEXT:             let %[[VALUE153:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), const<d128>(9999999999999999999999999999999999.e+2))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE153]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE153]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE152]], read<bool>(%[[VALUE153]]));
// DEFAULT-NEXT:         let %[[VALUE154:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE152]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE154]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(i575b) -> d128>(%[[VALUE_tests575]], widen<i575b, reason=arg>(neg<i124b, overflow=ub>(const<i124b>(9999999999999999999999999999999999000)))));
// DEFAULT-NEXT:             let %[[VALUE155:[0-9]+]]: d128 [synthetic] = neg<d128>(const<d128>(9999999999999999999999999999999999.e+3));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], read<d128>(%[[VALUE155]]));
// DEFAULT-NEXT:             let %[[VALUE156:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), neg<d128>(const<d128>(9999999999999999999999999999999999.e+3)))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE156]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE156]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE154]], read<bool>(%[[VALUE156]]));
// DEFAULT-NEXT:         let %[[VALUE157:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE154]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE157]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(i575b) -> d128>(%[[VALUE_tests575]], widen<i575b, reason=arg>(const<i131b>(999999999999999999999999999999999900000))));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], const<d128>(9999999999999999999999999999999999.e+5));
// DEFAULT-NEXT:             let %[[VALUE158:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), const<d128>(9999999999999999999999999999999999.e+5))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE158]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE158]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE157]], read<bool>(%[[VALUE158]]));
// DEFAULT-NEXT:         let %[[VALUE159:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE157]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE159]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(i575b) -> d128>(%[[VALUE_tests575]], widen<i575b, reason=arg>(neg<i138b, overflow=ub>(const<i138b>(99999999999999999999999999999999990000000)))));
// DEFAULT-NEXT:             let %[[VALUE160:[0-9]+]]: d128 [synthetic] = neg<d128>(const<d128>(9999999999999999999999999999999999.e+7));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], read<d128>(%[[VALUE160]]));
// DEFAULT-NEXT:             let %[[VALUE161:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), neg<d128>(const<d128>(9999999999999999999999999999999999.e+7)))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE161]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE161]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE159]], read<bool>(%[[VALUE161]]));
// DEFAULT-NEXT:         let %[[VALUE162:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE159]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE162]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(i575b) -> d128>(%[[VALUE_tests575]], widen<i575b, reason=arg>(const<i82b>(1234567890123456000000000))));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], const<d128>(1234567890123456000000000.));
// DEFAULT-NEXT:             let %[[VALUE163:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), const<d128>(1234567890123456000000000.))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE163]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE163]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE162]], read<bool>(%[[VALUE163]]));
// DEFAULT-NEXT:         let %[[VALUE164:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE162]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE164]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(i575b) -> d128>(%[[VALUE_tests575]], widen<i575b, reason=arg>(const<i133b>(3424231985445429000000000000000000000000))));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], const<d128>(3424231985445429000000000000000000e+6));
// DEFAULT-NEXT:             let %[[VALUE165:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), const<d128>(3424231985445429000000000000000000e+6))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE165]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE165]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE164]], read<bool>(%[[VALUE165]]));
// DEFAULT-NEXT:         let %[[VALUE166:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE164]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE166]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(i575b) -> d128>(%[[VALUE_tests575]], widen<i575b, reason=arg>(const<i247b>(99999999999999999999999999999999990000000000000000000000000000000000000000))));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], const<d128>(9999999999999999999999999999999999.e+40));
// DEFAULT-NEXT:             let %[[VALUE167:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), const<d128>(9999999999999999999999999999999999.e+40))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE167]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE167]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE166]], read<bool>(%[[VALUE167]]));
// DEFAULT-NEXT:         let %[[VALUE168:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE166]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE168]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(i575b) -> d128>(%[[VALUE_tests575]], widen<i575b, reason=arg>(neg<i324b, overflow=ub>(const<i324b>(9999999999999999999999999999999999000000000000000000000000000000000000000000000000000000000000000)))));
// DEFAULT-NEXT:             let %[[VALUE169:[0-9]+]]: d128 [synthetic] = neg<d128>(const<d128>(9999999999999999999999999999999999.e+63));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], read<d128>(%[[VALUE169]]));
// DEFAULT-NEXT:             let %[[VALUE170:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), neg<d128>(const<d128>(9999999999999999999999999999999999.e+63)))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE170]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE170]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE168]], read<bool>(%[[VALUE170]]));
// DEFAULT-NEXT:         let %[[VALUE171:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE168]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE171]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(i575b) -> d128>(%[[VALUE_tests575]], widen<i575b, reason=arg>(neg<i398b, overflow=ub>(const<i398b>(213855087769441389758947543987475900000000000000000000000000000000000000000000000000000000000000000000000000000000000000)))));
// DEFAULT-NEXT:             let %[[VALUE172:[0-9]+]]: d128 [synthetic] = neg<d128>(const<d128>(2138550877694413897589475439874759e+86));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], read<d128>(%[[VALUE172]]));
// DEFAULT-NEXT:             let %[[VALUE173:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), neg<d128>(const<d128>(2138550877694413897589475439874759e+86)))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE173]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE173]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE171]], read<bool>(%[[VALUE173]]));
// DEFAULT-NEXT:         let %[[VALUE174:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE171]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE174]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(i575b) -> d128>(%[[VALUE_tests575]], widen<i575b, reason=arg>(neg<i398b, overflow=ub>(const<i398b>(213855087769441389758947543987475950000000000000000000000000000000000000000000000000000000000000000000000000000000000000)))));
// DEFAULT-NEXT:             let %[[VALUE175:[0-9]+]]: d128 [synthetic] = neg<d128>(const<d128>(2138550877694413897589475439874760e+86));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], read<d128>(%[[VALUE175]]));
// DEFAULT-NEXT:             let %[[VALUE176:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), neg<d128>(const<d128>(2138550877694413897589475439874760e+86)))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE176]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE176]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE174]], read<bool>(%[[VALUE176]]));
// DEFAULT-NEXT:         let %[[VALUE177:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE174]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE177]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(i575b) -> d128>(%[[VALUE_tests575]], widen<i575b, reason=arg>(neg<i398b, overflow=ub>(const<i398b>(213855087769441389758947543987475949999999999999999999999999999999999999999999999999999999999999999999999999999999999999)))));
// DEFAULT-NEXT:             let %[[VALUE178:[0-9]+]]: d128 [synthetic] = neg<d128>(const<d128>(2138550877694413897589475439874759e+86));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], read<d128>(%[[VALUE178]]));
// DEFAULT-NEXT:             let %[[VALUE179:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), neg<d128>(const<d128>(2138550877694413897589475439874759e+86)))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE179]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE179]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE177]], read<bool>(%[[VALUE179]]));
// DEFAULT-NEXT:         let %[[VALUE180:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE177]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE180]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(i575b) -> d128>(%[[VALUE_tests575]], widen<i575b, reason=arg>(neg<i398b, overflow=ub>(const<i398b>(213855087769441389758947543987475999999999999999999999999999999999999999999999999999999999999999999999999999999999999999)))));
// DEFAULT-NEXT:             let %[[VALUE181:[0-9]+]]: d128 [synthetic] = neg<d128>(const<d128>(2138550877694413897589475439874760e+86));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], read<d128>(%[[VALUE181]]));
// DEFAULT-NEXT:             let %[[VALUE182:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), neg<d128>(const<d128>(2138550877694413897589475439874760e+86)))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE182]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE182]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE180]], read<bool>(%[[VALUE182]]));
// DEFAULT-NEXT:         let %[[VALUE183:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE180]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE183]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(i575b) -> d128>(%[[VALUE_tests575]], widen<i575b, reason=arg>(neg<i398b, overflow=ub>(const<i398b>(213855087769441389758947543987475800000000000000000000000000000000000000000000000000000000000000000000000000000000000000)))));
// DEFAULT-NEXT:             let %[[VALUE184:[0-9]+]]: d128 [synthetic] = neg<d128>(const<d128>(2138550877694413897589475439874758e+86));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], read<d128>(%[[VALUE184]]));
// DEFAULT-NEXT:             let %[[VALUE185:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), neg<d128>(const<d128>(2138550877694413897589475439874758e+86)))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE185]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE185]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE183]], read<bool>(%[[VALUE185]]));
// DEFAULT-NEXT:         let %[[VALUE186:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE183]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE186]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(i575b) -> d128>(%[[VALUE_tests575]], widen<i575b, reason=arg>(neg<i398b, overflow=ub>(const<i398b>(213855087769441389758947543987475850000000000000000000000000000000000000000000000000000000000000000000000000000000000000)))));
// DEFAULT-NEXT:             let %[[VALUE187:[0-9]+]]: d128 [synthetic] = neg<d128>(const<d128>(2138550877694413897589475439874758e+86));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], read<d128>(%[[VALUE187]]));
// DEFAULT-NEXT:             let %[[VALUE188:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), neg<d128>(const<d128>(2138550877694413897589475439874758e+86)))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE188]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE188]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE186]], read<bool>(%[[VALUE188]]));
// DEFAULT-NEXT:         let %[[VALUE189:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE186]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE189]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(i575b) -> d128>(%[[VALUE_tests575]], widen<i575b, reason=arg>(neg<i398b, overflow=ub>(const<i398b>(213855087769441389758947543987475850000000000000000000000000000000000000000000000000000000000000000000000000000000000001)))));
// DEFAULT-NEXT:             let %[[VALUE190:[0-9]+]]: d128 [synthetic] = neg<d128>(const<d128>(2138550877694413897589475439874759e+86));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], read<d128>(%[[VALUE190]]));
// DEFAULT-NEXT:             let %[[VALUE191:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), neg<d128>(const<d128>(2138550877694413897589475439874759e+86)))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE191]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE191]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE189]], read<bool>(%[[VALUE191]]));
// DEFAULT-NEXT:         let %[[VALUE192:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE189]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE192]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(i575b) -> d128>(%[[VALUE_tests575]], const<i575b>(61832600368276133515125630254911790000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000)));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], const<d128>(6183260036827613351512563025491179e+139));
// DEFAULT-NEXT:             let %[[VALUE193:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), const<d128>(6183260036827613351512563025491179e+139))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE193]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE193]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE192]], read<bool>(%[[VALUE193]]));
// DEFAULT-NEXT:         let %[[VALUE194:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE192]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE194]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(i575b) -> d128>(%[[VALUE_tests575]], const<i575b>(61832600368276133515125630254911797508782837275302959978515764023224306276632966792579100265310761247399417856504034834837841258576687802491886538775473291979151693037174783)));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], const<d128>(6183260036827613351512563025491180e+139));
// DEFAULT-NEXT:             let %[[VALUE195:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), const<d128>(6183260036827613351512563025491180e+139))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE195]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE195]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE194]], read<bool>(%[[VALUE195]]));
// DEFAULT-NEXT:         let %[[VALUE196:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE194]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE196]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(i575b) -> d128>(%[[VALUE_tests575]], neg<i575b, overflow=ub>(const<i575b>(61832600368276133515125630254911790000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000))));
// DEFAULT-NEXT:             let %[[VALUE197:[0-9]+]]: d128 [synthetic] = neg<d128>(const<d128>(6183260036827613351512563025491179e+139));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], read<d128>(%[[VALUE197]]));
// DEFAULT-NEXT:             let %[[VALUE198:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), neg<d128>(const<d128>(6183260036827613351512563025491179e+139)))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE198]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE198]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE196]], read<bool>(%[[VALUE198]]));
// DEFAULT-NEXT:         let %[[VALUE199:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE196]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE199]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(i575b) -> d128>(%[[VALUE_tests575]], sub<i575b, overflow=ub>(neg<i575b, overflow=ub>(const<i575b>(61832600368276133515125630254911797508782837275302959978515764023224306276632966792579100265310761247399417856504034834837841258576687802491886538775473291979151693037174783)), widen<i575b, reason=usual_arith>(const<i2b>(1)))));
// DEFAULT-NEXT:             let %[[VALUE200:[0-9]+]]: d128 [synthetic] = neg<d128>(const<d128>(6183260036827613351512563025491180e+139));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], read<d128>(%[[VALUE200]]));
// DEFAULT-NEXT:             let %[[VALUE201:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), neg<d128>(const<d128>(6183260036827613351512563025491180e+139)))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE201]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE201]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE199]], read<bool>(%[[VALUE201]]));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE199]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         write<d128>(%[[VALUE_a]], call<d128, signature=fn(u575b) -> d128>(%[[VALUE_testu575]], widen<u575b, reason=arg>(const<u1b>(0))));
// DEFAULT-NEXT:         write<d128>(%[[VALUE_b_5]], const<d128>(0.));
// DEFAULT-NEXT:         let %[[VALUE202:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), const<d128>(0.))
// DEFAULT-NEXT:             write<bool>(%[[VALUE202]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE202]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE203:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE202]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE203]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(u575b) -> d128>(%[[VALUE_testu575]], widen<u575b, reason=arg>(const<u5b>(17))));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], const<d128>(17.));
// DEFAULT-NEXT:             let %[[VALUE204:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), const<d128>(17.))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE204]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE204]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE203]], read<bool>(%[[VALUE204]]));
// DEFAULT-NEXT:         let %[[VALUE205:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE203]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE205]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(u575b) -> d128>(%[[VALUE_testu575]], widen<u575b, reason=arg>(const<u9b>(420))));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], const<d128>(420.));
// DEFAULT-NEXT:             let %[[VALUE206:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), const<d128>(420.))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE206]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE206]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE205]], read<bool>(%[[VALUE206]]));
// DEFAULT-NEXT:         let %[[VALUE207:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE205]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE207]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(u575b) -> d128>(%[[VALUE_testu575]], widen<u575b, reason=arg>(const<u30b>(888888888))));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], const<d128>(888888888.));
// DEFAULT-NEXT:             let %[[VALUE208:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), const<d128>(888888888.))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE208]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE208]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE207]], read<bool>(%[[VALUE208]]));
// DEFAULT-NEXT:         let %[[VALUE209:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE207]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE209]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(u575b) -> d128>(%[[VALUE_testu575]], widen<u575b, reason=arg>(const<u54b>(9999999999999000))));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], const<d128>(9999999999999000.));
// DEFAULT-NEXT:             let %[[VALUE210:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), const<d128>(9999999999999000.))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE210]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE210]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE209]], read<bool>(%[[VALUE210]]));
// DEFAULT-NEXT:         let %[[VALUE211:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE209]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE211]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(u575b) -> d128>(%[[VALUE_testu575]], widen<u575b, reason=arg>(const<u137b>(99999999999999999999999999999999990000000))));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], const<d128>(9999999999999999999999999999999999.e+7));
// DEFAULT-NEXT:             let %[[VALUE212:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), const<d128>(9999999999999999999999999999999999.e+7))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE212]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE212]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE211]], read<bool>(%[[VALUE212]]));
// DEFAULT-NEXT:         let %[[VALUE213:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE211]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE213]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(u575b) -> d128>(%[[VALUE_testu575]], widen<u575b, reason=arg>(const<u143b>(9999999999999999999999999999999999000000000))));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], const<d128>(9999999999999999999999999999999999.e+9));
// DEFAULT-NEXT:             let %[[VALUE214:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), const<d128>(9999999999999999999999999999999999.e+9))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE214]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE214]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE213]], read<bool>(%[[VALUE214]]));
// DEFAULT-NEXT:         let %[[VALUE215:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE213]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE215]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(u575b) -> d128>(%[[VALUE_testu575]], widen<u575b, reason=arg>(const<u157b>(99999999999999999999999999999999990000000000000))));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], const<d128>(9999999999999999999999999999999999.e+13));
// DEFAULT-NEXT:             let %[[VALUE216:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), const<d128>(9999999999999999999999999999999999.e+13))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE216]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE216]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE215]], read<bool>(%[[VALUE216]]));
// DEFAULT-NEXT:         let %[[VALUE217:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE215]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE217]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(u575b) -> d128>(%[[VALUE_testu575]], widen<u575b, reason=arg>(const<u163b>(9999999999999999999999999999999999000000000000000))));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], const<d128>(9999999999999999999999999999999999.e+15));
// DEFAULT-NEXT:             let %[[VALUE218:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), const<d128>(9999999999999999999999999999999999.e+15))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE218]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE218]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE217]], read<bool>(%[[VALUE218]]));
// DEFAULT-NEXT:         let %[[VALUE219:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE217]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE219]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(u575b) -> d128>(%[[VALUE_testu575]], widen<u575b, reason=arg>(const<u110b>(1234567890123456000000000000000000))));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], const<d128>(1234567890123456000000000000000000.));
// DEFAULT-NEXT:             let %[[VALUE220:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), const<d128>(1234567890123456000000000000000000.))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE220]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE220]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE219]], read<bool>(%[[VALUE220]]));
// DEFAULT-NEXT:         let %[[VALUE221:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE219]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE221]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(u575b) -> d128>(%[[VALUE_testu575]], widen<u575b, reason=arg>(const<u125b>(34242319854454290000000000000000000000))));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], const<d128>(3424231985445429000000000000000000e+4));
// DEFAULT-NEXT:             let %[[VALUE222:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), const<d128>(3424231985445429000000000000000000e+4))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE222]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE222]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE221]], read<bool>(%[[VALUE222]]));
// DEFAULT-NEXT:         let %[[VALUE223:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE221]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE223]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(u575b) -> d128>(%[[VALUE_testu575]], widen<u575b, reason=arg>(const<u223b>(9999999999999999999999999999999999000000000000000000000000000000000))));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], const<d128>(9999999999999999999999999999999999.e+33));
// DEFAULT-NEXT:             let %[[VALUE224:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), const<d128>(9999999999999999999999999999999999.e+33))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE224]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE224]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE223]], read<bool>(%[[VALUE224]]));
// DEFAULT-NEXT:         let %[[VALUE225:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE223]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE225]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(u575b) -> d128>(%[[VALUE_testu575]], widen<u575b, reason=arg>(const<u458b>(618935436546517949837539847534981700000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000))));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], const<d128>(6189354365465179498375398475349817e+104));
// DEFAULT-NEXT:             let %[[VALUE226:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), const<d128>(6189354365465179498375398475349817e+104))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE226]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE226]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE225]], read<bool>(%[[VALUE226]]));
// DEFAULT-NEXT:         let %[[VALUE227:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE225]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE227]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(u575b) -> d128>(%[[VALUE_testu575]], widen<u575b, reason=arg>(const<u458b>(618935436546517949837539847534981750000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000))));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], const<d128>(6189354365465179498375398475349818e+104));
// DEFAULT-NEXT:             let %[[VALUE228:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), const<d128>(6189354365465179498375398475349818e+104))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE228]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE228]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE227]], read<bool>(%[[VALUE228]]));
// DEFAULT-NEXT:         let %[[VALUE229:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE227]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE229]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(u575b) -> d128>(%[[VALUE_testu575]], widen<u575b, reason=arg>(const<u458b>(618935436546517949837539847534981749999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999))));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], const<d128>(6189354365465179498375398475349817e+104));
// DEFAULT-NEXT:             let %[[VALUE230:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), const<d128>(6189354365465179498375398475349817e+104))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE230]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE230]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE229]], read<bool>(%[[VALUE230]]));
// DEFAULT-NEXT:         let %[[VALUE231:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE229]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE231]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(u575b) -> d128>(%[[VALUE_testu575]], widen<u575b, reason=arg>(const<u458b>(618935436546517949837539847534981799999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999))));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], const<d128>(6189354365465179498375398475349818e+104));
// DEFAULT-NEXT:             let %[[VALUE232:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), const<d128>(6189354365465179498375398475349818e+104))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE232]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE232]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE231]], read<bool>(%[[VALUE232]]));
// DEFAULT-NEXT:         let %[[VALUE233:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE231]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE233]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(u575b) -> d128>(%[[VALUE_testu575]], widen<u575b, reason=arg>(const<u458b>(618935436546517949837539847534981800000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000))));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], const<d128>(6189354365465179498375398475349818e+104));
// DEFAULT-NEXT:             let %[[VALUE234:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), const<d128>(6189354365465179498375398475349818e+104))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE234]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE234]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE233]], read<bool>(%[[VALUE234]]));
// DEFAULT-NEXT:         let %[[VALUE235:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE233]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE235]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(u575b) -> d128>(%[[VALUE_testu575]], widen<u575b, reason=arg>(const<u458b>(618935436546517949837539847534981850000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000))));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], const<d128>(6189354365465179498375398475349818e+104));
// DEFAULT-NEXT:             let %[[VALUE236:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), const<d128>(6189354365465179498375398475349818e+104))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE236]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE236]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE235]], read<bool>(%[[VALUE236]]));
// DEFAULT-NEXT:         let %[[VALUE237:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE235]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE237]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(u575b) -> d128>(%[[VALUE_testu575]], widen<u575b, reason=arg>(const<u458b>(618935436546517949837539847534981850000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001))));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], const<d128>(6189354365465179498375398475349819e+104));
// DEFAULT-NEXT:             let %[[VALUE238:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), const<d128>(6189354365465179498375398475349819e+104))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE238]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE238]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE237]], read<bool>(%[[VALUE238]]));
// DEFAULT-NEXT:         let %[[VALUE239:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE237]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE239]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(u575b) -> d128>(%[[VALUE_testu575]], const<u575b>(99999999999999999999999999999999990000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000)));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], const<d128>(9999999999999999999999999999999999.e+139));
// DEFAULT-NEXT:             let %[[VALUE240:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), const<d128>(9999999999999999999999999999999999.e+139))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE240]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE240]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE239]], read<bool>(%[[VALUE240]]));
// DEFAULT-NEXT:         let %[[VALUE241:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE239]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE241]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(u575b) -> d128>(%[[VALUE_testu575]], const<u575b>(123665200736552267030251260509823500000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000)));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], const<d128>(1236652007365522670302512605098235e+140));
// DEFAULT-NEXT:             let %[[VALUE242:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), const<d128>(1236652007365522670302512605098235e+140))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE242]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE242]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE241]], read<bool>(%[[VALUE242]]));
// DEFAULT-NEXT:         let %[[VALUE243:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE241]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE243]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(u575b) -> d128>(%[[VALUE_testu575]], const<u575b>(123665200736552267030251260509823549999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999)));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], const<d128>(1236652007365522670302512605098235e+140));
// DEFAULT-NEXT:             let %[[VALUE244:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), const<d128>(1236652007365522670302512605098235e+140))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE244]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE244]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE243]], read<bool>(%[[VALUE244]]));
// DEFAULT-NEXT:         let %[[VALUE245:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE243]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE245]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(u575b) -> d128>(%[[VALUE_testu575]], const<u575b>(123665200736552267030251260509823550000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000)));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], const<d128>(1236652007365522670302512605098236e+140));
// DEFAULT-NEXT:             let %[[VALUE246:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), const<d128>(1236652007365522670302512605098236e+140))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE246]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE246]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE245]], read<bool>(%[[VALUE246]]));
// DEFAULT-NEXT:         let %[[VALUE247:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE245]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE247]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(u575b) -> d128>(%[[VALUE_testu575]], const<u575b>(123665200736552267030251260509823550000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001)));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], const<d128>(1236652007365522670302512605098236e+140));
// DEFAULT-NEXT:             let %[[VALUE248:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), const<d128>(1236652007365522670302512605098236e+140))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE248]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE248]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE247]], read<bool>(%[[VALUE248]]));
// DEFAULT-NEXT:         let %[[VALUE249:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE247]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE249]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(u575b) -> d128>(%[[VALUE_testu575]], const<u575b>(123665200736552267030251260509823550000000000000000000000000000000000000001000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000)));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], const<d128>(1236652007365522670302512605098236e+140));
// DEFAULT-NEXT:             let %[[VALUE250:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), const<d128>(1236652007365522670302512605098236e+140))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE250]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE250]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE249]], read<bool>(%[[VALUE250]]));
// DEFAULT-NEXT:         let %[[VALUE251:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE249]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE251]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a]], call<d128, signature=fn(u575b) -> d128>(%[[VALUE_testu575]], const<u575b>(123665200736552267030251260509823595017565674550605919957031528046448612553265933585158200530621522494798835713008069669675682517153375604983773077550946583958303386074349567)));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_5]], const<d128>(1236652007365522670302512605098236e+140));
// DEFAULT-NEXT:             let %[[VALUE252:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a]]), const<d128>(1236652007365522670302512605098236e+140))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE252]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE252]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_5]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE251]], read<bool>(%[[VALUE252]]));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE251]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
