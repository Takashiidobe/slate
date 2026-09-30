__float128 add(__float128 a, __float128 b) { return a + b; }

int main(void) {
  __float128 one  = 1.0Q;
  __float128 tiny = 0x1p-100Q;
  __float128 sum  = add(one, tiny);
  if (sum == one)
    return 1;
  if (sum - one != tiny)
    return 2;
  if ((__float128)42 != 42.0Q)
    return 3;
  if ((int)42.75Q != 42)
    return 4;
  if ((double)1.5Q != 1.5)
    return 5;
  if (sizeof(__float128) != 16)
    return 6;
  if (_Alignof(__float128) != 16)
    return 7;
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
// DEFAULT-NEXT:     fn %[[VALUE_add:[0-9]+]] @add(%[[VALUE_a:[0-9]+]] a: f128, %[[VALUE_b:[0-9]+]] b: f128) -> f128 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<f128, rounding=nearest_even, exceptions=ignore, contract=on>(read<f128>(%[[VALUE_a]]), read<f128>(%[[VALUE_b]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_one:[0-9]+]] one: f128 [storage=automatic] = const<f128>(1);
// DEFAULT-NEXT:         let %[[VALUE_tiny:[0-9]+]] tiny: f128 [storage=automatic] = const<f128>(7.88860905221011805411728565282786229E-31);
// DEFAULT-NEXT:         let %[[VALUE_sum:[0-9]+]] sum: f128 [storage=automatic] = call<f128, signature=fn(f128, f128) -> f128>(%[[VALUE_add]], read<f128>(%[[VALUE_one]]), read<f128>(%[[VALUE_tiny]]));
// DEFAULT-NEXT:         if eq<f128, exceptions=ignore>(read<f128>(%[[VALUE_sum]]), read<f128>(%[[VALUE_one]]))
// DEFAULT-NEXT:             return const<i32>(1);
// DEFAULT-NEXT:         if ne<f128, exceptions=ignore>(sub<f128, rounding=nearest_even, exceptions=ignore, contract=on>(read<f128>(%[[VALUE_sum]]), read<f128>(%[[VALUE_one]])), read<f128>(%[[VALUE_tiny]]))
// DEFAULT-NEXT:             return const<i32>(2);
// DEFAULT-NEXT:         if ne<f128, exceptions=ignore>(int_to_float<f128, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(42)), const<f128>(42))
// DEFAULT-NEXT:             return const<i32>(3);
// DEFAULT-NEXT:         if ne<i32>(float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f128>(42.75)), const<i32>(42))
// DEFAULT-NEXT:             return const<i32>(4);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(float_narrow<f64, reason=explicit, rounding=nearest_even, exceptions=ignore>(const<f128>(1.5)), const<f64>(1.5))
// DEFAULT-NEXT:             return const<i32>(5);
// DEFAULT-NEXT:         if ne<u64>(const<u64>(16), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(16))))
// DEFAULT-NEXT:             return const<i32>(6);
// DEFAULT-NEXT:         if ne<u64>(const<u64>(16), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(16))))
// DEFAULT-NEXT:             return const<i32>(7);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
