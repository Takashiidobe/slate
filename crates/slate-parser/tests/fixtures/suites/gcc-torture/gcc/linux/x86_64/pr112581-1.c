/* { dg-require-effective-target int32plus } */
/* PR tree-optimization/112581 */
/* reassociation, used to combine 2 bb to together,
   that made an unitialized variable unconditional used
   which then at runtime would cause an infinite loop.  */
int a = -1, b = 2501896061, c, d, e, f = 3, g;
int main() {
  unsigned h;
  int      i;
  d = 0;
  for (; d < 1; d++) {
    int j = ~-((6UL ^ a) / b);
    if (b)
    L:
      if (!f)
        continue;
    if (c)
      i = 1;
    if (j) {
      i = 0;
      while (e)
        ;
    }
    g = -1 % b;
    h = ~(b || h);
    f = g || 0;
    a = a || 0;
    if (!a)
      h = 0;
    while (h > 4294967294)
      if (i)
        break;
    if (c)
      goto L;
  }
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
// DEFAULT-NEXT:     global %0 a: i32 [storage=static] = neg<i32, overflow=ub>(const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     global %1 b: i32 [storage=static] = truncate<i32, reason=assign, fits=unknown>(const<i64>(2501896061)) [linkage=external];
// DEFAULT-NEXT:     global %2 c: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 d: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 e: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %5 f: i32 [storage=static] = const<i32>(3) [linkage=external];
// DEFAULT-NEXT:     global %6 g: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %7 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %9 h: u32 [storage=automatic];
// DEFAULT-NEXT:         let %10 i: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%3, const<i32>(0));
// DEFAULT-NEXT:         for %12
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%3), const<i32>(1))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %15: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:                 let %16: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%15), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%3, read<i32>(%16));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %11 j: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(not<u64>(neg<u64, overflow=wrap>(div<u64, by_zero=ub>(xor<u64>(const<u64>(6), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%0)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%1))))))));
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%1), const<i32>(0))
// DEFAULT-NEXT:                         label %8 L:
// DEFAULT-NEXT:                             if not<bool>(ne<i32>(read<i32>(%5), const<i32>(0)))
// DEFAULT-NEXT:                                 continue %12;
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%2), const<i32>(0))
// DEFAULT-NEXT:                         write<i32>(%10, const<i32>(1));
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%11), const<i32>(0))
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             write<i32>(%10, const<i32>(0));
// DEFAULT-NEXT:                             while %13 ne<i32>(read<i32>(%4), const<i32>(0))
// DEFAULT-NEXT:                                 ;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                     write<i32>(%6, rem<i32, by_zero=ub, min_by_neg_one=ub>(neg<i32, overflow=ub>(const<i32>(1)), read<i32>(%1)));
// DEFAULT-NEXT:                     write<u32>(%9, reinterpret<u32, reason=assign, fits=unknown>(not<i32>(from_bool<i32, reason=promotion>(logical_or<bool>(ne<i32>(read<i32>(%1), const<i32>(0)), ne<u32>(read<u32>(%9), const<u32>(0)))))));
// DEFAULT-NEXT:                     write<i32>(%5, from_bool<i32, reason=assign>(logical_or<bool>(ne<i32>(read<i32>(%6), const<i32>(0)), ne<i32>(const<i32>(0), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i32>(%0, from_bool<i32, reason=assign>(logical_or<bool>(ne<i32>(read<i32>(%0), const<i32>(0)), ne<i32>(const<i32>(0), const<i32>(0)))));
// DEFAULT-NEXT:                     if not<bool>(ne<i32>(read<i32>(%0), const<i32>(0)))
// DEFAULT-NEXT:                         write<u32>(%9, reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                     while %14 gt<i64>(reinterpret<i64, reason=usual_arith, fits=unknown>(widen<u64, reason=usual_arith>(read<u32>(%9))), const<i64>(4294967294))
// DEFAULT-NEXT:                         if ne<i32>(read<i32>(%10), const<i32>(0))
// DEFAULT-NEXT:                             break %14;
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%2), const<i32>(0))
// DEFAULT-NEXT:                         goto %8;
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
