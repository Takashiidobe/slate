/* Copyright (C) 2000, 2001 Free Software Foundation, Inc.  */

/* { dg-do preprocess } */
/* { dg-additional-options "-Wall" } */

/* Test we get signedness of ?: operator correct.  We would skip
   evaluation of one argument, and might therefore not transfer its
   unsignedness to the result.  */

/* Neil Booth, 19 Jul 2002.  */

#if (1 ? -2: 0 + 1U) < 0 /* { dg-warning {the left operand of ':' changes sign} } */
#error				/* { dg-bogus "error" } */
#endif

#if (0 ? 0 + 1U: -2) < 0 /* { dg-warning {the right operand of ':' changes sign} } */
#error				/* { dg-bogus "error" } */
#endif

/* PR preprocessor/112701 */
#if (0 ? 0/0u : -1) < 0 /* { dg-warning {the right operand of ':' changes sign} } */
#error /* { dg-bogus "error" } */
#endif

#if (0 ? 0u/0 : -1) < 0 /* { dg-warning {the right operand of ':' changes sign} } */
#error /* { dg-bogus "error" } */
#endif

#if (1 ? -1 : 0/0u) < 0 /* { dg-warning {the left operand of ':' changes sign} } */
#error /* { dg-bogus "error" } */
#endif

#if (1 ? -1 : 0u/0) < 0 /* { dg-warning {the left operand of ':' changes sign} } */
#error /* { dg-bogus "error" } */
#endif
// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-STD DEFAULT gnu23

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
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
