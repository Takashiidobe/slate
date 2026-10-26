// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ARGS /std:c11 /Zc:trigraphs- /Zc:trigraphs
static const char text[] = "??=??(??)??<??>??!??'??-??/??/";
_Static_assert(sizeof(text) == 10, "trigraph replacement");
#define SPLICED ??/
  1
_Static_assert(SPLICED, "trigraph line splice");
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
// DEFAULT-NEXT:     global %[[VALUE_text:[0-9]+]] text: array<i8, 10> [storage=static] [const] = code_units<array<i8, 10>>([35, 91, 93, 123, 125, 124, 94, 126, 92, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_trigraphs:[0-9]+]] trigraphs: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
