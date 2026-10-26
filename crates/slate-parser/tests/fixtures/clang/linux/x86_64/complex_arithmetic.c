// SLATE-FILECHECK-ARGS --dump-ir
// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-PREFIX-ARGS DEFAULT
// SLATE-FILECHECK-DEFINES BASIC
// SLATE-FILECHECK-PREFIX-ARGS BASIC -fcomplex-arithmetic=basic
// SLATE-FILECHECK-DEFINES IMPROVED
// SLATE-FILECHECK-PREFIX-ARGS IMPROVED -fcomplex-arithmetic=improved
// SLATE-FILECHECK-DEFINES PROMOTED
// SLATE-FILECHECK-WARNING PROMOTED
// SLATE-FILECHECK-PREFIX-ARGS PROMOTED -fcomplex-arithmetic=promoted
// SLATE-FILECHECK-DEFINES LIMITED
// SLATE-FILECHECK-PREFIX-ARGS LIMITED -fcomplex-arithmetic=improved -fcx-limited-range
// SLATE-FILECHECK-DEFINES FORTRAN
// SLATE-FILECHECK-PREFIX-ARGS FORTRAN -fcx-fortran-rules
// SLATE-FILECHECK-DEFINES FAST_MATH
// SLATE-FILECHECK-PREFIX-ARGS FAST_MATH -fcomplex-arithmetic=full -ffast-math
// SLATE-FILECHECK-DEFINES OVERRIDE
// SLATE-FILECHECK-PREFIX-ARGS OVERRIDE -ffast-math -fcomplex-arithmetic=improved
// SLATE-FILECHECK-DEFINES RESET
// SLATE-FILECHECK-PREFIX-ARGS RESET -fcomplex-arithmetic=improved -fno-fast-math

double _Complex folded = (1.0 + 2.0i) / (3.0 + 4.0i);

double _Complex divide(double _Complex a, double _Complex b) { return a / b; }

double _Complex multiply(double _Complex a, double _Complex b) { return a * b; }

float _Complex divide_float(float _Complex a, float _Complex b) { return a / b; }

long double _Complex divide_long(long double _Complex a, long double _Complex b) {
  a /= b;
  return a / b;
}

double _Complex restored(double _Complex a, double _Complex b) {
#pragma STDC CX_LIMITED_RANGE OFF
  {
#pragma STDC CX_LIMITED_RANGE DEFAULT
    return a / b;
  }
}

