/* Test for constant expressions: C90 aggregate initializers requiring
   constant expressions.  */
/* Origin: Joseph Myers <joseph@codesourcery.com> */
/* { dg-do compile } */
/* { dg-options "-std=iso9899:1990 -pedantic-errors -O2 -fno-trapping-math" } */

#include <float.h>
#include <limits.h>

double atan(double);

struct s { double d; };
struct t { int i; };

void
f (void)
{
  /* As in PR 14649 for static initializers.  */
  struct s a = { atan (1.0) }; /* { dg-error "is not a constant expression|near initialization" } */
  /* Overflow.  */
  struct t b = { INT_MAX + 1 }; /* { dg-warning "integer overflow in expression" } */
  /* { dg-error "overflow in constant expression" "constant" { target *-*-* } .-1 } */
  struct t c = { DBL_MAX }; /* { dg-warning "overflow in conversion from .double. to .int. changes value " } */
  /* { dg-error "overflow in constant expression" "constant" { target *-*-* } .-1 } */
  /* Bad operator outside sizeof.  */
  struct s d = { 1 ? 1.0 : atan (a.d) }; /* { dg-error "is not a constant expression|near initialization" } */
}

// SLATE-FILECHECK-STD DEFAULT iso9899:1990
// SLATE-FILECHECK-ARGS -fno-trapping-math
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
// DEFAULT-NEXT:     type @type0 s = struct {
// DEFAULT-NEXT:         field0 d: f64;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type1 t = struct {
// DEFAULT-NEXT:         field0 i: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     fn %0 @atan(%8 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %3 @f() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %4 a: @type0 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = call<f64, signature=fn(f64) -> f64>(%0, const<f64>(1.0)));
// DEFAULT-NEXT:         let %5 b: @type1 [storage=automatic] = aggregate<@type1, zero_fill=false>(field0 = add<i32, overflow=ub>(const<i32>(2147483647), const<i32>(1)));
// DEFAULT-NEXT:         let %6 c: @type1 [storage=automatic] = aggregate<@type1, zero_fill=false>(field0 = float_to_int<i32, reason=assign, out_of_range=ub, exceptions=ignore>(float_narrow<f64, reason=explicit, rounding=nearest_even, exceptions=ignore>(const<f80>(1.79769313486231570815E+308))));
// DEFAULT-NEXT:         let %7 d: @type0 [storage=automatic];
// DEFAULT-NEXT:         let %9: f64 [synthetic];
// DEFAULT-NEXT:         if ne<i32>(const<i32>(1), const<i32>(0))
// DEFAULT-NEXT:             write<f64>(%9, const<f64>(1.0));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<f64>(%9, call<f64, signature=fn(f64) -> f64>(%0, read<f64>(field0(%4))));
// DEFAULT-NEXT:         write<@type0>(%7, aggregate<@type0, zero_fill=false>(field0 = read<f64>(%9)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
