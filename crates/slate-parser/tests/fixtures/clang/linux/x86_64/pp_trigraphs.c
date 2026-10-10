// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-DEFINES C17
// SLATE-FILECHECK-STD C17 c17
// SLATE-FILECHECK-DEFINES GNU17
// SLATE-FILECHECK-STD GNU17 gnu17
// SLATE-FILECHECK-DEFINES C23
// SLATE-FILECHECK-STD C23 c23
// SLATE-FILECHECK-DEFINES GNU17-TRIGRAPHS
// SLATE-FILECHECK-STD GNU17-TRIGRAPHS gnu17
// SLATE-FILECHECK-PREFIX-ARGS GNU17-TRIGRAPHS -ftrigraphs
// SLATE-FILECHECK-DEFINES C17-NO-TRIGRAPHS
// SLATE-FILECHECK-STD C17-NO-TRIGRAPHS c17
// SLATE-FILECHECK-PREFIX-ARGS C17-NO-TRIGRAPHS -trigraphs -fno-trigraphs

const char spelled[] = "??= ??( ??) ??< ??> ??' ??! ??- ???/??/";
int escape_width = sizeof "??/n";

#define SPLICED_BY_TRIGRAPH ??/
int only_without_trigraphs;

// SLATE-FILECHECK-BEGIN C17
// C17: module {
// C17-NEXT:     target "x86_64-unknown-linux-gnu" {
// C17-NEXT:         endian = little;
// C17-NEXT:         pointer [size=8, align=8];
// C17-NEXT:         stack_alignment = 16;
// C17-NEXT:         long_double = f80;
// C17-NEXT:         storage bool [size=1, align=1];
// C17-NEXT:         storage i8, u8 [size=1, align=1];
// C17-NEXT:         storage i16, u16 [size=2, align=2];
// C17-NEXT:         storage i32, u32 [size=4, align=4];
// C17-NEXT:         storage i64, u64 [size=8, align=8];
// C17-NEXT:         storage i128, u128 [size=16, align=16];
// C17-NEXT:         storage bf16 [size=2, align=2];
// C17-NEXT:         storage f16 [size=2, align=2];
// C17-NEXT:         storage f32 [size=4, align=4];
// C17-NEXT:         storage f64 [size=8, align=8];
// C17-NEXT:         storage f80 [size=16, align=16];
// C17-NEXT:         storage f128 [size=16, align=16];
// C17-NEXT:         storage d32 [size=4, align=4];
// C17-NEXT:         storage d64 [size=8, align=8];
// C17-NEXT:         storage d128 [size=16, align=16];
// C17-NEXT:     }
// C17-NEXT:     global %[[VALUE_spelled:[0-9]+]] spelled: array<i8, 19> [storage=static] [const] [align=16] = code_units<array<i8, 19>>([35, 32, 91, 32, 93, 32, 123, 32, 125, 32, 94, 32, 124, 32, 126, 32, 63, 92, 0]) [linkage=external];
// C17-NEXT:     global %[[VALUE_escape_width:[0-9]+]] escape_width: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(2))) [linkage=external];
// C17-NEXT: }
// SLATE-FILECHECK-END C17
// SLATE-FILECHECK-BEGIN GNU17
// GNU17: module {
// GNU17-NEXT:     target "x86_64-unknown-linux-gnu" {
// GNU17-NEXT:         endian = little;
// GNU17-NEXT:         pointer [size=8, align=8];
// GNU17-NEXT:         stack_alignment = 16;
// GNU17-NEXT:         long_double = f80;
// GNU17-NEXT:         storage bool [size=1, align=1];
// GNU17-NEXT:         storage i8, u8 [size=1, align=1];
// GNU17-NEXT:         storage i16, u16 [size=2, align=2];
// GNU17-NEXT:         storage i32, u32 [size=4, align=4];
// GNU17-NEXT:         storage i64, u64 [size=8, align=8];
// GNU17-NEXT:         storage i128, u128 [size=16, align=16];
// GNU17-NEXT:         storage bf16 [size=2, align=2];
// GNU17-NEXT:         storage f16 [size=2, align=2];
// GNU17-NEXT:         storage f32 [size=4, align=4];
// GNU17-NEXT:         storage f64 [size=8, align=8];
// GNU17-NEXT:         storage f80 [size=16, align=16];
// GNU17-NEXT:         storage f128 [size=16, align=16];
// GNU17-NEXT:         storage d32 [size=4, align=4];
// GNU17-NEXT:         storage d64 [size=8, align=8];
// GNU17-NEXT:         storage d128 [size=16, align=16];
// GNU17-NEXT:     }
// GNU17-NEXT:     global %[[VALUE_spelled:[0-9]+]] spelled: array<i8, 40> [storage=static] [const] [align=16] = code_units<array<i8, 40>>([63, 63, 61, 32, 63, 63, 40, 32, 63, 63, 41, 32, 63, 63, 60, 32, 63, 63, 62, 32, 63, 63, 39, 32, 63, 63, 33, 32, 63, 63, 45, 32, 63, 63, 63, 47, 63, 63, 47, 0]) [linkage=external];
// GNU17-NEXT:     global %[[VALUE_escape_width:[0-9]+]] escape_width: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(5))) [linkage=external];
// GNU17-NEXT:     global %[[VALUE_only_without_trigraphs:[0-9]+]] only_without_trigraphs: i32 [storage=static] [linkage=external];
// GNU17-NEXT: }
// SLATE-FILECHECK-END GNU17
// SLATE-FILECHECK-BEGIN C23
// C23: module {
// C23-NEXT:     target "x86_64-unknown-linux-gnu" {
// C23-NEXT:         endian = little;
// C23-NEXT:         pointer [size=8, align=8];
// C23-NEXT:         stack_alignment = 16;
// C23-NEXT:         long_double = f80;
// C23-NEXT:         storage bool [size=1, align=1];
// C23-NEXT:         storage i8, u8 [size=1, align=1];
// C23-NEXT:         storage i16, u16 [size=2, align=2];
// C23-NEXT:         storage i32, u32 [size=4, align=4];
// C23-NEXT:         storage i64, u64 [size=8, align=8];
// C23-NEXT:         storage i128, u128 [size=16, align=16];
// C23-NEXT:         storage bf16 [size=2, align=2];
// C23-NEXT:         storage f16 [size=2, align=2];
// C23-NEXT:         storage f32 [size=4, align=4];
// C23-NEXT:         storage f64 [size=8, align=8];
// C23-NEXT:         storage f80 [size=16, align=16];
// C23-NEXT:         storage f128 [size=16, align=16];
// C23-NEXT:         storage d32 [size=4, align=4];
// C23-NEXT:         storage d64 [size=8, align=8];
// C23-NEXT:         storage d128 [size=16, align=16];
// C23-NEXT:     }
// C23-NEXT:     global %[[VALUE_spelled:[0-9]+]] spelled: array<i8, 40> [storage=static] [const] [align=16] = code_units<array<i8, 40>>([63, 63, 61, 32, 63, 63, 40, 32, 63, 63, 41, 32, 63, 63, 60, 32, 63, 63, 62, 32, 63, 63, 39, 32, 63, 63, 33, 32, 63, 63, 45, 32, 63, 63, 63, 47, 63, 63, 47, 0]) [linkage=external];
// C23-NEXT:     global %[[VALUE_escape_width:[0-9]+]] escape_width: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(5))) [linkage=external];
// C23-NEXT:     global %[[VALUE_only_without_trigraphs:[0-9]+]] only_without_trigraphs: i32 [storage=static] [linkage=external];
// C23-NEXT: }
// SLATE-FILECHECK-END C23
// SLATE-FILECHECK-BEGIN GNU17-TRIGRAPHS
// GNU17-TRIGRAPHS: module {
// GNU17-TRIGRAPHS-NEXT:     target "x86_64-unknown-linux-gnu" {
// GNU17-TRIGRAPHS-NEXT:         endian = little;
// GNU17-TRIGRAPHS-NEXT:         pointer [size=8, align=8];
// GNU17-TRIGRAPHS-NEXT:         stack_alignment = 16;
// GNU17-TRIGRAPHS-NEXT:         long_double = f80;
// GNU17-TRIGRAPHS-NEXT:         storage bool [size=1, align=1];
// GNU17-TRIGRAPHS-NEXT:         storage i8, u8 [size=1, align=1];
// GNU17-TRIGRAPHS-NEXT:         storage i16, u16 [size=2, align=2];
// GNU17-TRIGRAPHS-NEXT:         storage i32, u32 [size=4, align=4];
// GNU17-TRIGRAPHS-NEXT:         storage i64, u64 [size=8, align=8];
// GNU17-TRIGRAPHS-NEXT:         storage i128, u128 [size=16, align=16];
// GNU17-TRIGRAPHS-NEXT:         storage bf16 [size=2, align=2];
// GNU17-TRIGRAPHS-NEXT:         storage f16 [size=2, align=2];
// GNU17-TRIGRAPHS-NEXT:         storage f32 [size=4, align=4];
// GNU17-TRIGRAPHS-NEXT:         storage f64 [size=8, align=8];
// GNU17-TRIGRAPHS-NEXT:         storage f80 [size=16, align=16];
// GNU17-TRIGRAPHS-NEXT:         storage f128 [size=16, align=16];
// GNU17-TRIGRAPHS-NEXT:         storage d32 [size=4, align=4];
// GNU17-TRIGRAPHS-NEXT:         storage d64 [size=8, align=8];
// GNU17-TRIGRAPHS-NEXT:         storage d128 [size=16, align=16];
// GNU17-TRIGRAPHS-NEXT:     }
// GNU17-TRIGRAPHS-NEXT:     global %[[VALUE_spelled:[0-9]+]] spelled: array<i8, 19> [storage=static] [const] [align=16] = code_units<array<i8, 19>>([35, 32, 91, 32, 93, 32, 123, 32, 125, 32, 94, 32, 124, 32, 126, 32, 63, 92, 0]) [linkage=external];
// GNU17-TRIGRAPHS-NEXT:     global %[[VALUE_escape_width:[0-9]+]] escape_width: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(2))) [linkage=external];
// GNU17-TRIGRAPHS-NEXT: }
// SLATE-FILECHECK-END GNU17-TRIGRAPHS
// SLATE-FILECHECK-BEGIN C17-NO-TRIGRAPHS
// C17-NO-TRIGRAPHS: module {
// C17-NO-TRIGRAPHS-NEXT:     target "x86_64-unknown-linux-gnu" {
// C17-NO-TRIGRAPHS-NEXT:         endian = little;
// C17-NO-TRIGRAPHS-NEXT:         pointer [size=8, align=8];
// C17-NO-TRIGRAPHS-NEXT:         stack_alignment = 16;
// C17-NO-TRIGRAPHS-NEXT:         long_double = f80;
// C17-NO-TRIGRAPHS-NEXT:         storage bool [size=1, align=1];
// C17-NO-TRIGRAPHS-NEXT:         storage i8, u8 [size=1, align=1];
// C17-NO-TRIGRAPHS-NEXT:         storage i16, u16 [size=2, align=2];
// C17-NO-TRIGRAPHS-NEXT:         storage i32, u32 [size=4, align=4];
// C17-NO-TRIGRAPHS-NEXT:         storage i64, u64 [size=8, align=8];
// C17-NO-TRIGRAPHS-NEXT:         storage i128, u128 [size=16, align=16];
// C17-NO-TRIGRAPHS-NEXT:         storage bf16 [size=2, align=2];
// C17-NO-TRIGRAPHS-NEXT:         storage f16 [size=2, align=2];
// C17-NO-TRIGRAPHS-NEXT:         storage f32 [size=4, align=4];
// C17-NO-TRIGRAPHS-NEXT:         storage f64 [size=8, align=8];
// C17-NO-TRIGRAPHS-NEXT:         storage f80 [size=16, align=16];
// C17-NO-TRIGRAPHS-NEXT:         storage f128 [size=16, align=16];
// C17-NO-TRIGRAPHS-NEXT:         storage d32 [size=4, align=4];
// C17-NO-TRIGRAPHS-NEXT:         storage d64 [size=8, align=8];
// C17-NO-TRIGRAPHS-NEXT:         storage d128 [size=16, align=16];
// C17-NO-TRIGRAPHS-NEXT:     }
// C17-NO-TRIGRAPHS-NEXT:     global %[[VALUE_spelled:[0-9]+]] spelled: array<i8, 40> [storage=static] [const] [align=16] = code_units<array<i8, 40>>([63, 63, 61, 32, 63, 63, 40, 32, 63, 63, 41, 32, 63, 63, 60, 32, 63, 63, 62, 32, 63, 63, 39, 32, 63, 63, 33, 32, 63, 63, 45, 32, 63, 63, 63, 47, 63, 63, 47, 0]) [linkage=external];
// C17-NO-TRIGRAPHS-NEXT:     global %[[VALUE_escape_width:[0-9]+]] escape_width: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(5))) [linkage=external];
// C17-NO-TRIGRAPHS-NEXT:     global %[[VALUE_only_without_trigraphs:[0-9]+]] only_without_trigraphs: i32 [storage=static] [linkage=external];
// C17-NO-TRIGRAPHS-NEXT: }
// SLATE-FILECHECK-END C17-NO-TRIGRAPHS
