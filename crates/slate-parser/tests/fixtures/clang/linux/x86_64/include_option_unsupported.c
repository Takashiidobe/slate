int value;

// SLATE-FILECHECK-ARGS -iwithprefix=tests/fixtures/inputs/include_search/user
// SLATE-FILECHECK-DEFINES C17
// SLATE-FILECHECK-STD C17 c17
// SLATE-FILECHECK-WARNING C17

// SLATE-FILECHECK-BEGIN C17
// C17:   ⚠ unsupported compiler flag `-iwithprefix=tests/fixtures/inputs/include_search/user` ignored
// SLATE-FILECHECK-END C17
// SLATE-FILECHECK-BEGIN IR-C17
// IR-C17: module {
// IR-C17-NEXT:     target "x86_64-unknown-linux-gnu" {
// IR-C17-NEXT:         endian = little;
// IR-C17-NEXT:         pointer [size=8, align=8];
// IR-C17-NEXT:         stack_alignment = 16;
// IR-C17-NEXT:         long_double = f80;
// IR-C17-NEXT:         storage bool [size=1, align=1];
// IR-C17-NEXT:         storage i8, u8 [size=1, align=1];
// IR-C17-NEXT:         storage i16, u16 [size=2, align=2];
// IR-C17-NEXT:         storage i32, u32 [size=4, align=4];
// IR-C17-NEXT:         storage i64, u64 [size=8, align=8];
// IR-C17-NEXT:         storage i128, u128 [size=16, align=16];
// IR-C17-NEXT:         storage bf16 [size=2, align=2];
// IR-C17-NEXT:         storage f16 [size=2, align=2];
// IR-C17-NEXT:         storage f32 [size=4, align=4];
// IR-C17-NEXT:         storage f64 [size=8, align=8];
// IR-C17-NEXT:         storage f80 [size=16, align=16];
// IR-C17-NEXT:         storage f128 [size=16, align=16];
// IR-C17-NEXT:         storage d32 [size=4, align=4];
// IR-C17-NEXT:         storage d64 [size=8, align=8];
// IR-C17-NEXT:         storage d128 [size=16, align=16];
// IR-C17-NEXT:     }
// IR-C17-NEXT:     global %[[VALUE_value:[0-9]+]] value: i32 [storage=static] [linkage=external];
// IR-C17-NEXT: }
// SLATE-FILECHECK-END IR-C17
