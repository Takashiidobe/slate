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
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_x:[0-9]+]] x: i129b) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_baz:[0-9]+]] @baz(%[[VALUE_x_2:[0-9]+]] x: i32, %[[VALUE_y:[0-9]+]] y: i129b) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_q:[0-9]+]] q: array<ptr<void>, 2> [storage=automatic] [align=16] = aggregate<array<ptr<void>, 2>, zero_fill=false>(index0 = label_addr<ptr<void>>(%[[VALUE_l1:[0-9]+]]), index1 = label_addr<ptr<void>>(%[[VALUE_l2:[0-9]+]]));
// DEFAULT-NEXT:         label %[[VALUE_l2]] l2:
// DEFAULT-NEXT:             write<i32>(%[[VALUE_x_2]], call<i32, signature=fn(i32) -> i32>(%[[VALUE_foo]], call<i32, signature=fn(i32) -> i32>(%[[VALUE_foo]], const<i32>(3))));
// DEFAULT-NEXT:             call<i32, signature=fn(i32) -> i32>(%[[VALUE_foo]], call<i32, signature=fn(i32) -> i32>(%[[VALUE_foo]], const<i32>(3)));
// DEFAULT-NEXT:         call<i32, signature=fn(i129b) -> i32>(%[[VALUE_bar]], read<i129b>(%[[VALUE_y]]));
// DEFAULT-NEXT:         goto *read<ptr<void>>(deref(ptr_offset<ptr<ptr<void>>, subtract=false, element=ptr<void>, overflow=ub>(array_decay<ptr<ptr<void>>, length=Some(2)>(%[[VALUE_q]]), and<i32>(read<i32>(%[[VALUE_x_2]]), const<i32>(1)))));
// DEFAULT-NEXT:         label %[[VALUE_l1]] l1:
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_qux:[0-9]+]] @qux(%[[VALUE_x_3:[0-9]+]] x: i32, %[[VALUE_y_2:[0-9]+]] y: i129b) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_q_2:[0-9]+]] q: array<ptr<void>, 2> [storage=automatic] [align=16] = aggregate<array<ptr<void>, 2>, zero_fill=false>(index0 = label_addr<ptr<void>>(%[[VALUE_l1_2:[0-9]+]]), index1 = label_addr<ptr<void>>(%[[VALUE_l2_2:[0-9]+]]));
// DEFAULT-NEXT:         label %[[VALUE_l2_2]] l2:
// DEFAULT-NEXT:             write<i32>(%[[VALUE_x_3]], call<i32, signature=fn(i32) -> i32>(%[[VALUE_foo]], call<i32, signature=fn(i32) -> i32>(%[[VALUE_foo]], const<i32>(3))));
// DEFAULT-NEXT:             call<i32, signature=fn(i32) -> i32>(%[[VALUE_foo]], call<i32, signature=fn(i32) -> i32>(%[[VALUE_foo]], const<i32>(3)));
// DEFAULT-NEXT:         call<i32, signature=fn(i129b) -> i32>(%[[VALUE_bar]], read<i129b>(%[[VALUE_y_2]]));
// DEFAULT-NEXT:         label %[[VALUE_l1_2]] l1:
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
