/* { dg-do compile } */
/* { dg-options "-w -fdump-tree-gimple" } */

/* Things that should not be folded.  */

struct { long base; int tail; void * volatile ptr; } *s;
int foo3 (void) { return s == &s; }
int foo5 (void) { return s->ptr == s->ptr; }

struct { union { int i; short s } u; } x;
int foo6 (void) { return x.u.i == x.u.s; }

void **p;
int foo8 (void) { return p == &p; }
int foo9 (void) { return *p == p; }
int foo10 (void) { return *p == &p; }
int foo11 (void) { return p != &p; }
int foo12 (void) { return *p != p; }
int foo13 (void) { return *p != &p; }

/* { dg-final { scan-tree-dump-not "= 0;" "gimple" } } */
/* { dg-final { scan-tree-dump-not "= 1;" "gimple" } } */

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
// DEFAULT-NEXT:     global %[[VALUE_p:[0-9]+]] p: ptr<ptr<void>> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo3:[0-9]+]] @foo3() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<ptr<@type[[TYPE0]]>>(read<ptr<@type[[TYPE0]]>>(%[[VALUE_s]]), pointer_cast<ptr<@type[[TYPE0]]>, reason=usual_arith>(addr_of<ptr<ptr<@type[[TYPE0]]>>>(%[[VALUE_s]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo5:[0-9]+]] @foo5() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<ptr<void>>(read<ptr<void>, volatile>(field2(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_s]])))), read<ptr<void>, volatile>(field2(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_s]]))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo6:[0-9]+]] @foo6() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<i32>(read<i32>(field0(field0(%[[VALUE_x]]))), widen<i32, reason=promotion>(read<i16>(field1(field0(%[[VALUE_x]]))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo8:[0-9]+]] @foo8() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<ptr<ptr<void>>>(read<ptr<ptr<void>>>(%[[VALUE_p]]), pointer_cast<ptr<ptr<void>>, reason=usual_arith>(addr_of<ptr<ptr<ptr<void>>>>(%[[VALUE_p]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo9:[0-9]+]] @foo9() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<ptr<void>>(read<ptr<void>>(deref(read<ptr<ptr<void>>>(%[[VALUE_p]]))), pointer_cast<ptr<void>, reason=usual_arith>(read<ptr<ptr<void>>>(%[[VALUE_p]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo10:[0-9]+]] @foo10() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<ptr<void>>(read<ptr<void>>(deref(read<ptr<ptr<void>>>(%[[VALUE_p]]))), pointer_cast<ptr<void>, reason=usual_arith>(addr_of<ptr<ptr<ptr<void>>>>(%[[VALUE_p]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo11:[0-9]+]] @foo11() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ne<ptr<ptr<void>>>(read<ptr<ptr<void>>>(%[[VALUE_p]]), pointer_cast<ptr<ptr<void>>, reason=usual_arith>(addr_of<ptr<ptr<ptr<void>>>>(%[[VALUE_p]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo12:[0-9]+]] @foo12() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ne<ptr<void>>(read<ptr<void>>(deref(read<ptr<ptr<void>>>(%[[VALUE_p]]))), pointer_cast<ptr<void>, reason=usual_arith>(read<ptr<ptr<void>>>(%[[VALUE_p]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo13:[0-9]+]] @foo13() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ne<ptr<void>>(read<ptr<void>>(deref(read<ptr<ptr<void>>>(%[[VALUE_p]]))), pointer_cast<ptr<void>, reason=usual_arith>(addr_of<ptr<ptr<ptr<void>>>>(%[[VALUE_p]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
