/* PR tree-optimization/97888 */

int a = 1, c = 4, d, e;

int main() {
  int f = -173;
  int b;
  for (b = 0; b < 10; b++) {
    int g = f % (~0 && a), h = 0, i = 0;
    if (g)
      __builtin_unreachable();
    if (c)
      h = f;
    if (h > -173)
      e = d / i;
    f = h;
  }
  if (f != -173)
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
// DEFAULT-NEXT:     global %0 a: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %1 c: i32 [storage=static] = const<i32>(4) [linkage=external];
// DEFAULT-NEXT:     global %2 d: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 e: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %5 f: i32 [storage=automatic] = neg<i32, overflow=ub>(const<i32>(173));
// DEFAULT-NEXT:         let %6 b: i32 [storage=automatic];
// DEFAULT-NEXT:         for %10
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%6, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%6), const<i32>(10))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %11: i32 [synthetic] = read<i32>(%6);
// DEFAULT-NEXT:                 let %12: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%11), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%6, read<i32>(%12));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %7 g: i32 [storage=automatic] = rem<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%5), from_bool<i32, reason=promotion>(logical_and<bool>(ne<i32>(not<i32>(const<i32>(0)), const<i32>(0)), ne<i32>(read<i32>(%0), const<i32>(0)))));
// DEFAULT-NEXT:                     let %8 h: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                     let %9 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%7), const<i32>(0))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(__builtin_unreachable);
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%1), const<i32>(0))
// DEFAULT-NEXT:                         write<i32>(%8, read<i32>(%5));
// DEFAULT-NEXT:                     if gt<i32>(read<i32>(%8), neg<i32, overflow=ub>(const<i32>(173)))
// DEFAULT-NEXT:                         write<i32>(%3, div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%2), read<i32>(%9)));
// DEFAULT-NEXT:                     write<i32>(%5, read<i32>(%8));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%5), neg<i32, overflow=ub>(const<i32>(173)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
