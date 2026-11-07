typedef _Complex float cf128 __attribute__((__mode__(__TC__)));
typedef _Complex float cf80 __attribute__((mode(XC)));
typedef _Complex double cf64 __attribute__((mode(DC)));
typedef _Complex double cf32 __attribute__((mode(SC)));

int sizes[] = {sizeof(cf128), sizeof(cf80), sizeof(cf64), sizeof(cf32)};

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
// DEFAULT-NEXT:     type @type[[TYPE_cf128:[0-9]+]] cf128 = complex<f128>;
// DEFAULT-NEXT:     type @type[[TYPE_cf80:[0-9]+]] cf80 = complex<f80>;
// DEFAULT-NEXT:     type @type[[TYPE_cf64:[0-9]+]] cf64 = complex<f64>;
// DEFAULT-NEXT:     type @type[[TYPE_cf32:[0-9]+]] cf32 = complex<f32>;
// DEFAULT-NEXT:     global %[[VALUE_sizes:[0-9]+]] sizes: array<i32, 4> [storage=static] [align=16] = aggregate<array<i32, 4>, zero_fill=false>(index0 = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(size_of<u64, complex<f128>>(32) [c="__size_t"])), index1 = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(size_of<u64, complex<f80>>(32) [c="__size_t"])), index2 = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(size_of<u64, complex<f64>>(16) [c="__size_t"])), index3 = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(size_of<u64, complex<f32>>(8) [c="__size_t"]))) [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
