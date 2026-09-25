extern void exit(int);
extern void abort(void);

volatile int        a = 1;
volatile int        b = 0;
volatile int        x = 2;
volatile signed int r = 8;

void __attribute__((noinline)) foo(void) { exit(0); }

int main(void) {
  int si1 = a;
  int si2 = b;
  int i;

  for (i = 0; i < 100; ++i) {
    foo();
    if (x == 8)
      i++;
    r += i + si1 % si2;
  }
  abort();
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
// DEFAULT-NEXT:     global %2 a: volatile i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %3 b: volatile i32 [storage=static] = const<i32>(0) [linkage=external];
// DEFAULT-NEXT:     global %4 x: volatile i32 [storage=static] = const<i32>(2) [linkage=external];
// DEFAULT-NEXT:     global %5 r: volatile i32 [storage=static] = const<i32>(8) [linkage=external];
// DEFAULT-NEXT:     fn %0 @exit(%11 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %6 @foo() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%0, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %8 si1: i32 [storage=automatic] = read<i32, volatile>(%2);
// DEFAULT-NEXT:         let %9 si2: i32 [storage=automatic] = read<i32, volatile>(%3);
// DEFAULT-NEXT:         let %10 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %12
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%10, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%10), const<i32>(100))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %13: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:                 let %14: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%13), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%10, read<i32>(%14));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:                     if eq<i32>(read<i32, volatile>(%4), const<i32>(8))
// DEFAULT-NEXT:                         let %15: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:                         let %16: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%15), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%10, read<i32>(%16));
// DEFAULT-NEXT:                     let %17: i32 [synthetic] = read<i32, volatile>(%5);
// DEFAULT-NEXT:                     let %18: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%17), add<i32, overflow=ub>(read<i32>(%10), rem<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%8), read<i32>(%9))));
// DEFAULT-NEXT:                     write<i32, volatile>(%5, read<i32>(%18));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
