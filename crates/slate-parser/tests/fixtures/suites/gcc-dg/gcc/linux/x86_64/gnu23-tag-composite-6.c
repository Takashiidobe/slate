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
// DEFAULT-NEXT:     type @type[[TYPE_foo:[0-9]+]] foo = struct {
// DEFAULT-NEXT:         field0 x: ptr<@type[[TYPE_foo_2:[0-9]+]]>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_bar:[0-9]+]] bar = @type[[TYPE_foo]];
// DEFAULT-NEXT:     type @type[[TYPE_foo_2]] foo = struct {
// DEFAULT-NEXT:         field0 x: ptr<@type[[TYPE_foo]]>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_fo2:[0-9]+]] fo2 = struct {
// DEFAULT-NEXT:         field0 x: ptr<@type[[TYPE_fo2_2:[0-9]+]]>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_bar_2:[0-9]+]] bar = @type[[TYPE_fo2]];
// DEFAULT-NEXT:     type @type[[TYPE_fo2_2]] fo2 = struct {
// DEFAULT-NEXT:         field0 x: ptr<@type[[TYPE_fo2_3:[0-9]+]]>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_fo2_3]] fo2 = struct {
// DEFAULT-NEXT:         field0 x: ptr<@type[[TYPE_fo2]]>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_q:[0-9]+]] q: ptr<@type[[TYPE_foo]]> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_p0:[0-9]+]] p0: ptr<@type[[TYPE_foo_2]]> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_p1:[0-9]+]] p1: ptr<@type[[TYPE_foo_2]]> [storage=automatic];
// DEFAULT-NEXT:         conditional<ptr<@type[[TYPE_foo_2]]>>(ne<i32>(const<i32>(1), const<i32>(0)), read<ptr<@type[[TYPE_foo_2]]>>(%[[VALUE_p0]]), pointer_cast<ptr<@type[[TYPE_foo_2]]>, reason=usual_arith>(read<ptr<@type[[TYPE_foo]]>>(%[[VALUE_q]])));
// DEFAULT-NEXT:         conditional<ptr<@type[[TYPE_foo_2]]>>(ne<i32>(const<i32>(1), const<i32>(0)), read<ptr<@type[[TYPE_foo_2]]>>(%[[VALUE_p1]]), pointer_cast<ptr<@type[[TYPE_foo_2]]>, reason=usual_arith>(read<ptr<@type[[TYPE_foo]]>>(%[[VALUE_q]])));
// DEFAULT-NEXT:         conditional<ptr<@type[[TYPE_foo_2]]>>(ne<i32>(const<i32>(1), const<i32>(0)), read<ptr<@type[[TYPE_foo_2]]>>(%[[VALUE_p0]]), read<ptr<@type[[TYPE_foo_2]]>>(%[[VALUE_p1]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_g:[0-9]+]] @g() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_q_2:[0-9]+]] q: ptr<@type[[TYPE_fo2]]> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_p0_2:[0-9]+]] p0: ptr<@type[[TYPE_fo2_2]]> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_p1_2:[0-9]+]] p1: ptr<@type[[TYPE_fo2_3]]> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_p2:[0-9]+]] p2: ptr<@type[[TYPE_fo2]]> [storage=automatic];
// DEFAULT-NEXT:         conditional<ptr<@type[[TYPE_fo2_2]]>>(ne<i32>(const<i32>(1), const<i32>(0)), read<ptr<@type[[TYPE_fo2_2]]>>(%[[VALUE_p0_2]]), pointer_cast<ptr<@type[[TYPE_fo2_2]]>, reason=usual_arith>(read<ptr<@type[[TYPE_fo2]]>>(%[[VALUE_q_2]])));
// DEFAULT-NEXT:         conditional<ptr<@type[[TYPE_fo2_3]]>>(ne<i32>(const<i32>(1), const<i32>(0)), read<ptr<@type[[TYPE_fo2_3]]>>(%[[VALUE_p1_2]]), pointer_cast<ptr<@type[[TYPE_fo2_3]]>, reason=usual_arith>(read<ptr<@type[[TYPE_fo2]]>>(%[[VALUE_q_2]])));
// DEFAULT-NEXT:         conditional<ptr<@type[[TYPE_fo2]]>>(ne<i32>(const<i32>(1), const<i32>(0)), read<ptr<@type[[TYPE_fo2]]>>(%[[VALUE_p2]]), read<ptr<@type[[TYPE_fo2]]>>(%[[VALUE_q_2]]));
// DEFAULT-NEXT:         conditional<ptr<@type[[TYPE_fo2_2]]>>(ne<i32>(const<i32>(1), const<i32>(0)), read<ptr<@type[[TYPE_fo2_2]]>>(%[[VALUE_p0_2]]), pointer_cast<ptr<@type[[TYPE_fo2_2]]>, reason=usual_arith>(read<ptr<@type[[TYPE_fo2_3]]>>(%[[VALUE_p1_2]])));
// DEFAULT-NEXT:         conditional<ptr<@type[[TYPE_fo2]]>>(ne<i32>(const<i32>(1), const<i32>(0)), read<ptr<@type[[TYPE_fo2]]>>(%[[VALUE_p2]]), pointer_cast<ptr<@type[[TYPE_fo2]]>, reason=usual_arith>(read<ptr<@type[[TYPE_fo2_3]]>>(%[[VALUE_p1_2]])));
// DEFAULT-NEXT:         conditional<ptr<@type[[TYPE_fo2_2]]>>(ne<i32>(const<i32>(1), const<i32>(0)), read<ptr<@type[[TYPE_fo2_2]]>>(%[[VALUE_p0_2]]), pointer_cast<ptr<@type[[TYPE_fo2_2]]>, reason=usual_arith>(read<ptr<@type[[TYPE_fo2]]>>(%[[VALUE_p2]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
