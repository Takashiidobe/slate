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
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: i32 [storage=static] = const<i32>(2) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_e:[0-9]+]] e: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: i64 [storage=static] = widen<i64, reason=assign>(const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_g:[0-9]+]] g: i32 [storage=automatic] = const<i32>(6);
// DEFAULT-NEXT:         label %[[VALUE_L1:[0-9]+]] L1:
// DEFAULT-NEXT:             write<i32>(%[[VALUE_e]], read<i32>(%[[VALUE_d]]));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_a]]), const<i32>(0))
// DEFAULT-NEXT:             goto %[[VALUE_L1]];
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_g]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE0]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_g]], read<i32>(%[[VALUE1]]));
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic] = truncate<i32, reason=assign, fits=unknown>(shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%[[VALUE_c]]), not<i32>(or<i32>(not<i32>(read<i32>(%[[VALUE_e]])), not<i32>(read<i32>(%[[VALUE_g]]))))));
// DEFAULT-NEXT:         label %[[VALUE_L2:[0-9]+]] L2:
// DEFAULT-NEXT:             write<i64>(%[[VALUE_c]], mul<i64, overflow=ub>(rem<i64, by_zero=ub, min_by_neg_one=ub>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_b]])), read<i64>(%[[VALUE_c]])), widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i]]))));
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32>(%[[VALUE_e]]), const<i32>(0)))
// DEFAULT-NEXT:             goto %[[VALUE_L2]];
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
