struct field {
    unsigned int mask;
    int shift;
};

unsigned int be = __builtin_bswap32(0x01020304u);
static unsigned short be16 = __builtin_bswap16(0x0102);
int shift = __builtin_ffsll(0xf0ull) - 1;
struct field nested = (struct field){.mask = 0xf0u << __builtin_ctz(16u), .shift = __builtin_clzll(1ull)};

int scoped(unsigned int x) {
    static int counted = __builtin_popcount(0xffu);
    int automatic = __builtin_popcount(0xfu);
    return counted + automatic + __builtin_ctz(x);
}

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
// DEFAULT-NEXT:     type @type[[TYPE_field:[0-9]+]] field = struct {
// DEFAULT-NEXT:         field0 mask: u32;
// DEFAULT-NEXT:         field1 shift: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     global %[[VALUE_be:[0-9]+]] be: u32 [storage=static] = const<u32>(67305985) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_be16:[0-9]+]] be16: u16 [storage=static] = const<u16>(513) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_shift:[0-9]+]] shift: i32 [storage=static] = sub<i32, overflow=ub>(const<i32>(5), const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_nested:[0-9]+]] nested: @type[[TYPE_field]] [storage=static] = copy<@type[[TYPE_field]], reason=assign>(read<@type[[TYPE_field]]>(compound_literal %[[VALUE0:[0-9]+]] [storage=static] = aggregate<@type[[TYPE_field]], zero_fill=false>(field0 = shl<u32, overflow=wrap, amount_out_of_range=ub>(const<u32>(240), const<i32>(4)), field1 = const<i32>(63)))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_counted:[0-9]+]] counted: i32 [storage=static] = const<i32>(8) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_popcount:[0-9]+]] @__builtin_popcount(%[[VALUE1:[0-9]+]] <unnamed>: u32) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_ctz:[0-9]+]] @__builtin_ctz(%[[VALUE2:[0-9]+]] <unnamed>: u32) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_scoped:[0-9]+]] @scoped(%[[VALUE_x:[0-9]+]] x: u32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_automatic:[0-9]+]] automatic: i32 [storage=automatic] = call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_popcount]], const<u32>(15));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(%[[VALUE_counted]]), read<i32>(%[[VALUE_automatic]])), call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_ctz]], read<u32>(%[[VALUE_x]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
