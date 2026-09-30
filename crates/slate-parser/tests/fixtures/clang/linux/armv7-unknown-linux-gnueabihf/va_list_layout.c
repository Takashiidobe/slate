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
// IR-NEXT:     target "armv7-unknown-linux-gnueabihf" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=4, align=4];
// IR-NEXT:         stack_alignment = 8;
// IR-NEXT:         long_double = f64;
// IR-NEXT:         storage bool [size=1, align=1];
// IR-NEXT:         storage i8, u8 [size=1, align=1];
// IR-NEXT:         storage i16, u16 [size=2, align=2];
// IR-NEXT:         storage i32, u32 [size=4, align=4];
// IR-NEXT:         storage i64, u64 [size=8, align=8];
// IR-NEXT:         storage bf16 [size=2, align=2];
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=8];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     global %[[VALUE_va_list_size:[0-9]+]] va_list_size: i32 [storage=static] = reinterpret<i32, reason=assign, fits=always>(const<u32>(4)) [linkage=external];
// IR-NEXT:     global %[[VALUE_va_list_align:[0-9]+]] va_list_align: i32 [storage=static] = reinterpret<i32, reason=assign, fits=always>(const<u32>(4)) [linkage=external];
// IR-NEXT:     fn %[[VALUE_sum:[0-9]+]] @sum(%[[VALUE_count:[0-9]+]] count: i32, ...) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_ap:[0-9]+]] ap: va_list [storage=automatic];
// IR-NEXT:         va_start(%[[VALUE_ap]]);
// IR-NEXT:         let %[[VALUE_total:[0-9]+]] total: i32 [storage=automatic] = const<i32>(0);
// IR-NEXT:         for %[[VALUE0:[0-9]+]]
// IR-NEXT:             init:
// IR-NEXT:                 let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// IR-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), read<i32>(%[[VALUE_count]]))
// IR-NEXT:             increment: {
// IR-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// IR-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// IR-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE2]]));
// IR-NEXT:                 yield void;
// IR-NEXT:             }
// IR-NEXT:             body:
// IR-NEXT:                 let %[[VALUE3:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// IR-NEXT:                 let %[[VALUE4:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE3]]), va_arg<i32>(%[[VALUE_ap]]));
// IR-NEXT:                 write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE4]]));
// IR-NEXT:         va_end(%[[VALUE_ap]]);
// IR-NEXT:         return read<i32>(%[[VALUE_total]]);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
