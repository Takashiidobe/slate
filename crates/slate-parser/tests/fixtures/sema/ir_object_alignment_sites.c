// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir

int file_aligned __attribute__((aligned(16)));
_Alignas(32) int file_alignas;
int file_below __attribute__((aligned(1)));

int alignof_file_aligned = __alignof__(file_aligned);
int alignof_file_alignas = __alignof__(file_alignas);
int alignof_file_below = __alignof__(file_below);

int reader(void) {
    int block_aligned __attribute__((aligned(16)));
    _Alignas(32) int block_alignas;
    int block_below __attribute__((aligned(1)));
    static _Alignas(64) int static_local;
    int plain;
    return __alignof__(block_aligned) + __alignof__(block_alignas)
         + __alignof__(block_below) + __alignof__(static_local)
         + __alignof__(plain);
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
// IR-NEXT:     global %0 file_aligned: i32 [storage=static] [align=16] [linkage=external];
// IR-NEXT:     global %1 file_alignas: i32 [storage=static] [align=32] [linkage=external];
// IR-NEXT:     global %2 file_below: i32 [storage=static] [align=1] [linkage=external];
// IR-NEXT:     global %3 alignof_file_aligned: i32 [storage=static] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(16))) [linkage=external];
// IR-NEXT:     global %4 alignof_file_alignas: i32 [storage=static] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(32))) [linkage=external];
// IR-NEXT:     global %5 alignof_file_below: i32 [storage=static] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(1))) [linkage=external];
// IR-NEXT:     global %10 static_local: i32 [storage=static] [align=64] [linkage=internal];
// IR-NEXT:     fn %6 @reader() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %7 block_aligned: i32 [storage=automatic] [align=16];
// IR-NEXT:         let %8 block_alignas: i32 [storage=automatic] [align=32];
// IR-NEXT:         let %9 block_below: i32 [storage=automatic] [align=1];
// IR-NEXT:         let %11 plain: i32 [storage=automatic];
// IR-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=unknown>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(const<u64>(16), const<u64>(32)), const<u64>(1)), const<u64>(64)), const<u64>(4))));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
