/* Test C2Y _Generic features: __extension__ suppresses -Wc23-c2y-compat
   warnings (and the state is restored after __extension__).  */
/* { dg-do compile } */
/* { dg-options "-std=gnu2y -pedantic-errors -Wc23-c2y-compat" } */

_Static_assert (__extension__ _Generic (const int, int : 1, const int : 2) == 2);
_Static_assert (_Generic (const int, int : 1, const int : 2) == 2); /* { dg-warning "use of type name" } */

_Static_assert (__extension__ _Generic (void, int : 1, void : 2) == 2);
_Static_assert (_Generic (void, int : 1, void : 2) == 2); /* { dg-warning "use of type name" } */
/* { dg-warning "incomplete type" "incomplete type" { target *-*-* } .-1 } */

_Static_assert (__extension__ _Generic (int (), int (*) () : 1, int () : 2) == 2);
_Static_assert (_Generic (int (), int (*) () : 1, int () : 2) == 2); /* { dg-warning "use of type name" } */
/* { dg-warning "function type" "function type" { target *-*-* } .-1 } */

// SLATE-FILECHECK-STD DEFAULT gnu2y
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
