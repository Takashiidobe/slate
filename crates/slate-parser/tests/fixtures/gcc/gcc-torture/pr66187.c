/* PR tree-optimization/66187 */

int   a = 1, e = -1;
short b, f;

int main() {
  f     = e;
  int g = b < 0 ? 0 : f + b;
  if ((g & -4) < 0)
    a = 0;
  if (a)
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
// DEFAULT-NEXT:     global %1 e: i32 [storage=static] = neg<i32, overflow=ub>(const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     global %2 b: i16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 f: i16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<i16>(%3, truncate<i16, reason=assign, fits=unknown>(read<i32>(%1)));
// DEFAULT-NEXT:         let %5 g: i32 [storage=automatic] = conditional<i32>(lt<i32>(widen<i32, reason=promotion>(read<i16>(%2)), const<i32>(0)), const<i32>(0), add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%3)), widen<i32, reason=promotion>(read<i16>(%2))));
// DEFAULT-NEXT:         if lt<i32>(and<i32>(read<i32>(%5), neg<i32, overflow=ub>(const<i32>(4))), const<i32>(0))
// DEFAULT-NEXT:             write<i32>(%0, const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%0), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
