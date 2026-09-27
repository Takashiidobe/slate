/* { dg-do run { target { { i?86-*-* x86_64-*-* } && lp64 } } } */
/* { dg-options "-g" } */

volatile int vv;

__attribute__((noclone, noinline)) long f1(unsigned long x) {
  long f = (x << 12) | (x >> (64 - 12));
  long g = f;
  asm volatile("" : "+r"(f));
  vv++; /* { dg-final { gdb-test . "g" "f" } } */
  return f;
}

__attribute__((noclone, noinline)) long f2(unsigned long x, int y) {
  long f = (x << y) | (x >> (64 - y));
  long g = f;
  asm volatile("" : "+r"(f));
  vv++; /* { dg-final { gdb-test . "g" "f" } } */
  return f;
}

__attribute__((noclone, noinline)) long f3(unsigned long x, int y) {
  long f = (x >> y) | (x << (64 - y));
  long g = f;
  asm volatile("" : "+r"(f));
  vv++; /* { dg-final { gdb-test . "g" "f" } } */
  return f;
}

__attribute__((noclone, noinline)) unsigned int f4(unsigned int x) {
  unsigned int f = (x << 12) | (x >> (32 - 12));
  unsigned int g = f;
  asm volatile("" : "+r"(f));
  vv++; /* { dg-final { gdb-test . "g" "f" } } */
  return f;
}

__attribute__((noclone, noinline)) unsigned int f5(unsigned int x, int y) {
  unsigned int f = (x << y) | (x >> (32 - y));
  unsigned int g = f;
  asm volatile("" : "+r"(f));
  vv++; /* { dg-final { gdb-test . "g" "f" } } */
  return f;
}

__attribute__((noclone, noinline)) unsigned int f6(unsigned int x, int y) {
  unsigned int f = (x >> y) | (x << (32 - y));
  unsigned int g = f;
  asm volatile("" : "+r"(f));
  vv++; /* { dg-final { gdb-test . "g" "f" } } */
  return f;
}

