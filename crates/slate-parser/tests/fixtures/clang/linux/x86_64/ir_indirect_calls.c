// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir

typedef int (*Op)(int, int);
struct Vtbl { Op op; };

int add(int a, int b) { return a + b; }

Op designator(void) { return add; }
Op addressed(void) { return &add; }
int through_parameter(Op op, int a, int b) { return op(a, b); }
int through_deref(Op op) { return (*op)(1, 2); }
int through_local(void) { Op op = add; return op(3, 4); }
int through_field(struct Vtbl *v) { return v->op(5, 6); }
int through_table(Op *ops, int i) { return ops[i](7, 8); }
int through_conditional(Op a, Op b, int c) { return (c ? a : b)(9, 10); }
int through_variadic(int (*fmt)(const char *, ...)) { return fmt("x", 1); }
int as_direct(void) { return (*add)(11, 12); }
int compared(Op op) { return op == add && add != 0; }
int higher_order(int a) { return through_parameter(add, a, a); }
int sequenced(Op *ops, int i) { return ops[i++](i, i); }
int chained(Op (*pick)(void)) { return pick()(13, 14); }
int guarded(Op op, int n) { return op ? op(n, n) : 0; }

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
// IR-NEXT:     type @type[[TYPE_Op:[0-9]+]] Op = ptr<fn(i32, i32) -> i32>;
// IR-NEXT:     type @type[[TYPE_Vtbl:[0-9]+]] Vtbl = struct {
// IR-NEXT:         field0 op: ptr<fn(i32, i32) -> i32>;
// IR-NEXT:     } [size=8, align=8, offsets=[0]];
// IR-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([120, 0]) [linkage=internal];
// IR-NEXT:     fn %[[VALUE_add:[0-9]+]] @add(%[[VALUE_a:[0-9]+]] a: i32, %[[VALUE_b:[0-9]+]] b: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<i32, overflow=ub>(read<i32>(%[[VALUE_a]]), read<i32>(%[[VALUE_b]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_designator:[0-9]+]] @designator() -> ptr<fn(i32, i32) -> i32> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return function_decay<ptr<fn(i32, i32) -> i32>>(%[[VALUE_add]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_addressed:[0-9]+]] @addressed() -> ptr<fn(i32, i32) -> i32> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return addr_of<ptr<fn(i32, i32) -> i32>>(%[[VALUE_add]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_through_parameter:[0-9]+]] @through_parameter(%[[VALUE_op:[0-9]+]] op: ptr<fn(i32, i32) -> i32>, %[[VALUE_a_2:[0-9]+]] a: i32, %[[VALUE_b_2:[0-9]+]] b: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return call<i32, signature=fn(i32, i32) -> i32>(read<ptr<fn(i32, i32) -> i32>>(%[[VALUE_op]]), read<i32>(%[[VALUE_a_2]]), read<i32>(%[[VALUE_b_2]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_through_deref:[0-9]+]] @through_deref(%[[VALUE_op_2:[0-9]+]] op: ptr<fn(i32, i32) -> i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return call<i32, signature=fn(i32, i32) -> i32>(read<ptr<fn(i32, i32) -> i32>>(%[[VALUE_op_2]]), const<i32>(1), const<i32>(2));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_through_local:[0-9]+]] @through_local() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_op_3:[0-9]+]] op: ptr<fn(i32, i32) -> i32> [storage=automatic] = function_decay<ptr<fn(i32, i32) -> i32>>(%[[VALUE_add]]);
// IR-NEXT:         return call<i32, signature=fn(i32, i32) -> i32>(read<ptr<fn(i32, i32) -> i32>>(%[[VALUE_op_3]]), const<i32>(3), const<i32>(4));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_through_field:[0-9]+]] @through_field(%[[VALUE_v:[0-9]+]] v: ptr<@type[[TYPE_Vtbl]]>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return call<i32, signature=fn(i32, i32) -> i32>(read<ptr<fn(i32, i32) -> i32>>(field0(deref(read<ptr<@type[[TYPE_Vtbl]]>>(%[[VALUE_v]])))), const<i32>(5), const<i32>(6));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_through_table:[0-9]+]] @through_table(%[[VALUE_ops:[0-9]+]] ops: ptr<ptr<fn(i32, i32) -> i32>>, %[[VALUE_i:[0-9]+]] i: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return call<i32, signature=fn(i32, i32) -> i32>(read<ptr<fn(i32, i32) -> i32>>(deref(ptr_offset<ptr<ptr<fn(i32, i32) -> i32>>, subtract=false, element=ptr<fn(i32, i32) -> i32>, overflow=ub>(read<ptr<ptr<fn(i32, i32) -> i32>>>(%[[VALUE_ops]]), read<i32>(%[[VALUE_i]])))), const<i32>(7), const<i32>(8));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_through_conditional:[0-9]+]] @through_conditional(%[[VALUE_a_3:[0-9]+]] a: ptr<fn(i32, i32) -> i32>, %[[VALUE_b_3:[0-9]+]] b: ptr<fn(i32, i32) -> i32>, %[[VALUE_c:[0-9]+]] c: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return call<i32, signature=fn(i32, i32) -> i32>(conditional<ptr<fn(i32, i32) -> i32>>(ne<i32>(read<i32>(%[[VALUE_c]]), const<i32>(0)), read<ptr<fn(i32, i32) -> i32>>(%[[VALUE_a_3]]), read<ptr<fn(i32, i32) -> i32>>(%[[VALUE_b_3]])), const<i32>(9), const<i32>(10));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_through_variadic:[0-9]+]] @through_variadic(%[[VALUE_fmt:[0-9]+]] fmt: ptr<fn(ptr<const i8>, ...) -> i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return call<i32, signature=fn(ptr<const i8>, ...) -> i32>(read<ptr<fn(ptr<const i8>, ...) -> i32>>(%[[VALUE_fmt]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str]])), const<i32>(1));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_as_direct:[0-9]+]] @as_direct() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return call<i32, signature=fn(i32, i32) -> i32>(%[[VALUE_add]], const<i32>(11), const<i32>(12));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_compared:[0-9]+]] @compared(%[[VALUE_op_4:[0-9]+]] op: ptr<fn(i32, i32) -> i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(eq<ptr<fn(i32, i32) -> i32>>(read<ptr<fn(i32, i32) -> i32>>(%[[VALUE_op_4]]), function_decay<ptr<fn(i32, i32) -> i32>>(%[[VALUE_add]])), ne<ptr<fn(i32, i32) -> i32>>(function_decay<ptr<fn(i32, i32) -> i32>>(%[[VALUE_add]]), null<ptr<fn(i32, i32) -> i32>>)));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_higher_order:[0-9]+]] @higher_order(%[[VALUE_a_4:[0-9]+]] a: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return call<i32, signature=fn(ptr<fn(i32, i32) -> i32>, i32, i32) -> i32>(%[[VALUE_through_parameter]], function_decay<ptr<fn(i32, i32) -> i32>>(%[[VALUE_add]]), read<i32>(%[[VALUE_a_4]]), read<i32>(%[[VALUE_a_4]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_sequenced:[0-9]+]] @sequenced(%[[VALUE_ops_2:[0-9]+]] ops: ptr<ptr<fn(i32, i32) -> i32>>, %[[VALUE_i_2:[0-9]+]] i: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE0:[0-9]+]]: i32 [synthetic, unsequenced] = read<i32>(%[[VALUE_i_2]]);
// IR-NEXT:         let %[[VALUE1:[0-9]+]]: i32 [synthetic, unsequenced] = add<i32, overflow=ub>(read<i32>(%[[VALUE0]]), const<i32>(1));
// IR-NEXT:         write<i32, unsequenced>(%[[VALUE_i_2]], read<i32>(%[[VALUE1]]));
// IR-NEXT:         return call<i32, signature=fn(i32, i32) -> i32>(read<ptr<fn(i32, i32) -> i32>>(deref(ptr_offset<ptr<ptr<fn(i32, i32) -> i32>>, subtract=false, element=ptr<fn(i32, i32) -> i32>, overflow=ub>(read<ptr<ptr<fn(i32, i32) -> i32>>>(%[[VALUE_ops_2]]), read<i32>(%[[VALUE0]])))), read<i32>(%[[VALUE_i_2]]), read<i32>(%[[VALUE_i_2]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_chained:[0-9]+]] @chained(%[[VALUE_pick:[0-9]+]] pick: ptr<fn() -> ptr<fn(i32, i32) -> i32>>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return call<i32, signature=fn(i32, i32) -> i32>(call<ptr<fn(i32, i32) -> i32>, signature=fn() -> ptr<fn(i32, i32) -> i32>>(read<ptr<fn() -> ptr<fn(i32, i32) -> i32>>>(%[[VALUE_pick]])), const<i32>(13), const<i32>(14));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_guarded:[0-9]+]] @guarded(%[[VALUE_op_5:[0-9]+]] op: ptr<fn(i32, i32) -> i32>, %[[VALUE_n:[0-9]+]] n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE2:[0-9]+]]: i32 [synthetic];
// IR-NEXT:         if ne<ptr<fn(i32, i32) -> i32>>(read<ptr<fn(i32, i32) -> i32>>(%[[VALUE_op_5]]), null<ptr<fn(i32, i32) -> i32>>)
// IR-NEXT:             write<i32>(%[[VALUE2]], call<i32, signature=fn(i32, i32) -> i32>(read<ptr<fn(i32, i32) -> i32>>(%[[VALUE_op_5]]), read<i32>(%[[VALUE_n]]), read<i32>(%[[VALUE_n]])));
// IR-NEXT:         else
// IR-NEXT:             write<i32>(%[[VALUE2]], const<i32>(0));
// IR-NEXT:         return read<i32>(%[[VALUE2]]);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
