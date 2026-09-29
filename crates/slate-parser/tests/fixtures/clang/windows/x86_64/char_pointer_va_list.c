// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir
typedef char *va_list;

_Static_assert(_Generic((__builtin_va_list *)0, va_list *: 1, default: 0), "va_list is char *");

int first_vararg(int count, ...) {
    va_list ap;
    __builtin_va_start(ap, count);
    int value = __builtin_va_arg(ap, int);
    __builtin_va_end(ap);
    ap = (va_list)0;
    return value + count;
}

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "x86_64-pc-windows-msvc" {
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
// IR-NEXT:     type @type[[TYPE_va_list:[0-9]+]] va_list = ptr<i8>;
// IR-NEXT:     fn %[[VALUE_first_vararg:[0-9]+]] @first_vararg(%[[VALUE_count:[0-9]+]] count: i32, ...) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_ap:[0-9]+]] ap: ptr<i8> [storage=automatic];
// IR-NEXT:         va_start(%[[VALUE_ap]]);
// IR-NEXT:         let %[[VALUE_value:[0-9]+]] value: i32 [storage=automatic] = va_arg<i32>(%[[VALUE_ap]]);
// IR-NEXT:         va_end(%[[VALUE_ap]]);
// IR-NEXT:         write<ptr<i8>>(%[[VALUE_ap]], null<ptr<i8>>);
// IR-NEXT:         return add<i32, overflow=ub>(read<i32>(%[[VALUE_value]]), read<i32>(%[[VALUE_count]]));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
