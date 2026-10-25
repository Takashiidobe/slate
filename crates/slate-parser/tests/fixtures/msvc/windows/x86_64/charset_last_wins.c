// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir /std:c11 /source-charset:utf-8 /source-charset:.1252 /execution-charset:windows-1252
#if _MSVC_EXECUTION_CHARACTER_SET != 1252
#error "the last charset option wins"
#endif
const char narrow[] = "é€";
const unsigned short wide[] = L"é€";
const char utf8[] = u8"é";
const char named[] = "\u00e9\u20ac";
const unsigned short wide_named[] = L"\u00e9";
const char unrepresentable[] = "一Á\x81";
const unsigned short wide_unassigned[] = L"Á";
int plain = 'é';
int plain_named = '\u00e9';

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "x86_64-pc-windows-msvc" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=8, align=8];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f64;
// IR-NEXT:         storage bool [size=1, align=1];
// IR-NEXT:         storage i8, u8 [size=1, align=1];
// IR-NEXT:         storage i16, u16 [size=2, align=2];
// IR-NEXT:         storage i32, u32 [size=4, align=4];
// IR-NEXT:         storage i64, u64 [size=8, align=8];
// IR-NEXT:         storage i128, u128 [size=16, align=16];
// IR-NEXT:         storage bf16 [size=2, align=2];
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=8];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     global %[[VALUE_narrow:[0-9]+]] narrow: array<i8, 6> [storage=static] [const] = code_units<array<i8, 6>>([195, 169, 226, 130, 172, 0]) [linkage=external];
// IR-NEXT:     global %[[VALUE_wide:[0-9]+]] wide: array<u16, 6> [storage=static] [const] = code_units<array<u16, 6>>([195, 169, 226, 8218, 172, 0]) [linkage=external];
// IR-NEXT:     global %[[VALUE_utf8:[0-9]+]] utf8: array<i8, 5> [storage=static] [const] = code_units<array<i8, 5>>([195, 131, 194, 169, 0]) [linkage=external];
// IR-NEXT:     global %[[VALUE_named:[0-9]+]] named: array<i8, 3> [storage=static] [const] = code_units<array<i8, 3>>([233, 128, 0]) [linkage=external];
// IR-NEXT:     global %[[VALUE_wide_named:[0-9]+]] wide_named: array<u16, 2> [storage=static] [const] = code_units<array<u16, 2>>([233, 0]) [linkage=external];
// IR-NEXT:     global %[[VALUE_unrepresentable:[0-9]+]] unrepresentable: array<i8, 7> [storage=static] [const] = code_units<array<i8, 7>>([228, 184, 128, 195, 129, 129, 0]) [linkage=external];
// IR-NEXT:     global %[[VALUE_wide_unassigned:[0-9]+]] wide_unassigned: array<u16, 3> [storage=static] [const] = code_units<array<u16, 3>>([195, 129, 0]) [linkage=external];
// IR-NEXT:     global %[[VALUE_plain:[0-9]+]] plain: i32 [storage=static] = const<i32>(50089) [linkage=external];
// IR-NEXT:     global %[[VALUE_plain_named:[0-9]+]] plain_named: i32 [storage=static] = const<i32>(-23) [linkage=external];
// IR-NEXT: }
// SLATE-FILECHECK-END IR
