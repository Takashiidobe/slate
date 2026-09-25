/* PR rtl-optimization/44858 */

extern void abort(void);
int         a = 3;
int         b = 1;

__attribute__((noinline)) long long foo(int x, int y) { return x / y; }

__attribute__((noinline)) int bar(void) {
  int c  = 2;
  c     &= foo(1, b) > b;
  b      = (a != 0) | c;
  return c;
}

int main(void) {
  if (bar() != 0 || b != 1)
    abort();
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
// DEFAULT-NEXT:     global %1 a: i32 [storage=static] = const<i32>(3) [linkage=external];
// DEFAULT-NEXT:     global %2 b: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @foo(%4 x: i32, %5 y: i32) -> i64 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return widen<i64, reason=return>(div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%4), read<i32>(%5)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @bar() -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %7 c: i32 [storage=automatic] = const<i32>(2);
// DEFAULT-NEXT:         let %9: i32 [synthetic] = read<i32>(%7);
// DEFAULT-NEXT:         let %10: i32 [synthetic] = and<i32>(read<i32>(%9), from_bool<i32, reason=promotion>(gt<i64>(call<i64, signature=fn(i32, i32) -> i64>(%3, const<i32>(1), read<i32>(%2)), widen<i64, reason=usual_arith>(read<i32>(%2)))));
// DEFAULT-NEXT:         write<i32>(%7, read<i32>(%10));
// DEFAULT-NEXT:         write<i32>(%2, or<i32>(from_bool<i32, reason=promotion>(ne<i32>(read<i32>(%1), const<i32>(0))), read<i32>(%7)));
// DEFAULT-NEXT:         return read<i32>(%7);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%6), const<i32>(0)), ne<i32>(read<i32>(%2), const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