// SLATE-FILECHECK-BEGIN PROMOTED
// PROMOTED: -Whigher-precision-for-complex-division
// PROMOTED:   ⚠ excess precision is requested but the target does not support excess
// PROMOTED:   │ precision which may result in observable differences in complex division
// PROMOTED:   │ behavior, additional uses where the requested higher precision cannot be
// PROMOTED:   │ honored were found but not diagnosed
// PROMOTED:     ╭─[tests/fixtures/clang/linux/x86_64/complex_arithmetic.c:11:3]
// PROMOTED:  10 │ long double _Complex divide_long(long double _Complex a, long double _Complex b) {
// PROMOTED:  11 │   a /= b;
// PROMOTED:     ·   ──────
// PROMOTED:  12 │   return a / b;
// PROMOTED:     ╰────
// SLATE-FILECHECK-END PROMOTED
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
// DEFAULT-NEXT:     global %[[VALUE_folded:[0-9]+]] folded: complex<f64> [storage=static] = div<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(add<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(const<f64>(1.0), aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(2.0))), add<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(const<f64>(3.0), aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(4.0)))) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_divide:[0-9]+]] @divide(%[[VALUE_a:[0-9]+]] a: complex<f64>, %[[VALUE_b:[0-9]+]] b: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c, native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return div<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f64>>(%[[VALUE_a]]), read<complex<f64>>(%[[VALUE_b]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_multiply:[0-9]+]] @multiply(%[[VALUE_a_2:[0-9]+]] a: complex<f64>, %[[VALUE_b_2:[0-9]+]] b: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c, native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f64>>(%[[VALUE_a_2]]), read<complex<f64>>(%[[VALUE_b_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_divide_float:[0-9]+]] @divide_float(%[[VALUE_a_3:[0-9]+]] a: complex<f32>, %[[VALUE_b_3:[0-9]+]] b: complex<f32>) -> complex<f32> [linkage=external] [abi=sysv64(native_c, native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return div<complex<f32>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f32>>(%[[VALUE_a_3]]), read<complex<f32>>(%[[VALUE_b_3]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_divide_long:[0-9]+]] @divide_long(%[[VALUE_a_4:[0-9]+]] a: complex<f80>, %[[VALUE_b_4:[0-9]+]] b: complex<f80>) -> complex<f80> [linkage=external] [abi=sysv64(byval<align=16>, byval<align=16>) -> coerce<f80, f80>] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: complex<f80> [synthetic] = read<complex<f80>>(%[[VALUE_a_4]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: complex<f80> [synthetic] = div<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f80>>(%[[VALUE0]]), read<complex<f80>>(%[[VALUE_b_4]]));
// DEFAULT-NEXT:         write<complex<f80>>(%[[VALUE_a_4]], read<complex<f80>>(%[[VALUE1]]));
// DEFAULT-NEXT:         return div<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f80>>(%[[VALUE_a_4]]), read<complex<f80>>(%[[VALUE_b_4]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_restored:[0-9]+]] @restored(%[[VALUE_a_5:[0-9]+]] a: complex<f64>, %[[VALUE_b_5:[0-9]+]] b: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c, native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             return div<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f64>>(%[[VALUE_a_5]]), read<complex<f64>>(%[[VALUE_b_5]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN BASIC
// BASIC: module {
// BASIC-NEXT:     target "x86_64-unknown-linux-gnu" {
// BASIC-NEXT:         endian = little;
// BASIC-NEXT:         pointer [size=8, align=8];
// BASIC-NEXT:         stack_alignment = 16;
// BASIC-NEXT:         long_double = f80;
// BASIC-NEXT:         storage bool [size=1, align=1];
// BASIC-NEXT:         storage i8, u8 [size=1, align=1];
// BASIC-NEXT:         storage i16, u16 [size=2, align=2];
// BASIC-NEXT:         storage i32, u32 [size=4, align=4];
// BASIC-NEXT:         storage i64, u64 [size=8, align=8];
// BASIC-NEXT:         storage i128, u128 [size=16, align=16];
// BASIC-NEXT:         storage bf16 [size=2, align=2];
// BASIC-NEXT:         storage f16 [size=2, align=2];
// BASIC-NEXT:         storage f32 [size=4, align=4];
// BASIC-NEXT:         storage f64 [size=8, align=8];
// BASIC-NEXT:         storage f80 [size=16, align=16];
// BASIC-NEXT:         storage f128 [size=16, align=16];
// BASIC-NEXT:         storage d32 [size=4, align=4];
// BASIC-NEXT:         storage d64 [size=8, align=8];
// BASIC-NEXT:         storage d128 [size=16, align=16];
// BASIC-NEXT:     }
// BASIC-NEXT:     global %[[VALUE_folded:[0-9]+]] folded: complex<f64> [storage=static] = div<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(add<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(const<f64>(1.0), aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(2.0))), add<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(const<f64>(3.0), aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(4.0)))) [linkage=external];
// BASIC-NEXT:     fn %[[VALUE_divide:[0-9]+]] @divide(%[[VALUE_a:[0-9]+]] a: complex<f64>, %[[VALUE_b:[0-9]+]] b: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c, native_c) -> native_c] [fallthrough=ub_if_used] {
// BASIC-NEXT:         return div<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=basic>(read<complex<f64>>(%[[VALUE_a]]), read<complex<f64>>(%[[VALUE_b]]));
// BASIC-NEXT:     }
// BASIC-NEXT:     fn %[[VALUE_multiply:[0-9]+]] @multiply(%[[VALUE_a_2:[0-9]+]] a: complex<f64>, %[[VALUE_b_2:[0-9]+]] b: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c, native_c) -> native_c] [fallthrough=ub_if_used] {
// BASIC-NEXT:         return mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=basic>(read<complex<f64>>(%[[VALUE_a_2]]), read<complex<f64>>(%[[VALUE_b_2]]));
// BASIC-NEXT:     }
// BASIC-NEXT:     fn %[[VALUE_divide_float:[0-9]+]] @divide_float(%[[VALUE_a_3:[0-9]+]] a: complex<f32>, %[[VALUE_b_3:[0-9]+]] b: complex<f32>) -> complex<f32> [linkage=external] [abi=sysv64(native_c, native_c) -> native_c] [fallthrough=ub_if_used] {
// BASIC-NEXT:         return div<complex<f32>, complex=true, rounding=nearest_even, exceptions=ignore, range=basic>(read<complex<f32>>(%[[VALUE_a_3]]), read<complex<f32>>(%[[VALUE_b_3]]));
// BASIC-NEXT:     }
// BASIC-NEXT:     fn %[[VALUE_divide_long:[0-9]+]] @divide_long(%[[VALUE_a_4:[0-9]+]] a: complex<f80>, %[[VALUE_b_4:[0-9]+]] b: complex<f80>) -> complex<f80> [linkage=external] [abi=sysv64(byval<align=16>, byval<align=16>) -> coerce<f80, f80>] [fallthrough=ub_if_used] {
// BASIC-NEXT:         let %[[VALUE0:[0-9]+]]: complex<f80> [synthetic] = read<complex<f80>>(%[[VALUE_a_4]]);
// BASIC-NEXT:         let %[[VALUE1:[0-9]+]]: complex<f80> [synthetic] = div<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=basic>(read<complex<f80>>(%[[VALUE0]]), read<complex<f80>>(%[[VALUE_b_4]]));
// BASIC-NEXT:         write<complex<f80>>(%[[VALUE_a_4]], read<complex<f80>>(%[[VALUE1]]));
// BASIC-NEXT:         return div<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=basic>(read<complex<f80>>(%[[VALUE_a_4]]), read<complex<f80>>(%[[VALUE_b_4]]));
// BASIC-NEXT:     }
// BASIC-NEXT:     fn %[[VALUE_restored:[0-9]+]] @restored(%[[VALUE_a_5:[0-9]+]] a: complex<f64>, %[[VALUE_b_5:[0-9]+]] b: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c, native_c) -> native_c] [fallthrough=ub_if_used] {
// BASIC-NEXT:         {
// BASIC-NEXT:             return div<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=basic>(read<complex<f64>>(%[[VALUE_a_5]]), read<complex<f64>>(%[[VALUE_b_5]]));
// BASIC-NEXT:         }
// BASIC-NEXT:     }
// BASIC-NEXT: }
// SLATE-FILECHECK-END BASIC
// SLATE-FILECHECK-BEGIN IMPROVED
// IMPROVED: module {
// IMPROVED-NEXT:     target "x86_64-unknown-linux-gnu" {
// IMPROVED-NEXT:         endian = little;
// IMPROVED-NEXT:         pointer [size=8, align=8];
// IMPROVED-NEXT:         stack_alignment = 16;
// IMPROVED-NEXT:         long_double = f80;
// IMPROVED-NEXT:         storage bool [size=1, align=1];
// IMPROVED-NEXT:         storage i8, u8 [size=1, align=1];
// IMPROVED-NEXT:         storage i16, u16 [size=2, align=2];
// IMPROVED-NEXT:         storage i32, u32 [size=4, align=4];
// IMPROVED-NEXT:         storage i64, u64 [size=8, align=8];
// IMPROVED-NEXT:         storage i128, u128 [size=16, align=16];
// IMPROVED-NEXT:         storage bf16 [size=2, align=2];
// IMPROVED-NEXT:         storage f16 [size=2, align=2];
// IMPROVED-NEXT:         storage f32 [size=4, align=4];
// IMPROVED-NEXT:         storage f64 [size=8, align=8];
// IMPROVED-NEXT:         storage f80 [size=16, align=16];
// IMPROVED-NEXT:         storage f128 [size=16, align=16];
// IMPROVED-NEXT:         storage d32 [size=4, align=4];
// IMPROVED-NEXT:         storage d64 [size=8, align=8];
// IMPROVED-NEXT:         storage d128 [size=16, align=16];
// IMPROVED-NEXT:     }
// IMPROVED-NEXT:     global %[[VALUE_folded:[0-9]+]] folded: complex<f64> [storage=static] = div<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(add<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(const<f64>(1.0), aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(2.0))), add<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(const<f64>(3.0), aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(4.0)))) [linkage=external];
// IMPROVED-NEXT:     fn %[[VALUE_divide:[0-9]+]] @divide(%[[VALUE_a:[0-9]+]] a: complex<f64>, %[[VALUE_b:[0-9]+]] b: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c, native_c) -> native_c] [fallthrough=ub_if_used] {
// IMPROVED-NEXT:         return div<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=improved>(read<complex<f64>>(%[[VALUE_a]]), read<complex<f64>>(%[[VALUE_b]]));
// IMPROVED-NEXT:     }
// IMPROVED-NEXT:     fn %[[VALUE_multiply:[0-9]+]] @multiply(%[[VALUE_a_2:[0-9]+]] a: complex<f64>, %[[VALUE_b_2:[0-9]+]] b: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c, native_c) -> native_c] [fallthrough=ub_if_used] {
// IMPROVED-NEXT:         return mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=improved>(read<complex<f64>>(%[[VALUE_a_2]]), read<complex<f64>>(%[[VALUE_b_2]]));
// IMPROVED-NEXT:     }
// IMPROVED-NEXT:     fn %[[VALUE_divide_float:[0-9]+]] @divide_float(%[[VALUE_a_3:[0-9]+]] a: complex<f32>, %[[VALUE_b_3:[0-9]+]] b: complex<f32>) -> complex<f32> [linkage=external] [abi=sysv64(native_c, native_c) -> native_c] [fallthrough=ub_if_used] {
// IMPROVED-NEXT:         return div<complex<f32>, complex=true, rounding=nearest_even, exceptions=ignore, range=improved>(read<complex<f32>>(%[[VALUE_a_3]]), read<complex<f32>>(%[[VALUE_b_3]]));
// IMPROVED-NEXT:     }
// IMPROVED-NEXT:     fn %[[VALUE_divide_long:[0-9]+]] @divide_long(%[[VALUE_a_4:[0-9]+]] a: complex<f80>, %[[VALUE_b_4:[0-9]+]] b: complex<f80>) -> complex<f80> [linkage=external] [abi=sysv64(byval<align=16>, byval<align=16>) -> coerce<f80, f80>] [fallthrough=ub_if_used] {
// IMPROVED-NEXT:         let %[[VALUE0:[0-9]+]]: complex<f80> [synthetic] = read<complex<f80>>(%[[VALUE_a_4]]);
// IMPROVED-NEXT:         let %[[VALUE1:[0-9]+]]: complex<f80> [synthetic] = div<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=improved>(read<complex<f80>>(%[[VALUE0]]), read<complex<f80>>(%[[VALUE_b_4]]));
// IMPROVED-NEXT:         write<complex<f80>>(%[[VALUE_a_4]], read<complex<f80>>(%[[VALUE1]]));
// IMPROVED-NEXT:         return div<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=improved>(read<complex<f80>>(%[[VALUE_a_4]]), read<complex<f80>>(%[[VALUE_b_4]]));
// IMPROVED-NEXT:     }
// IMPROVED-NEXT:     fn %[[VALUE_restored:[0-9]+]] @restored(%[[VALUE_a_5:[0-9]+]] a: complex<f64>, %[[VALUE_b_5:[0-9]+]] b: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c, native_c) -> native_c] [fallthrough=ub_if_used] {
// IMPROVED-NEXT:         {
// IMPROVED-NEXT:             return div<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=improved>(read<complex<f64>>(%[[VALUE_a_5]]), read<complex<f64>>(%[[VALUE_b_5]]));
// IMPROVED-NEXT:         }
// IMPROVED-NEXT:     }
// IMPROVED-NEXT: }
// SLATE-FILECHECK-END IMPROVED
// SLATE-FILECHECK-BEGIN IR-PROMOTED
// IR-PROMOTED: module {
// IR-PROMOTED-NEXT:     target "x86_64-unknown-linux-gnu" {
// IR-PROMOTED-NEXT:         endian = little;
// IR-PROMOTED-NEXT:         pointer [size=8, align=8];
// IR-PROMOTED-NEXT:         stack_alignment = 16;
// IR-PROMOTED-NEXT:         long_double = f80;
// IR-PROMOTED-NEXT:         storage bool [size=1, align=1];
// IR-PROMOTED-NEXT:         storage i8, u8 [size=1, align=1];
// IR-PROMOTED-NEXT:         storage i16, u16 [size=2, align=2];
// IR-PROMOTED-NEXT:         storage i32, u32 [size=4, align=4];
// IR-PROMOTED-NEXT:         storage i64, u64 [size=8, align=8];
// IR-PROMOTED-NEXT:         storage i128, u128 [size=16, align=16];
// IR-PROMOTED-NEXT:         storage bf16 [size=2, align=2];
// IR-PROMOTED-NEXT:         storage f16 [size=2, align=2];
// IR-PROMOTED-NEXT:         storage f32 [size=4, align=4];
// IR-PROMOTED-NEXT:         storage f64 [size=8, align=8];
// IR-PROMOTED-NEXT:         storage f80 [size=16, align=16];
// IR-PROMOTED-NEXT:         storage f128 [size=16, align=16];
// IR-PROMOTED-NEXT:         storage d32 [size=4, align=4];
// IR-PROMOTED-NEXT:         storage d64 [size=8, align=8];
// IR-PROMOTED-NEXT:         storage d128 [size=16, align=16];
// IR-PROMOTED-NEXT:     }
// IR-PROMOTED-NEXT:     global %[[VALUE_folded:[0-9]+]] folded: complex<f64> [storage=static] = div<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(add<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(const<f64>(1.0), aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(2.0))), add<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(const<f64>(3.0), aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(4.0)))) [linkage=external];
// IR-PROMOTED-NEXT:     fn %[[VALUE_divide:[0-9]+]] @divide(%[[VALUE_a:[0-9]+]] a: complex<f64>, %[[VALUE_b:[0-9]+]] b: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c, native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-PROMOTED-NEXT:         return complex_convert<complex<f64>, reason=excess_precision, rounding=nearest_even, exceptions=ignore>(div<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=basic>(complex_convert<complex<f80>, reason=excess_precision>(read<complex<f64>>(%[[VALUE_a]])), complex_convert<complex<f80>, reason=excess_precision>(read<complex<f64>>(%[[VALUE_b]]))));
// IR-PROMOTED-NEXT:     }
// IR-PROMOTED-NEXT:     fn %[[VALUE_multiply:[0-9]+]] @multiply(%[[VALUE_a_2:[0-9]+]] a: complex<f64>, %[[VALUE_b_2:[0-9]+]] b: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c, native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-PROMOTED-NEXT:         return mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=basic>(read<complex<f64>>(%[[VALUE_a_2]]), read<complex<f64>>(%[[VALUE_b_2]]));
// IR-PROMOTED-NEXT:     }
// IR-PROMOTED-NEXT:     fn %[[VALUE_divide_float:[0-9]+]] @divide_float(%[[VALUE_a_3:[0-9]+]] a: complex<f32>, %[[VALUE_b_3:[0-9]+]] b: complex<f32>) -> complex<f32> [linkage=external] [abi=sysv64(native_c, native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-PROMOTED-NEXT:         return complex_convert<complex<f32>, reason=excess_precision, rounding=nearest_even, exceptions=ignore>(div<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=basic>(complex_convert<complex<f64>, reason=excess_precision>(read<complex<f32>>(%[[VALUE_a_3]])), complex_convert<complex<f64>, reason=excess_precision>(read<complex<f32>>(%[[VALUE_b_3]]))));
// IR-PROMOTED-NEXT:     }
// IR-PROMOTED-NEXT:     fn %[[VALUE_divide_long:[0-9]+]] @divide_long(%[[VALUE_a_4:[0-9]+]] a: complex<f80>, %[[VALUE_b_4:[0-9]+]] b: complex<f80>) -> complex<f80> [linkage=external] [abi=sysv64(byval<align=16>, byval<align=16>) -> coerce<f80, f80>] [fallthrough=ub_if_used] {
// IR-PROMOTED-NEXT:         let %[[VALUE0:[0-9]+]]: complex<f80> [synthetic] = read<complex<f80>>(%[[VALUE_a_4]]);
// IR-PROMOTED-NEXT:         let %[[VALUE1:[0-9]+]]: complex<f80> [synthetic] = div<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=improved>(read<complex<f80>>(%[[VALUE0]]), read<complex<f80>>(%[[VALUE_b_4]]));
// IR-PROMOTED-NEXT:         write<complex<f80>>(%[[VALUE_a_4]], read<complex<f80>>(%[[VALUE1]]));
// IR-PROMOTED-NEXT:         return div<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=improved>(read<complex<f80>>(%[[VALUE_a_4]]), read<complex<f80>>(%[[VALUE_b_4]]));
// IR-PROMOTED-NEXT:     }
// IR-PROMOTED-NEXT:     fn %[[VALUE_restored:[0-9]+]] @restored(%[[VALUE_a_5:[0-9]+]] a: complex<f64>, %[[VALUE_b_5:[0-9]+]] b: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c, native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-PROMOTED-NEXT:         {
// IR-PROMOTED-NEXT:             return complex_convert<complex<f64>, reason=excess_precision, rounding=nearest_even, exceptions=ignore>(div<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=basic>(complex_convert<complex<f80>, reason=excess_precision>(read<complex<f64>>(%[[VALUE_a_5]])), complex_convert<complex<f80>, reason=excess_precision>(read<complex<f64>>(%[[VALUE_b_5]]))));
// IR-PROMOTED-NEXT:         }
// IR-PROMOTED-NEXT:     }
// IR-PROMOTED-NEXT: }
// SLATE-FILECHECK-END IR-PROMOTED
// SLATE-FILECHECK-BEGIN LIMITED
// LIMITED: module {
// LIMITED-NEXT:     target "x86_64-unknown-linux-gnu" {
// LIMITED-NEXT:         endian = little;
// LIMITED-NEXT:         pointer [size=8, align=8];
// LIMITED-NEXT:         stack_alignment = 16;
// LIMITED-NEXT:         long_double = f80;
// LIMITED-NEXT:         storage bool [size=1, align=1];
// LIMITED-NEXT:         storage i8, u8 [size=1, align=1];
// LIMITED-NEXT:         storage i16, u16 [size=2, align=2];
// LIMITED-NEXT:         storage i32, u32 [size=4, align=4];
// LIMITED-NEXT:         storage i64, u64 [size=8, align=8];
// LIMITED-NEXT:         storage i128, u128 [size=16, align=16];
// LIMITED-NEXT:         storage bf16 [size=2, align=2];
// LIMITED-NEXT:         storage f16 [size=2, align=2];
// LIMITED-NEXT:         storage f32 [size=4, align=4];
// LIMITED-NEXT:         storage f64 [size=8, align=8];
// LIMITED-NEXT:         storage f80 [size=16, align=16];
// LIMITED-NEXT:         storage f128 [size=16, align=16];
// LIMITED-NEXT:         storage d32 [size=4, align=4];
// LIMITED-NEXT:         storage d64 [size=8, align=8];
// LIMITED-NEXT:         storage d128 [size=16, align=16];
// LIMITED-NEXT:     }
// LIMITED-NEXT:     global %[[VALUE_folded:[0-9]+]] folded: complex<f64> [storage=static] = div<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(add<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(const<f64>(1.0), aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(2.0))), add<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(const<f64>(3.0), aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(4.0)))) [linkage=external];
// LIMITED-NEXT:     fn %[[VALUE_divide:[0-9]+]] @divide(%[[VALUE_a:[0-9]+]] a: complex<f64>, %[[VALUE_b:[0-9]+]] b: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c, native_c) -> native_c] [fallthrough=ub_if_used] {
// LIMITED-NEXT:         return div<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=basic>(read<complex<f64>>(%[[VALUE_a]]), read<complex<f64>>(%[[VALUE_b]]));
// LIMITED-NEXT:     }
// LIMITED-NEXT:     fn %[[VALUE_multiply:[0-9]+]] @multiply(%[[VALUE_a_2:[0-9]+]] a: complex<f64>, %[[VALUE_b_2:[0-9]+]] b: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c, native_c) -> native_c] [fallthrough=ub_if_used] {
// LIMITED-NEXT:         return mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=basic>(read<complex<f64>>(%[[VALUE_a_2]]), read<complex<f64>>(%[[VALUE_b_2]]));
// LIMITED-NEXT:     }
// LIMITED-NEXT:     fn %[[VALUE_divide_float:[0-9]+]] @divide_float(%[[VALUE_a_3:[0-9]+]] a: complex<f32>, %[[VALUE_b_3:[0-9]+]] b: complex<f32>) -> complex<f32> [linkage=external] [abi=sysv64(native_c, native_c) -> native_c] [fallthrough=ub_if_used] {
// LIMITED-NEXT:         return div<complex<f32>, complex=true, rounding=nearest_even, exceptions=ignore, range=basic>(read<complex<f32>>(%[[VALUE_a_3]]), read<complex<f32>>(%[[VALUE_b_3]]));
// LIMITED-NEXT:     }
// LIMITED-NEXT:     fn %[[VALUE_divide_long:[0-9]+]] @divide_long(%[[VALUE_a_4:[0-9]+]] a: complex<f80>, %[[VALUE_b_4:[0-9]+]] b: complex<f80>) -> complex<f80> [linkage=external] [abi=sysv64(byval<align=16>, byval<align=16>) -> coerce<f80, f80>] [fallthrough=ub_if_used] {
// LIMITED-NEXT:         let %[[VALUE0:[0-9]+]]: complex<f80> [synthetic] = read<complex<f80>>(%[[VALUE_a_4]]);
// LIMITED-NEXT:         let %[[VALUE1:[0-9]+]]: complex<f80> [synthetic] = div<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=basic>(read<complex<f80>>(%[[VALUE0]]), read<complex<f80>>(%[[VALUE_b_4]]));
// LIMITED-NEXT:         write<complex<f80>>(%[[VALUE_a_4]], read<complex<f80>>(%[[VALUE1]]));
// LIMITED-NEXT:         return div<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=basic>(read<complex<f80>>(%[[VALUE_a_4]]), read<complex<f80>>(%[[VALUE_b_4]]));
// LIMITED-NEXT:     }
// LIMITED-NEXT:     fn %[[VALUE_restored:[0-9]+]] @restored(%[[VALUE_a_5:[0-9]+]] a: complex<f64>, %[[VALUE_b_5:[0-9]+]] b: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c, native_c) -> native_c] [fallthrough=ub_if_used] {
// LIMITED-NEXT:         {
// LIMITED-NEXT:             return div<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=basic>(read<complex<f64>>(%[[VALUE_a_5]]), read<complex<f64>>(%[[VALUE_b_5]]));
// LIMITED-NEXT:         }
// LIMITED-NEXT:     }
// LIMITED-NEXT: }
// SLATE-FILECHECK-END LIMITED
// SLATE-FILECHECK-BEGIN FORTRAN
// FORTRAN: module {
// FORTRAN-NEXT:     target "x86_64-unknown-linux-gnu" {
// FORTRAN-NEXT:         endian = little;
// FORTRAN-NEXT:         pointer [size=8, align=8];
// FORTRAN-NEXT:         stack_alignment = 16;
// FORTRAN-NEXT:         long_double = f80;
// FORTRAN-NEXT:         storage bool [size=1, align=1];
// FORTRAN-NEXT:         storage i8, u8 [size=1, align=1];
// FORTRAN-NEXT:         storage i16, u16 [size=2, align=2];
// FORTRAN-NEXT:         storage i32, u32 [size=4, align=4];
// FORTRAN-NEXT:         storage i64, u64 [size=8, align=8];
// FORTRAN-NEXT:         storage i128, u128 [size=16, align=16];
// FORTRAN-NEXT:         storage bf16 [size=2, align=2];
// FORTRAN-NEXT:         storage f16 [size=2, align=2];
// FORTRAN-NEXT:         storage f32 [size=4, align=4];
// FORTRAN-NEXT:         storage f64 [size=8, align=8];
// FORTRAN-NEXT:         storage f80 [size=16, align=16];
// FORTRAN-NEXT:         storage f128 [size=16, align=16];
// FORTRAN-NEXT:         storage d32 [size=4, align=4];
// FORTRAN-NEXT:         storage d64 [size=8, align=8];
// FORTRAN-NEXT:         storage d128 [size=16, align=16];
// FORTRAN-NEXT:     }
// FORTRAN-NEXT:     global %[[VALUE_folded:[0-9]+]] folded: complex<f64> [storage=static] = div<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(add<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(const<f64>(1.0), aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(2.0))), add<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(const<f64>(3.0), aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(4.0)))) [linkage=external];
// FORTRAN-NEXT:     fn %[[VALUE_divide:[0-9]+]] @divide(%[[VALUE_a:[0-9]+]] a: complex<f64>, %[[VALUE_b:[0-9]+]] b: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c, native_c) -> native_c] [fallthrough=ub_if_used] {
// FORTRAN-NEXT:         return div<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=improved>(read<complex<f64>>(%[[VALUE_a]]), read<complex<f64>>(%[[VALUE_b]]));
// FORTRAN-NEXT:     }
// FORTRAN-NEXT:     fn %[[VALUE_multiply:[0-9]+]] @multiply(%[[VALUE_a_2:[0-9]+]] a: complex<f64>, %[[VALUE_b_2:[0-9]+]] b: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c, native_c) -> native_c] [fallthrough=ub_if_used] {
// FORTRAN-NEXT:         return mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=improved>(read<complex<f64>>(%[[VALUE_a_2]]), read<complex<f64>>(%[[VALUE_b_2]]));
// FORTRAN-NEXT:     }
// FORTRAN-NEXT:     fn %[[VALUE_divide_float:[0-9]+]] @divide_float(%[[VALUE_a_3:[0-9]+]] a: complex<f32>, %[[VALUE_b_3:[0-9]+]] b: complex<f32>) -> complex<f32> [linkage=external] [abi=sysv64(native_c, native_c) -> native_c] [fallthrough=ub_if_used] {
// FORTRAN-NEXT:         return div<complex<f32>, complex=true, rounding=nearest_even, exceptions=ignore, range=improved>(read<complex<f32>>(%[[VALUE_a_3]]), read<complex<f32>>(%[[VALUE_b_3]]));
// FORTRAN-NEXT:     }
// FORTRAN-NEXT:     fn %[[VALUE_divide_long:[0-9]+]] @divide_long(%[[VALUE_a_4:[0-9]+]] a: complex<f80>, %[[VALUE_b_4:[0-9]+]] b: complex<f80>) -> complex<f80> [linkage=external] [abi=sysv64(byval<align=16>, byval<align=16>) -> coerce<f80, f80>] [fallthrough=ub_if_used] {
// FORTRAN-NEXT:         let %[[VALUE0:[0-9]+]]: complex<f80> [synthetic] = read<complex<f80>>(%[[VALUE_a_4]]);
// FORTRAN-NEXT:         let %[[VALUE1:[0-9]+]]: complex<f80> [synthetic] = div<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=improved>(read<complex<f80>>(%[[VALUE0]]), read<complex<f80>>(%[[VALUE_b_4]]));
// FORTRAN-NEXT:         write<complex<f80>>(%[[VALUE_a_4]], read<complex<f80>>(%[[VALUE1]]));
// FORTRAN-NEXT:         return div<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=improved>(read<complex<f80>>(%[[VALUE_a_4]]), read<complex<f80>>(%[[VALUE_b_4]]));
// FORTRAN-NEXT:     }
// FORTRAN-NEXT:     fn %[[VALUE_restored:[0-9]+]] @restored(%[[VALUE_a_5:[0-9]+]] a: complex<f64>, %[[VALUE_b_5:[0-9]+]] b: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c, native_c) -> native_c] [fallthrough=ub_if_used] {
// FORTRAN-NEXT:         {
// FORTRAN-NEXT:             return div<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=improved>(read<complex<f64>>(%[[VALUE_a_5]]), read<complex<f64>>(%[[VALUE_b_5]]));
// FORTRAN-NEXT:         }
// FORTRAN-NEXT:     }
// FORTRAN-NEXT: }
// SLATE-FILECHECK-END FORTRAN
// SLATE-FILECHECK-BEGIN FAST_MATH
// FAST_MATH: module {
// FAST_MATH-NEXT:     target "x86_64-unknown-linux-gnu" {
// FAST_MATH-NEXT:         endian = little;
// FAST_MATH-NEXT:         pointer [size=8, align=8];
// FAST_MATH-NEXT:         stack_alignment = 16;
// FAST_MATH-NEXT:         long_double = f80;
// FAST_MATH-NEXT:         storage bool [size=1, align=1];
// FAST_MATH-NEXT:         storage i8, u8 [size=1, align=1];
// FAST_MATH-NEXT:         storage i16, u16 [size=2, align=2];
// FAST_MATH-NEXT:         storage i32, u32 [size=4, align=4];
// FAST_MATH-NEXT:         storage i64, u64 [size=8, align=8];
// FAST_MATH-NEXT:         storage i128, u128 [size=16, align=16];
// FAST_MATH-NEXT:         storage bf16 [size=2, align=2];
// FAST_MATH-NEXT:         storage f16 [size=2, align=2];
// FAST_MATH-NEXT:         storage f32 [size=4, align=4];
// FAST_MATH-NEXT:         storage f64 [size=8, align=8];
// FAST_MATH-NEXT:         storage f80 [size=16, align=16];
// FAST_MATH-NEXT:         storage f128 [size=16, align=16];
// FAST_MATH-NEXT:         storage d32 [size=4, align=4];
// FAST_MATH-NEXT:         storage d64 [size=8, align=8];
// FAST_MATH-NEXT:         storage d128 [size=16, align=16];
// FAST_MATH-NEXT:     }
// FAST_MATH-NEXT:     global %[[VALUE_folded:[0-9]+]] folded: complex<f64> [storage=static] = div<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(add<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(const<f64>(1.0), aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(2.0))), add<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(const<f64>(3.0), aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(4.0)))) [linkage=external];
// FAST_MATH-NEXT:     fn %[[VALUE_divide:[0-9]+]] @divide(%[[VALUE_a:[0-9]+]] a: complex<f64>, %[[VALUE_b:[0-9]+]] b: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c, native_c) -> native_c] [fallthrough=ub_if_used] {
// FAST_MATH-NEXT:         return div<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=basic>(read<complex<f64>>(%[[VALUE_a]]), read<complex<f64>>(%[[VALUE_b]]));
// FAST_MATH-NEXT:     }
// FAST_MATH-NEXT:     fn %[[VALUE_multiply:[0-9]+]] @multiply(%[[VALUE_a_2:[0-9]+]] a: complex<f64>, %[[VALUE_b_2:[0-9]+]] b: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c, native_c) -> native_c] [fallthrough=ub_if_used] {
// FAST_MATH-NEXT:         return mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=basic>(read<complex<f64>>(%[[VALUE_a_2]]), read<complex<f64>>(%[[VALUE_b_2]]));
// FAST_MATH-NEXT:     }
// FAST_MATH-NEXT:     fn %[[VALUE_divide_float:[0-9]+]] @divide_float(%[[VALUE_a_3:[0-9]+]] a: complex<f32>, %[[VALUE_b_3:[0-9]+]] b: complex<f32>) -> complex<f32> [linkage=external] [abi=sysv64(native_c, native_c) -> native_c] [fallthrough=ub_if_used] {
// FAST_MATH-NEXT:         return div<complex<f32>, complex=true, rounding=nearest_even, exceptions=ignore, range=basic>(read<complex<f32>>(%[[VALUE_a_3]]), read<complex<f32>>(%[[VALUE_b_3]]));
// FAST_MATH-NEXT:     }
// FAST_MATH-NEXT:     fn %[[VALUE_divide_long:[0-9]+]] @divide_long(%[[VALUE_a_4:[0-9]+]] a: complex<f80>, %[[VALUE_b_4:[0-9]+]] b: complex<f80>) -> complex<f80> [linkage=external] [abi=sysv64(byval<align=16>, byval<align=16>) -> coerce<f80, f80>] [fallthrough=ub_if_used] {
// FAST_MATH-NEXT:         let %[[VALUE0:[0-9]+]]: complex<f80> [synthetic] = read<complex<f80>>(%[[VALUE_a_4]]);
// FAST_MATH-NEXT:         let %[[VALUE1:[0-9]+]]: complex<f80> [synthetic] = div<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=basic>(read<complex<f80>>(%[[VALUE0]]), read<complex<f80>>(%[[VALUE_b_4]]));
// FAST_MATH-NEXT:         write<complex<f80>>(%[[VALUE_a_4]], read<complex<f80>>(%[[VALUE1]]));
// FAST_MATH-NEXT:         return div<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=basic>(read<complex<f80>>(%[[VALUE_a_4]]), read<complex<f80>>(%[[VALUE_b_4]]));
// FAST_MATH-NEXT:     }
// FAST_MATH-NEXT:     fn %[[VALUE_restored:[0-9]+]] @restored(%[[VALUE_a_5:[0-9]+]] a: complex<f64>, %[[VALUE_b_5:[0-9]+]] b: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c, native_c) -> native_c] [fallthrough=ub_if_used] {
// FAST_MATH-NEXT:         {
// FAST_MATH-NEXT:             return div<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=basic>(read<complex<f64>>(%[[VALUE_a_5]]), read<complex<f64>>(%[[VALUE_b_5]]));
// FAST_MATH-NEXT:         }
// FAST_MATH-NEXT:     }
// FAST_MATH-NEXT: }
// SLATE-FILECHECK-END FAST_MATH
// SLATE-FILECHECK-BEGIN OVERRIDE
// OVERRIDE: module {
// OVERRIDE-NEXT:     target "x86_64-unknown-linux-gnu" {
// OVERRIDE-NEXT:         endian = little;
// OVERRIDE-NEXT:         pointer [size=8, align=8];
// OVERRIDE-NEXT:         stack_alignment = 16;
// OVERRIDE-NEXT:         long_double = f80;
// OVERRIDE-NEXT:         storage bool [size=1, align=1];
// OVERRIDE-NEXT:         storage i8, u8 [size=1, align=1];
// OVERRIDE-NEXT:         storage i16, u16 [size=2, align=2];
// OVERRIDE-NEXT:         storage i32, u32 [size=4, align=4];
// OVERRIDE-NEXT:         storage i64, u64 [size=8, align=8];
// OVERRIDE-NEXT:         storage i128, u128 [size=16, align=16];
// OVERRIDE-NEXT:         storage bf16 [size=2, align=2];
// OVERRIDE-NEXT:         storage f16 [size=2, align=2];
// OVERRIDE-NEXT:         storage f32 [size=4, align=4];
// OVERRIDE-NEXT:         storage f64 [size=8, align=8];
// OVERRIDE-NEXT:         storage f80 [size=16, align=16];
// OVERRIDE-NEXT:         storage f128 [size=16, align=16];
// OVERRIDE-NEXT:         storage d32 [size=4, align=4];
// OVERRIDE-NEXT:         storage d64 [size=8, align=8];
// OVERRIDE-NEXT:         storage d128 [size=16, align=16];
// OVERRIDE-NEXT:     }
// OVERRIDE-NEXT:     global %[[VALUE_folded:[0-9]+]] folded: complex<f64> [storage=static] = div<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(add<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(const<f64>(1.0), aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(2.0))), add<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(const<f64>(3.0), aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(4.0)))) [linkage=external];
// OVERRIDE-NEXT:     fn %[[VALUE_divide:[0-9]+]] @divide(%[[VALUE_a:[0-9]+]] a: complex<f64>, %[[VALUE_b:[0-9]+]] b: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c, native_c) -> native_c] [fallthrough=ub_if_used] {
// OVERRIDE-NEXT:         return div<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=improved>(read<complex<f64>>(%[[VALUE_a]]), read<complex<f64>>(%[[VALUE_b]]));
// OVERRIDE-NEXT:     }
// OVERRIDE-NEXT:     fn %[[VALUE_multiply:[0-9]+]] @multiply(%[[VALUE_a_2:[0-9]+]] a: complex<f64>, %[[VALUE_b_2:[0-9]+]] b: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c, native_c) -> native_c] [fallthrough=ub_if_used] {
// OVERRIDE-NEXT:         return mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=improved>(read<complex<f64>>(%[[VALUE_a_2]]), read<complex<f64>>(%[[VALUE_b_2]]));
// OVERRIDE-NEXT:     }
// OVERRIDE-NEXT:     fn %[[VALUE_divide_float:[0-9]+]] @divide_float(%[[VALUE_a_3:[0-9]+]] a: complex<f32>, %[[VALUE_b_3:[0-9]+]] b: complex<f32>) -> complex<f32> [linkage=external] [abi=sysv64(native_c, native_c) -> native_c] [fallthrough=ub_if_used] {
// OVERRIDE-NEXT:         return div<complex<f32>, complex=true, rounding=nearest_even, exceptions=ignore, range=improved>(read<complex<f32>>(%[[VALUE_a_3]]), read<complex<f32>>(%[[VALUE_b_3]]));
// OVERRIDE-NEXT:     }
// OVERRIDE-NEXT:     fn %[[VALUE_divide_long:[0-9]+]] @divide_long(%[[VALUE_a_4:[0-9]+]] a: complex<f80>, %[[VALUE_b_4:[0-9]+]] b: complex<f80>) -> complex<f80> [linkage=external] [abi=sysv64(byval<align=16>, byval<align=16>) -> coerce<f80, f80>] [fallthrough=ub_if_used] {
// OVERRIDE-NEXT:         let %[[VALUE0:[0-9]+]]: complex<f80> [synthetic] = read<complex<f80>>(%[[VALUE_a_4]]);
// OVERRIDE-NEXT:         let %[[VALUE1:[0-9]+]]: complex<f80> [synthetic] = div<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=improved>(read<complex<f80>>(%[[VALUE0]]), read<complex<f80>>(%[[VALUE_b_4]]));
// OVERRIDE-NEXT:         write<complex<f80>>(%[[VALUE_a_4]], read<complex<f80>>(%[[VALUE1]]));
// OVERRIDE-NEXT:         return div<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=improved>(read<complex<f80>>(%[[VALUE_a_4]]), read<complex<f80>>(%[[VALUE_b_4]]));
// OVERRIDE-NEXT:     }
// OVERRIDE-NEXT:     fn %[[VALUE_restored:[0-9]+]] @restored(%[[VALUE_a_5:[0-9]+]] a: complex<f64>, %[[VALUE_b_5:[0-9]+]] b: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c, native_c) -> native_c] [fallthrough=ub_if_used] {
// OVERRIDE-NEXT:         {
// OVERRIDE-NEXT:             return div<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=improved>(read<complex<f64>>(%[[VALUE_a_5]]), read<complex<f64>>(%[[VALUE_b_5]]));
// OVERRIDE-NEXT:         }
// OVERRIDE-NEXT:     }
// OVERRIDE-NEXT: }
// SLATE-FILECHECK-END OVERRIDE
// SLATE-FILECHECK-BEGIN RESET
// RESET: module {
// RESET-NEXT:     target "x86_64-unknown-linux-gnu" {
// RESET-NEXT:         endian = little;
// RESET-NEXT:         pointer [size=8, align=8];
// RESET-NEXT:         stack_alignment = 16;
// RESET-NEXT:         long_double = f80;
// RESET-NEXT:         storage bool [size=1, align=1];
// RESET-NEXT:         storage i8, u8 [size=1, align=1];
// RESET-NEXT:         storage i16, u16 [size=2, align=2];
// RESET-NEXT:         storage i32, u32 [size=4, align=4];
// RESET-NEXT:         storage i64, u64 [size=8, align=8];
// RESET-NEXT:         storage i128, u128 [size=16, align=16];
// RESET-NEXT:         storage bf16 [size=2, align=2];
// RESET-NEXT:         storage f16 [size=2, align=2];
// RESET-NEXT:         storage f32 [size=4, align=4];
// RESET-NEXT:         storage f64 [size=8, align=8];
// RESET-NEXT:         storage f80 [size=16, align=16];
// RESET-NEXT:         storage f128 [size=16, align=16];
// RESET-NEXT:         storage d32 [size=4, align=4];
// RESET-NEXT:         storage d64 [size=8, align=8];
// RESET-NEXT:         storage d128 [size=16, align=16];
// RESET-NEXT:     }
// RESET-NEXT:     global %[[VALUE_folded:[0-9]+]] folded: complex<f64> [storage=static] = div<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(add<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(const<f64>(1.0), aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(2.0))), add<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(const<f64>(3.0), aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(4.0)))) [linkage=external];
// RESET-NEXT:     fn %[[VALUE_divide:[0-9]+]] @divide(%[[VALUE_a:[0-9]+]] a: complex<f64>, %[[VALUE_b:[0-9]+]] b: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c, native_c) -> native_c] [fallthrough=ub_if_used] {
// RESET-NEXT:         return div<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f64>>(%[[VALUE_a]]), read<complex<f64>>(%[[VALUE_b]]));
// RESET-NEXT:     }
// RESET-NEXT:     fn %[[VALUE_multiply:[0-9]+]] @multiply(%[[VALUE_a_2:[0-9]+]] a: complex<f64>, %[[VALUE_b_2:[0-9]+]] b: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c, native_c) -> native_c] [fallthrough=ub_if_used] {
// RESET-NEXT:         return mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f64>>(%[[VALUE_a_2]]), read<complex<f64>>(%[[VALUE_b_2]]));
// RESET-NEXT:     }
// RESET-NEXT:     fn %[[VALUE_divide_float:[0-9]+]] @divide_float(%[[VALUE_a_3:[0-9]+]] a: complex<f32>, %[[VALUE_b_3:[0-9]+]] b: complex<f32>) -> complex<f32> [linkage=external] [abi=sysv64(native_c, native_c) -> native_c] [fallthrough=ub_if_used] {
// RESET-NEXT:         return div<complex<f32>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f32>>(%[[VALUE_a_3]]), read<complex<f32>>(%[[VALUE_b_3]]));
// RESET-NEXT:     }
// RESET-NEXT:     fn %[[VALUE_divide_long:[0-9]+]] @divide_long(%[[VALUE_a_4:[0-9]+]] a: complex<f80>, %[[VALUE_b_4:[0-9]+]] b: complex<f80>) -> complex<f80> [linkage=external] [abi=sysv64(byval<align=16>, byval<align=16>) -> coerce<f80, f80>] [fallthrough=ub_if_used] {
// RESET-NEXT:         let %[[VALUE0:[0-9]+]]: complex<f80> [synthetic] = read<complex<f80>>(%[[VALUE_a_4]]);
// RESET-NEXT:         let %[[VALUE1:[0-9]+]]: complex<f80> [synthetic] = div<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f80>>(%[[VALUE0]]), read<complex<f80>>(%[[VALUE_b_4]]));
// RESET-NEXT:         write<complex<f80>>(%[[VALUE_a_4]], read<complex<f80>>(%[[VALUE1]]));
// RESET-NEXT:         return div<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f80>>(%[[VALUE_a_4]]), read<complex<f80>>(%[[VALUE_b_4]]));
// RESET-NEXT:     }
// RESET-NEXT:     fn %[[VALUE_restored:[0-9]+]] @restored(%[[VALUE_a_5:[0-9]+]] a: complex<f64>, %[[VALUE_b_5:[0-9]+]] b: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c, native_c) -> native_c] [fallthrough=ub_if_used] {
// RESET-NEXT:         {
// RESET-NEXT:             return div<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f64>>(%[[VALUE_a_5]]), read<complex<f64>>(%[[VALUE_b_5]]));
// RESET-NEXT:         }
// RESET-NEXT:     }
// RESET-NEXT: }
// SLATE-FILECHECK-END RESET
