/* Test C23 UTF-8 characters.  Test valid usages.  */
/* { dg-do compile } */
/* { dg-options "-std=c23 -pedantic-errors" } */

unsigned char a = u8'a';
_Static_assert (u8'a' == 97);

unsigned char b = u8'\0';
_Static_assert (u8'\0' == 0);

unsigned char c = u8'\xff';
_Static_assert (u8'\xff' == 255);

unsigned char d = u8'\377';
_Static_assert (u8'\377' == 255);

_Static_assert (sizeof (u8'a') == 1);
_Static_assert (sizeof (u8'\0') == 1);
_Static_assert (sizeof (u8'\xff') == 1);
_Static_assert (sizeof (u8'\377') == 1);

_Static_assert (_Generic (u8'a', unsigned char: 1, default: 2) == 1);
_Static_assert (_Generic (u8'\0', unsigned char: 1, default: 2) == 1);
_Static_assert (_Generic (u8'\xff', unsigned char: 1, default: 2) == 1);
_Static_assert (_Generic (u8'\377', unsigned char: 1, default: 2) == 1);

#if u8'\0' - 1 < 0
#error "UTF-8 constants not unsigned in preprocessor"
#endif

// SLATE-FILECHECK-STD DEFAULT c23
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
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: u8 [storage=static] = const<u8>(97) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: u8 [storage=static] = const<u8>(0) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: u8 [storage=static] = const<u8>(255) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: u8 [storage=static] = const<u8>(255) [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
