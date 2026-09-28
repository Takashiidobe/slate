/* PR rtl-optimization/24899 */

extern void abort(void);

__attribute__((noinline)) int foo(int x, int y, int *z) {
  int a, b, c, d;

  a = b = 0;
  for (d = 0; d < y; d++) {
    if (z)
      b = d * *z;
    for (c = 0; c < x; c++)
      a += b;
  }

  return a;
}

int main(void) {
  if (foo(3, 2, 0) != 0)
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
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @foo(%2 x: i32, %3 y: i32, %4 z: ptr<i32>) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %5 a: i32 [storage=automatic];
// DEFAULT-NEXT:         let %6 b: i32 [storage=automatic];
// DEFAULT-NEXT:         let %7 c: i32 [storage=automatic];
// DEFAULT-NEXT:         let %8 d: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%6, const<i32>(0));
// DEFAULT-NEXT:         write<i32>(%5, const<i32>(0));
// DEFAULT-NEXT:         for %10
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%8, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%8), read<i32>(%3))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %12: i32 [synthetic] = read<i32>(%8);
// DEFAULT-NEXT:                 let %13: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%12), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%8, read<i32>(%13));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if ne<ptr<i32>>(read<ptr<i32>>(%4), null<ptr<i32>>)
// DEFAULT-NEXT:                         write<i32>(%6, mul<i32, overflow=ub>(read<i32>(%8), read<i32>(deref(read<ptr<i32>>(%4)))));
// DEFAULT-NEXT:                     for %11
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                             write<i32>(%7, const<i32>(0));
// DEFAULT-NEXT:                         condition: lt<i32>(read<i32>(%7), read<i32>(%2))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %14: i32 [synthetic] = read<i32>(%7);
// DEFAULT-NEXT:                             let %15: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%14), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%7, read<i32>(%15));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             let %16: i32 [synthetic] = read<i32>(%5);
// DEFAULT-NEXT:                             let %17: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%16), read<i32>(%6));
// DEFAULT-NEXT:                             write<i32>(%5, read<i32>(%17));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return read<i32>(%5);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, i32, ptr<i32>) -> i32>(%1, const<i32>(3), const<i32>(2), null<ptr<i32>>), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
