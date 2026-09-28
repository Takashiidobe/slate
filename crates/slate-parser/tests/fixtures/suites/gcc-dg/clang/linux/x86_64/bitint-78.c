/* PR tree-optimization/113568 */
/* { dg-do compile { target bitint } } */
/* { dg-options "-O2 -std=c23" } */

signed char c;
#if __BITINT_MAXWIDTH__ >= 464
_BitInt(464) g;

void
foo (void)
{
  _BitInt(464) a[2] = {};
  _BitInt(464) b;
  while (c)
    {
      b = g + 1;
      g = a[0];
      a[0] = b;
    }
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
// DEFAULT-NEXT:     global %0 c: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1 g: i464b [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %2 @foo() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %3 a: array<i464b, 2> [storage=automatic] [align=16] = aggregate<array<i464b, 2>, zero_fill=true>();
// DEFAULT-NEXT:         let %4 b: i464b [storage=automatic];
// DEFAULT-NEXT:         while %5 ne<i8>(read<i8>(%0), const<i8>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<i464b>(%4, add<i464b, overflow=ub>(read<i464b>(%1), widen<i464b, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                 write<i464b>(%1, read<i464b>(deref(ptr_offset<ptr<i464b>, subtract=false, element=i464b, overflow=ub>(array_decay<ptr<i464b>, length=Some(2)>(%3), const<i32>(0)))));
// DEFAULT-NEXT:                 write<i464b>(deref(ptr_offset<ptr<i464b>, subtract=false, element=i464b, overflow=ub>(array_decay<ptr<i464b>, length=Some(2)>(%3), const<i32>(0))), read<i464b>(%4));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
