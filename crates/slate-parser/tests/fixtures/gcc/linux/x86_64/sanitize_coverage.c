#ifdef __SANITIZE_COVERAGE__
int macro_coverage = 1;
#else
int macro_coverage = 0;
#endif
int feature_coverage = __has_feature(coverage_sanitizer);

// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-PREFIX-ARGS DEFAULT
// SLATE-FILECHECK-DEFINES TRACE
// SLATE-FILECHECK-PREFIX-ARGS TRACE -fsanitize-coverage=trace-pc,trace-cmp
// SLATE-FILECHECK-DEFINES NEGATED
// SLATE-FILECHECK-PREFIX-ARGS NEGATED -fsanitize-coverage=trace-pc -fno-sanitize-coverage=trace-pc

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
// DEFAULT-NEXT:     global %[[VALUE_macro_coverage:[0-9]+]] macro_coverage: i32 [storage=static] = const<i32>(0) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_feature_coverage:[0-9]+]] feature_coverage: i32 [storage=static] = const<i32>(0) [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN TRACE
// TRACE: module {
// TRACE-NEXT:     target "x86_64-unknown-linux-gnu" {
// TRACE-NEXT:         endian = little;
// TRACE-NEXT:         pointer [size=8, align=8];
// TRACE-NEXT:         stack_alignment = 16;
// TRACE-NEXT:         long_double = f80;
// TRACE-NEXT:         storage bool [size=1, align=1];
// TRACE-NEXT:         storage i8, u8 [size=1, align=1];
// TRACE-NEXT:         storage i16, u16 [size=2, align=2];
// TRACE-NEXT:         storage i32, u32 [size=4, align=4];
// TRACE-NEXT:         storage i64, u64 [size=8, align=8];
// TRACE-NEXT:         storage i128, u128 [size=16, align=16];
// TRACE-NEXT:         storage bf16 [size=2, align=2];
// TRACE-NEXT:         storage f16 [size=2, align=2];
// TRACE-NEXT:         storage f32 [size=4, align=4];
// TRACE-NEXT:         storage f64 [size=8, align=8];
// TRACE-NEXT:         storage f80 [size=16, align=16];
// TRACE-NEXT:         storage f128 [size=16, align=16];
// TRACE-NEXT:         storage d32 [size=4, align=4];
// TRACE-NEXT:         storage d64 [size=8, align=8];
// TRACE-NEXT:         storage d128 [size=16, align=16];
// TRACE-NEXT:     }
// TRACE-NEXT:     global %[[VALUE_macro_coverage:[0-9]+]] macro_coverage: i32 [storage=static] = const<i32>(0) [linkage=external];
// TRACE-NEXT:     global %[[VALUE_feature_coverage:[0-9]+]] feature_coverage: i32 [storage=static] = const<i32>(0) [linkage=external];
// TRACE-NEXT: }
// SLATE-FILECHECK-END TRACE
// SLATE-FILECHECK-BEGIN NEGATED
// NEGATED: module {
// NEGATED-NEXT:     target "x86_64-unknown-linux-gnu" {
// NEGATED-NEXT:         endian = little;
// NEGATED-NEXT:         pointer [size=8, align=8];
// NEGATED-NEXT:         stack_alignment = 16;
// NEGATED-NEXT:         long_double = f80;
// NEGATED-NEXT:         storage bool [size=1, align=1];
// NEGATED-NEXT:         storage i8, u8 [size=1, align=1];
// NEGATED-NEXT:         storage i16, u16 [size=2, align=2];
// NEGATED-NEXT:         storage i32, u32 [size=4, align=4];
// NEGATED-NEXT:         storage i64, u64 [size=8, align=8];
// NEGATED-NEXT:         storage i128, u128 [size=16, align=16];
// NEGATED-NEXT:         storage bf16 [size=2, align=2];
// NEGATED-NEXT:         storage f16 [size=2, align=2];
// NEGATED-NEXT:         storage f32 [size=4, align=4];
// NEGATED-NEXT:         storage f64 [size=8, align=8];
// NEGATED-NEXT:         storage f80 [size=16, align=16];
// NEGATED-NEXT:         storage f128 [size=16, align=16];
// NEGATED-NEXT:         storage d32 [size=4, align=4];
// NEGATED-NEXT:         storage d64 [size=8, align=8];
// NEGATED-NEXT:         storage d128 [size=16, align=16];
// NEGATED-NEXT:     }
// NEGATED-NEXT:     global %[[VALUE_macro_coverage:[0-9]+]] macro_coverage: i32 [storage=static] = const<i32>(0) [linkage=external];
// NEGATED-NEXT:     global %[[VALUE_feature_coverage:[0-9]+]] feature_coverage: i32 [storage=static] = const<i32>(0) [linkage=external];
// NEGATED-NEXT: }
// SLATE-FILECHECK-END NEGATED
