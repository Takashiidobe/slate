// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-DEFINES V543
// SLATE-FILECHECK-PREFIX-ARGS V543 -fgnuc-version=5.4.3
// SLATE-FILECHECK-DEFINES V7
// SLATE-FILECHECK-PREFIX-ARGS V7 -fgnuc-version=7
// SLATE-FILECHECK-DEFINES ZERO
// SLATE-FILECHECK-PREFIX-ARGS ZERO -fgnuc-version=0
// SLATE-FILECHECK-DEFINES EMPTY-C99
// SLATE-FILECHECK-PREFIX-ARGS EMPTY-C99 -fgnuc-version= -std=c99
// SLATE-FILECHECK-DEFINES MINOR
// SLATE-FILECHECK-PREFIX-ARGS MINOR -fgnuc-version=0.1
// SLATE-FILECHECK-DEFINES MS-COMPAT
// SLATE-FILECHECK-PREFIX-ARGS MS-COMPAT -fms-compatibility -fgnuc-version=5
// SLATE-FILECHECK-DEFINES MS-ZERO
// SLATE-FILECHECK-PREFIX-ARGS MS-ZERO -fms-compatibility -fgnuc-version=0

#ifdef __GNUC__
int val__GNUC__[__GNUC__ + 1];
#endif
#ifdef __GNUC_MINOR__
int val__GNUC_MINOR__[__GNUC_MINOR__ + 1];
#endif
#ifdef __GNUC_PATCHLEVEL__
int val__GNUC_PATCHLEVEL__[__GNUC_PATCHLEVEL__ + 1];
#endif
#ifdef __GNUC_STDC_INLINE__
int val__GNUC_STDC_INLINE__[__GNUC_STDC_INLINE__ + 1];
#endif
#ifdef __GXX_ABI_VERSION
int val__GXX_ABI_VERSION[__GXX_ABI_VERSION + 1];
#endif
#ifdef __GCC_ATOMIC_INT_LOCK_FREE
int val__GCC_ATOMIC_INT_LOCK_FREE[__GCC_ATOMIC_INT_LOCK_FREE + 1];
#endif
#ifdef __STDC__
int val__STDC__[__STDC__ + 1];
#endif
#ifdef __STRICT_ANSI__
int val__STRICT_ANSI__[__STRICT_ANSI__ + 1];
#endif

