int feature_coverage = __has_feature(coverage_sanitizer);

// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-PREFIX-ARGS DEFAULT
// SLATE-FILECHECK-DEFINES TRACE_PC
// SLATE-FILECHECK-PREFIX-ARGS TRACE_PC -fsanitize-coverage=trace-pc
// SLATE-FILECHECK-DEFINES TRACE_CMP
// SLATE-FILECHECK-PREFIX-ARGS TRACE_CMP -fsanitize-coverage=trace-cmp
// SLATE-FILECHECK-DEFINES TRACE_DIV
// SLATE-FILECHECK-PREFIX-ARGS TRACE_DIV -fsanitize-coverage=trace-div,trace-gep,pc-table,no-prune
// SLATE-FILECHECK-DEFINES STACK_DEPTH
// SLATE-FILECHECK-PREFIX-ARGS STACK_DEPTH -fsanitize-coverage=stack-depth
// SLATE-FILECHECK-DEFINES LEGACY_EDGE
// SLATE-FILECHECK-PREFIX-ARGS LEGACY_EDGE -fsanitize-coverage=edge
// SLATE-FILECHECK-DEFINES LEGACY_TRACE_BB
// SLATE-FILECHECK-PREFIX-ARGS LEGACY_TRACE_BB -fsanitize-coverage=trace-bb,8bit-counters
// SLATE-FILECHECK-DEFINES LEVEL_ZERO
// SLATE-FILECHECK-PREFIX-ARGS LEVEL_ZERO -fsanitize-coverage=trace-pc -fsanitize-coverage=0
// SLATE-FILECHECK-DEFINES LEVEL_HEX
// SLATE-FILECHECK-PREFIX-ARGS LEVEL_HEX -fsanitize-coverage=trace-cmp -fsanitize-coverage=0x3
// SLATE-FILECHECK-DEFINES NO_TRACE_PC
// SLATE-FILECHECK-PREFIX-ARGS NO_TRACE_PC -fsanitize-coverage=trace-pc -fno-sanitize-coverage=trace-pc
// SLATE-FILECHECK-DEFINES NO_EDGE
// SLATE-FILECHECK-PREFIX-ARGS NO_EDGE -fsanitize-coverage=trace-pc -fno-sanitize-coverage=edge
// SLATE-FILECHECK-DEFINES EMPTY
// SLATE-FILECHECK-PREFIX-ARGS EMPTY -fsanitize-coverage=
// SLATE-FILECHECK-DEFINES FUZZER
// SLATE-FILECHECK-PREFIX-ARGS FUZZER -fsanitize=fuzzer-no-link
// SLATE-FILECHECK-DEFINES FUZZER_ZERO
// SLATE-FILECHECK-PREFIX-ARGS FUZZER_ZERO -fsanitize=fuzzer-no-link -fsanitize-coverage=0
// SLATE-FILECHECK-DEFINES FUZZER_STACK_DEPTH
// SLATE-FILECHECK-PREFIX-ARGS FUZZER_STACK_DEPTH -fsanitize=fuzzer-no-link -fno-sanitize-coverage=trace-cmp,inline-8bit-counters,indirect-calls
// SLATE-FILECHECK-DEFINES FUZZER_REMOVED
// SLATE-FILECHECK-PREFIX-ARGS FUZZER_REMOVED -fsanitize=fuzzer -fno-sanitize=fuzzer
// SLATE-FILECHECK-DEFINES SCUDO
// SLATE-FILECHECK-PREFIX-ARGS SCUDO -fsanitize=scudo -fsanitize-coverage=trace-pc
// SLATE-FILECHECK-DEFINES SCUDO_REMOVED
// SLATE-FILECHECK-PREFIX-ARGS SCUDO_REMOVED -fno-sanitize=scudo -fsanitize=scudo -fsanitize-coverage=trace-pc
// SLATE-FILECHECK-DEFINES FUZZER_SCUDO
// SLATE-FILECHECK-PREFIX-ARGS FUZZER_SCUDO -fsanitize=fuzzer-no-link -fsanitize=scudo

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
// DEFAULT-NEXT:     global %[[VALUE_feature_coverage:[0-9]+]] feature_coverage: i32 [storage=static] = const<i32>(0) [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN TRACE_PC
// TRACE_PC: module {
// TRACE_PC-NEXT:     target "x86_64-unknown-linux-gnu" {
// TRACE_PC-NEXT:         endian = little;
// TRACE_PC-NEXT:         pointer [size=8, align=8];
// TRACE_PC-NEXT:         stack_alignment = 16;
// TRACE_PC-NEXT:         long_double = f80;
// TRACE_PC-NEXT:         storage bool [size=1, align=1];
// TRACE_PC-NEXT:         storage i8, u8 [size=1, align=1];
// TRACE_PC-NEXT:         storage i16, u16 [size=2, align=2];
// TRACE_PC-NEXT:         storage i32, u32 [size=4, align=4];
// TRACE_PC-NEXT:         storage i64, u64 [size=8, align=8];
// TRACE_PC-NEXT:         storage i128, u128 [size=16, align=16];
// TRACE_PC-NEXT:         storage bf16 [size=2, align=2];
// TRACE_PC-NEXT:         storage f16 [size=2, align=2];
// TRACE_PC-NEXT:         storage f32 [size=4, align=4];
// TRACE_PC-NEXT:         storage f64 [size=8, align=8];
// TRACE_PC-NEXT:         storage f80 [size=16, align=16];
// TRACE_PC-NEXT:         storage f128 [size=16, align=16];
// TRACE_PC-NEXT:         storage d32 [size=4, align=4];
// TRACE_PC-NEXT:         storage d64 [size=8, align=8];
// TRACE_PC-NEXT:         storage d128 [size=16, align=16];
// TRACE_PC-NEXT:     }
// TRACE_PC-NEXT:     global %[[VALUE_feature_coverage:[0-9]+]] feature_coverage: i32 [storage=static] = const<i32>(1) [linkage=external];
// TRACE_PC-NEXT: }
// SLATE-FILECHECK-END TRACE_PC
// SLATE-FILECHECK-BEGIN TRACE_CMP
// TRACE_CMP: module {
// TRACE_CMP-NEXT:     target "x86_64-unknown-linux-gnu" {
// TRACE_CMP-NEXT:         endian = little;
// TRACE_CMP-NEXT:         pointer [size=8, align=8];
// TRACE_CMP-NEXT:         stack_alignment = 16;
// TRACE_CMP-NEXT:         long_double = f80;
// TRACE_CMP-NEXT:         storage bool [size=1, align=1];
// TRACE_CMP-NEXT:         storage i8, u8 [size=1, align=1];
// TRACE_CMP-NEXT:         storage i16, u16 [size=2, align=2];
// TRACE_CMP-NEXT:         storage i32, u32 [size=4, align=4];
// TRACE_CMP-NEXT:         storage i64, u64 [size=8, align=8];
// TRACE_CMP-NEXT:         storage i128, u128 [size=16, align=16];
// TRACE_CMP-NEXT:         storage bf16 [size=2, align=2];
// TRACE_CMP-NEXT:         storage f16 [size=2, align=2];
// TRACE_CMP-NEXT:         storage f32 [size=4, align=4];
// TRACE_CMP-NEXT:         storage f64 [size=8, align=8];
// TRACE_CMP-NEXT:         storage f80 [size=16, align=16];
// TRACE_CMP-NEXT:         storage f128 [size=16, align=16];
// TRACE_CMP-NEXT:         storage d32 [size=4, align=4];
// TRACE_CMP-NEXT:         storage d64 [size=8, align=8];
// TRACE_CMP-NEXT:         storage d128 [size=16, align=16];
// TRACE_CMP-NEXT:     }
// TRACE_CMP-NEXT:     global %[[VALUE_feature_coverage:[0-9]+]] feature_coverage: i32 [storage=static] = const<i32>(1) [linkage=external];
// TRACE_CMP-NEXT: }
// SLATE-FILECHECK-END TRACE_CMP
// SLATE-FILECHECK-BEGIN TRACE_DIV
// TRACE_DIV: module {
// TRACE_DIV-NEXT:     target "x86_64-unknown-linux-gnu" {
// TRACE_DIV-NEXT:         endian = little;
// TRACE_DIV-NEXT:         pointer [size=8, align=8];
// TRACE_DIV-NEXT:         stack_alignment = 16;
// TRACE_DIV-NEXT:         long_double = f80;
// TRACE_DIV-NEXT:         storage bool [size=1, align=1];
// TRACE_DIV-NEXT:         storage i8, u8 [size=1, align=1];
// TRACE_DIV-NEXT:         storage i16, u16 [size=2, align=2];
// TRACE_DIV-NEXT:         storage i32, u32 [size=4, align=4];
// TRACE_DIV-NEXT:         storage i64, u64 [size=8, align=8];
// TRACE_DIV-NEXT:         storage i128, u128 [size=16, align=16];
// TRACE_DIV-NEXT:         storage bf16 [size=2, align=2];
// TRACE_DIV-NEXT:         storage f16 [size=2, align=2];
// TRACE_DIV-NEXT:         storage f32 [size=4, align=4];
// TRACE_DIV-NEXT:         storage f64 [size=8, align=8];
// TRACE_DIV-NEXT:         storage f80 [size=16, align=16];
// TRACE_DIV-NEXT:         storage f128 [size=16, align=16];
// TRACE_DIV-NEXT:         storage d32 [size=4, align=4];
// TRACE_DIV-NEXT:         storage d64 [size=8, align=8];
// TRACE_DIV-NEXT:         storage d128 [size=16, align=16];
// TRACE_DIV-NEXT:     }
// TRACE_DIV-NEXT:     global %[[VALUE_feature_coverage:[0-9]+]] feature_coverage: i32 [storage=static] = const<i32>(0) [linkage=external];
// TRACE_DIV-NEXT: }
// SLATE-FILECHECK-END TRACE_DIV
// SLATE-FILECHECK-BEGIN STACK_DEPTH
// STACK_DEPTH: module {
// STACK_DEPTH-NEXT:     target "x86_64-unknown-linux-gnu" {
// STACK_DEPTH-NEXT:         endian = little;
// STACK_DEPTH-NEXT:         pointer [size=8, align=8];
// STACK_DEPTH-NEXT:         stack_alignment = 16;
// STACK_DEPTH-NEXT:         long_double = f80;
// STACK_DEPTH-NEXT:         storage bool [size=1, align=1];
// STACK_DEPTH-NEXT:         storage i8, u8 [size=1, align=1];
// STACK_DEPTH-NEXT:         storage i16, u16 [size=2, align=2];
// STACK_DEPTH-NEXT:         storage i32, u32 [size=4, align=4];
// STACK_DEPTH-NEXT:         storage i64, u64 [size=8, align=8];
// STACK_DEPTH-NEXT:         storage i128, u128 [size=16, align=16];
// STACK_DEPTH-NEXT:         storage bf16 [size=2, align=2];
// STACK_DEPTH-NEXT:         storage f16 [size=2, align=2];
// STACK_DEPTH-NEXT:         storage f32 [size=4, align=4];
// STACK_DEPTH-NEXT:         storage f64 [size=8, align=8];
// STACK_DEPTH-NEXT:         storage f80 [size=16, align=16];
// STACK_DEPTH-NEXT:         storage f128 [size=16, align=16];
// STACK_DEPTH-NEXT:         storage d32 [size=4, align=4];
// STACK_DEPTH-NEXT:         storage d64 [size=8, align=8];
// STACK_DEPTH-NEXT:         storage d128 [size=16, align=16];
// STACK_DEPTH-NEXT:     }
// STACK_DEPTH-NEXT:     global %[[VALUE_feature_coverage:[0-9]+]] feature_coverage: i32 [storage=static] = const<i32>(1) [linkage=external];
// STACK_DEPTH-NEXT: }
// SLATE-FILECHECK-END STACK_DEPTH
// SLATE-FILECHECK-BEGIN LEGACY_EDGE
// LEGACY_EDGE: module {
// LEGACY_EDGE-NEXT:     target "x86_64-unknown-linux-gnu" {
// LEGACY_EDGE-NEXT:         endian = little;
// LEGACY_EDGE-NEXT:         pointer [size=8, align=8];
// LEGACY_EDGE-NEXT:         stack_alignment = 16;
// LEGACY_EDGE-NEXT:         long_double = f80;
// LEGACY_EDGE-NEXT:         storage bool [size=1, align=1];
// LEGACY_EDGE-NEXT:         storage i8, u8 [size=1, align=1];
// LEGACY_EDGE-NEXT:         storage i16, u16 [size=2, align=2];
// LEGACY_EDGE-NEXT:         storage i32, u32 [size=4, align=4];
// LEGACY_EDGE-NEXT:         storage i64, u64 [size=8, align=8];
// LEGACY_EDGE-NEXT:         storage i128, u128 [size=16, align=16];
// LEGACY_EDGE-NEXT:         storage bf16 [size=2, align=2];
// LEGACY_EDGE-NEXT:         storage f16 [size=2, align=2];
// LEGACY_EDGE-NEXT:         storage f32 [size=4, align=4];
// LEGACY_EDGE-NEXT:         storage f64 [size=8, align=8];
// LEGACY_EDGE-NEXT:         storage f80 [size=16, align=16];
// LEGACY_EDGE-NEXT:         storage f128 [size=16, align=16];
// LEGACY_EDGE-NEXT:         storage d32 [size=4, align=4];
// LEGACY_EDGE-NEXT:         storage d64 [size=8, align=8];
// LEGACY_EDGE-NEXT:         storage d128 [size=16, align=16];
// LEGACY_EDGE-NEXT:     }
// LEGACY_EDGE-NEXT:     global %[[VALUE_feature_coverage:[0-9]+]] feature_coverage: i32 [storage=static] = const<i32>(1) [linkage=external];
// LEGACY_EDGE-NEXT: }
// SLATE-FILECHECK-END LEGACY_EDGE
// SLATE-FILECHECK-BEGIN LEGACY_TRACE_BB
// LEGACY_TRACE_BB: module {
// LEGACY_TRACE_BB-NEXT:     target "x86_64-unknown-linux-gnu" {
// LEGACY_TRACE_BB-NEXT:         endian = little;
// LEGACY_TRACE_BB-NEXT:         pointer [size=8, align=8];
// LEGACY_TRACE_BB-NEXT:         stack_alignment = 16;
// LEGACY_TRACE_BB-NEXT:         long_double = f80;
// LEGACY_TRACE_BB-NEXT:         storage bool [size=1, align=1];
// LEGACY_TRACE_BB-NEXT:         storage i8, u8 [size=1, align=1];
// LEGACY_TRACE_BB-NEXT:         storage i16, u16 [size=2, align=2];
// LEGACY_TRACE_BB-NEXT:         storage i32, u32 [size=4, align=4];
// LEGACY_TRACE_BB-NEXT:         storage i64, u64 [size=8, align=8];
// LEGACY_TRACE_BB-NEXT:         storage i128, u128 [size=16, align=16];
// LEGACY_TRACE_BB-NEXT:         storage bf16 [size=2, align=2];
// LEGACY_TRACE_BB-NEXT:         storage f16 [size=2, align=2];
// LEGACY_TRACE_BB-NEXT:         storage f32 [size=4, align=4];
// LEGACY_TRACE_BB-NEXT:         storage f64 [size=8, align=8];
// LEGACY_TRACE_BB-NEXT:         storage f80 [size=16, align=16];
// LEGACY_TRACE_BB-NEXT:         storage f128 [size=16, align=16];
// LEGACY_TRACE_BB-NEXT:         storage d32 [size=4, align=4];
// LEGACY_TRACE_BB-NEXT:         storage d64 [size=8, align=8];
// LEGACY_TRACE_BB-NEXT:         storage d128 [size=16, align=16];
// LEGACY_TRACE_BB-NEXT:     }
// LEGACY_TRACE_BB-NEXT:     global %[[VALUE_feature_coverage:[0-9]+]] feature_coverage: i32 [storage=static] = const<i32>(0) [linkage=external];
// LEGACY_TRACE_BB-NEXT: }
// SLATE-FILECHECK-END LEGACY_TRACE_BB
// SLATE-FILECHECK-BEGIN LEVEL_ZERO
// LEVEL_ZERO: module {
// LEVEL_ZERO-NEXT:     target "x86_64-unknown-linux-gnu" {
// LEVEL_ZERO-NEXT:         endian = little;
// LEVEL_ZERO-NEXT:         pointer [size=8, align=8];
// LEVEL_ZERO-NEXT:         stack_alignment = 16;
// LEVEL_ZERO-NEXT:         long_double = f80;
// LEVEL_ZERO-NEXT:         storage bool [size=1, align=1];
// LEVEL_ZERO-NEXT:         storage i8, u8 [size=1, align=1];
// LEVEL_ZERO-NEXT:         storage i16, u16 [size=2, align=2];
// LEVEL_ZERO-NEXT:         storage i32, u32 [size=4, align=4];
// LEVEL_ZERO-NEXT:         storage i64, u64 [size=8, align=8];
// LEVEL_ZERO-NEXT:         storage i128, u128 [size=16, align=16];
// LEVEL_ZERO-NEXT:         storage bf16 [size=2, align=2];
// LEVEL_ZERO-NEXT:         storage f16 [size=2, align=2];
// LEVEL_ZERO-NEXT:         storage f32 [size=4, align=4];
// LEVEL_ZERO-NEXT:         storage f64 [size=8, align=8];
// LEVEL_ZERO-NEXT:         storage f80 [size=16, align=16];
// LEVEL_ZERO-NEXT:         storage f128 [size=16, align=16];
// LEVEL_ZERO-NEXT:         storage d32 [size=4, align=4];
// LEVEL_ZERO-NEXT:         storage d64 [size=8, align=8];
// LEVEL_ZERO-NEXT:         storage d128 [size=16, align=16];
// LEVEL_ZERO-NEXT:     }
// LEVEL_ZERO-NEXT:     global %[[VALUE_feature_coverage:[0-9]+]] feature_coverage: i32 [storage=static] = const<i32>(0) [linkage=external];
// LEVEL_ZERO-NEXT: }
// SLATE-FILECHECK-END LEVEL_ZERO
// SLATE-FILECHECK-BEGIN LEVEL_HEX
// LEVEL_HEX: module {
// LEVEL_HEX-NEXT:     target "x86_64-unknown-linux-gnu" {
// LEVEL_HEX-NEXT:         endian = little;
// LEVEL_HEX-NEXT:         pointer [size=8, align=8];
// LEVEL_HEX-NEXT:         stack_alignment = 16;
// LEVEL_HEX-NEXT:         long_double = f80;
// LEVEL_HEX-NEXT:         storage bool [size=1, align=1];
// LEVEL_HEX-NEXT:         storage i8, u8 [size=1, align=1];
// LEVEL_HEX-NEXT:         storage i16, u16 [size=2, align=2];
// LEVEL_HEX-NEXT:         storage i32, u32 [size=4, align=4];
// LEVEL_HEX-NEXT:         storage i64, u64 [size=8, align=8];
// LEVEL_HEX-NEXT:         storage i128, u128 [size=16, align=16];
// LEVEL_HEX-NEXT:         storage bf16 [size=2, align=2];
// LEVEL_HEX-NEXT:         storage f16 [size=2, align=2];
// LEVEL_HEX-NEXT:         storage f32 [size=4, align=4];
// LEVEL_HEX-NEXT:         storage f64 [size=8, align=8];
// LEVEL_HEX-NEXT:         storage f80 [size=16, align=16];
// LEVEL_HEX-NEXT:         storage f128 [size=16, align=16];
// LEVEL_HEX-NEXT:         storage d32 [size=4, align=4];
// LEVEL_HEX-NEXT:         storage d64 [size=8, align=8];
// LEVEL_HEX-NEXT:         storage d128 [size=16, align=16];
// LEVEL_HEX-NEXT:     }
// LEVEL_HEX-NEXT:     global %[[VALUE_feature_coverage:[0-9]+]] feature_coverage: i32 [storage=static] = const<i32>(0) [linkage=external];
// LEVEL_HEX-NEXT: }
// SLATE-FILECHECK-END LEVEL_HEX
// SLATE-FILECHECK-BEGIN NO_TRACE_PC
// NO_TRACE_PC: module {
// NO_TRACE_PC-NEXT:     target "x86_64-unknown-linux-gnu" {
// NO_TRACE_PC-NEXT:         endian = little;
// NO_TRACE_PC-NEXT:         pointer [size=8, align=8];
// NO_TRACE_PC-NEXT:         stack_alignment = 16;
// NO_TRACE_PC-NEXT:         long_double = f80;
// NO_TRACE_PC-NEXT:         storage bool [size=1, align=1];
// NO_TRACE_PC-NEXT:         storage i8, u8 [size=1, align=1];
// NO_TRACE_PC-NEXT:         storage i16, u16 [size=2, align=2];
// NO_TRACE_PC-NEXT:         storage i32, u32 [size=4, align=4];
// NO_TRACE_PC-NEXT:         storage i64, u64 [size=8, align=8];
// NO_TRACE_PC-NEXT:         storage i128, u128 [size=16, align=16];
// NO_TRACE_PC-NEXT:         storage bf16 [size=2, align=2];
// NO_TRACE_PC-NEXT:         storage f16 [size=2, align=2];
// NO_TRACE_PC-NEXT:         storage f32 [size=4, align=4];
// NO_TRACE_PC-NEXT:         storage f64 [size=8, align=8];
// NO_TRACE_PC-NEXT:         storage f80 [size=16, align=16];
// NO_TRACE_PC-NEXT:         storage f128 [size=16, align=16];
// NO_TRACE_PC-NEXT:         storage d32 [size=4, align=4];
// NO_TRACE_PC-NEXT:         storage d64 [size=8, align=8];
// NO_TRACE_PC-NEXT:         storage d128 [size=16, align=16];
// NO_TRACE_PC-NEXT:     }
// NO_TRACE_PC-NEXT:     global %[[VALUE_feature_coverage:[0-9]+]] feature_coverage: i32 [storage=static] = const<i32>(0) [linkage=external];
// NO_TRACE_PC-NEXT: }
// SLATE-FILECHECK-END NO_TRACE_PC
// SLATE-FILECHECK-BEGIN NO_EDGE
// NO_EDGE: module {
// NO_EDGE-NEXT:     target "x86_64-unknown-linux-gnu" {
// NO_EDGE-NEXT:         endian = little;
// NO_EDGE-NEXT:         pointer [size=8, align=8];
// NO_EDGE-NEXT:         stack_alignment = 16;
// NO_EDGE-NEXT:         long_double = f80;
// NO_EDGE-NEXT:         storage bool [size=1, align=1];
// NO_EDGE-NEXT:         storage i8, u8 [size=1, align=1];
// NO_EDGE-NEXT:         storage i16, u16 [size=2, align=2];
// NO_EDGE-NEXT:         storage i32, u32 [size=4, align=4];
// NO_EDGE-NEXT:         storage i64, u64 [size=8, align=8];
// NO_EDGE-NEXT:         storage i128, u128 [size=16, align=16];
// NO_EDGE-NEXT:         storage bf16 [size=2, align=2];
// NO_EDGE-NEXT:         storage f16 [size=2, align=2];
// NO_EDGE-NEXT:         storage f32 [size=4, align=4];
// NO_EDGE-NEXT:         storage f64 [size=8, align=8];
// NO_EDGE-NEXT:         storage f80 [size=16, align=16];
// NO_EDGE-NEXT:         storage f128 [size=16, align=16];
// NO_EDGE-NEXT:         storage d32 [size=4, align=4];
// NO_EDGE-NEXT:         storage d64 [size=8, align=8];
// NO_EDGE-NEXT:         storage d128 [size=16, align=16];
// NO_EDGE-NEXT:     }
// NO_EDGE-NEXT:     global %[[VALUE_feature_coverage:[0-9]+]] feature_coverage: i32 [storage=static] = const<i32>(1) [linkage=external];
// NO_EDGE-NEXT: }
// SLATE-FILECHECK-END NO_EDGE
// SLATE-FILECHECK-BEGIN EMPTY
// EMPTY: module {
// EMPTY-NEXT:     target "x86_64-unknown-linux-gnu" {
// EMPTY-NEXT:         endian = little;
// EMPTY-NEXT:         pointer [size=8, align=8];
// EMPTY-NEXT:         stack_alignment = 16;
// EMPTY-NEXT:         long_double = f80;
// EMPTY-NEXT:         storage bool [size=1, align=1];
// EMPTY-NEXT:         storage i8, u8 [size=1, align=1];
// EMPTY-NEXT:         storage i16, u16 [size=2, align=2];
// EMPTY-NEXT:         storage i32, u32 [size=4, align=4];
// EMPTY-NEXT:         storage i64, u64 [size=8, align=8];
// EMPTY-NEXT:         storage i128, u128 [size=16, align=16];
// EMPTY-NEXT:         storage bf16 [size=2, align=2];
// EMPTY-NEXT:         storage f16 [size=2, align=2];
// EMPTY-NEXT:         storage f32 [size=4, align=4];
// EMPTY-NEXT:         storage f64 [size=8, align=8];
// EMPTY-NEXT:         storage f80 [size=16, align=16];
// EMPTY-NEXT:         storage f128 [size=16, align=16];
// EMPTY-NEXT:         storage d32 [size=4, align=4];
// EMPTY-NEXT:         storage d64 [size=8, align=8];
// EMPTY-NEXT:         storage d128 [size=16, align=16];
// EMPTY-NEXT:     }
// EMPTY-NEXT:     global %[[VALUE_feature_coverage:[0-9]+]] feature_coverage: i32 [storage=static] = const<i32>(0) [linkage=external];
// EMPTY-NEXT: }
// SLATE-FILECHECK-END EMPTY
// SLATE-FILECHECK-BEGIN FUZZER
// FUZZER: module {
// FUZZER-NEXT:     target "x86_64-unknown-linux-gnu" {
// FUZZER-NEXT:         endian = little;
// FUZZER-NEXT:         pointer [size=8, align=8];
// FUZZER-NEXT:         stack_alignment = 16;
// FUZZER-NEXT:         long_double = f80;
// FUZZER-NEXT:         storage bool [size=1, align=1];
// FUZZER-NEXT:         storage i8, u8 [size=1, align=1];
// FUZZER-NEXT:         storage i16, u16 [size=2, align=2];
// FUZZER-NEXT:         storage i32, u32 [size=4, align=4];
// FUZZER-NEXT:         storage i64, u64 [size=8, align=8];
// FUZZER-NEXT:         storage i128, u128 [size=16, align=16];
// FUZZER-NEXT:         storage bf16 [size=2, align=2];
// FUZZER-NEXT:         storage f16 [size=2, align=2];
// FUZZER-NEXT:         storage f32 [size=4, align=4];
// FUZZER-NEXT:         storage f64 [size=8, align=8];
// FUZZER-NEXT:         storage f80 [size=16, align=16];
// FUZZER-NEXT:         storage f128 [size=16, align=16];
// FUZZER-NEXT:         storage d32 [size=4, align=4];
// FUZZER-NEXT:         storage d64 [size=8, align=8];
// FUZZER-NEXT:         storage d128 [size=16, align=16];
// FUZZER-NEXT:     }
// FUZZER-NEXT:     global %[[VALUE_feature_coverage:[0-9]+]] feature_coverage: i32 [storage=static] = const<i32>(1) [linkage=external];
// FUZZER-NEXT: }
// SLATE-FILECHECK-END FUZZER
// SLATE-FILECHECK-BEGIN FUZZER_ZERO
// FUZZER_ZERO: module {
// FUZZER_ZERO-NEXT:     target "x86_64-unknown-linux-gnu" {
// FUZZER_ZERO-NEXT:         endian = little;
// FUZZER_ZERO-NEXT:         pointer [size=8, align=8];
// FUZZER_ZERO-NEXT:         stack_alignment = 16;
// FUZZER_ZERO-NEXT:         long_double = f80;
// FUZZER_ZERO-NEXT:         storage bool [size=1, align=1];
// FUZZER_ZERO-NEXT:         storage i8, u8 [size=1, align=1];
// FUZZER_ZERO-NEXT:         storage i16, u16 [size=2, align=2];
// FUZZER_ZERO-NEXT:         storage i32, u32 [size=4, align=4];
// FUZZER_ZERO-NEXT:         storage i64, u64 [size=8, align=8];
// FUZZER_ZERO-NEXT:         storage i128, u128 [size=16, align=16];
// FUZZER_ZERO-NEXT:         storage bf16 [size=2, align=2];
// FUZZER_ZERO-NEXT:         storage f16 [size=2, align=2];
// FUZZER_ZERO-NEXT:         storage f32 [size=4, align=4];
// FUZZER_ZERO-NEXT:         storage f64 [size=8, align=8];
// FUZZER_ZERO-NEXT:         storage f80 [size=16, align=16];
// FUZZER_ZERO-NEXT:         storage f128 [size=16, align=16];
// FUZZER_ZERO-NEXT:         storage d32 [size=4, align=4];
// FUZZER_ZERO-NEXT:         storage d64 [size=8, align=8];
// FUZZER_ZERO-NEXT:         storage d128 [size=16, align=16];
// FUZZER_ZERO-NEXT:     }
// FUZZER_ZERO-NEXT:     global %[[VALUE_feature_coverage:[0-9]+]] feature_coverage: i32 [storage=static] = const<i32>(0) [linkage=external];
// FUZZER_ZERO-NEXT: }
// SLATE-FILECHECK-END FUZZER_ZERO
// SLATE-FILECHECK-BEGIN FUZZER_STACK_DEPTH
// FUZZER_STACK_DEPTH: module {
// FUZZER_STACK_DEPTH-NEXT:     target "x86_64-unknown-linux-gnu" {
// FUZZER_STACK_DEPTH-NEXT:         endian = little;
// FUZZER_STACK_DEPTH-NEXT:         pointer [size=8, align=8];
// FUZZER_STACK_DEPTH-NEXT:         stack_alignment = 16;
// FUZZER_STACK_DEPTH-NEXT:         long_double = f80;
// FUZZER_STACK_DEPTH-NEXT:         storage bool [size=1, align=1];
// FUZZER_STACK_DEPTH-NEXT:         storage i8, u8 [size=1, align=1];
// FUZZER_STACK_DEPTH-NEXT:         storage i16, u16 [size=2, align=2];
// FUZZER_STACK_DEPTH-NEXT:         storage i32, u32 [size=4, align=4];
// FUZZER_STACK_DEPTH-NEXT:         storage i64, u64 [size=8, align=8];
// FUZZER_STACK_DEPTH-NEXT:         storage i128, u128 [size=16, align=16];
// FUZZER_STACK_DEPTH-NEXT:         storage bf16 [size=2, align=2];
// FUZZER_STACK_DEPTH-NEXT:         storage f16 [size=2, align=2];
// FUZZER_STACK_DEPTH-NEXT:         storage f32 [size=4, align=4];
// FUZZER_STACK_DEPTH-NEXT:         storage f64 [size=8, align=8];
// FUZZER_STACK_DEPTH-NEXT:         storage f80 [size=16, align=16];
// FUZZER_STACK_DEPTH-NEXT:         storage f128 [size=16, align=16];
// FUZZER_STACK_DEPTH-NEXT:         storage d32 [size=4, align=4];
// FUZZER_STACK_DEPTH-NEXT:         storage d64 [size=8, align=8];
// FUZZER_STACK_DEPTH-NEXT:         storage d128 [size=16, align=16];
// FUZZER_STACK_DEPTH-NEXT:     }
// FUZZER_STACK_DEPTH-NEXT:     global %[[VALUE_feature_coverage:[0-9]+]] feature_coverage: i32 [storage=static] = const<i32>(1) [linkage=external];
// FUZZER_STACK_DEPTH-NEXT: }
// SLATE-FILECHECK-END FUZZER_STACK_DEPTH
// SLATE-FILECHECK-BEGIN FUZZER_REMOVED
// FUZZER_REMOVED: module {
// FUZZER_REMOVED-NEXT:     target "x86_64-unknown-linux-gnu" {
// FUZZER_REMOVED-NEXT:         endian = little;
// FUZZER_REMOVED-NEXT:         pointer [size=8, align=8];
// FUZZER_REMOVED-NEXT:         stack_alignment = 16;
// FUZZER_REMOVED-NEXT:         long_double = f80;
// FUZZER_REMOVED-NEXT:         storage bool [size=1, align=1];
// FUZZER_REMOVED-NEXT:         storage i8, u8 [size=1, align=1];
// FUZZER_REMOVED-NEXT:         storage i16, u16 [size=2, align=2];
// FUZZER_REMOVED-NEXT:         storage i32, u32 [size=4, align=4];
// FUZZER_REMOVED-NEXT:         storage i64, u64 [size=8, align=8];
// FUZZER_REMOVED-NEXT:         storage i128, u128 [size=16, align=16];
// FUZZER_REMOVED-NEXT:         storage bf16 [size=2, align=2];
// FUZZER_REMOVED-NEXT:         storage f16 [size=2, align=2];
// FUZZER_REMOVED-NEXT:         storage f32 [size=4, align=4];
// FUZZER_REMOVED-NEXT:         storage f64 [size=8, align=8];
// FUZZER_REMOVED-NEXT:         storage f80 [size=16, align=16];
// FUZZER_REMOVED-NEXT:         storage f128 [size=16, align=16];
// FUZZER_REMOVED-NEXT:         storage d32 [size=4, align=4];
// FUZZER_REMOVED-NEXT:         storage d64 [size=8, align=8];
// FUZZER_REMOVED-NEXT:         storage d128 [size=16, align=16];
// FUZZER_REMOVED-NEXT:     }
// FUZZER_REMOVED-NEXT:     global %[[VALUE_feature_coverage:[0-9]+]] feature_coverage: i32 [storage=static] = const<i32>(0) [linkage=external];
// FUZZER_REMOVED-NEXT: }
// SLATE-FILECHECK-END FUZZER_REMOVED
// SLATE-FILECHECK-BEGIN SCUDO
// SCUDO: module {
// SCUDO-NEXT:     target "x86_64-unknown-linux-gnu" {
// SCUDO-NEXT:         endian = little;
// SCUDO-NEXT:         pointer [size=8, align=8];
// SCUDO-NEXT:         stack_alignment = 16;
// SCUDO-NEXT:         long_double = f80;
// SCUDO-NEXT:         storage bool [size=1, align=1];
// SCUDO-NEXT:         storage i8, u8 [size=1, align=1];
// SCUDO-NEXT:         storage i16, u16 [size=2, align=2];
// SCUDO-NEXT:         storage i32, u32 [size=4, align=4];
// SCUDO-NEXT:         storage i64, u64 [size=8, align=8];
// SCUDO-NEXT:         storage i128, u128 [size=16, align=16];
// SCUDO-NEXT:         storage bf16 [size=2, align=2];
// SCUDO-NEXT:         storage f16 [size=2, align=2];
// SCUDO-NEXT:         storage f32 [size=4, align=4];
// SCUDO-NEXT:         storage f64 [size=8, align=8];
// SCUDO-NEXT:         storage f80 [size=16, align=16];
// SCUDO-NEXT:         storage f128 [size=16, align=16];
// SCUDO-NEXT:         storage d32 [size=4, align=4];
// SCUDO-NEXT:         storage d64 [size=8, align=8];
// SCUDO-NEXT:         storage d128 [size=16, align=16];
// SCUDO-NEXT:     }
// SCUDO-NEXT:     global %[[VALUE_feature_coverage:[0-9]+]] feature_coverage: i32 [storage=static] = const<i32>(0) [linkage=external];
// SCUDO-NEXT: }
// SLATE-FILECHECK-END SCUDO
// SLATE-FILECHECK-BEGIN SCUDO_REMOVED
// SCUDO_REMOVED: module {
// SCUDO_REMOVED-NEXT:     target "x86_64-unknown-linux-gnu" {
// SCUDO_REMOVED-NEXT:         endian = little;
// SCUDO_REMOVED-NEXT:         pointer [size=8, align=8];
// SCUDO_REMOVED-NEXT:         stack_alignment = 16;
// SCUDO_REMOVED-NEXT:         long_double = f80;
// SCUDO_REMOVED-NEXT:         storage bool [size=1, align=1];
// SCUDO_REMOVED-NEXT:         storage i8, u8 [size=1, align=1];
// SCUDO_REMOVED-NEXT:         storage i16, u16 [size=2, align=2];
// SCUDO_REMOVED-NEXT:         storage i32, u32 [size=4, align=4];
// SCUDO_REMOVED-NEXT:         storage i64, u64 [size=8, align=8];
// SCUDO_REMOVED-NEXT:         storage i128, u128 [size=16, align=16];
// SCUDO_REMOVED-NEXT:         storage bf16 [size=2, align=2];
// SCUDO_REMOVED-NEXT:         storage f16 [size=2, align=2];
// SCUDO_REMOVED-NEXT:         storage f32 [size=4, align=4];
// SCUDO_REMOVED-NEXT:         storage f64 [size=8, align=8];
// SCUDO_REMOVED-NEXT:         storage f80 [size=16, align=16];
// SCUDO_REMOVED-NEXT:         storage f128 [size=16, align=16];
// SCUDO_REMOVED-NEXT:         storage d32 [size=4, align=4];
// SCUDO_REMOVED-NEXT:         storage d64 [size=8, align=8];
// SCUDO_REMOVED-NEXT:         storage d128 [size=16, align=16];
// SCUDO_REMOVED-NEXT:     }
// SCUDO_REMOVED-NEXT:     global %[[VALUE_feature_coverage:[0-9]+]] feature_coverage: i32 [storage=static] = const<i32>(1) [linkage=external];
// SCUDO_REMOVED-NEXT: }
// SLATE-FILECHECK-END SCUDO_REMOVED
// SLATE-FILECHECK-BEGIN FUZZER_SCUDO
// FUZZER_SCUDO: module {
// FUZZER_SCUDO-NEXT:     target "x86_64-unknown-linux-gnu" {
// FUZZER_SCUDO-NEXT:         endian = little;
// FUZZER_SCUDO-NEXT:         pointer [size=8, align=8];
// FUZZER_SCUDO-NEXT:         stack_alignment = 16;
// FUZZER_SCUDO-NEXT:         long_double = f80;
// FUZZER_SCUDO-NEXT:         storage bool [size=1, align=1];
// FUZZER_SCUDO-NEXT:         storage i8, u8 [size=1, align=1];
// FUZZER_SCUDO-NEXT:         storage i16, u16 [size=2, align=2];
// FUZZER_SCUDO-NEXT:         storage i32, u32 [size=4, align=4];
// FUZZER_SCUDO-NEXT:         storage i64, u64 [size=8, align=8];
// FUZZER_SCUDO-NEXT:         storage i128, u128 [size=16, align=16];
// FUZZER_SCUDO-NEXT:         storage bf16 [size=2, align=2];
// FUZZER_SCUDO-NEXT:         storage f16 [size=2, align=2];
// FUZZER_SCUDO-NEXT:         storage f32 [size=4, align=4];
// FUZZER_SCUDO-NEXT:         storage f64 [size=8, align=8];
// FUZZER_SCUDO-NEXT:         storage f80 [size=16, align=16];
// FUZZER_SCUDO-NEXT:         storage f128 [size=16, align=16];
// FUZZER_SCUDO-NEXT:         storage d32 [size=4, align=4];
// FUZZER_SCUDO-NEXT:         storage d64 [size=8, align=8];
// FUZZER_SCUDO-NEXT:         storage d128 [size=16, align=16];
// FUZZER_SCUDO-NEXT:     }
// FUZZER_SCUDO-NEXT:     global %[[VALUE_feature_coverage:[0-9]+]] feature_coverage: i32 [storage=static] = const<i32>(1) [linkage=external];
// FUZZER_SCUDO-NEXT: }
// SLATE-FILECHECK-END FUZZER_SCUDO
