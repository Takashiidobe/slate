/* PR c/102989 */
/* { dg-do compile { target bitint } } */
/* { dg-options "-std=c23 -pedantic-errors" } */

#if __BITINT_MAXWIDTH__ >= 257
void
foo (_BitInt(135) *p, _BitInt(193) *q, _BitInt(257) *r)
{
  r[0] = (((p[0] + p[1] + p[2]) + q[0] + (p[3] + p[4] + p[5])) + q[1]) + r[1] + (((p[6] + p[7] + p[8]) + q[2] + (p[9] + p[10] + p[11])) + q[3]) + r[2];
}
#else
void
foo (void)
{
}
#endif

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
// DEFAULT-NEXT:     fn %0 @foo(%1 p: ptr<i135b>, %2 q: ptr<i193b>, %3 r: ptr<i257b>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i257b>(deref(ptr_offset<ptr<i257b>, subtract=false, element=i257b, overflow=ub>(read<ptr<i257b>>(%3), const<i32>(0))), add<i257b, overflow=ub>(add<i257b, overflow=ub>(add<i257b, overflow=ub>(widen<i257b, reason=usual_arith>(add<i193b, overflow=ub>(add<i193b, overflow=ub>(add<i193b, overflow=ub>(widen<i193b, reason=usual_arith>(add<i135b, overflow=ub>(add<i135b, overflow=ub>(read<i135b>(deref(ptr_offset<ptr<i135b>, subtract=false, element=i135b, overflow=ub>(read<ptr<i135b>>(%1), const<i32>(0)))), read<i135b>(deref(ptr_offset<ptr<i135b>, subtract=false, element=i135b, overflow=ub>(read<ptr<i135b>>(%1), const<i32>(1))))), read<i135b>(deref(ptr_offset<ptr<i135b>, subtract=false, element=i135b, overflow=ub>(read<ptr<i135b>>(%1), const<i32>(2)))))), read<i193b>(deref(ptr_offset<ptr<i193b>, subtract=false, element=i193b, overflow=ub>(read<ptr<i193b>>(%2), const<i32>(0))))), widen<i193b, reason=usual_arith>(add<i135b, overflow=ub>(add<i135b, overflow=ub>(read<i135b>(deref(ptr_offset<ptr<i135b>, subtract=false, element=i135b, overflow=ub>(read<ptr<i135b>>(%1), const<i32>(3)))), read<i135b>(deref(ptr_offset<ptr<i135b>, subtract=false, element=i135b, overflow=ub>(read<ptr<i135b>>(%1), const<i32>(4))))), read<i135b>(deref(ptr_offset<ptr<i135b>, subtract=false, element=i135b, overflow=ub>(read<ptr<i135b>>(%1), const<i32>(5))))))), read<i193b>(deref(ptr_offset<ptr<i193b>, subtract=false, element=i193b, overflow=ub>(read<ptr<i193b>>(%2), const<i32>(1)))))), read<i257b>(deref(ptr_offset<ptr<i257b>, subtract=false, element=i257b, overflow=ub>(read<ptr<i257b>>(%3), const<i32>(1))))), widen<i257b, reason=usual_arith>(add<i193b, overflow=ub>(add<i193b, overflow=ub>(add<i193b, overflow=ub>(widen<i193b, reason=usual_arith>(add<i135b, overflow=ub>(add<i135b, overflow=ub>(read<i135b>(deref(ptr_offset<ptr<i135b>, subtract=false, element=i135b, overflow=ub>(read<ptr<i135b>>(%1), const<i32>(6)))), read<i135b>(deref(ptr_offset<ptr<i135b>, subtract=false, element=i135b, overflow=ub>(read<ptr<i135b>>(%1), const<i32>(7))))), read<i135b>(deref(ptr_offset<ptr<i135b>, subtract=false, element=i135b, overflow=ub>(read<ptr<i135b>>(%1), const<i32>(8)))))), read<i193b>(deref(ptr_offset<ptr<i193b>, subtract=false, element=i193b, overflow=ub>(read<ptr<i193b>>(%2), const<i32>(2))))), widen<i193b, reason=usual_arith>(add<i135b, overflow=ub>(add<i135b, overflow=ub>(read<i135b>(deref(ptr_offset<ptr<i135b>, subtract=false, element=i135b, overflow=ub>(read<ptr<i135b>>(%1), const<i32>(9)))), read<i135b>(deref(ptr_offset<ptr<i135b>, subtract=false, element=i135b, overflow=ub>(read<ptr<i135b>>(%1), const<i32>(10))))), read<i135b>(deref(ptr_offset<ptr<i135b>, subtract=false, element=i135b, overflow=ub>(read<ptr<i135b>>(%1), const<i32>(11))))))), read<i193b>(deref(ptr_offset<ptr<i193b>, subtract=false, element=i193b, overflow=ub>(read<ptr<i193b>>(%2), const<i32>(3))))))), read<i257b>(deref(ptr_offset<ptr<i257b>, subtract=false, element=i257b, overflow=ub>(read<ptr<i257b>>(%3), const<i32>(2))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
