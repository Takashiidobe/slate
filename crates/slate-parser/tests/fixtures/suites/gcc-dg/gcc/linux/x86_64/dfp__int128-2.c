/* PR libgcc/65833 */
/* { dg-require-effective-target int128 } */
/* { dg-require-effective-target bitint } */
/* { dg-options "-O2 -std=gnu2x" } */

__attribute__((noipa)) _Decimal64
tests64 (__int128 b)
{
  return b;
}

__attribute__((noipa)) _Decimal64
testu64 (unsigned __int128 b)
{
  return b;
}

__attribute__((noipa)) _Decimal32
tests32 (__int128 b)
{
  return b;
}

__attribute__((noipa)) _Decimal32
testu32 (unsigned __int128 b)
{
  return b;
}

__attribute__((noipa)) _Decimal128
tests128 (__int128 b)
{
  return b;
}

__attribute__((noipa)) _Decimal128
testu128 (unsigned __int128 b)
{
  return b;
}

int
main ()
{
  {
    _Decimal64 a, b;
#define CHECK(x, y) (a = (x), b = (y), a != (y) || __builtin_memcmp (&a, &b, sizeof (a)))
#define C(x, y) ((((__int128) (x##ULL)) << 64) | (y##ULL))
#define UC(x, y) ((((unsigned __int128) (x##ULL)) << 64) | (y##ULL))
#define INT128_MAX ((__int128) ((((unsigned __int128) 1) << 127) - 1))
#define UINT128_MAX (~(unsigned __int128) 0)
    if (CHECK (tests64 (0LL), 0.DD)
	|| CHECK (tests64 (7LL), 7.DD)
	|| CHECK (tests64 (-42LL), -42.DD)
	|| CHECK (tests64 (-777777777LL), -777777777.DD)
	|| CHECK (tests64 (9999999999999000LL), 9999999999999000.DD)
	|| CHECK (tests64 (-9999999999999999LL), -9999999999999999.DD)
	|| CHECK (tests64 (-99999999999999994LL), -9999999999999999.e+1DD)
	|| CHECK (tests64 (99999999999999995LL), 1000000000000000.e+2DD)
	|| CHECK (tests64 (999999999999999900LL), 9999999999999999.e+2DD)
	|| CHECK (tests64 (999999999999999949LL), 9999999999999999.e+2DD)
	|| CHECK (tests64 (-(__int128) 9999999999999999000ULL), -9999999999999999.e+3DD)
	|| CHECK (tests64 (9999999999999999499ULL), 9999999999999999.e+3DD)
	|| CHECK (tests64 (C (0x36, 0x35c9adc5de9e7960)), 9999999999999999.e+5DD)
	|| CHECK (tests64 (C (0x36, 0x35c9adc5de9f3caf)), 9999999999999999.e+5DD)
	|| CHECK (tests64 (-C (0x21e, 0x19e0c9bab230bdc0)), -9999999999999999.e+6DD)
	|| CHECK (tests64 (-C (0x21e, 0x19e0c9bab2385edf)), -9999999999999999.e+6DD)
	|| CHECK (tests64 (C (0x1a24, 0x9b1f10a067e2c000)), 1234567890123456.e+8DD)
	|| CHECK (tests64 (C (0x2937babe64c6b8c, 0x10542c1f57200000)), 3424231985445429e+21DD)
	|| CHECK (tests64 (C (0x4b3b4ca85a86c25b, 0xefa958854dc00000)), 9999999999999999.e+22DD)
	|| CHECK (tests64 (C (0x4b3b4ca85a86c36a, 0xfc99bd62a6dfffff)), 9999999999999999.e+22DD)
	|| CHECK (tests64 (-C (0x4b3b4ca85a86c25b, 0xefa958854dc00000)), -9999999999999999.e+22DD)
	|| CHECK (tests64 (-C (0x1016b2fcff8f2cf6, 0xd16cf61904c00000)), -2138550877694459e+22DD)
	|| CHECK (tests64 (-C (0x1016b2fcff8f2e05, 0xde5d5af65de00000)), -2138550877694460e+22DD)
	|| CHECK (tests64 (-C (0x1016b2fcff8f2e05, 0xde5d5af65ddfffff)), -2138550877694459e+22DD)
	|| CHECK (tests64 (-C (0x1016b2fcff8f2f14, 0xeb4dbfd3b6ffffff)), -2138550877694460e+22DD)
	|| CHECK (tests64 (-C (0x1016b2fcff8f2ad8, 0xb78c2c5e52800000)), -2138550877694458e+22DD)
	|| CHECK (tests64 (-C (0x1016b2fcff8f2be7, 0xc47c913baba00000)), -2138550877694458e+22DD)
	|| CHECK (tests64 (-C (0x1016b2fcff8f2be7, 0xc47c913baba00001)), -2138550877694459e+22DD)
	|| CHECK (tests64 (C (0x7ffffffffffff947, 0xd26076f482000000)), 1701411834604692e+23DD)
	|| CHECK (tests64 (INT128_MAX), 1701411834604692e+23DD)
	|| CHECK (tests64 (-C (0x7ffffffffffff947, 0xd26076f482000000)), -1701411834604692e+23DD)
	|| CHECK (tests64 (-INT128_MAX - 1), -1701411834604692e+23DD))
      __builtin_abort ();
    if (CHECK (testu64 (0ULL), 0.DD)
	|| CHECK (testu64 (7ULL), 7.DD)
	|| CHECK (testu64 (42ULL), 42.DD)
	|| CHECK (testu64 (777777777ULL), 777777777.DD)
	|| CHECK (testu64 (9999999999999000ULL), 9999999999999000.DD)
	|| CHECK (testu64 (999999999999999900ULL), 9999999999999999.e+2DD)
	|| CHECK (testu64 (9999999999999999000ULL), 9999999999999999.e+3DD)
	|| CHECK (testu64 (UC (0x5, 0x6bc75e2d630fec77)), 9999999999999999.e+4DD)
	|| CHECK (testu64 (UC (0x36, 0x35c9adc5de9e7960)), 9999999999999999.e+5DD)
	|| CHECK (testu64 (UC (0x21e, 0x19e0c9bab230bdc0)), 9999999999999999.e+6DD)
	|| CHECK (testu64 (UC (0x1a24, 0x9b1f10a067e2c000)), 1234567890123456.e+8DD)
	|| CHECK (testu64 (UC (0x19c2d4b6fefc3378, 0xa349b93967400000)), 3424231985445429e+22DD)
	|| CHECK (testu64 (UC (0x2e90434dfef46d45, 0x1f6d190ddcc00000)), 6189354365465179e+22DD)
	|| CHECK (testu64 (UC (0x2e90434dfef46e54, 0x2c5d7deb35e00000)), 6189354365465180e+22DD)
	|| CHECK (testu64 (UC (0x2e90434dfef46e54, 0x2c5d7deb35dfffff)), 6189354365465179e+22DD)
	|| CHECK (testu64 (UC (0x2e90434dfef46f63, 0x394de2c88effffff)), 6189354365465180e+22DD)
	|| CHECK (testu64 (UC (0x2e90434dfef46b27, 0x058c4f532a800000)), 6189354365465178e+22DD)
	|| CHECK (testu64 (UC (0x2e90434dfef46c36, 0x127cb43083a00000)), 6189354365465178e+22DD)
	|| CHECK (testu64 (UC (0x2e90434dfef46c36, 0x127cb43083a00001)), 6189354365465179e+22DD)
	|| CHECK (testu64 (UC (0x4b3b4ca85a86c25b, 0xefa958854dc00000)), 9999999999999999.e+22DD)
	|| CHECK (testu64 (UC (0xfffffffffffff28f, 0xa4c0ede904000000)), 340282366920938.4e+24DD)
	|| CHECK (testu64 (UC (0xfffffffffffffd26, 0x2624de8e7f3fffff)), 340282366920938.4e+24DD)
	|| CHECK (testu64 (UC (0xfffffffffffffd26, 0x2624de8e7f400000)), 340282366920938.4e+24DD)
	|| CHECK (testu64 (UC (0xfffffffffffffd26, 0x2624de8e7f400001)), 340282366920938.5e+24DD)
	|| CHECK (testu64 (UC (0xfffffffffffffd26, 0x2624de90d34be400)), 340282366920938.5e+24DD)
	|| CHECK (testu64 (UINT128_MAX), 340282366920938.5e+24DD))
      __builtin_abort ();
  }
  {
    _Decimal32 a, b;
    if (CHECK (tests32 (0LL), 0.DF)
	|| CHECK (tests32 (7LL), 7.DF)
	|| CHECK (tests32 (-42LL), -42.DF)
	|| CHECK (tests32 (-777777LL), -777777.DF)
	|| CHECK (tests32 (9999000LL), 9999000.DF)
	|| CHECK (tests32 (-9999999LL), -9999999.DF)
	|| CHECK (tests32 (99999994LL), 9999999.e+1DF)
	|| CHECK (tests32 (99999995LL), 1000000.e+2DF)
	|| CHECK (tests32 (999999900LL), 9999999.e+2DF)
	|| CHECK (tests32 (-9999999000LL), -9999999.e+3DF)
	|| CHECK (tests32 (999999900000LL), 9999999.e+5DF)
	|| CHECK (tests32 (-9999999000000LL), -9999999.e+6DF)
	|| CHECK (tests32 (123456700000000000LL), 1234567.e+11DF)
	|| CHECK (tests32 (C (0x6ea49330, 0xa5672e5497c00000)), 3424231e+22DF)
	|| CHECK (tests32 (C (0x4b3b4c2a22c8a457, 0x48f8d71980000000)), 9999999.e+31DF)
	|| CHECK (tests32 (-C (0x4b3b4c2a22c8a457, 0x48f8d71980000000)), -9999999.e+31DF)
	|| CHECK (tests32 (-C (0x1016b30c6f76e61c, 0x6cefef0580000000)), -2138551e+31DF)
	|| CHECK (tests32 (-C (0x1016b34b8b55f62d, 0xcd389498c0000000)), -2138552e+31DF)
	|| CHECK (tests32 (-C (0x1016b34b8b55f62d, 0xcd389498bfffffff)), -2138551e+31DF)
	|| CHECK (tests32 (-C (0x1016b38aa735063f, 0x2d813a2bffffffff)), -2138552e+31DF)
	|| CHECK (tests32 (-C (0x1016b38aa735063f, 0x2d813a2c00000000)), -2138552e+31DF)
	|| CHECK (tests32 (-C (0x1016b3c9c3141650, 0x8dc9dfbf40000000)), -2138552e+31DF)
	|| CHECK (tests32 (-C (0x1016b3c9c3141650, 0x8dc9dfbf40000001)), -2138553e+31DF)
	|| CHECK (tests32 (C (0x7ffffbe294adefda, 0xd863b4a300000000)), 1701411e+32DF)
	|| CHECK (tests32 (INT128_MAX), 1701412e+32DF)
	|| CHECK (tests32 (-C (0x7ffffbe294adefda, 0xd863b4a300000000)), -1701411e+32DF)
	|| CHECK (tests32 (-INT128_MAX - 1LL), -1701412e+32DF))
      __builtin_abort ();
    if (CHECK (testu32 (0ULL), 0.DF)
	|| CHECK (testu32 (7ULL), 7.DF)
	|| CHECK (testu32 (42ULL), 42.DF)
	|| CHECK (testu32 (77777ULL), 77777.DF)
	|| CHECK (testu32 (9999000ULL), 9999000.DF)
	|| CHECK (testu32 (999999900ULL), 9999999.e+2DF)
	|| CHECK (testu32 (999999949ULL), 9999999.e+2DF)
	|| CHECK (testu32 (9999999000ULL), 9999999.e+3DF)
	|| CHECK (testu32 (9999999499ULL), 9999999.e+3DF)
	|| CHECK (testu32 (999999900000ULL), 9999999.e+5DF)
	|| CHECK (testu32 (9999999000000ULL), 9999999.e+6DF)
	|| CHECK (testu32 (123456700000000ULL), 1234567.e+8DF)
	|| CHECK (testu32 (UC (0x6ea49330, 0xa5672e5497c00000)), 3424231e+22DF)
	|| CHECK (testu32 (UC (0x785ee0436adaa08, 0xba7f48b5c0000000)), 9999999.e+30DF)
	|| CHECK (testu32 (UC (0x2e904596f4d9f2bb, 0x14fbca9180000000)), 6189359e+31DF)
	|| CHECK (testu32 (UC (0x2e9045d610b902cc, 0x75447024c0000000)), 6189360e+31DF)
	|| CHECK (testu32 (UC (0x2e9045d610b902cc, 0x75447024bfffffff)), 6189359e+31DF)
	|| CHECK (testu32 (UC (0x2e9046152c9812dd, 0xd58d15b7ffffffff)), 6189360e+31DF)
	|| CHECK (testu32 (UC (0x2e904518bd1bd298, 0x546a7f6b00000000)), 6189358e+31DF)
	|| CHECK (testu32 (UC (0x2e904557d8fae2a9, 0xb4b324fe40000000)), 6189358e+31DF)
	|| CHECK (testu32 (UC (0x2e904557d8fae2a9, 0xb4b324fe40000001)), 6189359e+31DF)
	|| CHECK (testu32 (UC (0xfffffcb356c92111, 0x367458c700000000)), 3402823e+32DF)
	|| CHECK (testu32 (UINT128_MAX), 3402824e+32DF))
      __builtin_abort ();
  }
  {
    _Decimal128 a, b;
    if (CHECK (tests128 (0LL), 0.DL)
	|| CHECK (tests128 (7LL), 7.DL)
	|| CHECK (tests128 (-42LL), -42.DL)
	|| CHECK (tests128 (-777777777LL), -777777777.DL)
	|| CHECK (tests128 (-12345678912345LL), -12345678912345.DL)
	|| CHECK (tests128 (123456789123456789LL), 123456789123456789.DL)
	|| CHECK (tests128 (C (0x2835cd9, 0xd1a22ada09c71c71)), 777777777777777777777777777.DL)
	|| CHECK (tests128 (C (0x1ed09bead87c0, 0x378d8e63fa0a1f00)), 9999999999999999999999999900000000.DL)
	|| CHECK (tests128 (-C (0x1ed09bead87c0, 0x378d8e63ffffffff)), -9999999999999999999999999999999999.DL)
	|| CHECK (tests128 (-C (0x13426172c74d82, 0x2b878fe7fffffffa)), -9999999999999999999999999999999999.e+1DL)
	|| CHECK (tests128 (C (0x13426172c74d82, 0x2b878fe7fffffffb)), 1000000000000000000000000000000000.e+2DL)
	|| CHECK (tests128 (C (0xc097ce7bc90715, 0xb34b9f0fffffff9c)), 9999999999999999999999999999999999.e+2DL)
	|| CHECK (tests128 (C (0xc097ce7bc90715, 0xb34b9f0fffffffcd)), 9999999999999999999999999999999999.e+2DL)
	|| CHECK (tests128 (-C (0x785ee10d5da46d9, 0x00f4369ffffffc18)), -9999999999999999999999999999999999.e+3DL)
	|| CHECK (tests128 (C (0x785ee10d5da46d9, 0x00f4369ffffffe0b)), 9999999999999999999999999999999999.e+3DL)
	|| CHECK (tests128 (C (0x7fffffffffffffff, 0xffffffffffffe9a0)), 1701411834604692317316873037158841.e+5DL)
	|| CHECK (tests128 (INT128_MAX), 1701411834604692317316873037158841.e+5DL)
	|| CHECK (tests128 (-INT128_MAX - 1), -1701411834604692317316873037158841.e+5DL))
      __builtin_abort ();
    if (CHECK (testu128 (0ULL), 0.DL)
	|| CHECK (testu128 (7ULL), 7.DL)
	|| CHECK (testu128 (42ULL), 42.DL)
	|| CHECK (testu128 (777777777ULL), 777777777.DL)
	|| CHECK (testu128 (UC (0x1431e0fae, 0x6d7217ca9ffffc18)), 99999999999999999999999999000.DL)
	|| CHECK (testu128 (UC (0xc097ce7bc90715, 0xb34b9f0fffffff9c)), 9999999999999999999999999999999999.e+2DL)
	|| CHECK (testu128 (UC (0x785ee10d5da46d9, 0x00f4369ffffffc18)), 9999999999999999999999999999999999.e+3DL)
	|| CHECK (testu128 (UC (0x4b3b4ca85a86c47a, 0x098a223fffffec77)), 9999999999999999999999999999999999.e+4DL)
	|| CHECK (testu128 (UC (0xffffffffffffffff, 0xffffffffffffd340)), 3402823669209384634633746074317682.e+5DL)
	|| CHECK (testu128 (UINT128_MAX), 3402823669209384634633746074317682.e+5DL))
      __builtin_abort ();
  }
}

