/* { dg-do compile } */
/* { dg-options "-w -fdump-tree-gimple" } */

/* Things that should be folded.  */

struct { long base; int tail; void * volatile ptr; } *s;
int foo1a (void) { return (s == &s->base); }
int foo1b (void) { return (&s->base == s); }
int foo2 (void) { return ((void *)s == (void *) &s->base); }
int foo4 (void) { return s->base == s->base; }
int foo5 (void) { return &s->ptr == &s->ptr; }
int foo6 (void) { return &s->ptr != &s->ptr; }
int foo7 (void) { return &s->base != &s->ptr; }

struct { union { int i; short s } u; } x;
int foo8 (void) { return &x.u.i == &x.u.s; }

/* { dg-final { scan-tree-dump-times "= 0" 1 "gimple" } } */
/* { dg-final { scan-tree-dump-times "= 1" 7 "gimple" } } */

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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 base: i64;
// DEFAULT-NEXT:         field1 tail: i32;
// DEFAULT-NEXT:         field2 ptr: volatile ptr<void>;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 8, 16]];
// DEFAULT-NEXT:     type @type[[TYPE1:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 u: @type[[TYPE2:[0-9]+]];
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE2]] = union {
// DEFAULT-NEXT:         field0 i: i32;
// DEFAULT-NEXT:         field1 s: i16;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// DEFAULT-NEXT:     global %[[VALUE_s:[0-9]+]] s: ptr<@type[[TYPE0]]> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_x:[0-9]+]] x: @type[[TYPE1]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo1a:[0-9]+]] @foo1a() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<ptr<@type[[TYPE0]]>>(read<ptr<@type[[TYPE0]]>>(%[[VALUE_s]]), pointer_cast<ptr<@type[[TYPE0]]>, reason=usual_arith>(addr_of<ptr<i64>>(field0(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_s]])))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo1b:[0-9]+]] @foo1b() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<ptr<i64>>(addr_of<ptr<i64>>(field0(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_s]])))), pointer_cast<ptr<i64>, reason=usual_arith>(read<ptr<@type[[TYPE0]]>>(%[[VALUE_s]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo2:[0-9]+]] @foo2() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<ptr<void>>(pointer_cast<ptr<void>, reason=explicit>(read<ptr<@type[[TYPE0]]>>(%[[VALUE_s]])), pointer_cast<ptr<void>, reason=explicit>(addr_of<ptr<i64>>(field0(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_s]])))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo4:[0-9]+]] @foo4() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<i64>(read<i64>(field0(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_s]])))), read<i64>(field0(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_s]]))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo5:[0-9]+]] @foo5() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<ptr<volatile ptr<void>>>(addr_of<ptr<volatile ptr<void>>>(field2(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_s]])))), addr_of<ptr<volatile ptr<void>>>(field2(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_s]]))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo6:[0-9]+]] @foo6() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ne<ptr<volatile ptr<void>>>(addr_of<ptr<volatile ptr<void>>>(field2(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_s]])))), addr_of<ptr<volatile ptr<void>>>(field2(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_s]]))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo7:[0-9]+]] @foo7() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ne<ptr<i64>>(addr_of<ptr<i64>>(field0(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_s]])))), pointer_cast<ptr<i64>, reason=usual_arith>(addr_of<ptr<volatile ptr<void>>>(field2(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_s]])))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo8:[0-9]+]] @foo8() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<ptr<i32>>(addr_of<ptr<i32>>(field0(field0(%[[VALUE_x]]))), pointer_cast<ptr<i32>, reason=usual_arith>(addr_of<ptr<i16>>(field1(field0(%[[VALUE_x]]))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
