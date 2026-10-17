int eval_method = __FLT_EVAL_METHOD__;
int eval_method_ts = __FLT_EVAL_METHOD_TS_18661_3__;
int iec_559 = __GCC_IEC_559;

#ifdef __FAST_MATH__
int fast_math = __FAST_MATH__;
#else
int fast_math = -1;
#endif

// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-PREFIX-ARGS DEFAULT
// SLATE-FILECHECK-DEFINES FAST
// SLATE-FILECHECK-PREFIX-ARGS FAST -ffast-math
// SLATE-FILECHECK-DEFINES SNAN
// SLATE-FILECHECK-PREFIX-ARGS SNAN -ffast-math -fsignaling-nans
// SLATE-FILECHECK-DEFINES STANDARD
// SLATE-FILECHECK-PREFIX-ARGS STANDARD -ffast-math -fexcess-precision=standard
// SLATE-FILECHECK-DEFINES ISOFAST
// SLATE-FILECHECK-PREFIX-ARGS ISOFAST -std=c11 -fexcess-precision=fast

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "i686-unknown-linux-gnu" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=4, align=4];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f80;
// DEFAULT-NEXT:         storage bool [size=1, align=1];
// DEFAULT-NEXT:         storage i8, u8 [size=1, align=1];
// DEFAULT-NEXT:         storage i16, u16 [size=2, align=2];
// DEFAULT-NEXT:         storage i32, u32 [size=4, align=4];
// DEFAULT-NEXT:         storage i64, u64 [size=8, align=4];
// DEFAULT-NEXT:         storage i128, u128 [size=16, align=16];
// DEFAULT-NEXT:         storage bf16 [size=2, align=2];
// DEFAULT-NEXT:         storage f16 [size=2, align=2];
// DEFAULT-NEXT:         storage f32 [size=4, align=4];
// DEFAULT-NEXT:         storage f64 [size=8, align=4];
// DEFAULT-NEXT:         storage f80 [size=12, align=4];
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     global %[[VALUE_eval_method:[0-9]+]] eval_method: i32 [storage=static] = const<i32>(2) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_eval_method_ts:[0-9]+]] eval_method_ts: i32 [storage=static] = const<i32>(2) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_iec_559:[0-9]+]] iec_559: i32 [storage=static] = const<i32>(2) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_fast_math:[0-9]+]] fast_math: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN FAST
// FAST: module {
// FAST-NEXT:     target "i686-unknown-linux-gnu" {
// FAST-NEXT:         endian = little;
// FAST-NEXT:         pointer [size=4, align=4];
// FAST-NEXT:         stack_alignment = 16;
// FAST-NEXT:         long_double = f80;
// FAST-NEXT:         storage bool [size=1, align=1];
// FAST-NEXT:         storage i8, u8 [size=1, align=1];
// FAST-NEXT:         storage i16, u16 [size=2, align=2];
// FAST-NEXT:         storage i32, u32 [size=4, align=4];
// FAST-NEXT:         storage i64, u64 [size=8, align=4];
// FAST-NEXT:         storage i128, u128 [size=16, align=16];
// FAST-NEXT:         storage bf16 [size=2, align=2];
// FAST-NEXT:         storage f16 [size=2, align=2];
// FAST-NEXT:         storage f32 [size=4, align=4];
// FAST-NEXT:         storage f64 [size=8, align=4];
// FAST-NEXT:         storage f80 [size=12, align=4];
// FAST-NEXT:         storage f128 [size=16, align=16];
// FAST-NEXT:         storage d32 [size=4, align=4];
// FAST-NEXT:         storage d64 [size=8, align=8];
// FAST-NEXT:         storage d128 [size=16, align=16];
// FAST-NEXT:     }
// FAST-NEXT:     global %[[VALUE_eval_method:[0-9]+]] eval_method: i32 [storage=static] = const<i32>(0) [linkage=external];
// FAST-NEXT:     global %[[VALUE_eval_method_ts:[0-9]+]] eval_method_ts: i32 [storage=static] = const<i32>(0) [linkage=external];
// FAST-NEXT:     global %[[VALUE_iec_559:[0-9]+]] iec_559: i32 [storage=static] = const<i32>(0) [linkage=external];
// FAST-NEXT:     global %[[VALUE_fast_math:[0-9]+]] fast_math: i32 [storage=static] = const<i32>(1) [linkage=external];
// FAST-NEXT: }
// SLATE-FILECHECK-END FAST
// SLATE-FILECHECK-BEGIN SNAN
// SNAN: module {
// SNAN-NEXT:     target "i686-unknown-linux-gnu" {
// SNAN-NEXT:         endian = little;
// SNAN-NEXT:         pointer [size=4, align=4];
// SNAN-NEXT:         stack_alignment = 16;
// SNAN-NEXT:         long_double = f80;
// SNAN-NEXT:         storage bool [size=1, align=1];
// SNAN-NEXT:         storage i8, u8 [size=1, align=1];
// SNAN-NEXT:         storage i16, u16 [size=2, align=2];
// SNAN-NEXT:         storage i32, u32 [size=4, align=4];
// SNAN-NEXT:         storage i64, u64 [size=8, align=4];
// SNAN-NEXT:         storage i128, u128 [size=16, align=16];
// SNAN-NEXT:         storage bf16 [size=2, align=2];
// SNAN-NEXT:         storage f16 [size=2, align=2];
// SNAN-NEXT:         storage f32 [size=4, align=4];
// SNAN-NEXT:         storage f64 [size=8, align=4];
// SNAN-NEXT:         storage f80 [size=12, align=4];
// SNAN-NEXT:         storage f128 [size=16, align=16];
// SNAN-NEXT:         storage d32 [size=4, align=4];
// SNAN-NEXT:         storage d64 [size=8, align=8];
// SNAN-NEXT:         storage d128 [size=16, align=16];
// SNAN-NEXT:     }
// SNAN-NEXT:     global %[[VALUE_eval_method:[0-9]+]] eval_method: i32 [storage=static] = const<i32>(0) [linkage=external];
// SNAN-NEXT:     global %[[VALUE_eval_method_ts:[0-9]+]] eval_method_ts: i32 [storage=static] = const<i32>(0) [linkage=external];
// SNAN-NEXT:     global %[[VALUE_iec_559:[0-9]+]] iec_559: i32 [storage=static] = const<i32>(0) [linkage=external];
// SNAN-NEXT:     global %[[VALUE_fast_math:[0-9]+]] fast_math: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// SNAN-NEXT: }
// SLATE-FILECHECK-END SNAN
// SLATE-FILECHECK-BEGIN STANDARD
// STANDARD: module {
// STANDARD-NEXT:     target "i686-unknown-linux-gnu" {
// STANDARD-NEXT:         endian = little;
// STANDARD-NEXT:         pointer [size=4, align=4];
// STANDARD-NEXT:         stack_alignment = 16;
// STANDARD-NEXT:         long_double = f80;
// STANDARD-NEXT:         storage bool [size=1, align=1];
// STANDARD-NEXT:         storage i8, u8 [size=1, align=1];
// STANDARD-NEXT:         storage i16, u16 [size=2, align=2];
// STANDARD-NEXT:         storage i32, u32 [size=4, align=4];
// STANDARD-NEXT:         storage i64, u64 [size=8, align=4];
// STANDARD-NEXT:         storage i128, u128 [size=16, align=16];
// STANDARD-NEXT:         storage bf16 [size=2, align=2];
// STANDARD-NEXT:         storage f16 [size=2, align=2];
// STANDARD-NEXT:         storage f32 [size=4, align=4];
// STANDARD-NEXT:         storage f64 [size=8, align=4];
// STANDARD-NEXT:         storage f80 [size=12, align=4];
// STANDARD-NEXT:         storage f128 [size=16, align=16];
// STANDARD-NEXT:         storage d32 [size=4, align=4];
// STANDARD-NEXT:         storage d64 [size=8, align=8];
// STANDARD-NEXT:         storage d128 [size=16, align=16];
// STANDARD-NEXT:     }
// STANDARD-NEXT:     global %[[VALUE_eval_method:[0-9]+]] eval_method: i32 [storage=static] = const<i32>(2) [linkage=external];
// STANDARD-NEXT:     global %[[VALUE_eval_method_ts:[0-9]+]] eval_method_ts: i32 [storage=static] = const<i32>(2) [linkage=external];
// STANDARD-NEXT:     global %[[VALUE_iec_559:[0-9]+]] iec_559: i32 [storage=static] = const<i32>(0) [linkage=external];
// STANDARD-NEXT:     global %[[VALUE_fast_math:[0-9]+]] fast_math: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// STANDARD-NEXT: }
// SLATE-FILECHECK-END STANDARD
// SLATE-FILECHECK-BEGIN ISOFAST
// ISOFAST: module {
// ISOFAST-NEXT:     target "i686-unknown-linux-gnu" {
// ISOFAST-NEXT:         endian = little;
// ISOFAST-NEXT:         pointer [size=4, align=4];
// ISOFAST-NEXT:         stack_alignment = 16;
// ISOFAST-NEXT:         long_double = f80;
// ISOFAST-NEXT:         storage bool [size=1, align=1];
// ISOFAST-NEXT:         storage i8, u8 [size=1, align=1];
// ISOFAST-NEXT:         storage i16, u16 [size=2, align=2];
// ISOFAST-NEXT:         storage i32, u32 [size=4, align=4];
// ISOFAST-NEXT:         storage i64, u64 [size=8, align=4];
// ISOFAST-NEXT:         storage i128, u128 [size=16, align=16];
// ISOFAST-NEXT:         storage bf16 [size=2, align=2];
// ISOFAST-NEXT:         storage f16 [size=2, align=2];
// ISOFAST-NEXT:         storage f32 [size=4, align=4];
// ISOFAST-NEXT:         storage f64 [size=8, align=4];
// ISOFAST-NEXT:         storage f80 [size=12, align=4];
// ISOFAST-NEXT:         storage f128 [size=16, align=16];
// ISOFAST-NEXT:         storage d32 [size=4, align=4];
// ISOFAST-NEXT:         storage d64 [size=8, align=8];
// ISOFAST-NEXT:         storage d128 [size=16, align=16];
// ISOFAST-NEXT:     }
// ISOFAST-NEXT:     global %[[VALUE_eval_method:[0-9]+]] eval_method: i32 [storage=static] = const<i32>(2) [linkage=external];
// ISOFAST-NEXT:     global %[[VALUE_eval_method_ts:[0-9]+]] eval_method_ts: i32 [storage=static] = const<i32>(2) [linkage=external];
// ISOFAST-NEXT:     global %[[VALUE_iec_559:[0-9]+]] iec_559: i32 [storage=static] = const<i32>(0) [linkage=external];
// ISOFAST-NEXT:     global %[[VALUE_fast_math:[0-9]+]] fast_math: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// ISOFAST-NEXT: }
// SLATE-FILECHECK-END ISOFAST
