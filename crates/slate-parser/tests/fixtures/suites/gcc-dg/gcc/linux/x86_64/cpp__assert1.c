/* Basic tests of the #assert preprocessor extension. */
/* { dg-do compile } */
/* { dg-options "-Wno-deprecated" } */

#define def unused expansion
#define fail  int fail

#assert abc (def)
#assert abc (ghi)
#assert abc (jkl)
#assert space ( s p a c e )

/* Basic: */
#if !#abc (def) || !#abc (ghi) || !#abc (jkl)
fail
#endif

/* any answer for #abc */
#if !#abc
fail
#endif

/* internal whitespace is collapsed,
   external whitespace is deleted  */
#if !#space (s p  a  c e) || !#space (  s p a c e  ) || #space (space)
fail
#endif

/* removing assertions */
#unassert abc (jkl)
#if !#abc || !#abc (def) || !#abc (ghi) || #abc (jkl)
fail
#endif

#unassert abc
#if #abc || #abc (def) || #abc (ghi) || #abc (jkl)
fail
#endif

int gobble

/* make sure it can succeed too.
   also check space before open paren isn't significant */
#if #space(s p a c e)
;
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
// DEFAULT-NEXT:     global %[[VALUE_gobble:[0-9]+]] gobble: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
