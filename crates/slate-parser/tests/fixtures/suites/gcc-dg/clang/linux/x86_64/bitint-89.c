/* PR tree-optimization/113818 */
/* { dg-do compile { target bitint } } */
/* { dg-options "-Os -fnon-call-exceptions -finstrument-functions-once" } */

int c, i;
void bar (int *);

#if __BITINT_MAXWIDTH__ >= 129
_BitInt(129) *a;
#else
_BitInt(63) *a;
#endif

void
foo (void)
{
  if (c)
    return;
  int q;
  a[i] = 0;
  bar (&q);
}

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
// DEFAULT-NEXT:     global %0 c: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1 i: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 a: ptr<i129b> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %2 @bar(%6 <unnamed>: ptr<i32>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %4 @foo() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%0), const<i32>(0))
// DEFAULT-NEXT:             return;
// DEFAULT-NEXT:         let %5 q: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i129b>(deref(ptr_offset<ptr<i129b>, subtract=false, element=i129b, overflow=ub>(read<ptr<i129b>>(%3), read<i32>(%1))), widen<i129b, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>) -> void>(%2, addr_of<ptr<i32>>(%5));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
