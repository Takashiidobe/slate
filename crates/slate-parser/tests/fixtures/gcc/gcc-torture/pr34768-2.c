int x;

int __attribute__((noinline)) foo(void) {
  x = -x;
  return 0;
}
int __attribute__((const, noinline)) bar(void) { return 0; }

int __attribute__((noinline)) test(int c) {
  int tmp = x;
  int res = (c ? foo : bar)();
  return tmp + x + res;
}

extern void abort(void);
int         main() {
  x = 1;
  if (test(1) != 0)
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
// DEFAULT-NEXT:     global %0 x: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %1 @foo() -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i32>(%0, neg<i32, overflow=ub>(read<i32>(%0)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %2 @bar() -> i32 [linkage=external] [inline=never] [definition=emitted] [memory=none] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @test(%4 c: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %5 tmp: i32 [storage=automatic] = read<i32>(%0);
// DEFAULT-NEXT:         let %6 res: i32 [storage=automatic] = call<i32, signature=fn() -> i32>(conditional<ptr<fn() -> i32>>(ne<i32>(read<i32>(%4), const<i32>(0)), function_decay<ptr<fn() -> i32>>(%1), function_decay<ptr<fn() -> i32>>(%2)));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(%5), read<i32>(%0)), read<i32>(%6));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %8 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<i32>(%0, const<i32>(1));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%3, const<i32>(1)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%7);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
