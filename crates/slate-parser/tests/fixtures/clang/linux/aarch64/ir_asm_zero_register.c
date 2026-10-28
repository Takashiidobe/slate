// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir
// SLATE-FILECHECK-STD IR gnu23

long cell;

void store(long value) {
    __asm__ volatile("str %x1, %0" : "=m"(cell) : "rZ"(0L));
    __asm__ volatile("str %x1, %0" : "=m"(cell) : "rZ"(11L));
    __asm__ volatile("str %x1, %0" : "=m"(cell) : "rZ"(value));
    __asm__ volatile("add %0, %0, %1" : "+r"(cell) : "rI"(11L));
}

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "aarch64-unknown-linux-gnu" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=8, align=8];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f128;
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
// IR-NEXT:     global %[[VALUE_cell:[0-9]+]] cell: i64 [storage=static] [linkage=external];
// IR-NEXT:     fn %[[VALUE_store:[0-9]+]] @store(%[[VALUE_value:[0-9]+]] value: i64) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         asm volatile "str %x1, %0" [options=nostack,preserves_flags] {
// IR-NEXT:             template: "str " %x1(64) ", " %0;
// IR-NEXT:             lateout 0 "m" [mem] width 64 place<i64>(%[[VALUE_cell]]);
// IR-NEXT:             in 1 "rZ" [reg | imm] -> imm width 64 const<i64>(0);
// IR-NEXT:         }
// IR-NEXT:         asm volatile "str %x1, %0" [options=nostack,preserves_flags] {
// IR-NEXT:             template: "str " %x1(64) ", " %0;
// IR-NEXT:             lateout 0 "m" [mem] width 64 place<i64>(%[[VALUE_cell]]);
// IR-NEXT:             in 1 "rZ" [reg | imm] -> reg width 64 const<i64>(11);
// IR-NEXT:         }
// IR-NEXT:         asm volatile "str %x1, %0" [options=nostack,preserves_flags] {
// IR-NEXT:             template: "str " %x1(64) ", " %0;
// IR-NEXT:             lateout 0 "m" [mem] width 64 place<i64>(%[[VALUE_cell]]);
// IR-NEXT:             in 1 "rZ" [reg | imm] -> reg width 64 read<i64>(%[[VALUE_value]]);
// IR-NEXT:         }
// IR-NEXT:         asm volatile "add %0, %0, %1" [options=nostack,preserves_flags] {
// IR-NEXT:             template: "add " %0 ", " %0 ", " %1;
// IR-NEXT:             inlateout 0 "r" [reg] width 64 place<i64>(%[[VALUE_cell]]);
// IR-NEXT:             in 1 "rI" [reg | imm] -> imm width 64 const<i64>(11);
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
