// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ARGS /std:c11 /Zc:trigraphs /Zc:trigraphs-
static const char text[] = "??=??(??)??<??>??!??'??-??/??/";
_Static_assert(sizeof(text) == 31, "trigraph replacement");
#define SPLICED ??/
_Static_assert(sizeof("??/") == 4, "no trigraph line splice");
int trigraphs;

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-pc-windows-msvc" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f64;
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
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     global %[[VALUE_text:[0-9]+]] text: array<i8, 31> [storage=static] [const] [align=16] = code_units<array<i8, 31>>([63, 63, 61, 63, 63, 40, 63, 63, 41, 63, 63, 60, 63, 63, 62, 63, 63, 33, 63, 63, 39, 63, 63, 45, 63, 63, 47, 63, 63, 47, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_trigraphs:[0-9]+]] trigraphs: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