// SLATE-FILECHECK-STD DEFAULT gnu2x
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
// DEFAULT-NEXT:     fn %0 @tests64(%1 b: i128) -> d64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return int_to_float<d64, reason=return, exact=false, rounding=nearest_even, exceptions=observable>(read<i128>(%1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %2 @testu64(%3 b: u128) -> d64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return int_to_float<d64, reason=return, exact=false, rounding=nearest_even, exceptions=observable>(read<u128>(%3));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @tests32(%5 b: i128) -> d32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return int_to_float<d32, reason=return, exact=false, rounding=nearest_even, exceptions=observable>(read<i128>(%5));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @testu32(%7 b: u128) -> d32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return int_to_float<d32, reason=return, exact=false, rounding=nearest_even, exceptions=observable>(read<u128>(%7));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @tests128(%9 b: i128) -> d128 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return int_to_float<d128, reason=return, exact=false, rounding=nearest_even, exceptions=observable>(read<i128>(%9));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @testu128(%11 b: u128) -> d128 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return int_to_float<d128, reason=return, exact=false, rounding=nearest_even, exceptions=observable>(read<u128>(%11));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %22 @__builtin_memcmp(%19 <unnamed>: ptr<const void>, %20 <unnamed>: ptr<const void>, %21 <unnamed>: u64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %23 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %12 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %13 a: d64 [storage=automatic];
// DEFAULT-NEXT:             let %14 b: d64 [storage=automatic];
// DEFAULT-NEXT:             write<d64>(%13, call<d64, signature=fn(i128) -> d64>(%0, widen<i128, reason=arg>(const<i64>(0))));
// DEFAULT-NEXT:             call<d64, signature=fn(i128) -> d64>(%0, widen<i128, reason=arg>(const<i64>(0)));
// DEFAULT-NEXT:             write<d64>(%14, const<d64>(0.));
// DEFAULT-NEXT:             let %24: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%13), const<d64>(0.))
// DEFAULT-NEXT:                 write<bool>(%24, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%24, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%13)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%14)), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             let %25: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%24)
// DEFAULT-NEXT:                 write<bool>(%25, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%13, call<d64, signature=fn(i128) -> d64>(%0, widen<i128, reason=arg>(const<i64>(7))));
// DEFAULT-NEXT:                 call<d64, signature=fn(i128) -> d64>(%0, widen<i128, reason=arg>(const<i64>(7)));
// DEFAULT-NEXT:                 write<d64>(%14, const<d64>(7.));
// DEFAULT-NEXT:                 let %26: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%13), const<d64>(7.))
// DEFAULT-NEXT:                     write<bool>(%26, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%26, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%13)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%14)), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%25, read<bool>(%26));
// DEFAULT-NEXT:             let %27: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%25)
// DEFAULT-NEXT:                 write<bool>(%27, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%13, call<d64, signature=fn(i128) -> d64>(%0, widen<i128, reason=arg>(neg<i64, overflow=ub>(const<i64>(42)))));
// DEFAULT-NEXT:                 call<d64, signature=fn(i128) -> d64>(%0, widen<i128, reason=arg>(neg<i64, overflow=ub>(const<i64>(42))));
// DEFAULT-NEXT:                 write<d64>(%14, neg<d64>(const<d64>(42.)));
// DEFAULT-NEXT:                 let %28: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%13), neg<d64>(const<d64>(42.)))
// DEFAULT-NEXT:                     write<bool>(%28, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%28, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%13)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%14)), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%27, read<bool>(%28));
// DEFAULT-NEXT:             let %29: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%27)
// DEFAULT-NEXT:                 write<bool>(%29, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%13, call<d64, signature=fn(i128) -> d64>(%0, widen<i128, reason=arg>(neg<i64, overflow=ub>(const<i64>(777777777)))));
// DEFAULT-NEXT:                 call<d64, signature=fn(i128) -> d64>(%0, widen<i128, reason=arg>(neg<i64, overflow=ub>(const<i64>(777777777))));
// DEFAULT-NEXT:                 write<d64>(%14, neg<d64>(const<d64>(777777777.)));
// DEFAULT-NEXT:                 let %30: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%13), neg<d64>(const<d64>(777777777.)))
// DEFAULT-NEXT:                     write<bool>(%30, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%30, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%13)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%14)), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%29, read<bool>(%30));
// DEFAULT-NEXT:             let %31: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%29)
// DEFAULT-NEXT:                 write<bool>(%31, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%13, call<d64, signature=fn(i128) -> d64>(%0, widen<i128, reason=arg>(const<i64>(9999999999999000))));
// DEFAULT-NEXT:                 call<d64, signature=fn(i128) -> d64>(%0, widen<i128, reason=arg>(const<i64>(9999999999999000)));
// DEFAULT-NEXT:                 write<d64>(%14, const<d64>(9999999999999000.));
// DEFAULT-NEXT:                 let %32: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%13), const<d64>(9999999999999000.))
// DEFAULT-NEXT:                     write<bool>(%32, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%32, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%13)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%14)), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%31, read<bool>(%32));
// DEFAULT-NEXT:             let %33: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%31)
// DEFAULT-NEXT:                 write<bool>(%33, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%13, call<d64, signature=fn(i128) -> d64>(%0, widen<i128, reason=arg>(neg<i64, overflow=ub>(const<i64>(9999999999999999)))));
// DEFAULT-NEXT:                 call<d64, signature=fn(i128) -> d64>(%0, widen<i128, reason=arg>(neg<i64, overflow=ub>(const<i64>(9999999999999999))));
// DEFAULT-NEXT:                 write<d64>(%14, neg<d64>(const<d64>(9999999999999999.)));
// DEFAULT-NEXT:                 let %34: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%13), neg<d64>(const<d64>(9999999999999999.)))
// DEFAULT-NEXT:                     write<bool>(%34, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%34, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%13)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%14)), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%33, read<bool>(%34));
// DEFAULT-NEXT:             let %35: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%33)
// DEFAULT-NEXT:                 write<bool>(%35, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%13, call<d64, signature=fn(i128) -> d64>(%0, widen<i128, reason=arg>(neg<i64, overflow=ub>(const<i64>(99999999999999994)))));
// DEFAULT-NEXT:                 call<d64, signature=fn(i128) -> d64>(%0, widen<i128, reason=arg>(neg<i64, overflow=ub>(const<i64>(99999999999999994))));
// DEFAULT-NEXT:                 write<d64>(%14, neg<d64>(const<d64>(9999999999999999.e+1)));
// DEFAULT-NEXT:                 let %36: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%13), neg<d64>(const<d64>(9999999999999999.e+1)))
// DEFAULT-NEXT:                     write<bool>(%36, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%36, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%13)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%14)), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%35, read<bool>(%36));
// DEFAULT-NEXT:             let %37: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%35)
// DEFAULT-NEXT:                 write<bool>(%37, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%13, call<d64, signature=fn(i128) -> d64>(%0, widen<i128, reason=arg>(const<i64>(99999999999999995))));
// DEFAULT-NEXT:                 call<d64, signature=fn(i128) -> d64>(%0, widen<i128, reason=arg>(const<i64>(99999999999999995)));
// DEFAULT-NEXT:                 write<d64>(%14, const<d64>(1000000000000000.e+2));
// DEFAULT-NEXT:                 let %38: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%13), const<d64>(1000000000000000.e+2))
// DEFAULT-NEXT:                     write<bool>(%38, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%38, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%13)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%14)), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%37, read<bool>(%38));
// DEFAULT-NEXT:             let %39: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%37)
// DEFAULT-NEXT:                 write<bool>(%39, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%13, call<d64, signature=fn(i128) -> d64>(%0, widen<i128, reason=arg>(const<i64>(999999999999999900))));
// DEFAULT-NEXT:                 call<d64, signature=fn(i128) -> d64>(%0, widen<i128, reason=arg>(const<i64>(999999999999999900)));
// DEFAULT-NEXT:                 write<d64>(%14, const<d64>(9999999999999999.e+2));
// DEFAULT-NEXT:                 let %40: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%13), const<d64>(9999999999999999.e+2))
// DEFAULT-NEXT:                     write<bool>(%40, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%40, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%13)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%14)), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%39, read<bool>(%40));
// DEFAULT-NEXT:             let %41: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%39)
// DEFAULT-NEXT:                 write<bool>(%41, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%13, call<d64, signature=fn(i128) -> d64>(%0, widen<i128, reason=arg>(const<i64>(999999999999999949))));
// DEFAULT-NEXT:                 call<d64, signature=fn(i128) -> d64>(%0, widen<i128, reason=arg>(const<i64>(999999999999999949)));
// DEFAULT-NEXT:                 write<d64>(%14, const<d64>(9999999999999999.e+2));
// DEFAULT-NEXT:                 let %42: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%13), const<d64>(9999999999999999.e+2))
// DEFAULT-NEXT:                     write<bool>(%42, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%42, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%13)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%14)), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%41, read<bool>(%42));
// DEFAULT-NEXT:             let %43: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%41)
// DEFAULT-NEXT:                 write<bool>(%43, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%13, call<d64, signature=fn(i128) -> d64>(%0, neg<i128, overflow=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(9999999999999999000))))));
// DEFAULT-NEXT:                 call<d64, signature=fn(i128) -> d64>(%0, neg<i128, overflow=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(9999999999999999000)))));
// DEFAULT-NEXT:                 write<d64>(%14, neg<d64>(const<d64>(9999999999999999.e+3)));
// DEFAULT-NEXT:                 let %44: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%13), neg<d64>(const<d64>(9999999999999999.e+3)))
// DEFAULT-NEXT:                     write<bool>(%44, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%44, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%13)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%14)), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%43, read<bool>(%44));
// DEFAULT-NEXT:             let %45: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%43)
// DEFAULT-NEXT:                 write<bool>(%45, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%13, call<d64, signature=fn(i128) -> d64>(%0, reinterpret<i128, reason=arg, fits=unknown>(widen<u128, reason=arg>(const<u64>(9999999999999999499)))));
// DEFAULT-NEXT:                 call<d64, signature=fn(i128) -> d64>(%0, reinterpret<i128, reason=arg, fits=unknown>(widen<u128, reason=arg>(const<u64>(9999999999999999499))));
// DEFAULT-NEXT:                 write<d64>(%14, const<d64>(9999999999999999.e+3));
// DEFAULT-NEXT:                 let %46: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%13), const<d64>(9999999999999999.e+3))
// DEFAULT-NEXT:                     write<bool>(%46, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%46, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%13)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%14)), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%45, read<bool>(%46));
// DEFAULT-NEXT:             let %47: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%45)
// DEFAULT-NEXT:                 write<bool>(%47, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%13, call<d64, signature=fn(i128) -> d64>(%0, or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(54))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(3875820019684112736))))));
// DEFAULT-NEXT:                 call<d64, signature=fn(i128) -> d64>(%0, or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(54))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(3875820019684112736)))));
// DEFAULT-NEXT:                 write<d64>(%14, const<d64>(9999999999999999.e+5));
// DEFAULT-NEXT:                 let %48: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%13), const<d64>(9999999999999999.e+5))
// DEFAULT-NEXT:                     write<bool>(%48, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%48, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%13)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%14)), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%47, read<bool>(%48));
// DEFAULT-NEXT:             let %49: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%47)
// DEFAULT-NEXT:                 write<bool>(%49, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%13, call<d64, signature=fn(i128) -> d64>(%0, or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(54))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(3875820019684162735))))));
// DEFAULT-NEXT:                 call<d64, signature=fn(i128) -> d64>(%0, or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(54))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(3875820019684162735)))));
// DEFAULT-NEXT:                 write<d64>(%14, const<d64>(9999999999999999.e+5));
// DEFAULT-NEXT:                 let %50: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%13), const<d64>(9999999999999999.e+5))
// DEFAULT-NEXT:                     write<bool>(%50, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%50, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%13)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%14)), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%49, read<bool>(%50));
// DEFAULT-NEXT:             let %51: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%49)
// DEFAULT-NEXT:                 write<bool>(%51, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%13, call<d64, signature=fn(i128) -> d64>(%0, neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(542))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(1864712049422024128)))))));
// DEFAULT-NEXT:                 call<d64, signature=fn(i128) -> d64>(%0, neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(542))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(1864712049422024128))))));
// DEFAULT-NEXT:                 write<d64>(%14, neg<d64>(const<d64>(9999999999999999.e+6)));
// DEFAULT-NEXT:                 let %52: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%13), neg<d64>(const<d64>(9999999999999999.e+6)))
// DEFAULT-NEXT:                     write<bool>(%52, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%52, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%13)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%14)), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%51, read<bool>(%52));
// DEFAULT-NEXT:             let %53: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%51)
// DEFAULT-NEXT:                 write<bool>(%53, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%13, call<d64, signature=fn(i128) -> d64>(%0, neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(542))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(1864712049422524127)))))));
// DEFAULT-NEXT:                 call<d64, signature=fn(i128) -> d64>(%0, neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(542))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(1864712049422524127))))));
// DEFAULT-NEXT:                 write<d64>(%14, neg<d64>(const<d64>(9999999999999999.e+6)));
// DEFAULT-NEXT:                 let %54: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%13), neg<d64>(const<d64>(9999999999999999.e+6)))
// DEFAULT-NEXT:                     write<bool>(%54, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%54, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%13)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%14)), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%53, read<bool>(%54));
// DEFAULT-NEXT:             let %55: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%53)
// DEFAULT-NEXT:                 write<bool>(%55, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%13, call<d64, signature=fn(i128) -> d64>(%0, or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(6692))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(11177671081280585728))))));
// DEFAULT-NEXT:                 call<d64, signature=fn(i128) -> d64>(%0, or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(6692))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(11177671081280585728)))));
// DEFAULT-NEXT:                 write<d64>(%14, const<d64>(1234567890123456.e+8));
// DEFAULT-NEXT:                 let %56: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%13), const<d64>(1234567890123456.e+8))
// DEFAULT-NEXT:                     write<bool>(%56, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%56, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%13)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%14)), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%55, read<bool>(%56));
// DEFAULT-NEXT:             let %57: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%55)
// DEFAULT-NEXT:                 write<bool>(%57, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%13, call<d64, signature=fn(i128) -> d64>(%0, or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(185627987885714316))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(1176613915767865344))))));
// DEFAULT-NEXT:                 call<d64, signature=fn(i128) -> d64>(%0, or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(185627987885714316))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(1176613915767865344)))));
// DEFAULT-NEXT:                 write<d64>(%14, const<d64>(3424231985445429e+21));
// DEFAULT-NEXT:                 let %58: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%13), const<d64>(3424231985445429e+21))
// DEFAULT-NEXT:                     write<bool>(%58, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%58, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%13)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%14)), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%57, read<bool>(%58));
// DEFAULT-NEXT:             let %59: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%57)
// DEFAULT-NEXT:                 write<bool>(%59, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%13, call<d64, signature=fn(i128) -> d64>(%0, or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(5421010862427521627))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(17269431575687200768))))));
// DEFAULT-NEXT:                 call<d64, signature=fn(i128) -> d64>(%0, or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(5421010862427521627))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(17269431575687200768)))));
// DEFAULT-NEXT:                 write<d64>(%14, const<d64>(9999999999999999.e+22));
// DEFAULT-NEXT:                 let %60: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%13), const<d64>(9999999999999999.e+22))
// DEFAULT-NEXT:                     write<bool>(%60, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%60, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%13)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%14)), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%59, read<bool>(%60));
// DEFAULT-NEXT:             let %61: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%59)
// DEFAULT-NEXT:                 write<bool>(%61, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%13, call<d64, signature=fn(i128) -> d64>(%0, or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(5421010862427521898))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(18201787600398712831))))));
// DEFAULT-NEXT:                 call<d64, signature=fn(i128) -> d64>(%0, or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(5421010862427521898))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(18201787600398712831)))));
// DEFAULT-NEXT:                 write<d64>(%14, const<d64>(9999999999999999.e+22));
// DEFAULT-NEXT:                 let %62: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%13), const<d64>(9999999999999999.e+22))
// DEFAULT-NEXT:                     write<bool>(%62, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%62, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%13)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%14)), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%61, read<bool>(%62));
// DEFAULT-NEXT:             let %63: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%61)
// DEFAULT-NEXT:                 write<bool>(%63, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%13, call<d64, signature=fn(i128) -> d64>(%0, neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(5421010862427521627))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(17269431575687200768)))))));
// DEFAULT-NEXT:                 call<d64, signature=fn(i128) -> d64>(%0, neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(5421010862427521627))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(17269431575687200768))))));
// DEFAULT-NEXT:                 write<d64>(%14, neg<d64>(const<d64>(9999999999999999.e+22)));
// DEFAULT-NEXT:                 let %64: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%13), neg<d64>(const<d64>(9999999999999999.e+22)))
// DEFAULT-NEXT:                     write<bool>(%64, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%64, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%13)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%14)), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%63, read<bool>(%64));
// DEFAULT-NEXT:             let %65: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%63)
// DEFAULT-NEXT:                 write<bool>(%65, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%13, call<d64, signature=fn(i128) -> d64>(%0, neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(1159310753783557366))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(15090707038725996544)))))));
// DEFAULT-NEXT:                 call<d64, signature=fn(i128) -> d64>(%0, neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(1159310753783557366))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(15090707038725996544))))));
// DEFAULT-NEXT:                 write<d64>(%14, neg<d64>(const<d64>(2138550877694459e+22)));
// DEFAULT-NEXT:                 let %66: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%13), neg<d64>(const<d64>(2138550877694459e+22)))
// DEFAULT-NEXT:                     write<bool>(%66, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%66, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%13)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%14)), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%65, read<bool>(%66));
// DEFAULT-NEXT:             let %67: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%65)
// DEFAULT-NEXT:                 write<bool>(%67, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%13, call<d64, signature=fn(i128) -> d64>(%0, neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(1159310753783557637))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(16023063063437508608)))))));
// DEFAULT-NEXT:                 call<d64, signature=fn(i128) -> d64>(%0, neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(1159310753783557637))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(16023063063437508608))))));
// DEFAULT-NEXT:                 write<d64>(%14, neg<d64>(const<d64>(2138550877694460e+22)));
// DEFAULT-NEXT:                 let %68: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%13), neg<d64>(const<d64>(2138550877694460e+22)))
// DEFAULT-NEXT:                     write<bool>(%68, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%68, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%13)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%14)), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%67, read<bool>(%68));
// DEFAULT-NEXT:             let %69: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%67)
// DEFAULT-NEXT:                 write<bool>(%69, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%13, call<d64, signature=fn(i128) -> d64>(%0, neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(1159310753783557637))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(16023063063437508607)))))));
// DEFAULT-NEXT:                 call<d64, signature=fn(i128) -> d64>(%0, neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(1159310753783557637))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(16023063063437508607))))));
// DEFAULT-NEXT:                 write<d64>(%14, neg<d64>(const<d64>(2138550877694459e+22)));
// DEFAULT-NEXT:                 let %70: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%13), neg<d64>(const<d64>(2138550877694459e+22)))
// DEFAULT-NEXT:                     write<bool>(%70, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%70, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%13)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%14)), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%69, read<bool>(%70));
// DEFAULT-NEXT:             let %71: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%69)
// DEFAULT-NEXT:                 write<bool>(%71, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%13, call<d64, signature=fn(i128) -> d64>(%0, neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(1159310753783557908))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(16955419088149020671)))))));
// DEFAULT-NEXT:                 call<d64, signature=fn(i128) -> d64>(%0, neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(1159310753783557908))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(16955419088149020671))))));
// DEFAULT-NEXT:                 write<d64>(%14, neg<d64>(const<d64>(2138550877694460e+22)));
// DEFAULT-NEXT:                 let %72: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%13), neg<d64>(const<d64>(2138550877694460e+22)))
// DEFAULT-NEXT:                     write<bool>(%72, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%72, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%13)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%14)), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%71, read<bool>(%72));
// DEFAULT-NEXT:             let %73: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%71)
// DEFAULT-NEXT:                 write<bool>(%73, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%13, call<d64, signature=fn(i128) -> d64>(%0, neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(1159310753783556824))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(13225994989302972416)))))));
// DEFAULT-NEXT:                 call<d64, signature=fn(i128) -> d64>(%0, neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(1159310753783556824))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(13225994989302972416))))));
// DEFAULT-NEXT:                 write<d64>(%14, neg<d64>(const<d64>(2138550877694458e+22)));
// DEFAULT-NEXT:                 let %74: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%13), neg<d64>(const<d64>(2138550877694458e+22)))
// DEFAULT-NEXT:                     write<bool>(%74, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%74, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%13)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%14)), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%73, read<bool>(%74));
// DEFAULT-NEXT:             let %75: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%73)
// DEFAULT-NEXT:                 write<bool>(%75, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%13, call<d64, signature=fn(i128) -> d64>(%0, neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(1159310753783557095))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(14158351014014484480)))))));
// DEFAULT-NEXT:                 call<d64, signature=fn(i128) -> d64>(%0, neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(1159310753783557095))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(14158351014014484480))))));
// DEFAULT-NEXT:                 write<d64>(%14, neg<d64>(const<d64>(2138550877694458e+22)));
// DEFAULT-NEXT:                 let %76: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%13), neg<d64>(const<d64>(2138550877694458e+22)))
// DEFAULT-NEXT:                     write<bool>(%76, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%76, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%13)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%14)), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%75, read<bool>(%76));
// DEFAULT-NEXT:             let %77: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%75)
// DEFAULT-NEXT:                 write<bool>(%77, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%13, call<d64, signature=fn(i128) -> d64>(%0, neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(1159310753783557095))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(14158351014014484481)))))));
// DEFAULT-NEXT:                 call<d64, signature=fn(i128) -> d64>(%0, neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(1159310753783557095))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(14158351014014484481))))));
// DEFAULT-NEXT:                 write<d64>(%14, neg<d64>(const<d64>(2138550877694459e+22)));
// DEFAULT-NEXT:                 let %78: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%13), neg<d64>(const<d64>(2138550877694459e+22)))
// DEFAULT-NEXT:                     write<bool>(%78, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%78, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%13)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%14)), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%77, read<bool>(%78));
// DEFAULT-NEXT:             let %79: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%77)
// DEFAULT-NEXT:                 write<bool>(%79, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%13, call<d64, signature=fn(i128) -> d64>(%0, or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(9223372036854774087))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(15159247138254225408))))));
// DEFAULT-NEXT:                 call<d64, signature=fn(i128) -> d64>(%0, or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(9223372036854774087))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(15159247138254225408)))));
// DEFAULT-NEXT:                 write<d64>(%14, const<d64>(1701411834604692e+23));
// DEFAULT-NEXT:                 let %80: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%13), const<d64>(1701411834604692e+23))
// DEFAULT-NEXT:                     write<bool>(%80, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%80, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%13)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%14)), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%79, read<bool>(%80));
// DEFAULT-NEXT:             let %81: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%79)
// DEFAULT-NEXT:                 write<bool>(%81, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%13, call<d64, signature=fn(i128) -> d64>(%0, reinterpret<i128, reason=explicit, fits=unknown>(sub<u128, overflow=wrap>(shl<u128, overflow=wrap, amount_out_of_range=ub>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(1))), const<i32>(127)), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(1)))))));
// DEFAULT-NEXT:                 call<d64, signature=fn(i128) -> d64>(%0, reinterpret<i128, reason=explicit, fits=unknown>(sub<u128, overflow=wrap>(shl<u128, overflow=wrap, amount_out_of_range=ub>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(1))), const<i32>(127)), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(1))))));
// DEFAULT-NEXT:                 write<d64>(%14, const<d64>(1701411834604692e+23));
// DEFAULT-NEXT:                 let %82: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%13), const<d64>(1701411834604692e+23))
// DEFAULT-NEXT:                     write<bool>(%82, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%82, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%13)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%14)), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%81, read<bool>(%82));
// DEFAULT-NEXT:             let %83: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%81)
// DEFAULT-NEXT:                 write<bool>(%83, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%13, call<d64, signature=fn(i128) -> d64>(%0, neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(9223372036854774087))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(15159247138254225408)))))));
// DEFAULT-NEXT:                 call<d64, signature=fn(i128) -> d64>(%0, neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(9223372036854774087))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(15159247138254225408))))));
// DEFAULT-NEXT:                 write<d64>(%14, neg<d64>(const<d64>(1701411834604692e+23)));
// DEFAULT-NEXT:                 let %84: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%13), neg<d64>(const<d64>(1701411834604692e+23)))
// DEFAULT-NEXT:                     write<bool>(%84, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%84, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%13)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%14)), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%83, read<bool>(%84));
// DEFAULT-NEXT:             let %85: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%83)
// DEFAULT-NEXT:                 write<bool>(%85, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%13, call<d64, signature=fn(i128) -> d64>(%0, sub<i128, overflow=ub>(neg<i128, overflow=ub>(reinterpret<i128, reason=explicit, fits=unknown>(sub<u128, overflow=wrap>(shl<u128, overflow=wrap, amount_out_of_range=ub>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(1))), const<i32>(127)), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(1)))))), widen<i128, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                 call<d64, signature=fn(i128) -> d64>(%0, sub<i128, overflow=ub>(neg<i128, overflow=ub>(reinterpret<i128, reason=explicit, fits=unknown>(sub<u128, overflow=wrap>(shl<u128, overflow=wrap, amount_out_of_range=ub>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(1))), const<i32>(127)), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(1)))))), widen<i128, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                 write<d64>(%14, neg<d64>(const<d64>(1701411834604692e+23)));
// DEFAULT-NEXT:                 let %86: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%13), neg<d64>(const<d64>(1701411834604692e+23)))
// DEFAULT-NEXT:                     write<bool>(%86, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%86, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%13)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%14)), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%85, read<bool>(%86));
// DEFAULT-NEXT:             if read<bool>(%85)
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%23);
// DEFAULT-NEXT:             write<d64>(%13, call<d64, signature=fn(u128) -> d64>(%2, widen<u128, reason=arg>(const<u64>(0))));
// DEFAULT-NEXT:             call<d64, signature=fn(u128) -> d64>(%2, widen<u128, reason=arg>(const<u64>(0)));
// DEFAULT-NEXT:             write<d64>(%14, const<d64>(0.));
// DEFAULT-NEXT:             let %87: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%13), const<d64>(0.))
// DEFAULT-NEXT:                 write<bool>(%87, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%87, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%13)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%14)), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             let %88: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%87)
// DEFAULT-NEXT:                 write<bool>(%88, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%13, call<d64, signature=fn(u128) -> d64>(%2, widen<u128, reason=arg>(const<u64>(7))));
// DEFAULT-NEXT:                 call<d64, signature=fn(u128) -> d64>(%2, widen<u128, reason=arg>(const<u64>(7)));
// DEFAULT-NEXT:                 write<d64>(%14, const<d64>(7.));
// DEFAULT-NEXT:                 let %89: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%13), const<d64>(7.))
// DEFAULT-NEXT:                     write<bool>(%89, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%89, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%13)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%14)), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%88, read<bool>(%89));
// DEFAULT-NEXT:             let %90: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%88)
// DEFAULT-NEXT:                 write<bool>(%90, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%13, call<d64, signature=fn(u128) -> d64>(%2, widen<u128, reason=arg>(const<u64>(42))));
// DEFAULT-NEXT:                 call<d64, signature=fn(u128) -> d64>(%2, widen<u128, reason=arg>(const<u64>(42)));
// DEFAULT-NEXT:                 write<d64>(%14, const<d64>(42.));
// DEFAULT-NEXT:                 let %91: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%13), const<d64>(42.))
// DEFAULT-NEXT:                     write<bool>(%91, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%91, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%13)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%14)), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%90, read<bool>(%91));
// DEFAULT-NEXT:             let %92: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%90)
// DEFAULT-NEXT:                 write<bool>(%92, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%13, call<d64, signature=fn(u128) -> d64>(%2, widen<u128, reason=arg>(const<u64>(777777777))));
// DEFAULT-NEXT:                 call<d64, signature=fn(u128) -> d64>(%2, widen<u128, reason=arg>(const<u64>(777777777)));
// DEFAULT-NEXT:                 write<d64>(%14, const<d64>(777777777.));
// DEFAULT-NEXT:                 let %93: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%13), const<d64>(777777777.))
// DEFAULT-NEXT:                     write<bool>(%93, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%93, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%13)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%14)), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%92, read<bool>(%93));
// DEFAULT-NEXT:             let %94: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%92)
// DEFAULT-NEXT:                 write<bool>(%94, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%13, call<d64, signature=fn(u128) -> d64>(%2, widen<u128, reason=arg>(const<u64>(9999999999999000))));
// DEFAULT-NEXT:                 call<d64, signature=fn(u128) -> d64>(%2, widen<u128, reason=arg>(const<u64>(9999999999999000)));
// DEFAULT-NEXT:                 write<d64>(%14, const<d64>(9999999999999000.));
// DEFAULT-NEXT:                 let %95: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%13), const<d64>(9999999999999000.))
// DEFAULT-NEXT:                     write<bool>(%95, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%95, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%13)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%14)), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%94, read<bool>(%95));
// DEFAULT-NEXT:             let %96: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%94)
// DEFAULT-NEXT:                 write<bool>(%96, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%13, call<d64, signature=fn(u128) -> d64>(%2, widen<u128, reason=arg>(const<u64>(999999999999999900))));
// DEFAULT-NEXT:                 call<d64, signature=fn(u128) -> d64>(%2, widen<u128, reason=arg>(const<u64>(999999999999999900)));
// DEFAULT-NEXT:                 write<d64>(%14, const<d64>(9999999999999999.e+2));
// DEFAULT-NEXT:                 let %97: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%13), const<d64>(9999999999999999.e+2))
// DEFAULT-NEXT:                     write<bool>(%97, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%97, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%13)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%14)), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%96, read<bool>(%97));
// DEFAULT-NEXT:             let %98: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%96)
// DEFAULT-NEXT:                 write<bool>(%98, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%13, call<d64, signature=fn(u128) -> d64>(%2, widen<u128, reason=arg>(const<u64>(9999999999999999000))));
// DEFAULT-NEXT:                 call<d64, signature=fn(u128) -> d64>(%2, widen<u128, reason=arg>(const<u64>(9999999999999999000)));
// DEFAULT-NEXT:                 write<d64>(%14, const<d64>(9999999999999999.e+3));
// DEFAULT-NEXT:                 let %99: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%13), const<d64>(9999999999999999.e+3))
// DEFAULT-NEXT:                     write<bool>(%99, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%99, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%13)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%14)), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%98, read<bool>(%99));
// DEFAULT-NEXT:             let %100: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%98)
// DEFAULT-NEXT:                 write<bool>(%100, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%13, call<d64, signature=fn(u128) -> d64>(%2, or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(5)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(7766279631452236919)))));
// DEFAULT-NEXT:                 call<d64, signature=fn(u128) -> d64>(%2, or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(5)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(7766279631452236919))));
// DEFAULT-NEXT:                 write<d64>(%14, const<d64>(9999999999999999.e+4));
// DEFAULT-NEXT:                 let %101: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%13), const<d64>(9999999999999999.e+4))
// DEFAULT-NEXT:                     write<bool>(%101, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%101, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%13)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%14)), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%100, read<bool>(%101));
// DEFAULT-NEXT:             let %102: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%100)
// DEFAULT-NEXT:                 write<bool>(%102, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%13, call<d64, signature=fn(u128) -> d64>(%2, or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(54)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(3875820019684112736)))));
// DEFAULT-NEXT:                 call<d64, signature=fn(u128) -> d64>(%2, or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(54)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(3875820019684112736))));
// DEFAULT-NEXT:                 write<d64>(%14, const<d64>(9999999999999999.e+5));
// DEFAULT-NEXT:                 let %103: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%13), const<d64>(9999999999999999.e+5))
// DEFAULT-NEXT:                     write<bool>(%103, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%103, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%13)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%14)), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%102, read<bool>(%103));
// DEFAULT-NEXT:             let %104: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%102)
// DEFAULT-NEXT:                 write<bool>(%104, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%13, call<d64, signature=fn(u128) -> d64>(%2, or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(542)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(1864712049422024128)))));
// DEFAULT-NEXT:                 call<d64, signature=fn(u128) -> d64>(%2, or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(542)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(1864712049422024128))));
// DEFAULT-NEXT:                 write<d64>(%14, const<d64>(9999999999999999.e+6));
// DEFAULT-NEXT:                 let %105: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%13), const<d64>(9999999999999999.e+6))
// DEFAULT-NEXT:                     write<bool>(%105, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%105, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%13)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%14)), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%104, read<bool>(%105));
// DEFAULT-NEXT:             let %106: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%104)
// DEFAULT-NEXT:                 write<bool>(%106, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%13, call<d64, signature=fn(u128) -> d64>(%2, or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(6692)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(11177671081280585728)))));
// DEFAULT-NEXT:                 call<d64, signature=fn(u128) -> d64>(%2, or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(6692)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(11177671081280585728))));
// DEFAULT-NEXT:                 write<d64>(%14, const<d64>(1234567890123456.e+8));
// DEFAULT-NEXT:                 let %107: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%13), const<d64>(1234567890123456.e+8))
// DEFAULT-NEXT:                     write<bool>(%107, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%107, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%13)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%14)), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%106, read<bool>(%107));
// DEFAULT-NEXT:             let %108: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%106)
// DEFAULT-NEXT:                 write<bool>(%108, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%13, call<d64, signature=fn(u128) -> d64>(%2, or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(1856279878857143160)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(11766139157678653440)))));
// DEFAULT-NEXT:                 call<d64, signature=fn(u128) -> d64>(%2, or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(1856279878857143160)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(11766139157678653440))));
// DEFAULT-NEXT:                 write<d64>(%14, const<d64>(3424231985445429e+22));
// DEFAULT-NEXT:                 let %109: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%13), const<d64>(3424231985445429e+22))
// DEFAULT-NEXT:                     write<bool>(%109, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%109, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%13)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%14)), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%108, read<bool>(%109));
// DEFAULT-NEXT:             let %110: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%108)
// DEFAULT-NEXT:                 write<bool>(%110, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%13, call<d64, signature=fn(u128) -> d64>(%2, or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(3355255724659993925)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(2264493734966067200)))));
// DEFAULT-NEXT:                 call<d64, signature=fn(u128) -> d64>(%2, or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(3355255724659993925)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(2264493734966067200))));
// DEFAULT-NEXT:                 write<d64>(%14, const<d64>(6189354365465179e+22));
// DEFAULT-NEXT:                 let %111: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%13), const<d64>(6189354365465179e+22))
// DEFAULT-NEXT:                     write<bool>(%111, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%111, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%13)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%14)), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%110, read<bool>(%111));
// DEFAULT-NEXT:             let %112: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%110)
// DEFAULT-NEXT:                 write<bool>(%112, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%13, call<d64, signature=fn(u128) -> d64>(%2, or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(3355255724659994196)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(3196849759677579264)))));
// DEFAULT-NEXT:                 call<d64, signature=fn(u128) -> d64>(%2, or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(3355255724659994196)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(3196849759677579264))));
// DEFAULT-NEXT:                 write<d64>(%14, const<d64>(6189354365465180e+22));
// DEFAULT-NEXT:                 let %113: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%13), const<d64>(6189354365465180e+22))
// DEFAULT-NEXT:                     write<bool>(%113, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%113, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%13)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%14)), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%112, read<bool>(%113));
// DEFAULT-NEXT:             let %114: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%112)
// DEFAULT-NEXT:                 write<bool>(%114, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%13, call<d64, signature=fn(u128) -> d64>(%2, or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(3355255724659994196)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(3196849759677579263)))));
// DEFAULT-NEXT:                 call<d64, signature=fn(u128) -> d64>(%2, or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(3355255724659994196)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(3196849759677579263))));
// DEFAULT-NEXT:                 write<d64>(%14, const<d64>(6189354365465179e+22));
// DEFAULT-NEXT:                 let %115: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%13), const<d64>(6189354365465179e+22))
// DEFAULT-NEXT:                     write<bool>(%115, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%115, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%13)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%14)), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%114, read<bool>(%115));
// DEFAULT-NEXT:             let %116: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%114)
// DEFAULT-NEXT:                 write<bool>(%116, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%13, call<d64, signature=fn(u128) -> d64>(%2, or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(3355255724659994467)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(4129205784389091327)))));
// DEFAULT-NEXT:                 call<d64, signature=fn(u128) -> d64>(%2, or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(3355255724659994467)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(4129205784389091327))));
// DEFAULT-NEXT:                 write<d64>(%14, const<d64>(6189354365465180e+22));
// DEFAULT-NEXT:                 let %117: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%13), const<d64>(6189354365465180e+22))
// DEFAULT-NEXT:                     write<bool>(%117, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%117, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%13)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%14)), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%116, read<bool>(%117));
// DEFAULT-NEXT:             let %118: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%116)
// DEFAULT-NEXT:                 write<bool>(%118, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%13, call<d64, signature=fn(u128) -> d64>(%2, or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(3355255724659993383)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(399781685543043072)))));
// DEFAULT-NEXT:                 call<d64, signature=fn(u128) -> d64>(%2, or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(3355255724659993383)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(399781685543043072))));
// DEFAULT-NEXT:                 write<d64>(%14, const<d64>(6189354365465178e+22));
// DEFAULT-NEXT:                 let %119: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%13), const<d64>(6189354365465178e+22))
// DEFAULT-NEXT:                     write<bool>(%119, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%119, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%13)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%14)), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%118, read<bool>(%119));
// DEFAULT-NEXT:             let %120: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%118)
// DEFAULT-NEXT:                 write<bool>(%120, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%13, call<d64, signature=fn(u128) -> d64>(%2, or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(3355255724659993654)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(1332137710254555136)))));
// DEFAULT-NEXT:                 call<d64, signature=fn(u128) -> d64>(%2, or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(3355255724659993654)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(1332137710254555136))));
// DEFAULT-NEXT:                 write<d64>(%14, const<d64>(6189354365465178e+22));
// DEFAULT-NEXT:                 let %121: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%13), const<d64>(6189354365465178e+22))
// DEFAULT-NEXT:                     write<bool>(%121, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%121, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%13)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%14)), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%120, read<bool>(%121));
// DEFAULT-NEXT:             let %122: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%120)
// DEFAULT-NEXT:                 write<bool>(%122, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%13, call<d64, signature=fn(u128) -> d64>(%2, or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(3355255724659993654)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(1332137710254555137)))));
// DEFAULT-NEXT:                 call<d64, signature=fn(u128) -> d64>(%2, or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(3355255724659993654)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(1332137710254555137))));
// DEFAULT-NEXT:                 write<d64>(%14, const<d64>(6189354365465179e+22));
// DEFAULT-NEXT:                 let %123: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%13), const<d64>(6189354365465179e+22))
// DEFAULT-NEXT:                     write<bool>(%123, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%123, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%13)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%14)), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%122, read<bool>(%123));
// DEFAULT-NEXT:             let %124: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%122)
// DEFAULT-NEXT:                 write<bool>(%124, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%13, call<d64, signature=fn(u128) -> d64>(%2, or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(5421010862427521627)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(17269431575687200768)))));
// DEFAULT-NEXT:                 call<d64, signature=fn(u128) -> d64>(%2, or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(5421010862427521627)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(17269431575687200768))));
// DEFAULT-NEXT:                 write<d64>(%14, const<d64>(9999999999999999.e+22));
// DEFAULT-NEXT:                 let %125: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%13), const<d64>(9999999999999999.e+22))
// DEFAULT-NEXT:                     write<bool>(%125, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%125, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%13)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%14)), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%124, read<bool>(%125));
// DEFAULT-NEXT:             let %126: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%124)
// DEFAULT-NEXT:                 write<bool>(%126, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%13, call<d64, signature=fn(u128) -> d64>(%2, or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(18446744073709548175)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(11871750202798899200)))));
// DEFAULT-NEXT:                 call<d64, signature=fn(u128) -> d64>(%2, or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(18446744073709548175)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(11871750202798899200))));
// DEFAULT-NEXT:                 write<d64>(%14, const<d64>(340282366920938.4e+24));
// DEFAULT-NEXT:                 let %127: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%13), const<d64>(340282366920938.4e+24))
// DEFAULT-NEXT:                     write<bool>(%127, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%127, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%13)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%14)), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%126, read<bool>(%127));
// DEFAULT-NEXT:             let %128: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%126)
// DEFAULT-NEXT:                 write<bool>(%128, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%13, call<d64, signature=fn(u128) -> d64>(%2, or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(18446744073709550886)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(2748566376204468223)))));
// DEFAULT-NEXT:                 call<d64, signature=fn(u128) -> d64>(%2, or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(18446744073709550886)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(2748566376204468223))));
// DEFAULT-NEXT:                 write<d64>(%14, const<d64>(340282366920938.4e+24));
// DEFAULT-NEXT:                 let %129: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%13), const<d64>(340282366920938.4e+24))
// DEFAULT-NEXT:                     write<bool>(%129, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%129, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%13)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%14)), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%128, read<bool>(%129));
// DEFAULT-NEXT:             let %130: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%128)
// DEFAULT-NEXT:                 write<bool>(%130, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%13, call<d64, signature=fn(u128) -> d64>(%2, or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(18446744073709550886)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(2748566376204468224)))));
// DEFAULT-NEXT:                 call<d64, signature=fn(u128) -> d64>(%2, or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(18446744073709550886)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(2748566376204468224))));
// DEFAULT-NEXT:                 write<d64>(%14, const<d64>(340282366920938.4e+24));
// DEFAULT-NEXT:                 let %131: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%13), const<d64>(340282366920938.4e+24))
// DEFAULT-NEXT:                     write<bool>(%131, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%131, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%13)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%14)), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%130, read<bool>(%131));
// DEFAULT-NEXT:             let %132: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%130)
// DEFAULT-NEXT:                 write<bool>(%132, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%13, call<d64, signature=fn(u128) -> d64>(%2, or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(18446744073709550886)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(2748566376204468225)))));
// DEFAULT-NEXT:                 call<d64, signature=fn(u128) -> d64>(%2, or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(18446744073709550886)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(2748566376204468225))));
// DEFAULT-NEXT:                 write<d64>(%14, const<d64>(340282366920938.5e+24));
// DEFAULT-NEXT:                 let %133: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%13), const<d64>(340282366920938.5e+24))
// DEFAULT-NEXT:                     write<bool>(%133, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%133, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%13)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%14)), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%132, read<bool>(%133));
// DEFAULT-NEXT:             let %134: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%132)
// DEFAULT-NEXT:                 write<bool>(%134, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%13, call<d64, signature=fn(u128) -> d64>(%2, or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(18446744073709550886)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(2748566386204468224)))));
// DEFAULT-NEXT:                 call<d64, signature=fn(u128) -> d64>(%2, or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(18446744073709550886)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(2748566386204468224))));
// DEFAULT-NEXT:                 write<d64>(%14, const<d64>(340282366920938.5e+24));
// DEFAULT-NEXT:                 let %135: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%13), const<d64>(340282366920938.5e+24))
// DEFAULT-NEXT:                     write<bool>(%135, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%135, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%13)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%14)), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%134, read<bool>(%135));
// DEFAULT-NEXT:             let %136: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%134)
// DEFAULT-NEXT:                 write<bool>(%136, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%13, call<d64, signature=fn(u128) -> d64>(%2, not<u128>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(0))))));
// DEFAULT-NEXT:                 call<d64, signature=fn(u128) -> d64>(%2, not<u128>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(0)))));
// DEFAULT-NEXT:                 write<d64>(%14, const<d64>(340282366920938.5e+24));
// DEFAULT-NEXT:                 let %137: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%13), const<d64>(340282366920938.5e+24))
// DEFAULT-NEXT:                     write<bool>(%137, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%137, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%13)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%14)), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%136, read<bool>(%137));
// DEFAULT-NEXT:             if read<bool>(%136)
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%23);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %15 a: d32 [storage=automatic];
// DEFAULT-NEXT:             let %16 b: d32 [storage=automatic];
// DEFAULT-NEXT:             write<d32>(%15, call<d32, signature=fn(i128) -> d32>(%4, widen<i128, reason=arg>(const<i64>(0))));
// DEFAULT-NEXT:             call<d32, signature=fn(i128) -> d32>(%4, widen<i128, reason=arg>(const<i64>(0)));
// DEFAULT-NEXT:             write<d32>(%16, const<d32>(0.));
// DEFAULT-NEXT:             let %138: bool [synthetic];
// DEFAULT-NEXT:             if ne<d32, exceptions=observable>(read<d32>(%15), const<d32>(0.))
// DEFAULT-NEXT:                 write<bool>(%138, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%138, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%15)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%16)), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:             let %139: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%138)
// DEFAULT-NEXT:                 write<bool>(%139, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%15, call<d32, signature=fn(i128) -> d32>(%4, widen<i128, reason=arg>(const<i64>(7))));
// DEFAULT-NEXT:                 call<d32, signature=fn(i128) -> d32>(%4, widen<i128, reason=arg>(const<i64>(7)));
// DEFAULT-NEXT:                 write<d32>(%16, const<d32>(7.));
// DEFAULT-NEXT:                 let %140: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%15), const<d32>(7.))
// DEFAULT-NEXT:                     write<bool>(%140, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%140, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%15)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%16)), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%139, read<bool>(%140));
// DEFAULT-NEXT:             let %141: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%139)
// DEFAULT-NEXT:                 write<bool>(%141, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%15, call<d32, signature=fn(i128) -> d32>(%4, widen<i128, reason=arg>(neg<i64, overflow=ub>(const<i64>(42)))));
// DEFAULT-NEXT:                 call<d32, signature=fn(i128) -> d32>(%4, widen<i128, reason=arg>(neg<i64, overflow=ub>(const<i64>(42))));
// DEFAULT-NEXT:                 write<d32>(%16, neg<d32>(const<d32>(42.)));
// DEFAULT-NEXT:                 let %142: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%15), neg<d32>(const<d32>(42.)))
// DEFAULT-NEXT:                     write<bool>(%142, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%142, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%15)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%16)), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%141, read<bool>(%142));
// DEFAULT-NEXT:             let %143: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%141)
// DEFAULT-NEXT:                 write<bool>(%143, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%15, call<d32, signature=fn(i128) -> d32>(%4, widen<i128, reason=arg>(neg<i64, overflow=ub>(const<i64>(777777)))));
// DEFAULT-NEXT:                 call<d32, signature=fn(i128) -> d32>(%4, widen<i128, reason=arg>(neg<i64, overflow=ub>(const<i64>(777777))));
// DEFAULT-NEXT:                 write<d32>(%16, neg<d32>(const<d32>(777777.)));
// DEFAULT-NEXT:                 let %144: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%15), neg<d32>(const<d32>(777777.)))
// DEFAULT-NEXT:                     write<bool>(%144, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%144, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%15)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%16)), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%143, read<bool>(%144));
// DEFAULT-NEXT:             let %145: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%143)
// DEFAULT-NEXT:                 write<bool>(%145, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%15, call<d32, signature=fn(i128) -> d32>(%4, widen<i128, reason=arg>(const<i64>(9999000))));
// DEFAULT-NEXT:                 call<d32, signature=fn(i128) -> d32>(%4, widen<i128, reason=arg>(const<i64>(9999000)));
// DEFAULT-NEXT:                 write<d32>(%16, const<d32>(9999000.));
// DEFAULT-NEXT:                 let %146: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%15), const<d32>(9999000.))
// DEFAULT-NEXT:                     write<bool>(%146, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%146, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%15)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%16)), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%145, read<bool>(%146));
// DEFAULT-NEXT:             let %147: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%145)
// DEFAULT-NEXT:                 write<bool>(%147, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%15, call<d32, signature=fn(i128) -> d32>(%4, widen<i128, reason=arg>(neg<i64, overflow=ub>(const<i64>(9999999)))));
// DEFAULT-NEXT:                 call<d32, signature=fn(i128) -> d32>(%4, widen<i128, reason=arg>(neg<i64, overflow=ub>(const<i64>(9999999))));
// DEFAULT-NEXT:                 write<d32>(%16, neg<d32>(const<d32>(9999999.)));
// DEFAULT-NEXT:                 let %148: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%15), neg<d32>(const<d32>(9999999.)))
// DEFAULT-NEXT:                     write<bool>(%148, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%148, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%15)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%16)), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%147, read<bool>(%148));
// DEFAULT-NEXT:             let %149: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%147)
// DEFAULT-NEXT:                 write<bool>(%149, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%15, call<d32, signature=fn(i128) -> d32>(%4, widen<i128, reason=arg>(const<i64>(99999994))));
// DEFAULT-NEXT:                 call<d32, signature=fn(i128) -> d32>(%4, widen<i128, reason=arg>(const<i64>(99999994)));
// DEFAULT-NEXT:                 write<d32>(%16, const<d32>(9999999.e+1));
// DEFAULT-NEXT:                 let %150: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%15), const<d32>(9999999.e+1))
// DEFAULT-NEXT:                     write<bool>(%150, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%150, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%15)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%16)), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%149, read<bool>(%150));
// DEFAULT-NEXT:             let %151: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%149)
// DEFAULT-NEXT:                 write<bool>(%151, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%15, call<d32, signature=fn(i128) -> d32>(%4, widen<i128, reason=arg>(const<i64>(99999995))));
// DEFAULT-NEXT:                 call<d32, signature=fn(i128) -> d32>(%4, widen<i128, reason=arg>(const<i64>(99999995)));
// DEFAULT-NEXT:                 write<d32>(%16, const<d32>(1000000.e+2));
// DEFAULT-NEXT:                 let %152: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%15), const<d32>(1000000.e+2))
// DEFAULT-NEXT:                     write<bool>(%152, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%152, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%15)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%16)), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%151, read<bool>(%152));
// DEFAULT-NEXT:             let %153: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%151)
// DEFAULT-NEXT:                 write<bool>(%153, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%15, call<d32, signature=fn(i128) -> d32>(%4, widen<i128, reason=arg>(const<i64>(999999900))));
// DEFAULT-NEXT:                 call<d32, signature=fn(i128) -> d32>(%4, widen<i128, reason=arg>(const<i64>(999999900)));
// DEFAULT-NEXT:                 write<d32>(%16, const<d32>(9999999.e+2));
// DEFAULT-NEXT:                 let %154: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%15), const<d32>(9999999.e+2))
// DEFAULT-NEXT:                     write<bool>(%154, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%154, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%15)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%16)), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%153, read<bool>(%154));
// DEFAULT-NEXT:             let %155: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%153)
// DEFAULT-NEXT:                 write<bool>(%155, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%15, call<d32, signature=fn(i128) -> d32>(%4, widen<i128, reason=arg>(neg<i64, overflow=ub>(const<i64>(9999999000)))));
// DEFAULT-NEXT:                 call<d32, signature=fn(i128) -> d32>(%4, widen<i128, reason=arg>(neg<i64, overflow=ub>(const<i64>(9999999000))));
// DEFAULT-NEXT:                 write<d32>(%16, neg<d32>(const<d32>(9999999.e+3)));
// DEFAULT-NEXT:                 let %156: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%15), neg<d32>(const<d32>(9999999.e+3)))
// DEFAULT-NEXT:                     write<bool>(%156, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%156, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%15)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%16)), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%155, read<bool>(%156));
// DEFAULT-NEXT:             let %157: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%155)
// DEFAULT-NEXT:                 write<bool>(%157, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%15, call<d32, signature=fn(i128) -> d32>(%4, widen<i128, reason=arg>(const<i64>(999999900000))));
// DEFAULT-NEXT:                 call<d32, signature=fn(i128) -> d32>(%4, widen<i128, reason=arg>(const<i64>(999999900000)));
// DEFAULT-NEXT:                 write<d32>(%16, const<d32>(9999999.e+5));
// DEFAULT-NEXT:                 let %158: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%15), const<d32>(9999999.e+5))
// DEFAULT-NEXT:                     write<bool>(%158, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%158, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%15)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%16)), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%157, read<bool>(%158));
// DEFAULT-NEXT:             let %159: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%157)
// DEFAULT-NEXT:                 write<bool>(%159, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%15, call<d32, signature=fn(i128) -> d32>(%4, widen<i128, reason=arg>(neg<i64, overflow=ub>(const<i64>(9999999000000)))));
// DEFAULT-NEXT:                 call<d32, signature=fn(i128) -> d32>(%4, widen<i128, reason=arg>(neg<i64, overflow=ub>(const<i64>(9999999000000))));
// DEFAULT-NEXT:                 write<d32>(%16, neg<d32>(const<d32>(9999999.e+6)));
// DEFAULT-NEXT:                 let %160: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%15), neg<d32>(const<d32>(9999999.e+6)))
// DEFAULT-NEXT:                     write<bool>(%160, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%160, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%15)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%16)), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%159, read<bool>(%160));
// DEFAULT-NEXT:             let %161: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%159)
// DEFAULT-NEXT:                 write<bool>(%161, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%15, call<d32, signature=fn(i128) -> d32>(%4, widen<i128, reason=arg>(const<i64>(123456700000000000))));
// DEFAULT-NEXT:                 call<d32, signature=fn(i128) -> d32>(%4, widen<i128, reason=arg>(const<i64>(123456700000000000)));
// DEFAULT-NEXT:                 write<d32>(%16, const<d32>(1234567.e+11));
// DEFAULT-NEXT:                 let %162: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%15), const<d32>(1234567.e+11))
// DEFAULT-NEXT:                     write<bool>(%162, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%162, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%15)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%16)), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%161, read<bool>(%162));
// DEFAULT-NEXT:             let %163: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%161)
// DEFAULT-NEXT:                 write<bool>(%163, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%15, call<d32, signature=fn(i128) -> d32>(%4, or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(1856279344))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(11918545879717380096))))));
// DEFAULT-NEXT:                 call<d32, signature=fn(i128) -> d32>(%4, or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(1856279344))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(11918545879717380096)))));
// DEFAULT-NEXT:                 write<d32>(%16, const<d32>(3424231e+22));
// DEFAULT-NEXT:                 let %164: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%15), const<d32>(3424231e+22))
// DEFAULT-NEXT:                     write<bool>(%164, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%164, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%15)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%16)), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%163, read<bool>(%164));
// DEFAULT-NEXT:             let %165: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%163)
// DEFAULT-NEXT:                 write<bool>(%165, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%15, call<d32, signature=fn(i128) -> d32>(%4, or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(5421010320326435927))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(5258189069476691968))))));
// DEFAULT-NEXT:                 call<d32, signature=fn(i128) -> d32>(%4, or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(5421010320326435927))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(5258189069476691968)))));
// DEFAULT-NEXT:                 write<d32>(%16, const<d32>(9999999.e+31));
// DEFAULT-NEXT:                 let %166: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%15), const<d32>(9999999.e+31))
// DEFAULT-NEXT:                     write<bool>(%166, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%166, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%15)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%16)), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%165, read<bool>(%166));
// DEFAULT-NEXT:             let %167: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%165)
// DEFAULT-NEXT:                 write<bool>(%167, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%15, call<d32, signature=fn(i128) -> d32>(%4, neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(5421010320326435927))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(5258189069476691968)))))));
// DEFAULT-NEXT:                 call<d32, signature=fn(i128) -> d32>(%4, neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(5421010320326435927))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(5258189069476691968))))));
// DEFAULT-NEXT:                 write<d32>(%16, neg<d32>(const<d32>(9999999.e+31)));
// DEFAULT-NEXT:                 let %168: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%15), neg<d32>(const<d32>(9999999.e+31)))
// DEFAULT-NEXT:                     write<bool>(%168, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%168, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%15)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%16)), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%167, read<bool>(%168));
// DEFAULT-NEXT:             let %169: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%167)
// DEFAULT-NEXT:                 write<bool>(%169, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%15, call<d32, signature=fn(i128) -> d32>(%4, neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(1159310820085523996))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(7849755482431422464)))))));
// DEFAULT-NEXT:                 call<d32, signature=fn(i128) -> d32>(%4, neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(1159310820085523996))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(7849755482431422464))))));
// DEFAULT-NEXT:                 write<d32>(%16, neg<d32>(const<d32>(2138551e+31)));
// DEFAULT-NEXT:                 let %170: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%15), neg<d32>(const<d32>(2138551e+31)))
// DEFAULT-NEXT:                     write<bool>(%170, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%170, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%15)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%16)), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%169, read<bool>(%170));
// DEFAULT-NEXT:             let %171: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%169)
// DEFAULT-NEXT:                 write<bool>(%171, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%15, call<d32, signature=fn(i128) -> d32>(%4, neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(1159311091136067117))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(14787732760248188928)))))));
// DEFAULT-NEXT:                 call<d32, signature=fn(i128) -> d32>(%4, neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(1159311091136067117))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(14787732760248188928))))));
// DEFAULT-NEXT:                 write<d32>(%16, neg<d32>(const<d32>(2138552e+31)));
// DEFAULT-NEXT:                 let %172: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%15), neg<d32>(const<d32>(2138552e+31)))
// DEFAULT-NEXT:                     write<bool>(%172, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%172, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%15)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%16)), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%171, read<bool>(%172));
// DEFAULT-NEXT:             let %173: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%171)
// DEFAULT-NEXT:                 write<bool>(%173, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%15, call<d32, signature=fn(i128) -> d32>(%4, neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(1159311091136067117))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(14787732760248188927)))))));
// DEFAULT-NEXT:                 call<d32, signature=fn(i128) -> d32>(%4, neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(1159311091136067117))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(14787732760248188927))))));
// DEFAULT-NEXT:                 write<d32>(%16, neg<d32>(const<d32>(2138551e+31)));
// DEFAULT-NEXT:                 let %174: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%15), neg<d32>(const<d32>(2138551e+31)))
// DEFAULT-NEXT:                     write<bool>(%174, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%174, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%15)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%16)), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%173, read<bool>(%174));
// DEFAULT-NEXT:             let %175: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%173)
// DEFAULT-NEXT:                 write<bool>(%175, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%15, call<d32, signature=fn(i128) -> d32>(%4, neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(1159311362186610239))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(3278965964355403775)))))));
// DEFAULT-NEXT:                 call<d32, signature=fn(i128) -> d32>(%4, neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(1159311362186610239))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(3278965964355403775))))));
// DEFAULT-NEXT:                 write<d32>(%16, neg<d32>(const<d32>(2138552e+31)));
// DEFAULT-NEXT:                 let %176: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%15), neg<d32>(const<d32>(2138552e+31)))
// DEFAULT-NEXT:                     write<bool>(%176, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%176, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%15)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%16)), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%175, read<bool>(%176));
// DEFAULT-NEXT:             let %177: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%175)
// DEFAULT-NEXT:                 write<bool>(%177, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%15, call<d32, signature=fn(i128) -> d32>(%4, neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(1159311362186610239))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(3278965964355403776)))))));
// DEFAULT-NEXT:                 call<d32, signature=fn(i128) -> d32>(%4, neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(1159311362186610239))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(3278965964355403776))))));
// DEFAULT-NEXT:                 write<d32>(%16, neg<d32>(const<d32>(2138552e+31)));
// DEFAULT-NEXT:                 let %178: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%15), neg<d32>(const<d32>(2138552e+31)))
// DEFAULT-NEXT:                     write<bool>(%178, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%178, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%15)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%16)), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%177, read<bool>(%178));
// DEFAULT-NEXT:             let %179: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%177)
// DEFAULT-NEXT:                 write<bool>(%179, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%15, call<d32, signature=fn(i128) -> d32>(%4, neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(1159311633237153360))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(10216943242172170240)))))));
// DEFAULT-NEXT:                 call<d32, signature=fn(i128) -> d32>(%4, neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(1159311633237153360))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(10216943242172170240))))));
// DEFAULT-NEXT:                 write<d32>(%16, neg<d32>(const<d32>(2138552e+31)));
// DEFAULT-NEXT:                 let %180: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%15), neg<d32>(const<d32>(2138552e+31)))
// DEFAULT-NEXT:                     write<bool>(%180, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%180, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%15)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%16)), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%179, read<bool>(%180));
// DEFAULT-NEXT:             let %181: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%179)
// DEFAULT-NEXT:                 write<bool>(%181, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%15, call<d32, signature=fn(i128) -> d32>(%4, neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(1159311633237153360))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(10216943242172170241)))))));
// DEFAULT-NEXT:                 call<d32, signature=fn(i128) -> d32>(%4, neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(1159311633237153360))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(10216943242172170241))))));
// DEFAULT-NEXT:                 write<d32>(%16, neg<d32>(const<d32>(2138553e+31)));
// DEFAULT-NEXT:                 let %182: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%15), neg<d32>(const<d32>(2138553e+31)))
// DEFAULT-NEXT:                     write<bool>(%182, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%182, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%15)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%16)), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%181, read<bool>(%182));
// DEFAULT-NEXT:             let %183: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%181)
// DEFAULT-NEXT:                 write<bool>(%183, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%15, call<d32, signature=fn(i128) -> d32>(%4, or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(9223367512453672922))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(15592504947059458048))))));
// DEFAULT-NEXT:                 call<d32, signature=fn(i128) -> d32>(%4, or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(9223367512453672922))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(15592504947059458048)))));
// DEFAULT-NEXT:                 write<d32>(%16, const<d32>(1701411e+32));
// DEFAULT-NEXT:                 let %184: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%15), const<d32>(1701411e+32))
// DEFAULT-NEXT:                     write<bool>(%184, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%184, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%15)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%16)), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%183, read<bool>(%184));
// DEFAULT-NEXT:             let %185: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%183)
// DEFAULT-NEXT:                 write<bool>(%185, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%15, call<d32, signature=fn(i128) -> d32>(%4, reinterpret<i128, reason=explicit, fits=unknown>(sub<u128, overflow=wrap>(shl<u128, overflow=wrap, amount_out_of_range=ub>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(1))), const<i32>(127)), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(1)))))));
// DEFAULT-NEXT:                 call<d32, signature=fn(i128) -> d32>(%4, reinterpret<i128, reason=explicit, fits=unknown>(sub<u128, overflow=wrap>(shl<u128, overflow=wrap, amount_out_of_range=ub>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(1))), const<i32>(127)), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(1))))));
// DEFAULT-NEXT:                 write<d32>(%16, const<d32>(1701412e+32));
// DEFAULT-NEXT:                 let %186: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%15), const<d32>(1701412e+32))
// DEFAULT-NEXT:                     write<bool>(%186, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%186, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%15)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%16)), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%185, read<bool>(%186));
// DEFAULT-NEXT:             let %187: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%185)
// DEFAULT-NEXT:                 write<bool>(%187, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%15, call<d32, signature=fn(i128) -> d32>(%4, neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(9223367512453672922))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(15592504947059458048)))))));
// DEFAULT-NEXT:                 call<d32, signature=fn(i128) -> d32>(%4, neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(9223367512453672922))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(15592504947059458048))))));
// DEFAULT-NEXT:                 write<d32>(%16, neg<d32>(const<d32>(1701411e+32)));
// DEFAULT-NEXT:                 let %188: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%15), neg<d32>(const<d32>(1701411e+32)))
// DEFAULT-NEXT:                     write<bool>(%188, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%188, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%15)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%16)), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%187, read<bool>(%188));
// DEFAULT-NEXT:             let %189: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%187)
// DEFAULT-NEXT:                 write<bool>(%189, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%15, call<d32, signature=fn(i128) -> d32>(%4, sub<i128, overflow=ub>(neg<i128, overflow=ub>(reinterpret<i128, reason=explicit, fits=unknown>(sub<u128, overflow=wrap>(shl<u128, overflow=wrap, amount_out_of_range=ub>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(1))), const<i32>(127)), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(1)))))), widen<i128, reason=usual_arith>(const<i64>(1)))));
// DEFAULT-NEXT:                 call<d32, signature=fn(i128) -> d32>(%4, sub<i128, overflow=ub>(neg<i128, overflow=ub>(reinterpret<i128, reason=explicit, fits=unknown>(sub<u128, overflow=wrap>(shl<u128, overflow=wrap, amount_out_of_range=ub>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(1))), const<i32>(127)), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(1)))))), widen<i128, reason=usual_arith>(const<i64>(1))));
// DEFAULT-NEXT:                 write<d32>(%16, neg<d32>(const<d32>(1701412e+32)));
// DEFAULT-NEXT:                 let %190: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%15), neg<d32>(const<d32>(1701412e+32)))
// DEFAULT-NEXT:                     write<bool>(%190, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%190, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%15)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%16)), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%189, read<bool>(%190));
// DEFAULT-NEXT:             if read<bool>(%189)
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%23);
// DEFAULT-NEXT:             write<d32>(%15, call<d32, signature=fn(u128) -> d32>(%6, widen<u128, reason=arg>(const<u64>(0))));
// DEFAULT-NEXT:             call<d32, signature=fn(u128) -> d32>(%6, widen<u128, reason=arg>(const<u64>(0)));
// DEFAULT-NEXT:             write<d32>(%16, const<d32>(0.));
// DEFAULT-NEXT:             let %191: bool [synthetic];
// DEFAULT-NEXT:             if ne<d32, exceptions=observable>(read<d32>(%15), const<d32>(0.))
// DEFAULT-NEXT:                 write<bool>(%191, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%191, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%15)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%16)), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:             let %192: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%191)
// DEFAULT-NEXT:                 write<bool>(%192, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%15, call<d32, signature=fn(u128) -> d32>(%6, widen<u128, reason=arg>(const<u64>(7))));
// DEFAULT-NEXT:                 call<d32, signature=fn(u128) -> d32>(%6, widen<u128, reason=arg>(const<u64>(7)));
// DEFAULT-NEXT:                 write<d32>(%16, const<d32>(7.));
// DEFAULT-NEXT:                 let %193: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%15), const<d32>(7.))
// DEFAULT-NEXT:                     write<bool>(%193, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%193, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%15)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%16)), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%192, read<bool>(%193));
// DEFAULT-NEXT:             let %194: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%192)
// DEFAULT-NEXT:                 write<bool>(%194, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%15, call<d32, signature=fn(u128) -> d32>(%6, widen<u128, reason=arg>(const<u64>(42))));
// DEFAULT-NEXT:                 call<d32, signature=fn(u128) -> d32>(%6, widen<u128, reason=arg>(const<u64>(42)));
// DEFAULT-NEXT:                 write<d32>(%16, const<d32>(42.));
// DEFAULT-NEXT:                 let %195: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%15), const<d32>(42.))
// DEFAULT-NEXT:                     write<bool>(%195, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%195, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%15)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%16)), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%194, read<bool>(%195));
// DEFAULT-NEXT:             let %196: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%194)
// DEFAULT-NEXT:                 write<bool>(%196, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%15, call<d32, signature=fn(u128) -> d32>(%6, widen<u128, reason=arg>(const<u64>(77777))));
// DEFAULT-NEXT:                 call<d32, signature=fn(u128) -> d32>(%6, widen<u128, reason=arg>(const<u64>(77777)));
// DEFAULT-NEXT:                 write<d32>(%16, const<d32>(77777.));
// DEFAULT-NEXT:                 let %197: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%15), const<d32>(77777.))
// DEFAULT-NEXT:                     write<bool>(%197, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%197, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%15)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%16)), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%196, read<bool>(%197));
// DEFAULT-NEXT:             let %198: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%196)
// DEFAULT-NEXT:                 write<bool>(%198, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%15, call<d32, signature=fn(u128) -> d32>(%6, widen<u128, reason=arg>(const<u64>(9999000))));
// DEFAULT-NEXT:                 call<d32, signature=fn(u128) -> d32>(%6, widen<u128, reason=arg>(const<u64>(9999000)));
// DEFAULT-NEXT:                 write<d32>(%16, const<d32>(9999000.));
// DEFAULT-NEXT:                 let %199: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%15), const<d32>(9999000.))
// DEFAULT-NEXT:                     write<bool>(%199, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%199, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%15)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%16)), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%198, read<bool>(%199));
// DEFAULT-NEXT:             let %200: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%198)
// DEFAULT-NEXT:                 write<bool>(%200, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%15, call<d32, signature=fn(u128) -> d32>(%6, widen<u128, reason=arg>(const<u64>(999999900))));
// DEFAULT-NEXT:                 call<d32, signature=fn(u128) -> d32>(%6, widen<u128, reason=arg>(const<u64>(999999900)));
// DEFAULT-NEXT:                 write<d32>(%16, const<d32>(9999999.e+2));
// DEFAULT-NEXT:                 let %201: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%15), const<d32>(9999999.e+2))
// DEFAULT-NEXT:                     write<bool>(%201, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%201, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%15)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%16)), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%200, read<bool>(%201));
// DEFAULT-NEXT:             let %202: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%200)
// DEFAULT-NEXT:                 write<bool>(%202, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%15, call<d32, signature=fn(u128) -> d32>(%6, widen<u128, reason=arg>(const<u64>(999999949))));
// DEFAULT-NEXT:                 call<d32, signature=fn(u128) -> d32>(%6, widen<u128, reason=arg>(const<u64>(999999949)));
// DEFAULT-NEXT:                 write<d32>(%16, const<d32>(9999999.e+2));
// DEFAULT-NEXT:                 let %203: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%15), const<d32>(9999999.e+2))
// DEFAULT-NEXT:                     write<bool>(%203, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%203, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%15)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%16)), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%202, read<bool>(%203));
// DEFAULT-NEXT:             let %204: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%202)
// DEFAULT-NEXT:                 write<bool>(%204, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%15, call<d32, signature=fn(u128) -> d32>(%6, widen<u128, reason=arg>(const<u64>(9999999000))));
// DEFAULT-NEXT:                 call<d32, signature=fn(u128) -> d32>(%6, widen<u128, reason=arg>(const<u64>(9999999000)));
// DEFAULT-NEXT:                 write<d32>(%16, const<d32>(9999999.e+3));
// DEFAULT-NEXT:                 let %205: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%15), const<d32>(9999999.e+3))
// DEFAULT-NEXT:                     write<bool>(%205, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%205, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%15)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%16)), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%204, read<bool>(%205));
// DEFAULT-NEXT:             let %206: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%204)
// DEFAULT-NEXT:                 write<bool>(%206, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%15, call<d32, signature=fn(u128) -> d32>(%6, widen<u128, reason=arg>(const<u64>(9999999499))));
// DEFAULT-NEXT:                 call<d32, signature=fn(u128) -> d32>(%6, widen<u128, reason=arg>(const<u64>(9999999499)));
// DEFAULT-NEXT:                 write<d32>(%16, const<d32>(9999999.e+3));
// DEFAULT-NEXT:                 let %207: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%15), const<d32>(9999999.e+3))
// DEFAULT-NEXT:                     write<bool>(%207, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%207, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%15)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%16)), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%206, read<bool>(%207));
// DEFAULT-NEXT:             let %208: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%206)
// DEFAULT-NEXT:                 write<bool>(%208, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%15, call<d32, signature=fn(u128) -> d32>(%6, widen<u128, reason=arg>(const<u64>(999999900000))));
// DEFAULT-NEXT:                 call<d32, signature=fn(u128) -> d32>(%6, widen<u128, reason=arg>(const<u64>(999999900000)));
// DEFAULT-NEXT:                 write<d32>(%16, const<d32>(9999999.e+5));
// DEFAULT-NEXT:                 let %209: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%15), const<d32>(9999999.e+5))
// DEFAULT-NEXT:                     write<bool>(%209, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%209, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%15)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%16)), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%208, read<bool>(%209));
// DEFAULT-NEXT:             let %210: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%208)
// DEFAULT-NEXT:                 write<bool>(%210, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%15, call<d32, signature=fn(u128) -> d32>(%6, widen<u128, reason=arg>(const<u64>(9999999000000))));
// DEFAULT-NEXT:                 call<d32, signature=fn(u128) -> d32>(%6, widen<u128, reason=arg>(const<u64>(9999999000000)));
// DEFAULT-NEXT:                 write<d32>(%16, const<d32>(9999999.e+6));
// DEFAULT-NEXT:                 let %211: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%15), const<d32>(9999999.e+6))
// DEFAULT-NEXT:                     write<bool>(%211, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%211, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%15)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%16)), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%210, read<bool>(%211));
// DEFAULT-NEXT:             let %212: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%210)
// DEFAULT-NEXT:                 write<bool>(%212, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%15, call<d32, signature=fn(u128) -> d32>(%6, widen<u128, reason=arg>(const<u64>(123456700000000))));
// DEFAULT-NEXT:                 call<d32, signature=fn(u128) -> d32>(%6, widen<u128, reason=arg>(const<u64>(123456700000000)));
// DEFAULT-NEXT:                 write<d32>(%16, const<d32>(1234567.e+8));
// DEFAULT-NEXT:                 let %213: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%15), const<d32>(1234567.e+8))
// DEFAULT-NEXT:                     write<bool>(%213, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%213, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%15)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%16)), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%212, read<bool>(%213));
// DEFAULT-NEXT:             let %214: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%212)
// DEFAULT-NEXT:                 write<bool>(%214, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%15, call<d32, signature=fn(u128) -> d32>(%6, or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(1856279344)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(11918545879717380096)))));
// DEFAULT-NEXT:                 call<d32, signature=fn(u128) -> d32>(%6, or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(1856279344)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(11918545879717380096))));
// DEFAULT-NEXT:                 write<d32>(%16, const<d32>(3424231e+22));
// DEFAULT-NEXT:                 let %215: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%15), const<d32>(3424231e+22))
// DEFAULT-NEXT:                     write<bool>(%215, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%215, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%15)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%16)), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%214, read<bool>(%215));
// DEFAULT-NEXT:             let %216: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%214)
// DEFAULT-NEXT:                 write<bool>(%216, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%15, call<d32, signature=fn(u128) -> d32>(%6, or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(542101032032643592)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(13438539758544355328)))));
// DEFAULT-NEXT:                 call<d32, signature=fn(u128) -> d32>(%6, or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(542101032032643592)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(13438539758544355328))));
// DEFAULT-NEXT:                 write<d32>(%16, const<d32>(9999999.e+30));
// DEFAULT-NEXT:                 let %217: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%15), const<d32>(9999999.e+30))
// DEFAULT-NEXT:                     write<bool>(%217, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%217, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%15)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%16)), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%216, read<bool>(%217));
// DEFAULT-NEXT:             let %218: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%216)
// DEFAULT-NEXT:                 write<bool>(%218, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%15, call<d32, signature=fn(u128) -> d32>(%6, or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(3355258237046354619)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(1512024826179485696)))));
// DEFAULT-NEXT:                 call<d32, signature=fn(u128) -> d32>(%6, or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(3355258237046354619)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(1512024826179485696))));
// DEFAULT-NEXT:                 write<d32>(%16, const<d32>(6189359e+31));
// DEFAULT-NEXT:                 let %219: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%15), const<d32>(6189359e+31))
// DEFAULT-NEXT:                     write<bool>(%219, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%219, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%15)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%16)), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%218, read<bool>(%219));
// DEFAULT-NEXT:             let %220: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%218)
// DEFAULT-NEXT:                 write<bool>(%220, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%15, call<d32, signature=fn(u128) -> d32>(%6, or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(3355258508096897740)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(8450002103996252160)))));
// DEFAULT-NEXT:                 call<d32, signature=fn(u128) -> d32>(%6, or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(3355258508096897740)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(8450002103996252160))));
// DEFAULT-NEXT:                 write<d32>(%16, const<d32>(6189360e+31));
// DEFAULT-NEXT:                 let %221: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%15), const<d32>(6189360e+31))
// DEFAULT-NEXT:                     write<bool>(%221, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%221, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%15)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%16)), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%220, read<bool>(%221));
// DEFAULT-NEXT:             let %222: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%220)
// DEFAULT-NEXT:                 write<bool>(%222, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%15, call<d32, signature=fn(u128) -> d32>(%6, or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(3355258508096897740)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(8450002103996252159)))));
// DEFAULT-NEXT:                 call<d32, signature=fn(u128) -> d32>(%6, or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(3355258508096897740)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(8450002103996252159))));
// DEFAULT-NEXT:                 write<d32>(%16, const<d32>(6189359e+31));
// DEFAULT-NEXT:                 let %223: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%15), const<d32>(6189359e+31))
// DEFAULT-NEXT:                     write<bool>(%223, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%223, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%15)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%16)), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%222, read<bool>(%223));
// DEFAULT-NEXT:             let %224: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%222)
// DEFAULT-NEXT:                 write<bool>(%224, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%15, call<d32, signature=fn(u128) -> d32>(%6, or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(3355258779147440861)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(15387979381813018623)))));
// DEFAULT-NEXT:                 call<d32, signature=fn(u128) -> d32>(%6, or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(3355258779147440861)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(15387979381813018623))));
// DEFAULT-NEXT:                 write<d32>(%16, const<d32>(6189360e+31));
// DEFAULT-NEXT:                 let %225: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%15), const<d32>(6189360e+31))
// DEFAULT-NEXT:                     write<bool>(%225, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%225, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%15)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%16)), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%224, read<bool>(%225));
// DEFAULT-NEXT:             let %226: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%224)
// DEFAULT-NEXT:                 write<bool>(%226, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%15, call<d32, signature=fn(u128) -> d32>(%6, or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(3355257694945268376)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(6082814344255504384)))));
// DEFAULT-NEXT:                 call<d32, signature=fn(u128) -> d32>(%6, or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(3355257694945268376)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(6082814344255504384))));
// DEFAULT-NEXT:                 write<d32>(%16, const<d32>(6189358e+31));
// DEFAULT-NEXT:                 let %227: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%15), const<d32>(6189358e+31))
// DEFAULT-NEXT:                     write<bool>(%227, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%227, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%15)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%16)), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%226, read<bool>(%227));
// DEFAULT-NEXT:             let %228: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%226)
// DEFAULT-NEXT:                 write<bool>(%228, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%15, call<d32, signature=fn(u128) -> d32>(%6, or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(3355257965995811497)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(13020791622072270848)))));
// DEFAULT-NEXT:                 call<d32, signature=fn(u128) -> d32>(%6, or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(3355257965995811497)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(13020791622072270848))));
// DEFAULT-NEXT:                 write<d32>(%16, const<d32>(6189358e+31));
// DEFAULT-NEXT:                 let %229: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%15), const<d32>(6189358e+31))
// DEFAULT-NEXT:                     write<bool>(%229, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%229, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%15)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%16)), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%228, read<bool>(%229));
// DEFAULT-NEXT:             let %230: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%228)
// DEFAULT-NEXT:                 write<bool>(%230, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%15, call<d32, signature=fn(u128) -> d32>(%6, or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(3355257965995811497)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(13020791622072270849)))));
// DEFAULT-NEXT:                 call<d32, signature=fn(u128) -> d32>(%6, or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(3355257965995811497)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(13020791622072270849))));
// DEFAULT-NEXT:                 write<d32>(%16, const<d32>(6189359e+31));
// DEFAULT-NEXT:                 let %231: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%15), const<d32>(6189359e+31))
// DEFAULT-NEXT:                     write<bool>(%231, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%231, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%15)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%16)), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%230, read<bool>(%231));
// DEFAULT-NEXT:             let %232: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%230)
// DEFAULT-NEXT:                 write<bool>(%232, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%15, call<d32, signature=fn(u128) -> d32>(%6, or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(18446740445918208273)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(3923858787068280832)))));
// DEFAULT-NEXT:                 call<d32, signature=fn(u128) -> d32>(%6, or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(18446740445918208273)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(3923858787068280832))));
// DEFAULT-NEXT:                 write<d32>(%16, const<d32>(3402823e+32));
// DEFAULT-NEXT:                 let %233: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%15), const<d32>(3402823e+32))
// DEFAULT-NEXT:                     write<bool>(%233, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%233, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%15)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%16)), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%232, read<bool>(%233));
// DEFAULT-NEXT:             let %234: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%232)
// DEFAULT-NEXT:                 write<bool>(%234, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%15, call<d32, signature=fn(u128) -> d32>(%6, not<u128>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(0))))));
// DEFAULT-NEXT:                 call<d32, signature=fn(u128) -> d32>(%6, not<u128>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(0)))));
// DEFAULT-NEXT:                 write<d32>(%16, const<d32>(3402824e+32));
// DEFAULT-NEXT:                 let %235: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%15), const<d32>(3402824e+32))
// DEFAULT-NEXT:                     write<bool>(%235, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%235, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%15)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%16)), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%234, read<bool>(%235));
// DEFAULT-NEXT:             if read<bool>(%234)
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%23);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %17 a: d128 [storage=automatic];
// DEFAULT-NEXT:             let %18 b: d128 [storage=automatic];
// DEFAULT-NEXT:             write<d128>(%17, call<d128, signature=fn(i128) -> d128>(%8, widen<i128, reason=arg>(const<i64>(0))));
// DEFAULT-NEXT:             call<d128, signature=fn(i128) -> d128>(%8, widen<i128, reason=arg>(const<i64>(0)));
// DEFAULT-NEXT:             write<d128>(%18, const<d128>(0.));
// DEFAULT-NEXT:             let %236: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%17), const<d128>(0.))
// DEFAULT-NEXT:                 write<bool>(%236, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%236, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%17)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%18)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             let %237: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%236)
// DEFAULT-NEXT:                 write<bool>(%237, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d128>(%17, call<d128, signature=fn(i128) -> d128>(%8, widen<i128, reason=arg>(const<i64>(7))));
// DEFAULT-NEXT:                 call<d128, signature=fn(i128) -> d128>(%8, widen<i128, reason=arg>(const<i64>(7)));
// DEFAULT-NEXT:                 write<d128>(%18, const<d128>(7.));
// DEFAULT-NEXT:                 let %238: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d128, exceptions=observable>(read<d128>(%17), const<d128>(7.))
// DEFAULT-NEXT:                     write<bool>(%238, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%238, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%17)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%18)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%237, read<bool>(%238));
// DEFAULT-NEXT:             let %239: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%237)
// DEFAULT-NEXT:                 write<bool>(%239, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d128>(%17, call<d128, signature=fn(i128) -> d128>(%8, widen<i128, reason=arg>(neg<i64, overflow=ub>(const<i64>(42)))));
// DEFAULT-NEXT:                 call<d128, signature=fn(i128) -> d128>(%8, widen<i128, reason=arg>(neg<i64, overflow=ub>(const<i64>(42))));
// DEFAULT-NEXT:                 write<d128>(%18, neg<d128>(const<d128>(42.)));
// DEFAULT-NEXT:                 let %240: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d128, exceptions=observable>(read<d128>(%17), neg<d128>(const<d128>(42.)))
// DEFAULT-NEXT:                     write<bool>(%240, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%240, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%17)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%18)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%239, read<bool>(%240));
// DEFAULT-NEXT:             let %241: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%239)
// DEFAULT-NEXT:                 write<bool>(%241, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d128>(%17, call<d128, signature=fn(i128) -> d128>(%8, widen<i128, reason=arg>(neg<i64, overflow=ub>(const<i64>(777777777)))));
// DEFAULT-NEXT:                 call<d128, signature=fn(i128) -> d128>(%8, widen<i128, reason=arg>(neg<i64, overflow=ub>(const<i64>(777777777))));
// DEFAULT-NEXT:                 write<d128>(%18, neg<d128>(const<d128>(777777777.)));
// DEFAULT-NEXT:                 let %242: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d128, exceptions=observable>(read<d128>(%17), neg<d128>(const<d128>(777777777.)))
// DEFAULT-NEXT:                     write<bool>(%242, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%242, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%17)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%18)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%241, read<bool>(%242));
// DEFAULT-NEXT:             let %243: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%241)
// DEFAULT-NEXT:                 write<bool>(%243, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d128>(%17, call<d128, signature=fn(i128) -> d128>(%8, widen<i128, reason=arg>(neg<i64, overflow=ub>(const<i64>(12345678912345)))));
// DEFAULT-NEXT:                 call<d128, signature=fn(i128) -> d128>(%8, widen<i128, reason=arg>(neg<i64, overflow=ub>(const<i64>(12345678912345))));
// DEFAULT-NEXT:                 write<d128>(%18, neg<d128>(const<d128>(12345678912345.)));
// DEFAULT-NEXT:                 let %244: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d128, exceptions=observable>(read<d128>(%17), neg<d128>(const<d128>(12345678912345.)))
// DEFAULT-NEXT:                     write<bool>(%244, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%244, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%17)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%18)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%243, read<bool>(%244));
// DEFAULT-NEXT:             let %245: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%243)
// DEFAULT-NEXT:                 write<bool>(%245, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d128>(%17, call<d128, signature=fn(i128) -> d128>(%8, widen<i128, reason=arg>(const<i64>(123456789123456789))));
// DEFAULT-NEXT:                 call<d128, signature=fn(i128) -> d128>(%8, widen<i128, reason=arg>(const<i64>(123456789123456789)));
// DEFAULT-NEXT:                 write<d128>(%18, const<d128>(123456789123456789.));
// DEFAULT-NEXT:                 let %246: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d128, exceptions=observable>(read<d128>(%17), const<d128>(123456789123456789.))
// DEFAULT-NEXT:                     write<bool>(%246, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%246, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%17)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%18)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%245, read<bool>(%246));
// DEFAULT-NEXT:             let %247: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%245)
// DEFAULT-NEXT:                 write<bool>(%247, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d128>(%17, call<d128, signature=fn(i128) -> d128>(%8, or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(42163417))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(15105683216109345905))))));
// DEFAULT-NEXT:                 call<d128, signature=fn(i128) -> d128>(%8, or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(42163417))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(15105683216109345905)))));
// DEFAULT-NEXT:                 write<d128>(%18, const<d128>(777777777777777777777777777.));
// DEFAULT-NEXT:                 let %248: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d128, exceptions=observable>(read<d128>(%17), const<d128>(777777777777777777777777777.))
// DEFAULT-NEXT:                     write<bool>(%248, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%248, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%17)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%18)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%247, read<bool>(%248));
// DEFAULT-NEXT:             let %249: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%247)
// DEFAULT-NEXT:                 write<bool>(%249, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d128>(%17, call<d128, signature=fn(i128) -> d128>(%8, or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(542101086242752))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(4003012203850112768))))));
// DEFAULT-NEXT:                 call<d128, signature=fn(i128) -> d128>(%8, or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(542101086242752))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(4003012203850112768)))));
// DEFAULT-NEXT:                 write<d128>(%18, const<d128>(9999999999999999999999999900000000.));
// DEFAULT-NEXT:                 let %250: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d128, exceptions=observable>(read<d128>(%17), const<d128>(9999999999999999999999999900000000.))
// DEFAULT-NEXT:                     write<bool>(%250, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%250, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%17)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%18)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%249, read<bool>(%250));
// DEFAULT-NEXT:             let %251: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%249)
// DEFAULT-NEXT:                 write<bool>(%251, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d128>(%17, call<d128, signature=fn(i128) -> d128>(%8, neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(542101086242752))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(4003012203950112767)))))));
// DEFAULT-NEXT:                 call<d128, signature=fn(i128) -> d128>(%8, neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(542101086242752))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(4003012203950112767))))));
// DEFAULT-NEXT:                 write<d128>(%18, neg<d128>(const<d128>(9999999999999999999999999999999999.)));
// DEFAULT-NEXT:                 let %252: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d128, exceptions=observable>(read<d128>(%17), neg<d128>(const<d128>(9999999999999999999999999999999999.)))
// DEFAULT-NEXT:                     write<bool>(%252, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%252, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%17)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%18)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%251, read<bool>(%252));
// DEFAULT-NEXT:             let %253: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%251)
// DEFAULT-NEXT:                 write<bool>(%253, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d128>(%17, call<d128, signature=fn(i128) -> d128>(%8, neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(5421010862427522))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(3136633892082024442)))))));
// DEFAULT-NEXT:                 call<d128, signature=fn(i128) -> d128>(%8, neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(5421010862427522))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(3136633892082024442))))));
// DEFAULT-NEXT:                 write<d128>(%18, neg<d128>(const<d128>(9999999999999999999999999999999999.e+1)));
// DEFAULT-NEXT:                 let %254: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d128, exceptions=observable>(read<d128>(%17), neg<d128>(const<d128>(9999999999999999999999999999999999.e+1)))
// DEFAULT-NEXT:                     write<bool>(%254, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%254, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%17)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%18)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%253, read<bool>(%254));
// DEFAULT-NEXT:             let %255: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%253)
// DEFAULT-NEXT:                 write<bool>(%255, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d128>(%17, call<d128, signature=fn(i128) -> d128>(%8, or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(5421010862427522))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(3136633892082024443))))));
// DEFAULT-NEXT:                 call<d128, signature=fn(i128) -> d128>(%8, or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(5421010862427522))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(3136633892082024443)))));
// DEFAULT-NEXT:                 write<d128>(%18, const<d128>(1000000000000000000000000000000000.e+2));
// DEFAULT-NEXT:                 let %256: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d128, exceptions=observable>(read<d128>(%17), const<d128>(1000000000000000000000000000000000.e+2))
// DEFAULT-NEXT:                     write<bool>(%256, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%256, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%17)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%18)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%255, read<bool>(%256));
// DEFAULT-NEXT:             let %257: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%255)
// DEFAULT-NEXT:                 write<bool>(%257, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d128>(%17, call<d128, signature=fn(i128) -> d128>(%8, or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(54210108624275221))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(12919594847110692764))))));
// DEFAULT-NEXT:                 call<d128, signature=fn(i128) -> d128>(%8, or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(54210108624275221))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(12919594847110692764)))));
// DEFAULT-NEXT:                 write<d128>(%18, const<d128>(9999999999999999999999999999999999.e+2));
// DEFAULT-NEXT:                 let %258: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d128, exceptions=observable>(read<d128>(%17), const<d128>(9999999999999999999999999999999999.e+2))
// DEFAULT-NEXT:                     write<bool>(%258, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%258, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%17)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%18)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%257, read<bool>(%258));
// DEFAULT-NEXT:             let %259: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%257)
// DEFAULT-NEXT:                 write<bool>(%259, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d128>(%17, call<d128, signature=fn(i128) -> d128>(%8, or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(54210108624275221))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(12919594847110692813))))));
// DEFAULT-NEXT:                 call<d128, signature=fn(i128) -> d128>(%8, or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(54210108624275221))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(12919594847110692813)))));
// DEFAULT-NEXT:                 write<d128>(%18, const<d128>(9999999999999999999999999999999999.e+2));
// DEFAULT-NEXT:                 let %260: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d128, exceptions=observable>(read<d128>(%17), const<d128>(9999999999999999999999999999999999.e+2))
// DEFAULT-NEXT:                     write<bool>(%260, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%260, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%17)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%18)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%259, read<bool>(%260));
// DEFAULT-NEXT:             let %261: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%259)
// DEFAULT-NEXT:                 write<bool>(%261, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d128>(%17, call<d128, signature=fn(i128) -> d128>(%8, neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(542101086242752217))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(68739955140066328)))))));
// DEFAULT-NEXT:                 call<d128, signature=fn(i128) -> d128>(%8, neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(542101086242752217))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(68739955140066328))))));
// DEFAULT-NEXT:                 write<d128>(%18, neg<d128>(const<d128>(9999999999999999999999999999999999.e+3)));
// DEFAULT-NEXT:                 let %262: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d128, exceptions=observable>(read<d128>(%17), neg<d128>(const<d128>(9999999999999999999999999999999999.e+3)))
// DEFAULT-NEXT:                     write<bool>(%262, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%262, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%17)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%18)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%261, read<bool>(%262));
// DEFAULT-NEXT:             let %263: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%261)
// DEFAULT-NEXT:                 write<bool>(%263, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d128>(%17, call<d128, signature=fn(i128) -> d128>(%8, or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(542101086242752217))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(68739955140066827))))));
// DEFAULT-NEXT:                 call<d128, signature=fn(i128) -> d128>(%8, or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(542101086242752217))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(68739955140066827)))));
// DEFAULT-NEXT:                 write<d128>(%18, const<d128>(9999999999999999999999999999999999.e+3));
// DEFAULT-NEXT:                 let %264: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d128, exceptions=observable>(read<d128>(%17), const<d128>(9999999999999999999999999999999999.e+3))
// DEFAULT-NEXT:                     write<bool>(%264, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%264, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%17)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%18)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%263, read<bool>(%264));
// DEFAULT-NEXT:             let %265: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%263)
// DEFAULT-NEXT:                 write<bool>(%265, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d128>(%17, call<d128, signature=fn(i128) -> d128>(%8, or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(9223372036854775807))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(18446744073709545888))))));
// DEFAULT-NEXT:                 call<d128, signature=fn(i128) -> d128>(%8, or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(9223372036854775807))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(18446744073709545888)))));
// DEFAULT-NEXT:                 write<d128>(%18, const<d128>(1701411834604692317316873037158841.e+5));
// DEFAULT-NEXT:                 let %266: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d128, exceptions=observable>(read<d128>(%17), const<d128>(1701411834604692317316873037158841.e+5))
// DEFAULT-NEXT:                     write<bool>(%266, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%266, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%17)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%18)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%265, read<bool>(%266));
// DEFAULT-NEXT:             let %267: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%265)
// DEFAULT-NEXT:                 write<bool>(%267, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d128>(%17, call<d128, signature=fn(i128) -> d128>(%8, reinterpret<i128, reason=explicit, fits=unknown>(sub<u128, overflow=wrap>(shl<u128, overflow=wrap, amount_out_of_range=ub>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(1))), const<i32>(127)), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(1)))))));
// DEFAULT-NEXT:                 call<d128, signature=fn(i128) -> d128>(%8, reinterpret<i128, reason=explicit, fits=unknown>(sub<u128, overflow=wrap>(shl<u128, overflow=wrap, amount_out_of_range=ub>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(1))), const<i32>(127)), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(1))))));
// DEFAULT-NEXT:                 write<d128>(%18, const<d128>(1701411834604692317316873037158841.e+5));
// DEFAULT-NEXT:                 let %268: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d128, exceptions=observable>(read<d128>(%17), const<d128>(1701411834604692317316873037158841.e+5))
// DEFAULT-NEXT:                     write<bool>(%268, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%268, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%17)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%18)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%267, read<bool>(%268));
// DEFAULT-NEXT:             let %269: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%267)
// DEFAULT-NEXT:                 write<bool>(%269, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d128>(%17, call<d128, signature=fn(i128) -> d128>(%8, sub<i128, overflow=ub>(neg<i128, overflow=ub>(reinterpret<i128, reason=explicit, fits=unknown>(sub<u128, overflow=wrap>(shl<u128, overflow=wrap, amount_out_of_range=ub>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(1))), const<i32>(127)), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(1)))))), widen<i128, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                 call<d128, signature=fn(i128) -> d128>(%8, sub<i128, overflow=ub>(neg<i128, overflow=ub>(reinterpret<i128, reason=explicit, fits=unknown>(sub<u128, overflow=wrap>(shl<u128, overflow=wrap, amount_out_of_range=ub>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(1))), const<i32>(127)), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(1)))))), widen<i128, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                 write<d128>(%18, neg<d128>(const<d128>(1701411834604692317316873037158841.e+5)));
// DEFAULT-NEXT:                 let %270: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d128, exceptions=observable>(read<d128>(%17), neg<d128>(const<d128>(1701411834604692317316873037158841.e+5)))
// DEFAULT-NEXT:                     write<bool>(%270, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%270, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%17)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%18)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%269, read<bool>(%270));
// DEFAULT-NEXT:             if read<bool>(%269)
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%23);
// DEFAULT-NEXT:             write<d128>(%17, call<d128, signature=fn(u128) -> d128>(%10, widen<u128, reason=arg>(const<u64>(0))));
// DEFAULT-NEXT:             call<d128, signature=fn(u128) -> d128>(%10, widen<u128, reason=arg>(const<u64>(0)));
// DEFAULT-NEXT:             write<d128>(%18, const<d128>(0.));
// DEFAULT-NEXT:             let %271: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%17), const<d128>(0.))
// DEFAULT-NEXT:                 write<bool>(%271, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%271, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%17)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%18)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             let %272: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%271)
// DEFAULT-NEXT:                 write<bool>(%272, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d128>(%17, call<d128, signature=fn(u128) -> d128>(%10, widen<u128, reason=arg>(const<u64>(7))));
// DEFAULT-NEXT:                 call<d128, signature=fn(u128) -> d128>(%10, widen<u128, reason=arg>(const<u64>(7)));
// DEFAULT-NEXT:                 write<d128>(%18, const<d128>(7.));
// DEFAULT-NEXT:                 let %273: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d128, exceptions=observable>(read<d128>(%17), const<d128>(7.))
// DEFAULT-NEXT:                     write<bool>(%273, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%273, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%17)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%18)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%272, read<bool>(%273));
// DEFAULT-NEXT:             let %274: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%272)
// DEFAULT-NEXT:                 write<bool>(%274, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d128>(%17, call<d128, signature=fn(u128) -> d128>(%10, widen<u128, reason=arg>(const<u64>(42))));
// DEFAULT-NEXT:                 call<d128, signature=fn(u128) -> d128>(%10, widen<u128, reason=arg>(const<u64>(42)));
// DEFAULT-NEXT:                 write<d128>(%18, const<d128>(42.));
// DEFAULT-NEXT:                 let %275: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d128, exceptions=observable>(read<d128>(%17), const<d128>(42.))
// DEFAULT-NEXT:                     write<bool>(%275, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%275, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%17)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%18)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%274, read<bool>(%275));
// DEFAULT-NEXT:             let %276: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%274)
// DEFAULT-NEXT:                 write<bool>(%276, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d128>(%17, call<d128, signature=fn(u128) -> d128>(%10, widen<u128, reason=arg>(const<u64>(777777777))));
// DEFAULT-NEXT:                 call<d128, signature=fn(u128) -> d128>(%10, widen<u128, reason=arg>(const<u64>(777777777)));
// DEFAULT-NEXT:                 write<d128>(%18, const<d128>(777777777.));
// DEFAULT-NEXT:                 let %277: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d128, exceptions=observable>(read<d128>(%17), const<d128>(777777777.))
// DEFAULT-NEXT:                     write<bool>(%277, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%277, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%17)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%18)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%276, read<bool>(%277));
// DEFAULT-NEXT:             let %278: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%276)
// DEFAULT-NEXT:                 write<bool>(%278, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d128>(%17, call<d128, signature=fn(u128) -> d128>(%10, or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(5421010862)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(7886392056514346008)))));
// DEFAULT-NEXT:                 call<d128, signature=fn(u128) -> d128>(%10, or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(5421010862)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(7886392056514346008))));
// DEFAULT-NEXT:                 write<d128>(%18, const<d128>(99999999999999999999999999000.));
// DEFAULT-NEXT:                 let %279: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d128, exceptions=observable>(read<d128>(%17), const<d128>(99999999999999999999999999000.))
// DEFAULT-NEXT:                     write<bool>(%279, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%279, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%17)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%18)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%278, read<bool>(%279));
// DEFAULT-NEXT:             let %280: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%278)
// DEFAULT-NEXT:                 write<bool>(%280, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d128>(%17, call<d128, signature=fn(u128) -> d128>(%10, or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(54210108624275221)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(12919594847110692764)))));
// DEFAULT-NEXT:                 call<d128, signature=fn(u128) -> d128>(%10, or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(54210108624275221)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(12919594847110692764))));
// DEFAULT-NEXT:                 write<d128>(%18, const<d128>(9999999999999999999999999999999999.e+2));
// DEFAULT-NEXT:                 let %281: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d128, exceptions=observable>(read<d128>(%17), const<d128>(9999999999999999999999999999999999.e+2))
// DEFAULT-NEXT:                     write<bool>(%281, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%281, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%17)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%18)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%280, read<bool>(%281));
// DEFAULT-NEXT:             let %282: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%280)
// DEFAULT-NEXT:                 write<bool>(%282, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d128>(%17, call<d128, signature=fn(u128) -> d128>(%10, or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(542101086242752217)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(68739955140066328)))));
// DEFAULT-NEXT:                 call<d128, signature=fn(u128) -> d128>(%10, or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(542101086242752217)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(68739955140066328))));
// DEFAULT-NEXT:                 write<d128>(%18, const<d128>(9999999999999999999999999999999999.e+3));
// DEFAULT-NEXT:                 let %283: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d128, exceptions=observable>(read<d128>(%17), const<d128>(9999999999999999999999999999999999.e+3))
// DEFAULT-NEXT:                     write<bool>(%283, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%283, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%17)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%18)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%282, read<bool>(%283));
// DEFAULT-NEXT:             let %284: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%282)
// DEFAULT-NEXT:                 write<bool>(%284, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d128>(%17, call<d128, signature=fn(u128) -> d128>(%10, or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(5421010862427522170)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(687399551400668279)))));
// DEFAULT-NEXT:                 call<d128, signature=fn(u128) -> d128>(%10, or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(5421010862427522170)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(687399551400668279))));
// DEFAULT-NEXT:                 write<d128>(%18, const<d128>(9999999999999999999999999999999999.e+4));
// DEFAULT-NEXT:                 let %285: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d128, exceptions=observable>(read<d128>(%17), const<d128>(9999999999999999999999999999999999.e+4))
// DEFAULT-NEXT:                     write<bool>(%285, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%285, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%17)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%18)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%284, read<bool>(%285));
// DEFAULT-NEXT:             let %286: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%284)
// DEFAULT-NEXT:                 write<bool>(%286, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d128>(%17, call<d128, signature=fn(u128) -> d128>(%10, or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(18446744073709551615)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(18446744073709540160)))));
// DEFAULT-NEXT:                 call<d128, signature=fn(u128) -> d128>(%10, or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(18446744073709551615)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(18446744073709540160))));
// DEFAULT-NEXT:                 write<d128>(%18, const<d128>(3402823669209384634633746074317682.e+5));
// DEFAULT-NEXT:                 let %287: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d128, exceptions=observable>(read<d128>(%17), const<d128>(3402823669209384634633746074317682.e+5))
// DEFAULT-NEXT:                     write<bool>(%287, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%287, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%17)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%18)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%286, read<bool>(%287));
// DEFAULT-NEXT:             let %288: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%286)
// DEFAULT-NEXT:                 write<bool>(%288, const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d128>(%17, call<d128, signature=fn(u128) -> d128>(%10, not<u128>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(0))))));
// DEFAULT-NEXT:                 call<d128, signature=fn(u128) -> d128>(%10, not<u128>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(0)))));
// DEFAULT-NEXT:                 write<d128>(%18, const<d128>(3402823669209384634633746074317682.e+5));
// DEFAULT-NEXT:                 let %289: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d128, exceptions=observable>(read<d128>(%17), const<d128>(3402823669209384634633746074317682.e+5))
// DEFAULT-NEXT:                     write<bool>(%289, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%289, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%22, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%17)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%18)), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%288, read<bool>(%289));
// DEFAULT-NEXT:             if read<bool>(%288)
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%23);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
