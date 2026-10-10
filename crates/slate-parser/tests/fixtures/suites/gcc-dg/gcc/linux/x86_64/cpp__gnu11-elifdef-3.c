/* Test #elifdef and #elifndef in GNU11.  */
/* { dg-do preprocess } */
/* { dg-options "-std=gnu11 -pedantic" } */

#define A
#undef B

#if 0
#elifdef A	/* { dg-warning "'#elifdef' before C23 is a GCC extension" } */
#define M1 1
#endif

#if M1 != 1
#error "#elifdef A did not apply"
#endif

#if 0
#elifdef B
#error "#elifdef B applied"
#endif

#if 0
#elifndef A
#error "#elifndef A applied"
#endif

#if 0
#elifndef B	/* { dg-warning "'#elifndef' before C23 is a GCC extension" } */
#define M2 2
#endif

#if M2 != 2
#error "#elifndef B did not apply"
#endif

#if 0
#elifdef A	/* { dg-warning "'#elifdef' before C23 is a GCC extension" } */
#else
#error "#elifdef A did not apply"
#endif

#if 0
#elifndef B	/* { dg-warning "'#elifndef' before C23 is a GCC extension" } */
#else
#error "#elifndef B did not apply"
#endif

#if 1
#elifdef A	/* { dg-warning "'#elifdef' before C23 is a GCC extension" } */
#endif

#if 1
#elifndef B	/* { dg-warning "'#elifndef' before C23 is a GCC extension" } */
#endif

/* As with #elif, the syntax of the new directives is relaxed after a
   non-skipped group.  */

#if 1
#elifdef x * y	/* { dg-warning "'#elifdef' before C23 is a GCC extension" } */
#endif

#if 1
#elifndef !	/* { dg-warning "'#elifndef' before C23 is a GCC extension" } */
#endif
// SLATE-FILECHECK-STD DEFAULT gnu11
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
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
