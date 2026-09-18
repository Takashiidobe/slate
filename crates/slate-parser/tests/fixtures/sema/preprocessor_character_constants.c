// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir -std=c23

#if '\xff' < 0
int plain_char_is_signed = 1;
#endif
#if 'ab' == 24930
int multicharacter_packs_bytes = 1;
#endif
#if L'\0' - 1 < 0
int wchar_is_signed = 1;
#endif
#if u'\0' - 1 > 0
int utf16_is_unsigned = 1;
#endif
#if U'\0' - 1 > 0
int utf32_is_unsigned = 1;
#endif
#if u8'\0' - 1 > 0
int utf8_is_unsigned = 1;
#endif

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "x86_64-unknown-linux-gnu" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=8, align=8];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f80;
// IR-NEXT:         storage bool [size=1, align=1];
// IR-NEXT:         storage i8, u8 [size=1, align=1];
// IR-NEXT:         storage i16, u16 [size=2, align=2];
// IR-NEXT:         storage i32, u32 [size=4, align=4];
// IR-NEXT:         storage i64, u64 [size=8, align=8];
// IR-NEXT:         storage i128, u128 [size=16, align=16];
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=8];
// IR-NEXT:         storage f80 [size=16, align=16];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     global %0 plain_char_is_signed: i32 [storage=static] = const<i32>(1) [linkage=external];
// IR-NEXT:     global %1 multicharacter_packs_bytes: i32 [storage=static] = const<i32>(1) [linkage=external];
// IR-NEXT:     global %2 wchar_is_signed: i32 [storage=static] = const<i32>(1) [linkage=external];
// IR-NEXT:     global %3 utf16_is_unsigned: i32 [storage=static] = const<i32>(1) [linkage=external];
// IR-NEXT:     global %4 utf32_is_unsigned: i32 [storage=static] = const<i32>(1) [linkage=external];
// IR-NEXT:     global %5 utf8_is_unsigned: i32 [storage=static] = const<i32>(1) [linkage=external];
// IR-NEXT: }
// SLATE-FILECHECK-END IR
