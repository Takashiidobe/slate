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

// SLATE-FILECHECK-FLAVOR gcc
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
// DEFAULT-NEXT:     fn %0 @tests64(%1 d: d64) -> i128 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return float_to_int<i128, reason=return, out_of_range=ub, exceptions=observable>(read<d64>(%1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %2 @testu64(%3 d: d64) -> u128 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return float_to_int<u128, reason=return, out_of_range=ub, exceptions=observable>(read<d64>(%3));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @tests32(%5 d: d32) -> i128 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return float_to_int<i128, reason=return, out_of_range=ub, exceptions=observable>(read<d32>(%5));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @testu32(%7 d: d32) -> u128 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return float_to_int<u128, reason=return, out_of_range=ub, exceptions=observable>(read<d32>(%7));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @tests128(%9 d: d128) -> i128 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return float_to_int<i128, reason=return, out_of_range=ub, exceptions=observable>(read<d128>(%9));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @testu128(%11 d: d128) -> u128 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return float_to_int<u128, reason=return, out_of_range=ub, exceptions=observable>(read<d128>(%11));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %12 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %14: bool [synthetic];
// DEFAULT-NEXT:         if ne<i128>(call<i128, signature=fn(d64) -> i128>(%0, const<d64>(0.)), widen<i128, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             write<bool>(%14, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%14, ne<i128>(call<i128, signature=fn(d64) -> i128>(%0, const<d64>(0.9999999999999999)), widen<i128, reason=usual_arith>(const<i32>(0))));
// DEFAULT-NEXT:         let %15: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%14)
// DEFAULT-NEXT:             write<bool>(%15, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%15, ne<i128>(call<i128, signature=fn(d64) -> i128>(%0, const<d64>(7.999999999999999)), widen<i128, reason=usual_arith>(const<i32>(7))));
// DEFAULT-NEXT:         let %16: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%15)
// DEFAULT-NEXT:             write<bool>(%16, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%16, ne<i128>(call<i128, signature=fn(d64) -> i128>(%0, neg<d64>(const<d64>(0.))), widen<i128, reason=usual_arith>(const<i32>(0))));
// DEFAULT-NEXT:         let %17: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%16)
// DEFAULT-NEXT:             write<bool>(%17, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%17, ne<i128>(call<i128, signature=fn(d64) -> i128>(%0, neg<d64>(const<d64>(0.9999999999999999))), widen<i128, reason=usual_arith>(const<i32>(0))));
// DEFAULT-NEXT:         let %18: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%17)
// DEFAULT-NEXT:             write<bool>(%18, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%18, ne<i128>(call<i128, signature=fn(d64) -> i128>(%0, neg<d64>(const<d64>(42.5))), widen<i128, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(42)))));
// DEFAULT-NEXT:         let %19: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%18)
// DEFAULT-NEXT:             write<bool>(%19, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%19, ne<i128>(call<i128, signature=fn(d64) -> i128>(%0, neg<d64>(const<d64>(34242319854.45429e+27))), neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(1856279878857143160))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(11766139157678653440)))))));
// DEFAULT-NEXT:         let %20: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%19)
// DEFAULT-NEXT:             write<bool>(%20, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%20, ne<i128>(call<i128, signature=fn(d64) -> i128>(%0, neg<d64>(const<d64>(213855087769445.9e+23))), neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(1159310753783557366))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(15090707038725996544)))))));
// DEFAULT-NEXT:         let %21: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%20)
// DEFAULT-NEXT:             write<bool>(%21, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%21, ne<i128>(call<i128, signature=fn(d64) -> i128>(%0, const<d64>(1701411834604692.0e+23)), or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(9223372036854774087))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(15159247138254225408))))));
// DEFAULT-NEXT:         let %22: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%21)
// DEFAULT-NEXT:             write<bool>(%22, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%22, ne<i128>(call<i128, signature=fn(d64) -> i128>(%0, neg<d64>(const<d64>(1701411834604692.0e+23))), neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(9223372036854774087))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(15159247138254225408)))))));
// DEFAULT-NEXT:         if read<bool>(%22)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%13);
// DEFAULT-NEXT:         let %23: bool [synthetic];
// DEFAULT-NEXT:         if ne<i128>(call<i128, signature=fn(d64) -> i128>(%0, const<d64>(1701411834604693.0e+23)), reinterpret<i128, reason=explicit, fits=unknown>(sub<u128, overflow=wrap>(shl<u128, overflow=wrap, amount_out_of_range=ub>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(1))), const<i32>(127)), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(1))))))
// DEFAULT-NEXT:             write<bool>(%23, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%23, ne<i128>(call<i128, signature=fn(d64) -> i128>(%0, const<d64>(9999999999999999e+369)), reinterpret<i128, reason=explicit, fits=unknown>(sub<u128, overflow=wrap>(shl<u128, overflow=wrap, amount_out_of_range=ub>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(1))), const<i32>(127)), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(1)))))));
// DEFAULT-NEXT:         let %24: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%23)
// DEFAULT-NEXT:             write<bool>(%24, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%24, ne<i128>(call<i128, signature=fn(d64) -> i128>(%0, neg<d64>(const<d64>(1701411834604693.0e+23))), sub<i128, overflow=ub>(neg<i128, overflow=ub>(reinterpret<i128, reason=explicit, fits=unknown>(sub<u128, overflow=wrap>(shl<u128, overflow=wrap, amount_out_of_range=ub>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(1))), const<i32>(127)), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(1)))))), widen<i128, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         let %25: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%24)
// DEFAULT-NEXT:             write<bool>(%25, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%25, ne<i128>(call<i128, signature=fn(d64) -> i128>(%0, neg<d64>(const<d64>(9999999999999999e+369))), sub<i128, overflow=ub>(neg<i128, overflow=ub>(reinterpret<i128, reason=explicit, fits=unknown>(sub<u128, overflow=wrap>(shl<u128, overflow=wrap, amount_out_of_range=ub>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(1))), const<i32>(127)), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(1)))))), widen<i128, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         if read<bool>(%25)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%13);
// DEFAULT-NEXT:         let %26: bool [synthetic];
// DEFAULT-NEXT:         if ne<u128>(call<u128, signature=fn(d64) -> u128>(%2, const<d64>(0.)), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%26, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%26, ne<u128>(call<u128, signature=fn(d64) -> u128>(%2, const<d64>(0.9999999999999999)), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(0)))));
// DEFAULT-NEXT:         let %27: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%26)
// DEFAULT-NEXT:             write<bool>(%27, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%27, ne<u128>(call<u128, signature=fn(d64) -> u128>(%2, neg<d64>(const<d64>(0.9999999999999999))), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(0)))));
// DEFAULT-NEXT:         let %28: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%27)
// DEFAULT-NEXT:             write<bool>(%28, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%28, ne<u128>(call<u128, signature=fn(d64) -> u128>(%2, neg<d64>(const<d64>(0.0))), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(0)))));
// DEFAULT-NEXT:         let %29: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%28)
// DEFAULT-NEXT:             write<bool>(%29, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%29, ne<u128>(call<u128, signature=fn(d64) -> u128>(%2, neg<d64>(const<d64>(0.5))), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(0)))));
// DEFAULT-NEXT:         let %30: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%29)
// DEFAULT-NEXT:             write<bool>(%30, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%30, ne<u128>(call<u128, signature=fn(d64) -> u128>(%2, const<d64>(42.99999999999999)), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(42)))));
// DEFAULT-NEXT:         let %31: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%30)
// DEFAULT-NEXT:             write<bool>(%31, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%31, ne<u128>(call<u128, signature=fn(d64) -> u128>(%2, const<d64>(42.e+21)), or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(2276)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(15210488237060521984)))));
// DEFAULT-NEXT:         let %32: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%31)
// DEFAULT-NEXT:             write<bool>(%32, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%32, ne<u128>(call<u128, signature=fn(d64) -> u128>(%2, const<d64>(34272319854.45429e+27)), reinterpret<u128, reason=usual_arith, fits=unknown>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(1857906182115871417))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(5328431695819440128)))))));
// DEFAULT-NEXT:         let %33: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%32)
// DEFAULT-NEXT:             write<bool>(%33, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%33, ne<u128>(call<u128, signature=fn(d64) -> u128>(%2, const<d64>(3402823669209384.0e+23)), reinterpret<u128, reason=usual_arith, fits=unknown>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(18446744073709548175))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(11871750202798899200)))))));
// DEFAULT-NEXT:         if read<bool>(%33)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%13);
// DEFAULT-NEXT:         let %34: bool [synthetic];
// DEFAULT-NEXT:         if ne<u128>(call<u128, signature=fn(d64) -> u128>(%2, neg<d64>(const<d64>(1.))), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%34, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%34, ne<u128>(call<u128, signature=fn(d64) -> u128>(%2, neg<d64>(const<d64>(42.5e+15))), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(0)))));
// DEFAULT-NEXT:         let %35: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%34)
// DEFAULT-NEXT:             write<bool>(%35, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%35, ne<u128>(call<u128, signature=fn(d64) -> u128>(%2, neg<d64>(const<d64>(9999999999999999e+369))), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(0)))));
// DEFAULT-NEXT:         let %36: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%35)
// DEFAULT-NEXT:             write<bool>(%36, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%36, ne<u128>(call<u128, signature=fn(d64) -> u128>(%2, const<d64>(3402823669209385.0e+23)), not<u128>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(0))))));
// DEFAULT-NEXT:         let %37: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%36)
// DEFAULT-NEXT:             write<bool>(%37, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%37, ne<u128>(call<u128, signature=fn(d64) -> u128>(%2, const<d64>(9999999999999999e+369)), not<u128>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(0))))));
// DEFAULT-NEXT:         if read<bool>(%37)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%13);
// DEFAULT-NEXT:         let %38: bool [synthetic];
// DEFAULT-NEXT:         if ne<i128>(call<i128, signature=fn(d32) -> i128>(%4, const<d32>(0.)), widen<i128, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             write<bool>(%38, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%38, ne<i128>(call<i128, signature=fn(d32) -> i128>(%4, const<d32>(0.9999999)), widen<i128, reason=usual_arith>(const<i32>(0))));
// DEFAULT-NEXT:         let %39: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%38)
// DEFAULT-NEXT:             write<bool>(%39, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%39, ne<i128>(call<i128, signature=fn(d32) -> i128>(%4, const<d32>(7.999999)), widen<i128, reason=usual_arith>(const<i32>(7))));
// DEFAULT-NEXT:         let %40: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%39)
// DEFAULT-NEXT:             write<bool>(%40, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%40, ne<i128>(call<i128, signature=fn(d32) -> i128>(%4, neg<d32>(const<d32>(0.000))), widen<i128, reason=usual_arith>(const<i32>(0))));
// DEFAULT-NEXT:         let %41: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%40)
// DEFAULT-NEXT:             write<bool>(%41, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%41, ne<i128>(call<i128, signature=fn(d32) -> i128>(%4, neg<d32>(const<d32>(0.9999999))), widen<i128, reason=usual_arith>(const<i32>(0))));
// DEFAULT-NEXT:         let %42: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%41)
// DEFAULT-NEXT:             write<bool>(%42, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%42, ne<i128>(call<i128, signature=fn(d32) -> i128>(%4, neg<d32>(const<d32>(1.))), widen<i128, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))));
// DEFAULT-NEXT:         let %43: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%42)
// DEFAULT-NEXT:             write<bool>(%43, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%43, ne<i128>(call<i128, signature=fn(d32) -> i128>(%4, neg<d32>(const<d32>(42.5))), widen<i128, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(42)))));
// DEFAULT-NEXT:         let %44: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%43)
// DEFAULT-NEXT:             write<bool>(%44, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%44, ne<i128>(call<i128, signature=fn(d32) -> i128>(%4, neg<d32>(const<d32>(3424.231e+27))), neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(185627934464))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(11262967254326706176)))))));
// DEFAULT-NEXT:         let %45: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%44)
// DEFAULT-NEXT:             write<bool>(%45, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%45, ne<i128>(call<i128, signature=fn(d32) -> i128>(%4, neg<d32>(const<d32>(213855.9e+32))), neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(1159315156894213938))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(8176927485242376192)))))));
// DEFAULT-NEXT:         let %46: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%45)
// DEFAULT-NEXT:             write<bool>(%46, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%46, ne<i128>(call<i128, signature=fn(d32) -> i128>(%4, const<d32>(1701411.0e+32)), or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(9223367512453672922))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(15592504947059458048))))));
// DEFAULT-NEXT:         let %47: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%46)
// DEFAULT-NEXT:             write<bool>(%47, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%47, ne<i128>(call<i128, signature=fn(d32) -> i128>(%4, neg<d32>(const<d32>(1701411.0e+32))), neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(9223367512453672922))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(15592504947059458048)))))));
// DEFAULT-NEXT:         if read<bool>(%47)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%13);
// DEFAULT-NEXT:         let %48: bool [synthetic];
// DEFAULT-NEXT:         if ne<i128>(call<i128, signature=fn(d32) -> i128>(%4, const<d32>(1701412.0e+32)), reinterpret<i128, reason=explicit, fits=unknown>(sub<u128, overflow=wrap>(shl<u128, overflow=wrap, amount_out_of_range=ub>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(1))), const<i32>(127)), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(1))))))
// DEFAULT-NEXT:             write<bool>(%48, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%48, ne<i128>(call<i128, signature=fn(d32) -> i128>(%4, const<d32>(9999999e+90)), reinterpret<i128, reason=explicit, fits=unknown>(sub<u128, overflow=wrap>(shl<u128, overflow=wrap, amount_out_of_range=ub>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(1))), const<i32>(127)), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(1)))))));
// DEFAULT-NEXT:         let %49: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%48)
// DEFAULT-NEXT:             write<bool>(%49, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%49, ne<i128>(call<i128, signature=fn(d32) -> i128>(%4, neg<d32>(const<d32>(1701412.0e+32))), sub<i128, overflow=ub>(neg<i128, overflow=ub>(reinterpret<i128, reason=explicit, fits=unknown>(sub<u128, overflow=wrap>(shl<u128, overflow=wrap, amount_out_of_range=ub>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(1))), const<i32>(127)), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(1)))))), widen<i128, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         let %50: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%49)
// DEFAULT-NEXT:             write<bool>(%50, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%50, ne<i128>(call<i128, signature=fn(d32) -> i128>(%4, neg<d32>(const<d32>(9999999e+90))), sub<i128, overflow=ub>(neg<i128, overflow=ub>(reinterpret<i128, reason=explicit, fits=unknown>(sub<u128, overflow=wrap>(shl<u128, overflow=wrap, amount_out_of_range=ub>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(1))), const<i32>(127)), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(1)))))), widen<i128, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         if read<bool>(%50)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%13);
// DEFAULT-NEXT:         let %51: bool [synthetic];
// DEFAULT-NEXT:         if ne<u128>(call<u128, signature=fn(d32) -> u128>(%6, const<d32>(0.)), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%51, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%51, ne<u128>(call<u128, signature=fn(d32) -> u128>(%6, const<d32>(0.9999999)), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(0)))));
// DEFAULT-NEXT:         let %52: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%51)
// DEFAULT-NEXT:             write<bool>(%52, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%52, ne<u128>(call<u128, signature=fn(d32) -> u128>(%6, neg<d32>(const<d32>(0.9999999))), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(0)))));
// DEFAULT-NEXT:         let %53: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%52)
// DEFAULT-NEXT:             write<bool>(%53, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%53, ne<u128>(call<u128, signature=fn(d32) -> u128>(%6, neg<d32>(const<d32>(0.5))), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(0)))));
// DEFAULT-NEXT:         let %54: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%53)
// DEFAULT-NEXT:             write<bool>(%54, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%54, ne<u128>(call<u128, signature=fn(d32) -> u128>(%6, neg<d32>(const<d32>(0.0000))), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(0)))));
// DEFAULT-NEXT:         let %55: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%54)
// DEFAULT-NEXT:             write<bool>(%55, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%55, ne<u128>(call<u128, signature=fn(d32) -> u128>(%6, neg<d32>(const<d32>(0.99999))), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(0)))));
// DEFAULT-NEXT:         let %56: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%55)
// DEFAULT-NEXT:             write<bool>(%56, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%56, ne<u128>(call<u128, signature=fn(d32) -> u128>(%6, const<d32>(42.99999)), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(42)))));
// DEFAULT-NEXT:         let %57: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%56)
// DEFAULT-NEXT:             write<bool>(%57, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%57, ne<u128>(call<u128, signature=fn(d32) -> u128>(%6, const<d32>(42.e+21)), or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(2276)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(15210488237060521984)))));
// DEFAULT-NEXT:         let %58: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%57)
// DEFAULT-NEXT:             write<bool>(%58, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%58, ne<u128>(call<u128, signature=fn(d32) -> u128>(%6, const<d32>(3402.823e+35)), or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(18446740445918208273)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(3923858787068280832)))));
// DEFAULT-NEXT:         if read<bool>(%58)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%13);
// DEFAULT-NEXT:         let %59: bool [synthetic];
// DEFAULT-NEXT:         if ne<u128>(call<u128, signature=fn(d32) -> u128>(%6, neg<d32>(const<d32>(1.))), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%59, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%59, ne<u128>(call<u128, signature=fn(d32) -> u128>(%6, neg<d32>(const<d32>(42.5e+15))), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(0)))));
// DEFAULT-NEXT:         let %60: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%59)
// DEFAULT-NEXT:             write<bool>(%60, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%60, ne<u128>(call<u128, signature=fn(d32) -> u128>(%6, neg<d32>(const<d32>(9999999e+90))), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(0)))));
// DEFAULT-NEXT:         let %61: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%60)
// DEFAULT-NEXT:             write<bool>(%61, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%61, ne<u128>(call<u128, signature=fn(d32) -> u128>(%6, const<d32>(3402.824e+35)), not<u128>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(0))))));
// DEFAULT-NEXT:         let %62: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%61)
// DEFAULT-NEXT:             write<bool>(%62, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%62, ne<u128>(call<u128, signature=fn(d32) -> u128>(%6, const<d32>(9999999e+90)), not<u128>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(0))))));
// DEFAULT-NEXT:         if read<bool>(%62)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%13);
// DEFAULT-NEXT:         let %63: bool [synthetic];
// DEFAULT-NEXT:         if ne<i128>(call<i128, signature=fn(d128) -> i128>(%8, const<d128>(0.)), widen<i128, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             write<bool>(%63, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%63, ne<i128>(call<i128, signature=fn(d128) -> i128>(%8, const<d128>(0.9999999999999999999999999999999999)), widen<i128, reason=usual_arith>(const<i32>(0))));
// DEFAULT-NEXT:         let %64: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%63)
// DEFAULT-NEXT:             write<bool>(%64, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%64, ne<i128>(call<i128, signature=fn(d128) -> i128>(%8, const<d128>(7.999999999999999999999999999999999)), widen<i128, reason=usual_arith>(const<i32>(7))));
// DEFAULT-NEXT:         let %65: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%64)
// DEFAULT-NEXT:             write<bool>(%65, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%65, ne<i128>(call<i128, signature=fn(d128) -> i128>(%8, neg<d128>(const<d128>(0.))), widen<i128, reason=usual_arith>(const<i32>(0))));
// DEFAULT-NEXT:         let %66: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%65)
// DEFAULT-NEXT:             write<bool>(%66, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%66, ne<i128>(call<i128, signature=fn(d128) -> i128>(%8, neg<d128>(const<d128>(0.9999999999999999999999999999999999))), widen<i128, reason=usual_arith>(const<i32>(0))));
// DEFAULT-NEXT:         let %67: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%66)
// DEFAULT-NEXT:             write<bool>(%67, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%67, ne<i128>(call<i128, signature=fn(d128) -> i128>(%8, neg<d128>(const<d128>(1.))), widen<i128, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1)))));
// DEFAULT-NEXT:         let %68: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%67)
// DEFAULT-NEXT:             write<bool>(%68, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%68, ne<i128>(call<i128, signature=fn(d128) -> i128>(%8, neg<d128>(const<d128>(42.5))), widen<i128, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(42)))));
// DEFAULT-NEXT:         let %69: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%68)
// DEFAULT-NEXT:             write<bool>(%69, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%69, ne<i128>(call<i128, signature=fn(d128) -> i128>(%8, neg<d128>(const<d128>(34242319854.45429439857871298745432e+27))), neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(1856279878857143399))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(1573018528550137216)))))));
// DEFAULT-NEXT:         let %70: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%69)
// DEFAULT-NEXT:             write<bool>(%70, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%70, ne<i128>(call<i128, signature=fn(d128) -> i128>(%8, neg<d128>(const<d128>(213855087769445.9e+23))), neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(1159310753783557366))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(15090707038725996544)))))));
// DEFAULT-NEXT:         let %71: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%70)
// DEFAULT-NEXT:             write<bool>(%71, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%71, ne<i128>(call<i128, signature=fn(d128) -> i128>(%8, const<d128>(1701411834604692317316873037158841.0e+5)), or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(9223372036854775807))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(18446744073709545888))))));
// DEFAULT-NEXT:         let %72: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%71)
// DEFAULT-NEXT:             write<bool>(%72, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%72, ne<i128>(call<i128, signature=fn(d128) -> i128>(%8, neg<d128>(const<d128>(1701411834604692317316873037158841.0e+5))), neg<i128, overflow=ub>(or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i128, reason=explicit, fits=unknown>(widen<u128, reason=explicit>(const<u64>(9223372036854775807))), const<i32>(64)), reinterpret<i128, reason=usual_arith, fits=unknown>(widen<u128, reason=usual_arith>(const<u64>(18446744073709545888)))))));
// DEFAULT-NEXT:         if read<bool>(%72)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%13);
// DEFAULT-NEXT:         let %73: bool [synthetic];
// DEFAULT-NEXT:         if ne<i128>(call<i128, signature=fn(d128) -> i128>(%8, const<d128>(1701411834604692317316873037158842.0e+5)), reinterpret<i128, reason=explicit, fits=unknown>(sub<u128, overflow=wrap>(shl<u128, overflow=wrap, amount_out_of_range=ub>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(1))), const<i32>(127)), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(1))))))
// DEFAULT-NEXT:             write<bool>(%73, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%73, ne<i128>(call<i128, signature=fn(d128) -> i128>(%8, const<d128>(9999999999999999999999999999999999e+6111)), reinterpret<i128, reason=explicit, fits=unknown>(sub<u128, overflow=wrap>(shl<u128, overflow=wrap, amount_out_of_range=ub>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(1))), const<i32>(127)), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(1)))))));
// DEFAULT-NEXT:         let %74: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%73)
// DEFAULT-NEXT:             write<bool>(%74, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%74, ne<i128>(call<i128, signature=fn(d128) -> i128>(%8, neg<d128>(const<d128>(1701411834604692317316873037158842.0e+5))), sub<i128, overflow=ub>(neg<i128, overflow=ub>(reinterpret<i128, reason=explicit, fits=unknown>(sub<u128, overflow=wrap>(shl<u128, overflow=wrap, amount_out_of_range=ub>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(1))), const<i32>(127)), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(1)))))), widen<i128, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         let %75: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%74)
// DEFAULT-NEXT:             write<bool>(%75, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%75, ne<i128>(call<i128, signature=fn(d128) -> i128>(%8, neg<d128>(const<d128>(9999999999999999999999999999999999e+6111))), sub<i128, overflow=ub>(neg<i128, overflow=ub>(reinterpret<i128, reason=explicit, fits=unknown>(sub<u128, overflow=wrap>(shl<u128, overflow=wrap, amount_out_of_range=ub>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(1))), const<i32>(127)), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(1)))))), widen<i128, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         if read<bool>(%75)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%13);
// DEFAULT-NEXT:         let %76: bool [synthetic];
// DEFAULT-NEXT:         if ne<u128>(call<u128, signature=fn(d128) -> u128>(%10, const<d128>(0.)), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%76, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%76, ne<u128>(call<u128, signature=fn(d128) -> u128>(%10, const<d128>(0.9999999999999999999999999999999999)), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(0)))));
// DEFAULT-NEXT:         let %77: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%76)
// DEFAULT-NEXT:             write<bool>(%77, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%77, ne<u128>(call<u128, signature=fn(d128) -> u128>(%10, neg<d128>(const<d128>(0.9999999999999999999999999999999999))), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(0)))));
// DEFAULT-NEXT:         let %78: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%77)
// DEFAULT-NEXT:             write<bool>(%78, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%78, ne<u128>(call<u128, signature=fn(d128) -> u128>(%10, neg<d128>(const<d128>(0.))), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(0)))));
// DEFAULT-NEXT:         let %79: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%78)
// DEFAULT-NEXT:             write<bool>(%79, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%79, ne<u128>(call<u128, signature=fn(d128) -> u128>(%10, neg<d128>(const<d128>(0.9999999999999999999999))), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(0)))));
// DEFAULT-NEXT:         let %80: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%79)
// DEFAULT-NEXT:             write<bool>(%80, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%80, ne<u128>(call<u128, signature=fn(d128) -> u128>(%10, neg<d128>(const<d128>(0.5))), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(0)))));
// DEFAULT-NEXT:         let %81: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%80)
// DEFAULT-NEXT:             write<bool>(%81, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%81, ne<u128>(call<u128, signature=fn(d128) -> u128>(%10, const<d128>(42.99999999999999999999999999999999)), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(42)))));
// DEFAULT-NEXT:         let %82: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%81)
// DEFAULT-NEXT:             write<bool>(%82, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%82, ne<u128>(call<u128, signature=fn(d128) -> u128>(%10, const<d128>(42.e+21)), or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(2276)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(15210488237060521984)))));
// DEFAULT-NEXT:         let %83: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%82)
// DEFAULT-NEXT:             write<bool>(%83, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%83, ne<u128>(call<u128, signature=fn(d128) -> u128>(%10, const<d128>(34242319854.45429439857871298745432e+21)), or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(1856279878857)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(2645246226444404542)))));
// DEFAULT-NEXT:         let %84: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%83)
// DEFAULT-NEXT:             write<bool>(%84, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%84, ne<u128>(call<u128, signature=fn(d128) -> u128>(%10, const<d128>(3402823669209384634633746074317.682e+8)), or<u128>(shl<u128, overflow=wrap, amount_out_of_range=ub>(widen<u128, reason=explicit>(const<u64>(18446744073709551615)), const<i32>(64)), widen<u128, reason=usual_arith>(const<u64>(18446744073709540160)))));
// DEFAULT-NEXT:         if read<bool>(%84)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%13);
// DEFAULT-NEXT:         let %85: bool [synthetic];
// DEFAULT-NEXT:         if ne<u128>(call<u128, signature=fn(d128) -> u128>(%10, neg<d128>(const<d128>(1.))), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%85, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%85, ne<u128>(call<u128, signature=fn(d128) -> u128>(%10, neg<d128>(const<d128>(42.5e+15))), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(0)))));
// DEFAULT-NEXT:         let %86: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%85)
// DEFAULT-NEXT:             write<bool>(%86, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%86, ne<u128>(call<u128, signature=fn(d128) -> u128>(%10, neg<d128>(const<d128>(9999999999999999999999999999999999e+6111))), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(const<i32>(0)))));
// DEFAULT-NEXT:         let %87: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%86)
// DEFAULT-NEXT:             write<bool>(%87, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%87, ne<u128>(call<u128, signature=fn(d128) -> u128>(%10, const<d128>(3402823669209384634633746074317.683e+8)), not<u128>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(0))))));
// DEFAULT-NEXT:         let %88: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%87)
// DEFAULT-NEXT:             write<bool>(%88, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%88, ne<u128>(call<u128, signature=fn(d128) -> u128>(%10, const<d128>(9999999999999999999999999999999999e+6111)), not<u128>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(0))))));
// DEFAULT-NEXT:         if read<bool>(%88)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%13);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
