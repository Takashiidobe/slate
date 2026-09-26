/* PR tree-optimization/98736 */

int  a[6];
char b, c;
int  main() {
  int d[4] = {0, 0, 0, 0};
  for (c = 0; c <= 5; c++) {
    for (b = 2; b != 0; b++)
      a[c] = 8;
    a[c] = d[3];
  }
  if (a[0] != 0)
    __builtin_abort();
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
// DEFAULT-NEXT:     global %0 a: array<i32, 6> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %1 b: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 c: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %3 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %4 d: array<i32, 4> [storage=automatic] [align=16] = aggregate<array<i32, 4>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(0), index2 = const<i32>(0), index3 = const<i32>(0));
// DEFAULT-NEXT:         for %5
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i8>(%2, truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:             condition: le<i32>(widen<i32, reason=promotion>(read<i8>(%2)), const<i32>(5))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %7: i8 [synthetic] = read<i8>(%2);
// DEFAULT-NEXT:                 let %8: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%7)), const<i32>(1)));
// DEFAULT-NEXT:                 write<i8>(%2, read<i8>(%8));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     for %6
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                             write<i8>(%1, truncate<i8, reason=assign, fits=always>(const<i32>(2)));
// DEFAULT-NEXT:                         condition: ne<i32>(widen<i32, reason=promotion>(read<i8>(%1)), const<i32>(0))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %9: i8 [synthetic] = read<i8>(%1);
// DEFAULT-NEXT:                             let %10: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%9)), const<i32>(1)));
// DEFAULT-NEXT:                             write<i8>(%1, read<i8>(%10));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(6)>(%0), widen<i32, reason=promotion>(read<i8>(%2)))), const<i32>(8));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(6)>(%0), widen<i32, reason=promotion>(read<i8>(%2)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%4), const<i32>(3)))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(6)>(%0), const<i32>(0)))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
