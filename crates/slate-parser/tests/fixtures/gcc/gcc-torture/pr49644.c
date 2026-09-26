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
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %2 a: array<complex<f64>, 12> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %3 c: ptr<complex<f64>> [storage=automatic] = array_decay<ptr<complex<f64>>, length=Some(12)>(%2);
// DEFAULT-NEXT:         let %4 s: complex<f64> [storage=automatic] = add<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(const<f64>(3.0), aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(1.0)));
// DEFAULT-NEXT:         let %5 b: array<f64, 12> [storage=automatic] [align=16] = aggregate<array<f64, 12>, zero_fill=false>(index0 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), index1 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), index2 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(3)), index3 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(4)), index4 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(5)), index5 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(6)), index6 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(7)), index7 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(8)), index8 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(9)), index9 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(10)), index10 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(11)), index11 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(12)));
// DEFAULT-NEXT:         let %6 d: ptr<f64> [storage=automatic] = array_decay<ptr<f64>, length=Some(12)>(%5);
// DEFAULT-NEXT:         let %7 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %8
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%7, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%7), const<i32>(6))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %9: i32 [synthetic] = read<i32>(%7);
// DEFAULT-NEXT:                 let %10: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%9), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%7, read<i32>(%10));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %11: ptr<f64> [synthetic] = read<ptr<f64>>(%6);
// DEFAULT-NEXT:                 let %12: ptr<f64> [synthetic] = ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%11), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<f64>>(%6, read<ptr<f64>>(%12));
// DEFAULT-NEXT:                 let %13: ptr<complex<f64>> [synthetic] = read<ptr<complex<f64>>>(%3);
// DEFAULT-NEXT:                 let %14: ptr<complex<f64>> [synthetic] = ptr_offset<ptr<complex<f64>>, subtract=false, element=complex<f64>, overflow=ub>(read<ptr<complex<f64>>>(%13), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<complex<f64>>>(%3, read<ptr<complex<f64>>>(%14));
// DEFAULT-NEXT:                 write<complex<f64>>(deref(read<ptr<complex<f64>>>(%13)), mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<f64>(deref(read<ptr<f64>>(%11))), read<complex<f64>>(%4)));
// DEFAULT-NEXT:         if logical_or<bool>(ne<ptr<complex<f64>>>(read<ptr<complex<f64>>>(%3), ptr_offset<ptr<complex<f64>>, subtract=false, element=complex<f64>, overflow=ub>(array_decay<ptr<complex<f64>>, length=Some(12)>(%2), const<i32>(6))), ne<ptr<f64>>(read<ptr<f64>>(%6), ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(12)>(%5), const<i32>(6))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
