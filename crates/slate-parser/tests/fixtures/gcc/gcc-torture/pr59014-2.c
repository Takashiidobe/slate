/* PR tree-optimization/59014 */

__attribute__((noinline, noclone)) long long int foo(long long int x,
                                                     long long int y) {
  if (((int)x | (int)y) != 0)
    return 6;
  return x + y;
}

int main() {
  if (sizeof(long long) == sizeof(int))
    return 0;
  int           shift_half = sizeof(int) * __CHAR_BIT__ / 2;
  long long int x          = (3LL << shift_half) << shift_half;
  long long int y          = (5LL << shift_half) << shift_half;
  long long int z          = foo(x, y);
  if (z != ((8LL << shift_half) << shift_half))
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
// DEFAULT-NEXT:     fn %0 @foo(%1 x: i64, %2 y: i64) -> i64 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<i32>(or<i32>(truncate<i32, reason=explicit, fits=unknown>(read<i64>(%1)), truncate<i32, reason=explicit, fits=unknown>(read<i64>(%2))), const<i32>(0))
// DEFAULT-NEXT:             return widen<i64, reason=return>(const<i32>(6));
// DEFAULT-NEXT:         return add<i64, overflow=ub>(read<i64>(%1), read<i64>(%2));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if eq<u64>(const<u64>(8), const<u64>(4))
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:         let %4 shift_half: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(div<u64, by_zero=ub>(mul<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))));
// DEFAULT-NEXT:         let %5 x: i64 [storage=automatic] = shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i64>(3), read<i32>(%4)), read<i32>(%4));
// DEFAULT-NEXT:         let %6 y: i64 [storage=automatic] = shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i64>(5), read<i32>(%4)), read<i32>(%4));
// DEFAULT-NEXT:         let %7 z: i64 [storage=automatic] = call<i64, signature=fn(i64, i64) -> i64>(%0, read<i64>(%5), read<i64>(%6));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%7), shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i64>(8), read<i32>(%4)), read<i32>(%4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
