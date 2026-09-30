// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
typedef int T;
typedef struct S { int v; } S;
static void px(T p) {}
static void (*gp(int T))(T x) { return px; }
static int sum(S *s, int n) { return s->v + n; }
static int (*gs(S s))(S *, int) { (void)s; return sum; }
static void (*(*nested(void))(void (*)(T)))(T) { return 0; }
static int (*arr(void))[sizeof(T)] { static int a[sizeof(T)]; return &a; }
int main(void) { gp(1)(2); return gs((S){1})(0, 0) + (arr() != 0) + (nested() != 0); }

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
// IR-NEXT:     type @type[[TYPE_T:[0-9]+]] T = i32;
// IR-NEXT:     type @type[[TYPE_S:[0-9]+]] S = struct {
// IR-NEXT:         field0 v: i32;
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     type @type[[TYPE_S_2:[0-9]+]] S = @type[[TYPE_S]];
// IR-NEXT:     global %[[VALUE_a:[0-9]+]] a: array<i32, 4> [storage=static] [align=16] [linkage=internal];
// IR-NEXT:     fn %[[VALUE_px:[0-9]+]] @px(%[[VALUE_p:[0-9]+]] p: i32) -> void [linkage=internal] [fallthrough=ret_void] {
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_gp:[0-9]+]] @gp(%[[VALUE_T:[0-9]+]] T: i32) -> ptr<fn(i32) -> void> [linkage=internal] [fallthrough=ub_if_used] {
// IR-NEXT:         return function_decay<ptr<fn(i32) -> void>>(%[[VALUE_px]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_sum:[0-9]+]] @sum(%[[VALUE_s:[0-9]+]] s: ptr<@type[[TYPE_S]]>, %[[VALUE_n:[0-9]+]] n: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<i32>(read<i32>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_s]])))), read<i32>(%[[VALUE_n]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_gs:[0-9]+]] @gs(%[[VALUE_s_2:[0-9]+]] s: @type[[TYPE_S]]) -> ptr<fn(ptr<@type[[TYPE_S]]>, i32) -> i32> [linkage=internal] [abi=sysv64(native_c) -> scalar] [fallthrough=ub_if_used] {
// IR-NEXT:         read<@type[[TYPE_S]]>(%[[VALUE_s_2]]);
// IR-NEXT:         return function_decay<ptr<fn(ptr<@type[[TYPE_S]]>, i32) -> i32>>(%[[VALUE_sum]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_nested:[0-9]+]] @nested() -> ptr<fn(ptr<fn(i32) -> void>) -> ptr<fn(i32) -> void>> [linkage=internal] [fallthrough=ub_if_used] {
// IR-NEXT:         return null<ptr<fn(ptr<fn(i32) -> void>) -> ptr<fn(i32) -> void>>>;
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_arr:[0-9]+]] @arr() -> ptr<array<i32, 4>> [linkage=internal] [fallthrough=ub_if_used] {
// IR-NEXT:         return addr_of<ptr<array<i32, 4>>>(%[[VALUE_a]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// IR-NEXT:         call<void>(call<ptr<fn(i32) -> void>>(%[[VALUE_gp]], const<i32>(1)), const<i32>(2));
// IR-NEXT:         return add<i32>(add<i32>(call<i32>(call<ptr<fn(ptr<@type[[TYPE_S]]>, i32) -> i32>, abi=sysv64(native_c) -> scalar>(%[[VALUE_gs]], copy<@type[[TYPE_S]], reason=arg>(read<@type[[TYPE_S]]>(compound_literal %[[VALUE0:[0-9]+]] [storage=automatic] = aggregate<@type[[TYPE_S]], zero_fill=false>(field0 = const<i32>(1))))), null<ptr<@type[[TYPE_S]]>>, const<i32>(0)), from_bool<i32>(ne<ptr<array<i32, 4>>>(call<ptr<array<i32, 4>>>(%[[VALUE_arr]]), null<ptr<array<i32, 4>>>))), from_bool<i32>(ne<ptr<fn(ptr<fn(i32) -> void>) -> ptr<fn(i32) -> void>>>(call<ptr<fn(ptr<fn(i32) -> void>) -> ptr<fn(i32) -> void>>>(%[[VALUE_nested]]), null<ptr<fn(ptr<fn(i32) -> void>) -> ptr<fn(i32) -> void>>>)));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
