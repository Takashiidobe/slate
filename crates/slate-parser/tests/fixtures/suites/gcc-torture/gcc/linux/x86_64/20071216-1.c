/* PR rtl-optimization/34490 */

extern void abort(void);

static int x;

int __attribute__((noinline)) bar(void) { return x; }

int foo(void) {
  long int b = bar();
  if ((unsigned long)b < -4095L)
    return b;
  if (-b != 38)
    b = -2;
  return b + 1;
}

int main(void) {
  x = 26;
  if (foo() != 26)
    abort();
  x = -39;
  if (foo() != -1)
    abort();
  x = -38;
  if (foo() != -37)
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
// DEFAULT-NEXT:     global %1 x: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %2 @bar() -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @foo() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %4 b: i64 [storage=automatic] = widen<i64, reason=assign>(call<i32, signature=fn() -> i32>(%2));
// DEFAULT-NEXT:         if lt<u64>(reinterpret<u64, reason=explicit, fits=unknown>(read<i64>(%4)), reinterpret<u64, reason=usual_arith, fits=unknown>(neg<i64, overflow=ub>(const<i64>(4095))))
// DEFAULT-NEXT:             return truncate<i32, reason=return, fits=unknown>(read<i64>(%4));
// DEFAULT-NEXT:         if ne<i64>(neg<i64, overflow=ub>(read<i64>(%4)), widen<i64, reason=usual_arith>(const<i32>(38)))
// DEFAULT-NEXT:             write<i64>(%4, widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(2))));
// DEFAULT-NEXT:         return truncate<i32, reason=return, fits=unknown>(add<i64, overflow=ub>(read<i64>(%4), widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<i32>(%1, const<i32>(26));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn() -> i32>(%3), const<i32>(26))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<i32>(%1, neg<i32, overflow=ub>(const<i32>(39)));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn() -> i32>(%3), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<i32>(%1, neg<i32, overflow=ub>(const<i32>(38)));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn() -> i32>(%3), neg<i32, overflow=ub>(const<i32>(37)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
