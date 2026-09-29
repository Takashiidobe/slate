/* PR c/49644 */

extern void abort(void);

int main() {
  _Complex double a[12], *c = a, s = 3.0 + 1.0i;
  double          b[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}, *d = b;
  int             i;
  for (i = 0; i < 6; i++)
    *c++ = *d++ * s;
  if (c != a + 6 || d != b + 6)
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
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: array<complex<f64>, 12> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_c:[0-9]+]] c: ptr<complex<f64>> [storage=automatic] = array_decay<ptr<complex<f64>>, length=Some(12)>(%[[VALUE_a]]);
// DEFAULT-NEXT:         let %[[VALUE_s:[0-9]+]] s: complex<f64> [storage=automatic] = add<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(const<f64>(3.0), aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(1.0)));
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: array<f64, 12> [storage=automatic] [align=16] = aggregate<array<f64, 12>, zero_fill=false>(index0 = int_to_float<f64, reason=assign,
// DEFAULT-SAME: exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), index1 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2)), index2 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(3)), index3 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(4)), index4 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(5)), index5 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(6)), index6 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(7)), index7 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(8)), index8 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(9)), index9 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(10)), index10 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(11)), index11 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(12)));
// DEFAULT-NEXT:         let %[[VALUE_d:[0-9]+]] d: ptr<f64> [storage=automatic] = array_decay<ptr<f64>, length=Some(12)>(%[[VALUE_b]]);
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(6))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: ptr<f64> [synthetic] = read<ptr<f64>>(%[[VALUE_d]]);
// DEFAULT-NEXT:                 let %[[VALUE4:[0-9]+]]: ptr<f64> [synthetic] = ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%[[VALUE3]]), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<f64>>(%[[VALUE_d]], read<ptr<f64>>(%[[VALUE4]]));
// DEFAULT-NEXT:                 let %[[VALUE5:[0-9]+]]: ptr<complex<f64>> [synthetic] = read<ptr<complex<f64>>>(%[[VALUE_c]]);
// DEFAULT-NEXT:                 let %[[VALUE6:[0-9]+]]: ptr<complex<f64>> [synthetic] = ptr_offset<ptr<complex<f64>>, subtract=false, element=complex<f64>, overflow=ub>(read<ptr<complex<f64>>>(%[[VALUE5]]), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<complex<f64>>>(%[[VALUE_c]], read<ptr<complex<f64>>>(%[[VALUE6]]));
// DEFAULT-NEXT:                 write<complex<f64>>(deref(read<ptr<complex<f64>>>(%[[VALUE5]])), mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<f64>(deref(read<ptr<f64>>(%[[VALUE3]]))), read<complex<f64>>(%[[VALUE_s]])));
// DEFAULT-NEXT:         if logical_or<bool>(ne<ptr<complex<f64>>>(read<ptr<complex<f64>>>(%[[VALUE_c]]), ptr_offset<ptr<complex<f64>>, subtract=false, element=complex<f64>, overflow=ub>(array_decay<ptr<complex<f64>>, length=Some(12)>(%[[VALUE_a]]), const<i32>(6))), ne<ptr<f64>>(read<ptr<f64>>(%[[VALUE_d]]), ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(12)>(%[[VALUE_b]]), const<i32>(6))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
