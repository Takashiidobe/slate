/* PR target/111408 */

int   a, b, c, d;
short e;

int foo() {
  c = a % (sizeof(int) * 8);
  if (b & 1 << c)
    return -1;
  return 0;
}

int main() {
  for (; e != 1; e++) {
    int g = foo();
    if (g + d - 9 + d)
      continue;
    for (;;)
      __builtin_abort();
  }
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
// DEFAULT-NEXT:     global %1 b: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 c: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 d: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 e: i16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %5 @foo() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i32>(%2, reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(rem<u64, by_zero=ub>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%0))), mul<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))))))));
// DEFAULT-NEXT:         if ne<i32>(and<i32>(read<i32>(%1), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), read<i32>(%2))), const<i32>(0))
// DEFAULT-NEXT:             return neg<i32, overflow=ub>(const<i32>(1));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         for %8
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: ne<i32>(widen<i32, reason=promotion>(read<i16>(%4)), const<i32>(1))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %10: i16 [synthetic] = read<i16>(%4);
// DEFAULT-NEXT:                 let %11: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%10)), const<i32>(1)));
// DEFAULT-NEXT:                 write<i16>(%4, read<i16>(%11));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %7 g: i32 [storage=automatic] = call<i32, signature=fn() -> i32>(%5);
// DEFAULT-NEXT:                     if ne<i32>(add<i32, overflow=ub>(sub<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(%7), read<i32>(%3)), const<i32>(9)), read<i32>(%3)), const<i32>(0))
// DEFAULT-NEXT:                         continue %8;
// DEFAULT-NEXT:                     for %9
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                         condition: omitted
// DEFAULT-NEXT:                         increment: omitted
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
