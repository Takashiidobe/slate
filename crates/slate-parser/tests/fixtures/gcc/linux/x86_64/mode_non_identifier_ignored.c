typedef int t __attribute__((mode(1)));
int __attribute__((__mode__(vector_size(8)))) v;
long d __attribute__((mode("DI")));
struct s {
    short f __attribute__((mode((1, 2))));
};
int parameter(int __attribute__((mode(vector_size(8)))) p) { return p; }

int sizes[] = {sizeof(t), sizeof(v), sizeof(d), sizeof(struct s)};

// SLATE-FILECHECK-STD DEFAULT gnu89
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
// DEFAULT-NEXT:     type @type[[TYPE_t:[0-9]+]] t = i32;
// DEFAULT-NEXT:     type @type[[TYPE_s:[0-9]+]] s = struct {
// DEFAULT-NEXT:         field0 f: i16;
// DEFAULT-NEXT:     } [size=2, align=2, offsets=[0]];
// DEFAULT-NEXT:     global %[[VALUE_v:[0-9]+]] v: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_sizes:[0-9]+]] sizes: array<i32, 4> [storage=static] [align=16] = aggregate<array<i32, 4>, zero_fill=false>(index0 = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(size_of<u64, i32>(4) [c="__size_t"])), index1 = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(size_of<u64, i32>(4) [c="__size_t"])), index2 = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(size_of<u64, i64>(8) [c="__size_t"])), index3 = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(size_of<u64, @type[[TYPE_s]]>(2) [c="__size_t"]))) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_parameter:[0-9]+]] @parameter(%[[VALUE_p:[0-9]+]] p: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_p]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
