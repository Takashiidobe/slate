// SLATE-FILECHECK-DEFINES DEFAULT

/* This testcase failed on Alpha at -O2 when simplifying conditional
   expressions.  */

int foo (void);

struct A
{
  int a, b, c, d;
};

void bar (struct A *x)
{
  int e, f;

  e = foo ();
  e = e / x->b;
  if (e < 1)
    e = 1;
  f = (x->a + x->c) / e;
  if (f < x->d)
    x->d -= (1 << 16) / 8;
}

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
// DEFAULT-NEXT:     type @type[[TYPE_A:[0-9]+]] A = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:         field1 b: i32;
// DEFAULT-NEXT:         field2 c: i32;
// DEFAULT-NEXT:         field3 d: i32;
// DEFAULT-NEXT:     } [size=16, align=4, offsets=[0, 4, 8, 12]];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_x:[0-9]+]] x: ptr<@type[[TYPE_A]]>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_e:[0-9]+]] e: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_f:[0-9]+]] f: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%[[VALUE_e]], call<i32, signature=fn() -> i32>(%[[VALUE_foo]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_e]], div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%[[VALUE_e]]), read<i32>(field1(deref(read<ptr<@type[[TYPE_A]]>>(%[[VALUE_x]]))))));
// DEFAULT-NEXT:         if lt<i32>(read<i32>(%[[VALUE_e]]), const<i32>(1))
// DEFAULT-NEXT:             write<i32>(%[[VALUE_e]], const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_f]], div<i32, by_zero=ub, min_by_neg_one=ub>(add<i32, overflow=ub>(read<i32>(field0(deref(read<ptr<@type[[TYPE_A]]>>(%[[VALUE_x]])))), read<i32>(field2(deref(read<ptr<@type[[TYPE_A]]>>(%[[VALUE_x]]))))), read<i32>(%[[VALUE_e]])));
// DEFAULT-NEXT:         if lt<i32>(read<i32>(%[[VALUE_f]]), read<i32>(field3(deref(read<ptr<@type[[TYPE_A]]>>(%[[VALUE_x]])))))
// DEFAULT-NEXT:             let %[[VALUE0:[0-9]+]]: ptr<@type[[TYPE_A]]> [synthetic] = read<ptr<@type[[TYPE_A]]>>(%[[VALUE_x]]);
// DEFAULT-NEXT:             let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(field3(deref(read<ptr<@type[[TYPE_A]]>>(%[[VALUE0]]))));
// DEFAULT-NEXT:             let %[[VALUE2:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE1]]), div<i32, by_zero=ub, min_by_neg_one=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(16)), const<i32>(8)));
// DEFAULT-NEXT:             write<i32>(field3(deref(read<ptr<@type[[TYPE_A]]>>(%[[VALUE0]]))), read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
