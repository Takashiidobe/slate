// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir

_Fract half_fract = 0.5r;
_Accum one_and_half_accum = 1.5k;
unsigned _Fract unsigned_half_fract = 0.5ur;
short _Fract short_half_fract = 0.5hr;
long _Accum long_one = 1.0lk;
unsigned long long _Accum unsigned_long_long = 1.0ullk;

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
// IR-NEXT:     global %0 half_fract: fixed<i16, 15> [storage=static] = const<fixed<i16, 15>>(16384) [linkage=external];
// IR-NEXT:     global %1 one_and_half_accum: fixed<i32, 15> [storage=static] = const<fixed<i32, 15>>(49152) [linkage=external];
// IR-NEXT:     global %2 unsigned_half_fract: fixed<u16, 16> [storage=static] = const<fixed<u16, 16>>(32768) [linkage=external];
// IR-NEXT:     global %3 short_half_fract: fixed<i8, 7> [storage=static] = const<fixed<i8, 7>>(64) [linkage=external];
// IR-NEXT:     global %4 long_one: fixed<i64, 31> [storage=static] = const<fixed<i64, 31>>(2147483648) [linkage=external];
// IR-NEXT:     global %5 unsigned_long_long: fixed<u128, 64> [storage=static] = const<fixed<u128, 64>>(18446744073709551616) [linkage=external];
// IR-NEXT: }
// SLATE-FILECHECK-END IR
