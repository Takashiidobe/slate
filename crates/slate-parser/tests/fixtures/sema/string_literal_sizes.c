// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir -std=c23

static_assert(sizeof("ab") == 3);
static_assert(sizeof("Ω") == 3);
static_assert(sizeof("Ω") == 3);
static_assert(sizeof("\xff") == 2);
static_assert(sizeof("\377") == 2);
static_assert(sizeof("\x41ΩZ") == 5);
static_assert(sizeof("a" "Ω") == 4);
static_assert(sizeof(u8"Ωx") == 4);
static_assert(sizeof(u"\U0001F600a") == 8);
static_assert(sizeof(u"\xd800") == 4);
static_assert(sizeof(U"\U0001F600a") == 12);
static_assert(sizeof(L"Ωab") == 16);

int greek_bound[sizeof("Ω")];
const char *greek = "Ω!";
const char *escapes = "\xff\0a";

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
// IR-NEXT:     global %0 greek_bound: array<i32, 3> [storage=static] [linkage=external];
// IR-NEXT:     global %3 .str3: array<i8, 4> [storage=static] = bytes<array<i8, 4>>([206, 169, 33, 0]) [linkage=internal];
// IR-NEXT:     global %1 greek: ptr<const i8> [storage=static] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(4)>(%3)) [linkage=external];
// IR-NEXT:     global %4 .str4: array<i8, 4> [storage=static] = bytes<array<i8, 4>>([255, 0, 97, 0]) [linkage=internal];
// IR-NEXT:     global %2 escapes: ptr<const i8> [storage=static] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(4)>(%4)) [linkage=external];
// IR-NEXT: }
// SLATE-FILECHECK-END IR
