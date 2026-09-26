/* PR rtl-optimization/57131 */

extern void abort(void);

int main() {
  volatile int       x1 = 0;
  volatile long long x2 = 0;
  volatile int       x3 = 0;
  volatile int       x4 = 1;
  volatile int       x5 = 1;
  volatile long long x6 = 1;
  long long          t  = ((x1 * (x2 << x3)) / (x4 * x5)) + x6;

  if (t != 1)
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
// DEFAULT-NEXT:     fn %1 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %2 x1: volatile i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %3 x2: volatile i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(0));
// DEFAULT-NEXT:         let %4 x3: volatile i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %5 x4: volatile i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:         let %6 x5: volatile i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:         let %7 x6: volatile i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(1));
// DEFAULT-NEXT:         let %8 t: i64 [storage=automatic] = add<i64, overflow=ub>(div<i64, by_zero=ub, min_by_neg_one=ub>(mul<i64, overflow=ub>(widen<i64, reason=usual_arith>(read<i32, volatile>(%2)), shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i64, volatile>(%3), read<i32, volatile>(%4))), widen<i64, reason=usual_arith>(mul<i32, overflow=ub>(read<i32, volatile>(%5), read<i32, volatile>(%6)))), read<i64, volatile>(%7));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%8), widen<i64, reason=usual_arith>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
