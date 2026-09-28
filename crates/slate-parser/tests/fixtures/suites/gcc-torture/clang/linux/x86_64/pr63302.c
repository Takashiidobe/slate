/* PR tree-optimization/63302 */

#ifdef __SIZEOF_INT128__
#if __SIZEOF_INT128__ * __CHAR_BIT__ == 128
#define USE_INT128
#endif
#endif
#if __SIZEOF_LONG_LONG__ * __CHAR_BIT__ == 64
#define USE_LLONG
#endif

#ifdef USE_INT128
__attribute__((noinline, noclone)) int foo(__int128 x) {
  __int128 v = x & (((__int128)-1 << 63) | 0x7ff);

  return v == 0 || v == ((__int128)-1 << 63);
}
#endif

#ifdef USE_LLONG
__attribute__((noinline, noclone)) int bar(long long x) {
  long long v = x & (((long long)-1 << 31) | 0x7ff);

  return v == 0 || v == ((long long)-1 << 31);
}
#endif

int main() {
#ifdef USE_INT128
  if (foo(0) != 1 || foo(1) != 0 || foo(0x800) != 1 || foo(0x801) != 0 ||
      foo((__int128)1 << 63) != 0 || foo((__int128)-1 << 63) != 1 ||
      foo(((__int128)-1 << 63) | 1) != 0 ||
      foo(((__int128)-1 << 63) | 0x800) != 1 ||
      foo(((__int128)-1 << 63) | 0x801) != 0)
    __builtin_abort();
#endif
#ifdef USE_LLONG
  if (bar(0) != 1 || bar(1) != 0 || bar(0x800) != 1 || bar(0x801) != 0 ||
      bar(1LL << 31) != 0 || bar(-1LL << 31) != 1 ||
      bar((-1LL << 31) | 1) != 0 || bar((-1LL << 31) | 0x800) != 1 ||
      bar((-1LL << 31) | 0x801) != 0)
    __builtin_abort();
#endif
  return 0;
}


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
// DEFAULT-NEXT:     fn %0 @foo(%1 x: i128) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %2 v: i128 [storage=automatic] = and<i128>(read<i128>(%1), or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i128, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))), const<i32>(63)), widen<i128, reason=usual_arith>(const<i32>(2047))));
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_or<bool>(eq<i128>(read<i128>(%2), widen<i128, reason=usual_arith>(const<i32>(0))), eq<i128>(read<i128>(%2), shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i128, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))), const<i32>(63)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @bar(%4 x: i64) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %5 v: i64 [storage=automatic] = and<i64>(read<i64>(%4), or<i64>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))), const<i32>(31)), widen<i64, reason=usual_arith>(const<i32>(2047))));
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_or<bool>(eq<i64>(read<i64>(%5), widen<i64, reason=usual_arith>(const<i32>(0))), eq<i64>(read<i64>(%5), shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))), const<i32>(31)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %6 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %8: bool [synthetic];
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i128) -> i32>(%0, widen<i128, reason=arg>(const<i32>(0))), const<i32>(1))
// DEFAULT-NEXT:             write<bool>(%8, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%8, ne<i32>(call<i32, signature=fn(i128) -> i32>(%0, widen<i128, reason=arg>(const<i32>(1))), const<i32>(0)));
// DEFAULT-NEXT:         let %9: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%8)
// DEFAULT-NEXT:             write<bool>(%9, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%9, ne<i32>(call<i32, signature=fn(i128) -> i32>(%0, widen<i128, reason=arg>(const<i32>(2048))), const<i32>(1)));
// DEFAULT-NEXT:         let %10: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%9)
// DEFAULT-NEXT:             write<bool>(%10, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%10, ne<i32>(call<i32, signature=fn(i128) -> i32>(%0, widen<i128, reason=arg>(const<i32>(2049))), const<i32>(0)));
// DEFAULT-NEXT:         let %11: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%10)
// DEFAULT-NEXT:             write<bool>(%11, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%11, ne<i32>(call<i32, signature=fn(i128) -> i32>(%0, shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i128, reason=explicit>(const<i32>(1)), const<i32>(63))), const<i32>(0)));
// DEFAULT-NEXT:         let %12: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%11)
// DEFAULT-NEXT:             write<bool>(%12, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%12, ne<i32>(call<i32, signature=fn(i128) -> i32>(%0, shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i128, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))), const<i32>(63))), const<i32>(1)));
// DEFAULT-NEXT:         let %13: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%12)
// DEFAULT-NEXT:             write<bool>(%13, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%13, ne<i32>(call<i32, signature=fn(i128) -> i32>(%0, or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i128, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))), const<i32>(63)), widen<i128, reason=usual_arith>(const<i32>(1)))), const<i32>(0)));
// DEFAULT-NEXT:         let %14: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%13)
// DEFAULT-NEXT:             write<bool>(%14, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%14, ne<i32>(call<i32, signature=fn(i128) -> i32>(%0, or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i128, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))), const<i32>(63)), widen<i128, reason=usual_arith>(const<i32>(2048)))), const<i32>(1)));
// DEFAULT-NEXT:         let %15: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%14)
// DEFAULT-NEXT:             write<bool>(%15, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%15, ne<i32>(call<i32, signature=fn(i128) -> i32>(%0, or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i128, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))), const<i32>(63)), widen<i128, reason=usual_arith>(const<i32>(2049)))), const<i32>(0)));
// DEFAULT-NEXT:         if read<bool>(%15)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%7);
// DEFAULT-NEXT:         let %16: bool [synthetic];
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i64) -> i32>(%3, widen<i64, reason=arg>(const<i32>(0))), const<i32>(1))
// DEFAULT-NEXT:             write<bool>(%16, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%16, ne<i32>(call<i32, signature=fn(i64) -> i32>(%3, widen<i64, reason=arg>(const<i32>(1))), const<i32>(0)));
// DEFAULT-NEXT:         let %17: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%16)
// DEFAULT-NEXT:             write<bool>(%17, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%17, ne<i32>(call<i32, signature=fn(i64) -> i32>(%3, widen<i64, reason=arg>(const<i32>(2048))), const<i32>(1)));
// DEFAULT-NEXT:         let %18: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%17)
// DEFAULT-NEXT:             write<bool>(%18, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%18, ne<i32>(call<i32, signature=fn(i64) -> i32>(%3, widen<i64, reason=arg>(const<i32>(2049))), const<i32>(0)));
// DEFAULT-NEXT:         let %19: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%18)
// DEFAULT-NEXT:             write<bool>(%19, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%19, ne<i32>(call<i32, signature=fn(i64) -> i32>(%3, shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i64>(1), const<i32>(31))), const<i32>(0)));
// DEFAULT-NEXT:         let %20: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%19)
// DEFAULT-NEXT:             write<bool>(%20, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%20, ne<i32>(call<i32, signature=fn(i64) -> i32>(%3, shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(neg<i64, overflow=ub>(const<i64>(1)), const<i32>(31))), const<i32>(1)));
// DEFAULT-NEXT:         let %21: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%20)
// DEFAULT-NEXT:             write<bool>(%21, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%21, ne<i32>(call<i32, signature=fn(i64) -> i32>(%3, or<i64>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(neg<i64, overflow=ub>(const<i64>(1)), const<i32>(31)), widen<i64, reason=usual_arith>(const<i32>(1)))), const<i32>(0)));
// DEFAULT-NEXT:         let %22: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%21)
// DEFAULT-NEXT:             write<bool>(%22, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%22, ne<i32>(call<i32, signature=fn(i64) -> i32>(%3, or<i64>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(neg<i64, overflow=ub>(const<i64>(1)), const<i32>(31)), widen<i64, reason=usual_arith>(const<i32>(2048)))), const<i32>(1)));
// DEFAULT-NEXT:         let %23: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%22)
// DEFAULT-NEXT:             write<bool>(%23, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%23, ne<i32>(call<i32, signature=fn(i64) -> i32>(%3, or<i64>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(neg<i64, overflow=ub>(const<i64>(1)), const<i32>(31)), widen<i64, reason=usual_arith>(const<i32>(2049)))), const<i32>(0)));
// DEFAULT-NEXT:         if read<bool>(%23)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%7);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
