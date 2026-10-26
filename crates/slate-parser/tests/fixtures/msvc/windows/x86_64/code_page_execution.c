// SLATE-FILECHECK-DEFINES CP1251
// SLATE-FILECHECK-WARNING CP1251
// SLATE-FILECHECK-PREFIX-ARGS CP1251 /execution-charset:windows-1251
// SLATE-FILECHECK-DEFINES CP437
// SLATE-FILECHECK-PREFIX-ARGS CP437 /execution-charset:.437
// SLATE-FILECHECK-DEFINES ASCII
// SLATE-FILECHECK-PREFIX-ARGS ASCII /execution-charset:US-ASCII
// SLATE-FILECHECK-DEFINES KOI8
// SLATE-FILECHECK-PREFIX-ARGS KOI8 /execution-charset:koi8-r
// SLATE-FILECHECK-ARGS --compact-ir

long code_page = _MSVC_EXECUTION_CHARACTER_SET;
const char text[] = "\u0416\u00E9\u20AC\u2030\u2554";
int character = '\u0416';

// SLATE-FILECHECK-BEGIN CP1251
// CP1251: -Wunrepresentable-character
// CP1251:   ⚠ character represented by universal-character-name '\u00E9' cannot be
// CP1251:   │ represented in the current code page (1251)
// CP1251:    ╭─[tests/fixtures/msvc/windows/x86_64/code_page_execution.c:3:21]
// CP1251:  2 │ long code_page = _MSVC_EXECUTION_CHARACTER_SET;
// CP1251:  3 │ const char text[] = "\u0416\u00E9\u20AC\u2030\u2554";
// CP1251:    ·                     ────────────────────────────────
// CP1251:  4 │ int character = '\u0416';
// CP1251:    ╰────
// CP1251: -Wunrepresentable-character
// CP1251:   ⚠ character represented by universal-character-name '\u2554' cannot be
// CP1251:   │ represented in the current code page (1251)
// CP1251:    ╭─[tests/fixtures/msvc/windows/x86_64/code_page_execution.c:3:21]
// CP1251:  2 │ long code_page = _MSVC_EXECUTION_CHARACTER_SET;
// CP1251:  3 │ const char text[] = "\u0416\u00E9\u20AC\u2030\u2554";
// CP1251:    ·                     ────────────────────────────────
// CP1251:  4 │ int character = '\u0416';
// CP1251:    ╰────
// SLATE-FILECHECK-END CP1251
// SLATE-FILECHECK-BEGIN IR-CP1251
// IR-CP1251: module {
// IR-CP1251-NEXT:     target "x86_64-pc-windows-msvc" {
// IR-CP1251-NEXT:         endian = little;
// IR-CP1251-NEXT:         pointer [size=8, align=8];
// IR-CP1251-NEXT:         stack_alignment = 16;
// IR-CP1251-NEXT:         long_double = f64;
// IR-CP1251-NEXT:         storage bool [size=1, align=1];
// IR-CP1251-NEXT:         storage i8, u8 [size=1, align=1];
// IR-CP1251-NEXT:         storage i16, u16 [size=2, align=2];
// IR-CP1251-NEXT:         storage i32, u32 [size=4, align=4];
// IR-CP1251-NEXT:         storage i64, u64 [size=8, align=8];
// IR-CP1251-NEXT:         storage i128, u128 [size=16, align=16];
// IR-CP1251-NEXT:         storage bf16 [size=2, align=2];
// IR-CP1251-NEXT:         storage f16 [size=2, align=2];
// IR-CP1251-NEXT:         storage f32 [size=4, align=4];
// IR-CP1251-NEXT:         storage f64 [size=8, align=8];
// IR-CP1251-NEXT:         storage f128 [size=16, align=16];
// IR-CP1251-NEXT:         storage d32 [size=4, align=4];
// IR-CP1251-NEXT:         storage d64 [size=8, align=8];
// IR-CP1251-NEXT:         storage d128 [size=16, align=16];
// IR-CP1251-NEXT:     }
// IR-CP1251-NEXT:     global %[[VALUE_code_page:[0-9]+]] code_page: i32 [storage=static] = const<i32>(1251) [linkage=external];
// IR-CP1251-NEXT:     global %[[VALUE_text:[0-9]+]] text: array<i8, 6> [storage=static] [const] = code_units<array<i8, 6>>([198, 63, 136, 137, 63, 0]) [linkage=external];
// IR-CP1251-NEXT:     global %[[VALUE_character:[0-9]+]] character: i32 [storage=static] = const<i32>(-58) [linkage=external];
// IR-CP1251-NEXT: }
// SLATE-FILECHECK-END IR-CP1251
// SLATE-FILECHECK-BEGIN CP437
// CP437: module {
// CP437-NEXT:     target "x86_64-pc-windows-msvc" {
// CP437-NEXT:         endian = little;
// CP437-NEXT:         pointer [size=8, align=8];
// CP437-NEXT:         stack_alignment = 16;
// CP437-NEXT:         long_double = f64;
// CP437-NEXT:         storage bool [size=1, align=1];
// CP437-NEXT:         storage i8, u8 [size=1, align=1];
// CP437-NEXT:         storage i16, u16 [size=2, align=2];
// CP437-NEXT:         storage i32, u32 [size=4, align=4];
// CP437-NEXT:         storage i64, u64 [size=8, align=8];
// CP437-NEXT:         storage i128, u128 [size=16, align=16];
// CP437-NEXT:         storage bf16 [size=2, align=2];
// CP437-NEXT:         storage f16 [size=2, align=2];
// CP437-NEXT:         storage f32 [size=4, align=4];
// CP437-NEXT:         storage f64 [size=8, align=8];
// CP437-NEXT:         storage f128 [size=16, align=16];
// CP437-NEXT:         storage d32 [size=4, align=4];
// CP437-NEXT:         storage d64 [size=8, align=8];
// CP437-NEXT:         storage d128 [size=16, align=16];
// CP437-NEXT:     }
// CP437-NEXT:     global %[[VALUE_code_page:[0-9]+]] code_page: i32 [storage=static] = const<i32>(437) [linkage=external];
// CP437-NEXT:     global %[[VALUE_text:[0-9]+]] text: array<i8, 6> [storage=static] [const] = code_units<array<i8, 6>>([63, 130, 63, 63, 201, 0]) [linkage=external];
// CP437-NEXT:     global %[[VALUE_character:[0-9]+]] character: i32 [storage=static] = const<i32>(63) [linkage=external];
// CP437-NEXT: }
// SLATE-FILECHECK-END CP437
// SLATE-FILECHECK-BEGIN ASCII
// ASCII: module {
// ASCII-NEXT:     target "x86_64-pc-windows-msvc" {
// ASCII-NEXT:         endian = little;
// ASCII-NEXT:         pointer [size=8, align=8];
// ASCII-NEXT:         stack_alignment = 16;
// ASCII-NEXT:         long_double = f64;
// ASCII-NEXT:         storage bool [size=1, align=1];
// ASCII-NEXT:         storage i8, u8 [size=1, align=1];
// ASCII-NEXT:         storage i16, u16 [size=2, align=2];
// ASCII-NEXT:         storage i32, u32 [size=4, align=4];
// ASCII-NEXT:         storage i64, u64 [size=8, align=8];
// ASCII-NEXT:         storage i128, u128 [size=16, align=16];
// ASCII-NEXT:         storage bf16 [size=2, align=2];
// ASCII-NEXT:         storage f16 [size=2, align=2];
// ASCII-NEXT:         storage f32 [size=4, align=4];
// ASCII-NEXT:         storage f64 [size=8, align=8];
// ASCII-NEXT:         storage f128 [size=16, align=16];
// ASCII-NEXT:         storage d32 [size=4, align=4];
// ASCII-NEXT:         storage d64 [size=8, align=8];
// ASCII-NEXT:         storage d128 [size=16, align=16];
// ASCII-NEXT:     }
// ASCII-NEXT:     global %[[VALUE_code_page:[0-9]+]] code_page: i32 [storage=static] = const<i32>(20127) [linkage=external];
// ASCII-NEXT:     global %[[VALUE_text:[0-9]+]] text: array<i8, 6> [storage=static] [const] = code_units<array<i8, 6>>([63, 63, 63, 63, 63, 0]) [linkage=external];
// ASCII-NEXT:     global %[[VALUE_character:[0-9]+]] character: i32 [storage=static] = const<i32>(63) [linkage=external];
// ASCII-NEXT: }
// SLATE-FILECHECK-END ASCII
// SLATE-FILECHECK-BEGIN KOI8
// KOI8: module {
// KOI8-NEXT:     target "x86_64-pc-windows-msvc" {
// KOI8-NEXT:         endian = little;
// KOI8-NEXT:         pointer [size=8, align=8];
// KOI8-NEXT:         stack_alignment = 16;
// KOI8-NEXT:         long_double = f64;
// KOI8-NEXT:         storage bool [size=1, align=1];
// KOI8-NEXT:         storage i8, u8 [size=1, align=1];
// KOI8-NEXT:         storage i16, u16 [size=2, align=2];
// KOI8-NEXT:         storage i32, u32 [size=4, align=4];
// KOI8-NEXT:         storage i64, u64 [size=8, align=8];
// KOI8-NEXT:         storage i128, u128 [size=16, align=16];
// KOI8-NEXT:         storage bf16 [size=2, align=2];
// KOI8-NEXT:         storage f16 [size=2, align=2];
// KOI8-NEXT:         storage f32 [size=4, align=4];
// KOI8-NEXT:         storage f64 [size=8, align=8];
// KOI8-NEXT:         storage f128 [size=16, align=16];
// KOI8-NEXT:         storage d32 [size=4, align=4];
// KOI8-NEXT:         storage d64 [size=8, align=8];
// KOI8-NEXT:         storage d128 [size=16, align=16];
// KOI8-NEXT:     }
// KOI8-NEXT:     global %[[VALUE_code_page:[0-9]+]] code_page: i32 [storage=static] = const<i32>(20866) [linkage=external];
// KOI8-NEXT:     global %[[VALUE_text:[0-9]+]] text: array<i8, 6> [storage=static] [const] = code_units<array<i8, 6>>([246, 63, 63, 63, 165, 0]) [linkage=external];
// KOI8-NEXT:     global %[[VALUE_character:[0-9]+]] character: i32 [storage=static] = const<i32>(-10) [linkage=external];
// KOI8-NEXT: }
// SLATE-FILECHECK-END KOI8
