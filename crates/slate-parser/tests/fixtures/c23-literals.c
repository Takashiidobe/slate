// C23 literal spellings
int decimal_separator __attribute__((slate_literal(1'000u)));
int binary_bitint __attribute__((slate_literal(0b1010wb)));
int hexadecimal __attribute__((slate_literal(0x2aUL)));
int decimal_float __attribute__((slate_literal(1.25e+2f)));
int hex_float __attribute__((slate_literal(0x1.fp+2)));
int character __attribute__((slate_literal('a')));
int escaped_string __attribute__((slate_literal("\N{SNOWMAN}")));
int utf8_string __attribute__((slate_literal(u8"text")));
int utf16_string __attribute__((slate_literal(u"text")));
int utf32_string __attribute__((slate_literal(U"text")));
int wide_string __attribute__((slate_literal(L"text")));
int unicode_name __attribute__((slate_literal(\u03B1name)));

// SLATE-FILECHECK-DEFINES DEFAULT

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
// DEFAULT-NEXT:     global %0 decimal_separator: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1 binary_bitint: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 hexadecimal: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 decimal_float: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 hex_float: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %5 character: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %6 escaped_string: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %7 utf8_string: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %8 utf16_string: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %9 utf32_string: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %10 wide_string: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %11 unicode_name: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
