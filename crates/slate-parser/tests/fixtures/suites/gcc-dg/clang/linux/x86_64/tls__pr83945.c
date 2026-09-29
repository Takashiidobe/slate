/* PR middle-end/83945 */
/* { dg-do compile { target tls } } */
/* { dg-options "-O2" } */

struct S { int a[1]; };
__thread struct T { int c; } e;
int f;
void bar (int);

void
foo (int f, int x)
{
  struct S *h = (struct S *) &e.c;
  for (;;)
    {
      int *a = h->a, i;
      for (i = x; i; i--)
	bar (a[f]);
      bar (a[f]);
    }
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
// DEFAULT-NEXT:     type @type[[TYPE_S:[0-9]+]] S = struct {
// DEFAULT-NEXT:         field0 a: array<i32, 1>;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_T:[0-9]+]] T = struct {
// DEFAULT-NEXT:         field0 c: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     global %[[VALUE_e:[0-9]+]] e: @type[[TYPE_T]] [storage=thread] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_f:[0-9]+]] f: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_f_2:[0-9]+]] f: i32, %[[VALUE_x:[0-9]+]] x: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_h:[0-9]+]] h: ptr<@type[[TYPE_S]]> [storage=automatic] = pointer_cast<ptr<@type[[TYPE_S]]>, reason=explicit>(addr_of<ptr<i32>>(field0(%[[VALUE_e]])));
// DEFAULT-NEXT:         for %[[VALUE1:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: omitted
// DEFAULT-NEXT:             increment: omitted
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE_a:[0-9]+]] a: ptr<i32> [storage=automatic] = array_decay<ptr<i32>, length=Some(1)>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_h]]))));
// DEFAULT-NEXT:                     let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:                     for %[[VALUE2:[0-9]+]]
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE_x]]));
// DEFAULT-NEXT:                         condition: ne<i32>(read<i32>(%[[VALUE_i]]), const<i32>(0))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %[[VALUE3:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                             let %[[VALUE4:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE3]]), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE4]]));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             call<void, signature=fn(i32) -> void>(%[[VALUE_bar]], read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_a]]), read<i32>(%[[VALUE_f_2]])))));
// DEFAULT-NEXT:                     call<void, signature=fn(i32) -> void>(%[[VALUE_bar]], read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_a]]), read<i32>(%[[VALUE_f_2]])))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
