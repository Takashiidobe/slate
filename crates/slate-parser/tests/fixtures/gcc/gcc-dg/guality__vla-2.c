/* PR debug/42801 */
/* { dg-do run } */
/* { dg-options "-g" } */

void __attribute__((noinline)) fn1(int *x, int y) {
  asm volatile("" : : "rm"(x), "rm"(y) : "memory");
}

static inline __attribute__((always_inline)) int fn2(int i) {
  int a[i];
  fn1(a, i);
  fn1(a, i); /* { dg-final { gdb-test . "sizeof (a)" "5 * sizeof (int)" } } */
  return i;
}

static inline __attribute__((always_inline)) int fn3(int i) {
  int a[i];
  fn1(a, i);
  fn1(a, i); /* { dg-final { gdb-test . "sizeof (a)" "6 * sizeof (int)" } } */
  return i;
}

static inline __attribute__((always_inline)) int fn4(int i) { return fn3(i); }

int __attribute__((noinline)) fn5(void) { return fn2(5) + 1; }

int __attribute__((noinline)) fn6(int i) {
  return fn2(i + 1) + fn4(i + 2) + fn4(i + 2) + 1;
}

int
main(void) {
  int x = 4;
  asm volatile("" : "+r"(x));
  fn5();
  fn6(x);
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
// DEFAULT-NEXT:     fn %0 @fn1(%1 x: ptr<i32>, %2 y: i32) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         asm volatile "" [dialect=att] {
// DEFAULT-NEXT:             in 0 "rm" [reg | mem] -> reg width 64 read<ptr<i32>>(%1);
// DEFAULT-NEXT:             in 1 "rm" [reg | mem] -> reg width 32 read<i32>(%2);
// DEFAULT-NEXT:             clobbers: memory;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @fn2(%4 i: i32) -> i32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %16: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%4)));
// DEFAULT-NEXT:         let %5 a: vla<i32, %16> [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>, i32) -> void>(%0, array_decay<ptr<i32>, length=None>(%5), read<i32>(%4));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>, i32) -> void>(%0, array_decay<ptr<i32>, length=None>(%5), read<i32>(%4));
// DEFAULT-NEXT:         return read<i32>(%4);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @fn3(%7 i: i32) -> i32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %17: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%7)));
// DEFAULT-NEXT:         let %8 a: vla<i32, %17> [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>, i32) -> void>(%0, array_decay<ptr<i32>, length=None>(%8), read<i32>(%7));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>, i32) -> void>(%0, array_decay<ptr<i32>, length=None>(%8), read<i32>(%7));
// DEFAULT-NEXT:         return read<i32>(%7);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @fn4(%10 i: i32) -> i32 [linkage=internal] [inline=always] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i32, signature=fn(i32) -> i32>(%6, read<i32>(%10));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @fn5() -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(call<i32, signature=fn(i32) -> i32>(%3, const<i32>(5)), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @fn6(%13 i: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(call<i32, signature=fn(i32) -> i32>(%3, add<i32, overflow=ub>(read<i32>(%13), const<i32>(1))), call<i32, signature=fn(i32) -> i32>(%9, add<i32, overflow=ub>(read<i32>(%13), const<i32>(2)))), call<i32, signature=fn(i32) -> i32>(%9, add<i32, overflow=ub>(read<i32>(%13), const<i32>(2)))), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %15 x: i32 [storage=automatic] = const<i32>(4);
// DEFAULT-NEXT:         asm volatile "" [dialect=att] {
// DEFAULT-NEXT:             inlateout 0 "r" [reg] width 32 place<i32>(%15);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%11);
// DEFAULT-NEXT:         call<i32, signature=fn(i32) -> i32>(%12, read<i32>(%15));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
