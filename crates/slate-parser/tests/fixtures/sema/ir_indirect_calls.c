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
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=8];
// IR-NEXT:         storage f80 [size=16, align=16];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     type @type0 Op = ptr<fn(i32, i32) -> i32>;
// IR-NEXT:     type @type1 Vtbl = struct {
// IR-NEXT:         field0 op: ptr<fn(i32, i32) -> i32>;
// IR-NEXT:     } [size=8, align=8, offsets=[0]];
// IR-NEXT:     global %39 .str39: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([120, 0]) [linkage=internal];
// IR-NEXT:     fn %2 @add(%3 a: i32, %4 b: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<i32, overflow=ub>(read<i32>(%3), read<i32>(%4));
// IR-NEXT:     }
// IR-NEXT:     fn %5 @designator() -> ptr<fn(i32, i32) -> i32> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return function_decay<ptr<fn(i32, i32) -> i32>>(%2);
// IR-NEXT:     }
// IR-NEXT:     fn %6 @addressed() -> ptr<fn(i32, i32) -> i32> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return addr_of<ptr<fn(i32, i32) -> i32>>(%2);
// IR-NEXT:     }
// IR-NEXT:     fn %7 @through_parameter(%8 op: ptr<fn(i32, i32) -> i32>, %9 a: i32, %10 b: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return call<i32, signature=fn(i32, i32) -> i32>(read<ptr<fn(i32, i32) -> i32>>(%8), read<i32>(%9), read<i32>(%10));
// IR-NEXT:     }
// IR-NEXT:     fn %11 @through_deref(%12 op: ptr<fn(i32, i32) -> i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return call<i32, signature=fn(i32, i32) -> i32>(read<ptr<fn(i32, i32) -> i32>>(%12), const<i32>(1), const<i32>(2));
// IR-NEXT:     }
// IR-NEXT:     fn %13 @through_local() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %14 op: ptr<fn(i32, i32) -> i32> [storage=automatic] = function_decay<ptr<fn(i32, i32) -> i32>>(%2);
// IR-NEXT:         return call<i32, signature=fn(i32, i32) -> i32>(read<ptr<fn(i32, i32) -> i32>>(%14), const<i32>(3), const<i32>(4));
// IR-NEXT:     }
// IR-NEXT:     fn %15 @through_field(%16 v: ptr<@type1>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return call<i32, signature=fn(i32, i32) -> i32>(read<ptr<fn(i32, i32) -> i32>>(field0(deref(read<ptr<@type1>>(%16)))), const<i32>(5), const<i32>(6));
// IR-NEXT:     }
// IR-NEXT:     fn %17 @through_table(%18 ops: ptr<ptr<fn(i32, i32) -> i32>>, %19 i: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return call<i32, signature=fn(i32, i32) -> i32>(read<ptr<fn(i32, i32) -> i32>>(deref(ptr_offset<ptr<ptr<fn(i32, i32) -> i32>>, subtract=false, element=ptr<fn(i32, i32) -> i32>, overflow=ub>(read<ptr<ptr<fn(i32, i32) -> i32>>>(%18), read<i32>(%19)))), const<i32>(7), const<i32>(8));
// IR-NEXT:     }
// IR-NEXT:     fn %20 @through_conditional(%21 a: ptr<fn(i32, i32) -> i32>, %22 b: ptr<fn(i32, i32) -> i32>, %23 c: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return call<i32, signature=fn(i32, i32) -> i32>(conditional<ptr<fn(i32, i32) -> i32>>(ne<i32>(read<i32>(%23), const<i32>(0)), read<ptr<fn(i32, i32) -> i32>>(%21), read<ptr<fn(i32, i32) -> i32>>(%22)), const<i32>(9), const<i32>(10));
// IR-NEXT:     }
// IR-NEXT:     fn %24 @through_variadic(%25 fmt: ptr<fn(ptr<const i8>, ...) -> i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return call<i32, signature=fn(ptr<const i8>, ...) -> i32>(read<ptr<fn(ptr<const i8>, ...) -> i32>>(%25), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%39)), const<i32>(1));
// IR-NEXT:     }
// IR-NEXT:     fn %26 @as_direct() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return call<i32, signature=fn(i32, i32) -> i32>(%2, const<i32>(11), const<i32>(12));
// IR-NEXT:     }
// IR-NEXT:     fn %27 @compared(%28 op: ptr<fn(i32, i32) -> i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(eq<ptr<fn(i32, i32) -> i32>>(read<ptr<fn(i32, i32) -> i32>>(%28), function_decay<ptr<fn(i32, i32) -> i32>>(%2)), ne<ptr<fn(i32, i32) -> i32>>(function_decay<ptr<fn(i32, i32) -> i32>>(%2), null<ptr<fn(i32, i32) -> i32>>)));
// IR-NEXT:     }
// IR-NEXT:     fn %29 @higher_order(%30 a: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return call<i32, signature=fn(ptr<fn(i32, i32) -> i32>, i32, i32) -> i32>(%7, function_decay<ptr<fn(i32, i32) -> i32>>(%2), read<i32>(%30), read<i32>(%30));
// IR-NEXT:     }
// IR-NEXT:     fn %31 @sequenced(%32 ops: ptr<ptr<fn(i32, i32) -> i32>>, %33 i: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %40: i32 [synthetic, unsequenced] = read<i32>(%33);
// IR-NEXT:         let %41: i32 [synthetic, unsequenced] = add<i32, overflow=ub>(read<i32>(%40), const<i32>(1));
// IR-NEXT:         write<i32, unsequenced>(%33, read<i32>(%41));
// IR-NEXT:         return call<i32, signature=fn(i32, i32) -> i32>(read<ptr<fn(i32, i32) -> i32>>(deref(ptr_offset<ptr<ptr<fn(i32, i32) -> i32>>, subtract=false, element=ptr<fn(i32, i32) -> i32>, overflow=ub>(read<ptr<ptr<fn(i32, i32) -> i32>>>(%32), read<i32>(%40)))), read<i32>(%33), read<i32>(%33));
// IR-NEXT:     }
// IR-NEXT:     fn %34 @chained(%35 pick: ptr<fn() -> ptr<fn(i32, i32) -> i32>>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return call<i32, signature=fn(i32, i32) -> i32>(call<ptr<fn(i32, i32) -> i32>, signature=fn() -> ptr<fn(i32, i32) -> i32>>(read<ptr<fn() -> ptr<fn(i32, i32) -> i32>>>(%35)), const<i32>(13), const<i32>(14));
// IR-NEXT:     }
// IR-NEXT:     fn %36 @guarded(%37 op: ptr<fn(i32, i32) -> i32>, %38 n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %42: i32 [synthetic];
// IR-NEXT:         if ne<ptr<fn(i32, i32) -> i32>>(read<ptr<fn(i32, i32) -> i32>>(%37), null<ptr<fn(i32, i32) -> i32>>)
// IR-NEXT:             write<i32>(%42, call<i32, signature=fn(i32, i32) -> i32>(read<ptr<fn(i32, i32) -> i32>>(%37), read<i32>(%38), read<i32>(%38)));
// IR-NEXT:         else
// IR-NEXT:             write<i32>(%42, const<i32>(0));
// IR-NEXT:         return read<i32>(%42);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
