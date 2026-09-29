/* PR target/85582 */

#ifdef __SIZEOF_INT128__
typedef __int128          S;
typedef unsigned __int128 U;
#else
typedef long long          S;
typedef unsigned long long U;
#endif

__attribute__((noipa)) U f1(U x, int y) { return x << (y & -2); }

__attribute__((noipa)) S f2(S x, int y) { return x >> (y & -2); }

__attribute__((noipa)) U f3(U x, int y) { return x >> (y & -2); }

int main() {
  U a = (U)1 << (sizeof(U) * __CHAR_BIT__ - 7);
  if (f1(a, 5) != ((U)1 << (sizeof(S) * __CHAR_BIT__ - 3)))
    __builtin_abort();
  S b = (U)0x101 << (sizeof(S) * __CHAR_BIT__ / 2 - 7);
  if (f1(b, sizeof(S) * __CHAR_BIT__ / 2) !=
      (U)0x101 << (sizeof(S) * __CHAR_BIT__ - 7))
    __builtin_abort();
  if (f1(b, sizeof(S) * __CHAR_BIT__ / 2 + 2) !=
      (U)0x101 << (sizeof(S) * __CHAR_BIT__ - 5))
    __builtin_abort();
  S c = (U)1 << (sizeof(S) * __CHAR_BIT__ - 1);
  if ((U)f2(c, 5) != ((U)0x1f << (sizeof(S) * __CHAR_BIT__ - 5)))
    __builtin_abort();
  if ((U)f2(c, sizeof(S) * __CHAR_BIT__ / 2) !=
      ((U)-1 << (sizeof(S) * __CHAR_BIT__ / 2 - 1)))
    __builtin_abort();
  if ((U)f2(c, sizeof(S) * __CHAR_BIT__ / 2 + 2) !=
      ((U)-1 << (sizeof(S) * __CHAR_BIT__ / 2 - 3)))
    __builtin_abort();
  U d = (U)1 << (sizeof(S) * __CHAR_BIT__ - 1);
  if (f3(c, 5) != ((U)0x1 << (sizeof(S) * __CHAR_BIT__ - 5)))
    __builtin_abort();
  if (f3(c, sizeof(S) * __CHAR_BIT__ / 2) !=
      ((U)1 << (sizeof(S) * __CHAR_BIT__ / 2 - 1)))
    __builtin_abort();
  if (f3(c, sizeof(S) * __CHAR_BIT__ / 2 + 2) !=
      ((U)1 << (sizeof(S) * __CHAR_BIT__ / 2 - 3)))
    __builtin_abort();
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
// DEFAULT-NEXT:     type @type[[TYPE_S:[0-9]+]] S = i128;
// DEFAULT-NEXT:     type @type[[TYPE_U:[0-9]+]] U = u128;
// DEFAULT-NEXT:     fn %[[VALUE_f1:[0-9]+]] @f1(%[[VALUE_x:[0-9]+]] x: u128, %[[VALUE_y:[0-9]+]] y: i32) -> u128 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shl<u128, overflow=wrap, amount_out_of_range=ub>(read<u128>(%[[VALUE_x]]), and<i32>(read<i32>(%[[VALUE_y]]), neg<i32, overflow=ub>(const<i32>(2))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f2:[0-9]+]] @f2(%[[VALUE_x_2:[0-9]+]] x: i128, %[[VALUE_y_2:[0-9]+]] y: i32) -> i128 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<i128, amount_out_of_range=ub, fill=sign_extend>(read<i128>(%[[VALUE_x_2]]), and<i32>(read<i32>(%[[VALUE_y_2]]), neg<i32, overflow=ub>(const<i32>(2))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f3:[0-9]+]] @f3(%[[VALUE_x_3:[0-9]+]] x: u128, %[[VALUE_y_3:[0-9]+]] y: i32) -> u128 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return shr<u128, amount_out_of_range=ub, fill=zero_extend>(read<u128>(%[[VALUE_x_3]]), and<i32>(read<i32>(%[[VALUE_y_3]]), neg<i32, overflow=ub>(const<i32>(2))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: u128 [storage=automatic] = shl<u128, overflow=wrap, amount_out_of_range=ub>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(1))), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(16), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(7)))));
// DEFAULT-NEXT:         if ne<u128>(call<u128, signature=fn(u128, i32) -> u128>(%[[VALUE_f1]], read<u128>(%[[VALUE_a]]), const<i32>(5)), shl<u128, overflow=wrap, amount_out_of_range=ub>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(1))), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(16), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: i128 [storage=automatic] = reinterpret<i128, reason=assign, fits=unknown>(shl<u128, overflow=wrap, amount_out_of_range=ub>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(257))), sub<u64, overflow=wrap>(div<u64, by_zero=ub>(mul<u64, overflow=wrap>(const<u64>(16), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(7))))));
// DEFAULT-NEXT:         if ne<u128>(call<u128, signature=fn(u128, i32) -> u128>(%[[VALUE_f1]], reinterpret<u128, reason=arg, fits=unknown>(read<i128>(%[[VALUE_b]])), reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=unknown>(div<u64, by_zero=ub>(mul<u64, overflow=wrap>(const<u64>(16), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))))), shl<u128, overflow=wrap, amount_out_of_range=ub>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(257))), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(16), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(7))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<u128>(call<u128, signature=fn(u128, i32) -> u128>(%[[VALUE_f1]], reinterpret<u128, reason=arg,
// DEFAULT-SAME: fits=unknown>(read<i128>(%[[VALUE_b]])), reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=unknown>(add<u64, overflow=wrap>(div<u64,
// DEFAULT-SAME: by_zero=ub>(mul<u64, overflow=wrap>(const<u64>(16), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))))), shl<u128, overflow=wrap, amount_out_of_range=ub>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(257))), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(16), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(5))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         let %[[VALUE_c:[0-9]+]] c: i128 [storage=automatic] = reinterpret<i128, reason=assign, fits=unknown>(shl<u128, overflow=wrap, amount_out_of_range=ub>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(1))), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(16), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))));
// DEFAULT-NEXT:         if ne<u128>(reinterpret<u128, reason=explicit, fits=unknown>(call<i128, signature=fn(i128, i32) -> i128>(%[[VALUE_f2]], read<i128>(%[[VALUE_c]]), const<i32>(5))), shl<u128, overflow=wrap, amount_out_of_range=ub>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(31))), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(16), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(5))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<u128>(reinterpret<u128, reason=explicit, fits=unknown>(call<i128, signature=fn(i128, i32) -> i128>(%[[VALUE_f2]],
// DEFAULT-SAME: read<i128>(%[[VALUE_c]]), reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=unknown>(div<u64, by_zero=ub>(mul<u64,
// DEFAULT-SAME: overflow=wrap>(const<u64>(16), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))))), shl<u128, overflow=wrap, amount_out_of_range=ub>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), sub<u64, overflow=wrap>(div<u64, by_zero=ub>(mul<u64, overflow=wrap>(const<u64>(16), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<u128>(reinterpret<u128, reason=explicit, fits=unknown>(call<i128, signature=fn(i128, i32) -> i128>(%[[VALUE_f2]],
// DEFAULT-SAME: read<i128>(%[[VALUE_c]]), reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=unknown>(add<u64, overflow=wrap>(div<u64,
// DEFAULT-SAME: by_zero=ub>(mul<u64, overflow=wrap>(const<u64>(16), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))))), shl<u128, overflow=wrap, amount_out_of_range=ub>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(neg<i32, overflow=ub>(const<i32>(1)))), sub<u64, overflow=wrap>(div<u64, by_zero=ub>(mul<u64, overflow=wrap>(const<u64>(16), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         let %[[VALUE_d:[0-9]+]] d: u128 [storage=automatic] = shl<u128, overflow=wrap, amount_out_of_range=ub>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(1))), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(16), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         if ne<u128>(call<u128, signature=fn(u128, i32) -> u128>(%[[VALUE_f3]], reinterpret<u128, reason=arg, fits=unknown>(read<i128>(%[[VALUE_c]])), const<i32>(5)), shl<u128, overflow=wrap, amount_out_of_range=ub>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(1))), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(16), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(5))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<u128>(call<u128, signature=fn(u128, i32) -> u128>(%[[VALUE_f3]], reinterpret<u128, reason=arg,
// DEFAULT-SAME: fits=unknown>(read<i128>(%[[VALUE_c]])), reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=unknown>(div<u64, by_zero=ub>(mul<u64,
// DEFAULT-SAME: overflow=wrap>(const<u64>(16), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))))), shl<u128, overflow=wrap, amount_out_of_range=ub>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(1))), sub<u64, overflow=wrap>(div<u64, by_zero=ub>(mul<u64, overflow=wrap>(const<u64>(16), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if ne<u128>(call<u128, signature=fn(u128, i32) -> u128>(%[[VALUE_f3]], reinterpret<u128, reason=arg,
// DEFAULT-SAME: fits=unknown>(read<i128>(%[[VALUE_c]])), reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=unknown>(add<u64, overflow=wrap>(div<u64,
// DEFAULT-SAME: by_zero=ub>(mul<u64, overflow=wrap>(const<u64>(16), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))))), shl<u128, overflow=wrap, amount_out_of_range=ub>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(1))), sub<u64, overflow=wrap>(div<u64, by_zero=ub>(mul<u64, overflow=wrap>(const<u64>(16), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
