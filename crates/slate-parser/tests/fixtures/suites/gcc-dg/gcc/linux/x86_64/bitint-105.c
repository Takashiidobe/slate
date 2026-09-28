/* PR middle-end/114628 */
/* { dg-do compile { target bitint } } */
/* { dg-options "-O2 -g" } */

int foo (int);
#if __BITINT_MAXWIDTH__ >= 129
__attribute__((returns_twice)) int bar (_BitInt(129) x);

void
baz (int x, _BitInt(129) y)
{
  void *q[] = { &&l1, &&l2 };
l2:
  x = foo (foo (3));
  bar (y);
  goto *q[x & 1];
l1:;
}

void
qux (int x, _BitInt(129) y)
{
  void *q[] = { &&l1, &&l2 };
l2:
  x = foo (foo (3));
  bar (y);
l1:;
}
#endif

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
// DEFAULT-NEXT:     fn %0 @foo(%14 <unnamed>: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %1 @bar(%15 x: i129b) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %2 @baz(%5 x: i32, %6 y: i129b) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %7 q: array<ptr<void>, 2> [storage=automatic] [align=16] = aggregate<array<ptr<void>, 2>, zero_fill=false>(index0 = label_addr<ptr<void>>(%4), index1 = label_addr<ptr<void>>(%3));
// DEFAULT-NEXT:         label %3 l2:
// DEFAULT-NEXT:             write<i32>(%5, call<i32, signature=fn(i32) -> i32>(%0, call<i32, signature=fn(i32) -> i32>(%0, const<i32>(3))));
// DEFAULT-NEXT:             call<i32, signature=fn(i32) -> i32>(%0, call<i32, signature=fn(i32) -> i32>(%0, const<i32>(3)));
// DEFAULT-NEXT:         call<i32, signature=fn(i129b) -> i32>(%1, read<i129b>(%6));
// DEFAULT-NEXT:         goto *read<ptr<void>>(deref(ptr_offset<ptr<ptr<void>>, subtract=false, element=ptr<void>, overflow=ub>(array_decay<ptr<ptr<void>>, length=Some(2)>(%7), and<i32>(read<i32>(%5), const<i32>(1)))));
// DEFAULT-NEXT:         label %4 l1:
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @qux(%11 x: i32, %12 y: i129b) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %13 q: array<ptr<void>, 2> [storage=automatic] [align=16] = aggregate<array<ptr<void>, 2>, zero_fill=false>(index0 = label_addr<ptr<void>>(%10), index1 = label_addr<ptr<void>>(%9));
// DEFAULT-NEXT:         label %9 l2:
// DEFAULT-NEXT:             write<i32>(%11, call<i32, signature=fn(i32) -> i32>(%0, call<i32, signature=fn(i32) -> i32>(%0, const<i32>(3))));
// DEFAULT-NEXT:             call<i32, signature=fn(i32) -> i32>(%0, call<i32, signature=fn(i32) -> i32>(%0, const<i32>(3)));
// DEFAULT-NEXT:         call<i32, signature=fn(i129b) -> i32>(%1, read<i129b>(%12));
// DEFAULT-NEXT:         label %10 l1:
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
