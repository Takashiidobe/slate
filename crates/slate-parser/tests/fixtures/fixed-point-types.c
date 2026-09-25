_Fract fract_value;
short _Fract short_fract_value;
long _Fract long_fract_value;
_Accum accum_value;
long _Accum long_accum_value;
_Sat _Fract saturated_fract_value;
_Sat long _Accum saturated_long_accum_value;
unsigned _Fract unsigned_fract_value;
unsigned short _Accum unsigned_short_accum_value;
signed long long _Fract signed_long_long_fract_value;
_Sat unsigned long _Accum saturated_unsigned_long_accum_value;

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
// DEFAULT-NEXT:     global %0 fract_value: fixed<i16, 15> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1 short_fract_value: fixed<i8, 7> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 long_fract_value: fixed<i32, 31> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 accum_value: fixed<i32, 15> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 long_accum_value: fixed<i64, 31> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %5 saturated_fract_value: sat_fixed<i16, 15> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %6 saturated_long_accum_value: sat_fixed<i64, 31> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %7 unsigned_fract_value: fixed<u16, 16> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %8 unsigned_short_accum_value: fixed<u16, 8> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %9 signed_long_long_fract_value: fixed<i64, 63> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %10 saturated_unsigned_long_accum_value: sat_fixed<u64, 32> [storage=static] [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
