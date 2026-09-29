/* With -ftree-coalesce-vars, one variable can have SSA names coalesced into
   partitions of other variables: the PHIs below put the two names of b into
   the partitions of a and of c.  All three variables are oversized vectors
   with no register mode, so both partitions are spilled and b legitimately
   lives in two distinct stack slots (its DECL_RTL becomes the "multiple
   places" marker).  Out-of-SSA must keep the two slots distinguishable
   without rejecting this state.  */

typedef long __attribute__((vector_size(16 * sizeof(long)))) v16di;

v16di        g0 = {1}, g1 = {5}, g2 = {3}, g3 = {7};
volatile int p, q;

int main(void) {
  v16di b = g0;
  v16di a;
  if (p)
    a = b;
  else
    a = g1;
  g1 = a;
  b  = g2;
  v16di c;
  if (q)
    c = b;
  else
    c = g3;
  g3 = c;
  if (g1[0] != 5 || g1[1] != 0 || g3[0] != 7 || g3[1] != 0)
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
// DEFAULT-NEXT:     type @type[[TYPE_v16di:[0-9]+]] v16di = vector<i64, 16>;
// DEFAULT-NEXT:     global %[[VALUE_g0:[0-9]+]] g0: vector<i64, 16> [storage=static] = aggregate<vector<i64, 16>, zero_fill=true>(index0 = widen<i64, reason=assign>(const<i32>(1))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g1:[0-9]+]] g1: vector<i64, 16> [storage=static] = aggregate<vector<i64, 16>, zero_fill=true>(index0 = widen<i64, reason=assign>(const<i32>(5))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g2:[0-9]+]] g2: vector<i64, 16> [storage=static] = aggregate<vector<i64, 16>, zero_fill=true>(index0 = widen<i64, reason=assign>(const<i32>(3))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g3:[0-9]+]] g3: vector<i64, 16> [storage=static] = aggregate<vector<i64, 16>, zero_fill=true>(index0 = widen<i64, reason=assign>(const<i32>(7))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_p:[0-9]+]] p: volatile i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_q:[0-9]+]] q: volatile i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: vector<i64, 16> [storage=automatic] = read<vector<i64, 16>>(%[[VALUE_g0]]);
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: vector<i64, 16> [storage=automatic];
// DEFAULT-NEXT:         if ne<i32>(read<i32, volatile>(%[[VALUE_p]]), const<i32>(0))
// DEFAULT-NEXT:             write<vector<i64, 16>>(%[[VALUE_a]], read<vector<i64, 16>>(%[[VALUE_b]]));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<vector<i64, 16>>(%[[VALUE_a]], read<vector<i64, 16>>(%[[VALUE_g1]]));
// DEFAULT-NEXT:         write<vector<i64, 16>>(%[[VALUE_g1]], read<vector<i64, 16>>(%[[VALUE_a]]));
// DEFAULT-NEXT:         write<vector<i64, 16>>(%[[VALUE_b]], read<vector<i64, 16>>(%[[VALUE_g2]]));
// DEFAULT-NEXT:         let %[[VALUE_c:[0-9]+]] c: vector<i64, 16> [storage=automatic];
// DEFAULT-NEXT:         if ne<i32>(read<i32, volatile>(%[[VALUE_q]]), const<i32>(0))
// DEFAULT-NEXT:             write<vector<i64, 16>>(%[[VALUE_c]], read<vector<i64, 16>>(%[[VALUE_b]]));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<vector<i64, 16>>(%[[VALUE_c]], read<vector<i64, 16>>(%[[VALUE_g3]]));
// DEFAULT-NEXT:         write<vector<i64, 16>>(%[[VALUE_g3]], read<vector<i64, 16>>(%[[VALUE_c]]));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i64>(read<i64>(lane(%[[VALUE_g1]], const<i32>(0))), widen<i64, reason=usual_arith>(const<i32>(5))), ne<i64>(read<i64>(lane(%[[VALUE_g1]], const<i32>(1))), widen<i64, reason=usual_arith>(const<i32>(0)))), ne<i64>(read<i64>(lane(%[[VALUE_g3]], const<i32>(0))), widen<i64, reason=usual_arith>(const<i32>(7)))), ne<i64>(read<i64>(lane(%[[VALUE_g3]], const<i32>(1))), widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
