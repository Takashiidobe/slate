// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir

void *target;

int literal_zero(int x) {
    target = &&done;
    if (x)
        goto *0;
done:
    return 0;
}

int folded_zero(int x) {
    target = &&done;
    if (x)
        goto *(1 - 1);
done:
    return 0;
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
// IR-NEXT:     global %[[VALUE_target:[0-9]+]] target: ptr<void> [storage=static] [linkage=external];
// IR-NEXT:     fn %[[VALUE_literal_zero:[0-9]+]] @literal_zero(%[[VALUE_x:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         write<ptr<void>>(%[[VALUE_target]], label_addr<ptr<void>>(%[[VALUE_done:[0-9]+]]));
// IR-NEXT:         if ne<i32>(read<i32>(%[[VALUE_x]]), const<i32>(0))
// IR-NEXT:             goto *null<ptr<void>>;
// IR-NEXT:         label %[[VALUE_done]] done:
// IR-NEXT:             return const<i32>(0);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_folded_zero:[0-9]+]] @folded_zero(%[[VALUE_x_2:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         write<ptr<void>>(%[[VALUE_target]], label_addr<ptr<void>>(%[[VALUE_done_2:[0-9]+]]));
// IR-NEXT:         if ne<i32>(read<i32>(%[[VALUE_x_2]]), const<i32>(0))
// IR-NEXT:             goto *null<ptr<void>>;
// IR-NEXT:         label %[[VALUE_done_2]] done:
// IR-NEXT:             return const<i32>(0);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
