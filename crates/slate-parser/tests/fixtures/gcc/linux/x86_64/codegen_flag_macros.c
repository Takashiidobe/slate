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
// SLATE-FILECHECK-DEFINES SMALLPIC
// SLATE-FILECHECK-PREFIX-ARGS SMALLPIC -fpic
// SLATE-FILECHECK-DEFINES SSP
// SLATE-FILECHECK-PREFIX-ARGS SSP -fstack-protector
// SLATE-FILECHECK-DEFINES EXPLICIT
// SLATE-FILECHECK-PREFIX-ARGS EXPLICIT -fstack-protector-explicit
// SLATE-FILECHECK-DEFINES CET
// SLATE-FILECHECK-PREFIX-ARGS CET -fcf-protection=branch -fcf-protection=return
// SLATE-FILECHECK-DEFINES CHECK
// SLATE-FILECHECK-PREFIX-ARGS CHECK -fcf-protection=check -fcf-protection=none
// SLATE-FILECHECK-DEFINES NOSSE
// SLATE-FILECHECK-PREFIX-ARGS NOSSE -mno-sse
// SLATE-FILECHECK-DEFINES NOSSE2
// SLATE-FILECHECK-PREFIX-ARGS NOSSE2 -mno-sse2
// SLATE-FILECHECK-DEFINES NOX87
// SLATE-FILECHECK-PREFIX-ARGS NOX87 -mno-80387
// SLATE-FILECHECK-DEFINES FPRET
// SLATE-FILECHECK-PREFIX-ARGS FPRET -mno-fp-ret-in-387 -mfp-ret-in-387
// SLATE-FILECHECK-DEFINES THREEDNOWA
// SLATE-FILECHECK-PREFIX-ARGS THREEDNOWA -m3dnowa
// SLATE-FILECHECK-DEFINES KERNEL
// SLATE-FILECHECK-PREFIX-ARGS KERNEL -mno-sse -mno-mmx -mno-sse2 -mno-3dnow -mno-80387 -mno-fp-ret-in-387 -mno-avx -mno-sse4a -fcf-protection=branch -mcmodel=kernel -fno-PIE -fstack-protector-strong

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
// DEFAULT-NEXT:     global %[[VALUE_macro_pic:[0-9]+]] macro_pic: i32 [storage=static] = const<i32>(2) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_macro_pie:[0-9]+]] macro_pie: i32 [storage=static] = const<i32>(2) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_macro_ssp:[0-9]+]] macro_ssp: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_macro_ssp_strong:[0-9]+]] macro_ssp_strong: i32 [storage=static] = const<i32>(3) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_macro_ssp_all:[0-9]+]] macro_ssp_all: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_macro_ssp_explicit:[0-9]+]] macro_ssp_explicit: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_macro_cet:[0-9]+]] macro_cet: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_macro_flt_eval_method:[0-9]+]] macro_flt_eval_method: i32 [storage=static] = const<i32>(0) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_macro_sse:[0-9]+]] macro_sse: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_macro_sse2:[0-9]+]] macro_sse2: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_macro_mmx:[0-9]+]] macro_mmx: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_macro_mmx_with_sse:[0-9]+]] macro_mmx_with_sse: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_macro_flt16_max:[0-9]+]] macro_flt16_max: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_macro_soft_float:[0-9]+]] macro_soft_float: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_macro_3dnow:[0-9]+]] macro_3dnow: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_macro_3dnow_a:[0-9]+]] macro_3dnow_a: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN SMALLPIC
// SMALLPIC: module {
// SMALLPIC-NEXT:     target "x86_64-unknown-linux-gnu" {
// SMALLPIC-NEXT:         endian = little;
// SMALLPIC-NEXT:         pointer [size=8, align=8];
// SMALLPIC-NEXT:         stack_alignment = 16;
// SMALLPIC-NEXT:         long_double = f80;
// SMALLPIC-NEXT:         storage bool [size=1, align=1];
// SMALLPIC-NEXT:         storage i8, u8 [size=1, align=1];
// SMALLPIC-NEXT:         storage i16, u16 [size=2, align=2];
// SMALLPIC-NEXT:         storage i32, u32 [size=4, align=4];
// SMALLPIC-NEXT:         storage i64, u64 [size=8, align=8];
// SMALLPIC-NEXT:         storage i128, u128 [size=16, align=16];
// SMALLPIC-NEXT:         storage bf16 [size=2, align=2];
// SMALLPIC-NEXT:         storage f16 [size=2, align=2];
// SMALLPIC-NEXT:         storage f32 [size=4, align=4];
// SMALLPIC-NEXT:         storage f64 [size=8, align=8];
// SMALLPIC-NEXT:         storage f80 [size=16, align=16];
// SMALLPIC-NEXT:         storage f128 [size=16, align=16];
// SMALLPIC-NEXT:         storage d32 [size=4, align=4];
// SMALLPIC-NEXT:         storage d64 [size=8, align=8];
// SMALLPIC-NEXT:         storage d128 [size=16, align=16];
// SMALLPIC-NEXT:     }
// SMALLPIC-NEXT:     global %[[VALUE_macro_pic:[0-9]+]] macro_pic: i32 [storage=static] = const<i32>(1) [linkage=external];
// SMALLPIC-NEXT:     global %[[VALUE_macro_pie:[0-9]+]] macro_pie: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// SMALLPIC-NEXT:     global %[[VALUE_macro_ssp:[0-9]+]] macro_ssp: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// SMALLPIC-NEXT:     global %[[VALUE_macro_ssp_strong:[0-9]+]] macro_ssp_strong: i32 [storage=static] = const<i32>(3) [linkage=external];
// SMALLPIC-NEXT:     global %[[VALUE_macro_ssp_all:[0-9]+]] macro_ssp_all: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// SMALLPIC-NEXT:     global %[[VALUE_macro_ssp_explicit:[0-9]+]] macro_ssp_explicit: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// SMALLPIC-NEXT:     global %[[VALUE_macro_cet:[0-9]+]] macro_cet: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// SMALLPIC-NEXT:     global %[[VALUE_macro_flt_eval_method:[0-9]+]] macro_flt_eval_method: i32 [storage=static] = const<i32>(0) [linkage=external];
// SMALLPIC-NEXT:     global %[[VALUE_macro_sse:[0-9]+]] macro_sse: i32 [storage=static] = const<i32>(1) [linkage=external];
// SMALLPIC-NEXT:     global %[[VALUE_macro_sse2:[0-9]+]] macro_sse2: i32 [storage=static] = const<i32>(1) [linkage=external];
// SMALLPIC-NEXT:     global %[[VALUE_macro_mmx:[0-9]+]] macro_mmx: i32 [storage=static] = const<i32>(1) [linkage=external];
// SMALLPIC-NEXT:     global %[[VALUE_macro_mmx_with_sse:[0-9]+]] macro_mmx_with_sse: i32 [storage=static] = const<i32>(1) [linkage=external];
// SMALLPIC-NEXT:     global %[[VALUE_macro_flt16_max:[0-9]+]] macro_flt16_max: i32 [storage=static] = const<i32>(1) [linkage=external];
// SMALLPIC-NEXT:     global %[[VALUE_macro_soft_float:[0-9]+]] macro_soft_float: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// SMALLPIC-NEXT:     global %[[VALUE_macro_3dnow:[0-9]+]] macro_3dnow: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// SMALLPIC-NEXT:     global %[[VALUE_macro_3dnow_a:[0-9]+]] macro_3dnow_a: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// SMALLPIC-NEXT: }
// SLATE-FILECHECK-END SMALLPIC
// SLATE-FILECHECK-BEGIN SSP
// SSP: module {
// SSP-NEXT:     target "x86_64-unknown-linux-gnu" {
// SSP-NEXT:         endian = little;
// SSP-NEXT:         pointer [size=8, align=8];
// SSP-NEXT:         stack_alignment = 16;
// SSP-NEXT:         long_double = f80;
// SSP-NEXT:         storage bool [size=1, align=1];
// SSP-NEXT:         storage i8, u8 [size=1, align=1];
// SSP-NEXT:         storage i16, u16 [size=2, align=2];
// SSP-NEXT:         storage i32, u32 [size=4, align=4];
// SSP-NEXT:         storage i64, u64 [size=8, align=8];
// SSP-NEXT:         storage i128, u128 [size=16, align=16];
// SSP-NEXT:         storage bf16 [size=2, align=2];
// SSP-NEXT:         storage f16 [size=2, align=2];
// SSP-NEXT:         storage f32 [size=4, align=4];
// SSP-NEXT:         storage f64 [size=8, align=8];
// SSP-NEXT:         storage f80 [size=16, align=16];
// SSP-NEXT:         storage f128 [size=16, align=16];
// SSP-NEXT:         storage d32 [size=4, align=4];
// SSP-NEXT:         storage d64 [size=8, align=8];
// SSP-NEXT:         storage d128 [size=16, align=16];
// SSP-NEXT:     }
// SSP-NEXT:     global %[[VALUE_macro_pic:[0-9]+]] macro_pic: i32 [storage=static] = const<i32>(2) [linkage=external];
// SSP-NEXT:     global %[[VALUE_macro_pie:[0-9]+]] macro_pie: i32 [storage=static] = const<i32>(2) [linkage=external];
// SSP-NEXT:     global %[[VALUE_macro_ssp:[0-9]+]] macro_ssp: i32 [storage=static] = const<i32>(1) [linkage=external];
// SSP-NEXT:     global %[[VALUE_macro_ssp_strong:[0-9]+]] macro_ssp_strong: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// SSP-NEXT:     global %[[VALUE_macro_ssp_all:[0-9]+]] macro_ssp_all: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// SSP-NEXT:     global %[[VALUE_macro_ssp_explicit:[0-9]+]] macro_ssp_explicit: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// SSP-NEXT:     global %[[VALUE_macro_cet:[0-9]+]] macro_cet: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// SSP-NEXT:     global %[[VALUE_macro_flt_eval_method:[0-9]+]] macro_flt_eval_method: i32 [storage=static] = const<i32>(0) [linkage=external];
// SSP-NEXT:     global %[[VALUE_macro_sse:[0-9]+]] macro_sse: i32 [storage=static] = const<i32>(1) [linkage=external];
// SSP-NEXT:     global %[[VALUE_macro_sse2:[0-9]+]] macro_sse2: i32 [storage=static] = const<i32>(1) [linkage=external];
// SSP-NEXT:     global %[[VALUE_macro_mmx:[0-9]+]] macro_mmx: i32 [storage=static] = const<i32>(1) [linkage=external];
// SSP-NEXT:     global %[[VALUE_macro_mmx_with_sse:[0-9]+]] macro_mmx_with_sse: i32 [storage=static] = const<i32>(1) [linkage=external];
// SSP-NEXT:     global %[[VALUE_macro_flt16_max:[0-9]+]] macro_flt16_max: i32 [storage=static] = const<i32>(1) [linkage=external];
// SSP-NEXT:     global %[[VALUE_macro_soft_float:[0-9]+]] macro_soft_float: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// SSP-NEXT:     global %[[VALUE_macro_3dnow:[0-9]+]] macro_3dnow: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// SSP-NEXT:     global %[[VALUE_macro_3dnow_a:[0-9]+]] macro_3dnow_a: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// SSP-NEXT: }
// SLATE-FILECHECK-END SSP
// SLATE-FILECHECK-BEGIN EXPLICIT
// EXPLICIT: module {
// EXPLICIT-NEXT:     target "x86_64-unknown-linux-gnu" {
// EXPLICIT-NEXT:         endian = little;
// EXPLICIT-NEXT:         pointer [size=8, align=8];
// EXPLICIT-NEXT:         stack_alignment = 16;
// EXPLICIT-NEXT:         long_double = f80;
// EXPLICIT-NEXT:         storage bool [size=1, align=1];
// EXPLICIT-NEXT:         storage i8, u8 [size=1, align=1];
// EXPLICIT-NEXT:         storage i16, u16 [size=2, align=2];
// EXPLICIT-NEXT:         storage i32, u32 [size=4, align=4];
// EXPLICIT-NEXT:         storage i64, u64 [size=8, align=8];
// EXPLICIT-NEXT:         storage i128, u128 [size=16, align=16];
// EXPLICIT-NEXT:         storage bf16 [size=2, align=2];
// EXPLICIT-NEXT:         storage f16 [size=2, align=2];
// EXPLICIT-NEXT:         storage f32 [size=4, align=4];
// EXPLICIT-NEXT:         storage f64 [size=8, align=8];
// EXPLICIT-NEXT:         storage f80 [size=16, align=16];
// EXPLICIT-NEXT:         storage f128 [size=16, align=16];
// EXPLICIT-NEXT:         storage d32 [size=4, align=4];
// EXPLICIT-NEXT:         storage d64 [size=8, align=8];
// EXPLICIT-NEXT:         storage d128 [size=16, align=16];
// EXPLICIT-NEXT:     }
// EXPLICIT-NEXT:     global %[[VALUE_macro_pic:[0-9]+]] macro_pic: i32 [storage=static] = const<i32>(2) [linkage=external];
// EXPLICIT-NEXT:     global %[[VALUE_macro_pie:[0-9]+]] macro_pie: i32 [storage=static] = const<i32>(2) [linkage=external];
// EXPLICIT-NEXT:     global %[[VALUE_macro_ssp:[0-9]+]] macro_ssp: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// EXPLICIT-NEXT:     global %[[VALUE_macro_ssp_strong:[0-9]+]] macro_ssp_strong: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// EXPLICIT-NEXT:     global %[[VALUE_macro_ssp_all:[0-9]+]] macro_ssp_all: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// EXPLICIT-NEXT:     global %[[VALUE_macro_ssp_explicit:[0-9]+]] macro_ssp_explicit: i32 [storage=static] = const<i32>(4) [linkage=external];
// EXPLICIT-NEXT:     global %[[VALUE_macro_cet:[0-9]+]] macro_cet: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// EXPLICIT-NEXT:     global %[[VALUE_macro_flt_eval_method:[0-9]+]] macro_flt_eval_method: i32 [storage=static] = const<i32>(0) [linkage=external];
// EXPLICIT-NEXT:     global %[[VALUE_macro_sse:[0-9]+]] macro_sse: i32 [storage=static] = const<i32>(1) [linkage=external];
// EXPLICIT-NEXT:     global %[[VALUE_macro_sse2:[0-9]+]] macro_sse2: i32 [storage=static] = const<i32>(1) [linkage=external];
// EXPLICIT-NEXT:     global %[[VALUE_macro_mmx:[0-9]+]] macro_mmx: i32 [storage=static] = const<i32>(1) [linkage=external];
// EXPLICIT-NEXT:     global %[[VALUE_macro_mmx_with_sse:[0-9]+]] macro_mmx_with_sse: i32 [storage=static] = const<i32>(1) [linkage=external];
// EXPLICIT-NEXT:     global %[[VALUE_macro_flt16_max:[0-9]+]] macro_flt16_max: i32 [storage=static] = const<i32>(1) [linkage=external];
// EXPLICIT-NEXT:     global %[[VALUE_macro_soft_float:[0-9]+]] macro_soft_float: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// EXPLICIT-NEXT:     global %[[VALUE_macro_3dnow:[0-9]+]] macro_3dnow: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// EXPLICIT-NEXT:     global %[[VALUE_macro_3dnow_a:[0-9]+]] macro_3dnow_a: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// EXPLICIT-NEXT: }
// SLATE-FILECHECK-END EXPLICIT
// SLATE-FILECHECK-BEGIN CET
// CET: module {
// CET-NEXT:     target "x86_64-unknown-linux-gnu" {
// CET-NEXT:         endian = little;
// CET-NEXT:         pointer [size=8, align=8];
// CET-NEXT:         stack_alignment = 16;
// CET-NEXT:         long_double = f80;
// CET-NEXT:         storage bool [size=1, align=1];
// CET-NEXT:         storage i8, u8 [size=1, align=1];
// CET-NEXT:         storage i16, u16 [size=2, align=2];
// CET-NEXT:         storage i32, u32 [size=4, align=4];
// CET-NEXT:         storage i64, u64 [size=8, align=8];
// CET-NEXT:         storage i128, u128 [size=16, align=16];
// CET-NEXT:         storage bf16 [size=2, align=2];
// CET-NEXT:         storage f16 [size=2, align=2];
// CET-NEXT:         storage f32 [size=4, align=4];
// CET-NEXT:         storage f64 [size=8, align=8];
// CET-NEXT:         storage f80 [size=16, align=16];
// CET-NEXT:         storage f128 [size=16, align=16];
// CET-NEXT:         storage d32 [size=4, align=4];
// CET-NEXT:         storage d64 [size=8, align=8];
// CET-NEXT:         storage d128 [size=16, align=16];
// CET-NEXT:     }
// CET-NEXT:     global %[[VALUE_macro_pic:[0-9]+]] macro_pic: i32 [storage=static] = const<i32>(2) [linkage=external];
// CET-NEXT:     global %[[VALUE_macro_pie:[0-9]+]] macro_pie: i32 [storage=static] = const<i32>(2) [linkage=external];
// CET-NEXT:     global %[[VALUE_macro_ssp:[0-9]+]] macro_ssp: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// CET-NEXT:     global %[[VALUE_macro_ssp_strong:[0-9]+]] macro_ssp_strong: i32 [storage=static] = const<i32>(3) [linkage=external];
// CET-NEXT:     global %[[VALUE_macro_ssp_all:[0-9]+]] macro_ssp_all: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// CET-NEXT:     global %[[VALUE_macro_ssp_explicit:[0-9]+]] macro_ssp_explicit: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// CET-NEXT:     global %[[VALUE_macro_cet:[0-9]+]] macro_cet: i32 [storage=static] = const<i32>(3) [linkage=external];
// CET-NEXT:     global %[[VALUE_macro_flt_eval_method:[0-9]+]] macro_flt_eval_method: i32 [storage=static] = const<i32>(0) [linkage=external];
// CET-NEXT:     global %[[VALUE_macro_sse:[0-9]+]] macro_sse: i32 [storage=static] = const<i32>(1) [linkage=external];
// CET-NEXT:     global %[[VALUE_macro_sse2:[0-9]+]] macro_sse2: i32 [storage=static] = const<i32>(1) [linkage=external];
// CET-NEXT:     global %[[VALUE_macro_mmx:[0-9]+]] macro_mmx: i32 [storage=static] = const<i32>(1) [linkage=external];
// CET-NEXT:     global %[[VALUE_macro_mmx_with_sse:[0-9]+]] macro_mmx_with_sse: i32 [storage=static] = const<i32>(1) [linkage=external];
// CET-NEXT:     global %[[VALUE_macro_flt16_max:[0-9]+]] macro_flt16_max: i32 [storage=static] = const<i32>(1) [linkage=external];
// CET-NEXT:     global %[[VALUE_macro_soft_float:[0-9]+]] macro_soft_float: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// CET-NEXT:     global %[[VALUE_macro_3dnow:[0-9]+]] macro_3dnow: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// CET-NEXT:     global %[[VALUE_macro_3dnow_a:[0-9]+]] macro_3dnow_a: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// CET-NEXT: }
// SLATE-FILECHECK-END CET
// SLATE-FILECHECK-BEGIN CHECK
// CHECK: module {
// CHECK-NEXT:     target "x86_64-unknown-linux-gnu" {
// CHECK-NEXT:         endian = little;
// CHECK-NEXT:         pointer [size=8, align=8];
// CHECK-NEXT:         stack_alignment = 16;
// CHECK-NEXT:         long_double = f80;
// CHECK-NEXT:         storage bool [size=1, align=1];
// CHECK-NEXT:         storage i8, u8 [size=1, align=1];
// CHECK-NEXT:         storage i16, u16 [size=2, align=2];
// CHECK-NEXT:         storage i32, u32 [size=4, align=4];
// CHECK-NEXT:         storage i64, u64 [size=8, align=8];
// CHECK-NEXT:         storage i128, u128 [size=16, align=16];
// CHECK-NEXT:         storage bf16 [size=2, align=2];
// CHECK-NEXT:         storage f16 [size=2, align=2];
// CHECK-NEXT:         storage f32 [size=4, align=4];
// CHECK-NEXT:         storage f64 [size=8, align=8];
// CHECK-NEXT:         storage f80 [size=16, align=16];
// CHECK-NEXT:         storage f128 [size=16, align=16];
// CHECK-NEXT:         storage d32 [size=4, align=4];
// CHECK-NEXT:         storage d64 [size=8, align=8];
// CHECK-NEXT:         storage d128 [size=16, align=16];
// CHECK-NEXT:     }
// CHECK-NEXT:     global %[[VALUE_macro_pic:[0-9]+]] macro_pic: i32 [storage=static] = const<i32>(2) [linkage=external];
// CHECK-NEXT:     global %[[VALUE_macro_pie:[0-9]+]] macro_pie: i32 [storage=static] = const<i32>(2) [linkage=external];
// CHECK-NEXT:     global %[[VALUE_macro_ssp:[0-9]+]] macro_ssp: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// CHECK-NEXT:     global %[[VALUE_macro_ssp_strong:[0-9]+]] macro_ssp_strong: i32 [storage=static] = const<i32>(3) [linkage=external];
// CHECK-NEXT:     global %[[VALUE_macro_ssp_all:[0-9]+]] macro_ssp_all: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// CHECK-NEXT:     global %[[VALUE_macro_ssp_explicit:[0-9]+]] macro_ssp_explicit: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// CHECK-NEXT:     global %[[VALUE_macro_cet:[0-9]+]] macro_cet: i32 [storage=static] = const<i32>(8) [linkage=external];
// CHECK-NEXT:     global %[[VALUE_macro_flt_eval_method:[0-9]+]] macro_flt_eval_method: i32 [storage=static] = const<i32>(0) [linkage=external];
// CHECK-NEXT:     global %[[VALUE_macro_sse:[0-9]+]] macro_sse: i32 [storage=static] = const<i32>(1) [linkage=external];
// CHECK-NEXT:     global %[[VALUE_macro_sse2:[0-9]+]] macro_sse2: i32 [storage=static] = const<i32>(1) [linkage=external];
// CHECK-NEXT:     global %[[VALUE_macro_mmx:[0-9]+]] macro_mmx: i32 [storage=static] = const<i32>(1) [linkage=external];
// CHECK-NEXT:     global %[[VALUE_macro_mmx_with_sse:[0-9]+]] macro_mmx_with_sse: i32 [storage=static] = const<i32>(1) [linkage=external];
// CHECK-NEXT:     global %[[VALUE_macro_flt16_max:[0-9]+]] macro_flt16_max: i32 [storage=static] = const<i32>(1) [linkage=external];
// CHECK-NEXT:     global %[[VALUE_macro_soft_float:[0-9]+]] macro_soft_float: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// CHECK-NEXT:     global %[[VALUE_macro_3dnow:[0-9]+]] macro_3dnow: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// CHECK-NEXT:     global %[[VALUE_macro_3dnow_a:[0-9]+]] macro_3dnow_a: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// CHECK-NEXT: }
// SLATE-FILECHECK-END CHECK
// SLATE-FILECHECK-BEGIN NOSSE
// NOSSE: module {
// NOSSE-NEXT:     target "x86_64-unknown-linux-gnu" {
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
// NOSSE-NEXT:     global %[[VALUE_macro_pie:[0-9]+]] macro_pie: i32 [storage=static] = const<i32>(2) [linkage=external];
// NOSSE-NEXT:     global %[[VALUE_macro_ssp:[0-9]+]] macro_ssp: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// NOSSE-NEXT:     global %[[VALUE_macro_ssp_strong:[0-9]+]] macro_ssp_strong: i32 [storage=static] = const<i32>(3) [linkage=external];
// NOSSE-NEXT:     global %[[VALUE_macro_ssp_all:[0-9]+]] macro_ssp_all: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// NOSSE-NEXT:     global %[[VALUE_macro_ssp_explicit:[0-9]+]] macro_ssp_explicit: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// NOSSE-NEXT:     global %[[VALUE_macro_cet:[0-9]+]] macro_cet: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// NOSSE-NEXT:     global %[[VALUE_macro_flt_eval_method:[0-9]+]] macro_flt_eval_method: i32 [storage=static] = const<i32>(2) [linkage=external];
// NOSSE-NEXT:     global %[[VALUE_macro_sse:[0-9]+]] macro_sse: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// NOSSE-NEXT:     global %[[VALUE_macro_sse2:[0-9]+]] macro_sse2: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// NOSSE-NEXT:     global %[[VALUE_macro_mmx:[0-9]+]] macro_mmx: i32 [storage=static] = const<i32>(1) [linkage=external];
// NOSSE-NEXT:     global %[[VALUE_macro_mmx_with_sse:[0-9]+]] macro_mmx_with_sse: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// NOSSE-NEXT:     global %[[VALUE_macro_flt16_max:[0-9]+]] macro_flt16_max: i32 [storage=static] = const<i32>(1) [linkage=external];
// NOSSE-NEXT:     global %[[VALUE_macro_soft_float:[0-9]+]] macro_soft_float: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// NOSSE-NEXT:     global %[[VALUE_macro_3dnow:[0-9]+]] macro_3dnow: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// NOSSE-NEXT:     global %[[VALUE_macro_3dnow_a:[0-9]+]] macro_3dnow_a: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// NOSSE-NEXT: }
// SLATE-FILECHECK-END NOSSE
// SLATE-FILECHECK-BEGIN NOSSE2
// NOSSE2: module {
// NOSSE2-NEXT:     target "x86_64-unknown-linux-gnu" {
// NOSSE2-NEXT:         endian = little;
// NOSSE2-NEXT:         pointer [size=8, align=8];
// NOSSE2-NEXT:         stack_alignment = 16;
// NOSSE2-NEXT:         long_double = f80;
// NOSSE2-NEXT:         storage bool [size=1, align=1];
// NOSSE2-NEXT:         storage i8, u8 [size=1, align=1];
// NOSSE2-NEXT:         storage i16, u16 [size=2, align=2];
// NOSSE2-NEXT:         storage i32, u32 [size=4, align=4];
// NOSSE2-NEXT:         storage i64, u64 [size=8, align=8];
// NOSSE2-NEXT:         storage i128, u128 [size=16, align=16];
// NOSSE2-NEXT:         storage bf16 [size=2, align=2];
// NOSSE2-NEXT:         storage f16 [size=2, align=2];
// NOSSE2-NEXT:         storage f32 [size=4, align=4];
// NOSSE2-NEXT:         storage f64 [size=8, align=8];
// NOSSE2-NEXT:         storage f80 [size=16, align=16];
// NOSSE2-NEXT:         storage f128 [size=16, align=16];
// NOSSE2-NEXT:         storage d32 [size=4, align=4];
// NOSSE2-NEXT:         storage d64 [size=8, align=8];
// NOSSE2-NEXT:         storage d128 [size=16, align=16];
// NOSSE2-NEXT:     }
// NOSSE2-NEXT:     global %[[VALUE_macro_pic:[0-9]+]] macro_pic: i32 [storage=static] = const<i32>(2) [linkage=external];
// NOSSE2-NEXT:     global %[[VALUE_macro_pie:[0-9]+]] macro_pie: i32 [storage=static] = const<i32>(2) [linkage=external];
// NOSSE2-NEXT:     global %[[VALUE_macro_ssp:[0-9]+]] macro_ssp: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// NOSSE2-NEXT:     global %[[VALUE_macro_ssp_strong:[0-9]+]] macro_ssp_strong: i32 [storage=static] = const<i32>(3) [linkage=external];
// NOSSE2-NEXT:     global %[[VALUE_macro_ssp_all:[0-9]+]] macro_ssp_all: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// NOSSE2-NEXT:     global %[[VALUE_macro_ssp_explicit:[0-9]+]] macro_ssp_explicit: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// NOSSE2-NEXT:     global %[[VALUE_macro_cet:[0-9]+]] macro_cet: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// NOSSE2-NEXT:     global %[[VALUE_macro_flt_eval_method:[0-9]+]] macro_flt_eval_method: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// NOSSE2-NEXT:     global %[[VALUE_macro_sse:[0-9]+]] macro_sse: i32 [storage=static] = const<i32>(1) [linkage=external];
// NOSSE2-NEXT:     global %[[VALUE_macro_sse2:[0-9]+]] macro_sse2: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// NOSSE2-NEXT:     global %[[VALUE_macro_mmx:[0-9]+]] macro_mmx: i32 [storage=static] = const<i32>(1) [linkage=external];
// NOSSE2-NEXT:     global %[[VALUE_macro_mmx_with_sse:[0-9]+]] macro_mmx_with_sse: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// NOSSE2-NEXT:     global %[[VALUE_macro_flt16_max:[0-9]+]] macro_flt16_max: i32 [storage=static] = const<i32>(1) [linkage=external];
// NOSSE2-NEXT:     global %[[VALUE_macro_soft_float:[0-9]+]] macro_soft_float: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// NOSSE2-NEXT:     global %[[VALUE_macro_3dnow:[0-9]+]] macro_3dnow: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// NOSSE2-NEXT:     global %[[VALUE_macro_3dnow_a:[0-9]+]] macro_3dnow_a: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// NOSSE2-NEXT: }
// SLATE-FILECHECK-END NOSSE2
// SLATE-FILECHECK-BEGIN NOX87
// NOX87: module {
// NOX87-NEXT:     target "x86_64-unknown-linux-gnu" {
// NOX87-NEXT:         endian = little;
// NOX87-NEXT:         pointer [size=8, align=8];
// NOX87-NEXT:         stack_alignment = 16;
// NOX87-NEXT:         long_double = f80;
// NOX87-NEXT:         storage bool [size=1, align=1];
// NOX87-NEXT:         storage i8, u8 [size=1, align=1];
// NOX87-NEXT:         storage i16, u16 [size=2, align=2];
// NOX87-NEXT:         storage i32, u32 [size=4, align=4];
// NOX87-NEXT:         storage i64, u64 [size=8, align=8];
// NOX87-NEXT:         storage i128, u128 [size=16, align=16];
// NOX87-NEXT:         storage bf16 [size=2, align=2];
// NOX87-NEXT:         storage f16 [size=2, align=2];
// NOX87-NEXT:         storage f32 [size=4, align=4];
// NOX87-NEXT:         storage f64 [size=8, align=8];
// NOX87-NEXT:         storage f80 [size=16, align=16];
// NOX87-NEXT:         storage f128 [size=16, align=16];
// NOX87-NEXT:         storage d32 [size=4, align=4];
// NOX87-NEXT:         storage d64 [size=8, align=8];
// NOX87-NEXT:         storage d128 [size=16, align=16];
// NOX87-NEXT:     }
// NOX87-NEXT:     global %[[VALUE_macro_pic:[0-9]+]] macro_pic: i32 [storage=static] = const<i32>(2) [linkage=external];
// NOX87-NEXT:     global %[[VALUE_macro_pie:[0-9]+]] macro_pie: i32 [storage=static] = const<i32>(2) [linkage=external];
// NOX87-NEXT:     global %[[VALUE_macro_ssp:[0-9]+]] macro_ssp: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// NOX87-NEXT:     global %[[VALUE_macro_ssp_strong:[0-9]+]] macro_ssp_strong: i32 [storage=static] = const<i32>(3) [linkage=external];
// NOX87-NEXT:     global %[[VALUE_macro_ssp_all:[0-9]+]] macro_ssp_all: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// NOX87-NEXT:     global %[[VALUE_macro_ssp_explicit:[0-9]+]] macro_ssp_explicit: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// NOX87-NEXT:     global %[[VALUE_macro_cet:[0-9]+]] macro_cet: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// NOX87-NEXT:     global %[[VALUE_macro_flt_eval_method:[0-9]+]] macro_flt_eval_method: i32 [storage=static] = const<i32>(0) [linkage=external];
// NOX87-NEXT:     global %[[VALUE_macro_sse:[0-9]+]] macro_sse: i32 [storage=static] = const<i32>(1) [linkage=external];
// NOX87-NEXT:     global %[[VALUE_macro_sse2:[0-9]+]] macro_sse2: i32 [storage=static] = const<i32>(1) [linkage=external];
// NOX87-NEXT:     global %[[VALUE_macro_mmx:[0-9]+]] macro_mmx: i32 [storage=static] = const<i32>(1) [linkage=external];
// NOX87-NEXT:     global %[[VALUE_macro_mmx_with_sse:[0-9]+]] macro_mmx_with_sse: i32 [storage=static] = const<i32>(1) [linkage=external];
// NOX87-NEXT:     global %[[VALUE_macro_flt16_max:[0-9]+]] macro_flt16_max: i32 [storage=static] = const<i32>(1) [linkage=external];
// NOX87-NEXT:     global %[[VALUE_macro_soft_float:[0-9]+]] macro_soft_float: i32 [storage=static] = const<i32>(1) [linkage=external];
// NOX87-NEXT:     global %[[VALUE_macro_3dnow:[0-9]+]] macro_3dnow: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// NOX87-NEXT:     global %[[VALUE_macro_3dnow_a:[0-9]+]] macro_3dnow_a: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// NOX87-NEXT: }
// SLATE-FILECHECK-END NOX87
// SLATE-FILECHECK-BEGIN FPRET
// FPRET: module {
// FPRET-NEXT:     target "x86_64-unknown-linux-gnu" {
// FPRET-NEXT:         endian = little;
// FPRET-NEXT:         pointer [size=8, align=8];
// FPRET-NEXT:         stack_alignment = 16;
// FPRET-NEXT:         long_double = f80;
// FPRET-NEXT:         storage bool [size=1, align=1];
// FPRET-NEXT:         storage i8, u8 [size=1, align=1];
// FPRET-NEXT:         storage i16, u16 [size=2, align=2];
// FPRET-NEXT:         storage i32, u32 [size=4, align=4];
// FPRET-NEXT:         storage i64, u64 [size=8, align=8];
// FPRET-NEXT:         storage i128, u128 [size=16, align=16];
// FPRET-NEXT:         storage bf16 [size=2, align=2];
// FPRET-NEXT:         storage f16 [size=2, align=2];
// FPRET-NEXT:         storage f32 [size=4, align=4];
// FPRET-NEXT:         storage f64 [size=8, align=8];
// FPRET-NEXT:         storage f80 [size=16, align=16];
// FPRET-NEXT:         storage f128 [size=16, align=16];
// FPRET-NEXT:         storage d32 [size=4, align=4];
// FPRET-NEXT:         storage d64 [size=8, align=8];
// FPRET-NEXT:         storage d128 [size=16, align=16];
// FPRET-NEXT:     }
// FPRET-NEXT:     global %[[VALUE_macro_pic:[0-9]+]] macro_pic: i32 [storage=static] = const<i32>(2) [linkage=external];
// FPRET-NEXT:     global %[[VALUE_macro_pie:[0-9]+]] macro_pie: i32 [storage=static] = const<i32>(2) [linkage=external];
// FPRET-NEXT:     global %[[VALUE_macro_ssp:[0-9]+]] macro_ssp: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// FPRET-NEXT:     global %[[VALUE_macro_ssp_strong:[0-9]+]] macro_ssp_strong: i32 [storage=static] = const<i32>(3) [linkage=external];
// FPRET-NEXT:     global %[[VALUE_macro_ssp_all:[0-9]+]] macro_ssp_all: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// FPRET-NEXT:     global %[[VALUE_macro_ssp_explicit:[0-9]+]] macro_ssp_explicit: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// FPRET-NEXT:     global %[[VALUE_macro_cet:[0-9]+]] macro_cet: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// FPRET-NEXT:     global %[[VALUE_macro_flt_eval_method:[0-9]+]] macro_flt_eval_method: i32 [storage=static] = const<i32>(0) [linkage=external];
// FPRET-NEXT:     global %[[VALUE_macro_sse:[0-9]+]] macro_sse: i32 [storage=static] = const<i32>(1) [linkage=external];
// FPRET-NEXT:     global %[[VALUE_macro_sse2:[0-9]+]] macro_sse2: i32 [storage=static] = const<i32>(1) [linkage=external];
// FPRET-NEXT:     global %[[VALUE_macro_mmx:[0-9]+]] macro_mmx: i32 [storage=static] = const<i32>(1) [linkage=external];
// FPRET-NEXT:     global %[[VALUE_macro_mmx_with_sse:[0-9]+]] macro_mmx_with_sse: i32 [storage=static] = const<i32>(1) [linkage=external];
// FPRET-NEXT:     global %[[VALUE_macro_flt16_max:[0-9]+]] macro_flt16_max: i32 [storage=static] = const<i32>(1) [linkage=external];
// FPRET-NEXT:     global %[[VALUE_macro_soft_float:[0-9]+]] macro_soft_float: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// FPRET-NEXT:     global %[[VALUE_macro_3dnow:[0-9]+]] macro_3dnow: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// FPRET-NEXT:     global %[[VALUE_macro_3dnow_a:[0-9]+]] macro_3dnow_a: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// FPRET-NEXT: }
// SLATE-FILECHECK-END FPRET
// SLATE-FILECHECK-BEGIN THREEDNOWA
// THREEDNOWA: module {
// THREEDNOWA-NEXT:     target "x86_64-unknown-linux-gnu" {
// THREEDNOWA-NEXT:         endian = little;
// THREEDNOWA-NEXT:         pointer [size=8, align=8];
// THREEDNOWA-NEXT:         stack_alignment = 16;
// THREEDNOWA-NEXT:         long_double = f80;
// THREEDNOWA-NEXT:         storage bool [size=1, align=1];
// THREEDNOWA-NEXT:         storage i8, u8 [size=1, align=1];
// THREEDNOWA-NEXT:         storage i16, u16 [size=2, align=2];
// THREEDNOWA-NEXT:         storage i32, u32 [size=4, align=4];
// THREEDNOWA-NEXT:         storage i64, u64 [size=8, align=8];
// THREEDNOWA-NEXT:         storage i128, u128 [size=16, align=16];
// THREEDNOWA-NEXT:         storage bf16 [size=2, align=2];
// THREEDNOWA-NEXT:         storage f16 [size=2, align=2];
// THREEDNOWA-NEXT:         storage f32 [size=4, align=4];
// THREEDNOWA-NEXT:         storage f64 [size=8, align=8];
// THREEDNOWA-NEXT:         storage f80 [size=16, align=16];
// THREEDNOWA-NEXT:         storage f128 [size=16, align=16];
// THREEDNOWA-NEXT:         storage d32 [size=4, align=4];
// THREEDNOWA-NEXT:         storage d64 [size=8, align=8];
// THREEDNOWA-NEXT:         storage d128 [size=16, align=16];
// THREEDNOWA-NEXT:     }
// THREEDNOWA-NEXT:     global %[[VALUE_macro_pic:[0-9]+]] macro_pic: i32 [storage=static] = const<i32>(2) [linkage=external];
// THREEDNOWA-NEXT:     global %[[VALUE_macro_pie:[0-9]+]] macro_pie: i32 [storage=static] = const<i32>(2) [linkage=external];
// THREEDNOWA-NEXT:     global %[[VALUE_macro_ssp:[0-9]+]] macro_ssp: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// THREEDNOWA-NEXT:     global %[[VALUE_macro_ssp_strong:[0-9]+]] macro_ssp_strong: i32 [storage=static] = const<i32>(3) [linkage=external];
// THREEDNOWA-NEXT:     global %[[VALUE_macro_ssp_all:[0-9]+]] macro_ssp_all: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// THREEDNOWA-NEXT:     global %[[VALUE_macro_ssp_explicit:[0-9]+]] macro_ssp_explicit: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// THREEDNOWA-NEXT:     global %[[VALUE_macro_cet:[0-9]+]] macro_cet: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// THREEDNOWA-NEXT:     global %[[VALUE_macro_flt_eval_method:[0-9]+]] macro_flt_eval_method: i32 [storage=static] = const<i32>(0) [linkage=external];
// THREEDNOWA-NEXT:     global %[[VALUE_macro_sse:[0-9]+]] macro_sse: i32 [storage=static] = const<i32>(1) [linkage=external];
// THREEDNOWA-NEXT:     global %[[VALUE_macro_sse2:[0-9]+]] macro_sse2: i32 [storage=static] = const<i32>(1) [linkage=external];
// THREEDNOWA-NEXT:     global %[[VALUE_macro_mmx:[0-9]+]] macro_mmx: i32 [storage=static] = const<i32>(1) [linkage=external];
// THREEDNOWA-NEXT:     global %[[VALUE_macro_mmx_with_sse:[0-9]+]] macro_mmx_with_sse: i32 [storage=static] = const<i32>(1) [linkage=external];
// THREEDNOWA-NEXT:     global %[[VALUE_macro_flt16_max:[0-9]+]] macro_flt16_max: i32 [storage=static] = const<i32>(1) [linkage=external];
// THREEDNOWA-NEXT:     global %[[VALUE_macro_soft_float:[0-9]+]] macro_soft_float: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// THREEDNOWA-NEXT:     global %[[VALUE_macro_3dnow:[0-9]+]] macro_3dnow: i32 [storage=static] = const<i32>(1) [linkage=external];
// THREEDNOWA-NEXT:     global %[[VALUE_macro_3dnow_a:[0-9]+]] macro_3dnow_a: i32 [storage=static] = const<i32>(1) [linkage=external];
// THREEDNOWA-NEXT: }
// SLATE-FILECHECK-END THREEDNOWA
// SLATE-FILECHECK-BEGIN KERNEL
// KERNEL: module {
// KERNEL-NEXT:     target "x86_64-unknown-linux-gnu" {
// KERNEL-NEXT:         endian = little;
// KERNEL-NEXT:         pointer [size=8, align=8];
// KERNEL-NEXT:         stack_alignment = 16;
// KERNEL-NEXT:         long_double = f80;
// KERNEL-NEXT:         storage bool [size=1, align=1];
// KERNEL-NEXT:         storage i8, u8 [size=1, align=1];
// KERNEL-NEXT:         storage i16, u16 [size=2, align=2];
// KERNEL-NEXT:         storage i32, u32 [size=4, align=4];
// KERNEL-NEXT:         storage i64, u64 [size=8, align=8];
// KERNEL-NEXT:         storage i128, u128 [size=16, align=16];
// KERNEL-NEXT:         storage bf16 [size=2, align=2];
// KERNEL-NEXT:         storage f16 [size=2, align=2];
// KERNEL-NEXT:         storage f32 [size=4, align=4];
// KERNEL-NEXT:         storage f64 [size=8, align=8];
// KERNEL-NEXT:         storage f80 [size=16, align=16];
// KERNEL-NEXT:         storage f128 [size=16, align=16];
// KERNEL-NEXT:         storage d32 [size=4, align=4];
// KERNEL-NEXT:         storage d64 [size=8, align=8];
// KERNEL-NEXT:         storage d128 [size=16, align=16];
// KERNEL-NEXT:     }
// KERNEL-NEXT:     global %[[VALUE_macro_pic:[0-9]+]] macro_pic: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// KERNEL-NEXT:     global %[[VALUE_macro_pie:[0-9]+]] macro_pie: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// KERNEL-NEXT:     global %[[VALUE_macro_ssp:[0-9]+]] macro_ssp: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// KERNEL-NEXT:     global %[[VALUE_macro_ssp_strong:[0-9]+]] macro_ssp_strong: i32 [storage=static] = const<i32>(3) [linkage=external];
// KERNEL-NEXT:     global %[[VALUE_macro_ssp_all:[0-9]+]] macro_ssp_all: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// KERNEL-NEXT:     global %[[VALUE_macro_ssp_explicit:[0-9]+]] macro_ssp_explicit: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// KERNEL-NEXT:     global %[[VALUE_macro_cet:[0-9]+]] macro_cet: i32 [storage=static] = const<i32>(1) [linkage=external];
// KERNEL-NEXT:     global %[[VALUE_macro_flt_eval_method:[0-9]+]] macro_flt_eval_method: i32 [storage=static] = const<i32>(0) [linkage=external];
// KERNEL-NEXT:     global %[[VALUE_macro_sse:[0-9]+]] macro_sse: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// KERNEL-NEXT:     global %[[VALUE_macro_sse2:[0-9]+]] macro_sse2: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// KERNEL-NEXT:     global %[[VALUE_macro_mmx:[0-9]+]] macro_mmx: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// KERNEL-NEXT:     global %[[VALUE_macro_mmx_with_sse:[0-9]+]] macro_mmx_with_sse: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// KERNEL-NEXT:     global %[[VALUE_macro_flt16_max:[0-9]+]] macro_flt16_max: i32 [storage=static] = const<i32>(1) [linkage=external];
// KERNEL-NEXT:     global %[[VALUE_macro_soft_float:[0-9]+]] macro_soft_float: i32 [storage=static] = const<i32>(1) [linkage=external];
// KERNEL-NEXT:     global %[[VALUE_macro_3dnow:[0-9]+]] macro_3dnow: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// KERNEL-NEXT:     global %[[VALUE_macro_3dnow_a:[0-9]+]] macro_3dnow_a: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// KERNEL-NEXT: }
// SLATE-FILECHECK-END KERNEL