int
main() {
  f1(0x123456789abcde0fUL);
  f2(0x123456789abcde0fUL, 18);
  f3(0x123456789abcde0fUL, 17);
  f4(0x12345678);
  f5(0x12345678, 18);
  f6(0x12345678, 17);
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
// DEFAULT-NEXT:     fn %1 @f1(%2 x: u64) -> i64 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %3 f: i64 [storage=automatic] = reinterpret<i64, reason=assign, fits=unknown>(or<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%2), const<i32>(12)), shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%2), sub<i32, overflow=ub>(const<i32>(64), const<i32>(12)))));
// DEFAULT-NEXT:         let %4 g: i64 [storage=automatic] = read<i64>(%3);
// DEFAULT-NEXT:         asm volatile "" [dialect=att] {
// DEFAULT-NEXT:             out 0 "+r" place<i64>(%3);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         let %30: i32 [synthetic] = read<i32, volatile>(%0);
// DEFAULT-NEXT:         let %31: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%30), const<i32>(1));
// DEFAULT-NEXT:         write<i32, volatile>(%0, read<i32>(%31));
// DEFAULT-NEXT:         return read<i64>(%3);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @f2(%6 x: u64, %7 y: i32) -> i64 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %8 f: i64 [storage=automatic] = reinterpret<i64, reason=assign, fits=unknown>(or<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%6), read<i32>(%7)), shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%6), sub<i32, overflow=ub>(const<i32>(64), read<i32>(%7)))));
// DEFAULT-NEXT:         let %9 g: i64 [storage=automatic] = read<i64>(%8);
// DEFAULT-NEXT:         asm volatile "" [dialect=att] {
// DEFAULT-NEXT:             out 0 "+r" place<i64>(%8);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         let %32: i32 [synthetic] = read<i32, volatile>(%0);
// DEFAULT-NEXT:         let %33: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%32), const<i32>(1));
// DEFAULT-NEXT:         write<i32, volatile>(%0, read<i32>(%33));
// DEFAULT-NEXT:         return read<i64>(%8);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @f3(%11 x: u64, %12 y: i32) -> i64 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %13 f: i64 [storage=automatic] = reinterpret<i64, reason=assign, fits=unknown>(or<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%11), read<i32>(%12)), shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%11), sub<i32, overflow=ub>(const<i32>(64), read<i32>(%12)))));
// DEFAULT-NEXT:         let %14 g: i64 [storage=automatic] = read<i64>(%13);
// DEFAULT-NEXT:         asm volatile "" [dialect=att] {
// DEFAULT-NEXT:             out 0 "+r" place<i64>(%13);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         let %34: i32 [synthetic] = read<i32, volatile>(%0);
// DEFAULT-NEXT:         let %35: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%34), const<i32>(1));
// DEFAULT-NEXT:         write<i32, volatile>(%0, read<i32>(%35));
// DEFAULT-NEXT:         return read<i64>(%13);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @f4(%16 x: u32) -> u32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %17 f: u32 [storage=automatic] = or<u32>(shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%16), const<i32>(12)), shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%16), sub<i32, overflow=ub>(const<i32>(32), const<i32>(12))));
// DEFAULT-NEXT:         let %18 g: u32 [storage=automatic] = read<u32>(%17);
// DEFAULT-NEXT:         asm volatile "" [dialect=att] {
// DEFAULT-NEXT:             out 0 "+r" place<u32>(%17);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         let %36: i32 [synthetic] = read<i32, volatile>(%0);
// DEFAULT-NEXT:         let %37: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%36), const<i32>(1));
// DEFAULT-NEXT:         write<i32, volatile>(%0, read<i32>(%37));
// DEFAULT-NEXT:         return read<u32>(%17);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %19 @f5(%20 x: u32, %21 y: i32) -> u32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %22 f: u32 [storage=automatic] = or<u32>(shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%20), read<i32>(%21)), shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%20), sub<i32, overflow=ub>(const<i32>(32), read<i32>(%21))));
// DEFAULT-NEXT:         let %23 g: u32 [storage=automatic] = read<u32>(%22);
// DEFAULT-NEXT:         asm volatile "" [dialect=att] {
// DEFAULT-NEXT:             out 0 "+r" place<u32>(%22);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         let %38: i32 [synthetic] = read<i32, volatile>(%0);
// DEFAULT-NEXT:         let %39: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%38), const<i32>(1));
// DEFAULT-NEXT:         write<i32, volatile>(%0, read<i32>(%39));
// DEFAULT-NEXT:         return read<u32>(%22);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %24 @f6(%25 x: u32, %26 y: i32) -> u32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %27 f: u32 [storage=automatic] = or<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%25), read<i32>(%26)), shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%25), sub<i32, overflow=ub>(const<i32>(32), read<i32>(%26))));
// DEFAULT-NEXT:         let %28 g: u32 [storage=automatic] = read<u32>(%27);
// DEFAULT-NEXT:         asm volatile "" [dialect=att] {
// DEFAULT-NEXT:             out 0 "+r" place<u32>(%27);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         let %40: i32 [synthetic] = read<i32, volatile>(%0);
// DEFAULT-NEXT:         let %41: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%40), const<i32>(1));
// DEFAULT-NEXT:         write<i32, volatile>(%0, read<i32>(%41));
// DEFAULT-NEXT:         return read<u32>(%27);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %29 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i64, signature=fn(u64) -> i64>(%1, const<u64>(1311768467463790095));
// DEFAULT-NEXT:         call<i64, signature=fn(u64, i32) -> i64>(%5, const<u64>(1311768467463790095), const<i32>(18));
// DEFAULT-NEXT:         call<i64, signature=fn(u64, i32) -> i64>(%10, const<u64>(1311768467463790095), const<i32>(17));
// DEFAULT-NEXT:         call<u32, signature=fn(u32) -> u32>(%15, reinterpret<u32, reason=arg, fits=always>(const<i32>(305419896)));
// DEFAULT-NEXT:         call<u32, signature=fn(u32, i32) -> u32>(%19, reinterpret<u32, reason=arg, fits=always>(const<i32>(305419896)), const<i32>(18));
// DEFAULT-NEXT:         call<u32, signature=fn(u32, i32) -> u32>(%24, reinterpret<u32, reason=arg, fits=always>(const<i32>(305419896)), const<i32>(17));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
