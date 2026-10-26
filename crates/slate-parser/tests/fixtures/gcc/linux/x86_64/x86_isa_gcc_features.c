// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-DEFINES AMX
// SLATE-FILECHECK-PREFIX-ARGS AMX -mamx-bf16
// SLATE-FILECHECK-DEFINES VP2
// SLATE-FILECHECK-PREFIX-ARGS VP2 -mavx512vp2intersect -mno-avx512dq
// SLATE-FILECHECK-DEFINES VAES
// SLATE-FILECHECK-PREFIX-ARGS VAES -mvaes -mno-xsave
// SLATE-FILECHECK-DEFINES V2-NO-SSE42
// SLATE-FILECHECK-PREFIX-ARGS V2-NO-SSE42 -march=x86-64-v2 -mno-sse4.2
// SLATE-FILECHECK-DEFINES FMA4
// SLATE-FILECHECK-PREFIX-ARGS FMA4 -mfma4
// SLATE-FILECHECK-DEFINES USERMSR
// SLATE-FILECHECK-PREFIX-ARGS USERMSR -musermsr
// SLATE-FILECHECK-DEFINES SHA512
// SLATE-FILECHECK-PREFIX-ARGS SHA512 -msha512

#ifdef __AMX_BF16__
int val__AMX_BF16__[__AMX_BF16__ + 1];
#endif
#ifdef __AMX_TILE__
int val__AMX_TILE__[__AMX_TILE__ + 1];
#endif
#ifdef __XSAVE__
int val__XSAVE__[__XSAVE__ + 1];
#endif
#ifdef __AVX512VP2INTERSECT__
int val__AVX512VP2INTERSECT__[__AVX512VP2INTERSECT__ + 1];
#endif
#ifdef __AVX512DQ__
int val__AVX512DQ__[__AVX512DQ__ + 1];
#endif
#ifdef __AVX512F__
int val__AVX512F__[__AVX512F__ + 1];
#endif
#ifdef __VAES__
int val__VAES__[__VAES__ + 1];
#endif
#ifdef __AES__
int val__AES__[__AES__ + 1];
#endif
#ifdef __AVX__
int val__AVX__[__AVX__ + 1];
#endif
#ifdef __AVX2__
int val__AVX2__[__AVX2__ + 1];
#endif
#ifdef __SHA512__
int val__SHA512__[__SHA512__ + 1];
#endif
#ifdef __CRC32__
int val__CRC32__[__CRC32__ + 1];
#endif
#ifdef __POPCNT__
int val__POPCNT__[__POPCNT__ + 1];
#endif
#ifdef __SSE4_2__
int val__SSE4_2__[__SSE4_2__ + 1];
#endif
#ifdef __FMA4__
int val__FMA4__[__FMA4__ + 1];
#endif
#ifdef __FP_FAST_FMA
int val__FP_FAST_FMA[__FP_FAST_FMA + 1];
#endif
#ifdef __USER_MSR__
int val__USER_MSR__[__USER_MSR__ + 1];
#endif
#ifdef __USERMSR__
int val__USERMSR__[__USERMSR__ + 1];
#endif

