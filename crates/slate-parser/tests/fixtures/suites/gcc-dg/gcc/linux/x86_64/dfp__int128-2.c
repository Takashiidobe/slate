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
// DEFAULT-NEXT:     fn %[[VALUE_tests64:[0-9]+]] @tests64(%[[VALUE_b:[0-9]+]] b: i128) -> d64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return int_to_float<d64, reason=return, exact=false, rounding=nearest_even, exceptions=observable>(read<i128>(%[[VALUE_b]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testu64:[0-9]+]] @testu64(%[[VALUE_b_2:[0-9]+]] b: u128) -> d64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return int_to_float<d64, reason=return, exact=false, rounding=nearest_even, exceptions=observable>(read<u128>(%[[VALUE_b_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_tests32:[0-9]+]] @tests32(%[[VALUE_b_3:[0-9]+]] b: i128) -> d32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return int_to_float<d32, reason=return, exact=false, rounding=nearest_even, exceptions=observable>(read<i128>(%[[VALUE_b_3]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testu32:[0-9]+]] @testu32(%[[VALUE_b_4:[0-9]+]] b: u128) -> d32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return int_to_float<d32, reason=return, exact=false, rounding=nearest_even, exceptions=observable>(read<u128>(%[[VALUE_b_4]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_tests128:[0-9]+]] @tests128(%[[VALUE_b_5:[0-9]+]] b: i128) -> d128 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return int_to_float<d128, reason=return, exact=false, rounding=nearest_even, exceptions=observable>(read<i128>(%[[VALUE_b_5]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testu128:[0-9]+]] @testu128(%[[VALUE_b_6:[0-9]+]] b: u128) -> d128 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return int_to_float<d128, reason=return, exact=false, rounding=nearest_even, exceptions=observable>(read<u128>(%[[VALUE_b_6]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_memcmp:[0-9]+]] @__builtin_memcmp(%[[VALUE0:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE1:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE2:[0-9]+]] <unnamed>: u64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_a:[0-9]+]] a: d64 [storage=automatic];
// DEFAULT-NEXT:             let %[[VALUE_b_7:[0-9]+]] b: d64 [storage=automatic];
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(i128) -> d64>(%[[VALUE_tests64]], widen<i128, reason=arg>(const<i64>(0))));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_7]], const<d64>(0.));
// DEFAULT-NEXT:             let %[[VALUE3:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(0.))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE3]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE3]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_7]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             let %[[VALUE4:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE3]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE4]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_a]], call<d64, signature=fn(i128) -> d64>(%[[VALUE_tests64]], widen<i128, reason=arg>(const<i64>(7))));
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_b_7]], const<d64>(7.));
// DEFAULT-NEXT:                 let %[[VALUE5:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(7.))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE5]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE5]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_7]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE4]], read<bool>(%[[VALUE5]]));
// DEFAULT-NEXT:             let %[[VALUE6:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE4]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE6]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_a]], call<d64, signature=fn(i128) -> d64>(%[[VALUE_tests64]], widen<i128, reason=arg>(neg<i64, overflow=ub>(const<i64>(42)))));
// DEFAULT-NEXT:                 let %[[VALUE7:[0-9]+]]: d64 [synthetic] = neg<d64>(const<d64>(42.));
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_b_7]], read<d64>(%[[VALUE7]]));
// DEFAULT-NEXT:                 let %[[VALUE8:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), neg<d64>(const<d64>(42.)))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE8]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE8]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_7]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE6]], read<bool>(%[[VALUE8]]));
// DEFAULT-NEXT:             let %[[VALUE9:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE6]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE9]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_a]], call<d64, signature=fn(i128) -> d64>(%[[VALUE_tests64]], widen<i128, reason=arg>(neg<i64, overflow=ub>(const<i64>(777777777)))));
// DEFAULT-NEXT:                 let %[[VALUE10:[0-9]+]]: d64 [synthetic] = neg<d64>(const<d64>(777777777.));
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_b_7]], read<d64>(%[[VALUE10]]));
// DEFAULT-NEXT:                 let %[[VALUE11:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), neg<d64>(const<d64>(777777777.)))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE11]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE11]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_7]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE9]], read<bool>(%[[VALUE11]]));
// DEFAULT-NEXT:             let %[[VALUE12:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE9]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE12]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_a]], call<d64, signature=fn(i128) -> d64>(%[[VALUE_tests64]], widen<i128, reason=arg>(const<i64>(9999999999999000))));
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_b_7]], const<d64>(9999999999999000.));
// DEFAULT-NEXT:                 let %[[VALUE13:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(9999999999999000.))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE13]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE13]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_7]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE12]], read<bool>(%[[VALUE13]]));
// DEFAULT-NEXT:             let %[[VALUE14:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE12]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE14]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_a]], call<d64, signature=fn(i128) -> d64>(%[[VALUE_tests64]], widen<i128, reason=arg>(neg<i64, overflow=ub>(const<i64>(9999999999999999)))));
// DEFAULT-NEXT:                 let %[[VALUE15:[0-9]+]]: d64 [synthetic] = neg<d64>(const<d64>(9999999999999999.));
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_b_7]], read<d64>(%[[VALUE15]]));
// DEFAULT-NEXT:                 let %[[VALUE16:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), neg<d64>(const<d64>(9999999999999999.)))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE16]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE16]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_7]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE14]], read<bool>(%[[VALUE16]]));
// DEFAULT-NEXT:             let %[[VALUE17:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE14]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE17]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_a]], call<d64, signature=fn(i128) -> d64>(%[[VALUE_tests64]], widen<i128, reason=arg>(neg<i64, overflow=ub>(const<i64>(99999999999999994)))));
// DEFAULT-NEXT:                 let %[[VALUE18:[0-9]+]]: d64 [synthetic] = neg<d64>(const<d64>(9999999999999999.e+1));
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_b_7]], read<d64>(%[[VALUE18]]));
// DEFAULT-NEXT:                 let %[[VALUE19:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), neg<d64>(const<d64>(9999999999999999.e+1)))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE19]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE19]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_7]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE17]], read<bool>(%[[VALUE19]]));
// DEFAULT-NEXT:             let %[[VALUE20:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE17]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE20]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_a]], call<d64, signature=fn(i128) -> d64>(%[[VALUE_tests64]], widen<i128, reason=arg>(const<i64>(99999999999999995))));
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_b_7]], const<d64>(1000000000000000.e+2));
// DEFAULT-NEXT:                 let %[[VALUE21:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(1000000000000000.e+2))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE21]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE21]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_7]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE20]], read<bool>(%[[VALUE21]]));
// DEFAULT-NEXT:             let %[[VALUE22:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE20]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE22]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_a]], call<d64, signature=fn(i128) -> d64>(%[[VALUE_tests64]], widen<i128, reason=arg>(const<i64>(999999999999999900))));
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_b_7]], const<d64>(9999999999999999.e+2));
// DEFAULT-NEXT:                 let %[[VALUE23:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(9999999999999999.e+2))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE23]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE23]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_7]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE22]], read<bool>(%[[VALUE23]]));
// DEFAULT-NEXT:             let %[[VALUE24:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE22]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE24]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_a]], call<d64, signature=fn(i128) -> d64>(%[[VALUE_tests64]], widen<i128, reason=arg>(const<i64>(999999999999999949))));
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_b_7]], const<d64>(9999999999999999.e+2));
// DEFAULT-NEXT:                 let %[[VALUE25:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(9999999999999999.e+2))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE25]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE25]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_7]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE24]], read<bool>(%[[VALUE25]]));
// DEFAULT-NEXT:             let %[[VALUE26:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE24]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE26]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_a]], call<d64, signature=fn(i128) -> d64>(%[[VALUE_tests64]], neg<i128, overflow=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(9999999999999999000))))));
// DEFAULT-NEXT:                 let %[[VALUE27:[0-9]+]]: d64 [synthetic] = neg<d64>(const<d64>(9999999999999999.e+3));
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_b_7]], read<d64>(%[[VALUE27]]));
// DEFAULT-NEXT:                 let %[[VALUE28:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), neg<d64>(const<d64>(9999999999999999.e+3)))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE28]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE28]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_7]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE26]], read<bool>(%[[VALUE28]]));
// DEFAULT-NEXT:             let %[[VALUE29:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE26]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE29]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_a]], call<d64, signature=fn(i128) -> d64>(%[[VALUE_tests64]], reinterpret<i128, reason=arg, fits=unknown>(widen<u128, reason=arg>(const<u64>(9999999999999999499)))));
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_b_7]], const<d64>(9999999999999999.e+3));
// DEFAULT-NEXT:                 let %[[VALUE30:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(9999999999999999.e+3))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE30]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE30]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_7]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE29]], read<bool>(%[[VALUE30]]));
// DEFAULT-NEXT:             let %[[VALUE31:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE29]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE31]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_a]], call<d64, signature=fn(i128) -> d64>(%[[VALUE_tests64]], or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(54))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(3875820019684112736))))));
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_b_7]], const<d64>(9999999999999999.e+5));
// DEFAULT-NEXT:                 let %[[VALUE32:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(9999999999999999.e+5))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE32]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE32]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_7]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE31]], read<bool>(%[[VALUE32]]));
// DEFAULT-NEXT:             let %[[VALUE33:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE31]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE33]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_a]], call<d64, signature=fn(i128) -> d64>(%[[VALUE_tests64]], or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(54))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(3875820019684162735))))));
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_b_7]], const<d64>(9999999999999999.e+5));
// DEFAULT-NEXT:                 let %[[VALUE34:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(9999999999999999.e+5))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE34]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE34]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_7]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE33]], read<bool>(%[[VALUE34]]));
// DEFAULT-NEXT:             let %[[VALUE35:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE33]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE35]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_a]], call<d64, signature=fn(i128) -> d64>(%[[VALUE_tests64]], neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(542))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(1864712049422024128)))))));
// DEFAULT-NEXT:                 let %[[VALUE36:[0-9]+]]: d64 [synthetic] = neg<d64>(const<d64>(9999999999999999.e+6));
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_b_7]], read<d64>(%[[VALUE36]]));
// DEFAULT-NEXT:                 let %[[VALUE37:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), neg<d64>(const<d64>(9999999999999999.e+6)))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE37]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE37]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_7]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE35]], read<bool>(%[[VALUE37]]));
// DEFAULT-NEXT:             let %[[VALUE38:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE35]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE38]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_a]], call<d64, signature=fn(i128) -> d64>(%[[VALUE_tests64]], neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(542))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(1864712049422524127)))))));
// DEFAULT-NEXT:                 let %[[VALUE39:[0-9]+]]: d64 [synthetic] = neg<d64>(const<d64>(9999999999999999.e+6));
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_b_7]], read<d64>(%[[VALUE39]]));
// DEFAULT-NEXT:                 let %[[VALUE40:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), neg<d64>(const<d64>(9999999999999999.e+6)))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE40]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE40]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_7]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE38]], read<bool>(%[[VALUE40]]));
// DEFAULT-NEXT:             let %[[VALUE41:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE38]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE41]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_a]], call<d64, signature=fn(i128) -> d64>(%[[VALUE_tests64]], or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(6692))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(11177671081280585728))))));
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_b_7]], const<d64>(1234567890123456.e+8));
// DEFAULT-NEXT:                 let %[[VALUE42:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(1234567890123456.e+8))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE42]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE42]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_7]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE41]], read<bool>(%[[VALUE42]]));
// DEFAULT-NEXT:             let %[[VALUE43:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE41]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE43]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_a]], call<d64, signature=fn(i128) -> d64>(%[[VALUE_tests64]], or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(185627987885714316))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(1176613915767865344))))));
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_b_7]], const<d64>(3424231985445429e+21));
// DEFAULT-NEXT:                 let %[[VALUE44:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(3424231985445429e+21))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE44]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE44]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_7]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE43]], read<bool>(%[[VALUE44]]));
// DEFAULT-NEXT:             let %[[VALUE45:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE43]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE45]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_a]], call<d64, signature=fn(i128) -> d64>(%[[VALUE_tests64]], or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(5421010862427521627))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(17269431575687200768))))));
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_b_7]], const<d64>(9999999999999999.e+22));
// DEFAULT-NEXT:                 let %[[VALUE46:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(9999999999999999.e+22))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE46]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE46]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_7]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE45]], read<bool>(%[[VALUE46]]));
// DEFAULT-NEXT:             let %[[VALUE47:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE45]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE47]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_a]], call<d64, signature=fn(i128) -> d64>(%[[VALUE_tests64]], or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(5421010862427521898))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(18201787600398712831))))));
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_b_7]], const<d64>(9999999999999999.e+22));
// DEFAULT-NEXT:                 let %[[VALUE48:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(9999999999999999.e+22))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE48]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE48]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_7]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE47]], read<bool>(%[[VALUE48]]));
// DEFAULT-NEXT:             let %[[VALUE49:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE47]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE49]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_a]], call<d64, signature=fn(i128) -> d64>(%[[VALUE_tests64]], neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(5421010862427521627))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(17269431575687200768)))))));
// DEFAULT-NEXT:                 let %[[VALUE50:[0-9]+]]: d64 [synthetic] = neg<d64>(const<d64>(9999999999999999.e+22));
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_b_7]], read<d64>(%[[VALUE50]]));
// DEFAULT-NEXT:                 let %[[VALUE51:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), neg<d64>(const<d64>(9999999999999999.e+22)))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE51]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE51]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_7]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE49]], read<bool>(%[[VALUE51]]));
// DEFAULT-NEXT:             let %[[VALUE52:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE49]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE52]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_a]], call<d64, signature=fn(i128) -> d64>(%[[VALUE_tests64]], neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(1159310753783557366))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(15090707038725996544)))))));
// DEFAULT-NEXT:                 let %[[VALUE53:[0-9]+]]: d64 [synthetic] = neg<d64>(const<d64>(2138550877694459e+22));
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_b_7]], read<d64>(%[[VALUE53]]));
// DEFAULT-NEXT:                 let %[[VALUE54:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), neg<d64>(const<d64>(2138550877694459e+22)))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE54]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE54]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_7]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE52]], read<bool>(%[[VALUE54]]));
// DEFAULT-NEXT:             let %[[VALUE55:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE52]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE55]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_a]], call<d64, signature=fn(i128) -> d64>(%[[VALUE_tests64]], neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(1159310753783557637))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(16023063063437508608)))))));
// DEFAULT-NEXT:                 let %[[VALUE56:[0-9]+]]: d64 [synthetic] = neg<d64>(const<d64>(2138550877694460e+22));
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_b_7]], read<d64>(%[[VALUE56]]));
// DEFAULT-NEXT:                 let %[[VALUE57:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), neg<d64>(const<d64>(2138550877694460e+22)))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE57]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE57]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_7]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE55]], read<bool>(%[[VALUE57]]));
// DEFAULT-NEXT:             let %[[VALUE58:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE55]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE58]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_a]], call<d64, signature=fn(i128) -> d64>(%[[VALUE_tests64]], neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(1159310753783557637))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(16023063063437508607)))))));
// DEFAULT-NEXT:                 let %[[VALUE59:[0-9]+]]: d64 [synthetic] = neg<d64>(const<d64>(2138550877694459e+22));
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_b_7]], read<d64>(%[[VALUE59]]));
// DEFAULT-NEXT:                 let %[[VALUE60:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), neg<d64>(const<d64>(2138550877694459e+22)))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE60]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE60]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_7]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE58]], read<bool>(%[[VALUE60]]));
// DEFAULT-NEXT:             let %[[VALUE61:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE58]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE61]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_a]], call<d64, signature=fn(i128) -> d64>(%[[VALUE_tests64]], neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(1159310753783557908))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(16955419088149020671)))))));
// DEFAULT-NEXT:                 let %[[VALUE62:[0-9]+]]: d64 [synthetic] = neg<d64>(const<d64>(2138550877694460e+22));
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_b_7]], read<d64>(%[[VALUE62]]));
// DEFAULT-NEXT:                 let %[[VALUE63:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), neg<d64>(const<d64>(2138550877694460e+22)))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE63]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE63]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_7]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE61]], read<bool>(%[[VALUE63]]));
// DEFAULT-NEXT:             let %[[VALUE64:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE61]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE64]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_a]], call<d64, signature=fn(i128) -> d64>(%[[VALUE_tests64]], neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(1159310753783556824))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(13225994989302972416)))))));
// DEFAULT-NEXT:                 let %[[VALUE65:[0-9]+]]: d64 [synthetic] = neg<d64>(const<d64>(2138550877694458e+22));
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_b_7]], read<d64>(%[[VALUE65]]));
// DEFAULT-NEXT:                 let %[[VALUE66:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), neg<d64>(const<d64>(2138550877694458e+22)))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE66]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE66]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_7]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE64]], read<bool>(%[[VALUE66]]));
// DEFAULT-NEXT:             let %[[VALUE67:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE64]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE67]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_a]], call<d64, signature=fn(i128) -> d64>(%[[VALUE_tests64]], neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(1159310753783557095))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(14158351014014484480)))))));
// DEFAULT-NEXT:                 let %[[VALUE68:[0-9]+]]: d64 [synthetic] = neg<d64>(const<d64>(2138550877694458e+22));
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_b_7]], read<d64>(%[[VALUE68]]));
// DEFAULT-NEXT:                 let %[[VALUE69:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), neg<d64>(const<d64>(2138550877694458e+22)))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE69]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE69]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_7]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE67]], read<bool>(%[[VALUE69]]));
// DEFAULT-NEXT:             let %[[VALUE70:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE67]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE70]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_a]], call<d64, signature=fn(i128) -> d64>(%[[VALUE_tests64]], neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(1159310753783557095))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(14158351014014484481)))))));
// DEFAULT-NEXT:                 let %[[VALUE71:[0-9]+]]: d64 [synthetic] = neg<d64>(const<d64>(2138550877694459e+22));
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_b_7]], read<d64>(%[[VALUE71]]));
// DEFAULT-NEXT:                 let %[[VALUE72:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), neg<d64>(const<d64>(2138550877694459e+22)))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE72]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE72]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_7]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE70]], read<bool>(%[[VALUE72]]));
// DEFAULT-NEXT:             let %[[VALUE73:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE70]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE73]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_a]], call<d64, signature=fn(i128) -> d64>(%[[VALUE_tests64]], or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(9223372036854774087))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(15159247138254225408))))));
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_b_7]], const<d64>(1701411834604692e+23));
// DEFAULT-NEXT:                 let %[[VALUE74:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(1701411834604692e+23))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE74]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE74]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_7]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE73]], read<bool>(%[[VALUE74]]));
// DEFAULT-NEXT:             let %[[VALUE75:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE73]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE75]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_a]], call<d64, signature=fn(i128) -> d64>(%[[VALUE_tests64]], reinterpret<i128, reason=explicit, fits=unknown>(sub<u128, overflow=wrap>(shl<u128, overflow=wrap, amount_out_of_range=ub>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(1))), const<i32>(127)), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(1)))))));
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_b_7]], const<d64>(1701411834604692e+23));
// DEFAULT-NEXT:                 let %[[VALUE76:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(1701411834604692e+23))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE76]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE76]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_7]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE75]], read<bool>(%[[VALUE76]]));
// DEFAULT-NEXT:             let %[[VALUE77:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE75]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE77]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_a]], call<d64, signature=fn(i128) -> d64>(%[[VALUE_tests64]], neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(9223372036854774087))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(15159247138254225408)))))));
// DEFAULT-NEXT:                 let %[[VALUE78:[0-9]+]]: d64 [synthetic] = neg<d64>(const<d64>(1701411834604692e+23));
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_b_7]], read<d64>(%[[VALUE78]]));
// DEFAULT-NEXT:                 let %[[VALUE79:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), neg<d64>(const<d64>(1701411834604692e+23)))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE79]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE79]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_7]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE77]], read<bool>(%[[VALUE79]]));
// DEFAULT-NEXT:             let %[[VALUE80:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE77]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE80]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_a]], call<d64, signature=fn(i128) -> d64>(%[[VALUE_tests64]], sub<i128, overflow=ub>(neg<i128, overflow=ub>(reinterpret<i128, reason=explicit, fits=unknown>(sub<u128, overflow=wrap>(shl<u128, overflow=wrap, amount_out_of_range=ub>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(1))), const<i32>(127)), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(1)))))), widen<i128, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                 let %[[VALUE81:[0-9]+]]: d64 [synthetic] = neg<d64>(const<d64>(1701411834604692e+23));
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_b_7]], read<d64>(%[[VALUE81]]));
// DEFAULT-NEXT:                 let %[[VALUE82:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), neg<d64>(const<d64>(1701411834604692e+23)))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE82]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE82]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_7]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE80]], read<bool>(%[[VALUE82]]));
// DEFAULT-NEXT:             if read<bool>(%[[VALUE80]])
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:             write<d64>(%[[VALUE_a]], call<d64, signature=fn(u128) -> d64>(%[[VALUE_testu64]], widen<u128, reason=arg>(const<u64>(0))));
// DEFAULT-NEXT:             write<d64>(%[[VALUE_b_7]], const<d64>(0.));
// DEFAULT-NEXT:             let %[[VALUE83:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(0.))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE83]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE83]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_7]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:             let %[[VALUE84:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE83]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE84]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_a]], call<d64, signature=fn(u128) -> d64>(%[[VALUE_testu64]], widen<u128, reason=arg>(const<u64>(7))));
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_b_7]], const<d64>(7.));
// DEFAULT-NEXT:                 let %[[VALUE85:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(7.))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE85]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE85]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_7]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE84]], read<bool>(%[[VALUE85]]));
// DEFAULT-NEXT:             let %[[VALUE86:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE84]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE86]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_a]], call<d64, signature=fn(u128) -> d64>(%[[VALUE_testu64]], widen<u128, reason=arg>(const<u64>(42))));
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_b_7]], const<d64>(42.));
// DEFAULT-NEXT:                 let %[[VALUE87:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(42.))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE87]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE87]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_7]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE86]], read<bool>(%[[VALUE87]]));
// DEFAULT-NEXT:             let %[[VALUE88:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE86]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE88]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_a]], call<d64, signature=fn(u128) -> d64>(%[[VALUE_testu64]], widen<u128, reason=arg>(const<u64>(777777777))));
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_b_7]], const<d64>(777777777.));
// DEFAULT-NEXT:                 let %[[VALUE89:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(777777777.))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE89]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE89]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_7]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE88]], read<bool>(%[[VALUE89]]));
// DEFAULT-NEXT:             let %[[VALUE90:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE88]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE90]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_a]], call<d64, signature=fn(u128) -> d64>(%[[VALUE_testu64]], widen<u128, reason=arg>(const<u64>(9999999999999000))));
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_b_7]], const<d64>(9999999999999000.));
// DEFAULT-NEXT:                 let %[[VALUE91:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(9999999999999000.))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE91]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE91]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_7]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE90]], read<bool>(%[[VALUE91]]));
// DEFAULT-NEXT:             let %[[VALUE92:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE90]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE92]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_a]], call<d64, signature=fn(u128) -> d64>(%[[VALUE_testu64]], widen<u128, reason=arg>(const<u64>(999999999999999900))));
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_b_7]], const<d64>(9999999999999999.e+2));
// DEFAULT-NEXT:                 let %[[VALUE93:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(9999999999999999.e+2))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE93]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE93]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_7]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE92]], read<bool>(%[[VALUE93]]));
// DEFAULT-NEXT:             let %[[VALUE94:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE92]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE94]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_a]], call<d64, signature=fn(u128) -> d64>(%[[VALUE_testu64]], widen<u128, reason=arg>(const<u64>(9999999999999999000))));
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_b_7]], const<d64>(9999999999999999.e+3));
// DEFAULT-NEXT:                 let %[[VALUE95:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(9999999999999999.e+3))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE95]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE95]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_7]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE94]], read<bool>(%[[VALUE95]]));
// DEFAULT-NEXT:             let %[[VALUE96:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE94]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE96]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_a]], call<d64, signature=fn(u128) -> d64>(%[[VALUE_testu64]], or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(5)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(7766279631452236919)))));
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_b_7]], const<d64>(9999999999999999.e+4));
// DEFAULT-NEXT:                 let %[[VALUE97:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(9999999999999999.e+4))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE97]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE97]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_7]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE96]], read<bool>(%[[VALUE97]]));
// DEFAULT-NEXT:             let %[[VALUE98:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE96]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE98]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_a]], call<d64, signature=fn(u128) -> d64>(%[[VALUE_testu64]], or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(54)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(3875820019684112736)))));
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_b_7]], const<d64>(9999999999999999.e+5));
// DEFAULT-NEXT:                 let %[[VALUE99:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(9999999999999999.e+5))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE99]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE99]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_7]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE98]], read<bool>(%[[VALUE99]]));
// DEFAULT-NEXT:             let %[[VALUE100:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE98]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE100]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_a]], call<d64, signature=fn(u128) -> d64>(%[[VALUE_testu64]], or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(542)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(1864712049422024128)))));
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_b_7]], const<d64>(9999999999999999.e+6));
// DEFAULT-NEXT:                 let %[[VALUE101:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(9999999999999999.e+6))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE101]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE101]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_7]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE100]], read<bool>(%[[VALUE101]]));
// DEFAULT-NEXT:             let %[[VALUE102:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE100]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE102]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_a]], call<d64, signature=fn(u128) -> d64>(%[[VALUE_testu64]], or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(6692)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(11177671081280585728)))));
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_b_7]], const<d64>(1234567890123456.e+8));
// DEFAULT-NEXT:                 let %[[VALUE103:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(1234567890123456.e+8))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE103]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE103]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_7]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE102]], read<bool>(%[[VALUE103]]));
// DEFAULT-NEXT:             let %[[VALUE104:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE102]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE104]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_a]], call<d64, signature=fn(u128) -> d64>(%[[VALUE_testu64]], or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(1856279878857143160)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(11766139157678653440)))));
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_b_7]], const<d64>(3424231985445429e+22));
// DEFAULT-NEXT:                 let %[[VALUE105:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(3424231985445429e+22))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE105]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE105]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_7]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE104]], read<bool>(%[[VALUE105]]));
// DEFAULT-NEXT:             let %[[VALUE106:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE104]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE106]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_a]], call<d64, signature=fn(u128) -> d64>(%[[VALUE_testu64]], or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(3355255724659993925)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(2264493734966067200)))));
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_b_7]], const<d64>(6189354365465179e+22));
// DEFAULT-NEXT:                 let %[[VALUE107:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(6189354365465179e+22))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE107]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE107]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_7]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE106]], read<bool>(%[[VALUE107]]));
// DEFAULT-NEXT:             let %[[VALUE108:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE106]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE108]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_a]], call<d64, signature=fn(u128) -> d64>(%[[VALUE_testu64]], or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(3355255724659994196)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(3196849759677579264)))));
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_b_7]], const<d64>(6189354365465180e+22));
// DEFAULT-NEXT:                 let %[[VALUE109:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(6189354365465180e+22))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE109]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE109]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_7]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE108]], read<bool>(%[[VALUE109]]));
// DEFAULT-NEXT:             let %[[VALUE110:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE108]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE110]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_a]], call<d64, signature=fn(u128) -> d64>(%[[VALUE_testu64]], or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(3355255724659994196)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(3196849759677579263)))));
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_b_7]], const<d64>(6189354365465179e+22));
// DEFAULT-NEXT:                 let %[[VALUE111:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(6189354365465179e+22))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE111]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE111]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_7]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE110]], read<bool>(%[[VALUE111]]));
// DEFAULT-NEXT:             let %[[VALUE112:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE110]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE112]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_a]], call<d64, signature=fn(u128) -> d64>(%[[VALUE_testu64]], or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(3355255724659994467)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(4129205784389091327)))));
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_b_7]], const<d64>(6189354365465180e+22));
// DEFAULT-NEXT:                 let %[[VALUE113:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(6189354365465180e+22))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE113]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE113]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_7]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE112]], read<bool>(%[[VALUE113]]));
// DEFAULT-NEXT:             let %[[VALUE114:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE112]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE114]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_a]], call<d64, signature=fn(u128) -> d64>(%[[VALUE_testu64]], or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(3355255724659993383)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(399781685543043072)))));
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_b_7]], const<d64>(6189354365465178e+22));
// DEFAULT-NEXT:                 let %[[VALUE115:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(6189354365465178e+22))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE115]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE115]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_7]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE114]], read<bool>(%[[VALUE115]]));
// DEFAULT-NEXT:             let %[[VALUE116:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE114]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE116]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_a]], call<d64, signature=fn(u128) -> d64>(%[[VALUE_testu64]], or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(3355255724659993654)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(1332137710254555136)))));
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_b_7]], const<d64>(6189354365465178e+22));
// DEFAULT-NEXT:                 let %[[VALUE117:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(6189354365465178e+22))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE117]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE117]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_7]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE116]], read<bool>(%[[VALUE117]]));
// DEFAULT-NEXT:             let %[[VALUE118:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE116]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE118]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_a]], call<d64, signature=fn(u128) -> d64>(%[[VALUE_testu64]], or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(3355255724659993654)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(1332137710254555137)))));
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_b_7]], const<d64>(6189354365465179e+22));
// DEFAULT-NEXT:                 let %[[VALUE119:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(6189354365465179e+22))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE119]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE119]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_7]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE118]], read<bool>(%[[VALUE119]]));
// DEFAULT-NEXT:             let %[[VALUE120:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE118]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE120]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_a]], call<d64, signature=fn(u128) -> d64>(%[[VALUE_testu64]], or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(5421010862427521627)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(17269431575687200768)))));
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_b_7]], const<d64>(9999999999999999.e+22));
// DEFAULT-NEXT:                 let %[[VALUE121:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(9999999999999999.e+22))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE121]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE121]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_7]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE120]], read<bool>(%[[VALUE121]]));
// DEFAULT-NEXT:             let %[[VALUE122:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE120]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE122]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_a]], call<d64, signature=fn(u128) -> d64>(%[[VALUE_testu64]], or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(18446744073709548175)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(11871750202798899200)))));
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_b_7]], const<d64>(340282366920938.4e+24));
// DEFAULT-NEXT:                 let %[[VALUE123:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(340282366920938.4e+24))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE123]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE123]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_7]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE122]], read<bool>(%[[VALUE123]]));
// DEFAULT-NEXT:             let %[[VALUE124:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE122]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE124]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_a]], call<d64, signature=fn(u128) -> d64>(%[[VALUE_testu64]], or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(18446744073709550886)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(2748566376204468223)))));
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_b_7]], const<d64>(340282366920938.4e+24));
// DEFAULT-NEXT:                 let %[[VALUE125:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(340282366920938.4e+24))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE125]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE125]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_7]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE124]], read<bool>(%[[VALUE125]]));
// DEFAULT-NEXT:             let %[[VALUE126:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE124]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE126]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_a]], call<d64, signature=fn(u128) -> d64>(%[[VALUE_testu64]], or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(18446744073709550886)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(2748566376204468224)))));
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_b_7]], const<d64>(340282366920938.4e+24));
// DEFAULT-NEXT:                 let %[[VALUE127:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(340282366920938.4e+24))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE127]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE127]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_7]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE126]], read<bool>(%[[VALUE127]]));
// DEFAULT-NEXT:             let %[[VALUE128:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE126]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE128]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_a]], call<d64, signature=fn(u128) -> d64>(%[[VALUE_testu64]], or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(18446744073709550886)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(2748566376204468225)))));
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_b_7]], const<d64>(340282366920938.5e+24));
// DEFAULT-NEXT:                 let %[[VALUE129:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(340282366920938.5e+24))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE129]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE129]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_7]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE128]], read<bool>(%[[VALUE129]]));
// DEFAULT-NEXT:             let %[[VALUE130:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE128]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE130]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_a]], call<d64, signature=fn(u128) -> d64>(%[[VALUE_testu64]], or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(18446744073709550886)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(2748566386204468224)))));
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_b_7]], const<d64>(340282366920938.5e+24));
// DEFAULT-NEXT:                 let %[[VALUE131:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(340282366920938.5e+24))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE131]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE131]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_7]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE130]], read<bool>(%[[VALUE131]]));
// DEFAULT-NEXT:             let %[[VALUE132:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE130]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE132]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_a]], call<d64, signature=fn(u128) -> d64>(%[[VALUE_testu64]], not<u128>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(0))))));
// DEFAULT-NEXT:                 write<d64>(%[[VALUE_b_7]], const<d64>(340282366920938.5e+24));
// DEFAULT-NEXT:                 let %[[VALUE133:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d64, exceptions=observable>(read<d64>(%[[VALUE_a]]), const<d64>(340282366920938.5e+24))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE133]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE133]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d64>>(%[[VALUE_b_7]])), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE132]], read<bool>(%[[VALUE133]]));
// DEFAULT-NEXT:             if read<bool>(%[[VALUE132]])
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_a_2:[0-9]+]] a: d32 [storage=automatic];
// DEFAULT-NEXT:             let %[[VALUE_b_8:[0-9]+]] b: d32 [storage=automatic];
// DEFAULT-NEXT:             write<d32>(%[[VALUE_a_2]], call<d32, signature=fn(i128) -> d32>(%[[VALUE_tests32]], widen<i128, reason=arg>(const<i64>(0))));
// DEFAULT-NEXT:             write<d32>(%[[VALUE_b_8]], const<d32>(0.));
// DEFAULT-NEXT:             let %[[VALUE134:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d32, exceptions=observable>(read<d32>(%[[VALUE_a_2]]), const<d32>(0.))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE134]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE134]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_a_2]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_b_8]])), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:             let %[[VALUE135:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE134]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE135]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_a_2]], call<d32, signature=fn(i128) -> d32>(%[[VALUE_tests32]], widen<i128, reason=arg>(const<i64>(7))));
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_b_8]], const<d32>(7.));
// DEFAULT-NEXT:                 let %[[VALUE136:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%[[VALUE_a_2]]), const<d32>(7.))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE136]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE136]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_a_2]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_b_8]])), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE135]], read<bool>(%[[VALUE136]]));
// DEFAULT-NEXT:             let %[[VALUE137:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE135]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE137]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_a_2]], call<d32, signature=fn(i128) -> d32>(%[[VALUE_tests32]], widen<i128, reason=arg>(neg<i64, overflow=ub>(const<i64>(42)))));
// DEFAULT-NEXT:                 let %[[VALUE138:[0-9]+]]: d32 [synthetic] = neg<d32>(const<d32>(42.));
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_b_8]], read<d32>(%[[VALUE138]]));
// DEFAULT-NEXT:                 let %[[VALUE139:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%[[VALUE_a_2]]), neg<d32>(const<d32>(42.)))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE139]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE139]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_a_2]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_b_8]])), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE137]], read<bool>(%[[VALUE139]]));
// DEFAULT-NEXT:             let %[[VALUE140:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE137]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE140]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_a_2]], call<d32, signature=fn(i128) -> d32>(%[[VALUE_tests32]], widen<i128, reason=arg>(neg<i64, overflow=ub>(const<i64>(777777)))));
// DEFAULT-NEXT:                 let %[[VALUE141:[0-9]+]]: d32 [synthetic] = neg<d32>(const<d32>(777777.));
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_b_8]], read<d32>(%[[VALUE141]]));
// DEFAULT-NEXT:                 let %[[VALUE142:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%[[VALUE_a_2]]), neg<d32>(const<d32>(777777.)))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE142]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE142]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_a_2]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_b_8]])), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE140]], read<bool>(%[[VALUE142]]));
// DEFAULT-NEXT:             let %[[VALUE143:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE140]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE143]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_a_2]], call<d32, signature=fn(i128) -> d32>(%[[VALUE_tests32]], widen<i128, reason=arg>(const<i64>(9999000))));
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_b_8]], const<d32>(9999000.));
// DEFAULT-NEXT:                 let %[[VALUE144:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%[[VALUE_a_2]]), const<d32>(9999000.))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE144]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE144]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_a_2]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_b_8]])), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE143]], read<bool>(%[[VALUE144]]));
// DEFAULT-NEXT:             let %[[VALUE145:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE143]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE145]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_a_2]], call<d32, signature=fn(i128) -> d32>(%[[VALUE_tests32]], widen<i128, reason=arg>(neg<i64, overflow=ub>(const<i64>(9999999)))));
// DEFAULT-NEXT:                 let %[[VALUE146:[0-9]+]]: d32 [synthetic] = neg<d32>(const<d32>(9999999.));
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_b_8]], read<d32>(%[[VALUE146]]));
// DEFAULT-NEXT:                 let %[[VALUE147:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%[[VALUE_a_2]]), neg<d32>(const<d32>(9999999.)))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE147]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE147]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_a_2]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_b_8]])), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE145]], read<bool>(%[[VALUE147]]));
// DEFAULT-NEXT:             let %[[VALUE148:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE145]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE148]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_a_2]], call<d32, signature=fn(i128) -> d32>(%[[VALUE_tests32]], widen<i128, reason=arg>(const<i64>(99999994))));
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_b_8]], const<d32>(9999999.e+1));
// DEFAULT-NEXT:                 let %[[VALUE149:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%[[VALUE_a_2]]), const<d32>(9999999.e+1))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE149]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE149]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_a_2]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_b_8]])), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE148]], read<bool>(%[[VALUE149]]));
// DEFAULT-NEXT:             let %[[VALUE150:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE148]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE150]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_a_2]], call<d32, signature=fn(i128) -> d32>(%[[VALUE_tests32]], widen<i128, reason=arg>(const<i64>(99999995))));
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_b_8]], const<d32>(1000000.e+2));
// DEFAULT-NEXT:                 let %[[VALUE151:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%[[VALUE_a_2]]), const<d32>(1000000.e+2))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE151]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE151]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_a_2]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_b_8]])), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE150]], read<bool>(%[[VALUE151]]));
// DEFAULT-NEXT:             let %[[VALUE152:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE150]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE152]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_a_2]], call<d32, signature=fn(i128) -> d32>(%[[VALUE_tests32]], widen<i128, reason=arg>(const<i64>(999999900))));
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_b_8]], const<d32>(9999999.e+2));
// DEFAULT-NEXT:                 let %[[VALUE153:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%[[VALUE_a_2]]), const<d32>(9999999.e+2))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE153]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE153]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_a_2]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_b_8]])), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE152]], read<bool>(%[[VALUE153]]));
// DEFAULT-NEXT:             let %[[VALUE154:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE152]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE154]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_a_2]], call<d32, signature=fn(i128) -> d32>(%[[VALUE_tests32]], widen<i128, reason=arg>(neg<i64, overflow=ub>(const<i64>(9999999000)))));
// DEFAULT-NEXT:                 let %[[VALUE155:[0-9]+]]: d32 [synthetic] = neg<d32>(const<d32>(9999999.e+3));
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_b_8]], read<d32>(%[[VALUE155]]));
// DEFAULT-NEXT:                 let %[[VALUE156:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%[[VALUE_a_2]]), neg<d32>(const<d32>(9999999.e+3)))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE156]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE156]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_a_2]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_b_8]])), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE154]], read<bool>(%[[VALUE156]]));
// DEFAULT-NEXT:             let %[[VALUE157:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE154]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE157]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_a_2]], call<d32, signature=fn(i128) -> d32>(%[[VALUE_tests32]], widen<i128, reason=arg>(const<i64>(999999900000))));
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_b_8]], const<d32>(9999999.e+5));
// DEFAULT-NEXT:                 let %[[VALUE158:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%[[VALUE_a_2]]), const<d32>(9999999.e+5))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE158]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE158]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_a_2]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_b_8]])), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE157]], read<bool>(%[[VALUE158]]));
// DEFAULT-NEXT:             let %[[VALUE159:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE157]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE159]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_a_2]], call<d32, signature=fn(i128) -> d32>(%[[VALUE_tests32]], widen<i128, reason=arg>(neg<i64, overflow=ub>(const<i64>(9999999000000)))));
// DEFAULT-NEXT:                 let %[[VALUE160:[0-9]+]]: d32 [synthetic] = neg<d32>(const<d32>(9999999.e+6));
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_b_8]], read<d32>(%[[VALUE160]]));
// DEFAULT-NEXT:                 let %[[VALUE161:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%[[VALUE_a_2]]), neg<d32>(const<d32>(9999999.e+6)))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE161]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE161]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_a_2]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_b_8]])), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE159]], read<bool>(%[[VALUE161]]));
// DEFAULT-NEXT:             let %[[VALUE162:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE159]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE162]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_a_2]], call<d32, signature=fn(i128) -> d32>(%[[VALUE_tests32]], widen<i128, reason=arg>(const<i64>(123456700000000000))));
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_b_8]], const<d32>(1234567.e+11));
// DEFAULT-NEXT:                 let %[[VALUE163:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%[[VALUE_a_2]]), const<d32>(1234567.e+11))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE163]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE163]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_a_2]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_b_8]])), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE162]], read<bool>(%[[VALUE163]]));
// DEFAULT-NEXT:             let %[[VALUE164:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE162]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE164]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_a_2]], call<d32, signature=fn(i128) -> d32>(%[[VALUE_tests32]], or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(1856279344))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(11918545879717380096))))));
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_b_8]], const<d32>(3424231e+22));
// DEFAULT-NEXT:                 let %[[VALUE165:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%[[VALUE_a_2]]), const<d32>(3424231e+22))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE165]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE165]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_a_2]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_b_8]])), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE164]], read<bool>(%[[VALUE165]]));
// DEFAULT-NEXT:             let %[[VALUE166:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE164]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE166]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_a_2]], call<d32, signature=fn(i128) -> d32>(%[[VALUE_tests32]], or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(5421010320326435927))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(5258189069476691968))))));
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_b_8]], const<d32>(9999999.e+31));
// DEFAULT-NEXT:                 let %[[VALUE167:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%[[VALUE_a_2]]), const<d32>(9999999.e+31))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE167]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE167]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_a_2]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_b_8]])), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE166]], read<bool>(%[[VALUE167]]));
// DEFAULT-NEXT:             let %[[VALUE168:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE166]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE168]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_a_2]], call<d32, signature=fn(i128) -> d32>(%[[VALUE_tests32]], neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(5421010320326435927))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(5258189069476691968)))))));
// DEFAULT-NEXT:                 let %[[VALUE169:[0-9]+]]: d32 [synthetic] = neg<d32>(const<d32>(9999999.e+31));
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_b_8]], read<d32>(%[[VALUE169]]));
// DEFAULT-NEXT:                 let %[[VALUE170:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%[[VALUE_a_2]]), neg<d32>(const<d32>(9999999.e+31)))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE170]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE170]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_a_2]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_b_8]])), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE168]], read<bool>(%[[VALUE170]]));
// DEFAULT-NEXT:             let %[[VALUE171:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE168]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE171]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_a_2]], call<d32, signature=fn(i128) -> d32>(%[[VALUE_tests32]], neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(1159310820085523996))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(7849755482431422464)))))));
// DEFAULT-NEXT:                 let %[[VALUE172:[0-9]+]]: d32 [synthetic] = neg<d32>(const<d32>(2138551e+31));
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_b_8]], read<d32>(%[[VALUE172]]));
// DEFAULT-NEXT:                 let %[[VALUE173:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%[[VALUE_a_2]]), neg<d32>(const<d32>(2138551e+31)))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE173]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE173]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_a_2]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_b_8]])), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE171]], read<bool>(%[[VALUE173]]));
// DEFAULT-NEXT:             let %[[VALUE174:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE171]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE174]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_a_2]], call<d32, signature=fn(i128) -> d32>(%[[VALUE_tests32]], neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(1159311091136067117))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(14787732760248188928)))))));
// DEFAULT-NEXT:                 let %[[VALUE175:[0-9]+]]: d32 [synthetic] = neg<d32>(const<d32>(2138552e+31));
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_b_8]], read<d32>(%[[VALUE175]]));
// DEFAULT-NEXT:                 let %[[VALUE176:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%[[VALUE_a_2]]), neg<d32>(const<d32>(2138552e+31)))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE176]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE176]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_a_2]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_b_8]])), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE174]], read<bool>(%[[VALUE176]]));
// DEFAULT-NEXT:             let %[[VALUE177:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE174]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE177]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_a_2]], call<d32, signature=fn(i128) -> d32>(%[[VALUE_tests32]], neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(1159311091136067117))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(14787732760248188927)))))));
// DEFAULT-NEXT:                 let %[[VALUE178:[0-9]+]]: d32 [synthetic] = neg<d32>(const<d32>(2138551e+31));
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_b_8]], read<d32>(%[[VALUE178]]));
// DEFAULT-NEXT:                 let %[[VALUE179:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%[[VALUE_a_2]]), neg<d32>(const<d32>(2138551e+31)))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE179]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE179]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_a_2]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_b_8]])), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE177]], read<bool>(%[[VALUE179]]));
// DEFAULT-NEXT:             let %[[VALUE180:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE177]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE180]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_a_2]], call<d32, signature=fn(i128) -> d32>(%[[VALUE_tests32]], neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(1159311362186610239))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(3278965964355403775)))))));
// DEFAULT-NEXT:                 let %[[VALUE181:[0-9]+]]: d32 [synthetic] = neg<d32>(const<d32>(2138552e+31));
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_b_8]], read<d32>(%[[VALUE181]]));
// DEFAULT-NEXT:                 let %[[VALUE182:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%[[VALUE_a_2]]), neg<d32>(const<d32>(2138552e+31)))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE182]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE182]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_a_2]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_b_8]])), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE180]], read<bool>(%[[VALUE182]]));
// DEFAULT-NEXT:             let %[[VALUE183:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE180]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE183]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_a_2]], call<d32, signature=fn(i128) -> d32>(%[[VALUE_tests32]], neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(1159311362186610239))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(3278965964355403776)))))));
// DEFAULT-NEXT:                 let %[[VALUE184:[0-9]+]]: d32 [synthetic] = neg<d32>(const<d32>(2138552e+31));
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_b_8]], read<d32>(%[[VALUE184]]));
// DEFAULT-NEXT:                 let %[[VALUE185:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%[[VALUE_a_2]]), neg<d32>(const<d32>(2138552e+31)))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE185]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE185]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_a_2]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_b_8]])), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE183]], read<bool>(%[[VALUE185]]));
// DEFAULT-NEXT:             let %[[VALUE186:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE183]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE186]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_a_2]], call<d32, signature=fn(i128) -> d32>(%[[VALUE_tests32]], neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(1159311633237153360))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(10216943242172170240)))))));
// DEFAULT-NEXT:                 let %[[VALUE187:[0-9]+]]: d32 [synthetic] = neg<d32>(const<d32>(2138552e+31));
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_b_8]], read<d32>(%[[VALUE187]]));
// DEFAULT-NEXT:                 let %[[VALUE188:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%[[VALUE_a_2]]), neg<d32>(const<d32>(2138552e+31)))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE188]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE188]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_a_2]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_b_8]])), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE186]], read<bool>(%[[VALUE188]]));
// DEFAULT-NEXT:             let %[[VALUE189:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE186]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE189]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_a_2]], call<d32, signature=fn(i128) -> d32>(%[[VALUE_tests32]], neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(1159311633237153360))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(10216943242172170241)))))));
// DEFAULT-NEXT:                 let %[[VALUE190:[0-9]+]]: d32 [synthetic] = neg<d32>(const<d32>(2138553e+31));
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_b_8]], read<d32>(%[[VALUE190]]));
// DEFAULT-NEXT:                 let %[[VALUE191:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%[[VALUE_a_2]]), neg<d32>(const<d32>(2138553e+31)))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE191]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE191]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_a_2]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_b_8]])), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE189]], read<bool>(%[[VALUE191]]));
// DEFAULT-NEXT:             let %[[VALUE192:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE189]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE192]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_a_2]], call<d32, signature=fn(i128) -> d32>(%[[VALUE_tests32]], or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(9223367512453672922))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(15592504947059458048))))));
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_b_8]], const<d32>(1701411e+32));
// DEFAULT-NEXT:                 let %[[VALUE193:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%[[VALUE_a_2]]), const<d32>(1701411e+32))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE193]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE193]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_a_2]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_b_8]])), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE192]], read<bool>(%[[VALUE193]]));
// DEFAULT-NEXT:             let %[[VALUE194:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE192]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE194]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_a_2]], call<d32, signature=fn(i128) -> d32>(%[[VALUE_tests32]], reinterpret<i128, reason=explicit, fits=unknown>(sub<u128, overflow=wrap>(shl<u128, overflow=wrap, amount_out_of_range=ub>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(1))), const<i32>(127)), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(1)))))));
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_b_8]], const<d32>(1701412e+32));
// DEFAULT-NEXT:                 let %[[VALUE195:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%[[VALUE_a_2]]), const<d32>(1701412e+32))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE195]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE195]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_a_2]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_b_8]])), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE194]], read<bool>(%[[VALUE195]]));
// DEFAULT-NEXT:             let %[[VALUE196:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE194]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE196]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_a_2]], call<d32, signature=fn(i128) -> d32>(%[[VALUE_tests32]], neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(9223367512453672922))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(15592504947059458048)))))));
// DEFAULT-NEXT:                 let %[[VALUE197:[0-9]+]]: d32 [synthetic] = neg<d32>(const<d32>(1701411e+32));
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_b_8]], read<d32>(%[[VALUE197]]));
// DEFAULT-NEXT:                 let %[[VALUE198:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%[[VALUE_a_2]]), neg<d32>(const<d32>(1701411e+32)))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE198]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE198]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_a_2]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_b_8]])), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE196]], read<bool>(%[[VALUE198]]));
// DEFAULT-NEXT:             let %[[VALUE199:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE196]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE199]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_a_2]], call<d32, signature=fn(i128) -> d32>(%[[VALUE_tests32]], sub<i128, overflow=ub>(neg<i128, overflow=ub>(reinterpret<i128, reason=explicit, fits=unknown>(sub<u128, overflow=wrap>(shl<u128, overflow=wrap, amount_out_of_range=ub>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(1))), const<i32>(127)), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(1)))))), widen<i128, reason=usual_arith>(const<i64>(1)))));
// DEFAULT-NEXT:                 let %[[VALUE200:[0-9]+]]: d32 [synthetic] = neg<d32>(const<d32>(1701412e+32));
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_b_8]], read<d32>(%[[VALUE200]]));
// DEFAULT-NEXT:                 let %[[VALUE201:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%[[VALUE_a_2]]), neg<d32>(const<d32>(1701412e+32)))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE201]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE201]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_a_2]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_b_8]])), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE199]], read<bool>(%[[VALUE201]]));
// DEFAULT-NEXT:             if read<bool>(%[[VALUE199]])
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:             write<d32>(%[[VALUE_a_2]], call<d32, signature=fn(u128) -> d32>(%[[VALUE_testu32]], widen<u128, reason=arg>(const<u64>(0))));
// DEFAULT-NEXT:             write<d32>(%[[VALUE_b_8]], const<d32>(0.));
// DEFAULT-NEXT:             let %[[VALUE202:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d32, exceptions=observable>(read<d32>(%[[VALUE_a_2]]), const<d32>(0.))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE202]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE202]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_a_2]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_b_8]])), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:             let %[[VALUE203:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE202]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE203]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_a_2]], call<d32, signature=fn(u128) -> d32>(%[[VALUE_testu32]], widen<u128, reason=arg>(const<u64>(7))));
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_b_8]], const<d32>(7.));
// DEFAULT-NEXT:                 let %[[VALUE204:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%[[VALUE_a_2]]), const<d32>(7.))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE204]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE204]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_a_2]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_b_8]])), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE203]], read<bool>(%[[VALUE204]]));
// DEFAULT-NEXT:             let %[[VALUE205:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE203]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE205]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_a_2]], call<d32, signature=fn(u128) -> d32>(%[[VALUE_testu32]], widen<u128, reason=arg>(const<u64>(42))));
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_b_8]], const<d32>(42.));
// DEFAULT-NEXT:                 let %[[VALUE206:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%[[VALUE_a_2]]), const<d32>(42.))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE206]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE206]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_a_2]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_b_8]])), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE205]], read<bool>(%[[VALUE206]]));
// DEFAULT-NEXT:             let %[[VALUE207:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE205]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE207]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_a_2]], call<d32, signature=fn(u128) -> d32>(%[[VALUE_testu32]], widen<u128, reason=arg>(const<u64>(77777))));
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_b_8]], const<d32>(77777.));
// DEFAULT-NEXT:                 let %[[VALUE208:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%[[VALUE_a_2]]), const<d32>(77777.))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE208]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE208]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_a_2]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_b_8]])), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE207]], read<bool>(%[[VALUE208]]));
// DEFAULT-NEXT:             let %[[VALUE209:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE207]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE209]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_a_2]], call<d32, signature=fn(u128) -> d32>(%[[VALUE_testu32]], widen<u128, reason=arg>(const<u64>(9999000))));
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_b_8]], const<d32>(9999000.));
// DEFAULT-NEXT:                 let %[[VALUE210:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%[[VALUE_a_2]]), const<d32>(9999000.))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE210]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE210]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_a_2]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_b_8]])), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE209]], read<bool>(%[[VALUE210]]));
// DEFAULT-NEXT:             let %[[VALUE211:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE209]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE211]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_a_2]], call<d32, signature=fn(u128) -> d32>(%[[VALUE_testu32]], widen<u128, reason=arg>(const<u64>(999999900))));
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_b_8]], const<d32>(9999999.e+2));
// DEFAULT-NEXT:                 let %[[VALUE212:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%[[VALUE_a_2]]), const<d32>(9999999.e+2))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE212]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE212]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_a_2]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_b_8]])), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE211]], read<bool>(%[[VALUE212]]));
// DEFAULT-NEXT:             let %[[VALUE213:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE211]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE213]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_a_2]], call<d32, signature=fn(u128) -> d32>(%[[VALUE_testu32]], widen<u128, reason=arg>(const<u64>(999999949))));
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_b_8]], const<d32>(9999999.e+2));
// DEFAULT-NEXT:                 let %[[VALUE214:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%[[VALUE_a_2]]), const<d32>(9999999.e+2))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE214]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE214]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_a_2]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_b_8]])), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE213]], read<bool>(%[[VALUE214]]));
// DEFAULT-NEXT:             let %[[VALUE215:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE213]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE215]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_a_2]], call<d32, signature=fn(u128) -> d32>(%[[VALUE_testu32]], widen<u128, reason=arg>(const<u64>(9999999000))));
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_b_8]], const<d32>(9999999.e+3));
// DEFAULT-NEXT:                 let %[[VALUE216:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%[[VALUE_a_2]]), const<d32>(9999999.e+3))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE216]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE216]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_a_2]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_b_8]])), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE215]], read<bool>(%[[VALUE216]]));
// DEFAULT-NEXT:             let %[[VALUE217:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE215]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE217]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_a_2]], call<d32, signature=fn(u128) -> d32>(%[[VALUE_testu32]], widen<u128, reason=arg>(const<u64>(9999999499))));
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_b_8]], const<d32>(9999999.e+3));
// DEFAULT-NEXT:                 let %[[VALUE218:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%[[VALUE_a_2]]), const<d32>(9999999.e+3))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE218]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE218]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_a_2]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_b_8]])), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE217]], read<bool>(%[[VALUE218]]));
// DEFAULT-NEXT:             let %[[VALUE219:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE217]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE219]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_a_2]], call<d32, signature=fn(u128) -> d32>(%[[VALUE_testu32]], widen<u128, reason=arg>(const<u64>(999999900000))));
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_b_8]], const<d32>(9999999.e+5));
// DEFAULT-NEXT:                 let %[[VALUE220:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%[[VALUE_a_2]]), const<d32>(9999999.e+5))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE220]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE220]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_a_2]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_b_8]])), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE219]], read<bool>(%[[VALUE220]]));
// DEFAULT-NEXT:             let %[[VALUE221:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE219]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE221]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_a_2]], call<d32, signature=fn(u128) -> d32>(%[[VALUE_testu32]], widen<u128, reason=arg>(const<u64>(9999999000000))));
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_b_8]], const<d32>(9999999.e+6));
// DEFAULT-NEXT:                 let %[[VALUE222:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%[[VALUE_a_2]]), const<d32>(9999999.e+6))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE222]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE222]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_a_2]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_b_8]])), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE221]], read<bool>(%[[VALUE222]]));
// DEFAULT-NEXT:             let %[[VALUE223:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE221]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE223]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_a_2]], call<d32, signature=fn(u128) -> d32>(%[[VALUE_testu32]], widen<u128, reason=arg>(const<u64>(123456700000000))));
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_b_8]], const<d32>(1234567.e+8));
// DEFAULT-NEXT:                 let %[[VALUE224:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%[[VALUE_a_2]]), const<d32>(1234567.e+8))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE224]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE224]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_a_2]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_b_8]])), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE223]], read<bool>(%[[VALUE224]]));
// DEFAULT-NEXT:             let %[[VALUE225:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE223]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE225]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_a_2]], call<d32, signature=fn(u128) -> d32>(%[[VALUE_testu32]], or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(1856279344)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(11918545879717380096)))));
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_b_8]], const<d32>(3424231e+22));
// DEFAULT-NEXT:                 let %[[VALUE226:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%[[VALUE_a_2]]), const<d32>(3424231e+22))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE226]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE226]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_a_2]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_b_8]])), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE225]], read<bool>(%[[VALUE226]]));
// DEFAULT-NEXT:             let %[[VALUE227:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE225]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE227]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_a_2]], call<d32, signature=fn(u128) -> d32>(%[[VALUE_testu32]], or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(542101032032643592)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(13438539758544355328)))));
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_b_8]], const<d32>(9999999.e+30));
// DEFAULT-NEXT:                 let %[[VALUE228:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%[[VALUE_a_2]]), const<d32>(9999999.e+30))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE228]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE228]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_a_2]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_b_8]])), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE227]], read<bool>(%[[VALUE228]]));
// DEFAULT-NEXT:             let %[[VALUE229:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE227]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE229]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_a_2]], call<d32, signature=fn(u128) -> d32>(%[[VALUE_testu32]], or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(3355258237046354619)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(1512024826179485696)))));
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_b_8]], const<d32>(6189359e+31));
// DEFAULT-NEXT:                 let %[[VALUE230:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%[[VALUE_a_2]]), const<d32>(6189359e+31))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE230]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE230]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_a_2]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_b_8]])), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE229]], read<bool>(%[[VALUE230]]));
// DEFAULT-NEXT:             let %[[VALUE231:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE229]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE231]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_a_2]], call<d32, signature=fn(u128) -> d32>(%[[VALUE_testu32]], or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(3355258508096897740)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(8450002103996252160)))));
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_b_8]], const<d32>(6189360e+31));
// DEFAULT-NEXT:                 let %[[VALUE232:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%[[VALUE_a_2]]), const<d32>(6189360e+31))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE232]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE232]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_a_2]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_b_8]])), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE231]], read<bool>(%[[VALUE232]]));
// DEFAULT-NEXT:             let %[[VALUE233:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE231]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE233]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_a_2]], call<d32, signature=fn(u128) -> d32>(%[[VALUE_testu32]], or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(3355258508096897740)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(8450002103996252159)))));
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_b_8]], const<d32>(6189359e+31));
// DEFAULT-NEXT:                 let %[[VALUE234:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%[[VALUE_a_2]]), const<d32>(6189359e+31))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE234]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE234]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_a_2]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_b_8]])), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE233]], read<bool>(%[[VALUE234]]));
// DEFAULT-NEXT:             let %[[VALUE235:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE233]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE235]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_a_2]], call<d32, signature=fn(u128) -> d32>(%[[VALUE_testu32]], or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(3355258779147440861)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(15387979381813018623)))));
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_b_8]], const<d32>(6189360e+31));
// DEFAULT-NEXT:                 let %[[VALUE236:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%[[VALUE_a_2]]), const<d32>(6189360e+31))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE236]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE236]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_a_2]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_b_8]])), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE235]], read<bool>(%[[VALUE236]]));
// DEFAULT-NEXT:             let %[[VALUE237:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE235]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE237]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_a_2]], call<d32, signature=fn(u128) -> d32>(%[[VALUE_testu32]], or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(3355257694945268376)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(6082814344255504384)))));
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_b_8]], const<d32>(6189358e+31));
// DEFAULT-NEXT:                 let %[[VALUE238:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%[[VALUE_a_2]]), const<d32>(6189358e+31))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE238]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE238]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_a_2]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_b_8]])), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE237]], read<bool>(%[[VALUE238]]));
// DEFAULT-NEXT:             let %[[VALUE239:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE237]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE239]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_a_2]], call<d32, signature=fn(u128) -> d32>(%[[VALUE_testu32]], or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(3355257965995811497)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(13020791622072270848)))));
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_b_8]], const<d32>(6189358e+31));
// DEFAULT-NEXT:                 let %[[VALUE240:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%[[VALUE_a_2]]), const<d32>(6189358e+31))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE240]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE240]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_a_2]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_b_8]])), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE239]], read<bool>(%[[VALUE240]]));
// DEFAULT-NEXT:             let %[[VALUE241:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE239]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE241]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_a_2]], call<d32, signature=fn(u128) -> d32>(%[[VALUE_testu32]], or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(3355257965995811497)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(13020791622072270849)))));
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_b_8]], const<d32>(6189359e+31));
// DEFAULT-NEXT:                 let %[[VALUE242:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%[[VALUE_a_2]]), const<d32>(6189359e+31))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE242]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE242]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_a_2]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_b_8]])), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE241]], read<bool>(%[[VALUE242]]));
// DEFAULT-NEXT:             let %[[VALUE243:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE241]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE243]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_a_2]], call<d32, signature=fn(u128) -> d32>(%[[VALUE_testu32]], or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(18446740445918208273)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(3923858787068280832)))));
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_b_8]], const<d32>(3402823e+32));
// DEFAULT-NEXT:                 let %[[VALUE244:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%[[VALUE_a_2]]), const<d32>(3402823e+32))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE244]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE244]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_a_2]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_b_8]])), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE243]], read<bool>(%[[VALUE244]]));
// DEFAULT-NEXT:             let %[[VALUE245:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE243]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE245]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_a_2]], call<d32, signature=fn(u128) -> d32>(%[[VALUE_testu32]], not<u128>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(0))))));
// DEFAULT-NEXT:                 write<d32>(%[[VALUE_b_8]], const<d32>(3402824e+32));
// DEFAULT-NEXT:                 let %[[VALUE246:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d32, exceptions=observable>(read<d32>(%[[VALUE_a_2]]), const<d32>(3402824e+32))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE246]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE246]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_a_2]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d32>>(%[[VALUE_b_8]])), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE245]], read<bool>(%[[VALUE246]]));
// DEFAULT-NEXT:             if read<bool>(%[[VALUE245]])
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_a_3:[0-9]+]] a: d128 [storage=automatic];
// DEFAULT-NEXT:             let %[[VALUE_b_9:[0-9]+]] b: d128 [storage=automatic];
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a_3]], call<d128, signature=fn(i128) -> d128>(%[[VALUE_tests128]], widen<i128, reason=arg>(const<i64>(0))));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_9]], const<d128>(0.));
// DEFAULT-NEXT:             let %[[VALUE247:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a_3]]), const<d128>(0.))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE247]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE247]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a_3]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_9]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             let %[[VALUE248:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE247]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE248]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d128>(%[[VALUE_a_3]], call<d128, signature=fn(i128) -> d128>(%[[VALUE_tests128]], widen<i128, reason=arg>(const<i64>(7))));
// DEFAULT-NEXT:                 write<d128>(%[[VALUE_b_9]], const<d128>(7.));
// DEFAULT-NEXT:                 let %[[VALUE249:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a_3]]), const<d128>(7.))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE249]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE249]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a_3]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_9]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE248]], read<bool>(%[[VALUE249]]));
// DEFAULT-NEXT:             let %[[VALUE250:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE248]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE250]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d128>(%[[VALUE_a_3]], call<d128, signature=fn(i128) -> d128>(%[[VALUE_tests128]], widen<i128, reason=arg>(neg<i64, overflow=ub>(const<i64>(42)))));
// DEFAULT-NEXT:                 let %[[VALUE251:[0-9]+]]: d128 [synthetic] = neg<d128>(const<d128>(42.));
// DEFAULT-NEXT:                 write<d128>(%[[VALUE_b_9]], read<d128>(%[[VALUE251]]));
// DEFAULT-NEXT:                 let %[[VALUE252:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a_3]]), neg<d128>(const<d128>(42.)))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE252]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE252]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a_3]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_9]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE250]], read<bool>(%[[VALUE252]]));
// DEFAULT-NEXT:             let %[[VALUE253:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE250]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE253]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d128>(%[[VALUE_a_3]], call<d128, signature=fn(i128) -> d128>(%[[VALUE_tests128]], widen<i128, reason=arg>(neg<i64, overflow=ub>(const<i64>(777777777)))));
// DEFAULT-NEXT:                 let %[[VALUE254:[0-9]+]]: d128 [synthetic] = neg<d128>(const<d128>(777777777.));
// DEFAULT-NEXT:                 write<d128>(%[[VALUE_b_9]], read<d128>(%[[VALUE254]]));
// DEFAULT-NEXT:                 let %[[VALUE255:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a_3]]), neg<d128>(const<d128>(777777777.)))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE255]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE255]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a_3]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_9]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE253]], read<bool>(%[[VALUE255]]));
// DEFAULT-NEXT:             let %[[VALUE256:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE253]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE256]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d128>(%[[VALUE_a_3]], call<d128, signature=fn(i128) -> d128>(%[[VALUE_tests128]], widen<i128, reason=arg>(neg<i64, overflow=ub>(const<i64>(12345678912345)))));
// DEFAULT-NEXT:                 let %[[VALUE257:[0-9]+]]: d128 [synthetic] = neg<d128>(const<d128>(12345678912345.));
// DEFAULT-NEXT:                 write<d128>(%[[VALUE_b_9]], read<d128>(%[[VALUE257]]));
// DEFAULT-NEXT:                 let %[[VALUE258:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a_3]]), neg<d128>(const<d128>(12345678912345.)))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE258]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE258]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a_3]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_9]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE256]], read<bool>(%[[VALUE258]]));
// DEFAULT-NEXT:             let %[[VALUE259:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE256]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE259]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d128>(%[[VALUE_a_3]], call<d128, signature=fn(i128) -> d128>(%[[VALUE_tests128]], widen<i128, reason=arg>(const<i64>(123456789123456789))));
// DEFAULT-NEXT:                 write<d128>(%[[VALUE_b_9]], const<d128>(123456789123456789.));
// DEFAULT-NEXT:                 let %[[VALUE260:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a_3]]), const<d128>(123456789123456789.))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE260]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE260]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a_3]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_9]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE259]], read<bool>(%[[VALUE260]]));
// DEFAULT-NEXT:             let %[[VALUE261:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE259]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE261]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d128>(%[[VALUE_a_3]], call<d128, signature=fn(i128) -> d128>(%[[VALUE_tests128]], or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(42163417))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(15105683216109345905))))));
// DEFAULT-NEXT:                 write<d128>(%[[VALUE_b_9]], const<d128>(777777777777777777777777777.));
// DEFAULT-NEXT:                 let %[[VALUE262:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a_3]]), const<d128>(777777777777777777777777777.))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE262]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE262]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a_3]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_9]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE261]], read<bool>(%[[VALUE262]]));
// DEFAULT-NEXT:             let %[[VALUE263:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE261]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE263]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d128>(%[[VALUE_a_3]], call<d128, signature=fn(i128) -> d128>(%[[VALUE_tests128]], or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(542101086242752))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(4003012203850112768))))));
// DEFAULT-NEXT:                 write<d128>(%[[VALUE_b_9]], const<d128>(9999999999999999999999999900000000.));
// DEFAULT-NEXT:                 let %[[VALUE264:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a_3]]), const<d128>(9999999999999999999999999900000000.))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE264]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE264]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a_3]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_9]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE263]], read<bool>(%[[VALUE264]]));
// DEFAULT-NEXT:             let %[[VALUE265:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE263]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE265]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d128>(%[[VALUE_a_3]], call<d128, signature=fn(i128) -> d128>(%[[VALUE_tests128]], neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(542101086242752))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(4003012203950112767)))))));
// DEFAULT-NEXT:                 let %[[VALUE266:[0-9]+]]: d128 [synthetic] = neg<d128>(const<d128>(9999999999999999999999999999999999.));
// DEFAULT-NEXT:                 write<d128>(%[[VALUE_b_9]], read<d128>(%[[VALUE266]]));
// DEFAULT-NEXT:                 let %[[VALUE267:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a_3]]), neg<d128>(const<d128>(9999999999999999999999999999999999.)))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE267]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE267]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a_3]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_9]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE265]], read<bool>(%[[VALUE267]]));
// DEFAULT-NEXT:             let %[[VALUE268:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE265]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE268]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d128>(%[[VALUE_a_3]], call<d128, signature=fn(i128) -> d128>(%[[VALUE_tests128]], neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(5421010862427522))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(3136633892082024442)))))));
// DEFAULT-NEXT:                 let %[[VALUE269:[0-9]+]]: d128 [synthetic] = neg<d128>(const<d128>(9999999999999999999999999999999999.e+1));
// DEFAULT-NEXT:                 write<d128>(%[[VALUE_b_9]], read<d128>(%[[VALUE269]]));
// DEFAULT-NEXT:                 let %[[VALUE270:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a_3]]), neg<d128>(const<d128>(9999999999999999999999999999999999.e+1)))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE270]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE270]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a_3]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_9]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE268]], read<bool>(%[[VALUE270]]));
// DEFAULT-NEXT:             let %[[VALUE271:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE268]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE271]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d128>(%[[VALUE_a_3]], call<d128, signature=fn(i128) -> d128>(%[[VALUE_tests128]], or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(5421010862427522))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(3136633892082024443))))));
// DEFAULT-NEXT:                 write<d128>(%[[VALUE_b_9]], const<d128>(1000000000000000000000000000000000.e+2));
// DEFAULT-NEXT:                 let %[[VALUE272:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a_3]]), const<d128>(1000000000000000000000000000000000.e+2))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE272]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE272]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a_3]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_9]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE271]], read<bool>(%[[VALUE272]]));
// DEFAULT-NEXT:             let %[[VALUE273:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE271]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE273]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d128>(%[[VALUE_a_3]], call<d128, signature=fn(i128) -> d128>(%[[VALUE_tests128]], or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(54210108624275221))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(12919594847110692764))))));
// DEFAULT-NEXT:                 write<d128>(%[[VALUE_b_9]], const<d128>(9999999999999999999999999999999999.e+2));
// DEFAULT-NEXT:                 let %[[VALUE274:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a_3]]), const<d128>(9999999999999999999999999999999999.e+2))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE274]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE274]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a_3]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_9]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE273]], read<bool>(%[[VALUE274]]));
// DEFAULT-NEXT:             let %[[VALUE275:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE273]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE275]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d128>(%[[VALUE_a_3]], call<d128, signature=fn(i128) -> d128>(%[[VALUE_tests128]], or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(54210108624275221))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(12919594847110692813))))));
// DEFAULT-NEXT:                 write<d128>(%[[VALUE_b_9]], const<d128>(9999999999999999999999999999999999.e+2));
// DEFAULT-NEXT:                 let %[[VALUE276:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a_3]]), const<d128>(9999999999999999999999999999999999.e+2))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE276]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE276]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a_3]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_9]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE275]], read<bool>(%[[VALUE276]]));
// DEFAULT-NEXT:             let %[[VALUE277:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE275]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE277]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d128>(%[[VALUE_a_3]], call<d128, signature=fn(i128) -> d128>(%[[VALUE_tests128]], neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(542101086242752217))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(68739955140066328)))))));
// DEFAULT-NEXT:                 let %[[VALUE278:[0-9]+]]: d128 [synthetic] = neg<d128>(const<d128>(9999999999999999999999999999999999.e+3));
// DEFAULT-NEXT:                 write<d128>(%[[VALUE_b_9]], read<d128>(%[[VALUE278]]));
// DEFAULT-NEXT:                 let %[[VALUE279:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a_3]]), neg<d128>(const<d128>(9999999999999999999999999999999999.e+3)))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE279]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE279]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a_3]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_9]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE277]], read<bool>(%[[VALUE279]]));
// DEFAULT-NEXT:             let %[[VALUE280:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE277]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE280]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d128>(%[[VALUE_a_3]], call<d128, signature=fn(i128) -> d128>(%[[VALUE_tests128]], or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(542101086242752217))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(68739955140066827))))));
// DEFAULT-NEXT:                 write<d128>(%[[VALUE_b_9]], const<d128>(9999999999999999999999999999999999.e+3));
// DEFAULT-NEXT:                 let %[[VALUE281:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a_3]]), const<d128>(9999999999999999999999999999999999.e+3))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE281]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE281]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a_3]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_9]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE280]], read<bool>(%[[VALUE281]]));
// DEFAULT-NEXT:             let %[[VALUE282:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE280]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE282]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d128>(%[[VALUE_a_3]], call<d128, signature=fn(i128) -> d128>(%[[VALUE_tests128]], or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(9223372036854775807))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(18446744073709545888))))));
// DEFAULT-NEXT:                 write<d128>(%[[VALUE_b_9]], const<d128>(1701411834604692317316873037158841.e+5));
// DEFAULT-NEXT:                 let %[[VALUE283:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a_3]]), const<d128>(1701411834604692317316873037158841.e+5))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE283]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE283]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a_3]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_9]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE282]], read<bool>(%[[VALUE283]]));
// DEFAULT-NEXT:             let %[[VALUE284:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE282]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE284]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d128>(%[[VALUE_a_3]], call<d128, signature=fn(i128) -> d128>(%[[VALUE_tests128]], reinterpret<i128, reason=explicit, fits=unknown>(sub<u128, overflow=wrap>(shl<u128, overflow=wrap, amount_out_of_range=ub>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(1))), const<i32>(127)), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(1)))))));
// DEFAULT-NEXT:                 write<d128>(%[[VALUE_b_9]], const<d128>(1701411834604692317316873037158841.e+5));
// DEFAULT-NEXT:                 let %[[VALUE285:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a_3]]), const<d128>(1701411834604692317316873037158841.e+5))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE285]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE285]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a_3]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_9]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE284]], read<bool>(%[[VALUE285]]));
// DEFAULT-NEXT:             let %[[VALUE286:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE284]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE286]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d128>(%[[VALUE_a_3]], call<d128, signature=fn(i128) -> d128>(%[[VALUE_tests128]], sub<i128, overflow=ub>(neg<i128, overflow=ub>(reinterpret<i128, reason=explicit, fits=unknown>(sub<u128, overflow=wrap>(shl<u128, overflow=wrap, amount_out_of_range=ub>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(1))), const<i32>(127)), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(1)))))), widen<i128, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                 let %[[VALUE287:[0-9]+]]: d128 [synthetic] = neg<d128>(const<d128>(1701411834604692317316873037158841.e+5));
// DEFAULT-NEXT:                 write<d128>(%[[VALUE_b_9]], read<d128>(%[[VALUE287]]));
// DEFAULT-NEXT:                 let %[[VALUE288:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a_3]]), neg<d128>(const<d128>(1701411834604692317316873037158841.e+5)))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE288]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE288]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a_3]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_9]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE286]], read<bool>(%[[VALUE288]]));
// DEFAULT-NEXT:             if read<bool>(%[[VALUE286]])
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:             write<d128>(%[[VALUE_a_3]], call<d128, signature=fn(u128) -> d128>(%[[VALUE_testu128]], widen<u128, reason=arg>(const<u64>(0))));
// DEFAULT-NEXT:             write<d128>(%[[VALUE_b_9]], const<d128>(0.));
// DEFAULT-NEXT:             let %[[VALUE289:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a_3]]), const<d128>(0.))
// DEFAULT-NEXT:                 write<bool>(%[[VALUE289]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<bool>(%[[VALUE289]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a_3]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_9]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:             let %[[VALUE290:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE289]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE290]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d128>(%[[VALUE_a_3]], call<d128, signature=fn(u128) -> d128>(%[[VALUE_testu128]], widen<u128, reason=arg>(const<u64>(7))));
// DEFAULT-NEXT:                 write<d128>(%[[VALUE_b_9]], const<d128>(7.));
// DEFAULT-NEXT:                 let %[[VALUE291:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a_3]]), const<d128>(7.))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE291]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE291]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a_3]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_9]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE290]], read<bool>(%[[VALUE291]]));
// DEFAULT-NEXT:             let %[[VALUE292:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE290]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE292]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d128>(%[[VALUE_a_3]], call<d128, signature=fn(u128) -> d128>(%[[VALUE_testu128]], widen<u128, reason=arg>(const<u64>(42))));
// DEFAULT-NEXT:                 write<d128>(%[[VALUE_b_9]], const<d128>(42.));
// DEFAULT-NEXT:                 let %[[VALUE293:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a_3]]), const<d128>(42.))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE293]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE293]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a_3]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_9]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE292]], read<bool>(%[[VALUE293]]));
// DEFAULT-NEXT:             let %[[VALUE294:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE292]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE294]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d128>(%[[VALUE_a_3]], call<d128, signature=fn(u128) -> d128>(%[[VALUE_testu128]], widen<u128, reason=arg>(const<u64>(777777777))));
// DEFAULT-NEXT:                 write<d128>(%[[VALUE_b_9]], const<d128>(777777777.));
// DEFAULT-NEXT:                 let %[[VALUE295:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a_3]]), const<d128>(777777777.))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE295]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE295]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a_3]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_9]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE294]], read<bool>(%[[VALUE295]]));
// DEFAULT-NEXT:             let %[[VALUE296:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE294]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE296]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d128>(%[[VALUE_a_3]], call<d128, signature=fn(u128) -> d128>(%[[VALUE_testu128]], or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(5421010862)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(7886392056514346008)))));
// DEFAULT-NEXT:                 write<d128>(%[[VALUE_b_9]], const<d128>(99999999999999999999999999000.));
// DEFAULT-NEXT:                 let %[[VALUE297:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a_3]]), const<d128>(99999999999999999999999999000.))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE297]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE297]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a_3]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_9]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE296]], read<bool>(%[[VALUE297]]));
// DEFAULT-NEXT:             let %[[VALUE298:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE296]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE298]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d128>(%[[VALUE_a_3]], call<d128, signature=fn(u128) -> d128>(%[[VALUE_testu128]], or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(54210108624275221)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(12919594847110692764)))));
// DEFAULT-NEXT:                 write<d128>(%[[VALUE_b_9]], const<d128>(9999999999999999999999999999999999.e+2));
// DEFAULT-NEXT:                 let %[[VALUE299:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a_3]]), const<d128>(9999999999999999999999999999999999.e+2))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE299]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE299]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a_3]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_9]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE298]], read<bool>(%[[VALUE299]]));
// DEFAULT-NEXT:             let %[[VALUE300:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE298]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE300]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d128>(%[[VALUE_a_3]], call<d128, signature=fn(u128) -> d128>(%[[VALUE_testu128]], or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(542101086242752217)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(68739955140066328)))));
// DEFAULT-NEXT:                 write<d128>(%[[VALUE_b_9]], const<d128>(9999999999999999999999999999999999.e+3));
// DEFAULT-NEXT:                 let %[[VALUE301:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a_3]]), const<d128>(9999999999999999999999999999999999.e+3))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE301]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE301]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a_3]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_9]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE300]], read<bool>(%[[VALUE301]]));
// DEFAULT-NEXT:             let %[[VALUE302:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE300]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE302]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d128>(%[[VALUE_a_3]], call<d128, signature=fn(u128) -> d128>(%[[VALUE_testu128]], or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(5421010862427522170)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(687399551400668279)))));
// DEFAULT-NEXT:                 write<d128>(%[[VALUE_b_9]], const<d128>(9999999999999999999999999999999999.e+4));
// DEFAULT-NEXT:                 let %[[VALUE303:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a_3]]), const<d128>(9999999999999999999999999999999999.e+4))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE303]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE303]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a_3]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_9]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE302]], read<bool>(%[[VALUE303]]));
// DEFAULT-NEXT:             let %[[VALUE304:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE302]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE304]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d128>(%[[VALUE_a_3]], call<d128, signature=fn(u128) -> d128>(%[[VALUE_testu128]], or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(18446744073709551615)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(18446744073709540160)))));
// DEFAULT-NEXT:                 write<d128>(%[[VALUE_b_9]], const<d128>(3402823669209384634633746074317682.e+5));
// DEFAULT-NEXT:                 let %[[VALUE305:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a_3]]), const<d128>(3402823669209384634633746074317682.e+5))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE305]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE305]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a_3]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_9]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE304]], read<bool>(%[[VALUE305]]));
// DEFAULT-NEXT:             let %[[VALUE306:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             if read<bool>(%[[VALUE304]])
// DEFAULT-NEXT:                 write<bool>(%[[VALUE306]], const<bool>(true));
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<d128>(%[[VALUE_a_3]], call<d128, signature=fn(u128) -> d128>(%[[VALUE_testu128]], not<u128>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(0))))));
// DEFAULT-NEXT:                 write<d128>(%[[VALUE_b_9]], const<d128>(3402823669209384634633746074317682.e+5));
// DEFAULT-NEXT:                 let %[[VALUE307:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<d128, exceptions=observable>(read<d128>(%[[VALUE_a_3]]), const<d128>(3402823669209384634633746074317682.e+5))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE307]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE307]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_a_3]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<d128>>(%[[VALUE_b_9]])), const<u64>(16)), const<i32>(0)));
// DEFAULT-NEXT:                 write<bool>(%[[VALUE306]], read<bool>(%[[VALUE307]]));
// DEFAULT-NEXT:             if read<bool>(%[[VALUE306]])
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
