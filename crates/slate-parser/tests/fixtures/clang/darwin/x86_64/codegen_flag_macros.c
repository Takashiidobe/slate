#ifdef __pic__
int macro_pic = __pic__;
#else
int macro_pic = -1;
#endif

#ifdef __pie__
int macro_pie = __pie__;
#else
int macro_pie = -1;
#endif

#ifdef __SSP__
int macro_ssp = __SSP__;
#else
int macro_ssp = -1;
#endif

#ifdef __SSP_STRONG__
int macro_ssp_strong = __SSP_STRONG__;
#else
int macro_ssp_strong = -1;
#endif

#ifdef __SSP_ALL__
int macro_ssp_all = __SSP_ALL__;
#else
int macro_ssp_all = -1;
#endif

#ifdef __SSP_EXPLICIT__
int macro_ssp_explicit = __SSP_EXPLICIT__;
#else
int macro_ssp_explicit = -1;
#endif

#ifdef __CET__
int macro_cet = __CET__;
#else
int macro_cet = -1;
#endif

#ifdef __FLT_EVAL_METHOD__
int macro_flt_eval_method = __FLT_EVAL_METHOD__;
#else
int macro_flt_eval_method = -1;
#endif

#ifdef __SSE__
int macro_sse = 1;
#else
int macro_sse = -1;
#endif

#ifdef __SSE2__
int macro_sse2 = 1;
#else
int macro_sse2 = -1;
#endif

#ifdef __MMX__
int macro_mmx = 1;
#else
int macro_mmx = -1;
#endif

#ifdef __MMX_WITH_SSE__
int macro_mmx_with_sse = 1;
#else
int macro_mmx_with_sse = -1;
#endif

#ifdef __FLT16_MAX__
int macro_flt16_max = 1;
#else
int macro_flt16_max = -1;
#endif

#ifdef _SOFT_FLOAT
int macro_soft_float = 1;
#else
int macro_soft_float = -1;
#endif

#ifdef __3dNOW__
int macro_3dnow = 1;
#else
int macro_3dnow = -1;
#endif

#ifdef __3dNOW_A__
int macro_3dnow_a = 1;
#else
int macro_3dnow_a = -1;
#endif

// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-PREFIX-ARGS DEFAULT
// SLATE-FILECHECK-DEFINES NOPIC
// SLATE-FILECHECK-PREFIX-ARGS NOPIC -fno-pic
// SLATE-FILECHECK-DEFINES NOSSE
// SLATE-FILECHECK-PREFIX-ARGS NOSSE -mno-sse -mno-mmx

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-apple-darwin" {
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
// DEFAULT-NEXT:     global %[[VALUE_macro_pic:[0-9]+]] macro_pic: i32 [storage=static] = const<i32>(2) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_macro_pie:[0-9]+]] macro_pie: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_macro_ssp:[0-9]+]] macro_ssp: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_macro_ssp_strong:[0-9]+]] macro_ssp_strong: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_macro_ssp_all:[0-9]+]] macro_ssp_all: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_macro_ssp_explicit:[0-9]+]] macro_ssp_explicit: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_macro_cet:[0-9]+]] macro_cet: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_macro_flt_eval_method:[0-9]+]] macro_flt_eval_method: i32 [storage=static] = const<i32>(0) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_macro_sse:[0-9]+]] macro_sse: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_macro_sse2:[0-9]+]] macro_sse2: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_macro_mmx:[0-9]+]] macro_mmx: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_macro_mmx_with_sse:[0-9]+]] macro_mmx_with_sse: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_macro_flt16_max:[0-9]+]] macro_flt16_max: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_macro_soft_float:[0-9]+]] macro_soft_float: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_macro_3dnow:[0-9]+]] macro_3dnow: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_macro_3dnow_a:[0-9]+]] macro_3dnow_a: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN NOPIC
// NOPIC: module {
// NOPIC-NEXT:     target "x86_64-apple-darwin" {
// NOPIC-NEXT:         endian = little;
// NOPIC-NEXT:         pointer [size=8, align=8];
// NOPIC-NEXT:         stack_alignment = 16;
// NOPIC-NEXT:         long_double = f80;
// NOPIC-NEXT:         storage bool [size=1, align=1];
// NOPIC-NEXT:         storage i8, u8 [size=1, align=1];
// NOPIC-NEXT:         storage i16, u16 [size=2, align=2];
// NOPIC-NEXT:         storage i32, u32 [size=4, align=4];
// NOPIC-NEXT:         storage i64, u64 [size=8, align=8];
// NOPIC-NEXT:         storage i128, u128 [size=16, align=16];
// NOPIC-NEXT:         storage bf16 [size=2, align=2];
// NOPIC-NEXT:         storage f16 [size=2, align=2];
// NOPIC-NEXT:         storage f32 [size=4, align=4];
// NOPIC-NEXT:         storage f64 [size=8, align=8];
// NOPIC-NEXT:         storage f80 [size=16, align=16];
// NOPIC-NEXT:         storage f128 [size=16, align=16];
// NOPIC-NEXT:         storage d32 [size=4, align=4];
// NOPIC-NEXT:         storage d64 [size=8, align=8];
// NOPIC-NEXT:         storage d128 [size=16, align=16];
// NOPIC-NEXT:     }
// NOPIC-NEXT:     global %[[VALUE_macro_pic:[0-9]+]] macro_pic: i32 [storage=static] = const<i32>(2) [linkage=external];
// NOPIC-NEXT:     global %[[VALUE_macro_pie:[0-9]+]] macro_pie: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// NOPIC-NEXT:     global %[[VALUE_macro_ssp:[0-9]+]] macro_ssp: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// NOPIC-NEXT:     global %[[VALUE_macro_ssp_strong:[0-9]+]] macro_ssp_strong: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// NOPIC-NEXT:     global %[[VALUE_macro_ssp_all:[0-9]+]] macro_ssp_all: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// NOPIC-NEXT:     global %[[VALUE_macro_ssp_explicit:[0-9]+]] macro_ssp_explicit: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// NOPIC-NEXT:     global %[[VALUE_macro_cet:[0-9]+]] macro_cet: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// NOPIC-NEXT:     global %[[VALUE_macro_flt_eval_method:[0-9]+]] macro_flt_eval_method: i32 [storage=static] = const<i32>(0) [linkage=external];
// NOPIC-NEXT:     global %[[VALUE_macro_sse:[0-9]+]] macro_sse: i32 [storage=static] = const<i32>(1) [linkage=external];
// NOPIC-NEXT:     global %[[VALUE_macro_sse2:[0-9]+]] macro_sse2: i32 [storage=static] = const<i32>(1) [linkage=external];
// NOPIC-NEXT:     global %[[VALUE_macro_mmx:[0-9]+]] macro_mmx: i32 [storage=static] = const<i32>(1) [linkage=external];
// NOPIC-NEXT:     global %[[VALUE_macro_mmx_with_sse:[0-9]+]] macro_mmx_with_sse: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// NOPIC-NEXT:     global %[[VALUE_macro_flt16_max:[0-9]+]] macro_flt16_max: i32 [storage=static] = const<i32>(1) [linkage=external];
// NOPIC-NEXT:     global %[[VALUE_macro_soft_float:[0-9]+]] macro_soft_float: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// NOPIC-NEXT:     global %[[VALUE_macro_3dnow:[0-9]+]] macro_3dnow: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// NOPIC-NEXT:     global %[[VALUE_macro_3dnow_a:[0-9]+]] macro_3dnow_a: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// NOPIC-NEXT: }
// SLATE-FILECHECK-END NOPIC
// SLATE-FILECHECK-BEGIN NOSSE
// NOSSE: module {
// NOSSE-NEXT:     target "x86_64-apple-darwin" {
// NOSSE-NEXT:         endian = little;
// NOSSE-NEXT:         pointer [size=8, align=8];
// NOSSE-NEXT:         stack_alignment = 16;
// NOSSE-NEXT:         long_double = f80;
// NOSSE-NEXT:         storage bool [size=1, align=1];
// NOSSE-NEXT:         storage i8, u8 [size=1, align=1];
// NOSSE-NEXT:         storage i16, u16 [size=2, align=2];
// NOSSE-NEXT:         storage i32, u32 [size=4, align=4];
// NOSSE-NEXT:         storage i64, u64 [size=8, align=8];
// NOSSE-NEXT:         storage i128, u128 [size=16, align=16];
// NOSSE-NEXT:         storage bf16 [size=2, align=2];
// NOSSE-NEXT:         storage f16 [size=2, align=2];
// NOSSE-NEXT:         storage f32 [size=4, align=4];
// NOSSE-NEXT:         storage f64 [size=8, align=8];
// NOSSE-NEXT:         storage f80 [size=16, align=16];
// NOSSE-NEXT:         storage f128 [size=16, align=16];
// NOSSE-NEXT:         storage d32 [size=4, align=4];
// NOSSE-NEXT:         storage d64 [size=8, align=8];
// NOSSE-NEXT:         storage d128 [size=16, align=16];
// NOSSE-NEXT:     }
// NOSSE-NEXT:     global %[[VALUE_macro_pic:[0-9]+]] macro_pic: i32 [storage=static] = const<i32>(2) [linkage=external];
// NOSSE-NEXT:     global %[[VALUE_macro_pie:[0-9]+]] macro_pie: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// NOSSE-NEXT:     global %[[VALUE_macro_ssp:[0-9]+]] macro_ssp: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// NOSSE-NEXT:     global %[[VALUE_macro_ssp_strong:[0-9]+]] macro_ssp_strong: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// NOSSE-NEXT:     global %[[VALUE_macro_ssp_all:[0-9]+]] macro_ssp_all: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// NOSSE-NEXT:     global %[[VALUE_macro_ssp_explicit:[0-9]+]] macro_ssp_explicit: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// NOSSE-NEXT:     global %[[VALUE_macro_cet:[0-9]+]] macro_cet: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// NOSSE-NEXT:     global %[[VALUE_macro_flt_eval_method:[0-9]+]] macro_flt_eval_method: i32 [storage=static] = const<i32>(2) [linkage=external];
// NOSSE-NEXT:     global %[[VALUE_macro_sse:[0-9]+]] macro_sse: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// NOSSE-NEXT:     global %[[VALUE_macro_sse2:[0-9]+]] macro_sse2: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// NOSSE-NEXT:     global %[[VALUE_macro_mmx:[0-9]+]] macro_mmx: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// NOSSE-NEXT:     global %[[VALUE_macro_mmx_with_sse:[0-9]+]] macro_mmx_with_sse: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// NOSSE-NEXT:     global %[[VALUE_macro_flt16_max:[0-9]+]] macro_flt16_max: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// NOSSE-NEXT:     global %[[VALUE_macro_soft_float:[0-9]+]] macro_soft_float: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// NOSSE-NEXT:     global %[[VALUE_macro_3dnow:[0-9]+]] macro_3dnow: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// NOSSE-NEXT:     global %[[VALUE_macro_3dnow_a:[0-9]+]] macro_3dnow_a: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// NOSSE-NEXT: }
// SLATE-FILECHECK-END NOSSE
