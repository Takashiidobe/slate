/* Test that imaginary constants are diagnosed in C23 mode: -pedantic.  */
/* { dg-do run } */
/* { dg-options "-std=c23 -pedantic" } */
/* { dg-add-options float64x } */
/* { dg-require-effective-target float64x } */

_Complex _Float64x a = 1.if64x;	/* { dg-warning "imaginary constants are a C2Y feature or GCC extension" } */
_Complex _Float64x b = 2.F64xj;	/* { dg-warning "imaginary constants are a C2Y feature or GCC extension" } */
_Complex _Float64x c = 3.f64xi;	/* { dg-warning "imaginary constants are a C2Y feature or GCC extension" } */
_Complex _Float64x d = 4.JF64x;	/* { dg-warning "imaginary constants are a C2Y feature or GCC extension" } */
__extension__ _Complex _Float64x e = 1.if64x;
__extension__ _Complex _Float64x f = 2.F64xj;
__extension__ _Complex _Float64x g = 3.f64xi;
__extension__ _Complex _Float64x h = 4.JF64x;

int
main ()
{
  if (a * a != -1.f64x
      || b * b != -4.f64x
      || c * c != -9.f64x
      || d * d != -16.f64x
      || e * e != -1.f64x
      || f * f != -4.f64x
      || g * g != -9.f64x
      || h * h != -16.f64x)
    __builtin_abort ();
}

// SLATE-FILECHECK-STD DEFAULT c23
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
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: complex<f80> [storage=static] = aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = const<f80>(1)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: complex<f80> [storage=static] = aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = const<f80>(2)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: complex<f80> [storage=static] = aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = const<f80>(3)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: complex<f80> [storage=static] = aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = const<f80>(4)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_e:[0-9]+]] e: complex<f80> [storage=static] = aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = const<f80>(1)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_f:[0-9]+]] f: complex<f80> [storage=static] = aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = const<f80>(2)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g:[0-9]+]] g: complex<f80> [storage=static] = aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = const<f80>(3)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_h:[0-9]+]] h: complex<f80> [storage=static] = aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = const<f80>(4)) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<complex<f80>, exceptions=observable>(mul<complex<f80>, complex=true, rounding=nearest_even, exceptions=observable,
// DEFAULT-SAME: range=full>(read<complex<f80>>(%[[VALUE_a]]),
// DEFAULT-SAME: read<complex<f80>>(%[[VALUE_a]])), neg<f80>(const<f80>(1))), ne<complex<f80>, exceptions=observable>(mul<complex<f80>, complex=true, rounding=nearest_even,
// DEFAULT-SAME: exceptions=observable, range=full>(read<complex<f80>>(%[[VALUE_b]]),
// DEFAULT-SAME: read<complex<f80>>(%[[VALUE_b]])), neg<f80>(const<f80>(4)))), ne<complex<f80>, exceptions=observable>(mul<complex<f80>, complex=true, rounding=nearest_even,
// DEFAULT-SAME: exceptions=observable, range=full>(read<complex<f80>>(%[[VALUE_c]]),
// DEFAULT-SAME: read<complex<f80>>(%[[VALUE_c]])), neg<f80>(const<f80>(9)))), ne<complex<f80>, exceptions=observable>(mul<complex<f80>, complex=true, rounding=nearest_even,
// DEFAULT-SAME: exceptions=observable, range=full>(read<complex<f80>>(%[[VALUE_d]]),
// DEFAULT-SAME: read<complex<f80>>(%[[VALUE_d]])), neg<f80>(const<f80>(16)))), ne<complex<f80>, exceptions=observable>(mul<complex<f80>, complex=true, rounding=nearest_even,
// DEFAULT-SAME: exceptions=observable, range=full>(read<complex<f80>>(%[[VALUE_e]]),
// DEFAULT-SAME: read<complex<f80>>(%[[VALUE_e]])), neg<f80>(const<f80>(1)))), ne<complex<f80>, exceptions=observable>(mul<complex<f80>, complex=true, rounding=nearest_even,
// DEFAULT-SAME: exceptions=observable, range=full>(read<complex<f80>>(%[[VALUE_f]]),
// DEFAULT-SAME: read<complex<f80>>(%[[VALUE_f]])), neg<f80>(const<f80>(4)))), ne<complex<f80>, exceptions=observable>(mul<complex<f80>, complex=true, rounding=nearest_even,
// DEFAULT-SAME: exceptions=observable, range=full>(read<complex<f80>>(%[[VALUE_g]]),
// DEFAULT-SAME: read<complex<f80>>(%[[VALUE_g]])), neg<f80>(const<f80>(9)))), ne<complex<f80>, exceptions=observable>(mul<complex<f80>, complex=true, rounding=nearest_even,
// DEFAULT-SAME: exceptions=observable, range=full>(read<complex<f80>>(%[[VALUE_h]]),
// DEFAULT-SAME: read<complex<f80>>(%[[VALUE_h]])), neg<f80>(const<f80>(16))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
