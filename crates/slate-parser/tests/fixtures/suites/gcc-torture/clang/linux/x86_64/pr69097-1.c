/* PR tree-optimization/69097 */

int          a, b;
unsigned int c;

int main() {
  int d = b;
  b     = ~(~a + (~d | b));
  a     = ~(~c >> b);
  c     = a % b;
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
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: u32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_d:[0-9]+]] d: i32 [storage=automatic] = read<i32>(%[[VALUE_b]]);
// DEFAULT-NEXT:         write<i32>(%[[VALUE_b]], not<i32>(add<i32, overflow=ub>(not<i32>(read<i32>(%[[VALUE_a]])), or<i32>(not<i32>(read<i32>(%[[VALUE_d]])), read<i32>(%[[VALUE_b]])))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_a]], reinterpret<i32, reason=assign, fits=unknown>(not<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(not<u32>(read<u32>(%[[VALUE_c]])), read<i32>(%[[VALUE_b]])))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_c]], reinterpret<u32, reason=assign, fits=unknown>(rem<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%[[VALUE_a]]), read<i32>(%[[VALUE_b]]))));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
