/* { dg-do compile } */
/* { dg-options "-std=c99" } */

/* When GCC reads UTF-8-encoded input into its internal UTF-8
representation, it does not apply any transformation to the data, and
in particular it makes no attempt to verify that the encoding is valid
UTF-8.  Historically, if any non-ASCII characters were found outside a
string or comment, they were treated as stray tokens and did not
necessarily produce an error, e.g. if, as in this test, they disappear
in the preprocessor.  Now that UTF-8 is also supported in identifiers,
the basic structure of this process has not changed; GCC just treats
invalid UTF-8 as a stray token.  This test verifies that the historical
behavior is unchanged.  In the future, if GCC were changed, say, to
validate the UTF-8 on input, then this test would no longer be
appropriate.  */


#define a b(
#define b(x) q
/* The line below contains invalid UTF-8.  */
int aœ);

// SLATE-FILECHECK-STD DEFAULT c99
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
// DEFAULT-NEXT:     global %0 q: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
