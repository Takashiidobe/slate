/* P2290R3 - Delimited escape sequences */
/* { dg-do compile } */
/* { dg-require-effective-target wchar } */
/* { dg-options "-std=gnu99 -Wno-c++-compat -pedantic-errors" } */

typedef __CHAR32_TYPE__ char32_t;

const char32_t *a = U"\u{1234}";	/* { dg-error "delimited escape sequences are only valid in" } */
const char32_t *b = U"\x{1234}";	/* { dg-error "delimited escape sequences are only valid in" } */
const char32_t *c = U"\o{1234}";	/* { dg-error "delimited escape sequences are only valid in" } */

// SLATE-FILECHECK-STD DEFAULT gnu99
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
// DEFAULT-NEXT:     type @type[[TYPE_char32_t:[0-9]+]] char32_t = u32;
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<u32, 2> [storage=static] = code_units<array<u32, 2>>([4660, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: ptr<const u32> [storage=static] = pointer_cast<ptr<const u32>, reason=assign>(array_decay<ptr<u32>, length=Some(2)>(%[[VALUE_str]])) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<u32, 2> [storage=static] = code_units<array<u32, 2>>([4660, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: ptr<const u32> [storage=static] = pointer_cast<ptr<const u32>, reason=assign>(array_decay<ptr<u32>, length=Some(2)>(%[[VALUE_str_2]])) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<u32, 2> [storage=static] = code_units<array<u32, 2>>([668, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: ptr<const u32> [storage=static] = pointer_cast<ptr<const u32>, reason=assign>(array_decay<ptr<u32>, length=Some(2)>(%[[VALUE_str_3]])) [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
