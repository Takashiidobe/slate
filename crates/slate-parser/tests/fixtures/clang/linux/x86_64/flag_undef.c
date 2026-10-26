// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-DEFINES UNDEF
// SLATE-FILECHECK-PREFIX-ARGS UNDEF -undef
// SLATE-FILECHECK-DEFINES OPTIONS
// SLATE-FILECHECK-PREFIX-ARGS OPTIONS -undef -std=c23 -O2 -ffast-math -fopenmp -pthread -fexceptions -DX
// SLATE-FILECHECK-DEFINES FREESTANDING
// SLATE-FILECHECK-PREFIX-ARGS FREESTANDING -undef -ffreestanding

#ifdef __STDC__
int val__STDC__[__STDC__ + 1];
#endif
#ifdef __STDC_VERSION__
int val__STDC_VERSION__[__STDC_VERSION__ + 1];
#endif
#ifdef __STDC_HOSTED__
int val__STDC_HOSTED__[__STDC_HOSTED__ + 1];
#endif
#ifdef __STDC_UTF_16__
int val__STDC_UTF_16__[__STDC_UTF_16__ + 1];
#endif
#ifdef __STDC_IEC_559__
int val__STDC_IEC_559__[__STDC_IEC_559__ + 1];
#endif
#ifdef _STDC_PREDEF_H
int val_STDC_PREDEF_H[_STDC_PREDEF_H + 1];
#endif
#ifdef __GNUC__
int val__GNUC__[__GNUC__ + 1];
#endif
#ifdef __clang__
int val__clang__[__clang__ + 1];
#endif
#ifdef __linux__
int val__linux__[__linux__ + 1];
#endif
#ifdef linux
int vallinux[linux + 1];
#endif
#ifdef __x86_64__
int val__x86_64__[__x86_64__ + 1];
#endif
#ifdef __OPTIMIZE__
int val__OPTIMIZE__[__OPTIMIZE__ + 1];
#endif
#ifdef __FAST_MATH__
int val__FAST_MATH__[__FAST_MATH__ + 1];
#endif
#ifdef _OPENMP
int val_OPENMP[_OPENMP + 1];
#endif
#ifdef _REENTRANT
int val_REENTRANT[_REENTRANT + 1];
#endif
#ifdef __EXCEPTIONS
int val__EXCEPTIONS[__EXCEPTIONS + 1];
#endif
#ifdef __CHAR8_TYPE__
int val__CHAR8_TYPE__[__CHAR8_TYPE__ + 1];
#endif
#ifdef X
int valX[X + 1];
#endif

