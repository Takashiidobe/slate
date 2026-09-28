/* PR target/85582 */

int       a, b, d = 2, e;
long long c = 1;

int main() {
  int g = 6;
L1:
  e = d;
  if (a)
    goto L1;
  g--;
  int i = c >> ~(~e | ~g);
L2:
  c = (b % c) * i;
  if (!e)
    goto L2;
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
// DEFAULT-NEXT:     global %1 b: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 d: i32 [storage=static] = const<i32>(2) [linkage=external];
// DEFAULT-NEXT:     global %3 e: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 c: i64 [storage=static] = widen<i64, reason=assign>(const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     fn %5 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %8 g: i32 [storage=automatic] = const<i32>(6);
// DEFAULT-NEXT:         label %6 L1:
// DEFAULT-NEXT:             write<i32>(%3, read<i32>(%2));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%0), const<i32>(0))
// DEFAULT-NEXT:             goto %6;
// DEFAULT-NEXT:         let %10: i32 [synthetic] = read<i32>(%8);
// DEFAULT-NEXT:         let %11: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%10), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%8, read<i32>(%11));
// DEFAULT-NEXT:         let %9 i: i32 [storage=automatic] = truncate<i32, reason=assign, fits=unknown>(shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%4), not<i32>(or<i32>(not<i32>(read<i32>(%3)), not<i32>(read<i32>(%8))))));
// DEFAULT-NEXT:         label %7 L2:
// DEFAULT-NEXT:             write<i64>(%4, mul<i64, overflow=ub>(rem<i64, by_zero=ub, min_by_neg_one=ub>(widen<i64, reason=usual_arith>(read<i32>(%1)), read<i64>(%4)), widen<i64, reason=usual_arith>(read<i32>(%9))));
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32>(%3), const<i32>(0)))
// DEFAULT-NEXT:             goto %7;
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