// SLATE-FILECHECK-BEGIN AMX
// AMX: module {
// AMX-NEXT:     target "x86_64-unknown-linux-gnu" {
// AMX-NEXT:         endian = little;
// AMX-NEXT:         pointer [size=8, align=8];
// AMX-NEXT:         stack_alignment = 16;
// AMX-NEXT:         long_double = f80;
// AMX-NEXT:         storage bool [size=1, align=1];
// AMX-NEXT:         storage i8, u8 [size=1, align=1];
// AMX-NEXT:         storage i16, u16 [size=2, align=2];
// AMX-NEXT:         storage i32, u32 [size=4, align=4];
// AMX-NEXT:         storage i64, u64 [size=8, align=8];
// AMX-NEXT:         storage i128, u128 [size=16, align=16];
// AMX-NEXT:         storage bf16 [size=2, align=2];
// AMX-NEXT:         storage f16 [size=2, align=2];
// AMX-NEXT:         storage f32 [size=4, align=4];
// AMX-NEXT:         storage f64 [size=8, align=8];
// AMX-NEXT:         storage f80 [size=16, align=16];
// AMX-NEXT:         storage f128 [size=16, align=16];
// AMX-NEXT:         storage d32 [size=4, align=4];
// AMX-NEXT:         storage d64 [size=8, align=8];
// AMX-NEXT:         storage d128 [size=16, align=16];
// AMX-NEXT:     }
// AMX-NEXT:     global %[[VALUE_val__AMX_BF16__:[0-9]+]] val__AMX_BF16__: array<i32, 2> [storage=static] [linkage=external];
// AMX-NEXT:     global %[[VALUE_val__AMX_TILE__:[0-9]+]] val__AMX_TILE__: array<i32, 2> [storage=static] [linkage=external];
// AMX-NEXT: }
// SLATE-FILECHECK-END AMX
// SLATE-FILECHECK-BEGIN VP2
// VP2: module {
// VP2-NEXT:     target "x86_64-unknown-linux-gnu" {
// VP2-NEXT:         endian = little;
// VP2-NEXT:         pointer [size=8, align=8];
// VP2-NEXT:         stack_alignment = 16;
// VP2-NEXT:         long_double = f80;
// VP2-NEXT:         storage bool [size=1, align=1];
// VP2-NEXT:         storage i8, u8 [size=1, align=1];
// VP2-NEXT:         storage i16, u16 [size=2, align=2];
// VP2-NEXT:         storage i32, u32 [size=4, align=4];
// VP2-NEXT:         storage i64, u64 [size=8, align=8];
// VP2-NEXT:         storage i128, u128 [size=16, align=16];
// VP2-NEXT:         storage bf16 [size=2, align=2];
// VP2-NEXT:         storage f16 [size=2, align=2];
// VP2-NEXT:         storage f32 [size=4, align=4];
// VP2-NEXT:         storage f64 [size=8, align=8];
// VP2-NEXT:         storage f80 [size=16, align=16];
// VP2-NEXT:         storage f128 [size=16, align=16];
// VP2-NEXT:         storage d32 [size=4, align=4];
// VP2-NEXT:         storage d64 [size=8, align=8];
// VP2-NEXT:         storage d128 [size=16, align=16];
// VP2-NEXT:     }
// VP2-NEXT:     global %[[VALUE_val__XSAVE__:[0-9]+]] val__XSAVE__: array<i32, 2> [storage=static] [linkage=external];
// VP2-NEXT:     global %[[VALUE_val__AVX512VP2INTERSECT__:[0-9]+]] val__AVX512VP2INTERSECT__: array<i32, 2> [storage=static] [linkage=external];
// VP2-NEXT:     global %[[VALUE_val__AVX512F__:[0-9]+]] val__AVX512F__: array<i32, 2> [storage=static] [linkage=external];
// VP2-NEXT:     global %[[VALUE_val__AVX__:[0-9]+]] val__AVX__: array<i32, 2> [storage=static] [linkage=external];
// VP2-NEXT:     global %[[VALUE_val__AVX2__:[0-9]+]] val__AVX2__: array<i32, 2> [storage=static] [linkage=external];
// VP2-NEXT:     global %[[VALUE_val__CRC32__:[0-9]+]] val__CRC32__: array<i32, 2> [storage=static] [linkage=external];
// VP2-NEXT:     global %[[VALUE_val__POPCNT__:[0-9]+]] val__POPCNT__: array<i32, 2> [storage=static] [linkage=external];
// VP2-NEXT:     global %[[VALUE_val__SSE4_2__:[0-9]+]] val__SSE4_2__: array<i32, 2> [storage=static] [linkage=external];
// VP2-NEXT:     global %[[VALUE_val__FP_FAST_FMA:[0-9]+]] val__FP_FAST_FMA: array<i32, 2> [storage=static] [linkage=external];
// VP2-NEXT: }
// SLATE-FILECHECK-END VP2
// SLATE-FILECHECK-BEGIN VAES
// VAES: module {
// VAES-NEXT:     target "x86_64-unknown-linux-gnu" {
// VAES-NEXT:         endian = little;
// VAES-NEXT:         pointer [size=8, align=8];
// VAES-NEXT:         stack_alignment = 16;
// VAES-NEXT:         long_double = f80;
// VAES-NEXT:         storage bool [size=1, align=1];
// VAES-NEXT:         storage i8, u8 [size=1, align=1];
// VAES-NEXT:         storage i16, u16 [size=2, align=2];
// VAES-NEXT:         storage i32, u32 [size=4, align=4];
// VAES-NEXT:         storage i64, u64 [size=8, align=8];
// VAES-NEXT:         storage i128, u128 [size=16, align=16];
// VAES-NEXT:         storage bf16 [size=2, align=2];
// VAES-NEXT:         storage f16 [size=2, align=2];
// VAES-NEXT:         storage f32 [size=4, align=4];
// VAES-NEXT:         storage f64 [size=8, align=8];
// VAES-NEXT:         storage f80 [size=16, align=16];
// VAES-NEXT:         storage f128 [size=16, align=16];
// VAES-NEXT:         storage d32 [size=4, align=4];
// VAES-NEXT:         storage d64 [size=8, align=8];
// VAES-NEXT:         storage d128 [size=16, align=16];
// VAES-NEXT:     }
// VAES-NEXT:     global %[[VALUE_val__VAES__:[0-9]+]] val__VAES__: array<i32, 2> [storage=static] [linkage=external];
// VAES-NEXT:     global %[[VALUE_val__CRC32__:[0-9]+]] val__CRC32__: array<i32, 2> [storage=static] [linkage=external];
// VAES-NEXT:     global %[[VALUE_val__POPCNT__:[0-9]+]] val__POPCNT__: array<i32, 2> [storage=static] [linkage=external];
// VAES-NEXT:     global %[[VALUE_val__SSE4_2__:[0-9]+]] val__SSE4_2__: array<i32, 2> [storage=static] [linkage=external];
// VAES-NEXT: }
// SLATE-FILECHECK-END VAES
// SLATE-FILECHECK-BEGIN V2-NO-SSE42
// V2-NO-SSE42: module {
// V2-NO-SSE42-NEXT:     target "x86_64-unknown-linux-gnu" {
// V2-NO-SSE42-NEXT:         endian = little;
// V2-NO-SSE42-NEXT:         pointer [size=8, align=8];
// V2-NO-SSE42-NEXT:         stack_alignment = 16;
// V2-NO-SSE42-NEXT:         long_double = f80;
// V2-NO-SSE42-NEXT:         storage bool [size=1, align=1];
// V2-NO-SSE42-NEXT:         storage i8, u8 [size=1, align=1];
// V2-NO-SSE42-NEXT:         storage i16, u16 [size=2, align=2];
// V2-NO-SSE42-NEXT:         storage i32, u32 [size=4, align=4];
// V2-NO-SSE42-NEXT:         storage i64, u64 [size=8, align=8];
// V2-NO-SSE42-NEXT:         storage i128, u128 [size=16, align=16];
// V2-NO-SSE42-NEXT:         storage bf16 [size=2, align=2];
// V2-NO-SSE42-NEXT:         storage f16 [size=2, align=2];
// V2-NO-SSE42-NEXT:         storage f32 [size=4, align=4];
// V2-NO-SSE42-NEXT:         storage f64 [size=8, align=8];
// V2-NO-SSE42-NEXT:         storage f80 [size=16, align=16];
// V2-NO-SSE42-NEXT:         storage f128 [size=16, align=16];
// V2-NO-SSE42-NEXT:         storage d32 [size=4, align=4];
// V2-NO-SSE42-NEXT:         storage d64 [size=8, align=8];
// V2-NO-SSE42-NEXT:         storage d128 [size=16, align=16];
// V2-NO-SSE42-NEXT:     }
// V2-NO-SSE42-NEXT:     global %[[VALUE_val__POPCNT__:[0-9]+]] val__POPCNT__: array<i32, 2> [storage=static] [linkage=external];
// V2-NO-SSE42-NEXT: }
// SLATE-FILECHECK-END V2-NO-SSE42
// SLATE-FILECHECK-BEGIN FMA4
// FMA4: module {
// FMA4-NEXT:     target "x86_64-unknown-linux-gnu" {
// FMA4-NEXT:         endian = little;
// FMA4-NEXT:         pointer [size=8, align=8];
// FMA4-NEXT:         stack_alignment = 16;
// FMA4-NEXT:         long_double = f80;
// FMA4-NEXT:         storage bool [size=1, align=1];
// FMA4-NEXT:         storage i8, u8 [size=1, align=1];
// FMA4-NEXT:         storage i16, u16 [size=2, align=2];
// FMA4-NEXT:         storage i32, u32 [size=4, align=4];
// FMA4-NEXT:         storage i64, u64 [size=8, align=8];
// FMA4-NEXT:         storage i128, u128 [size=16, align=16];
// FMA4-NEXT:         storage bf16 [size=2, align=2];
// FMA4-NEXT:         storage f16 [size=2, align=2];
// FMA4-NEXT:         storage f32 [size=4, align=4];
// FMA4-NEXT:         storage f64 [size=8, align=8];
// FMA4-NEXT:         storage f80 [size=16, align=16];
// FMA4-NEXT:         storage f128 [size=16, align=16];
// FMA4-NEXT:         storage d32 [size=4, align=4];
// FMA4-NEXT:         storage d64 [size=8, align=8];
// FMA4-NEXT:         storage d128 [size=16, align=16];
// FMA4-NEXT:     }
// FMA4-NEXT:     global %[[VALUE_val__XSAVE__:[0-9]+]] val__XSAVE__: array<i32, 2> [storage=static] [linkage=external];
// FMA4-NEXT:     global %[[VALUE_val__AVX__:[0-9]+]] val__AVX__: array<i32, 2> [storage=static] [linkage=external];
// FMA4-NEXT:     global %[[VALUE_val__CRC32__:[0-9]+]] val__CRC32__: array<i32, 2> [storage=static] [linkage=external];
// FMA4-NEXT:     global %[[VALUE_val__POPCNT__:[0-9]+]] val__POPCNT__: array<i32, 2> [storage=static] [linkage=external];
// FMA4-NEXT:     global %[[VALUE_val__SSE4_2__:[0-9]+]] val__SSE4_2__: array<i32, 2> [storage=static] [linkage=external];
// FMA4-NEXT:     global %[[VALUE_val__FMA4__:[0-9]+]] val__FMA4__: array<i32, 2> [storage=static] [linkage=external];
// FMA4-NEXT:     global %[[VALUE_val__FP_FAST_FMA:[0-9]+]] val__FP_FAST_FMA: array<i32, 2> [storage=static] [linkage=external];
// FMA4-NEXT: }
// SLATE-FILECHECK-END FMA4
// SLATE-FILECHECK-BEGIN USERMSR
// USERMSR: module {
// USERMSR-NEXT:     target "x86_64-unknown-linux-gnu" {
// USERMSR-NEXT:         endian = little;
// USERMSR-NEXT:         pointer [size=8, align=8];
// USERMSR-NEXT:         stack_alignment = 16;
// USERMSR-NEXT:         long_double = f80;
// USERMSR-NEXT:         storage bool [size=1, align=1];
// USERMSR-NEXT:         storage i8, u8 [size=1, align=1];
// USERMSR-NEXT:         storage i16, u16 [size=2, align=2];
// USERMSR-NEXT:         storage i32, u32 [size=4, align=4];
// USERMSR-NEXT:         storage i64, u64 [size=8, align=8];
// USERMSR-NEXT:         storage i128, u128 [size=16, align=16];
// USERMSR-NEXT:         storage bf16 [size=2, align=2];
// USERMSR-NEXT:         storage f16 [size=2, align=2];
// USERMSR-NEXT:         storage f32 [size=4, align=4];
// USERMSR-NEXT:         storage f64 [size=8, align=8];
// USERMSR-NEXT:         storage f80 [size=16, align=16];
// USERMSR-NEXT:         storage f128 [size=16, align=16];
// USERMSR-NEXT:         storage d32 [size=4, align=4];
// USERMSR-NEXT:         storage d64 [size=8, align=8];
// USERMSR-NEXT:         storage d128 [size=16, align=16];
// USERMSR-NEXT:     }
// USERMSR-NEXT:     global %[[VALUE_val__USER_MSR__:[0-9]+]] val__USER_MSR__: array<i32, 2> [storage=static] [linkage=external];
// USERMSR-NEXT: }
// SLATE-FILECHECK-END USERMSR
// SLATE-FILECHECK-BEGIN SHA512
// SHA512: module {
// SHA512-NEXT:     target "x86_64-unknown-linux-gnu" {
// SHA512-NEXT:         endian = little;
// SHA512-NEXT:         pointer [size=8, align=8];
// SHA512-NEXT:         stack_alignment = 16;
// SHA512-NEXT:         long_double = f80;
// SHA512-NEXT:         storage bool [size=1, align=1];
// SHA512-NEXT:         storage i8, u8 [size=1, align=1];
// SHA512-NEXT:         storage i16, u16 [size=2, align=2];
// SHA512-NEXT:         storage i32, u32 [size=4, align=4];
// SHA512-NEXT:         storage i64, u64 [size=8, align=8];
// SHA512-NEXT:         storage i128, u128 [size=16, align=16];
// SHA512-NEXT:         storage bf16 [size=2, align=2];
// SHA512-NEXT:         storage f16 [size=2, align=2];
// SHA512-NEXT:         storage f32 [size=4, align=4];
// SHA512-NEXT:         storage f64 [size=8, align=8];
// SHA512-NEXT:         storage f80 [size=16, align=16];
// SHA512-NEXT:         storage f128 [size=16, align=16];
// SHA512-NEXT:         storage d32 [size=4, align=4];
// SHA512-NEXT:         storage d64 [size=8, align=8];
// SHA512-NEXT:         storage d128 [size=16, align=16];
// SHA512-NEXT:     }
// SHA512-NEXT:     global %[[VALUE_val__XSAVE__:[0-9]+]] val__XSAVE__: array<i32, 2> [storage=static] [linkage=external];
// SHA512-NEXT:     global %[[VALUE_val__AVX__:[0-9]+]] val__AVX__: array<i32, 2> [storage=static] [linkage=external];
// SHA512-NEXT:     global %[[VALUE_val__SHA512__:[0-9]+]] val__SHA512__: array<i32, 2> [storage=static] [linkage=external];
// SHA512-NEXT:     global %[[VALUE_val__CRC32__:[0-9]+]] val__CRC32__: array<i32, 2> [storage=static] [linkage=external];
// SHA512-NEXT:     global %[[VALUE_val__POPCNT__:[0-9]+]] val__POPCNT__: array<i32, 2> [storage=static] [linkage=external];
// SHA512-NEXT:     global %[[VALUE_val__SSE4_2__:[0-9]+]] val__SSE4_2__: array<i32, 2> [storage=static] [linkage=external];
// SHA512-NEXT: }
// SLATE-FILECHECK-END SHA512
