// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir
int va_list_size = sizeof(__builtin_va_list);
int va_list_align = _Alignof(__builtin_va_list);

int sum(int count, ...) {
    __builtin_va_list ap;
    __builtin_va_start(ap, count);
    int total = 0;
    for (int i = 0; i < count; i++)
        total += __builtin_va_arg(ap, int);
    __builtin_va_end(ap);
    return total;
}

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "aarch64-pc-windows-msvc" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=8, align=8];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f64;
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
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     global %0 va_list_size: i32 [storage=static] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(8))) [linkage=external];
// IR-NEXT:     global %1 va_list_align: i32 [storage=static] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(8))) [linkage=external];
// IR-NEXT:     fn %2 @sum(%3 count: i32, ...) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %4 ap: ptr<i8> [storage=automatic];
// IR-NEXT:         va_start(%4);
// IR-NEXT:         let %5 total: i32 [storage=automatic] = const<i32>(0);
// IR-NEXT:         for %7
// IR-NEXT:             init:
// IR-NEXT:                 let %6 i: i32 [storage=automatic] = const<i32>(0);
// IR-NEXT:             condition: lt<i32>(read<i32>(%6), read<i32>(%3))
// IR-NEXT:             increment: {
// IR-NEXT:                 let %8: i32 [synthetic] = read<i32>(%6);
// IR-NEXT:                 let %9: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%8), const<i32>(1));
// IR-NEXT:                 write<i32>(%6, read<i32>(%9));
// IR-NEXT:                 yield void;
// IR-NEXT:             }
// IR-NEXT:             body:
// IR-NEXT:                 let %10: i32 [synthetic] = read<i32>(%5);
// IR-NEXT:                 let %11: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%10), va_arg<i32>(%4));
// IR-NEXT:                 write<i32>(%5, read<i32>(%11));
// IR-NEXT:         va_end(%4);
// IR-NEXT:         return read<i32>(%5);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
