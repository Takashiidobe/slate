/* PR middle-end/111151 */

int main() {
  unsigned a = (1U + __INT_MAX__) / 2U;
  unsigned b = 1U;
  unsigned c = (a * 2U > b * 2U ? a * 2U : b * 2U) * 2U;
  if (c != 0U)
    __builtin_abort();
  int d = (-__INT_MAX__ - 1) / 2;
  int e = 10;
  int f = (d * 2 > e * 5 ? d * 2 : e * 5) * 6;
  if (f != 300)
    __builtin_abort();
  int g = (-__INT_MAX__ - 1) / 2;
  int h = 0;
  int i = (g * 2 > h * 5 ? g * 2 : h * 5) / -1;
  if (i != 0)
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
// DEFAULT-NEXT:     fn %0 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %1 a: u32 [storage=automatic] = div<u32, by_zero=ub>(add<u32, overflow=wrap>(const<u32>(1), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647))), const<u32>(2));
// DEFAULT-NEXT:         let %2 b: u32 [storage=automatic] = const<u32>(1);
// DEFAULT-NEXT:         let %3 c: u32 [storage=automatic] = mul<u32, overflow=wrap>(conditional<u32>(gt<u32>(mul<u32, overflow=wrap>(read<u32>(%1), const<u32>(2)), mul<u32, overflow=wrap>(read<u32>(%2), const<u32>(2))), mul<u32, overflow=wrap>(read<u32>(%1), const<u32>(2)), mul<u32, overflow=wrap>(read<u32>(%2), const<u32>(2))), const<u32>(2));
// DEFAULT-NEXT:         if ne<u32>(read<u32>(%3), const<u32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         let %4 d: i32 [storage=automatic] = div<i32, by_zero=ub, min_by_neg_one=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)), const<i32>(2));
// DEFAULT-NEXT:         let %5 e: i32 [storage=automatic] = const<i32>(10);
// DEFAULT-NEXT:         let %6 f: i32 [storage=automatic] = mul<i32, overflow=ub>(conditional<i32>(gt<i32>(mul<i32, overflow=ub>(read<i32>(%4), const<i32>(2)), mul<i32, overflow=ub>(read<i32>(%5), const<i32>(5))), mul<i32, overflow=ub>(read<i32>(%4), const<i32>(2)), mul<i32, overflow=ub>(read<i32>(%5), const<i32>(5))), const<i32>(6));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%6), const<i32>(300))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         let %7 g: i32 [storage=automatic] = div<i32, by_zero=ub, min_by_neg_one=ub>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1)), const<i32>(2));
// DEFAULT-NEXT:         let %8 h: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %9 i: i32 [storage=automatic] = div<i32, by_zero=ub, min_by_neg_one=ub>(conditional<i32>(gt<i32>(mul<i32, overflow=ub>(read<i32>(%7), const<i32>(2)), mul<i32, overflow=ub>(read<i32>(%8), const<i32>(5))), mul<i32, overflow=ub>(read<i32>(%7), const<i32>(2)), mul<i32, overflow=ub>(read<i32>(%8), const<i32>(5))), neg<i32, overflow=ub>(const<i32>(1)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%9), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
