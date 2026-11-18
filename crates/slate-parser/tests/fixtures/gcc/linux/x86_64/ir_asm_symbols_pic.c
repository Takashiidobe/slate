// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS -fPIC --dump-ir --compact-ir
// SLATE-FILECHECK-STD IR gnu23
int g;
static int local;
__attribute__((visibility("protected"))) int protected_g;

void pic_symbols(void) {
    asm("# %0" : : "i"(&g));
    asm("# %0" : : "i"(&local));
    asm("# %0" : : "i"(&protected_g));
    asm("# %0" : : "s"(&g));
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
// IR-NEXT:     machine [pic=2];
// IR-NEXT:     global %[[VALUE_g:[0-9]+]] g: i32 [storage=static] [linkage=external];
// IR-NEXT:     global %[[VALUE_local:[0-9]+]] local: i32 [storage=static] [linkage=internal];
// IR-NEXT:     global %[[VALUE_protected_g:[0-9]+]] protected_g: i32 [storage=static] [linkage=external] [visibility=protected];
// IR-NEXT:     fn %[[VALUE_pic_symbols:[0-9]+]] @pic_symbols() -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         asm "# %0" [dialect=att] [options=nomem,nostack] [alternative=none] {
// IR-NEXT:             template: "# " %0;
// IR-NEXT:             in 0 "i" [imm | sym] width 64 addr_of<ptr<i32>>(%[[VALUE_g]]);
// IR-NEXT:             rejected: 0 (operand 0: not-constant);
// IR-NEXT:         }
// IR-NEXT:         asm "# %0" [dialect=att] [options=nomem,nostack] {
// IR-NEXT:             template: "# " %0;
// IR-NEXT:             in 0 "i" [imm | sym] -> sym width 64 sym<offset=0>(%[[VALUE_local]]);
// IR-NEXT:         }
// IR-NEXT:         asm "# %0" [dialect=att] [options=nomem,nostack] [alternative=none] {
// IR-NEXT:             template: "# " %0;
// IR-NEXT:             in 0 "i" [imm | sym] width 64 addr_of<ptr<i32>>(%[[VALUE_protected_g]]);
// IR-NEXT:             rejected: 0 (operand 0: not-constant);
// IR-NEXT:         }
// IR-NEXT:         asm "# %0" [dialect=att] [options=nomem,nostack] [alternative=none] {
// IR-NEXT:             template: "# " %0;
// IR-NEXT:             in 0 "s" [sym] width 64 addr_of<ptr<i32>>(%[[VALUE_g]]);
// IR-NEXT:             rejected: 0 (operand 0: not-constant);
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
