// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir

int indirect(int x) {
    void *target = x ? &&yes : &&no;
    goto *target;
yes: return 1;
no: return 0;
}
void direct_address(void) {
    __label__ done;
    goto *&&done;
done: return;
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
// IR-NEXT:     fn %[[VALUE_indirect:[0-9]+]] @indirect(%[[VALUE_x:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_target:[0-9]+]] target: ptr<void> [storage=automatic] = conditional<ptr<void>>(ne<i32>(read<i32>(%[[VALUE_x]]), const<i32>(0)), label_addr<ptr<void>>(%[[VALUE_yes:[0-9]+]]), label_addr<ptr<void>>(%[[VALUE_no:[0-9]+]]));
// IR-NEXT:         goto *read<ptr<void>>(%[[VALUE_target]]);
// IR-NEXT:         label %[[VALUE_yes]] yes:
// IR-NEXT:             return const<i32>(1);
// IR-NEXT:         label %[[VALUE_no]] no:
// IR-NEXT:             return const<i32>(0);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_direct_address:[0-9]+]] @direct_address() -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         goto *label_addr<ptr<void>>(%[[VALUE_done:[0-9]+]]);
// IR-NEXT:         label %[[VALUE_done]] done:
// IR-NEXT:             return;
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
