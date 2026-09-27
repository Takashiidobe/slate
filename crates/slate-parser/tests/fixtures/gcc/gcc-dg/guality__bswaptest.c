/* { dg-do run { target { { i?86-*-* x86_64-*-* } && lp64 } } } */
/* { dg-options "-g" } */

volatile int vv;

__attribute__((noclone, noinline)) long foo(long x) {
  long f = __builtin_bswap64(x);
  long g = f;
  asm volatile("" : "+r"(f));
  vv++; /* { dg-final { gdb-test . "g" "f" } } */
  return f;
}

__attribute__((noclone, noinline)) int bar(int x) {
  int f = __builtin_bswap32(x);
  int g = f;
  asm volatile("" : "+r"(f));
  vv++; /* { dg-final { gdb-test . "g" "f" } } */
  return f;
}

int
main() {
  foo(0x123456789abcde0fUL);
  bar(0x12345678);
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
// DEFAULT-NEXT:     global %0 vv: volatile i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %11 @__builtin_bswap64(%10 <unnamed>: u64) -> u64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %1 @foo(%2 x: i64) -> i64 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %3 f: i64 [storage=automatic] = reinterpret<i64, reason=assign, fits=unknown>(call<u64, signature=fn(u64) -> u64>(%11, reinterpret<u64, reason=arg, fits=unknown>(read<i64>(%2))));
// DEFAULT-NEXT:         let %4 g: i64 [storage=automatic] = read<i64>(%3);
// DEFAULT-NEXT:         asm volatile "" [dialect=att] {
// DEFAULT-NEXT:             inlateout 0 "r" [reg] width 64 place<i64>(%3);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         let %14: i32 [synthetic] = read<i32, volatile>(%0);
// DEFAULT-NEXT:         let %15: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%14), const<i32>(1));
// DEFAULT-NEXT:         write<i32, volatile>(%0, read<i32>(%15));
// DEFAULT-NEXT:         return read<i64>(%3);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @__builtin_bswap32(%12 <unnamed>: u32) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %5 @bar(%6 x: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %7 f: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(call<u32, signature=fn(u32) -> u32>(%13, reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%6))));
// DEFAULT-NEXT:         let %8 g: i32 [storage=automatic] = read<i32>(%7);
// DEFAULT-NEXT:         asm volatile "" [dialect=att] {
// DEFAULT-NEXT:             inlateout 0 "r" [reg] width 32 place<i32>(%7);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         let %16: i32 [synthetic] = read<i32, volatile>(%0);
// DEFAULT-NEXT:         let %17: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%16), const<i32>(1));
// DEFAULT-NEXT:         write<i32, volatile>(%0, read<i32>(%17));
// DEFAULT-NEXT:         return read<i32>(%7);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i64, signature=fn(i64) -> i64>(%1, reinterpret<i64, reason=arg, fits=always>(const<u64>(1311768467463790095)));
// DEFAULT-NEXT:         call<i32, signature=fn(i32) -> i32>(%5, const<i32>(305419896));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
