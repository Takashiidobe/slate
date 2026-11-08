// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir
// SLATE-FILECHECK-STD IR gnu23
int global;

void pairs(long long a, double d) {
    asm("@ %0 %Q1 %R1 %H2" : "=&r"(a) : "r"(a), "r"(d));
}

void symbols(void) {
    asm("@ %0 %c0 x%1" : : "i"(&global), "s"(&global));
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
// IR-NEXT:     global %[[VALUE_global:[0-9]+]] global: i32 [storage=static] [linkage=external];
// IR-NEXT:     fn %[[VALUE_pairs:[0-9]+]] @pairs(%[[VALUE_a:[0-9]+]] a: i64, %[[VALUE_d:[0-9]+]] d: f64) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         asm "@ %0 %Q1 %R1 %H2" [options=pure,nomem,nostack,preserves_flags] {
// IR-NEXT:             template: "@ " %0 " " %Q1 " " %R1 " " %H2;
// IR-NEXT:             out 0 "r" [reg_pair] width 64 place<i64>(%[[VALUE_a]]);
// IR-NEXT:             in 1 "r" [reg_pair] width 64 read<i64>(%[[VALUE_a]]);
// IR-NEXT:             in 2 "r" [reg_pair] width 64 read<f64>(%[[VALUE_d]]);
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_symbols:[0-9]+]] @symbols() -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         asm "@ %0 %c0 x%1" [options=nomem,nostack,preserves_flags] {
// IR-NEXT:             template: "@ #" %c0 " " %c0 " x#" %c1;
// IR-NEXT:             in 0 "i" [imm | sym] -> sym width 32 sym<offset=0>(%[[VALUE_global]]);
// IR-NEXT:             in 1 "s" [sym] width 32 sym<offset=0>(%[[VALUE_global]]);
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
