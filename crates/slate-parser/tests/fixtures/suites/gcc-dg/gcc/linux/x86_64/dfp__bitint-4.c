/* PR c/102989 */
/* { dg-require-effective-target bitint } */
/* { dg-options "-O2 -std=c23 -pedantic-errors" } */

#if __BITINT_MAXWIDTH__ >= 192
__attribute__((noipa)) _Decimal64
tests192 (_BitInt(192) b)
{
  return b;
}

__attribute__((noipa)) _Decimal64
testu192 (unsigned _BitInt(192) b)
{
  return b;
}
#endif

#if __BITINT_MAXWIDTH__ >= 575
__attribute__((noipa)) _Decimal64
tests575 (_BitInt(575) b)
{
  return b;
}

__attribute__((noipa)) _Decimal64
testu575 (unsigned _BitInt(575) b)
{
  return b;
}
#endif

int
main ()
{
  _Decimal64 a, b;
#define CHECK(x, y) (a = (x), b = (y), a != (y) || __builtin_memcmp (&a, &b, sizeof (a)))
#if __BITINT_MAXWIDTH__ >= 192
  if (CHECK (tests192 (0wb), 0.DD)
      || CHECK (tests192 (7wb), 7.DD)
      || CHECK (tests192 (-42wb), -42.DD)
      || CHECK (tests192 (-777777777wb), -777777777.DD)
      || CHECK (tests192 (9999999999999000wb), 9999999999999000.DD)
      || CHECK (tests192 (-9999999999999999wb), -9999999999999999.DD)
      || CHECK (tests192 (-99999999999999994wb), -9999999999999999.e+1DD)
      || CHECK (tests192 (99999999999999995wb), 1000000000000000.e+2DD)
      || CHECK (tests192 (999999999999999900wb), 9999999999999999.e+2DD)
      || CHECK (tests192 (999999999999999949wb), 9999999999999999.e+2DD)
      || CHECK (tests192 (-9999999999999999000wb), -9999999999999999.e+3DD)
      || CHECK (tests192 (9999999999999999499wb), 9999999999999999.e+3DD)
      || CHECK (tests192 (999999999999999900000wb), 9999999999999999.e+5DD)
      || CHECK (tests192 (999999999999999949999wb), 9999999999999999.e+5DD)
      || CHECK (tests192 (-9999999999999999000000wb), -9999999999999999.e+6DD)
      || CHECK (tests192 (-9999999999999999499999wb), -9999999999999999.e+6DD)
      || CHECK (tests192 (123456789012345600000000wb), 1234567890123456.e+8DD)
      || CHECK (tests192 (34242319854454290000000000000000000000wb), 3424231985445429e+22DD)
      || CHECK (tests192 (999999999999999900000000000000000000000000000000wb), 9999999999999999.e+32DD)
      || CHECK (tests192 (999999999999999949999999999999999999999999999999wb), 9999999999999999.e+32DD)
      || CHECK (tests192 (-999999999999999900000000000000000000000000000000000000000wb), -9999999999999999.e+41DD)
      || CHECK (tests192 (-2138550877694459000000000000000000000000000000000000000000wb), -2138550877694459e+42DD)
      || CHECK (tests192 (-2138550877694459500000000000000000000000000000000000000000wb), -2138550877694460e+42DD)
      || CHECK (tests192 (-2138550877694459499999999999999999999999999999999999999999wb), -2138550877694459e+42DD)
      || CHECK (tests192 (-2138550877694459999999999999999999999999999999999999999999wb), -2138550877694460e+42DD)
      || CHECK (tests192 (-2138550877694458000000000000000000000000000000000000000000wb), -2138550877694458e+42DD)
      || CHECK (tests192 (-2138550877694458500000000000000000000000000000000000000000wb), -2138550877694458e+42DD)
      || CHECK (tests192 (-2138550877694458500000000000000000000000000000000000000001wb), -2138550877694459e+42DD)
      || CHECK (tests192 (3138550867693340000000000000000000000000000000000000000000wb), 3138550867693340e+42DD)
      || CHECK (tests192 (3138550867693340381917894711603833208051177722232017256447wb), 3138550867693340e+42DD)
      || CHECK (tests192 (-3138550867693340000000000000000000000000000000000000000000wb), -3138550867693340e+42DD)
      || CHECK (tests192 (-3138550867693340381917894711603833208051177722232017256447wb - 1wb), -3138550867693340e+42DD))
    __builtin_abort ();
  if (CHECK (testu192 (0uwb), 0.DD)
      || CHECK (testu192 (7uwb), 7.DD)
      || CHECK (testu192 (42uwb), 42.DD)
      || CHECK (testu192 (777777777uwb), 777777777.DD)
      || CHECK (testu192 (9999999999999000uwb), 9999999999999000.DD)
      || CHECK (testu192 (999999999999999900uwb), 9999999999999999.e+2DD)
      || CHECK (testu192 (9999999999999999000uwb), 9999999999999999.e+3DD)
      || CHECK (testu192 (99999999999999994999uwb), 9999999999999999.e+4DD)
      || CHECK (testu192 (999999999999999900000uwb), 9999999999999999.e+5DD)
      || CHECK (testu192 (9999999999999999000000uwb), 9999999999999999.e+6DD)
      || CHECK (testu192 (123456789012345600000000uwb), 1234567890123456.e+8DD)
      || CHECK (testu192 (34242319854454290000000000000000000000uwb), 3424231985445429e+22DD)
      || CHECK (testu192 (9999999999999999000000000000000000000000000000000uwb), 9999999999999999.e+33DD)
      || CHECK (testu192 (618935436546517900000000000000000000000000000000000000uwb), 6189354365465179e+38DD)
      || CHECK (testu192 (618935436546517950000000000000000000000000000000000000uwb), 6189354365465180e+38DD)
      || CHECK (testu192 (618935436546517949999999999999999999999999999999999999uwb), 6189354365465179e+38DD)
      || CHECK (testu192 (618935436546517999999999999999999999999999999999999999uwb), 6189354365465180e+38DD)
      || CHECK (testu192 (618935436546517800000000000000000000000000000000000000uwb), 6189354365465178e+38DD)
      || CHECK (testu192 (618935436546517850000000000000000000000000000000000000uwb), 6189354365465178e+38DD)
      || CHECK (testu192 (618935436546517850000000000000000000000000000000000001uwb), 6189354365465179e+38DD)
      || CHECK (testu192 (99999999999999990000000000000000000000000000000000000000uwb), 9999999999999999.e+40DD)
      || CHECK (testu192 (6277101735386680000000000000000000000000000000000000000000uwb), 6277101735386680e+42DD)
      || CHECK (testu192 (6277101735386680499999999999999999999999999999999999999999uwb), 6277101735386680e+42DD)
      || CHECK (testu192 (6277101735386680500000000000000000000000000000000000000000uwb), 6277101735386680e+42DD)
      || CHECK (testu192 (6277101735386680500000000000000000000000000000000000000001uwb), 6277101735386681e+42DD)
      || CHECK (testu192 (6277101735386680500000000000000000000010000000000000000000uwb), 6277101735386681e+42DD)
      || CHECK (testu192 (6277101735386680763835789423207666416102355444464034512895uwb), 6277101735386681e+42DD))
    __builtin_abort ();
#endif
#if __BITINT_MAXWIDTH__ >= 575
  if (CHECK (tests575 (0wb), 0.DD)
      || CHECK (tests575 (7wb), 7.DD)
      || CHECK (tests575 (-42wb), -42.DD)
      || CHECK (tests575 (-444444444wb), -444444444.DD)
      || CHECK (tests575 (9999999999999000wb), 9999999999999000.DD)
      || CHECK (tests575 (-9999999999999999wb), -9999999999999999.DD)
      || CHECK (tests575 (999999999999999900wb), 9999999999999999.e+2DD)
      || CHECK (tests575 (-9999999999999999000wb), -9999999999999999.e+3DD)
      || CHECK (tests575 (999999999999999900000wb), 9999999999999999.e+5DD)
      || CHECK (tests575 (-99999999999999990000000wb), -9999999999999999.e+7DD)
      || CHECK (tests575 (1234567890123456000000000wb), 1234567890123456.e+9DD)
      || CHECK (tests575 (3424231985445429000000000000000000000000wb), 3424231985445429e+24DD)
      || CHECK (tests575 (99999999999999990000000000000000000000000000000000000000wb), 9999999999999999.e+40DD)
      || CHECK (tests575 (-9999999999999999000000000000000000000000000000000000000000000000000000000000000wb), -9999999999999999.e+63DD)
      || CHECK (tests575 (-213855087769445900000000000000000000000000000000000000000000000000000000000000000000000000000000000000wb), -2138550877694459e+86DD)
      || CHECK (tests575 (-213855087769445950000000000000000000000000000000000000000000000000000000000000000000000000000000000000wb), -2138550877694460e+86DD)
      || CHECK (tests575 (-213855087769445949999999999999999999999999999999999999999999999999999999999999999999999999999999999999wb), -2138550877694459e+86DD)
      || CHECK (tests575 (-213855087769445999999999999999999999999999999999999999999999999999999999999999999999999999999999999999wb), -2138550877694460e+86DD)
      || CHECK (tests575 (-213855087769445800000000000000000000000000000000000000000000000000000000000000000000000000000000000000wb), -2138550877694458e+86DD)
      || CHECK (tests575 (-213855087769445850000000000000000000000000000000000000000000000000000000000000000000000000000000000000wb), -2138550877694458e+86DD)
      || CHECK (tests575 (-213855087769445850000000000000000000000000000000000000000000000000000000000000000000000000000000000001wb), -2138550877694459e+86DD)
      || CHECK (tests575 (61832600368276130000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000wb), 6183260036827613e+157DD)
      || CHECK (tests575 (61832600368276133515125630254911797508782837275302959978515764023224306276632966792579100265310761247399417856504034834837841258576687802491886538775473291979151693037174783wb), 6183260036827613e+157DD)
      || CHECK (tests575 (-61832600368276130000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000wb), -6183260036827613e+157DD)
      || CHECK (tests575 (-61832600368276133515125630254911797508782837275302959978515764023224306276632966792579100265310761247399417856504034834837841258576687802491886538775473291979151693037174783wb - 1wb), -6183260036827613e+157DD))
    __builtin_abort ();
  if (CHECK (testu575 (0uwb), 0.DD)
      || CHECK (testu575 (17uwb), 17.DD)
      || CHECK (testu575 (420uwb), 420.DD)
      || CHECK (testu575 (888888888uwb), 888888888.DD)
      || CHECK (testu575 (9999999999999000uwb), 9999999999999000.DD)
      || CHECK (testu575 (99999999999999990000000uwb), 9999999999999999.e+7DD)
      || CHECK (testu575 (9999999999999999000000000uwb), 9999999999999999.e+9DD)
      || CHECK (testu575 (99999999999999990000000000000uwb), 9999999999999999.e+13DD)
      || CHECK (testu575 (9999999999999999000000000000000uwb), 9999999999999999.e+15DD)
      || CHECK (testu575 (1234567890123456000000000000000000uwb), 1234567890123456.e+18DD)
      || CHECK (testu575 (34242319854454290000000000000000000000uwb), 3424231985445429e+22DD)
      || CHECK (testu575 (9999999999999999000000000000000000000000000000000uwb), 9999999999999999.e+33DD)
      || CHECK (testu575 (618935436546517900000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000uwb), 6189354365465179e+104DD)
      || CHECK (testu575 (618935436546517950000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000uwb), 6189354365465180e+104DD)
      || CHECK (testu575 (618935436546517949999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999uwb), 6189354365465179e+104DD)
      || CHECK (testu575 (618935436546517999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999uwb), 6189354365465180e+104DD)
      || CHECK (testu575 (618935436546517800000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000uwb), 6189354365465178e+104DD)
      || CHECK (testu575 (618935436546517850000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000uwb), 6189354365465178e+104DD)
      || CHECK (testu575 (618935436546517850000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001uwb), 6189354365465179e+104DD)
      || CHECK (testu575 (99999999999999990000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000uwb), 9999999999999999.e+139DD)
      || CHECK (testu575 (123665200736552200000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000uwb), 1236652007365522e+158DD)
      || CHECK (testu575 (123665200736552249999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999uwb), 1236652007365522e+158DD)
      || CHECK (testu575 (123665200736552250000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000uwb), 1236652007365522e+158DD)
      || CHECK (testu575 (123665200736552250000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001uwb), 1236652007365523e+158DD)
      || CHECK (testu575 (123665200736552250000000000000000000000000000000000000001000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000uwb), 1236652007365523e+158DD)
      || CHECK (testu575 (123665200736552267030251260509823595017565674550605919957031528046448612553265933585158200530621522494798835713008069669675682517153375604983773077550946583958303386074349567uwb), 1236652007365523e+158DD))
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
// DEFAULT-NEXT:     fn %[[VALUE_tests192:[0-9]+]] @tests192(%[[VALUE_b:[0-9]+]] b: i192b) -> d64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return int_to_float<d64, reason=return, exact=false, rounding=nearest_even, exceptions=observable>(read<i192b>(%[[VALUE_b]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testu192:[0-9]+]] @testu192(%[[VALUE_b_2:[0-9]+]] b: u192b) -> d64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return int_to_float<d64, reason=return, exact=false, rounding=nearest_even, exceptions=observable>(read<u192b>(%[[VALUE_b_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_tests575:[0-9]+]] @tests575(%[[VALUE_b_3:[0-9]+]] b: i575b) -> d64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return int_to_float<d64, reason=return, exact=false, rounding=nearest_even, exceptions=observable>(read<i575b>(%[[VALUE_b_3]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testu575:[0-9]+]] @testu575(%[[VALUE_b_4:[0-9]+]] b: u575b) -> d64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return int_to_float<d64, reason=return, exact=false, rounding=nearest_even, exceptions=observable>(read<u575b>(%[[VALUE_b_4]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_memcmp:[0-9]+]] @__builtin_memcmp(%[[VALUE0:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE1:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE2:[0-9]+]] <unnamed>: u64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: d64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_b_5:[0-9]+]] b: d64 [storage=automatic];
// DEFAULT-NEXT:         write<d64>(%[[VALUE_a]], call<d64, signature=fn(i192b) -> d64>(%[[VALUE_tests192]], widen<i192b, reason=arg>(const<i2b>(0))));
// DEFAULT-NEXT:         call<d64, signature=fn(i192b) -> d64>(%[[VALUE_tests192]], widen<i192b, reason=arg>(const<i2b>(0)));
// DEFAULT-NEXT:         write<d64>(%[[VALUE_b_5]], const<d64>(0.));
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(0.))
// DEFAULT-NEXT:             write<bool>(%[[VALUE3]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE3]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE3]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE4]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(i192b) -> d64>(%[[VALUE_tests192]], widen<i192b, reason=arg>(const<i4b>(7))));
// DEFAULT-NEXT:             call<d64, signature=fn(i192b) -> d64>(%[[VALUE_tests192]], widen<i192b, reason=arg>(const<i4b>(7)));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], const<d64>(7.));
// DEFAULT-NEXT:             let %[[VALUE5:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(7.))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE5]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE5]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE4]], read<bool>(%[[VALUE5]]));
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE4]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE6]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(i192b) -> d64>(%[[VALUE_tests192]], widen<i192b, reason=arg>(neg<i7b, overflow=ub>(const<i7b>(42)))));
// DEFAULT-NEXT:             call<d64, signature=fn(i192b) -> d64>(%[[VALUE_tests192]], widen<i192b, reason=arg>(neg<i7b, overflow=ub>(const<i7b>(42))));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], neg<d64>(const<d64>(42.)));
// DEFAULT-NEXT:             let %[[VALUE7:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), neg<d64>(const<d64>(42.)))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE7]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE7]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE6]], read<bool>(%[[VALUE7]]));
// DEFAULT-NEXT:         let %[[VALUE8:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE6]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE8]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(i192b) -> d64>(%[[VALUE_tests192]], widen<i192b, reason=arg>(neg<i31b, overflow=ub>(const<i31b>(777777777)))));
// DEFAULT-NEXT:             call<d64, signature=fn(i192b) -> d64>(%[[VALUE_tests192]], widen<i192b, reason=arg>(neg<i31b, overflow=ub>(const<i31b>(777777777))));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], neg<d64>(const<d64>(777777777.)));
// DEFAULT-NEXT:             let %[[VALUE9:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), neg<d64>(const<d64>(777777777.)))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE9]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE9]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE8]], read<bool>(%[[VALUE9]]));
// DEFAULT-NEXT:         let %[[VALUE10:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE8]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE10]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(i192b) -> d64>(%[[VALUE_tests192]], widen<i192b, reason=arg>(const<i55b>(9999999999999000))));
// DEFAULT-NEXT:             call<d64, signature=fn(i192b) -> d64>(%[[VALUE_tests192]], widen<i192b, reason=arg>(const<i55b>(9999999999999000)));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], const<d64>(9999999999999000.));
// DEFAULT-NEXT:             let %[[VALUE11:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(9999999999999000.))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE11]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE11]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE10]], read<bool>(%[[VALUE11]]));
// DEFAULT-NEXT:         let %[[VALUE12:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE10]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE12]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(i192b) -> d64>(%[[VALUE_tests192]], widen<i192b, reason=arg>(neg<i55b, overflow=ub>(const<i55b>(9999999999999999)))));
// DEFAULT-NEXT:             call<d64, signature=fn(i192b) -> d64>(%[[VALUE_tests192]], widen<i192b, reason=arg>(neg<i55b, overflow=ub>(const<i55b>(9999999999999999))));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], neg<d64>(const<d64>(9999999999999999.)));
// DEFAULT-NEXT:             let %[[VALUE13:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), neg<d64>(const<d64>(9999999999999999.)))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE13]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE13]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE12]], read<bool>(%[[VALUE13]]));
// DEFAULT-NEXT:         let %[[VALUE14:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE12]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE14]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(i192b) -> d64>(%[[VALUE_tests192]], widen<i192b, reason=arg>(neg<i58b, overflow=ub>(const<i58b>(99999999999999994)))));
// DEFAULT-NEXT:             call<d64, signature=fn(i192b) -> d64>(%[[VALUE_tests192]], widen<i192b, reason=arg>(neg<i58b, overflow=ub>(const<i58b>(99999999999999994))));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], neg<d64>(const<d64>(9999999999999999.e+1)));
// DEFAULT-NEXT:             let %[[VALUE15:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), neg<d64>(const<d64>(9999999999999999.e+1)))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE15]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE15]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE14]], read<bool>(%[[VALUE15]]));
// DEFAULT-NEXT:         let %[[VALUE16:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE14]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE16]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(i192b) -> d64>(%[[VALUE_tests192]], widen<i192b, reason=arg>(const<i58b>(99999999999999995))));
// DEFAULT-NEXT:             call<d64, signature=fn(i192b) -> d64>(%[[VALUE_tests192]], widen<i192b, reason=arg>(const<i58b>(99999999999999995)));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], const<d64>(1000000000000000.e+2));
// DEFAULT-NEXT:             let %[[VALUE17:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(1000000000000000.e+2))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE17]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE17]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE16]], read<bool>(%[[VALUE17]]));
// DEFAULT-NEXT:         let %[[VALUE18:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE16]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE18]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(i192b) -> d64>(%[[VALUE_tests192]], widen<i192b, reason=arg>(const<i61b>(999999999999999900))));
// DEFAULT-NEXT:             call<d64, signature=fn(i192b) -> d64>(%[[VALUE_tests192]], widen<i192b, reason=arg>(const<i61b>(999999999999999900)));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], const<d64>(9999999999999999.e+2));
// DEFAULT-NEXT:             let %[[VALUE19:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(9999999999999999.e+2))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE19]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE19]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE18]], read<bool>(%[[VALUE19]]));
// DEFAULT-NEXT:         let %[[VALUE20:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE18]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE20]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(i192b) -> d64>(%[[VALUE_tests192]], widen<i192b, reason=arg>(const<i61b>(999999999999999949))));
// DEFAULT-NEXT:             call<d64, signature=fn(i192b) -> d64>(%[[VALUE_tests192]], widen<i192b, reason=arg>(const<i61b>(999999999999999949)));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], const<d64>(9999999999999999.e+2));
// DEFAULT-NEXT:             let %[[VALUE21:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(9999999999999999.e+2))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE21]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE21]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE20]], read<bool>(%[[VALUE21]]));
// DEFAULT-NEXT:         let %[[VALUE22:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE20]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE22]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(i192b) -> d64>(%[[VALUE_tests192]], widen<i192b, reason=arg>(neg<i65b, overflow=ub>(const<i65b>(9999999999999999000)))));
// DEFAULT-NEXT:             call<d64, signature=fn(i192b) -> d64>(%[[VALUE_tests192]], widen<i192b, reason=arg>(neg<i65b, overflow=ub>(const<i65b>(9999999999999999000))));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], neg<d64>(const<d64>(9999999999999999.e+3)));
// DEFAULT-NEXT:             let %[[VALUE23:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), neg<d64>(const<d64>(9999999999999999.e+3)))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE23]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE23]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE22]], read<bool>(%[[VALUE23]]));
// DEFAULT-NEXT:         let %[[VALUE24:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE22]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE24]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(i192b) -> d64>(%[[VALUE_tests192]], widen<i192b, reason=arg>(const<i65b>(9999999999999999499))));
// DEFAULT-NEXT:             call<d64, signature=fn(i192b) -> d64>(%[[VALUE_tests192]], widen<i192b, reason=arg>(const<i65b>(9999999999999999499)));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], const<d64>(9999999999999999.e+3));
// DEFAULT-NEXT:             let %[[VALUE25:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(9999999999999999.e+3))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE25]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE25]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE24]], read<bool>(%[[VALUE25]]));
// DEFAULT-NEXT:         let %[[VALUE26:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE24]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE26]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(i192b) -> d64>(%[[VALUE_tests192]], widen<i192b, reason=arg>(const<i71b>(999999999999999900000))));
// DEFAULT-NEXT:             call<d64, signature=fn(i192b) -> d64>(%[[VALUE_tests192]], widen<i192b, reason=arg>(const<i71b>(999999999999999900000)));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], const<d64>(9999999999999999.e+5));
// DEFAULT-NEXT:             let %[[VALUE27:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(9999999999999999.e+5))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE27]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE27]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE26]], read<bool>(%[[VALUE27]]));
// DEFAULT-NEXT:         let %[[VALUE28:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE26]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE28]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(i192b) -> d64>(%[[VALUE_tests192]], widen<i192b, reason=arg>(const<i71b>(999999999999999949999))));
// DEFAULT-NEXT:             call<d64, signature=fn(i192b) -> d64>(%[[VALUE_tests192]], widen<i192b, reason=arg>(const<i71b>(999999999999999949999)));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], const<d64>(9999999999999999.e+5));
// DEFAULT-NEXT:             let %[[VALUE29:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(9999999999999999.e+5))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE29]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE29]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE28]], read<bool>(%[[VALUE29]]));
// DEFAULT-NEXT:         let %[[VALUE30:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE28]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE30]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(i192b) -> d64>(%[[VALUE_tests192]], widen<i192b, reason=arg>(neg<i75b, overflow=ub>(const<i75b>(9999999999999999000000)))));
// DEFAULT-NEXT:             call<d64, signature=fn(i192b) -> d64>(%[[VALUE_tests192]], widen<i192b, reason=arg>(neg<i75b, overflow=ub>(const<i75b>(9999999999999999000000))));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], neg<d64>(const<d64>(9999999999999999.e+6)));
// DEFAULT-NEXT:             let %[[VALUE31:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), neg<d64>(const<d64>(9999999999999999.e+6)))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE31]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE31]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE30]], read<bool>(%[[VALUE31]]));
// DEFAULT-NEXT:         let %[[VALUE32:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE30]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE32]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(i192b) -> d64>(%[[VALUE_tests192]], widen<i192b, reason=arg>(neg<i75b, overflow=ub>(const<i75b>(9999999999999999499999)))));
// DEFAULT-NEXT:             call<d64, signature=fn(i192b) -> d64>(%[[VALUE_tests192]], widen<i192b, reason=arg>(neg<i75b, overflow=ub>(const<i75b>(9999999999999999499999))));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], neg<d64>(const<d64>(9999999999999999.e+6)));
// DEFAULT-NEXT:             let %[[VALUE33:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), neg<d64>(const<d64>(9999999999999999.e+6)))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE33]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE33]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE32]], read<bool>(%[[VALUE33]]));
// DEFAULT-NEXT:         let %[[VALUE34:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE32]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE34]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(i192b) -> d64>(%[[VALUE_tests192]], widen<i192b, reason=arg>(const<i78b>(123456789012345600000000))));
// DEFAULT-NEXT:             call<d64, signature=fn(i192b) -> d64>(%[[VALUE_tests192]], widen<i192b, reason=arg>(const<i78b>(123456789012345600000000)));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], const<d64>(1234567890123456.e+8));
// DEFAULT-NEXT:             let %[[VALUE35:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(1234567890123456.e+8))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE35]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE35]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE34]], read<bool>(%[[VALUE35]]));
// DEFAULT-NEXT:         let %[[VALUE36:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE34]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE36]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(i192b) -> d64>(%[[VALUE_tests192]], widen<i192b, reason=arg>(const<i126b>(34242319854454290000000000000000000000))));
// DEFAULT-NEXT:             call<d64, signature=fn(i192b) -> d64>(%[[VALUE_tests192]], widen<i192b, reason=arg>(const<i126b>(34242319854454290000000000000000000000)));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], const<d64>(3424231985445429e+22));
// DEFAULT-NEXT:             let %[[VALUE37:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(3424231985445429e+22))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE37]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE37]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE36]], read<bool>(%[[VALUE37]]));
// DEFAULT-NEXT:         let %[[VALUE38:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE36]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE38]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(i192b) -> d64>(%[[VALUE_tests192]], widen<i192b, reason=arg>(const<i161b>(999999999999999900000000000000000000000000000000))));
// DEFAULT-NEXT:             call<d64, signature=fn(i192b) -> d64>(%[[VALUE_tests192]], widen<i192b, reason=arg>(const<i161b>(999999999999999900000000000000000000000000000000)));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], const<d64>(9999999999999999.e+32));
// DEFAULT-NEXT:             let %[[VALUE39:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(9999999999999999.e+32))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE39]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE39]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE38]], read<bool>(%[[VALUE39]]));
// DEFAULT-NEXT:         let %[[VALUE40:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE38]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE40]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(i192b) -> d64>(%[[VALUE_tests192]], widen<i192b, reason=arg>(const<i161b>(999999999999999949999999999999999999999999999999))));
// DEFAULT-NEXT:             call<d64, signature=fn(i192b) -> d64>(%[[VALUE_tests192]], widen<i192b, reason=arg>(const<i161b>(999999999999999949999999999999999999999999999999)));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], const<d64>(9999999999999999.e+32));
// DEFAULT-NEXT:             let %[[VALUE41:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(9999999999999999.e+32))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE41]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE41]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE40]], read<bool>(%[[VALUE41]]));
// DEFAULT-NEXT:         let %[[VALUE42:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE40]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE42]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(i192b) -> d64>(%[[VALUE_tests192]], widen<i192b, reason=arg>(neg<i191b, overflow=ub>(const<i191b>(999999999999999900000000000000000000000000000000000000000)))));
// DEFAULT-NEXT:             call<d64, signature=fn(i192b) -> d64>(%[[VALUE_tests192]], widen<i192b, reason=arg>(neg<i191b, overflow=ub>(const<i191b>(999999999999999900000000000000000000000000000000000000000))));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], neg<d64>(const<d64>(9999999999999999.e+41)));
// DEFAULT-NEXT:             let %[[VALUE43:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), neg<d64>(const<d64>(9999999999999999.e+41)))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE43]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE43]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE42]], read<bool>(%[[VALUE43]]));
// DEFAULT-NEXT:         let %[[VALUE44:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE42]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE44]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(i192b) -> d64>(%[[VALUE_tests192]], neg<i192b, overflow=ub>(const<i192b>(2138550877694459000000000000000000000000000000000000000000))));
// DEFAULT-NEXT:             call<d64, signature=fn(i192b) -> d64>(%[[VALUE_tests192]], neg<i192b, overflow=ub>(const<i192b>(2138550877694459000000000000000000000000000000000000000000)));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], neg<d64>(const<d64>(2138550877694459e+42)));
// DEFAULT-NEXT:             let %[[VALUE45:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), neg<d64>(const<d64>(2138550877694459e+42)))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE45]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE45]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE44]], read<bool>(%[[VALUE45]]));
// DEFAULT-NEXT:         let %[[VALUE46:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE44]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE46]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(i192b) -> d64>(%[[VALUE_tests192]], neg<i192b, overflow=ub>(const<i192b>(2138550877694459500000000000000000000000000000000000000000))));
// DEFAULT-NEXT:             call<d64, signature=fn(i192b) -> d64>(%[[VALUE_tests192]], neg<i192b, overflow=ub>(const<i192b>(2138550877694459500000000000000000000000000000000000000000)));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], neg<d64>(const<d64>(2138550877694460e+42)));
// DEFAULT-NEXT:             let %[[VALUE47:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), neg<d64>(const<d64>(2138550877694460e+42)))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE47]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE47]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE46]], read<bool>(%[[VALUE47]]));
// DEFAULT-NEXT:         let %[[VALUE48:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE46]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE48]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(i192b) -> d64>(%[[VALUE_tests192]], neg<i192b, overflow=ub>(const<i192b>(2138550877694459499999999999999999999999999999999999999999))));
// DEFAULT-NEXT:             call<d64, signature=fn(i192b) -> d64>(%[[VALUE_tests192]], neg<i192b, overflow=ub>(const<i192b>(2138550877694459499999999999999999999999999999999999999999)));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], neg<d64>(const<d64>(2138550877694459e+42)));
// DEFAULT-NEXT:             let %[[VALUE49:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), neg<d64>(const<d64>(2138550877694459e+42)))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE49]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE49]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE48]], read<bool>(%[[VALUE49]]));
// DEFAULT-NEXT:         let %[[VALUE50:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE48]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE50]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(i192b) -> d64>(%[[VALUE_tests192]], neg<i192b, overflow=ub>(const<i192b>(2138550877694459999999999999999999999999999999999999999999))));
// DEFAULT-NEXT:             call<d64, signature=fn(i192b) -> d64>(%[[VALUE_tests192]], neg<i192b, overflow=ub>(const<i192b>(2138550877694459999999999999999999999999999999999999999999)));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], neg<d64>(const<d64>(2138550877694460e+42)));
// DEFAULT-NEXT:             let %[[VALUE51:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), neg<d64>(const<d64>(2138550877694460e+42)))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE51]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE51]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE50]], read<bool>(%[[VALUE51]]));
// DEFAULT-NEXT:         let %[[VALUE52:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE50]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE52]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(i192b) -> d64>(%[[VALUE_tests192]], neg<i192b, overflow=ub>(const<i192b>(2138550877694458000000000000000000000000000000000000000000))));
// DEFAULT-NEXT:             call<d64, signature=fn(i192b) -> d64>(%[[VALUE_tests192]], neg<i192b, overflow=ub>(const<i192b>(2138550877694458000000000000000000000000000000000000000000)));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], neg<d64>(const<d64>(2138550877694458e+42)));
// DEFAULT-NEXT:             let %[[VALUE53:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), neg<d64>(const<d64>(2138550877694458e+42)))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE53]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE53]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE52]], read<bool>(%[[VALUE53]]));
// DEFAULT-NEXT:         let %[[VALUE54:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE52]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE54]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(i192b) -> d64>(%[[VALUE_tests192]], neg<i192b, overflow=ub>(const<i192b>(2138550877694458500000000000000000000000000000000000000000))));
// DEFAULT-NEXT:             call<d64, signature=fn(i192b) -> d64>(%[[VALUE_tests192]], neg<i192b, overflow=ub>(const<i192b>(2138550877694458500000000000000000000000000000000000000000)));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], neg<d64>(const<d64>(2138550877694458e+42)));
// DEFAULT-NEXT:             let %[[VALUE55:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), neg<d64>(const<d64>(2138550877694458e+42)))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE55]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE55]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE54]], read<bool>(%[[VALUE55]]));
// DEFAULT-NEXT:         let %[[VALUE56:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE54]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE56]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(i192b) -> d64>(%[[VALUE_tests192]], neg<i192b, overflow=ub>(const<i192b>(2138550877694458500000000000000000000000000000000000000001))));
// DEFAULT-NEXT:             call<d64, signature=fn(i192b) -> d64>(%[[VALUE_tests192]], neg<i192b, overflow=ub>(const<i192b>(2138550877694458500000000000000000000000000000000000000001)));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], neg<d64>(const<d64>(2138550877694459e+42)));
// DEFAULT-NEXT:             let %[[VALUE57:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), neg<d64>(const<d64>(2138550877694459e+42)))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE57]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE57]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE56]], read<bool>(%[[VALUE57]]));
// DEFAULT-NEXT:         let %[[VALUE58:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE56]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE58]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(i192b) -> d64>(%[[VALUE_tests192]], const<i192b>(3138550867693340000000000000000000000000000000000000000000)));
// DEFAULT-NEXT:             call<d64, signature=fn(i192b) -> d64>(%[[VALUE_tests192]], const<i192b>(3138550867693340000000000000000000000000000000000000000000));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], const<d64>(3138550867693340e+42));
// DEFAULT-NEXT:             let %[[VALUE59:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(3138550867693340e+42))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE59]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE59]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE58]], read<bool>(%[[VALUE59]]));
// DEFAULT-NEXT:         let %[[VALUE60:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE58]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE60]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(i192b) -> d64>(%[[VALUE_tests192]], const<i192b>(3138550867693340381917894711603833208051177722232017256447)));
// DEFAULT-NEXT:             call<d64, signature=fn(i192b) -> d64>(%[[VALUE_tests192]], const<i192b>(3138550867693340381917894711603833208051177722232017256447));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], const<d64>(3138550867693340e+42));
// DEFAULT-NEXT:             let %[[VALUE61:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(3138550867693340e+42))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE61]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE61]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE60]], read<bool>(%[[VALUE61]]));
// DEFAULT-NEXT:         let %[[VALUE62:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE60]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE62]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(i192b) -> d64>(%[[VALUE_tests192]], neg<i192b, overflow=ub>(const<i192b>(3138550867693340000000000000000000000000000000000000000000))));
// DEFAULT-NEXT:             call<d64, signature=fn(i192b) -> d64>(%[[VALUE_tests192]], neg<i192b, overflow=ub>(const<i192b>(3138550867693340000000000000000000000000000000000000000000)));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], neg<d64>(const<d64>(3138550867693340e+42)));
// DEFAULT-NEXT:             let %[[VALUE63:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), neg<d64>(const<d64>(3138550867693340e+42)))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE63]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE63]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE62]], read<bool>(%[[VALUE63]]));
// DEFAULT-NEXT:         let %[[VALUE64:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE62]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE64]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(i192b) -> d64>(%[[VALUE_tests192]], sub<i192b, overflow=ub>(neg<i192b, overflow=ub>(const<i192b>(3138550867693340381917894711603833208051177722232017256447)), widen<i192b, reason=usual_arith>(const<i2b>(1)))));
// DEFAULT-NEXT:             call<d64, signature=fn(i192b) -> d64>(%[[VALUE_tests192]], sub<i192b, overflow=ub>(neg<i192b, overflow=ub>(const<i192b>(3138550867693340381917894711603833208051177722232017256447)), widen<i192b, reason=usual_arith>(const<i2b>(1))));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], neg<d64>(const<d64>(3138550867693340e+42)));
// DEFAULT-NEXT:             let %[[VALUE65:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), neg<d64>(const<d64>(3138550867693340e+42)))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE65]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE65]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE64]], read<bool>(%[[VALUE65]]));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE64]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         write<d64>(%[[VALUE_a]], call<d64, signature=fn(u192b) -> d64>(%[[VALUE_testu192]], widen<u192b, reason=arg>(const<u1b>(0))));
// DEFAULT-NEXT:         call<d64, signature=fn(u192b) -> d64>(%[[VALUE_testu192]], widen<u192b, reason=arg>(const<u1b>(0)));
// DEFAULT-NEXT:         write<d64>(%[[VALUE_b_5]], const<d64>(0.));
// DEFAULT-NEXT:         let %[[VALUE66:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(0.))
// DEFAULT-NEXT:             write<bool>(%[[VALUE66]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE66]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE67:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE66]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE67]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(u192b) -> d64>(%[[VALUE_testu192]], widen<u192b, reason=arg>(const<u3b>(7))));
// DEFAULT-NEXT:             call<d64, signature=fn(u192b) -> d64>(%[[VALUE_testu192]], widen<u192b, reason=arg>(const<u3b>(7)));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], const<d64>(7.));
// DEFAULT-NEXT:             let %[[VALUE68:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(7.))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE68]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE68]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE67]], read<bool>(%[[VALUE68]]));
// DEFAULT-NEXT:         let %[[VALUE69:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE67]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE69]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(u192b) -> d64>(%[[VALUE_testu192]], widen<u192b, reason=arg>(const<u6b>(42))));
// DEFAULT-NEXT:             call<d64, signature=fn(u192b) -> d64>(%[[VALUE_testu192]], widen<u192b, reason=arg>(const<u6b>(42)));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], const<d64>(42.));
// DEFAULT-NEXT:             let %[[VALUE70:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(42.))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE70]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE70]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE69]], read<bool>(%[[VALUE70]]));
// DEFAULT-NEXT:         let %[[VALUE71:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE69]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE71]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(u192b) -> d64>(%[[VALUE_testu192]], widen<u192b, reason=arg>(const<u30b>(777777777))));
// DEFAULT-NEXT:             call<d64, signature=fn(u192b) -> d64>(%[[VALUE_testu192]], widen<u192b, reason=arg>(const<u30b>(777777777)));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], const<d64>(777777777.));
// DEFAULT-NEXT:             let %[[VALUE72:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(777777777.))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE72]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE72]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE71]], read<bool>(%[[VALUE72]]));
// DEFAULT-NEXT:         let %[[VALUE73:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE71]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE73]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(u192b) -> d64>(%[[VALUE_testu192]], widen<u192b, reason=arg>(const<u54b>(9999999999999000))));
// DEFAULT-NEXT:             call<d64, signature=fn(u192b) -> d64>(%[[VALUE_testu192]], widen<u192b, reason=arg>(const<u54b>(9999999999999000)));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], const<d64>(9999999999999000.));
// DEFAULT-NEXT:             let %[[VALUE74:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(9999999999999000.))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE74]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE74]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE73]], read<bool>(%[[VALUE74]]));
// DEFAULT-NEXT:         let %[[VALUE75:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE73]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE75]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(u192b) -> d64>(%[[VALUE_testu192]], widen<u192b, reason=arg>(const<u60b>(999999999999999900))));
// DEFAULT-NEXT:             call<d64, signature=fn(u192b) -> d64>(%[[VALUE_testu192]], widen<u192b, reason=arg>(const<u60b>(999999999999999900)));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], const<d64>(9999999999999999.e+2));
// DEFAULT-NEXT:             let %[[VALUE76:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(9999999999999999.e+2))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE76]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE76]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE75]], read<bool>(%[[VALUE76]]));
// DEFAULT-NEXT:         let %[[VALUE77:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE75]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE77]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(u192b) -> d64>(%[[VALUE_testu192]], widen<u192b, reason=arg>(const<u64b>(9999999999999999000))));
// DEFAULT-NEXT:             call<d64, signature=fn(u192b) -> d64>(%[[VALUE_testu192]], widen<u192b, reason=arg>(const<u64b>(9999999999999999000)));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], const<d64>(9999999999999999.e+3));
// DEFAULT-NEXT:             let %[[VALUE78:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(9999999999999999.e+3))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE78]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE78]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE77]], read<bool>(%[[VALUE78]]));
// DEFAULT-NEXT:         let %[[VALUE79:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE77]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE79]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(u192b) -> d64>(%[[VALUE_testu192]], widen<u192b, reason=arg>(const<u67b>(99999999999999994999))));
// DEFAULT-NEXT:             call<d64, signature=fn(u192b) -> d64>(%[[VALUE_testu192]], widen<u192b, reason=arg>(const<u67b>(99999999999999994999)));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], const<d64>(9999999999999999.e+4));
// DEFAULT-NEXT:             let %[[VALUE80:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(9999999999999999.e+4))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE80]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE80]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE79]], read<bool>(%[[VALUE80]]));
// DEFAULT-NEXT:         let %[[VALUE81:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE79]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE81]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(u192b) -> d64>(%[[VALUE_testu192]], widen<u192b, reason=arg>(const<u70b>(999999999999999900000))));
// DEFAULT-NEXT:             call<d64, signature=fn(u192b) -> d64>(%[[VALUE_testu192]], widen<u192b, reason=arg>(const<u70b>(999999999999999900000)));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], const<d64>(9999999999999999.e+5));
// DEFAULT-NEXT:             let %[[VALUE82:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(9999999999999999.e+5))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE82]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE82]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE81]], read<bool>(%[[VALUE82]]));
// DEFAULT-NEXT:         let %[[VALUE83:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE81]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE83]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(u192b) -> d64>(%[[VALUE_testu192]], widen<u192b, reason=arg>(const<u74b>(9999999999999999000000))));
// DEFAULT-NEXT:             call<d64, signature=fn(u192b) -> d64>(%[[VALUE_testu192]], widen<u192b, reason=arg>(const<u74b>(9999999999999999000000)));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], const<d64>(9999999999999999.e+6));
// DEFAULT-NEXT:             let %[[VALUE84:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(9999999999999999.e+6))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE84]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE84]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE83]], read<bool>(%[[VALUE84]]));
// DEFAULT-NEXT:         let %[[VALUE85:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE83]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE85]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(u192b) -> d64>(%[[VALUE_testu192]], widen<u192b, reason=arg>(const<u77b>(123456789012345600000000))));
// DEFAULT-NEXT:             call<d64, signature=fn(u192b) -> d64>(%[[VALUE_testu192]], widen<u192b, reason=arg>(const<u77b>(123456789012345600000000)));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], const<d64>(1234567890123456.e+8));
// DEFAULT-NEXT:             let %[[VALUE86:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(1234567890123456.e+8))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE86]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE86]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE85]], read<bool>(%[[VALUE86]]));
// DEFAULT-NEXT:         let %[[VALUE87:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE85]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE87]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(u192b) -> d64>(%[[VALUE_testu192]], widen<u192b, reason=arg>(const<u125b>(34242319854454290000000000000000000000))));
// DEFAULT-NEXT:             call<d64, signature=fn(u192b) -> d64>(%[[VALUE_testu192]], widen<u192b, reason=arg>(const<u125b>(34242319854454290000000000000000000000)));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], const<d64>(3424231985445429e+22));
// DEFAULT-NEXT:             let %[[VALUE88:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(3424231985445429e+22))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE88]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE88]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE87]], read<bool>(%[[VALUE88]]));
// DEFAULT-NEXT:         let %[[VALUE89:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE87]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE89]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(u192b) -> d64>(%[[VALUE_testu192]], widen<u192b, reason=arg>(const<u163b>(9999999999999999000000000000000000000000000000000))));
// DEFAULT-NEXT:             call<d64, signature=fn(u192b) -> d64>(%[[VALUE_testu192]], widen<u192b, reason=arg>(const<u163b>(9999999999999999000000000000000000000000000000000)));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], const<d64>(9999999999999999.e+33));
// DEFAULT-NEXT:             let %[[VALUE90:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(9999999999999999.e+33))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE90]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE90]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE89]], read<bool>(%[[VALUE90]]));
// DEFAULT-NEXT:         let %[[VALUE91:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE89]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE91]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(u192b) -> d64>(%[[VALUE_testu192]], widen<u192b, reason=arg>(const<u179b>(618935436546517900000000000000000000000000000000000000))));
// DEFAULT-NEXT:             call<d64, signature=fn(u192b) -> d64>(%[[VALUE_testu192]], widen<u192b, reason=arg>(const<u179b>(618935436546517900000000000000000000000000000000000000)));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], const<d64>(6189354365465179e+38));
// DEFAULT-NEXT:             let %[[VALUE92:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(6189354365465179e+38))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE92]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE92]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE91]], read<bool>(%[[VALUE92]]));
// DEFAULT-NEXT:         let %[[VALUE93:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE91]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE93]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(u192b) -> d64>(%[[VALUE_testu192]], widen<u192b, reason=arg>(const<u179b>(618935436546517950000000000000000000000000000000000000))));
// DEFAULT-NEXT:             call<d64, signature=fn(u192b) -> d64>(%[[VALUE_testu192]], widen<u192b, reason=arg>(const<u179b>(618935436546517950000000000000000000000000000000000000)));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], const<d64>(6189354365465180e+38));
// DEFAULT-NEXT:             let %[[VALUE94:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(6189354365465180e+38))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE94]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE94]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE93]], read<bool>(%[[VALUE94]]));
// DEFAULT-NEXT:         let %[[VALUE95:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE93]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE95]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(u192b) -> d64>(%[[VALUE_testu192]], widen<u192b, reason=arg>(const<u179b>(618935436546517949999999999999999999999999999999999999))));
// DEFAULT-NEXT:             call<d64, signature=fn(u192b) -> d64>(%[[VALUE_testu192]], widen<u192b, reason=arg>(const<u179b>(618935436546517949999999999999999999999999999999999999)));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], const<d64>(6189354365465179e+38));
// DEFAULT-NEXT:             let %[[VALUE96:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(6189354365465179e+38))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE96]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE96]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE95]], read<bool>(%[[VALUE96]]));
// DEFAULT-NEXT:         let %[[VALUE97:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE95]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE97]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(u192b) -> d64>(%[[VALUE_testu192]], widen<u192b, reason=arg>(const<u179b>(618935436546517999999999999999999999999999999999999999))));
// DEFAULT-NEXT:             call<d64, signature=fn(u192b) -> d64>(%[[VALUE_testu192]], widen<u192b, reason=arg>(const<u179b>(618935436546517999999999999999999999999999999999999999)));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], const<d64>(6189354365465180e+38));
// DEFAULT-NEXT:             let %[[VALUE98:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(6189354365465180e+38))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE98]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE98]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE97]], read<bool>(%[[VALUE98]]));
// DEFAULT-NEXT:         let %[[VALUE99:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE97]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE99]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(u192b) -> d64>(%[[VALUE_testu192]], widen<u192b, reason=arg>(const<u179b>(618935436546517800000000000000000000000000000000000000))));
// DEFAULT-NEXT:             call<d64, signature=fn(u192b) -> d64>(%[[VALUE_testu192]], widen<u192b, reason=arg>(const<u179b>(618935436546517800000000000000000000000000000000000000)));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], const<d64>(6189354365465178e+38));
// DEFAULT-NEXT:             let %[[VALUE100:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(6189354365465178e+38))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE100]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE100]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE99]], read<bool>(%[[VALUE100]]));
// DEFAULT-NEXT:         let %[[VALUE101:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE99]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE101]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(u192b) -> d64>(%[[VALUE_testu192]], widen<u192b, reason=arg>(const<u179b>(618935436546517850000000000000000000000000000000000000))));
// DEFAULT-NEXT:             call<d64, signature=fn(u192b) -> d64>(%[[VALUE_testu192]], widen<u192b, reason=arg>(const<u179b>(618935436546517850000000000000000000000000000000000000)));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], const<d64>(6189354365465178e+38));
// DEFAULT-NEXT:             let %[[VALUE102:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(6189354365465178e+38))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE102]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE102]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE101]], read<bool>(%[[VALUE102]]));
// DEFAULT-NEXT:         let %[[VALUE103:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE101]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE103]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(u192b) -> d64>(%[[VALUE_testu192]], widen<u192b, reason=arg>(const<u179b>(618935436546517850000000000000000000000000000000000001))));
// DEFAULT-NEXT:             call<d64, signature=fn(u192b) -> d64>(%[[VALUE_testu192]], widen<u192b, reason=arg>(const<u179b>(618935436546517850000000000000000000000000000000000001)));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], const<d64>(6189354365465179e+38));
// DEFAULT-NEXT:             let %[[VALUE104:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(6189354365465179e+38))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE104]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE104]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE103]], read<bool>(%[[VALUE104]]));
// DEFAULT-NEXT:         let %[[VALUE105:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE103]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE105]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(u192b) -> d64>(%[[VALUE_testu192]], widen<u192b, reason=arg>(const<u187b>(99999999999999990000000000000000000000000000000000000000))));
// DEFAULT-NEXT:             call<d64, signature=fn(u192b) -> d64>(%[[VALUE_testu192]], widen<u192b, reason=arg>(const<u187b>(99999999999999990000000000000000000000000000000000000000)));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], const<d64>(9999999999999999.e+40));
// DEFAULT-NEXT:             let %[[VALUE106:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(9999999999999999.e+40))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE106]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE106]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE105]], read<bool>(%[[VALUE106]]));
// DEFAULT-NEXT:         let %[[VALUE107:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE105]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE107]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(u192b) -> d64>(%[[VALUE_testu192]], const<u192b>(6277101735386680000000000000000000000000000000000000000000)));
// DEFAULT-NEXT:             call<d64, signature=fn(u192b) -> d64>(%[[VALUE_testu192]], const<u192b>(6277101735386680000000000000000000000000000000000000000000));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], const<d64>(6277101735386680e+42));
// DEFAULT-NEXT:             let %[[VALUE108:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(6277101735386680e+42))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE108]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE108]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE107]], read<bool>(%[[VALUE108]]));
// DEFAULT-NEXT:         let %[[VALUE109:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE107]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE109]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(u192b) -> d64>(%[[VALUE_testu192]], const<u192b>(6277101735386680499999999999999999999999999999999999999999)));
// DEFAULT-NEXT:             call<d64, signature=fn(u192b) -> d64>(%[[VALUE_testu192]], const<u192b>(6277101735386680499999999999999999999999999999999999999999));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], const<d64>(6277101735386680e+42));
// DEFAULT-NEXT:             let %[[VALUE110:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(6277101735386680e+42))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE110]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE110]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE109]], read<bool>(%[[VALUE110]]));
// DEFAULT-NEXT:         let %[[VALUE111:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE109]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE111]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(u192b) -> d64>(%[[VALUE_testu192]], const<u192b>(6277101735386680500000000000000000000000000000000000000000)));
// DEFAULT-NEXT:             call<d64, signature=fn(u192b) -> d64>(%[[VALUE_testu192]], const<u192b>(6277101735386680500000000000000000000000000000000000000000));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], const<d64>(6277101735386680e+42));
// DEFAULT-NEXT:             let %[[VALUE112:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(6277101735386680e+42))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE112]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE112]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE111]], read<bool>(%[[VALUE112]]));
// DEFAULT-NEXT:         let %[[VALUE113:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE111]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE113]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(u192b) -> d64>(%[[VALUE_testu192]], const<u192b>(6277101735386680500000000000000000000000000000000000000001)));
// DEFAULT-NEXT:             call<d64, signature=fn(u192b) -> d64>(%[[VALUE_testu192]], const<u192b>(6277101735386680500000000000000000000000000000000000000001));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], const<d64>(6277101735386681e+42));
// DEFAULT-NEXT:             let %[[VALUE114:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(6277101735386681e+42))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE114]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE114]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE113]], read<bool>(%[[VALUE114]]));
// DEFAULT-NEXT:         let %[[VALUE115:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE113]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE115]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(u192b) -> d64>(%[[VALUE_testu192]], const<u192b>(6277101735386680500000000000000000000010000000000000000000)));
// DEFAULT-NEXT:             call<d64, signature=fn(u192b) -> d64>(%[[VALUE_testu192]], const<u192b>(6277101735386680500000000000000000000010000000000000000000));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], const<d64>(6277101735386681e+42));
// DEFAULT-NEXT:             let %[[VALUE116:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(6277101735386681e+42))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE116]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE116]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE115]], read<bool>(%[[VALUE116]]));
// DEFAULT-NEXT:         let %[[VALUE117:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE115]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE117]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(u192b) -> d64>(%[[VALUE_testu192]], const<u192b>(6277101735386680763835789423207666416102355444464034512895)));
// DEFAULT-NEXT:             call<d64, signature=fn(u192b) -> d64>(%[[VALUE_testu192]], const<u192b>(6277101735386680763835789423207666416102355444464034512895));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], const<d64>(6277101735386681e+42));
// DEFAULT-NEXT:             let %[[VALUE118:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(6277101735386681e+42))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE118]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE118]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE117]], read<bool>(%[[VALUE118]]));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE117]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         write<d64>(%[[VALUE_a]], call<d64, signature=fn(i575b) -> d64>(%[[VALUE_tests575]], widen<i575b, reason=arg>(const<i2b>(0))));
// DEFAULT-NEXT:         call<d64, signature=fn(i575b) -> d64>(%[[VALUE_tests575]], widen<i575b, reason=arg>(const<i2b>(0)));
// DEFAULT-NEXT:         write<d64>(%[[VALUE_b_5]], const<d64>(0.));
// DEFAULT-NEXT:         let %[[VALUE119:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(0.))
// DEFAULT-NEXT:             write<bool>(%[[VALUE119]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE119]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE120:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE119]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE120]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(i575b) -> d64>(%[[VALUE_tests575]], widen<i575b, reason=arg>(const<i4b>(7))));
// DEFAULT-NEXT:             call<d64, signature=fn(i575b) -> d64>(%[[VALUE_tests575]], widen<i575b, reason=arg>(const<i4b>(7)));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], const<d64>(7.));
// DEFAULT-NEXT:             let %[[VALUE121:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(7.))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE121]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE121]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE120]], read<bool>(%[[VALUE121]]));
// DEFAULT-NEXT:         let %[[VALUE122:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE120]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE122]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(i575b) -> d64>(%[[VALUE_tests575]], widen<i575b, reason=arg>(neg<i7b, overflow=ub>(const<i7b>(42)))));
// DEFAULT-NEXT:             call<d64, signature=fn(i575b) -> d64>(%[[VALUE_tests575]], widen<i575b, reason=arg>(neg<i7b, overflow=ub>(const<i7b>(42))));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], neg<d64>(const<d64>(42.)));
// DEFAULT-NEXT:             let %[[VALUE123:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), neg<d64>(const<d64>(42.)))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE123]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE123]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE122]], read<bool>(%[[VALUE123]]));
// DEFAULT-NEXT:         let %[[VALUE124:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE122]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE124]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(i575b) -> d64>(%[[VALUE_tests575]], widen<i575b, reason=arg>(neg<i30b, overflow=ub>(const<i30b>(444444444)))));
// DEFAULT-NEXT:             call<d64, signature=fn(i575b) -> d64>(%[[VALUE_tests575]], widen<i575b, reason=arg>(neg<i30b, overflow=ub>(const<i30b>(444444444))));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], neg<d64>(const<d64>(444444444.)));
// DEFAULT-NEXT:             let %[[VALUE125:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), neg<d64>(const<d64>(444444444.)))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE125]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE125]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE124]], read<bool>(%[[VALUE125]]));
// DEFAULT-NEXT:         let %[[VALUE126:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE124]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE126]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(i575b) -> d64>(%[[VALUE_tests575]], widen<i575b, reason=arg>(const<i55b>(9999999999999000))));
// DEFAULT-NEXT:             call<d64, signature=fn(i575b) -> d64>(%[[VALUE_tests575]], widen<i575b, reason=arg>(const<i55b>(9999999999999000)));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], const<d64>(9999999999999000.));
// DEFAULT-NEXT:             let %[[VALUE127:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(9999999999999000.))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE127]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE127]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE126]], read<bool>(%[[VALUE127]]));
// DEFAULT-NEXT:         let %[[VALUE128:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE126]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE128]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(i575b) -> d64>(%[[VALUE_tests575]], widen<i575b, reason=arg>(neg<i55b, overflow=ub>(const<i55b>(9999999999999999)))));
// DEFAULT-NEXT:             call<d64, signature=fn(i575b) -> d64>(%[[VALUE_tests575]], widen<i575b, reason=arg>(neg<i55b, overflow=ub>(const<i55b>(9999999999999999))));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], neg<d64>(const<d64>(9999999999999999.)));
// DEFAULT-NEXT:             let %[[VALUE129:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), neg<d64>(const<d64>(9999999999999999.)))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE129]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE129]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE128]], read<bool>(%[[VALUE129]]));
// DEFAULT-NEXT:         let %[[VALUE130:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE128]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE130]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(i575b) -> d64>(%[[VALUE_tests575]], widen<i575b, reason=arg>(const<i61b>(999999999999999900))));
// DEFAULT-NEXT:             call<d64, signature=fn(i575b) -> d64>(%[[VALUE_tests575]], widen<i575b, reason=arg>(const<i61b>(999999999999999900)));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], const<d64>(9999999999999999.e+2));
// DEFAULT-NEXT:             let %[[VALUE131:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(9999999999999999.e+2))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE131]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE131]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE130]], read<bool>(%[[VALUE131]]));
// DEFAULT-NEXT:         let %[[VALUE132:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE130]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE132]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(i575b) -> d64>(%[[VALUE_tests575]], widen<i575b, reason=arg>(neg<i65b, overflow=ub>(const<i65b>(9999999999999999000)))));
// DEFAULT-NEXT:             call<d64, signature=fn(i575b) -> d64>(%[[VALUE_tests575]], widen<i575b, reason=arg>(neg<i65b, overflow=ub>(const<i65b>(9999999999999999000))));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], neg<d64>(const<d64>(9999999999999999.e+3)));
// DEFAULT-NEXT:             let %[[VALUE133:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), neg<d64>(const<d64>(9999999999999999.e+3)))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE133]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE133]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE132]], read<bool>(%[[VALUE133]]));
// DEFAULT-NEXT:         let %[[VALUE134:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE132]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE134]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(i575b) -> d64>(%[[VALUE_tests575]], widen<i575b, reason=arg>(const<i71b>(999999999999999900000))));
// DEFAULT-NEXT:             call<d64, signature=fn(i575b) -> d64>(%[[VALUE_tests575]], widen<i575b, reason=arg>(const<i71b>(999999999999999900000)));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], const<d64>(9999999999999999.e+5));
// DEFAULT-NEXT:             let %[[VALUE135:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(9999999999999999.e+5))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE135]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE135]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE134]], read<bool>(%[[VALUE135]]));
// DEFAULT-NEXT:         let %[[VALUE136:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE134]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE136]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(i575b) -> d64>(%[[VALUE_tests575]], widen<i575b, reason=arg>(neg<i78b, overflow=ub>(const<i78b>(99999999999999990000000)))));
// DEFAULT-NEXT:             call<d64, signature=fn(i575b) -> d64>(%[[VALUE_tests575]], widen<i575b, reason=arg>(neg<i78b, overflow=ub>(const<i78b>(99999999999999990000000))));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], neg<d64>(const<d64>(9999999999999999.e+7)));
// DEFAULT-NEXT:             let %[[VALUE137:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), neg<d64>(const<d64>(9999999999999999.e+7)))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE137]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE137]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE136]], read<bool>(%[[VALUE137]]));
// DEFAULT-NEXT:         let %[[VALUE138:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE136]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE138]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(i575b) -> d64>(%[[VALUE_tests575]], widen<i575b, reason=arg>(const<i82b>(1234567890123456000000000))));
// DEFAULT-NEXT:             call<d64, signature=fn(i575b) -> d64>(%[[VALUE_tests575]], widen<i575b, reason=arg>(const<i82b>(1234567890123456000000000)));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], const<d64>(1234567890123456.e+9));
// DEFAULT-NEXT:             let %[[VALUE139:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(1234567890123456.e+9))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE139]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE139]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE138]], read<bool>(%[[VALUE139]]));
// DEFAULT-NEXT:         let %[[VALUE140:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE138]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE140]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(i575b) -> d64>(%[[VALUE_tests575]], widen<i575b, reason=arg>(const<i133b>(3424231985445429000000000000000000000000))));
// DEFAULT-NEXT:             call<d64, signature=fn(i575b) -> d64>(%[[VALUE_tests575]], widen<i575b, reason=arg>(const<i133b>(3424231985445429000000000000000000000000)));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], const<d64>(3424231985445429e+24));
// DEFAULT-NEXT:             let %[[VALUE141:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(3424231985445429e+24))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE141]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE141]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE140]], read<bool>(%[[VALUE141]]));
// DEFAULT-NEXT:         let %[[VALUE142:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE140]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE142]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(i575b) -> d64>(%[[VALUE_tests575]], widen<i575b, reason=arg>(const<i188b>(99999999999999990000000000000000000000000000000000000000))));
// DEFAULT-NEXT:             call<d64, signature=fn(i575b) -> d64>(%[[VALUE_tests575]], widen<i575b, reason=arg>(const<i188b>(99999999999999990000000000000000000000000000000000000000)));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], const<d64>(9999999999999999.e+40));
// DEFAULT-NEXT:             let %[[VALUE143:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(9999999999999999.e+40))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE143]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE143]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE142]], read<bool>(%[[VALUE143]]));
// DEFAULT-NEXT:         let %[[VALUE144:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE142]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE144]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(i575b) -> d64>(%[[VALUE_tests575]], widen<i575b, reason=arg>(neg<i264b, overflow=ub>(const<i264b>(9999999999999999000000000000000000000000000000000000000000000000000000000000000)))));
// DEFAULT-NEXT:             call<d64, signature=fn(i575b) -> d64>(%[[VALUE_tests575]], widen<i575b, reason=arg>(neg<i264b, overflow=ub>(const<i264b>(9999999999999999000000000000000000000000000000000000000000000000000000000000000))));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], neg<d64>(const<d64>(9999999999999999.e+63)));
// DEFAULT-NEXT:             let %[[VALUE145:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), neg<d64>(const<d64>(9999999999999999.e+63)))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE145]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE145]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE144]], read<bool>(%[[VALUE145]]));
// DEFAULT-NEXT:         let %[[VALUE146:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE144]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE146]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(i575b) -> d64>(%[[VALUE_tests575]], widen<i575b, reason=arg>(neg<i338b, overflow=ub>(const<i338b>(213855087769445900000000000000000000000000000000000000000000000000000000000000000000000000000000000000)))));
// DEFAULT-NEXT:             call<d64, signature=fn(i575b) -> d64>(%[[VALUE_tests575]], widen<i575b, reason=arg>(neg<i338b, overflow=ub>(const<i338b>(213855087769445900000000000000000000000000000000000000000000000000000000000000000000000000000000000000))));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], neg<d64>(const<d64>(2138550877694459e+86)));
// DEFAULT-NEXT:             let %[[VALUE147:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), neg<d64>(const<d64>(2138550877694459e+86)))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE147]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE147]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE146]], read<bool>(%[[VALUE147]]));
// DEFAULT-NEXT:         let %[[VALUE148:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE146]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE148]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(i575b) -> d64>(%[[VALUE_tests575]], widen<i575b, reason=arg>(neg<i338b, overflow=ub>(const<i338b>(213855087769445950000000000000000000000000000000000000000000000000000000000000000000000000000000000000)))));
// DEFAULT-NEXT:             call<d64, signature=fn(i575b) -> d64>(%[[VALUE_tests575]], widen<i575b, reason=arg>(neg<i338b, overflow=ub>(const<i338b>(213855087769445950000000000000000000000000000000000000000000000000000000000000000000000000000000000000))));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], neg<d64>(const<d64>(2138550877694460e+86)));
// DEFAULT-NEXT:             let %[[VALUE149:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), neg<d64>(const<d64>(2138550877694460e+86)))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE149]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE149]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE148]], read<bool>(%[[VALUE149]]));
// DEFAULT-NEXT:         let %[[VALUE150:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE148]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE150]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(i575b) -> d64>(%[[VALUE_tests575]], widen<i575b, reason=arg>(neg<i338b, overflow=ub>(const<i338b>(213855087769445949999999999999999999999999999999999999999999999999999999999999999999999999999999999999)))));
// DEFAULT-NEXT:             call<d64, signature=fn(i575b) -> d64>(%[[VALUE_tests575]], widen<i575b, reason=arg>(neg<i338b, overflow=ub>(const<i338b>(213855087769445949999999999999999999999999999999999999999999999999999999999999999999999999999999999999))));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], neg<d64>(const<d64>(2138550877694459e+86)));
// DEFAULT-NEXT:             let %[[VALUE151:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), neg<d64>(const<d64>(2138550877694459e+86)))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE151]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE151]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE150]], read<bool>(%[[VALUE151]]));
// DEFAULT-NEXT:         let %[[VALUE152:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE150]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE152]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(i575b) -> d64>(%[[VALUE_tests575]], widen<i575b, reason=arg>(neg<i338b, overflow=ub>(const<i338b>(213855087769445999999999999999999999999999999999999999999999999999999999999999999999999999999999999999)))));
// DEFAULT-NEXT:             call<d64, signature=fn(i575b) -> d64>(%[[VALUE_tests575]], widen<i575b, reason=arg>(neg<i338b, overflow=ub>(const<i338b>(213855087769445999999999999999999999999999999999999999999999999999999999999999999999999999999999999999))));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], neg<d64>(const<d64>(2138550877694460e+86)));
// DEFAULT-NEXT:             let %[[VALUE153:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), neg<d64>(const<d64>(2138550877694460e+86)))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE153]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE153]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE152]], read<bool>(%[[VALUE153]]));
// DEFAULT-NEXT:         let %[[VALUE154:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE152]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE154]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(i575b) -> d64>(%[[VALUE_tests575]], widen<i575b, reason=arg>(neg<i338b, overflow=ub>(const<i338b>(213855087769445800000000000000000000000000000000000000000000000000000000000000000000000000000000000000)))));
// DEFAULT-NEXT:             call<d64, signature=fn(i575b) -> d64>(%[[VALUE_tests575]], widen<i575b, reason=arg>(neg<i338b, overflow=ub>(const<i338b>(213855087769445800000000000000000000000000000000000000000000000000000000000000000000000000000000000000))));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], neg<d64>(const<d64>(2138550877694458e+86)));
// DEFAULT-NEXT:             let %[[VALUE155:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), neg<d64>(const<d64>(2138550877694458e+86)))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE155]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE155]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE154]], read<bool>(%[[VALUE155]]));
// DEFAULT-NEXT:         let %[[VALUE156:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE154]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE156]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(i575b) -> d64>(%[[VALUE_tests575]], widen<i575b, reason=arg>(neg<i338b, overflow=ub>(const<i338b>(213855087769445850000000000000000000000000000000000000000000000000000000000000000000000000000000000000)))));
// DEFAULT-NEXT:             call<d64, signature=fn(i575b) -> d64>(%[[VALUE_tests575]], widen<i575b, reason=arg>(neg<i338b, overflow=ub>(const<i338b>(213855087769445850000000000000000000000000000000000000000000000000000000000000000000000000000000000000))));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], neg<d64>(const<d64>(2138550877694458e+86)));
// DEFAULT-NEXT:             let %[[VALUE157:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), neg<d64>(const<d64>(2138550877694458e+86)))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE157]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE157]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE156]], read<bool>(%[[VALUE157]]));
// DEFAULT-NEXT:         let %[[VALUE158:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE156]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE158]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(i575b) -> d64>(%[[VALUE_tests575]], widen<i575b, reason=arg>(neg<i338b, overflow=ub>(const<i338b>(213855087769445850000000000000000000000000000000000000000000000000000000000000000000000000000000000001)))));
// DEFAULT-NEXT:             call<d64, signature=fn(i575b) -> d64>(%[[VALUE_tests575]], widen<i575b, reason=arg>(neg<i338b, overflow=ub>(const<i338b>(213855087769445850000000000000000000000000000000000000000000000000000000000000000000000000000000000001))));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], neg<d64>(const<d64>(2138550877694459e+86)));
// DEFAULT-NEXT:             let %[[VALUE159:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), neg<d64>(const<d64>(2138550877694459e+86)))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE159]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE159]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE158]], read<bool>(%[[VALUE159]]));
// DEFAULT-NEXT:         let %[[VALUE160:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE158]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE160]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(i575b) -> d64>(%[[VALUE_tests575]], const<i575b>(61832600368276130000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000)));
// DEFAULT-NEXT:             call<d64, signature=fn(i575b) -> d64>(%[[VALUE_tests575]], const<i575b>(61832600368276130000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], const<d64>(6183260036827613e+157));
// DEFAULT-NEXT:             let %[[VALUE161:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(6183260036827613e+157))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE161]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE161]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE160]], read<bool>(%[[VALUE161]]));
// DEFAULT-NEXT:         let %[[VALUE162:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE160]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE162]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(i575b) -> d64>(%[[VALUE_tests575]], const<i575b>(61832600368276133515125630254911797508782837275302959978515764023224306276632966792579100265310761247399417856504034834837841258576687802491886538775473291979151693037174783)));
// DEFAULT-NEXT:             call<d64, signature=fn(i575b) -> d64>(%[[VALUE_tests575]], const<i575b>(61832600368276133515125630254911797508782837275302959978515764023224306276632966792579100265310761247399417856504034834837841258576687802491886538775473291979151693037174783));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], const<d64>(6183260036827613e+157));
// DEFAULT-NEXT:             let %[[VALUE163:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(6183260036827613e+157))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE163]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE163]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE162]], read<bool>(%[[VALUE163]]));
// DEFAULT-NEXT:         let %[[VALUE164:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE162]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE164]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(i575b) -> d64>(%[[VALUE_tests575]], neg<i575b, overflow=ub>(const<i575b>(61832600368276130000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000))));
// DEFAULT-NEXT:             call<d64, signature=fn(i575b) -> d64>(%[[VALUE_tests575]], neg<i575b, overflow=ub>(const<i575b>(61832600368276130000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000)));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], neg<d64>(const<d64>(6183260036827613e+157)));
// DEFAULT-NEXT:             let %[[VALUE165:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), neg<d64>(const<d64>(6183260036827613e+157)))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE165]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE165]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE164]], read<bool>(%[[VALUE165]]));
// DEFAULT-NEXT:         let %[[VALUE166:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE164]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE166]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(i575b) -> d64>(%[[VALUE_tests575]], sub<i575b, overflow=ub>(neg<i575b, overflow=ub>(const<i575b>(61832600368276133515125630254911797508782837275302959978515764023224306276632966792579100265310761247399417856504034834837841258576687802491886538775473291979151693037174783)), widen<i575b, reason=usual_arith>(const<i2b>(1)))));
// DEFAULT-NEXT:             call<d64, signature=fn(i575b) -> d64>(%[[VALUE_tests575]], sub<i575b, overflow=ub>(neg<i575b, overflow=ub>(const<i575b>(61832600368276133515125630254911797508782837275302959978515764023224306276632966792579100265310761247399417856504034834837841258576687802491886538775473291979151693037174783)), widen<i575b, reason=usual_arith>(const<i2b>(1))));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], neg<d64>(const<d64>(6183260036827613e+157)));
// DEFAULT-NEXT:             let %[[VALUE167:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), neg<d64>(const<d64>(6183260036827613e+157)))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE167]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE167]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE166]], read<bool>(%[[VALUE167]]));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE166]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         write<d64>(%[[VALUE_a]], call<d64, signature=fn(u575b) -> d64>(%[[VALUE_testu575]], widen<u575b, reason=arg>(const<u1b>(0))));
// DEFAULT-NEXT:         call<d64, signature=fn(u575b) -> d64>(%[[VALUE_testu575]], widen<u575b, reason=arg>(const<u1b>(0)));
// DEFAULT-NEXT:         write<d64>(%[[VALUE_b_5]], const<d64>(0.));
// DEFAULT-NEXT:         let %[[VALUE168:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(0.))
// DEFAULT-NEXT:             write<bool>(%[[VALUE168]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE168]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE169:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE168]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE169]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(u575b) -> d64>(%[[VALUE_testu575]], widen<u575b, reason=arg>(const<u5b>(17))));
// DEFAULT-NEXT:             call<d64, signature=fn(u575b) -> d64>(%[[VALUE_testu575]], widen<u575b, reason=arg>(const<u5b>(17)));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], const<d64>(17.));
// DEFAULT-NEXT:             let %[[VALUE170:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(17.))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE170]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE170]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE169]], read<bool>(%[[VALUE170]]));
// DEFAULT-NEXT:         let %[[VALUE171:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE169]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE171]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(u575b) -> d64>(%[[VALUE_testu575]], widen<u575b, reason=arg>(const<u9b>(420))));
// DEFAULT-NEXT:             call<d64, signature=fn(u575b) -> d64>(%[[VALUE_testu575]], widen<u575b, reason=arg>(const<u9b>(420)));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], const<d64>(420.));
// DEFAULT-NEXT:             let %[[VALUE172:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(420.))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE172]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE172]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE171]], read<bool>(%[[VALUE172]]));
// DEFAULT-NEXT:         let %[[VALUE173:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE171]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE173]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(u575b) -> d64>(%[[VALUE_testu575]], widen<u575b, reason=arg>(const<u30b>(888888888))));
// DEFAULT-NEXT:             call<d64, signature=fn(u575b) -> d64>(%[[VALUE_testu575]], widen<u575b, reason=arg>(const<u30b>(888888888)));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], const<d64>(888888888.));
// DEFAULT-NEXT:             let %[[VALUE174:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(888888888.))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE174]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE174]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE173]], read<bool>(%[[VALUE174]]));
// DEFAULT-NEXT:         let %[[VALUE175:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE173]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE175]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(u575b) -> d64>(%[[VALUE_testu575]], widen<u575b, reason=arg>(const<u54b>(9999999999999000))));
// DEFAULT-NEXT:             call<d64, signature=fn(u575b) -> d64>(%[[VALUE_testu575]], widen<u575b, reason=arg>(const<u54b>(9999999999999000)));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], const<d64>(9999999999999000.));
// DEFAULT-NEXT:             let %[[VALUE176:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(9999999999999000.))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE176]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE176]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE175]], read<bool>(%[[VALUE176]]));
// DEFAULT-NEXT:         let %[[VALUE177:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE175]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE177]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(u575b) -> d64>(%[[VALUE_testu575]], widen<u575b, reason=arg>(const<u77b>(99999999999999990000000))));
// DEFAULT-NEXT:             call<d64, signature=fn(u575b) -> d64>(%[[VALUE_testu575]], widen<u575b, reason=arg>(const<u77b>(99999999999999990000000)));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], const<d64>(9999999999999999.e+7));
// DEFAULT-NEXT:             let %[[VALUE178:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(9999999999999999.e+7))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE178]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE178]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE177]], read<bool>(%[[VALUE178]]));
// DEFAULT-NEXT:         let %[[VALUE179:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE177]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE179]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(u575b) -> d64>(%[[VALUE_testu575]], widen<u575b, reason=arg>(const<u84b>(9999999999999999000000000))));
// DEFAULT-NEXT:             call<d64, signature=fn(u575b) -> d64>(%[[VALUE_testu575]], widen<u575b, reason=arg>(const<u84b>(9999999999999999000000000)));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], const<d64>(9999999999999999.e+9));
// DEFAULT-NEXT:             let %[[VALUE180:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(9999999999999999.e+9))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE180]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE180]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE179]], read<bool>(%[[VALUE180]]));
// DEFAULT-NEXT:         let %[[VALUE181:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE179]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE181]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(u575b) -> d64>(%[[VALUE_testu575]], widen<u575b, reason=arg>(const<u97b>(99999999999999990000000000000))));
// DEFAULT-NEXT:             call<d64, signature=fn(u575b) -> d64>(%[[VALUE_testu575]], widen<u575b, reason=arg>(const<u97b>(99999999999999990000000000000)));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], const<d64>(9999999999999999.e+13));
// DEFAULT-NEXT:             let %[[VALUE182:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(9999999999999999.e+13))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE182]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE182]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE181]], read<bool>(%[[VALUE182]]));
// DEFAULT-NEXT:         let %[[VALUE183:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE181]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE183]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(u575b) -> d64>(%[[VALUE_testu575]], widen<u575b, reason=arg>(const<u103b>(9999999999999999000000000000000))));
// DEFAULT-NEXT:             call<d64, signature=fn(u575b) -> d64>(%[[VALUE_testu575]], widen<u575b, reason=arg>(const<u103b>(9999999999999999000000000000000)));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], const<d64>(9999999999999999.e+15));
// DEFAULT-NEXT:             let %[[VALUE184:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(9999999999999999.e+15))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE184]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE184]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE183]], read<bool>(%[[VALUE184]]));
// DEFAULT-NEXT:         let %[[VALUE185:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE183]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE185]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(u575b) -> d64>(%[[VALUE_testu575]], widen<u575b, reason=arg>(const<u110b>(1234567890123456000000000000000000))));
// DEFAULT-NEXT:             call<d64, signature=fn(u575b) -> d64>(%[[VALUE_testu575]], widen<u575b, reason=arg>(const<u110b>(1234567890123456000000000000000000)));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], const<d64>(1234567890123456.e+18));
// DEFAULT-NEXT:             let %[[VALUE186:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(1234567890123456.e+18))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE186]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE186]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE185]], read<bool>(%[[VALUE186]]));
// DEFAULT-NEXT:         let %[[VALUE187:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE185]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE187]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(u575b) -> d64>(%[[VALUE_testu575]], widen<u575b, reason=arg>(const<u125b>(34242319854454290000000000000000000000))));
// DEFAULT-NEXT:             call<d64, signature=fn(u575b) -> d64>(%[[VALUE_testu575]], widen<u575b, reason=arg>(const<u125b>(34242319854454290000000000000000000000)));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], const<d64>(3424231985445429e+22));
// DEFAULT-NEXT:             let %[[VALUE188:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(3424231985445429e+22))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE188]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE188]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE187]], read<bool>(%[[VALUE188]]));
// DEFAULT-NEXT:         let %[[VALUE189:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE187]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE189]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(u575b) -> d64>(%[[VALUE_testu575]], widen<u575b, reason=arg>(const<u163b>(9999999999999999000000000000000000000000000000000))));
// DEFAULT-NEXT:             call<d64, signature=fn(u575b) -> d64>(%[[VALUE_testu575]], widen<u575b, reason=arg>(const<u163b>(9999999999999999000000000000000000000000000000000)));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], const<d64>(9999999999999999.e+33));
// DEFAULT-NEXT:             let %[[VALUE190:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(9999999999999999.e+33))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE190]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE190]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE189]], read<bool>(%[[VALUE190]]));
// DEFAULT-NEXT:         let %[[VALUE191:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE189]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE191]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(u575b) -> d64>(%[[VALUE_testu575]], widen<u575b, reason=arg>(const<u398b>(618935436546517900000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000))));
// DEFAULT-NEXT:             call<d64, signature=fn(u575b) -> d64>(%[[VALUE_testu575]], widen<u575b, reason=arg>(const<u398b>(618935436546517900000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000)));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], const<d64>(6189354365465179e+104));
// DEFAULT-NEXT:             let %[[VALUE192:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(6189354365465179e+104))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE192]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE192]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE191]], read<bool>(%[[VALUE192]]));
// DEFAULT-NEXT:         let %[[VALUE193:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE191]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE193]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(u575b) -> d64>(%[[VALUE_testu575]], widen<u575b, reason=arg>(const<u398b>(618935436546517950000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000))));
// DEFAULT-NEXT:             call<d64, signature=fn(u575b) -> d64>(%[[VALUE_testu575]], widen<u575b, reason=arg>(const<u398b>(618935436546517950000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000)));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], const<d64>(6189354365465180e+104));
// DEFAULT-NEXT:             let %[[VALUE194:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(6189354365465180e+104))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE194]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE194]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE193]], read<bool>(%[[VALUE194]]));
// DEFAULT-NEXT:         let %[[VALUE195:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE193]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE195]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(u575b) -> d64>(%[[VALUE_testu575]], widen<u575b, reason=arg>(const<u398b>(618935436546517949999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999))));
// DEFAULT-NEXT:             call<d64, signature=fn(u575b) -> d64>(%[[VALUE_testu575]], widen<u575b, reason=arg>(const<u398b>(618935436546517949999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999)));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], const<d64>(6189354365465179e+104));
// DEFAULT-NEXT:             let %[[VALUE196:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(6189354365465179e+104))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE196]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE196]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE195]], read<bool>(%[[VALUE196]]));
// DEFAULT-NEXT:         let %[[VALUE197:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE195]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE197]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(u575b) -> d64>(%[[VALUE_testu575]], widen<u575b, reason=arg>(const<u398b>(618935436546517999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999))));
// DEFAULT-NEXT:             call<d64, signature=fn(u575b) -> d64>(%[[VALUE_testu575]], widen<u575b, reason=arg>(const<u398b>(618935436546517999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999)));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], const<d64>(6189354365465180e+104));
// DEFAULT-NEXT:             let %[[VALUE198:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(6189354365465180e+104))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE198]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE198]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE197]], read<bool>(%[[VALUE198]]));
// DEFAULT-NEXT:         let %[[VALUE199:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE197]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE199]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(u575b) -> d64>(%[[VALUE_testu575]], widen<u575b, reason=arg>(const<u398b>(618935436546517800000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000))));
// DEFAULT-NEXT:             call<d64, signature=fn(u575b) -> d64>(%[[VALUE_testu575]], widen<u575b, reason=arg>(const<u398b>(618935436546517800000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000)));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], const<d64>(6189354365465178e+104));
// DEFAULT-NEXT:             let %[[VALUE200:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(6189354365465178e+104))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE200]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE200]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE199]], read<bool>(%[[VALUE200]]));
// DEFAULT-NEXT:         let %[[VALUE201:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE199]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE201]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(u575b) -> d64>(%[[VALUE_testu575]], widen<u575b, reason=arg>(const<u398b>(618935436546517850000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000))));
// DEFAULT-NEXT:             call<d64, signature=fn(u575b) -> d64>(%[[VALUE_testu575]], widen<u575b, reason=arg>(const<u398b>(618935436546517850000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000)));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], const<d64>(6189354365465178e+104));
// DEFAULT-NEXT:             let %[[VALUE202:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(6189354365465178e+104))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE202]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE202]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE201]], read<bool>(%[[VALUE202]]));
// DEFAULT-NEXT:         let %[[VALUE203:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE201]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE203]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(u575b) -> d64>(%[[VALUE_testu575]], widen<u575b, reason=arg>(const<u398b>(618935436546517850000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001))));
// DEFAULT-NEXT:             call<d64, signature=fn(u575b) -> d64>(%[[VALUE_testu575]], widen<u575b, reason=arg>(const<u398b>(618935436546517850000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001)));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], const<d64>(6189354365465179e+104));
// DEFAULT-NEXT:             let %[[VALUE204:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(6189354365465179e+104))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE204]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE204]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE203]], read<bool>(%[[VALUE204]]));
// DEFAULT-NEXT:         let %[[VALUE205:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE203]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE205]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(u575b) -> d64>(%[[VALUE_testu575]], widen<u575b, reason=arg>(const<u515b>(99999999999999990000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000))));
// DEFAULT-NEXT:             call<d64, signature=fn(u575b) -> d64>(%[[VALUE_testu575]], widen<u575b, reason=arg>(const<u515b>(99999999999999990000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000)));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], const<d64>(9999999999999999.e+139));
// DEFAULT-NEXT:             let %[[VALUE206:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(9999999999999999.e+139))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE206]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE206]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE205]], read<bool>(%[[VALUE206]]));
// DEFAULT-NEXT:         let %[[VALUE207:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE205]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE207]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(u575b) -> d64>(%[[VALUE_testu575]], const<u575b>(123665200736552200000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000)));
// DEFAULT-NEXT:             call<d64, signature=fn(u575b) -> d64>(%[[VALUE_testu575]], const<u575b>(123665200736552200000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], const<d64>(1236652007365522e+158));
// DEFAULT-NEXT:             let %[[VALUE208:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(1236652007365522e+158))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE208]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE208]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE207]], read<bool>(%[[VALUE208]]));
// DEFAULT-NEXT:         let %[[VALUE209:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE207]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE209]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(u575b) -> d64>(%[[VALUE_testu575]], const<u575b>(123665200736552249999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999)));
// DEFAULT-NEXT:             call<d64, signature=fn(u575b) -> d64>(%[[VALUE_testu575]], const<u575b>(123665200736552249999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], const<d64>(1236652007365522e+158));
// DEFAULT-NEXT:             let %[[VALUE210:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(1236652007365522e+158))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE210]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE210]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE209]], read<bool>(%[[VALUE210]]));
// DEFAULT-NEXT:         let %[[VALUE211:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE209]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE211]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(u575b) -> d64>(%[[VALUE_testu575]], const<u575b>(123665200736552250000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000)));
// DEFAULT-NEXT:             call<d64, signature=fn(u575b) -> d64>(%[[VALUE_testu575]], const<u575b>(123665200736552250000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], const<d64>(1236652007365522e+158));
// DEFAULT-NEXT:             let %[[VALUE212:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(1236652007365522e+158))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE212]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE212]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE211]], read<bool>(%[[VALUE212]]));
// DEFAULT-NEXT:         let %[[VALUE213:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE211]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE213]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(u575b) -> d64>(%[[VALUE_testu575]], const<u575b>(123665200736552250000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001)));
// DEFAULT-NEXT:             call<d64, signature=fn(u575b) -> d64>(%[[VALUE_testu575]], const<u575b>(123665200736552250000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], const<d64>(1236652007365523e+158));
// DEFAULT-NEXT:             let %[[VALUE214:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(1236652007365523e+158))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE214]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE214]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE213]], read<bool>(%[[VALUE214]]));
// DEFAULT-NEXT:         let %[[VALUE215:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE213]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE215]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(u575b) -> d64>(%[[VALUE_testu575]], const<u575b>(123665200736552250000000000000000000000000000000000000001000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000)));
// DEFAULT-NEXT:             call<d64, signature=fn(u575b) -> d64>(%[[VALUE_testu575]], const<u575b>(123665200736552250000000000000000000000000000000000000001000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], const<d64>(1236652007365523e+158));
// DEFAULT-NEXT:             let %[[VALUE216:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(1236652007365523e+158))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE216]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE216]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE215]], read<bool>(%[[VALUE216]]));
// DEFAULT-NEXT:         let %[[VALUE217:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE215]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE217]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(u575b) -> d64>(%[[VALUE_testu575]], const<u575b>(123665200736552267030251260509823595017565674550605919957031528046448612553265933585158200530621522494798835713008069669675682517153375604983773077550946583958303386074349567)));
// DEFAULT-NEXT:             call<d64, signature=fn(u575b) -> d64>(%[[VALUE_testu575]], const<u575b>(123665200736552267030251260509823595017565674550605919957031528046448612553265933585158200530621522494798835713008069669675682517153375604983773077550946583958303386074349567));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_5]], const<d64>(1236652007365523e+158));
// DEFAULT-NEXT:             let %[[VALUE218:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(1236652007365523e+158))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE218]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE218]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_5]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             write<bool>(%[[VALUE217]], read<bool>(%[[VALUE218]]));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE217]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
