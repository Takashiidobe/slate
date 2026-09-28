/* Test for constant expressions: overflow and constant expressions
   with -fwrapv: overflows still count as such for the purposes of
   constant expressions even when they have defined values at
   runtime.  */
/* Origin: Joseph Myers <joseph@codesourcery.com> */
/* { dg-do compile } */
/* { dg-options "-std=iso9899:1990 -pedantic-errors -fwrapv" } */

#include <limits.h>

enum e {
  E0 = 0 * (INT_MAX + 1), /* { dg-warning "21:integer overflow in expression" } */
  /* { dg-error "3:overflow in constant expression" "constant" { target *-*-* } .-1 } */
  E1 = 0 * (INT_MIN / -1), /* { dg-warning "21:integer overflow in expression" } */
  /* { dg-error "3:overflow in constant expression" "constant" { target *-*-* } .-1 } */
  E2 = 0 * (INT_MAX * INT_MAX), /* { dg-warning "21:integer overflow in expression" } */
  /* { dg-error "3:overflow in constant expression" "constant" { target *-*-* } .-1 } */
  E3 = 0 * (INT_MIN - 1), /* { dg-warning "21:integer overflow in expression" } */
  /* { dg-error "3:overflow in constant expression" "constant" { target *-*-* } .-1 } */
  E4 = 0 * (unsigned)(INT_MIN - 1), /* { dg-warning "31:integer overflow in expression" } */
  /* { dg-error "3:overflow in constant expression" "constant" { target *-*-* } .-1 } */
  E5 = 0 * -INT_MIN, /* { dg-warning "12:integer overflow in expression" } */
  /* { dg-error "3:overflow in constant expression" "constant" { target *-*-* } .-1 } */
  E6 = 0 * !-INT_MIN, /* { dg-warning "13:integer overflow in expression" } */
  /* { dg-error "8:not an integer constant" "constant" { target *-*-* } .-1 } */
  E7 = INT_MIN % -1 /* { dg-warning "16:integer overflow in expression" } */
  /* { dg-error "1:overflow in constant expression" "constant" { target *-*-* } .+1 } */
};

// SLATE-FILECHECK-STD DEFAULT iso9899:1990
// SLATE-FILECHECK-ARGS -fwrapv
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
// DEFAULT-NEXT:     type @type0 e = enum : u32 {
// DEFAULT-NEXT:         %0 E0 = const<i32>(0);
// DEFAULT-NEXT:         %1 E1 = const<i32>(0);
// DEFAULT-NEXT:         %2 E2 = const<i32>(0);
// DEFAULT-NEXT:         %3 E3 = const<i32>(0);
// DEFAULT-NEXT:         %4 E4 = const<i32>(0);
// DEFAULT-NEXT:         %5 E5 = const<i32>(0);
// DEFAULT-NEXT:         %6 E6 = const<i32>(0);
// DEFAULT-NEXT:         %7 E7 = const<i32>(0);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
