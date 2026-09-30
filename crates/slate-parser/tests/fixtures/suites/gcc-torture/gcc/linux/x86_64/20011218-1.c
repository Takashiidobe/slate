// SLATE-FILECHECK-DEFINES DEFAULT

/* This testcase failed on Alpha at -O2 because $27 hard register
   for the indirect call was exposed too early and reload couldn't
   allocate it for multiplication and division.  */

/* { dg-require-effective-target indirect_calls } */

struct S {
  int a, b;
  void (*f) (long, int);
};

void foo (struct S *x)
{
  long c = x->a * 50;
  c /= (long) x->b;
  c *= (long) x->b;
  x->f (c, 0);
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
// DEFAULT-NEXT:     type @type[[TYPE_S:[0-9]+]] S = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:         field1 b: i32;
// DEFAULT-NEXT:         field2 f: ptr<fn(i64, i32) -> void>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_x:[0-9]+]] x: ptr<@type[[TYPE_S]]>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_c:[0-9]+]] c: i64 [storage=automatic] = widen<i64, reason=assign>(mul<i32, overflow=ub>(read<i32>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_x]])))), const<i32>(50)));
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: i64 [synthetic] = read<i64>(%[[VALUE_c]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i64 [synthetic] = div<i64, by_zero=ub, min_by_neg_one=ub>(read<i64>(%[[VALUE0]]), widen<i64, reason=explicit>(read<i32>(field1(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_x]]))))));
// DEFAULT-NEXT:         write<i64>(%[[VALUE_c]], read<i64>(%[[VALUE1]]));
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: i64 [synthetic] = read<i64>(%[[VALUE_c]]);
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: i64 [synthetic] = mul<i64, overflow=ub>(read<i64>(%[[VALUE2]]), widen<i64, reason=explicit>(read<i32>(field1(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_x]]))))));
// DEFAULT-NEXT:         write<i64>(%[[VALUE_c]], read<i64>(%[[VALUE3]]));
// DEFAULT-NEXT:         call<void, signature=fn(i64, i32) -> void>(read<ptr<fn(i64, i32) -> void>>(field2(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_x]])))), read<i64>(%[[VALUE_c]]), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
