static int Sub(int a, int b) { return b - a; }

static unsigned Select(unsigned a, unsigned b, unsigned c) {
  const int pa_minus_pb = Sub((a >> 8) & 0xff, (b >> 8) & 0xff) +
                          Sub((a >> 0) & 0xff, (b >> 0) & 0xff);
  return (pa_minus_pb <= 0) ? a : b;
}

__attribute__((noinline)) unsigned Predictor(unsigned              left,
                                             const unsigned *const top) {
  const unsigned pred = Select(top[1], left, top[0]);
  return pred;
}

int main(void) {
  const unsigned top[2] = {0xff7a7a7a, 0xff7a7a7a};
  const unsigned left   = 0xff7b7b7b;
  const unsigned pred   = Predictor(left, top /*+ 1*/);
  if (pred == left)
    return 0;
  return 1;
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
// DEFAULT-NEXT:     fn %[[VALUE_Sub:[0-9]+]] @Sub(%[[VALUE_a:[0-9]+]] a: i32, %[[VALUE_b:[0-9]+]] b: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return sub<i32, overflow=ub>(read<i32>(%[[VALUE_b]]), read<i32>(%[[VALUE_a]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_Select:[0-9]+]] @Select(%[[VALUE_a_2:[0-9]+]] a: u32, %[[VALUE_b_2:[0-9]+]] b: u32, %[[VALUE_c:[0-9]+]] c: u32) -> u32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_pa_minus_pb:[0-9]+]] pa_minus_pb: i32 [storage=automatic] [const] = add<i32, overflow=ub>(call<i32, signature=fn(i32, i32) ->
// DEFAULT-SAME: i32>(%[[VALUE_Sub]], reinterpret<i32, reason=arg, fits=unknown>(and<u32>(shr<u32, amount_out_of_range=ub,
// DEFAULT-SAME: fill=zero_extend>(read<u32>(%[[VALUE_a_2]]), const<i32>(8)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(255)))), reinterpret<i32, reason=arg,
// DEFAULT-SAME: fits=unknown>(and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%[[VALUE_b_2]]), const<i32>(8)), reinterpret<u32, reason=usual_arith,
// DEFAULT-SAME: fits=always>(const<i32>(255))))), call<i32, signature=fn(i32, i32) -> i32>(%[[VALUE_Sub]], reinterpret<i32, reason=arg, fits=unknown>(and<u32>(shr<u32,
// DEFAULT-SAME: amount_out_of_range=ub, fill=zero_extend>(read<u32>(%[[VALUE_a_2]]), const<i32>(0)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(255)))),
// DEFAULT-SAME: reinterpret<i32, reason=arg, fits=unknown>(and<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%[[VALUE_b_2]]), const<i32>(0)),
// DEFAULT-SAME: reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(255))))));
// DEFAULT-NEXT:         return conditional<u32>(le<i32>(read<i32>(%[[VALUE_pa_minus_pb]]), const<i32>(0)), read<u32>(%[[VALUE_a_2]]), read<u32>(%[[VALUE_b_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_Predictor:[0-9]+]] @Predictor(%[[VALUE_left:[0-9]+]] left: u32, %[[VALUE_top:[0-9]+]] top: ptr<const u32> [const]) -> u32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_pred:[0-9]+]] pred: u32 [storage=automatic] [const] = call<u32, signature=fn(u32, u32, u32) -> u32>(%[[VALUE_Select]], read<u32>(deref(ptr_offset<ptr<const u32>, subtract=false, element=u32, overflow=ub>(read<ptr<const u32>>(%[[VALUE_top]]), const<i32>(1)))), read<u32>(%[[VALUE_left]]), read<u32>(deref(ptr_offset<ptr<const u32>, subtract=false, element=u32, overflow=ub>(read<ptr<const u32>>(%[[VALUE_top]]), const<i32>(0)))));
// DEFAULT-NEXT:         return read<u32>(%[[VALUE_pred]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_top_2:[0-9]+]] top: array<u32, 2> [storage=automatic] [const] = aggregate<array<u32, 2>, zero_fill=false>(index0 = const<u32>(4286216826), index1 = const<u32>(4286216826));
// DEFAULT-NEXT:         let %[[VALUE_left_2:[0-9]+]] left: u32 [storage=automatic] [const] = const<u32>(4286282619);
// DEFAULT-NEXT:         let %[[VALUE_pred_2:[0-9]+]] pred: u32 [storage=automatic] [const] = call<u32, signature=fn(u32, ptr<const u32>) -> u32>(%[[VALUE_Predictor]], read<u32>(%[[VALUE_left_2]]), array_decay<ptr<const u32>, length=Some(2)>(%[[VALUE_top_2]]));
// DEFAULT-NEXT:         if eq<u32>(read<u32>(%[[VALUE_pred_2]]), read<u32>(%[[VALUE_left_2]]))
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:         return const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
