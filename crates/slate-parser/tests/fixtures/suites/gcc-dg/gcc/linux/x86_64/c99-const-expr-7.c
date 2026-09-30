/* Test for constant expressions: overflow and constant expressions;
   see also overflow-warn-*.c for some other cases.  */
/* Origin: Joseph Myers <joseph@codesourcery.com> */
/* { dg-do compile } */
/* { dg-options "-std=iso9899:1999 -pedantic-errors" } */

#include <float.h>
#include <limits.h>

int a = DBL_MAX; /* { dg-warning "overflow in conversion" } */
/* { dg-error "overflow in constant expression" "constant" { target *-*-* } .-1 } */
int b = (int) DBL_MAX; /* { dg-error "overflow" } */
unsigned int c = -1.0; /* { dg-warning "overflow in conversion" } */
/* { dg-error "overflow in constant expression" "constant" { target *-*-* } .-1 } */
unsigned int d = (unsigned)-1.0; /* { dg-error "overflow" } */

int e = 0 << 1000; /* { dg-warning "shift count" } */
/* { dg-error "constant" "constant" { target *-*-* } .-1 } */
int f = 0 << -1; /* { dg-warning "shift count" } */
/* { dg-error "constant" "constant" { target *-*-* } .-1 } */
int g = 0 >> 1000; /* { dg-warning "shift count" } */
/* { dg-error "constant" "constant" { target *-*-* } .-1 } */
int h = 0 >> -1; /* { dg-warning "shift count" } */
/* { dg-error "constant" "constant" { target *-*-* } .-1 } */

int b1 = (0 ? (int) DBL_MAX : 0);
unsigned int d1 = (0 ? (unsigned int)-1.0 : 0);
int e1 = (0 ? 0 << 1000 : 0);
int f1 = (0 ? 0 << -1 : 0);
int g1 = (0 ? 0 >> 1000 : 0);
int h1 = (0 ? 0 >> -1: 0);

int i = -1 << 0;
/* { dg-error "constant" "constant" { target *-*-* } .-1 } */

int j[1] = { DBL_MAX }; /* { dg-warning "overflow in conversion" } */
/* { dg-error "overflow in constant expression" "constant" { target *-*-* } .-1 } */

int array[2] = { [0 * (INT_MAX + 1)] = 0 }; /* { dg-warning "integer overflow in expression" } */
/* { dg-error "overflow in constant expression" "constant" { target *-*-* } .-1 } */

_Bool k = INT_MAX + 1; /* { dg-warning "integer overflow in expression" } */
/* { dg-error "overflow in constant expression" "constant" { target *-*-* } .-1 } */

// SLATE-FILECHECK-STD DEFAULT iso9899:1999
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
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: i32 [storage=static] = float_to_int<i32, reason=assign, out_of_range=ub, exceptions=ignore>(float_narrow<f64, reason=explicit, rounding=nearest_even, exceptions=ignore>(const<f80>(1.79769313486231570815E+308))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: i32 [storage=static] = float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(float_narrow<f64, reason=explicit, rounding=nearest_even, exceptions=ignore>(const<f80>(1.79769313486231570815E+308))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: u32 [storage=static] = float_to_int<u32, reason=assign, out_of_range=ub, exceptions=ignore>(neg<f64>(const<f64>(1.0))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: u32 [storage=static] = float_to_int<u32, reason=explicit, out_of_range=ub, exceptions=ignore>(neg<f64>(const<f64>(1.0))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_e:[0-9]+]] e: i32 [storage=static] = shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(0), const<i32>(1000)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_f:[0-9]+]] f: i32 [storage=static] = shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(0), neg<i32, overflow=ub>(const<i32>(1))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g:[0-9]+]] g: i32 [storage=static] = shr<i32, amount_out_of_range=ub, fill=sign_extend>(const<i32>(0), const<i32>(1000)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_h:[0-9]+]] h: i32 [storage=static] = shr<i32, amount_out_of_range=ub, fill=sign_extend>(const<i32>(0), neg<i32, overflow=ub>(const<i32>(1))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b1:[0-9]+]] b1: i32 [storage=static] = conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(float_narrow<f64, reason=explicit, rounding=nearest_even, exceptions=ignore>(const<f80>(1.79769313486231570815E+308))), const<i32>(0)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d1:[0-9]+]] d1: u32 [storage=static] = conditional<u32>(ne<i32>(const<i32>(0), const<i32>(0)), float_to_int<u32, reason=explicit, out_of_range=ub, exceptions=ignore>(neg<f64>(const<f64>(1.0))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_e1:[0-9]+]] e1: i32 [storage=static] = conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(0), const<i32>(1000)), const<i32>(0)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_f1:[0-9]+]] f1: i32 [storage=static] = conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(0), neg<i32, overflow=ub>(const<i32>(1))), const<i32>(0)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g1:[0-9]+]] g1: i32 [storage=static] = conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), shr<i32, amount_out_of_range=ub, fill=sign_extend>(const<i32>(0), const<i32>(1000)), const<i32>(0)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_h1:[0-9]+]] h1: i32 [storage=static] = conditional<i32>(ne<i32>(const<i32>(0), const<i32>(0)), shr<i32, amount_out_of_range=ub, fill=sign_extend>(const<i32>(0), neg<i32, overflow=ub>(const<i32>(1))), const<i32>(0)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_i:[0-9]+]] i: i32 [storage=static] = shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(neg<i32, overflow=ub>(const<i32>(1)), const<i32>(0)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_j:[0-9]+]] j: array<i32, 1> [storage=static] = aggregate<array<i32, 1>, zero_fill=false>(index0 = float_to_int<i32, reason=assign, out_of_range=ub, exceptions=ignore>(float_narrow<f64, reason=explicit, rounding=nearest_even, exceptions=ignore>(const<f80>(1.79769313486231570815E+308)))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_array:[0-9]+]] array: array<i32, 2> [storage=static] = aggregate<array<i32, 2>, zero_fill=true>(index0 = const<i32>(0)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_k:[0-9]+]] k: bool [storage=static] = ne<i32, reason=assign>(add<i32, overflow=ub>(const<i32>(2147483647), const<i32>(1)), const<i32>(0)) [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
