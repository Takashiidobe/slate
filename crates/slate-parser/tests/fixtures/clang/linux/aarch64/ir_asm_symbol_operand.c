// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir
// SLATE-FILECHECK-STD IR gnu23

int counter;
int table[4];

int twice(int value) {
    return value * 2;
}

void *symbols(void) {
    void *address;
    __asm__("adrp %0, %1\n\tadd %0, %0, :lo12:%1" : "=r"(address) : "S"(&counter));
    __asm__("adrp %0, %1" : "=r"(address) : "S"(&table[2]));
    __asm__("adr %0, %1" : "=r"(address) : "S"(twice));
    return address;
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
// IR-NEXT:     global %[[VALUE_counter:[0-9]+]] counter: i32 [storage=static] [linkage=external];
// IR-NEXT:     global %[[VALUE_table:[0-9]+]] table: array<i32, 4> [storage=static] [linkage=external];
// IR-NEXT:     fn %[[VALUE_twice:[0-9]+]] @twice(%[[VALUE_value:[0-9]+]] value: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return mul<i32, overflow=ub>(read<i32>(%[[VALUE_value]]), const<i32>(2));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_symbols:[0-9]+]] @symbols() -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_address:[0-9]+]] address: ptr<void> [storage=automatic];
// IR-NEXT:         asm "adrp %0, %1\n\tadd %0, %0, :lo12:%1" [options=pure,nomem,nostack,preserves_flags] {
// IR-NEXT:             template: "adrp " %0 ", " %1 "\n\tadd " %0 ", " %0 ", :lo12:" %1;
// IR-NEXT:             lateout 0 "r" [reg] width 64 place<ptr<void>>(%[[VALUE_address]]);
// IR-NEXT:             in 1 "S" [sym] width 64 sym<offset=0>(%[[VALUE_counter]]);
// IR-NEXT:         }
// IR-NEXT:         asm "adrp %0, %1" [options=pure,nomem,nostack,preserves_flags] {
// IR-NEXT:             template: "adrp " %0 ", " %1;
// IR-NEXT:             lateout 0 "r" [reg] width 64 place<ptr<void>>(%[[VALUE_address]]);
// IR-NEXT:             in 1 "S" [sym] width 64 sym<offset=8>(%[[VALUE_table]]);
// IR-NEXT:         }
// IR-NEXT:         asm "adr %0, %1" [options=pure,nomem,nostack,preserves_flags] {
// IR-NEXT:             template: "adr " %0 ", " %1;
// IR-NEXT:             lateout 0 "r" [reg] width 64 place<ptr<void>>(%[[VALUE_address]]);
// IR-NEXT:             in 1 "S" [sym] width 64 sym<offset=0>(%[[VALUE_twice]]);
// IR-NEXT:         }
// IR-NEXT:         return read<ptr<void>>(%[[VALUE_address]]);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
