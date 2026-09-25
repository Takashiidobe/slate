/* PR tree-optimization/65216 */

int          a, b = 62, e;
volatile int c, d;

int main() {
  int f = 0;
  for (a = 0; a < 2; a++) {
    b &= (8 ^ f) & 1;
    for (e = 0; e < 6; e++)
      if (c)
        f = d;
  }
  if (b != 0)
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
// DEFAULT-NEXT:     global %0 a: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1 b: i32 [storage=static] = const<i32>(62) [linkage=external];
// DEFAULT-NEXT:     global %2 e: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 c: volatile i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 d: volatile i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %5 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %6 f: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %7
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%0, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%0), const<i32>(2))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %9: i32 [synthetic] = read<i32>(%0);
// DEFAULT-NEXT:                 let %10: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%9), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%0, read<i32>(%10));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %11: i32 [synthetic] = read<i32>(%1);
// DEFAULT-NEXT:                     let %12: i32 [synthetic] = and<i32>(read<i32>(%11), and<i32>(xor<i32>(const<i32>(8), read<i32>(%6)), const<i32>(1)));
// DEFAULT-NEXT:                     write<i32>(%1, read<i32>(%12));
// DEFAULT-NEXT:                     for %8
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                             write<i32>(%2, const<i32>(0));
// DEFAULT-NEXT:                         condition: lt<i32>(read<i32>(%2), const<i32>(6))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %13: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:                             let %14: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%13), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%2, read<i32>(%14));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             if ne<i32>(read<i32, volatile>(%3), const<i32>(0))
// DEFAULT-NEXT:                                 write<i32>(%6, read<i32, volatile>(%4));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%1), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