// SLATE-FILECHECK-BEGIN UNDEF
// UNDEF: module {
// UNDEF-NEXT:     target "x86_64-unknown-linux-gnu" {
// UNDEF-NEXT:         endian = little;
// UNDEF-NEXT:         pointer [size=8, align=8];
// UNDEF-NEXT:         stack_alignment = 16;
// UNDEF-NEXT:         long_double = f80;
// UNDEF-NEXT:         storage bool [size=1, align=1];
// UNDEF-NEXT:         storage i8, u8 [size=1, align=1];
// UNDEF-NEXT:         storage i16, u16 [size=2, align=2];
// UNDEF-NEXT:         storage i32, u32 [size=4, align=4];
// UNDEF-NEXT:         storage i64, u64 [size=8, align=8];
// UNDEF-NEXT:         storage i128, u128 [size=16, align=16];
// UNDEF-NEXT:         storage bf16 [size=2, align=2];
// UNDEF-NEXT:         storage f16 [size=2, align=2];
// UNDEF-NEXT:         storage f32 [size=4, align=4];
// UNDEF-NEXT:         storage f64 [size=8, align=8];
// UNDEF-NEXT:         storage f80 [size=16, align=16];
// UNDEF-NEXT:         storage f128 [size=16, align=16];
// UNDEF-NEXT:         storage d32 [size=4, align=4];
// UNDEF-NEXT:         storage d64 [size=8, align=8];
// UNDEF-NEXT:         storage d128 [size=16, align=16];
// UNDEF-NEXT:     }
// UNDEF-NEXT:     global %[[VALUE_val__STDC__:[0-9]+]] val__STDC__: array<i32, 2> [storage=static] [linkage=external];
// UNDEF-NEXT:     global %[[VALUE_val__STDC_VERSION__:[0-9]+]] val__STDC_VERSION__: array<i32, 201711> [storage=static] [align=16] [linkage=external];
// UNDEF-NEXT:     global %[[VALUE_val__STDC_HOSTED__:[0-9]+]] val__STDC_HOSTED__: array<i32, 2> [storage=static] [linkage=external];
// UNDEF-NEXT:     global %[[VALUE_val__STDC_UTF_16__:[0-9]+]] val__STDC_UTF_16__: array<i32, 2> [storage=static] [linkage=external];
// UNDEF-NEXT: }
// SLATE-FILECHECK-END UNDEF
// SLATE-FILECHECK-BEGIN OPTIONS
// OPTIONS: module {
// OPTIONS-NEXT:     target "x86_64-unknown-linux-gnu" {
// OPTIONS-NEXT:         endian = little;
// OPTIONS-NEXT:         pointer [size=8, align=8];
// OPTIONS-NEXT:         stack_alignment = 16;
// OPTIONS-NEXT:         long_double = f80;
// OPTIONS-NEXT:         storage bool [size=1, align=1];
// OPTIONS-NEXT:         storage i8, u8 [size=1, align=1];
// OPTIONS-NEXT:         storage i16, u16 [size=2, align=2];
// OPTIONS-NEXT:         storage i32, u32 [size=4, align=4];
// OPTIONS-NEXT:         storage i64, u64 [size=8, align=8];
// OPTIONS-NEXT:         storage i128, u128 [size=16, align=16];
// OPTIONS-NEXT:         storage bf16 [size=2, align=2];
// OPTIONS-NEXT:         storage f16 [size=2, align=2];
// OPTIONS-NEXT:         storage f32 [size=4, align=4];
// OPTIONS-NEXT:         storage f64 [size=8, align=8];
// OPTIONS-NEXT:         storage f80 [size=16, align=16];
// OPTIONS-NEXT:         storage f128 [size=16, align=16];
// OPTIONS-NEXT:         storage d32 [size=4, align=4];
// OPTIONS-NEXT:         storage d64 [size=8, align=8];
// OPTIONS-NEXT:         storage d128 [size=16, align=16];
// OPTIONS-NEXT:     }
// OPTIONS-NEXT:     global %[[VALUE_val__STDC__:[0-9]+]] val__STDC__: array<i32, 2> [storage=static] [linkage=external];
// OPTIONS-NEXT:     global %[[VALUE_val__STDC_VERSION__:[0-9]+]] val__STDC_VERSION__: array<i32, 202312> [storage=static] [align=16] [linkage=external];
// OPTIONS-NEXT:     global %[[VALUE_val__STDC_HOSTED__:[0-9]+]] val__STDC_HOSTED__: array<i32, 2> [storage=static] [linkage=external];
// OPTIONS-NEXT:     global %[[VALUE_val__STDC_UTF_16__:[0-9]+]] val__STDC_UTF_16__: array<i32, 2> [storage=static] [linkage=external];
// OPTIONS-NEXT:     global %[[VALUE_valX:[0-9]+]] valX: array<i32, 2> [storage=static] [linkage=external];
// OPTIONS-NEXT: }
// SLATE-FILECHECK-END OPTIONS
// SLATE-FILECHECK-BEGIN FREESTANDING
// FREESTANDING: module {
// FREESTANDING-NEXT:     target "x86_64-unknown-linux-gnu" {
// FREESTANDING-NEXT:         endian = little;
// FREESTANDING-NEXT:         pointer [size=8, align=8];
// FREESTANDING-NEXT:         stack_alignment = 16;
// FREESTANDING-NEXT:         long_double = f80;
// FREESTANDING-NEXT:         storage bool [size=1, align=1];
// FREESTANDING-NEXT:         storage i8, u8 [size=1, align=1];
// FREESTANDING-NEXT:         storage i16, u16 [size=2, align=2];
// FREESTANDING-NEXT:         storage i32, u32 [size=4, align=4];
// FREESTANDING-NEXT:         storage i64, u64 [size=8, align=8];
// FREESTANDING-NEXT:         storage i128, u128 [size=16, align=16];
// FREESTANDING-NEXT:         storage bf16 [size=2, align=2];
// FREESTANDING-NEXT:         storage f16 [size=2, align=2];
// FREESTANDING-NEXT:         storage f32 [size=4, align=4];
// FREESTANDING-NEXT:         storage f64 [size=8, align=8];
// FREESTANDING-NEXT:         storage f80 [size=16, align=16];
// FREESTANDING-NEXT:         storage f128 [size=16, align=16];
// FREESTANDING-NEXT:         storage d32 [size=4, align=4];
// FREESTANDING-NEXT:         storage d64 [size=8, align=8];
// FREESTANDING-NEXT:         storage d128 [size=16, align=16];
// FREESTANDING-NEXT:     }
// FREESTANDING-NEXT:     global %[[VALUE_val__STDC__:[0-9]+]] val__STDC__: array<i32, 2> [storage=static] [linkage=external];
// FREESTANDING-NEXT:     global %[[VALUE_val__STDC_VERSION__:[0-9]+]] val__STDC_VERSION__: array<i32, 201711> [storage=static] [align=16] [linkage=external];
// FREESTANDING-NEXT:     global %[[VALUE_val__STDC_HOSTED__:[0-9]+]] val__STDC_HOSTED__: array<i32, 1> [storage=static] [linkage=external];
// FREESTANDING-NEXT:     global %[[VALUE_val__STDC_UTF_16__:[0-9]+]] val__STDC_UTF_16__: array<i32, 2> [storage=static] [linkage=external];
// FREESTANDING-NEXT: }
// SLATE-FILECHECK-END FREESTANDING
