// SLATE-FILECHECK-DEFINES DEFAULT
int runtime(void);
unsigned char runtime_bcd(unsigned value);

struct header {
    const char *name;
    unsigned long length;
};

static int selected_then = 1 ? 3 : runtime();
static int selected_else = sizeof(int) == 8 ? runtime() : 7;
static unsigned char bcd = __builtin_constant_p(42u) ? ((42u / 10) << 4) + 42u % 10 : runtime_bcd(42u);
static int nested = 0 ? runtime() : sizeof(long) == 8 ? 5 : runtime();
static long gnu_elvis = 4 ?: runtime();
static const struct header headers[] = {
    {"f", "f" ? sizeof("f") - 1 : 0},
    {(void *)0, (void *)0 ? sizeof((void *)0) - 1 : 0},
};

int runtime_conditional(int x) {
    int automatic = 1 ? 3 : runtime();
    return automatic + (x ? 1 : runtime());
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
// DEFAULT-NEXT:     type @type[[TYPE_header:[0-9]+]] header = struct {
// DEFAULT-NEXT:         field0 name: ptr<const i8>;
// DEFAULT-NEXT:         field1 length: u64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     global %[[VALUE_selected_then:[0-9]+]] selected_then: i32 [storage=static] = const<i32>(3) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_selected_else:[0-9]+]] selected_else: i32 [storage=static] = const<i32>(7) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_bcd:[0-9]+]] bcd: u8 [storage=static] = truncate<u8, reason=assign, fits=unknown>(add<u32, overflow=wrap>(shl<u32, overflow=wrap, amount_out_of_range=ub>(div<u32, by_zero=ub>(const<u32>(42), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(10))), const<i32>(4)), rem<u32, by_zero=ub>(const<u32>(42), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(10))))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_nested:[0-9]+]] nested: i32 [storage=static] = const<i32>(5) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_gnu_elvis:[0-9]+]] gnu_elvis: i64 [storage=static] = widen<i64, reason=assign>(const<i32>(4)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([102, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_headers:[0-9]+]] headers: array<@type[[TYPE_header]], 2> [storage=static] [const] [align=16] = aggregate<array<@type[[TYPE_header]], 2>, zero_fill=false>(index0 = aggregate<@type[[TYPE_header]], zero_fill=false>(field0 = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str]])), field1 = reinterpret<u64, reason=assign>(sub<u64, overflow=wrap>(size_of<u64, array<i8, 2>>(2) [c="__size_t"], reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))) [c="__size_t"]) [c="__size_t"])), index1 = aggregate<@type[[TYPE_header]], zero_fill=false>(field0 = null<ptr<const i8>>, field1 = reinterpret<u64, reason=assign>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))) [c="__size_t"]))) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_runtime:[0-9]+]] @runtime() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_runtime_bcd:[0-9]+]] @runtime_bcd(%[[VALUE_value:[0-9]+]] value: u32) -> u8 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_runtime_conditional:[0-9]+]] @runtime_conditional(%[[VALUE_x:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_automatic:[0-9]+]] automatic: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: i32 [synthetic];
// DEFAULT-NEXT:         if ne<i32>(const<i32>(1), const<i32>(0))
// DEFAULT-NEXT:             write<i32>(%[[VALUE0]], const<i32>(3));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<i32>(%[[VALUE0]], call<i32, signature=fn() -> i32>(%[[VALUE_runtime]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_automatic]], read<i32>(%[[VALUE0]]));
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i32 [synthetic];
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_x]]), const<i32>(0))
// DEFAULT-NEXT:             write<i32>(%[[VALUE1]], const<i32>(1));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<i32>(%[[VALUE1]], call<i32, signature=fn() -> i32>(%[[VALUE_runtime]]));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%[[VALUE_automatic]]), read<i32>(%[[VALUE1]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
