/* PR libgcc/65833 */
/* { dg-require-effective-target int128 } */
/* { dg-require-effective-target bitint } */
/* { dg-options "-O2 -std=gnu2x" } */

#define INT128_MAX ((__int128) ((((unsigned __int128) 1) << 127) - 1))
#define UINT128_MAX (~(unsigned __int128) 0)
#define C(x, y) ((((__int128) (x##ULL)) << 64) | (y##ULL))
#define UC(x, y) ((((unsigned __int128) (x##ULL)) << 64) | (y##ULL))

__attribute__((noipa)) __int128
tests64 (_Decimal64 d)
{
  return d;
}

__attribute__((noipa)) unsigned __int128
testu64 (_Decimal64 d)
{
  return d;
}

__attribute__((noipa)) __int128
tests32 (_Decimal32 d)
{
  return d;
}

__attribute__((noipa)) unsigned __int128
testu32 (_Decimal32 d)
{
  return d;
}

__attribute__((noipa)) __int128
tests128 (_Decimal128 d)
{
  return d;
}

__attribute__((noipa)) unsigned __int128
testu128 (_Decimal128 d)
{
  return d;
}

int
main ()
{
  if (tests64 (0.DD) != 0
      || tests64 (0.9999999999999999DD) != 0
      || tests64 (7.999999999999999DD) != 7
      || tests64 (-0.DD) != 0
      || tests64 (-0.9999999999999999DD) != 0
      || tests64 (-42.5DD) != -42
      || tests64 (-34242319854.45429e+27DD) != -C (0x19c2d4b6fefc3378, 0xa349b93967400000)
      || tests64 (-213855087769445.9e+23DD) != -C (0x1016b2fcff8f2cf6, 0xd16cf61904c00000)
      || tests64 (1701411834604692.0e+23DD) != C (0x7ffffffffffff947, 0xd26076f482000000)
      || tests64 (-1701411834604692.0e+23DD) != -C (0x7ffffffffffff947, 0xd26076f482000000))
    __builtin_abort ();
  if (tests64 (1701411834604693.0e+23DD) != INT128_MAX
      || tests64 (9999999999999999e+369DD) != INT128_MAX
      || tests64 (-1701411834604693.0e+23DD) != -INT128_MAX - 1
      || tests64 (-9999999999999999e+369DD) != -INT128_MAX - 1)
    __builtin_abort ();
  if (testu64 (0.DD) != 0
      || testu64 (0.9999999999999999DD) != 0
      || testu64 (-0.9999999999999999DD) != 0
      || testu64 (-0.0DD) != 0
      || testu64 (-0.5DD) != 0
      || testu64 (42.99999999999999DD) != 42
      || testu64 (42.e+21DD) != UC (0x8e4, 0xd316827686400000)
      || testu64 (34272319854.45429e+27DD) != C (0x19c89bd43b04cab9, 0x49f2646567400000)
      || testu64 (3402823669209384.0e+23DD) != C (0xfffffffffffff28f, 0xa4c0ede904000000))
    __builtin_abort ();
  if (testu64 (-1.DD) != 0
      || testu64 (-42.5e+15DD) != 0
      || testu64 (-9999999999999999e+369DD) != 0
      || testu64 (3402823669209385.0e+23DD) != UINT128_MAX
      || testu64 (9999999999999999e+369DD) != UINT128_MAX)
    __builtin_abort ();

  if (tests32 (0.DF) != 0
      || tests32 (0.9999999DF) != 0
      || tests32 (7.999999DF) != 7
      || tests32 (-0.000DF) != 0
      || tests32 (-0.9999999DF) != 0
      || tests32 (-1.DF) != -1
      || tests32 (-42.5DF) != -42
      || tests32 (-3424.231e+27DF) != -C (0x2b38497f00, 0x9c4e190b47000000)
      || tests32 (-213855.9e+32DF) != -C (0x1016b6fe2d67e732, 0x717a483980000000)
      || tests32 (1701411.0e+32DF) != C (0x7ffffbe294adefda, 0xd863b4a300000000)
      || tests32 (-1701411.0e+32DF) != -C (0x7ffffbe294adefda, 0xd863b4a300000000))
    __builtin_abort ();
  if (tests32 (1701412.0e+32DF) != INT128_MAX
      || tests32 (9999999e+90DF) != INT128_MAX
      || tests32 (-1701412.0e+32DF) != -INT128_MAX - 1
      || tests32 (-9999999e+90DF) != -INT128_MAX - 1)
    __builtin_abort ();
  if (testu32 (0.DF) != 0
      || testu32 (0.9999999DF) != 0
      || testu32 (-0.9999999DF) != 0
      || testu32 (-0.5DF) != 0
      || testu32 (-0.0000DF) != 0
      || testu32 (-0.99999DF) != 0
      || testu32 (42.99999DF) != 42
      || testu32 (42.e+21DF) != UC (0x8e4, 0xd316827686400000)
      || testu32 (3402.823e+35DF) != UC (0xfffffcb356c92111, 0x367458c700000000))
    __builtin_abort ();
  if (testu32 (-1.DF) != 0
      || testu32 (-42.5e+15DF) != 0
      || testu32 (-9999999e+90DF) != 0
      || testu32 (3402.824e+35DF) != UINT128_MAX
      || testu32 (9999999e+90DF) != UINT128_MAX)
    __builtin_abort ();

  if (tests128 (0.DL) != 0
      || tests128 (0.9999999999999999999999999999999999DL) != 0
      || tests128 (7.999999999999999999999999999999999DL) != 7
      || tests128 (-0.DL) != 0
      || tests128 (-0.9999999999999999999999999999999999DL) != 0
      || tests128 (-1.DL) != -1
      || tests128 (-42.5DL) != -42
      || tests128 (-34242319854.45429439857871298745432e+27DL) != -C (0x19c2d4b6fefc3467, 0x15d47c047b56ad80)
      || tests128 (-213855087769445.9e+23DL) != -C (0x1016b2fcff8f2cf6, 0xd16cf61904c00000)
      || tests128 (1701411834604692317316873037158841.0e+5DL) != C (0x7fffffffffffffff, 0xffffffffffffe9a0)
      || tests128 (-1701411834604692317316873037158841.0e+5DL) != -C (0x7fffffffffffffff, 0xffffffffffffe9a0))
    __builtin_abort ();
  if (tests128 (1701411834604692317316873037158842.0e+5DL) != INT128_MAX
      || tests128 (9999999999999999999999999999999999e+6111DL) != INT128_MAX
      || tests128 (-1701411834604692317316873037158842.0e+5DL) != -INT128_MAX - 1
      || tests128 (-9999999999999999999999999999999999e+6111DL) != -INT128_MAX - 1)
    __builtin_abort ();
  if (testu128 (0.DL) != 0
      || testu128 (0.9999999999999999999999999999999999DL) != 0
      || testu128 (-0.9999999999999999999999999999999999DL) != 0
      || testu128 (-0.DL) != 0
      || testu128 (-0.9999999999999999999999DL) != 0
      || testu128 (-0.5DL) != 0
      || testu128 (42.99999999999999999999999999999999DL) != 42
      || testu128 (42.e+21DL) != UC (0x8e4, 0xd316827686400000)
      || testu128 (34242319854.45429439857871298745432e+21DL) != UC (0x1b032e71cc9, 0x24b5cd6d86a9473e)
      || testu128 (3402823669209384634633746074317.682e+8DL) != UC (0xffffffffffffffff, 0xffffffffffffd340))
    __builtin_abort ();
  if (testu128 (-1.DL) != 0
      || testu128 (-42.5e+15DL) != 0
      || testu128 (-9999999999999999999999999999999999e+6111DL) != 0
      || testu128 (3402823669209384634633746074317.683e+8DL) != UINT128_MAX
      || testu128 (9999999999999999999999999999999999e+6111DL) != UINT128_MAX)
    __builtin_abort ();
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
// DEFAULT-NEXT:     fn %[[VALUE_tests64:[0-9]+]] @tests64(%[[VALUE_d:[0-9]+]] d: d64) -> i128 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return float_to_int<i128, reason=return, out_of_range=ub, exceptions=observable>(read<d64>(%[[VALUE_d]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testu64:[0-9]+]] @testu64(%[[VALUE_d_2:[0-9]+]] d: d64) -> u128 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return float_to_int<u128, reason=return, out_of_range=ub, exceptions=observable>(read<d64>(%[[VALUE_d_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_tests32:[0-9]+]] @tests32(%[[VALUE_d_3:[0-9]+]] d: d32) -> i128 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return float_to_int<i128, reason=return, out_of_range=ub, exceptions=observable>(read<d32>(%[[VALUE_d_3]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testu32:[0-9]+]] @testu32(%[[VALUE_d_4:[0-9]+]] d: d32) -> u128 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return float_to_int<u128, reason=return, out_of_range=ub, exceptions=observable>(read<d32>(%[[VALUE_d_4]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_tests128:[0-9]+]] @tests128(%[[VALUE_d_5:[0-9]+]] d: d128) -> i128 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return float_to_int<i128, reason=return, out_of_range=ub, exceptions=observable>(read<d128>(%[[VALUE_d_5]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testu128:[0-9]+]] @testu128(%[[VALUE_d_6:[0-9]+]] d: d128) -> u128 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return float_to_int<u128, reason=return, out_of_range=ub, exceptions=observable>(read<d128>(%[[VALUE_d_6]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<i128>(call<i128, signature=fn(d64) -> i128>(%[[VALUE_tests64]], const<d64>(0.)), widen<i128, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             write<bool>(%[[VALUE0]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE0]], ne<i128>(call<i128, signature=fn(d64) -> i128>(%[[VALUE_tests64]], const<d64>(0.9999999999999999)), widen<i128, reason=usual_arith>(const<i32>(0))));
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE0]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE1]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE1]], ne<i128>(call<i128, signature=fn(d64) -> i128>(%[[VALUE_tests64]], const<d64>(7.999999999999999)), widen<i128, reason=usual_arith>(const<i32>(7))));
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE1]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE2]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE2]], ne<i128>(call<i128, signature=fn(d64) -> i128>(%[[VALUE_tests64]], neg<d64>(const<d64>(0.))), widen<i128, reason=usual_arith>(const<i32>(0))));
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE2]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE3]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE3]], ne<i128>(call<i128, signature=fn(d64) -> i128>(%[[VALUE_tests64]], neg<d64>(const<d64>(0.9999999999999999))), widen<i128, reason=usual_arith>(const<i32>(0))));
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE3]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE4]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE4]], ne<i128>(call<i128, signature=fn(d64) -> i128>(%[[VALUE_tests64]], neg<d64>(const<d64>(42.5))), widen<i128, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(42)))));
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE4]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE5]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE5]], ne<i128>(call<i128, signature=fn(d64) -> i128>(%[[VALUE_tests64]], neg<d64>(const<d64>(34242319854.45429e+27))), neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(1856279878857143160))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(11766139157678653440)))))));
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE5]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE6]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE6]], ne<i128>(call<i128, signature=fn(d64) -> i128>(%[[VALUE_tests64]], neg<d64>(const<d64>(213855087769445.9e+23))), neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(1159310753783557366))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(15090707038725996544)))))));
// DEFAULT-NEXT:         let %[[VALUE7:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE6]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE7]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE7]], ne<i128>(call<i128, signature=fn(d64) -> i128>(%[[VALUE_tests64]], const<d64>(1701411834604692.0e+23)), or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(9223372036854774087))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(15159247138254225408))))));
// DEFAULT-NEXT:         let %[[VALUE8:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE7]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE8]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE8]], ne<i128>(call<i128, signature=fn(d64) -> i128>(%[[VALUE_tests64]], neg<d64>(const<d64>(1701411834604692.0e+23))), neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(9223372036854774087))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(15159247138254225408)))))));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE8]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         let %[[VALUE9:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<i128>(call<i128, signature=fn(d64) -> i128>(%[[VALUE_tests64]], const<d64>(1701411834604693.0e+23)), reinterpret<i128, reason=explicit, fits=unknown>(sub<u128, overflow=wrap>(shl<u128, overflow=wrap, amount_out_of_range=ub>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(1))), const<i32>(127)), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(1))))))
// DEFAULT-NEXT:             write<bool>(%[[VALUE9]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE9]], ne<i128>(call<i128, signature=fn(d64) -> i128>(%[[VALUE_tests64]], const<d64>(9999999999999999e+369)), reinterpret<i128, reason=explicit, fits=unknown>(sub<u128, overflow=wrap>(shl<u128, overflow=wrap, amount_out_of_range=ub>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(1))), const<i32>(127)), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(1)))))));
// DEFAULT-NEXT:         let %[[VALUE10:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE9]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE10]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE10]], ne<i128>(call<i128, signature=fn(d64) -> i128>(%[[VALUE_tests64]], neg<d64>(const<d64>(1701411834604693.0e+23))), sub<i128, overflow=ub>(neg<i128, overflow=ub>(reinterpret<i128, reason=explicit, fits=unknown>(sub<u128, overflow=wrap>(shl<u128, overflow=wrap, amount_out_of_range=ub>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(1))), const<i32>(127)), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(1)))))), widen<i128, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         let %[[VALUE11:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE10]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE11]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE11]], ne<i128>(call<i128, signature=fn(d64) -> i128>(%[[VALUE_tests64]], neg<d64>(const<d64>(9999999999999999e+369))), sub<i128, overflow=ub>(neg<i128, overflow=ub>(reinterpret<i128, reason=explicit, fits=unknown>(sub<u128, overflow=wrap>(shl<u128, overflow=wrap, amount_out_of_range=ub>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(1))), const<i32>(127)), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(1)))))), widen<i128, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE11]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         let %[[VALUE12:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<u128>(call<u128, signature=fn(d64) -> u128>(%[[VALUE_testu64]], const<d64>(0.)), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%[[VALUE12]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE12]], ne<u128>(call<u128, signature=fn(d64) -> u128>(%[[VALUE_testu64]], const<d64>(0.9999999999999999)), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(0)))));
// DEFAULT-NEXT:         let %[[VALUE13:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE12]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE13]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE13]], ne<u128>(call<u128, signature=fn(d64) -> u128>(%[[VALUE_testu64]], neg<d64>(const<d64>(0.9999999999999999))), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(0)))));
// DEFAULT-NEXT:         let %[[VALUE14:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE13]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE14]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE14]], ne<u128>(call<u128, signature=fn(d64) -> u128>(%[[VALUE_testu64]], neg<d64>(const<d64>(0.0))), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(0)))));
// DEFAULT-NEXT:         let %[[VALUE15:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE14]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE15]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE15]], ne<u128>(call<u128, signature=fn(d64) -> u128>(%[[VALUE_testu64]], neg<d64>(const<d64>(0.5))), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(0)))));
// DEFAULT-NEXT:         let %[[VALUE16:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE15]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE16]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE16]], ne<u128>(call<u128, signature=fn(d64) -> u128>(%[[VALUE_testu64]], const<d64>(42.99999999999999)), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(42)))));
// DEFAULT-NEXT:         let %[[VALUE17:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE16]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE17]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE17]], ne<u128>(call<u128, signature=fn(d64) -> u128>(%[[VALUE_testu64]], const<d64>(42.e+21)), or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(2276)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(15210488237060521984)))));
// DEFAULT-NEXT:         let %[[VALUE18:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE17]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE18]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE18]], ne<u128>(call<u128, signature=fn(d64) -> u128>(%[[VALUE_testu64]], const<d64>(34272319854.45429e+27)), reinterpret<u128, reason=usual_arith, fits=unknown>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(1857906182115871417))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(5328431695819440128)))))));
// DEFAULT-NEXT:         let %[[VALUE19:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE18]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE19]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE19]], ne<u128>(call<u128, signature=fn(d64) -> u128>(%[[VALUE_testu64]], const<d64>(3402823669209384.0e+23)), reinterpret<u128, reason=usual_arith, fits=unknown>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(18446744073709548175))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(11871750202798899200)))))));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE19]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         let %[[VALUE20:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<u128>(call<u128, signature=fn(d64) -> u128>(%[[VALUE_testu64]], neg<d64>(const<d64>(1.))), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%[[VALUE20]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE20]], ne<u128>(call<u128, signature=fn(d64) -> u128>(%[[VALUE_testu64]], neg<d64>(const<d64>(42.5e+15))), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(0)))));
// DEFAULT-NEXT:         let %[[VALUE21:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE20]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE21]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE21]], ne<u128>(call<u128, signature=fn(d64) -> u128>(%[[VALUE_testu64]], neg<d64>(const<d64>(9999999999999999e+369))), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(0)))));
// DEFAULT-NEXT:         let %[[VALUE22:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE21]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE22]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE22]], ne<u128>(call<u128, signature=fn(d64) -> u128>(%[[VALUE_testu64]], const<d64>(3402823669209385.0e+23)), not<u128>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(0))))));
// DEFAULT-NEXT:         let %[[VALUE23:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE22]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE23]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE23]], ne<u128>(call<u128, signature=fn(d64) -> u128>(%[[VALUE_testu64]], const<d64>(9999999999999999e+369)), not<u128>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(0))))));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE23]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         let %[[VALUE24:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<i128>(call<i128, signature=fn(d32) -> i128>(%[[VALUE_tests32]], const<d32>(0.)), widen<i128, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             write<bool>(%[[VALUE24]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE24]], ne<i128>(call<i128, signature=fn(d32) -> i128>(%[[VALUE_tests32]], const<d32>(0.9999999)), widen<i128, reason=usual_arith>(const<i32>(0))));
// DEFAULT-NEXT:         let %[[VALUE25:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE24]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE25]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE25]], ne<i128>(call<i128, signature=fn(d32) -> i128>(%[[VALUE_tests32]], const<d32>(7.999999)), widen<i128, reason=usual_arith>(const<i32>(7))));
// DEFAULT-NEXT:         let %[[VALUE26:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE25]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE26]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE26]], ne<i128>(call<i128, signature=fn(d32) -> i128>(%[[VALUE_tests32]], neg<d32>(const<d32>(0.000))), widen<i128, reason=usual_arith>(const<i32>(0))));
// DEFAULT-NEXT:         let %[[VALUE27:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE26]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE27]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE27]], ne<i128>(call<i128, signature=fn(d32) -> i128>(%[[VALUE_tests32]], neg<d32>(const<d32>(0.9999999))), widen<i128, reason=usual_arith>(const<i32>(0))));
// DEFAULT-NEXT:         let %[[VALUE28:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE27]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE28]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE28]], ne<i128>(call<i128, signature=fn(d32) -> i128>(%[[VALUE_tests32]], neg<d32>(const<d32>(1.))), widen<i128, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))));
// DEFAULT-NEXT:         let %[[VALUE29:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE28]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE29]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE29]], ne<i128>(call<i128, signature=fn(d32) -> i128>(%[[VALUE_tests32]], neg<d32>(const<d32>(42.5))), widen<i128, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(42)))));
// DEFAULT-NEXT:         let %[[VALUE30:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE29]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE30]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE30]], ne<i128>(call<i128, signature=fn(d32) -> i128>(%[[VALUE_tests32]], neg<d32>(const<d32>(3424.231e+27))), neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(185627934464))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(11262967254326706176)))))));
// DEFAULT-NEXT:         let %[[VALUE31:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE30]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE31]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE31]], ne<i128>(call<i128, signature=fn(d32) -> i128>(%[[VALUE_tests32]], neg<d32>(const<d32>(213855.9e+32))), neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(1159315156894213938))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(8176927485242376192)))))));
// DEFAULT-NEXT:         let %[[VALUE32:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE31]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE32]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE32]], ne<i128>(call<i128, signature=fn(d32) -> i128>(%[[VALUE_tests32]], const<d32>(1701411.0e+32)), or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(9223367512453672922))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(15592504947059458048))))));
// DEFAULT-NEXT:         let %[[VALUE33:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE32]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE33]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE33]], ne<i128>(call<i128, signature=fn(d32) -> i128>(%[[VALUE_tests32]], neg<d32>(const<d32>(1701411.0e+32))), neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(9223367512453672922))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(15592504947059458048)))))));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE33]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         let %[[VALUE34:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<i128>(call<i128, signature=fn(d32) -> i128>(%[[VALUE_tests32]], const<d32>(1701412.0e+32)), reinterpret<i128, reason=explicit, fits=unknown>(sub<u128, overflow=wrap>(shl<u128, overflow=wrap, amount_out_of_range=ub>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(1))), const<i32>(127)), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(1))))))
// DEFAULT-NEXT:             write<bool>(%[[VALUE34]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE34]], ne<i128>(call<i128, signature=fn(d32) -> i128>(%[[VALUE_tests32]], const<d32>(9999999e+90)), reinterpret<i128, reason=explicit, fits=unknown>(sub<u128, overflow=wrap>(shl<u128, overflow=wrap, amount_out_of_range=ub>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(1))), const<i32>(127)), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(1)))))));
// DEFAULT-NEXT:         let %[[VALUE35:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE34]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE35]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE35]], ne<i128>(call<i128, signature=fn(d32) -> i128>(%[[VALUE_tests32]], neg<d32>(const<d32>(1701412.0e+32))), sub<i128, overflow=ub>(neg<i128, overflow=ub>(reinterpret<i128, reason=explicit, fits=unknown>(sub<u128, overflow=wrap>(shl<u128, overflow=wrap, amount_out_of_range=ub>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(1))), const<i32>(127)), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(1)))))), widen<i128, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         let %[[VALUE36:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE35]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE36]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE36]], ne<i128>(call<i128, signature=fn(d32) -> i128>(%[[VALUE_tests32]], neg<d32>(const<d32>(9999999e+90))), sub<i128, overflow=ub>(neg<i128, overflow=ub>(reinterpret<i128, reason=explicit, fits=unknown>(sub<u128, overflow=wrap>(shl<u128, overflow=wrap, amount_out_of_range=ub>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(1))), const<i32>(127)), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(1)))))), widen<i128, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE36]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         let %[[VALUE37:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<u128>(call<u128, signature=fn(d32) -> u128>(%[[VALUE_testu32]], const<d32>(0.)), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%[[VALUE37]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE37]], ne<u128>(call<u128, signature=fn(d32) -> u128>(%[[VALUE_testu32]], const<d32>(0.9999999)), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(0)))));
// DEFAULT-NEXT:         let %[[VALUE38:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE37]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE38]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE38]], ne<u128>(call<u128, signature=fn(d32) -> u128>(%[[VALUE_testu32]], neg<d32>(const<d32>(0.9999999))), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(0)))));
// DEFAULT-NEXT:         let %[[VALUE39:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE38]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE39]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE39]], ne<u128>(call<u128, signature=fn(d32) -> u128>(%[[VALUE_testu32]], neg<d32>(const<d32>(0.5))), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(0)))));
// DEFAULT-NEXT:         let %[[VALUE40:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE39]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE40]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE40]], ne<u128>(call<u128, signature=fn(d32) -> u128>(%[[VALUE_testu32]], neg<d32>(const<d32>(0.0000))), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(0)))));
// DEFAULT-NEXT:         let %[[VALUE41:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE40]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE41]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE41]], ne<u128>(call<u128, signature=fn(d32) -> u128>(%[[VALUE_testu32]], neg<d32>(const<d32>(0.99999))), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(0)))));
// DEFAULT-NEXT:         let %[[VALUE42:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE41]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE42]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE42]], ne<u128>(call<u128, signature=fn(d32) -> u128>(%[[VALUE_testu32]], const<d32>(42.99999)), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(42)))));
// DEFAULT-NEXT:         let %[[VALUE43:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE42]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE43]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE43]], ne<u128>(call<u128, signature=fn(d32) -> u128>(%[[VALUE_testu32]], const<d32>(42.e+21)), or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(2276)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(15210488237060521984)))));
// DEFAULT-NEXT:         let %[[VALUE44:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE43]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE44]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE44]], ne<u128>(call<u128, signature=fn(d32) -> u128>(%[[VALUE_testu32]], const<d32>(3402.823e+35)), or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(18446740445918208273)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(3923858787068280832)))));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE44]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         let %[[VALUE45:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<u128>(call<u128, signature=fn(d32) -> u128>(%[[VALUE_testu32]], neg<d32>(const<d32>(1.))), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%[[VALUE45]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE45]], ne<u128>(call<u128, signature=fn(d32) -> u128>(%[[VALUE_testu32]], neg<d32>(const<d32>(42.5e+15))), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(0)))));
// DEFAULT-NEXT:         let %[[VALUE46:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE45]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE46]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE46]], ne<u128>(call<u128, signature=fn(d32) -> u128>(%[[VALUE_testu32]], neg<d32>(const<d32>(9999999e+90))), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(0)))));
// DEFAULT-NEXT:         let %[[VALUE47:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE46]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE47]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE47]], ne<u128>(call<u128, signature=fn(d32) -> u128>(%[[VALUE_testu32]], const<d32>(3402.824e+35)), not<u128>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(0))))));
// DEFAULT-NEXT:         let %[[VALUE48:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE47]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE48]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE48]], ne<u128>(call<u128, signature=fn(d32) -> u128>(%[[VALUE_testu32]], const<d32>(9999999e+90)), not<u128>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(0))))));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE48]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         let %[[VALUE49:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<i128>(call<i128, signature=fn(d128) -> i128>(%[[VALUE_tests128]], const<d128>(0.)), widen<i128, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             write<bool>(%[[VALUE49]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE49]], ne<i128>(call<i128, signature=fn(d128) -> i128>(%[[VALUE_tests128]], const<d128>(0.9999999999999999999999999999999999)), widen<i128, reason=usual_arith>(const<i32>(0))));
// DEFAULT-NEXT:         let %[[VALUE50:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE49]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE50]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE50]], ne<i128>(call<i128, signature=fn(d128) -> i128>(%[[VALUE_tests128]], const<d128>(7.999999999999999999999999999999999)), widen<i128, reason=usual_arith>(const<i32>(7))));
// DEFAULT-NEXT:         let %[[VALUE51:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE50]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE51]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE51]], ne<i128>(call<i128, signature=fn(d128) -> i128>(%[[VALUE_tests128]], neg<d128>(const<d128>(0.))), widen<i128, reason=usual_arith>(const<i32>(0))));
// DEFAULT-NEXT:         let %[[VALUE52:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE51]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE52]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE52]], ne<i128>(call<i128, signature=fn(d128) -> i128>(%[[VALUE_tests128]], neg<d128>(const<d128>(0.9999999999999999999999999999999999))), widen<i128, reason=usual_arith>(const<i32>(0))));
// DEFAULT-NEXT:         let %[[VALUE53:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE52]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE53]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE53]], ne<i128>(call<i128, signature=fn(d128) -> i128>(%[[VALUE_tests128]], neg<d128>(const<d128>(1.))), widen<i128, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))));
// DEFAULT-NEXT:         let %[[VALUE54:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE53]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE54]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE54]], ne<i128>(call<i128, signature=fn(d128) -> i128>(%[[VALUE_tests128]], neg<d128>(const<d128>(42.5))), widen<i128, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(42)))));
// DEFAULT-NEXT:         let %[[VALUE55:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE54]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE55]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE55]], ne<i128>(call<i128, signature=fn(d128) -> i128>(%[[VALUE_tests128]], neg<d128>(const<d128>(34242319854.45429439857871298745432e+27))), neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(1856279878857143399))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(1573018528550137216)))))));
// DEFAULT-NEXT:         let %[[VALUE56:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE55]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE56]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE56]], ne<i128>(call<i128, signature=fn(d128) -> i128>(%[[VALUE_tests128]], neg<d128>(const<d128>(213855087769445.9e+23))), neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(1159310753783557366))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(15090707038725996544)))))));
// DEFAULT-NEXT:         let %[[VALUE57:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE56]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE57]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE57]], ne<i128>(call<i128, signature=fn(d128) -> i128>(%[[VALUE_tests128]], const<d128>(1701411834604692317316873037158841.0e+5)), or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(9223372036854775807))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(18446744073709545888))))));
// DEFAULT-NEXT:         let %[[VALUE58:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE57]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE58]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE58]], ne<i128>(call<i128, signature=fn(d128) -> i128>(%[[VALUE_tests128]], neg<d128>(const<d128>(1701411834604692317316873037158841.0e+5))), neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(9223372036854775807))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(18446744073709545888)))))));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE58]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         let %[[VALUE59:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<i128>(call<i128, signature=fn(d128) -> i128>(%[[VALUE_tests128]], const<d128>(1701411834604692317316873037158842.0e+5)), reinterpret<i128, reason=explicit, fits=unknown>(sub<u128, overflow=wrap>(shl<u128, overflow=wrap, amount_out_of_range=ub>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(1))), const<i32>(127)), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(1))))))
// DEFAULT-NEXT:             write<bool>(%[[VALUE59]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE59]], ne<i128>(call<i128, signature=fn(d128) -> i128>(%[[VALUE_tests128]], const<d128>(9999999999999999999999999999999999e+6111)), reinterpret<i128, reason=explicit, fits=unknown>(sub<u128, overflow=wrap>(shl<u128, overflow=wrap, amount_out_of_range=ub>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(1))), const<i32>(127)), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(1)))))));
// DEFAULT-NEXT:         let %[[VALUE60:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE59]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE60]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE60]], ne<i128>(call<i128, signature=fn(d128) -> i128>(%[[VALUE_tests128]], neg<d128>(const<d128>(1701411834604692317316873037158842.0e+5))), sub<i128, overflow=ub>(neg<i128, overflow=ub>(reinterpret<i128, reason=explicit, fits=unknown>(sub<u128, overflow=wrap>(shl<u128, overflow=wrap, amount_out_of_range=ub>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(1))), const<i32>(127)), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(1)))))), widen<i128, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         let %[[VALUE61:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE60]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE61]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE61]], ne<i128>(call<i128, signature=fn(d128) -> i128>(%[[VALUE_tests128]], neg<d128>(const<d128>(9999999999999999999999999999999999e+6111))), sub<i128, overflow=ub>(neg<i128, overflow=ub>(reinterpret<i128, reason=explicit, fits=unknown>(sub<u128, overflow=wrap>(shl<u128, overflow=wrap, amount_out_of_range=ub>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(1))), const<i32>(127)), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(1)))))), widen<i128, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE61]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         let %[[VALUE62:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<u128>(call<u128, signature=fn(d128) -> u128>(%[[VALUE_testu128]], const<d128>(0.)), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%[[VALUE62]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE62]], ne<u128>(call<u128, signature=fn(d128) -> u128>(%[[VALUE_testu128]], const<d128>(0.9999999999999999999999999999999999)), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(0)))));
// DEFAULT-NEXT:         let %[[VALUE63:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE62]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE63]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE63]], ne<u128>(call<u128, signature=fn(d128) -> u128>(%[[VALUE_testu128]], neg<d128>(const<d128>(0.9999999999999999999999999999999999))), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(0)))));
// DEFAULT-NEXT:         let %[[VALUE64:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE63]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE64]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE64]], ne<u128>(call<u128, signature=fn(d128) -> u128>(%[[VALUE_testu128]], neg<d128>(const<d128>(0.))), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(0)))));
// DEFAULT-NEXT:         let %[[VALUE65:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE64]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE65]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE65]], ne<u128>(call<u128, signature=fn(d128) -> u128>(%[[VALUE_testu128]], neg<d128>(const<d128>(0.9999999999999999999999))), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(0)))));
// DEFAULT-NEXT:         let %[[VALUE66:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE65]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE66]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE66]], ne<u128>(call<u128, signature=fn(d128) -> u128>(%[[VALUE_testu128]], neg<d128>(const<d128>(0.5))), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(0)))));
// DEFAULT-NEXT:         let %[[VALUE67:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE66]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE67]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE67]], ne<u128>(call<u128, signature=fn(d128) -> u128>(%[[VALUE_testu128]], const<d128>(42.99999999999999999999999999999999)), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(42)))));
// DEFAULT-NEXT:         let %[[VALUE68:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE67]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE68]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE68]], ne<u128>(call<u128, signature=fn(d128) -> u128>(%[[VALUE_testu128]], const<d128>(42.e+21)), or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(2276)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(15210488237060521984)))));
// DEFAULT-NEXT:         let %[[VALUE69:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE68]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE69]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE69]], ne<u128>(call<u128, signature=fn(d128) -> u128>(%[[VALUE_testu128]], const<d128>(34242319854.45429439857871298745432e+21)), or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(1856279878857)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(2645246226444404542)))));
// DEFAULT-NEXT:         let %[[VALUE70:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE69]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE70]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE70]], ne<u128>(call<u128, signature=fn(d128) -> u128>(%[[VALUE_testu128]], const<d128>(3402823669209384634633746074317.682e+8)), or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(18446744073709551615)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(18446744073709540160)))));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE70]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         let %[[VALUE71:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<u128>(call<u128, signature=fn(d128) -> u128>(%[[VALUE_testu128]], neg<d128>(const<d128>(1.))), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%[[VALUE71]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE71]], ne<u128>(call<u128, signature=fn(d128) -> u128>(%[[VALUE_testu128]], neg<d128>(const<d128>(42.5e+15))), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(0)))));
// DEFAULT-NEXT:         let %[[VALUE72:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE71]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE72]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE72]], ne<u128>(call<u128, signature=fn(d128) -> u128>(%[[VALUE_testu128]], neg<d128>(const<d128>(9999999999999999999999999999999999e+6111))), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(0)))));
// DEFAULT-NEXT:         let %[[VALUE73:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE72]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE73]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE73]], ne<u128>(call<u128, signature=fn(d128) -> u128>(%[[VALUE_testu128]], const<d128>(3402823669209384634633746074317.683e+8)), not<u128>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(0))))));
// DEFAULT-NEXT:         let %[[VALUE74:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE73]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE74]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE74]], ne<u128>(call<u128, signature=fn(d128) -> u128>(%[[VALUE_testu128]], const<d128>(9999999999999999999999999999999999e+6111)), not<u128>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(0))))));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE74]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
