/* PR middle-end/88074 */
/* { dg-do compile } */
/* { dg-options "-O2 -fdump-tree-optimized" } */
/* { dg-add-options float128 } */
/* { dg-require-effective-target float128 } */
/* { dg-final { scan-tree-dump-not "link_error " "optimized" } } */

extern void link_error (void);
int
main ()
{
  if (((__FLT128_MAX__ * 0.5 + __FLT128_MAX__ * 0.5i)
       / (__FLT128_MAX__ * 0.25 + __FLT128_MAX__ * 0.25i))
      != (_Complex _Float128) 2)
    link_error ();
  return 0;
}

// SLATE-FILECHECK-STD DEFAULT gnu23
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
// DEFAULT-NEXT:     fn %[[VALUE_link_error:[0-9]+]] @link_error() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<complex<f128>, exceptions=observable>(div<complex<f128>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(add<complex<f128>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(mul<f128, rounding=nearest_even, exceptions=observable, contract=fast>(const<f128>(1.18973149535723176508575932662800702E+4932), float_widen<f128, reason=usual_arith>(const<f64>(0.5))), mul<complex<f128>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(const<f128>(1.18973149535723176508575932662800702E+4932), complex_convert<complex<f128>, reason=usual_arith>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(0.5))))), add<complex<f128>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(mul<f128, rounding=nearest_even, exceptions=observable, contract=fast>(const<f128>(1.18973149535723176508575932662800702E+4932), float_widen<f128, reason=usual_arith>(const<f64>(0.25))), mul<complex<f128>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(const<f128>(1.18973149535723176508575932662800702E+4932), complex_convert<complex<f128>, reason=usual_arith>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(0.25)))))), real_to_complex<complex<f128>, reason=explicit>(int_to_float<f128, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
