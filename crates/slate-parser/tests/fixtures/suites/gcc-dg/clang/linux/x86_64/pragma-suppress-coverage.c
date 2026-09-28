/* Verify that we use emit diagnostics for #pragma GCC suppress_coverage.  */

/* { dg-do assemble } */
/* { dg-options "-fdiagnostics-show-caret" } */

#pragma GCC suppress_coverage end
/* { dg-warning "no matching begin for '#pragma GCC suppress_coverage end'" "" { target *-*-* } .-1 }
   { dg-begin-multiline-output "" }
 #pragma GCC suppress_coverage end
         ^~~
   { dg-end-multiline-output "" }  */

#pragma GCC suppress_coverage
/* { dg-warning "'#pragma GCC suppress_coverage' must be followed by 'begin' or 'end'" "" { target *-*-* } .-1 }
   { dg-begin-multiline-output "" }
 #pragma GCC suppress_coverage
         ^~~
   { dg-end-multiline-output "" }  */

#pragma GCC suppress_coverage begin more
/* { dg-warning "junk at end of '#pragma GCC suppress_coverage'" "" { target *-*-* } .-1 }
   { dg-begin-multiline-output "" }
 #pragma GCC suppress_coverage begin more
                                     ^~~~
   { dg-end-multiline-output "" }  */

#pragma GCC suppress_coverage begin
/* { dg-warning "'#pragma GCC suppress_coverage begin' was already in effect, ignored" "" { target *-*-* } .-1 }
   { dg-begin-multiline-output "" }
 #pragma GCC suppress_coverage begin
         ^~~
   { dg-end-multiline-output "" }  */

// SLATE-FILECHECK-STD DEFAULT gnu23
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