// SLATE-FILECHECK-BEGIN V543
// V543: module {
// V543-NEXT:     target "x86_64-unknown-linux-gnu" {
// V543-NEXT:         endian = little;
// V543-NEXT:         pointer [size=8, align=8];
// V543-NEXT:         stack_alignment = 16;
// V543-NEXT:         long_double = f80;
// V543-NEXT:         storage bool [size=1, align=1];
// V543-NEXT:         storage i8, u8 [size=1, align=1];
// V543-NEXT:         storage i16, u16 [size=2, align=2];
// V543-NEXT:         storage i32, u32 [size=4, align=4];
// V543-NEXT:         storage i64, u64 [size=8, align=8];
// V543-NEXT:         storage i128, u128 [size=16, align=16];
// V543-NEXT:         storage bf16 [size=2, align=2];
// V543-NEXT:         storage f16 [size=2, align=2];
// V543-NEXT:         storage f32 [size=4, align=4];
// V543-NEXT:         storage f64 [size=8, align=8];
// V543-NEXT:         storage f80 [size=16, align=16];
// V543-NEXT:         storage f128 [size=16, align=16];
// V543-NEXT:         storage d32 [size=4, align=4];
// V543-NEXT:         storage d64 [size=8, align=8];
// V543-NEXT:         storage d128 [size=16, align=16];
// V543-NEXT:     }
// V543-NEXT:     global %[[VALUE_val__GNUC__:[0-9]+]] val__GNUC__: array<i32, 6> [storage=static] [align=16] [linkage=external];
// V543-NEXT:     global %[[VALUE_val__GNUC_MINOR__:[0-9]+]] val__GNUC_MINOR__: array<i32, 5> [storage=static] [align=16] [linkage=external];
// V543-NEXT:     global %[[VALUE_val__GNUC_PATCHLEVEL__:[0-9]+]] val__GNUC_PATCHLEVEL__: array<i32, 4> [storage=static] [align=16] [linkage=external];
// V543-NEXT:     global %[[VALUE_val__GNUC_STDC_INLINE__:[0-9]+]] val__GNUC_STDC_INLINE__: array<i32, 2> [storage=static] [linkage=external];
// V543-NEXT:     global %[[VALUE_val__GXX_ABI_VERSION:[0-9]+]] val__GXX_ABI_VERSION: array<i32, 1003> [storage=static] [align=16] [linkage=external];
// V543-NEXT:     global %[[VALUE_val__GCC_ATOMIC_INT_LOCK_FREE:[0-9]+]] val__GCC_ATOMIC_INT_LOCK_FREE: array<i32, 3> [storage=static] [linkage=external];
// V543-NEXT:     global %[[VALUE_val__STDC__:[0-9]+]] val__STDC__: array<i32, 2> [storage=static] [linkage=external];
// V543-NEXT: }
// SLATE-FILECHECK-END V543
// SLATE-FILECHECK-BEGIN V7
// V7: module {
// V7-NEXT:     target "x86_64-unknown-linux-gnu" {
// V7-NEXT:         endian = little;
// V7-NEXT:         pointer [size=8, align=8];
// V7-NEXT:         stack_alignment = 16;
// V7-NEXT:         long_double = f80;
// V7-NEXT:         storage bool [size=1, align=1];
// V7-NEXT:         storage i8, u8 [size=1, align=1];
// V7-NEXT:         storage i16, u16 [size=2, align=2];
// V7-NEXT:         storage i32, u32 [size=4, align=4];
// V7-NEXT:         storage i64, u64 [size=8, align=8];
// V7-NEXT:         storage i128, u128 [size=16, align=16];
// V7-NEXT:         storage bf16 [size=2, align=2];
// V7-NEXT:         storage f16 [size=2, align=2];
// V7-NEXT:         storage f32 [size=4, align=4];
// V7-NEXT:         storage f64 [size=8, align=8];
// V7-NEXT:         storage f80 [size=16, align=16];
// V7-NEXT:         storage f128 [size=16, align=16];
// V7-NEXT:         storage d32 [size=4, align=4];
// V7-NEXT:         storage d64 [size=8, align=8];
// V7-NEXT:         storage d128 [size=16, align=16];
// V7-NEXT:     }
// V7-NEXT:     global %[[VALUE_val__GNUC__:[0-9]+]] val__GNUC__: array<i32, 8> [storage=static] [align=16] [linkage=external];
// V7-NEXT:     global %[[VALUE_val__GNUC_MINOR__:[0-9]+]] val__GNUC_MINOR__: array<i32, 1> [storage=static] [linkage=external];
// V7-NEXT:     global %[[VALUE_val__GNUC_PATCHLEVEL__:[0-9]+]] val__GNUC_PATCHLEVEL__: array<i32, 1> [storage=static] [linkage=external];
// V7-NEXT:     global %[[VALUE_val__GNUC_STDC_INLINE__:[0-9]+]] val__GNUC_STDC_INLINE__: array<i32, 2> [storage=static] [linkage=external];
// V7-NEXT:     global %[[VALUE_val__GXX_ABI_VERSION:[0-9]+]] val__GXX_ABI_VERSION: array<i32, 1003> [storage=static] [align=16] [linkage=external];
// V7-NEXT:     global %[[VALUE_val__GCC_ATOMIC_INT_LOCK_FREE:[0-9]+]] val__GCC_ATOMIC_INT_LOCK_FREE: array<i32, 3> [storage=static] [linkage=external];
// V7-NEXT:     global %[[VALUE_val__STDC__:[0-9]+]] val__STDC__: array<i32, 2> [storage=static] [linkage=external];
// V7-NEXT: }
// SLATE-FILECHECK-END V7
// SLATE-FILECHECK-BEGIN ZERO
// ZERO: module {
// ZERO-NEXT:     target "x86_64-unknown-linux-gnu" {
// ZERO-NEXT:         endian = little;
// ZERO-NEXT:         pointer [size=8, align=8];
// ZERO-NEXT:         stack_alignment = 16;
// ZERO-NEXT:         long_double = f80;
// ZERO-NEXT:         storage bool [size=1, align=1];
// ZERO-NEXT:         storage i8, u8 [size=1, align=1];
// ZERO-NEXT:         storage i16, u16 [size=2, align=2];
// ZERO-NEXT:         storage i32, u32 [size=4, align=4];
// ZERO-NEXT:         storage i64, u64 [size=8, align=8];
// ZERO-NEXT:         storage i128, u128 [size=16, align=16];
// ZERO-NEXT:         storage bf16 [size=2, align=2];
// ZERO-NEXT:         storage f16 [size=2, align=2];
// ZERO-NEXT:         storage f32 [size=4, align=4];
// ZERO-NEXT:         storage f64 [size=8, align=8];
// ZERO-NEXT:         storage f80 [size=16, align=16];
// ZERO-NEXT:         storage f128 [size=16, align=16];
// ZERO-NEXT:         storage d32 [size=4, align=4];
// ZERO-NEXT:         storage d64 [size=8, align=8];
// ZERO-NEXT:         storage d128 [size=16, align=16];
// ZERO-NEXT:     }
// ZERO-NEXT:     global %[[VALUE_val__STDC__:[0-9]+]] val__STDC__: array<i32, 2> [storage=static] [linkage=external];
// ZERO-NEXT: }
// SLATE-FILECHECK-END ZERO
// SLATE-FILECHECK-BEGIN EMPTY-C99
// EMPTY-C99: module {
// EMPTY-C99-NEXT:     target "x86_64-unknown-linux-gnu" {
// EMPTY-C99-NEXT:         endian = little;
// EMPTY-C99-NEXT:         pointer [size=8, align=8];
// EMPTY-C99-NEXT:         stack_alignment = 16;
// EMPTY-C99-NEXT:         long_double = f80;
// EMPTY-C99-NEXT:         storage bool [size=1, align=1];
// EMPTY-C99-NEXT:         storage i8, u8 [size=1, align=1];
// EMPTY-C99-NEXT:         storage i16, u16 [size=2, align=2];
// EMPTY-C99-NEXT:         storage i32, u32 [size=4, align=4];
// EMPTY-C99-NEXT:         storage i64, u64 [size=8, align=8];
// EMPTY-C99-NEXT:         storage i128, u128 [size=16, align=16];
// EMPTY-C99-NEXT:         storage bf16 [size=2, align=2];
// EMPTY-C99-NEXT:         storage f16 [size=2, align=2];
// EMPTY-C99-NEXT:         storage f32 [size=4, align=4];
// EMPTY-C99-NEXT:         storage f64 [size=8, align=8];
// EMPTY-C99-NEXT:         storage f80 [size=16, align=16];
// EMPTY-C99-NEXT:         storage f128 [size=16, align=16];
// EMPTY-C99-NEXT:         storage d32 [size=4, align=4];
// EMPTY-C99-NEXT:         storage d64 [size=8, align=8];
// EMPTY-C99-NEXT:         storage d128 [size=16, align=16];
// EMPTY-C99-NEXT:     }
// EMPTY-C99-NEXT:     global %[[VALUE_val__STDC__:[0-9]+]] val__STDC__: array<i32, 2> [storage=static] [linkage=external];
// EMPTY-C99-NEXT:     global %[[VALUE_val__STRICT_ANSI__:[0-9]+]] val__STRICT_ANSI__: array<i32, 2> [storage=static] [linkage=external];
// EMPTY-C99-NEXT: }
// SLATE-FILECHECK-END EMPTY-C99
// SLATE-FILECHECK-BEGIN MINOR
// MINOR: module {
// MINOR-NEXT:     target "x86_64-unknown-linux-gnu" {
// MINOR-NEXT:         endian = little;
// MINOR-NEXT:         pointer [size=8, align=8];
// MINOR-NEXT:         stack_alignment = 16;
// MINOR-NEXT:         long_double = f80;
// MINOR-NEXT:         storage bool [size=1, align=1];
// MINOR-NEXT:         storage i8, u8 [size=1, align=1];
// MINOR-NEXT:         storage i16, u16 [size=2, align=2];
// MINOR-NEXT:         storage i32, u32 [size=4, align=4];
// MINOR-NEXT:         storage i64, u64 [size=8, align=8];
// MINOR-NEXT:         storage i128, u128 [size=16, align=16];
// MINOR-NEXT:         storage bf16 [size=2, align=2];
// MINOR-NEXT:         storage f16 [size=2, align=2];
// MINOR-NEXT:         storage f32 [size=4, align=4];
// MINOR-NEXT:         storage f64 [size=8, align=8];
// MINOR-NEXT:         storage f80 [size=16, align=16];
// MINOR-NEXT:         storage f128 [size=16, align=16];
// MINOR-NEXT:         storage d32 [size=4, align=4];
// MINOR-NEXT:         storage d64 [size=8, align=8];
// MINOR-NEXT:         storage d128 [size=16, align=16];
// MINOR-NEXT:     }
// MINOR-NEXT:     global %[[VALUE_val__GNUC__:[0-9]+]] val__GNUC__: array<i32, 1> [storage=static] [linkage=external];
// MINOR-NEXT:     global %[[VALUE_val__GNUC_MINOR__:[0-9]+]] val__GNUC_MINOR__: array<i32, 2> [storage=static] [linkage=external];
// MINOR-NEXT:     global %[[VALUE_val__GNUC_PATCHLEVEL__:[0-9]+]] val__GNUC_PATCHLEVEL__: array<i32, 1> [storage=static] [linkage=external];
// MINOR-NEXT:     global %[[VALUE_val__GNUC_STDC_INLINE__:[0-9]+]] val__GNUC_STDC_INLINE__: array<i32, 2> [storage=static] [linkage=external];
// MINOR-NEXT:     global %[[VALUE_val__GXX_ABI_VERSION:[0-9]+]] val__GXX_ABI_VERSION: array<i32, 1003> [storage=static] [align=16] [linkage=external];
// MINOR-NEXT:     global %[[VALUE_val__GCC_ATOMIC_INT_LOCK_FREE:[0-9]+]] val__GCC_ATOMIC_INT_LOCK_FREE: array<i32, 3> [storage=static] [linkage=external];
// MINOR-NEXT:     global %[[VALUE_val__STDC__:[0-9]+]] val__STDC__: array<i32, 2> [storage=static] [linkage=external];
// MINOR-NEXT: }
// SLATE-FILECHECK-END MINOR
// SLATE-FILECHECK-BEGIN MS-COMPAT
// MS-COMPAT: module {
// MS-COMPAT-NEXT:     target "x86_64-unknown-linux-gnu" {
// MS-COMPAT-NEXT:         endian = little;
// MS-COMPAT-NEXT:         pointer [size=8, align=8];
// MS-COMPAT-NEXT:         stack_alignment = 16;
// MS-COMPAT-NEXT:         long_double = f80;
// MS-COMPAT-NEXT:         storage bool [size=1, align=1];
// MS-COMPAT-NEXT:         storage i8, u8 [size=1, align=1];
// MS-COMPAT-NEXT:         storage i16, u16 [size=2, align=2];
// MS-COMPAT-NEXT:         storage i32, u32 [size=4, align=4];
// MS-COMPAT-NEXT:         storage i64, u64 [size=8, align=8];
// MS-COMPAT-NEXT:         storage i128, u128 [size=16, align=16];
// MS-COMPAT-NEXT:         storage bf16 [size=2, align=2];
// MS-COMPAT-NEXT:         storage f16 [size=2, align=2];
// MS-COMPAT-NEXT:         storage f32 [size=4, align=4];
// MS-COMPAT-NEXT:         storage f64 [size=8, align=8];
// MS-COMPAT-NEXT:         storage f80 [size=16, align=16];
// MS-COMPAT-NEXT:         storage f128 [size=16, align=16];
// MS-COMPAT-NEXT:         storage d32 [size=4, align=4];
// MS-COMPAT-NEXT:         storage d64 [size=8, align=8];
// MS-COMPAT-NEXT:         storage d128 [size=16, align=16];
// MS-COMPAT-NEXT:     }
// MS-COMPAT-NEXT:     global %[[VALUE_val__GNUC__:[0-9]+]] val__GNUC__: array<i32, 6> [storage=static] [align=16] [linkage=external];
// MS-COMPAT-NEXT:     global %[[VALUE_val__GNUC_MINOR__:[0-9]+]] val__GNUC_MINOR__: array<i32, 1> [storage=static] [linkage=external];
// MS-COMPAT-NEXT:     global %[[VALUE_val__GNUC_PATCHLEVEL__:[0-9]+]] val__GNUC_PATCHLEVEL__: array<i32, 1> [storage=static] [linkage=external];
// MS-COMPAT-NEXT:     global %[[VALUE_val__GNUC_STDC_INLINE__:[0-9]+]] val__GNUC_STDC_INLINE__: array<i32, 2> [storage=static] [linkage=external];
// MS-COMPAT-NEXT:     global %[[VALUE_val__GXX_ABI_VERSION:[0-9]+]] val__GXX_ABI_VERSION: array<i32, 1003> [storage=static] [align=16] [linkage=external];
// MS-COMPAT-NEXT:     global %[[VALUE_val__GCC_ATOMIC_INT_LOCK_FREE:[0-9]+]] val__GCC_ATOMIC_INT_LOCK_FREE: array<i32, 3> [storage=static] [linkage=external];
// MS-COMPAT-NEXT: }
// SLATE-FILECHECK-END MS-COMPAT
// SLATE-FILECHECK-BEGIN MS-ZERO
// MS-ZERO: module {
// MS-ZERO-NEXT:     target "x86_64-unknown-linux-gnu" {
// MS-ZERO-NEXT:         endian = little;
// MS-ZERO-NEXT:         pointer [size=8, align=8];
// MS-ZERO-NEXT:         stack_alignment = 16;
// MS-ZERO-NEXT:         long_double = f80;
// MS-ZERO-NEXT:         storage bool [size=1, align=1];
// MS-ZERO-NEXT:         storage i8, u8 [size=1, align=1];
// MS-ZERO-NEXT:         storage i16, u16 [size=2, align=2];
// MS-ZERO-NEXT:         storage i32, u32 [size=4, align=4];
// MS-ZERO-NEXT:         storage i64, u64 [size=8, align=8];
// MS-ZERO-NEXT:         storage i128, u128 [size=16, align=16];
// MS-ZERO-NEXT:         storage bf16 [size=2, align=2];
// MS-ZERO-NEXT:         storage f16 [size=2, align=2];
// MS-ZERO-NEXT:         storage f32 [size=4, align=4];
// MS-ZERO-NEXT:         storage f64 [size=8, align=8];
// MS-ZERO-NEXT:         storage f80 [size=16, align=16];
// MS-ZERO-NEXT:         storage f128 [size=16, align=16];
// MS-ZERO-NEXT:         storage d32 [size=4, align=4];
// MS-ZERO-NEXT:         storage d64 [size=8, align=8];
// MS-ZERO-NEXT:         storage d128 [size=16, align=16];
// MS-ZERO-NEXT:     }
// MS-ZERO-NEXT: }
// SLATE-FILECHECK-END MS-ZERO
