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
// DEFAULT-NEXT:     type @type0 A = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:         field1 b: i32;
// DEFAULT-NEXT:         field2 c: i32;
// DEFAULT-NEXT:         field3 d: i32;
// DEFAULT-NEXT:     } [size=16, align=4, offsets=[0, 4, 8, 12]];
// DEFAULT-NEXT:     fn %0 @foo() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %2 @bar(%3 x: ptr<@type0>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %4 e: i32 [storage=automatic];
// DEFAULT-NEXT:         let %5 f: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%4, call<i32, signature=fn() -> i32>(%0));
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%0);
// DEFAULT-NEXT:         write<i32>(%4, div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%4), read<i32>(field1(deref(read<ptr<@type0>>(%3))))));
// DEFAULT-NEXT:         if lt<i32>(read<i32>(%4), const<i32>(1))
// DEFAULT-NEXT:             write<i32>(%4, const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%5, div<i32, by_zero=ub, min_by_neg_one=ub>(add<i32, overflow=ub>(read<i32>(field0(deref(read<ptr<@type0>>(%3)))), read<i32>(field2(deref(read<ptr<@type0>>(%3))))), read<i32>(%4)));
// DEFAULT-NEXT:         if lt<i32>(read<i32>(%5), read<i32>(field3(deref(read<ptr<@type0>>(%3)))))
// DEFAULT-NEXT:             let %6: ptr<@type0> [synthetic] = read<ptr<@type0>>(%3);
// DEFAULT-NEXT:             let %7: i32 [synthetic] = read<i32>(field3(deref(read<ptr<@type0>>(%6))));
// DEFAULT-NEXT:             let %8: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%7), div<i32, by_zero=ub, min_by_neg_one=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(16)), const<i32>(8)));
// DEFAULT-NEXT:             write<i32>(field3(deref(read<ptr<@type0>>(%6))), read<i32>(%8));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
