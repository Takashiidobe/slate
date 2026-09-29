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
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_x:[0-9]+]] x: i128) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_v:[0-9]+]] v: i128 [storage=automatic] = and<i128>(read<i128>(%[[VALUE_x]]), or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i128, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))), const<i32>(63)), widen<i128, reason=usual_arith>(const<i32>(2047))));
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_or<bool>(eq<i128>(read<i128>(%[[VALUE_v]]), widen<i128, reason=usual_arith>(const<i32>(0))), eq<i128>(read<i128>(%[[VALUE_v]]), shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i128, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))), const<i32>(63)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_x_2:[0-9]+]] x: i64) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_v_2:[0-9]+]] v: i64 [storage=automatic] = and<i64>(read<i64>(%[[VALUE_x_2]]), or<i64>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))), const<i32>(31)), widen<i64, reason=usual_arith>(const<i32>(2047))));
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_or<bool>(eq<i64>(read<i64>(%[[VALUE_v_2]]), widen<i64, reason=usual_arith>(const<i32>(0))), eq<i64>(read<i64>(%[[VALUE_v_2]]), shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i64, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))), const<i32>(31)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i128) -> i32>(%[[VALUE_foo]], widen<i128, reason=arg>(const<i32>(0))), const<i32>(1))
// DEFAULT-NEXT:             write<bool>(%[[VALUE0]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE0]], ne<i32>(call<i32, signature=fn(i128) -> i32>(%[[VALUE_foo]], widen<i128, reason=arg>(const<i32>(1))), const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE0]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE1]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE1]], ne<i32>(call<i32, signature=fn(i128) -> i32>(%[[VALUE_foo]], widen<i128, reason=arg>(const<i32>(2048))), const<i32>(1)));
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE1]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE2]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE2]], ne<i32>(call<i32, signature=fn(i128) -> i32>(%[[VALUE_foo]], widen<i128, reason=arg>(const<i32>(2049))), const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE2]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE3]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE3]], ne<i32>(call<i32, signature=fn(i128) -> i32>(%[[VALUE_foo]], shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i128, reason=explicit>(const<i32>(1)), const<i32>(63))), const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE3]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE4]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE4]], ne<i32>(call<i32, signature=fn(i128) -> i32>(%[[VALUE_foo]], shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i128, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))), const<i32>(63))), const<i32>(1)));
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE4]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE5]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE5]], ne<i32>(call<i32, signature=fn(i128) -> i32>(%[[VALUE_foo]], or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i128, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))), const<i32>(63)), widen<i128, reason=usual_arith>(const<i32>(1)))), const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE5]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE6]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE6]], ne<i32>(call<i32, signature=fn(i128) -> i32>(%[[VALUE_foo]], or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i128, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))), const<i32>(63)), widen<i128, reason=usual_arith>(const<i32>(2048)))), const<i32>(1)));
// DEFAULT-NEXT:         let %[[VALUE7:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE6]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE7]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE7]], ne<i32>(call<i32, signature=fn(i128) -> i32>(%[[VALUE_foo]], or<i128>(shl<i128, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i128, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1))), const<i32>(63)), widen<i128, reason=usual_arith>(const<i32>(2049)))), const<i32>(0)));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE7]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         let %[[VALUE8:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i64) -> i32>(%[[VALUE_bar]], widen<i64, reason=arg>(const<i32>(0))), const<i32>(1))
// DEFAULT-NEXT:             write<bool>(%[[VALUE8]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE8]], ne<i32>(call<i32, signature=fn(i64) -> i32>(%[[VALUE_bar]], widen<i64, reason=arg>(const<i32>(1))), const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE9:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE8]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE9]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE9]], ne<i32>(call<i32, signature=fn(i64) -> i32>(%[[VALUE_bar]], widen<i64, reason=arg>(const<i32>(2048))), const<i32>(1)));
// DEFAULT-NEXT:         let %[[VALUE10:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE9]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE10]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE10]], ne<i32>(call<i32, signature=fn(i64) -> i32>(%[[VALUE_bar]], widen<i64, reason=arg>(const<i32>(2049))), const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE11:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE10]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE11]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE11]], ne<i32>(call<i32, signature=fn(i64) -> i32>(%[[VALUE_bar]], shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i64>(1), const<i32>(31))), const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE12:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE11]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE12]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE12]], ne<i32>(call<i32, signature=fn(i64) -> i32>(%[[VALUE_bar]], shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(neg<i64, overflow=ub>(const<i64>(1)), const<i32>(31))), const<i32>(1)));
// DEFAULT-NEXT:         let %[[VALUE13:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE12]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE13]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE13]], ne<i32>(call<i32, signature=fn(i64) -> i32>(%[[VALUE_bar]], or<i64>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(neg<i64, overflow=ub>(const<i64>(1)), const<i32>(31)), widen<i64, reason=usual_arith>(const<i32>(1)))), const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE14:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE13]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE14]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE14]], ne<i32>(call<i32, signature=fn(i64) -> i32>(%[[VALUE_bar]], or<i64>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(neg<i64, overflow=ub>(const<i64>(1)), const<i32>(31)), widen<i64, reason=usual_arith>(const<i32>(2048)))), const<i32>(1)));
// DEFAULT-NEXT:         let %[[VALUE15:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE14]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE15]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE15]], ne<i32>(call<i32, signature=fn(i64) -> i32>(%[[VALUE_bar]], or<i64>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(neg<i64, overflow=ub>(const<i64>(1)), const<i32>(31)), widen<i64, reason=usual_arith>(const<i32>(2049)))), const<i32>(0)));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE15]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
