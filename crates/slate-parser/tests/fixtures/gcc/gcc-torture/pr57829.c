/* PR rtl-optimization/57829 */

__attribute__((noinline, noclone)) int f1(int k) {
  return 2 | ((k - 1) >> ((int)sizeof(int) * __CHAR_BIT__ - 1));
}

__attribute__((noinline, noclone)) long int f2(long int k) {
  return 2L | ((k - 1L) >> ((int)sizeof(long int) * __CHAR_BIT__ - 1));
}

__attribute__((noinline, noclone)) int f3(int k) {
  k &= 63;
  return 4 | ((k + 2) >> 5);
}

int main() {
  if (f1(1) != 2 || f2(1L) != 2L || f3(63) != 6 || f3(1) != 4)
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
// DEFAULT-NEXT:     fn %0 @f1(%1 k: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return or<i32>(const<i32>(2), shr<i32, amount_out_of_range=ub, fill=sign_extend>(sub<i32, overflow=ub>(read<i32>(%1), const<i32>(1)), sub<i32, overflow=ub>(mul<i32, overflow=ub>(reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=always>(const<u64>(4))), const<i32>(8)), const<i32>(1))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %2 @f2(%3 k: i64) -> i64 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return or<i64>(const<i64>(2), shr<i64, amount_out_of_range=ub, fill=sign_extend>(sub<i64, overflow=ub>(read<i64>(%3), const<i64>(1)), sub<i32, overflow=ub>(mul<i32, overflow=ub>(reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=always>(const<u64>(8))), const<i32>(8)), const<i32>(1))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @f3(%5 k: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %7: i32 [synthetic] = read<i32>(%5);
// DEFAULT-NEXT:         let %8: i32 [synthetic] = and<i32>(read<i32>(%7), const<i32>(63));
// DEFAULT-NEXT:         write<i32>(%5, read<i32>(%8));
// DEFAULT-NEXT:         return or<i32>(const<i32>(4), shr<i32, amount_out_of_range=ub, fill=sign_extend>(add<i32, overflow=ub>(read<i32>(%5), const<i32>(2)), const<i32>(5)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %9: bool [synthetic];
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%0, const<i32>(1)), const<i32>(2))
// DEFAULT-NEXT:             write<bool>(%9, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%9, ne<i64>(call<i64, signature=fn(i64) -> i64>(%2, const<i64>(1)), const<i64>(2)));
// DEFAULT-NEXT:         let %10: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%9)
// DEFAULT-NEXT:             write<bool>(%10, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%10, ne<i32>(call<i32, signature=fn(i32) -> i32>(%4, const<i32>(63)), const<i32>(6)));
// DEFAULT-NEXT:         let %11: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%10)
// DEFAULT-NEXT:             write<bool>(%11, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%11, ne<i32>(call<i32, signature=fn(i32) -> i32>(%4, const<i32>(1)), const<i32>(4)));
// DEFAULT-NEXT:         if read<bool>(%11)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
