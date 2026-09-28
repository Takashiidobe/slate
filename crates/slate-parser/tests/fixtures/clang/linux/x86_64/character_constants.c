// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir -std=c23

static_assert(sizeof('a') == 4);
static_assert(sizeof(u8'a') == 1);
static_assert(sizeof(u'a') == 2);
static_assert(sizeof(U'a') == 4);
static_assert(sizeof(L'a') == 4);

int plain = 'a';
int signed_byte = '\xff';
int multicharacter = 'ab';
int multicharacter_truncated = 'abcde';
int wide = L'Ω';
int wide_escape = L'\xff';
unsigned char utf8 = u8'a';
unsigned short utf16 = u'Ω';
unsigned int utf32 = U'\U0001F600';

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
// IR-NEXT:         storage bf16 [size=2, align=2];
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=8];
// IR-NEXT:         storage f80 [size=16, align=16];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     global %0 plain: i32 [storage=static] = const<i32>(97) [linkage=external];
// IR-NEXT:     global %1 signed_byte: i32 [storage=static] = const<i32>(-1) [linkage=external];
// IR-NEXT:     global %2 multicharacter: i32 [storage=static] = const<i32>(24930) [linkage=external];
// IR-NEXT:     global %3 multicharacter_truncated: i32 [storage=static] = const<i32>(1650680933) [linkage=external];
// IR-NEXT:     global %4 wide: i32 [storage=static] = const<i32>(937) [linkage=external];
// IR-NEXT:     global %5 wide_escape: i32 [storage=static] = const<i32>(255) [linkage=external];
// IR-NEXT:     global %6 utf8: u8 [storage=static] = const<u8>(97) [linkage=external];
// IR-NEXT:     global %7 utf16: u16 [storage=static] = const<u16>(937) [linkage=external];
// IR-NEXT:     global %8 utf32: u32 [storage=static] = const<u32>(128512) [linkage=external];
// IR-NEXT: }
// SLATE-FILECHECK-END IR
