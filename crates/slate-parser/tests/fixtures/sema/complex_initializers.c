// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir -std=c23

_Complex float pair = {1.0f, 2.0f};
_Complex double mixed = {1, 2.5};
_Complex float single = {3.0f};
_Complex float plain = 4.0f;
struct S { _Complex float c; int k; };
struct S s = {{5.0f, 6.0f}, 7};
_Complex float array[2] = {{1.0f, 2.0f}, {3.0f, 4.0f}};

_Complex float local(float x) {
  _Complex float l = {x, 1.0f};
  return l;
}

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "x86_64-unknown-linux-gnu" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=8, align=8];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f80;
// IR-NEXT:         storage bool [size=1, align=1];
// IR-NEXT:         storage i8, u8 [size=1, align=1];
// IR-NEXT:         storage i16, u16 [size=2, align=2];
// IR-NEXT:         storage i32, u32 [size=4, align=4];
// IR-NEXT:         storage i64, u64 [size=8, align=8];
// IR-NEXT:         storage i128, u128 [size=16, align=16];
// IR-NEXT:         storage bf16 [size=2, align=2];
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=8];
// IR-NEXT:         storage f80 [size=16, align=16];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     type @type0 S = struct {
// IR-NEXT:         field0 c: complex<f32>;
// IR-NEXT:         field1 k: i32;
// IR-NEXT:     } [size=12, align=4, offsets=[0, 8]];
// IR-NEXT:     global %0 pair: complex<f32> [storage=static] = aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(1.0), index1 = const<f32>(2.0)) [linkage=external];
// IR-NEXT:     global %1 mixed: complex<f64> [storage=static] = aggregate<complex<f64>, zero_fill=false>(index0 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), index1 = const<f64>(2.5)) [linkage=external];
// IR-NEXT:     global %2 single: complex<f32> [storage=static] = real_to_complex<complex<f32>, reason=assign>(const<f32>(3.0)) [linkage=external];
// IR-NEXT:     global %3 plain: complex<f32> [storage=static] = real_to_complex<complex<f32>, reason=assign>(const<f32>(4.0)) [linkage=external];
// IR-NEXT:     global %5 s: @type0 [storage=static] = aggregate<@type0, zero_fill=false>(field0 = aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(5.0), index1 = const<f32>(6.0)), field1 = const<i32>(7)) [linkage=external];
// IR-NEXT:     global %6 array: array<complex<f32>, 2> [storage=static] [align=16] = aggregate<array<complex<f32>, 2>, zero_fill=false>(index0 = aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(1.0), index1 = const<f32>(2.0)), index1 = aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(3.0), index1 = const<f32>(4.0))) [linkage=external];
// IR-NEXT:     fn %7 @local(%8 x: f32) -> complex<f32> [linkage=external] [abi=sysv64(scalar) -> coerce<pair<f32>>] [fallthrough=ub_if_used] {
// IR-NEXT:         let %9 l: complex<f32> [storage=automatic] = aggregate<complex<f32>, zero_fill=false>(index0 = read<f32>(%8), index1 = const<f32>(1.0));
// IR-NEXT:         return read<complex<f32>>(%9);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
