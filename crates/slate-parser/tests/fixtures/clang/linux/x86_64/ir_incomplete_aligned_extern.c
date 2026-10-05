// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir -std=gnu11

struct rq;

extern struct rq runqueues __attribute__((aligned(64)));
extern _Alignas(32) struct rq idle;
extern int table[] __attribute__((aligned(16)));

typedef struct rq aligned_rq __attribute__((aligned(128)));
extern aligned_rq typed;
extern aligned_rq lowered __attribute__((aligned(8)));

struct rq *current(void) { return &runqueues; }
struct rq *idle_queue(void) { return &idle; }
aligned_rq *typed_queue(void) { return &typed; }
aligned_rq *lowered_queue(void) { return &lowered; }
int *first(void) { return table; }

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
// IR-NEXT:     type @type[[TYPE_rq:[0-9]+]] rq = struct incomplete;
// IR-NEXT:     type @type[[TYPE_aligned_rq:[0-9]+]] aligned_rq = @type[[TYPE_rq]];
// IR-NEXT:     extern %[[VALUE_runqueues:[0-9]+]] runqueues: @type[[TYPE_rq]] [storage=static] [align=64] [linkage=external];
// IR-NEXT:     extern %[[VALUE_idle:[0-9]+]] idle: @type[[TYPE_rq]] [storage=static] [align=32] [linkage=external];
// IR-NEXT:     extern %[[VALUE_table:[0-9]+]] table: array<i32, incomplete> [storage=static] [align=16] [linkage=external];
// IR-NEXT:     extern %[[VALUE_typed:[0-9]+]] typed: @type[[TYPE_rq]] [storage=static] [linkage=external];
// IR-NEXT:     extern %[[VALUE_lowered:[0-9]+]] lowered: @type[[TYPE_rq]] [storage=static] [align=8] [linkage=external];
// IR-NEXT:     fn %[[VALUE_current:[0-9]+]] @current() -> ptr<@type[[TYPE_rq]]> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return addr_of<ptr<@type[[TYPE_rq]]>>(%[[VALUE_runqueues]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_idle_queue:[0-9]+]] @idle_queue() -> ptr<@type[[TYPE_rq]]> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return addr_of<ptr<@type[[TYPE_rq]]>>(%[[VALUE_idle]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_typed_queue:[0-9]+]] @typed_queue() -> ptr<@type[[TYPE_rq]]> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return addr_of<ptr<@type[[TYPE_rq]]>>(%[[VALUE_typed]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_lowered_queue:[0-9]+]] @lowered_queue() -> ptr<@type[[TYPE_rq]]> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return addr_of<ptr<@type[[TYPE_rq]]>>(%[[VALUE_lowered]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_first:[0-9]+]] @first() -> ptr<i32> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return array_decay<ptr<i32>, length=None>(%[[VALUE_table]]);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
