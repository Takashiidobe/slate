// SLATE-FILECHECK-DEFINES EBCDIC
// SLATE-FILECHECK-PREFIX-ARGS EBCDIC -fexec-charset=IBM1047 -fwide-exec-charset=UTF-32BE
// SLATE-FILECHECK-DEFINES CP1252
// SLATE-FILECHECK-PREFIX-ARGS CP1252 -fexec-charset=windows-1252 -fwide-exec-charset=ucs-4le
// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ARGS --compact-ir

const char narrow_name[] = __GNUC_EXECUTION_CHARSET_NAME;
const char wide_name[] = __GNUC_WIDE_EXECUTION_CHARSET_NAME;
#if 'A' == -63
int pp_constant_is_ebcdic;
#endif
const char text[] = "a\x41\101\né";
int multi = 'ab';
const char utf8[] = u8"aé";
const __WCHAR_TYPE__ wide[] = L"A\x41é";
int wide_character = L'\n';

// SLATE-FILECHECK-BEGIN EBCDIC
// EBCDIC: module {
// EBCDIC-NEXT:     target "x86_64-unknown-linux-gnu" {
// EBCDIC-NEXT:         endian = little;
// EBCDIC-NEXT:         pointer [size=8, align=8];
// EBCDIC-NEXT:         stack_alignment = 16;
// EBCDIC-NEXT:         long_double = f80;
// EBCDIC-NEXT:         storage bool [size=1, align=1];
// EBCDIC-NEXT:         storage i8, u8 [size=1, align=1];
// EBCDIC-NEXT:         storage i16, u16 [size=2, align=2];
// EBCDIC-NEXT:         storage i32, u32 [size=4, align=4];
// EBCDIC-NEXT:         storage i64, u64 [size=8, align=8];
// EBCDIC-NEXT:         storage i128, u128 [size=16, align=16];
// EBCDIC-NEXT:         storage bf16 [size=2, align=2];
// EBCDIC-NEXT:         storage f16 [size=2, align=2];
// EBCDIC-NEXT:         storage f32 [size=4, align=4];
// EBCDIC-NEXT:         storage f64 [size=8, align=8];
// EBCDIC-NEXT:         storage f80 [size=16, align=16];
// EBCDIC-NEXT:         storage f128 [size=16, align=16];
// EBCDIC-NEXT:         storage d32 [size=4, align=4];
// EBCDIC-NEXT:         storage d64 [size=8, align=8];
// EBCDIC-NEXT:         storage d128 [size=16, align=16];
// EBCDIC-NEXT:     }
// EBCDIC-NEXT:     global %[[VALUE_narrow_name:[0-9]+]] narrow_name: array<i8, 8> [storage=static] [const] = code_units<array<i8, 8>>([201, 194, 212, 241, 240, 244, 247, 0]) [linkage=external];
// EBCDIC-NEXT:     global %[[VALUE_wide_name:[0-9]+]] wide_name: array<i8, 9> [storage=static] [const] = code_units<array<i8, 9>>([228, 227, 198, 96, 243, 242, 194, 197, 0]) [linkage=external];
// EBCDIC-NEXT:     global %[[VALUE_pp_constant_is_ebcdic:[0-9]+]] pp_constant_is_ebcdic: i32 [storage=static] [linkage=external];
// EBCDIC-NEXT:     global %[[VALUE_text:[0-9]+]] text: array<i8, 6> [storage=static] [const] = code_units<array<i8, 6>>([129, 65, 65, 37, 81, 0]) [linkage=external];
// EBCDIC-NEXT:     global %[[VALUE_multi:[0-9]+]] multi: i32 [storage=static] = const<i32>(33154) [linkage=external];
// EBCDIC-NEXT:     global %[[VALUE_utf8:[0-9]+]] utf8: array<i8, 4> [storage=static] [const] = code_units<array<i8, 4>>([97, 195, 169, 0]) [linkage=external];
// EBCDIC-NEXT:     global %[[VALUE_wide:[0-9]+]] wide: array<i32, 4> [storage=static] [const] [align=16] = code_units<array<i32, 4>>([1090519040, 65, 3909091328, 0]) [linkage=external];
// EBCDIC-NEXT:     global %[[VALUE_wide_character:[0-9]+]] wide_character: i32 [storage=static] = const<i32>(167772160) [linkage=external];
// EBCDIC-NEXT: }
// SLATE-FILECHECK-END EBCDIC
// SLATE-FILECHECK-BEGIN CP1252
// CP1252: module {
// CP1252-NEXT:     target "x86_64-unknown-linux-gnu" {
// CP1252-NEXT:         endian = little;
// CP1252-NEXT:         pointer [size=8, align=8];
// CP1252-NEXT:         stack_alignment = 16;
// CP1252-NEXT:         long_double = f80;
// CP1252-NEXT:         storage bool [size=1, align=1];
// CP1252-NEXT:         storage i8, u8 [size=1, align=1];
// CP1252-NEXT:         storage i16, u16 [size=2, align=2];
// CP1252-NEXT:         storage i32, u32 [size=4, align=4];
// CP1252-NEXT:         storage i64, u64 [size=8, align=8];
// CP1252-NEXT:         storage i128, u128 [size=16, align=16];
// CP1252-NEXT:         storage bf16 [size=2, align=2];
// CP1252-NEXT:         storage f16 [size=2, align=2];
// CP1252-NEXT:         storage f32 [size=4, align=4];
// CP1252-NEXT:         storage f64 [size=8, align=8];
// CP1252-NEXT:         storage f80 [size=16, align=16];
// CP1252-NEXT:         storage f128 [size=16, align=16];
// CP1252-NEXT:         storage d32 [size=4, align=4];
// CP1252-NEXT:         storage d64 [size=8, align=8];
// CP1252-NEXT:         storage d128 [size=16, align=16];
// CP1252-NEXT:     }
// CP1252-NEXT:     global %[[VALUE_narrow_name:[0-9]+]] narrow_name: array<i8, 13> [storage=static] [const] = code_units<array<i8, 13>>([119, 105, 110, 100, 111, 119, 115, 45, 49, 50, 53, 50, 0]) [linkage=external];
// CP1252-NEXT:     global %[[VALUE_wide_name:[0-9]+]] wide_name: array<i8, 8> [storage=static] [const] = code_units<array<i8, 8>>([117, 99, 115, 45, 52, 108, 101, 0]) [linkage=external];
// CP1252-NEXT:     global %[[VALUE_text:[0-9]+]] text: array<i8, 6> [storage=static] [const] = code_units<array<i8, 6>>([97, 65, 65, 10, 233, 0]) [linkage=external];
// CP1252-NEXT:     global %[[VALUE_multi:[0-9]+]] multi: i32 [storage=static] = const<i32>(24930) [linkage=external];
// CP1252-NEXT:     global %[[VALUE_utf8:[0-9]+]] utf8: array<i8, 4> [storage=static] [const] = code_units<array<i8, 4>>([97, 195, 169, 0]) [linkage=external];
// CP1252-NEXT:     global %[[VALUE_wide:[0-9]+]] wide: array<i32, 4> [storage=static] [const] [align=16] = code_units<array<i32, 4>>([65, 65, 233, 0]) [linkage=external];
// CP1252-NEXT:     global %[[VALUE_wide_character:[0-9]+]] wide_character: i32 [storage=static] = const<i32>(10) [linkage=external];
// CP1252-NEXT: }
// SLATE-FILECHECK-END CP1252
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
// DEFAULT-NEXT:     global %[[VALUE_narrow_name:[0-9]+]] narrow_name: array<i8, 6> [storage=static] [const] = code_units<array<i8, 6>>([85, 84, 70, 45, 56, 0]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_wide_name:[0-9]+]] wide_name: array<i8, 9> [storage=static] [const] = code_units<array<i8, 9>>([85, 84, 70, 45, 51, 50, 76, 69, 0]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_text:[0-9]+]] text: array<i8, 7> [storage=static] [const] = code_units<array<i8, 7>>([97, 65, 65, 10, 195, 169, 0]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_multi:[0-9]+]] multi: i32 [storage=static] = const<i32>(24930) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_utf8:[0-9]+]] utf8: array<i8, 4> [storage=static] [const] = code_units<array<i8, 4>>([97, 195, 169, 0]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_wide:[0-9]+]] wide: array<i32, 4> [storage=static] [const] [align=16] = code_units<array<i32, 4>>([65, 65, 233, 0]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_wide_character:[0-9]+]] wide_character: i32 [storage=static] = const<i32>(10) [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
