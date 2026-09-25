/* PR tree-optimization/80426 */
/* Testcase by <ishiura-compiler@ml.kwansei.ac.jp> */

#define INT_MAX 0x7fffffff
#define INT_MIN (-INT_MAX - 1)

int x;

int main(void) {
  volatile int a = 0;
  volatile int b = -INT_MAX;
  int          j;

  for (j = 0; j < 18; j += 1) {
    x = ((a == 0) != (b - (int)(INT_MIN)));
  }

  if (x != 0)
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
// DEFAULT-NEXT:     global %0 x: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %1 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %2 a: volatile i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %3 b: volatile i32 [storage=automatic] = neg<i32, overflow=ub>(const<i32>(2147483647));
// DEFAULT-NEXT:         let %4 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %5
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%4, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%4), const<i32>(18))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %6: i32 [synthetic] = read<i32>(%4);
// DEFAULT-NEXT:                 let %7: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%6), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%4, read<i32>(%7));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<i32>(%0, from_bool<i32, reason=assign>(ne<i32>(from_bool<i32, reason=promotion>(eq<i32>(read<i32, volatile>(%2), const<i32>(0))), sub<i32, overflow=ub>(read<i32, volatile>(%3), sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1))))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%0), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
