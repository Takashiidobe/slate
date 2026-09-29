/* { dg-do compile } */
/* { dg-options "-std=gnu23" } */

#define NEST(...) typeof(({ (__VA_ARGS__){ }; }))

int f()
{
    typedef struct foo bar;
    struct foo { NEST(struct foo { bar *x; }) *x; } *q;
    typeof(q->x) p0;
    typeof(q->x) p1;
    1 ? p0 : q;
    1 ? p1 : q;
    1 ? p0 : p1;
}

int g()
{
    typedef struct fo2 bar;
    struct fo2 { NEST(struct fo2 { NEST(struct fo2 { bar *x; }) * x; }) *x; } *q;
    typeof(q->x) p0;
    typeof(q->x->x) p1;
    typeof(q->x->x->x) p2;
    1 ? p0 : q;
    1 ? p1 : q;
    1 ? p2 : q;
    1 ? p0 : p1;
    1 ? p2 : p1;
    1 ? p0 : p2;
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
// DEFAULT-NEXT:     type @type0 foo = struct {
// DEFAULT-NEXT:         field0 x: ptr<@type2>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type1 bar = @type0;
// DEFAULT-NEXT:     type @type2 foo = struct {
// DEFAULT-NEXT:         field0 x: ptr<@type0>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type3 fo2 = struct {
// DEFAULT-NEXT:         field0 x: ptr<@type5>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type4 bar = @type3;
// DEFAULT-NEXT:     type @type5 fo2 = struct {
// DEFAULT-NEXT:         field0 x: ptr<@type6>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type6 fo2 = struct {
// DEFAULT-NEXT:         field0 x: ptr<@type3>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     fn %0 @f() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %4 q: ptr<@type0> [storage=automatic];
// DEFAULT-NEXT:         let %5 p0: ptr<@type2> [storage=automatic];
// DEFAULT-NEXT:         let %6 p1: ptr<@type2> [storage=automatic];
// DEFAULT-NEXT:         conditional<ptr<@type2>>(ne<i32>(const<i32>(1), const<i32>(0)), read<ptr<@type2>>(%5), pointer_cast<ptr<@type2>, reason=usual_arith>(read<ptr<@type0>>(%4)));
// DEFAULT-NEXT:         conditional<ptr<@type2>>(ne<i32>(const<i32>(1), const<i32>(0)), read<ptr<@type2>>(%6), pointer_cast<ptr<@type2>, reason=usual_arith>(read<ptr<@type0>>(%4)));
// DEFAULT-NEXT:         conditional<ptr<@type2>>(ne<i32>(const<i32>(1), const<i32>(0)), read<ptr<@type2>>(%5), read<ptr<@type2>>(%6));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @g() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %12 q: ptr<@type3> [storage=automatic];
// DEFAULT-NEXT:         let %13 p0: ptr<@type5> [storage=automatic];
// DEFAULT-NEXT:         let %14 p1: ptr<@type6> [storage=automatic];
// DEFAULT-NEXT:         let %15 p2: ptr<@type3> [storage=automatic];
// DEFAULT-NEXT:         conditional<ptr<@type5>>(ne<i32>(const<i32>(1), const<i32>(0)), read<ptr<@type5>>(%13), pointer_cast<ptr<@type5>, reason=usual_arith>(read<ptr<@type3>>(%12)));
// DEFAULT-NEXT:         conditional<ptr<@type6>>(ne<i32>(const<i32>(1), const<i32>(0)), read<ptr<@type6>>(%14), pointer_cast<ptr<@type6>, reason=usual_arith>(read<ptr<@type3>>(%12)));
// DEFAULT-NEXT:         conditional<ptr<@type3>>(ne<i32>(const<i32>(1), const<i32>(0)), read<ptr<@type3>>(%15), read<ptr<@type3>>(%12));
// DEFAULT-NEXT:         conditional<ptr<@type5>>(ne<i32>(const<i32>(1), const<i32>(0)), read<ptr<@type5>>(%13), pointer_cast<ptr<@type5>, reason=usual_arith>(read<ptr<@type6>>(%14)));
// DEFAULT-NEXT:         conditional<ptr<@type3>>(ne<i32>(const<i32>(1), const<i32>(0)), read<ptr<@type3>>(%15), pointer_cast<ptr<@type3>, reason=usual_arith>(read<ptr<@type6>>(%14)));
// DEFAULT-NEXT:         conditional<ptr<@type5>>(ne<i32>(const<i32>(1), const<i32>(0)), read<ptr<@type5>>(%13), pointer_cast<ptr<@type5>, reason=usual_arith>(read<ptr<@type3>>(%15)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
