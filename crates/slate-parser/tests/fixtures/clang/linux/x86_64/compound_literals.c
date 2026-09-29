// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir -std=c23

struct P { int x; int y; };
int *global = (int[]){1, 2, 3};
static_assert(sizeof((int[]){1, 2, 3}) == 12);

int local(int n) {
  struct P p = (struct P){n, 2};
  struct P *q = &(struct P){.y = n};
  int first = (int[]){4, 5}[1];
  return p.x + q->y + first + (int){n};
}

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "x86_64-unknown-linux-gnu" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=8, align=8];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f80;
// IR-NEXT:         storage bool [size=1, align=1];
// IR-NEXT:         storage i8, u8 [size=1, align=1];
// IR-NEXT:         storage i16, u16 [size=2, align=2];
// IR-NEXT:         storage i32, u32 [size=4, align=4];
// IR-NEXT:         storage i64, u64 [size=8, align=8];
// IR-NEXT:         storage i128, u128 [size=16, align=16];
// IR-NEXT:         storage bf16 [size=2, align=2];
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=8];
// IR-NEXT:         storage f80 [size=16, align=16];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     type @type[[TYPE_P:[0-9]+]] P = struct {
// IR-NEXT:         field0 x: i32;
// IR-NEXT:         field1 y: i32;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// IR-NEXT:     global %[[VALUE_global:[0-9]+]] global: ptr<i32> [storage=static] = array_decay<ptr<i32>, length=Some(3)>(compound_literal %[[VALUE0:[0-9]+]] [storage=static] = aggregate<array<i32, 3>, zero_fill=false>(index0 = const<i32>(1), index1 = const<i32>(2), index2 = const<i32>(3))) [linkage=external];
// IR-NEXT:     fn %[[VALUE_local:[0-9]+]] @local(%[[VALUE_n:[0-9]+]] n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_p:[0-9]+]] p: @type[[TYPE_P]] [storage=automatic] = copy<@type[[TYPE_P]], reason=assign>(read<@type[[TYPE_P]]>(compound_literal %[[VALUE1:[0-9]+]] [storage=automatic] = aggregate<@type[[TYPE_P]], zero_fill=false>(field0 = read<i32>(%[[VALUE_n]]), field1 = const<i32>(2))));
// IR-NEXT:         let %[[VALUE_q:[0-9]+]] q: ptr<@type[[TYPE_P]]> [storage=automatic] = addr_of<ptr<@type[[TYPE_P]]>>(compound_literal %[[VALUE2:[0-9]+]] [storage=automatic] = aggregate<@type[[TYPE_P]], zero_fill=true>(field1 = read<i32>(%[[VALUE_n]])));
// IR-NEXT:         let %[[VALUE_first:[0-9]+]] first: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(compound_literal %[[VALUE3:[0-9]+]] [storage=automatic] = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(4), index1 = const<i32>(5))), const<i32>(1))));
// IR-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(field0(%[[VALUE_p]])), read<i32>(field1(deref(read<ptr<@type[[TYPE_P]]>>(%[[VALUE_q]]))))), read<i32>(%[[VALUE_first]])), read<i32>(compound_literal %[[VALUE4:[0-9]+]] [storage=automatic] = read<i32>(%[[VALUE_n]])));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
