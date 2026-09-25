/* PR tree-optimization/119030 */

static inline unsigned foo(long long x) { return x & 0x8000; }

static inline long long bar(long long x) {
  if (foo(x))
    return -1000L;
  else
    return x >> 16;
}

long long x = -0x20000LL;

int main() {
  if (bar(x) >= 0)
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
// DEFAULT-NEXT:     global %4 x: i64 [storage=static] = neg<i64, overflow=ub>(const<i64>(131072)) [linkage=external];
// DEFAULT-NEXT:     fn %0 @foo(%1 x: i64) -> u32 [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(truncate<i32, reason=return, fits=unknown>(and<i64>(read<i64>(%1), widen<i64, reason=usual_arith>(const<i32>(32768)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %2 @bar(%3 x: i64) -> i64 [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(i64) -> u32>(%0, read<i64>(%3)), const<u32>(0))
// DEFAULT-NEXT:             return neg<i64, overflow=ub>(const<i64>(1000));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             return shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%3), const<i32>(16));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ge<i64>(call<i64, signature=fn(i64) -> i64>(%2, read<i64>(%4)), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
