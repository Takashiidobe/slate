/* PR c/102989 */
/* { dg-do compile { target { bitint && dfp } } } */
/* { dg-options "-O2 -std=c23 -pedantic-errors" } */

#if __BITINT_MAXWIDTH__ >= 129
void
foo (_BitInt(129) *x, _Decimal64 *y)
{
  x[0] = y[0];
  y[1] = x[1];
}
#endif

/* _Decimal* <-> _BitInt conversions are unsupported for now.  */
/* { dg-prune-output "unsupported conversion between" } */

// SLATE-FILECHECK-FLAVOR gcc
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
// DEFAULT-NEXT:     fn %0 @foo(%1 x: ptr<i129b>, %2 y: ptr<d64>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i129b>(deref(ptr_offset<ptr<i129b>, subtract=false, element=i129b, overflow=ub>(read<ptr<i129b>>(%1), const<i32>(0))), float_to_int<i129b, reason=assign, out_of_range=ub, exceptions=observable>(read<d64>(deref(ptr_offset<ptr<d64>, subtract=false, element=d64, overflow=ub>(read<ptr<d64>>(%2), const<i32>(0))))));
// DEFAULT-NEXT:         write<d64>(deref(ptr_offset<ptr<d64>, subtract=false, element=d64, overflow=ub>(read<ptr<d64>>(%2), const<i32>(1))), int_to_float<d64, reason=assign, exact=false, rounding=nearest_even, exceptions=observable>(read<i129b>(deref(ptr_offset<ptr<i129b>, subtract=false, element=i129b, overflow=ub>(read<ptr<i129b>>(%1), const<i32>(1))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
