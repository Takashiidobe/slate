// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-DEFINES C89
// SLATE-FILECHECK-STD C89 c89
// SLATE-FILECHECK-DEFINES C23
// SLATE-FILECHECK-STD C23 c23

#define PASTE(a, b) a##b
#define STR(x) #x
#define XSTR(x) STR(x)

const char pasted[] = XSTR(PASTE(:, :));
const char spaced[] = XSTR(: :);
const char scoped[] = XSTR([[gnu::unused]]);

// SLATE-FILECHECK-BEGIN C89
// C89: module {
// C89-NEXT:     target "x86_64-unknown-linux-gnu" {
// C89-NEXT:         endian = little;
// C89-NEXT:         pointer [size=8, align=8];
// C89-NEXT:         stack_alignment = 16;
// C89-NEXT:         long_double = f80;
// C89-NEXT:         storage bool [size=1, align=1];
// C89-NEXT:         storage i8, u8 [size=1, align=1];
// C89-NEXT:         storage i16, u16 [size=2, align=2];
// C89-NEXT:         storage i32, u32 [size=4, align=4];
// C89-NEXT:         storage i64, u64 [size=8, align=8];
// C89-NEXT:         storage i128, u128 [size=16, align=16];
// C89-NEXT:         storage bf16 [size=2, align=2];
// C89-NEXT:         storage f16 [size=2, align=2];
// C89-NEXT:         storage f32 [size=4, align=4];
// C89-NEXT:         storage f64 [size=8, align=8];
// C89-NEXT:         storage f80 [size=16, align=16];
// C89-NEXT:         storage f128 [size=16, align=16];
// C89-NEXT:         storage d32 [size=4, align=4];
// C89-NEXT:         storage d64 [size=8, align=8];
// C89-NEXT:         storage d128 [size=16, align=16];
// C89-NEXT:     }
// C89-NEXT:     global %[[VALUE_pasted:[0-9]+]] pasted: array<i8, 3> [storage=static] [const] = code_units<array<i8, 3>>([58, 58, 0]) [linkage=external];
// C89-NEXT:     global %[[VALUE_spaced:[0-9]+]] spaced: array<i8, 4> [storage=static] [const] = code_units<array<i8, 4>>([58, 32, 58, 0]) [linkage=external];
// C89-NEXT:     global %[[VALUE_scoped:[0-9]+]] scoped: array<i8, 16> [storage=static] [const] [align=16] = code_units<array<i8, 16>>([91, 91, 103, 110, 117, 58, 58, 117, 110, 117, 115, 101, 100, 93, 93, 0]) [linkage=external];
// C89-NEXT: }
// SLATE-FILECHECK-END C89
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
// C23-NEXT:     global %[[VALUE_pasted:[0-9]+]] pasted: array<i8, 3> [storage=static] [const] = code_units<array<i8, 3>>([58, 58, 0]) [linkage=external];
// C23-NEXT:     global %[[VALUE_spaced:[0-9]+]] spaced: array<i8, 4> [storage=static] [const] = code_units<array<i8, 4>>([58, 32, 58, 0]) [linkage=external];
// C23-NEXT:     global %[[VALUE_scoped:[0-9]+]] scoped: array<i8, 16> [storage=static] [const] [align=16] = code_units<array<i8, 16>>([91, 91, 103, 110, 117, 58, 58, 117, 110, 117, 115, 101, 100, 93, 93, 0]) [linkage=external];
// C23-NEXT: }
// SLATE-FILECHECK-END C23
