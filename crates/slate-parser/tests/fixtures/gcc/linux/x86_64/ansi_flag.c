// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-DEFINES ANSI
// SLATE-FILECHECK-STD ANSI gnu23
// SLATE-FILECHECK-PREFIX-ARGS ANSI -ansi
// SLATE-FILECHECK-DEFINES ANSI-THEN-STD
// SLATE-FILECHECK-STD ANSI-THEN-STD gnu23
// SLATE-FILECHECK-PREFIX-ARGS ANSI-THEN-STD -ansi -std=gnu17
// SLATE-FILECHECK-DEFINES STD-THEN-ANSI
// SLATE-FILECHECK-STD STD-THEN-ANSI gnu23
// SLATE-FILECHECK-PREFIX-ARGS STD-THEN-ANSI -std=gnu17 -ansi

#ifdef __STRICT_ANSI__
int strict_ansi;
#endif
#ifdef __STDC_VERSION__
int has_stdc_version;
#endif

// SLATE-FILECHECK-BEGIN ANSI
// ANSI: module {
// ANSI-NEXT:     target "x86_64-unknown-linux-gnu" {
// ANSI-NEXT:         endian = little;
// ANSI-NEXT:         pointer [size=8, align=8];
// ANSI-NEXT:         stack_alignment = 16;
// ANSI-NEXT:         long_double = f80;
// ANSI-NEXT:         storage bool [size=1, align=1];
// ANSI-NEXT:         storage i8, u8 [size=1, align=1];
// ANSI-NEXT:         storage i16, u16 [size=2, align=2];
// ANSI-NEXT:         storage i32, u32 [size=4, align=4];
// ANSI-NEXT:         storage i64, u64 [size=8, align=8];
// ANSI-NEXT:         storage i128, u128 [size=16, align=16];
// ANSI-NEXT:         storage bf16 [size=2, align=2];
// ANSI-NEXT:         storage f16 [size=2, align=2];
// ANSI-NEXT:         storage f32 [size=4, align=4];
// ANSI-NEXT:         storage f64 [size=8, align=8];
// ANSI-NEXT:         storage f80 [size=16, align=16];
// ANSI-NEXT:         storage f128 [size=16, align=16];
// ANSI-NEXT:         storage d32 [size=4, align=4];
// ANSI-NEXT:         storage d64 [size=8, align=8];
// ANSI-NEXT:         storage d128 [size=16, align=16];
// ANSI-NEXT:     }
// ANSI-NEXT:     global %[[VALUE_strict_ansi:[0-9]+]] strict_ansi: i32 [storage=static] [linkage=external];
// ANSI-NEXT: }
// SLATE-FILECHECK-END ANSI
// SLATE-FILECHECK-BEGIN ANSI-THEN-STD
// ANSI-THEN-STD: module {
// ANSI-THEN-STD-NEXT:     target "x86_64-unknown-linux-gnu" {
// ANSI-THEN-STD-NEXT:         endian = little;
// ANSI-THEN-STD-NEXT:         pointer [size=8, align=8];
// ANSI-THEN-STD-NEXT:         stack_alignment = 16;
// ANSI-THEN-STD-NEXT:         long_double = f80;
// ANSI-THEN-STD-NEXT:         storage bool [size=1, align=1];
// ANSI-THEN-STD-NEXT:         storage i8, u8 [size=1, align=1];
// ANSI-THEN-STD-NEXT:         storage i16, u16 [size=2, align=2];
// ANSI-THEN-STD-NEXT:         storage i32, u32 [size=4, align=4];
// ANSI-THEN-STD-NEXT:         storage i64, u64 [size=8, align=8];
// ANSI-THEN-STD-NEXT:         storage i128, u128 [size=16, align=16];
// ANSI-THEN-STD-NEXT:         storage bf16 [size=2, align=2];
// ANSI-THEN-STD-NEXT:         storage f16 [size=2, align=2];
// ANSI-THEN-STD-NEXT:         storage f32 [size=4, align=4];
// ANSI-THEN-STD-NEXT:         storage f64 [size=8, align=8];
// ANSI-THEN-STD-NEXT:         storage f80 [size=16, align=16];
// ANSI-THEN-STD-NEXT:         storage f128 [size=16, align=16];
// ANSI-THEN-STD-NEXT:         storage d32 [size=4, align=4];
// ANSI-THEN-STD-NEXT:         storage d64 [size=8, align=8];
// ANSI-THEN-STD-NEXT:         storage d128 [size=16, align=16];
// ANSI-THEN-STD-NEXT:     }
// ANSI-THEN-STD-NEXT:     global %[[VALUE_has_stdc_version:[0-9]+]] has_stdc_version: i32 [storage=static] [linkage=external];
// ANSI-THEN-STD-NEXT: }
// SLATE-FILECHECK-END ANSI-THEN-STD
// SLATE-FILECHECK-BEGIN STD-THEN-ANSI
// STD-THEN-ANSI: module {
// STD-THEN-ANSI-NEXT:     target "x86_64-unknown-linux-gnu" {
// STD-THEN-ANSI-NEXT:         endian = little;
// STD-THEN-ANSI-NEXT:         pointer [size=8, align=8];
// STD-THEN-ANSI-NEXT:         stack_alignment = 16;
// STD-THEN-ANSI-NEXT:         long_double = f80;
// STD-THEN-ANSI-NEXT:         storage bool [size=1, align=1];
// STD-THEN-ANSI-NEXT:         storage i8, u8 [size=1, align=1];
// STD-THEN-ANSI-NEXT:         storage i16, u16 [size=2, align=2];
// STD-THEN-ANSI-NEXT:         storage i32, u32 [size=4, align=4];
// STD-THEN-ANSI-NEXT:         storage i64, u64 [size=8, align=8];
// STD-THEN-ANSI-NEXT:         storage i128, u128 [size=16, align=16];
// STD-THEN-ANSI-NEXT:         storage bf16 [size=2, align=2];
// STD-THEN-ANSI-NEXT:         storage f16 [size=2, align=2];
// STD-THEN-ANSI-NEXT:         storage f32 [size=4, align=4];
// STD-THEN-ANSI-NEXT:         storage f64 [size=8, align=8];
// STD-THEN-ANSI-NEXT:         storage f80 [size=16, align=16];
// STD-THEN-ANSI-NEXT:         storage f128 [size=16, align=16];
// STD-THEN-ANSI-NEXT:         storage d32 [size=4, align=4];
// STD-THEN-ANSI-NEXT:         storage d64 [size=8, align=8];
// STD-THEN-ANSI-NEXT:         storage d128 [size=16, align=16];
// STD-THEN-ANSI-NEXT:     }
// STD-THEN-ANSI-NEXT:     global %[[VALUE_strict_ansi:[0-9]+]] strict_ansi: i32 [storage=static] [linkage=external];
// STD-THEN-ANSI-NEXT: }
// SLATE-FILECHECK-END STD-THEN-ANSI
