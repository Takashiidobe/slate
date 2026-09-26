/* PR rtl-optimization/60116 */
/* Reported by Zhendong Su <su@cs.ucdavis.edu> */

extern void abort(void);

int  a, b, c, d = 1, e, f = 1, h, i, k;
char g, j;

void fn1(void) {
  int l;
  e = 0;
  c = 0;
  for (;;) {
    k = a && b;
    j = k * 54;
    g = j * 147;
    l = ~g + (long long)e && 1;
    if (d)
      c = l;
    else
      h = i = l * 9UL;
    if (f)
      return;
  }
}

int main(void) {
  fn1();
  if (c != 1)
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
// DEFAULT-NEXT:     global %1 a: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 b: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 c: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 d: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %5 e: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %6 f: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %7 h: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %8 i: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %9 k: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %10 g: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %11 j: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %12 @fn1() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %13 l: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%5, const<i32>(0));
// DEFAULT-NEXT:         write<i32>(%3, const<i32>(0));
// DEFAULT-NEXT:         for %15
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: omitted
// DEFAULT-NEXT:             increment: omitted
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<i32>(%9, from_bool<i32, reason=assign>(logical_and<bool>(ne<i32>(read<i32>(%1), const<i32>(0)), ne<i32>(read<i32>(%2), const<i32>(0)))));
// DEFAULT-NEXT:                     write<i8>(%11, truncate<i8, reason=assign, fits=unknown>(mul<i32, overflow=ub>(read<i32>(%9), const<i32>(54))));
// DEFAULT-NEXT:                     write<i8>(%10, truncate<i8, reason=assign, fits=unknown>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%11)), const<i32>(147))));
// DEFAULT-NEXT:                     write<i32>(%13, from_bool<i32, reason=assign>(logical_and<bool>(ne<i64>(add<i64, overflow=ub>(widen<i64, reason=usual_arith>(not<i32>(widen<i32, reason=promotion>(read<i8>(%10)))), widen<i64, reason=explicit>(read<i32>(%5))), const<i64>(0)), ne<i32>(const<i32>(1), const<i32>(0)))));
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%4), const<i32>(0))
// DEFAULT-NEXT:                         write<i32>(%3, read<i32>(%13));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         write<i32>(%8, reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%13))), const<u64>(9)))));
// DEFAULT-NEXT:                         write<i32>(%7, reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%13))), const<u64>(9)))));
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%6), const<i32>(0))
// DEFAULT-NEXT:                         return;
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%12);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%3), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
