#if U'\x{0}' != 0 || U'\u{e9}' != 233 || '\o{101}' != 65 || '\x{41}' != 65
#error incorrect delimited escape
#endif
_Static_assert(U'\u{10fffd}' == 0x10fffd, "unicode");
_Static_assert(u'\o{137775}' == 0xbffd, "octal");
_Static_assert(L'\x{0000001234}' == 0x1234, "hex");
int zero = U'\x{0}';
int short_unicode = U'\u{e9}';
int octal = '\o{101}';
int hex = '\x{41}';
char numeric[] = "\x{41}B\o{101}7";
char unicode[] = "\u{e9}";
unsigned short surrogate[] = u"\u{1f600}";
unsigned int code_units[] = U"\x{1f600}\o{373000}";

// SLATE-FILECHECK-STD C2Y c2y
// SLATE-FILECHECK-DEFINES C2Y
// SLATE-FILECHECK-STD C11 c11
// SLATE-FILECHECK-DEFINES C11

// SLATE-FILECHECK-BEGIN C2Y
// C2Y: module {
// C2Y-NEXT:     target "x86_64-unknown-linux-gnu" {
// C2Y-NEXT:         endian = little;
// C2Y-NEXT:         pointer [size=8, align=8];
// C2Y-NEXT:         stack_alignment = 16;
// C2Y-NEXT:         long_double = f80;
// C2Y-NEXT:         storage bool [size=1, align=1];
// C2Y-NEXT:         storage i8, u8 [size=1, align=1];
// C2Y-NEXT:         storage i16, u16 [size=2, align=2];
// C2Y-NEXT:         storage i32, u32 [size=4, align=4];
// C2Y-NEXT:         storage i64, u64 [size=8, align=8];
// C2Y-NEXT:         storage i128, u128 [size=16, align=16];
// C2Y-NEXT:         storage bf16 [size=2, align=2];
// C2Y-NEXT:         storage f16 [size=2, align=2];
// C2Y-NEXT:         storage f32 [size=4, align=4];
// C2Y-NEXT:         storage f64 [size=8, align=8];
// C2Y-NEXT:         storage f80 [size=16, align=16];
// C2Y-NEXT:         storage f128 [size=16, align=16];
// C2Y-NEXT:         storage d32 [size=4, align=4];
// C2Y-NEXT:         storage d64 [size=8, align=8];
// C2Y-NEXT:         storage d128 [size=16, align=16];
// C2Y-NEXT:     }
// C2Y-NEXT:     global %[[VALUE_zero:[0-9]+]] zero: i32 [storage=static] = reinterpret<i32, reason=assign, fits=always>(const<u32>(0)) [linkage=external];
// C2Y-NEXT:     global %[[VALUE_short_unicode:[0-9]+]] short_unicode: i32 [storage=static] = reinterpret<i32, reason=assign, fits=always>(const<u32>(233)) [linkage=external];
// C2Y-NEXT:     global %[[VALUE_octal:[0-9]+]] octal: i32 [storage=static] = const<i32>(65) [linkage=external];
// C2Y-NEXT:     global %[[VALUE_hex:[0-9]+]] hex: i32 [storage=static] = const<i32>(65) [linkage=external];
// C2Y-NEXT:     global %[[VALUE_numeric:[0-9]+]] numeric: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([65, 66, 65, 55, 0]) [linkage=external];
// C2Y-NEXT:     global %[[VALUE_unicode:[0-9]+]] unicode: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([195, 169, 0]) [linkage=external];
// C2Y-NEXT:     global %[[VALUE_surrogate:[0-9]+]] surrogate: array<u16, 3> [storage=static] = code_units<array<u16, 3>>([55357, 56832, 0]) [linkage=external];
// C2Y-NEXT:     global %[[VALUE_code_units:[0-9]+]] code_units: array<u32, 3> [storage=static] = code_units<array<u32, 3>>([128512, 128512, 0]) [linkage=external];
// C2Y-NEXT: }
// SLATE-FILECHECK-END C2Y
// SLATE-FILECHECK-BEGIN C11
// C11: module {
// C11-NEXT:     target "x86_64-unknown-linux-gnu" {
// C11-NEXT:         endian = little;
// C11-NEXT:         pointer [size=8, align=8];
// C11-NEXT:         stack_alignment = 16;
// C11-NEXT:         long_double = f80;
// C11-NEXT:         storage bool [size=1, align=1];
// C11-NEXT:         storage i8, u8 [size=1, align=1];
// C11-NEXT:         storage i16, u16 [size=2, align=2];
// C11-NEXT:         storage i32, u32 [size=4, align=4];
// C11-NEXT:         storage i64, u64 [size=8, align=8];
// C11-NEXT:         storage i128, u128 [size=16, align=16];
// C11-NEXT:         storage bf16 [size=2, align=2];
// C11-NEXT:         storage f16 [size=2, align=2];
// C11-NEXT:         storage f32 [size=4, align=4];
// C11-NEXT:         storage f64 [size=8, align=8];
// C11-NEXT:         storage f80 [size=16, align=16];
// C11-NEXT:         storage f128 [size=16, align=16];
// C11-NEXT:         storage d32 [size=4, align=4];
// C11-NEXT:         storage d64 [size=8, align=8];
// C11-NEXT:         storage d128 [size=16, align=16];
// C11-NEXT:     }
// C11-NEXT:     global %[[VALUE_zero:[0-9]+]] zero: i32 [storage=static] = reinterpret<i32, reason=assign, fits=always>(const<u32>(0)) [linkage=external];
// C11-NEXT:     global %[[VALUE_short_unicode:[0-9]+]] short_unicode: i32 [storage=static] = reinterpret<i32, reason=assign, fits=always>(const<u32>(233)) [linkage=external];
// C11-NEXT:     global %[[VALUE_octal:[0-9]+]] octal: i32 [storage=static] = const<i32>(65) [linkage=external];
// C11-NEXT:     global %[[VALUE_hex:[0-9]+]] hex: i32 [storage=static] = const<i32>(65) [linkage=external];
// C11-NEXT:     global %[[VALUE_numeric:[0-9]+]] numeric: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([65, 66, 65, 55, 0]) [linkage=external];
// C11-NEXT:     global %[[VALUE_unicode:[0-9]+]] unicode: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([195, 169, 0]) [linkage=external];
// C11-NEXT:     global %[[VALUE_surrogate:[0-9]+]] surrogate: array<u16, 3> [storage=static] = code_units<array<u16, 3>>([55357, 56832, 0]) [linkage=external];
// C11-NEXT:     global %[[VALUE_code_units:[0-9]+]] code_units: array<u32, 3> [storage=static] = code_units<array<u32, 3>>([128512, 128512, 0]) [linkage=external];
// C11-NEXT: }
// SLATE-FILECHECK-END C11
