// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-STD IR gnu23
int g;
static int local;
void fn(void);

void pie_symbols(void) {
    asm("# %0" : : "i"(&g));
    asm("# %0" : : "i"(&local));
    asm("# %0" : : "i"(fn));
    asm("# %0" : : "i"("abc"));
    asm("# %0" : : "s"(&g));
    asm("# %0" : : "ri"(&g));
    asm("# %0" : : "g"(&g));
}

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "i686-unknown-linux-gnu" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=4, align=4];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f80;
// IR-NEXT:         storage bool [size=1, align=1];
// IR-NEXT:         storage i8, u8 [size=1, align=1];
// IR-NEXT:         storage i16, u16 [size=2, align=2];
// IR-NEXT:         storage i32, u32 [size=4, align=4];
// IR-NEXT:         storage i64, u64 [size=8, align=4];
// IR-NEXT:         storage i128, u128 [size=16, align=16];
// IR-NEXT:         storage bf16 [size=2, align=2];
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=4];
// IR-NEXT:         storage f80 [size=12, align=4];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     global %[[VALUE_g:[0-9]+]] g: i32 [storage=static] [linkage=external];
// IR-NEXT:     global %[[VALUE_local:[0-9]+]] local: i32 [storage=static] [linkage=internal];
// IR-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([97, 98, 99, 0]) [linkage=internal];
// IR-NEXT:     fn %[[VALUE_fn:[0-9]+]] @fn() -> void [linkage=external];
// IR-NEXT:     fn %[[VALUE_pie_symbols:[0-9]+]] @pie_symbols() -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         asm "# %0" [dialect=att] [options=nostack] [alternative=none] {
// IR-NEXT:             template: "# " %0;
// IR-NEXT:             in 0 "i" [imm | sym] width 32 addr_of<ptr<i32>>(%[[VALUE_g]]);
// IR-NEXT:             rejected: 0 (operand 0: not-constant);
// IR-NEXT:         }
// IR-NEXT:         asm "# %0" [dialect=att] [options=nostack] [alternative=none] {
// IR-NEXT:             template: "# " %0;
// IR-NEXT:             in 0 "i" [imm | sym] width 32 addr_of<ptr<i32>>(%[[VALUE_local]]);
// IR-NEXT:             rejected: 0 (operand 0: not-constant);
// IR-NEXT:         }
// IR-NEXT:         asm "# %0" [dialect=att] [options=nostack] [alternative=none] {
// IR-NEXT:             template: "# " %0;
// IR-NEXT:             in 0 "i" [imm | sym] width 32 function_decay<ptr<fn() -> void>>(%[[VALUE_fn]]);
// IR-NEXT:             rejected: 0 (operand 0: not-constant);
// IR-NEXT:         }
// IR-NEXT:         asm "# %0" [dialect=att] [options=nostack] [alternative=none] {
// IR-NEXT:             template: "# " %0;
// IR-NEXT:             in 0 "i" [imm | sym] width 32 array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str]]);
// IR-NEXT:             rejected: 0 (operand 0: not-constant);
// IR-NEXT:         }
// IR-NEXT:         asm "# %0" [dialect=att] [options=nostack] {
// IR-NEXT:             template: "# " %0;
// IR-NEXT:             in 0 "s" [sym] width 32 sym<offset=0>(%[[VALUE_g]]);
// IR-NEXT:         }
// IR-NEXT:         asm "# %0" [dialect=att] [options=nostack] {
// IR-NEXT:             template: "# " %0;
// IR-NEXT:             in 0 "ri" [reg | imm | sym] -> reg width 32 addr_of<ptr<i32>>(%[[VALUE_g]]);
// IR-NEXT:         }
// IR-NEXT:         asm "# %0" [dialect=att] [options=nostack] {
// IR-NEXT:             template: "# " %0;
// IR-NEXT:             in 0 "g" [reg | mem | imm | sym] -> reg width 32 addr_of<ptr<i32>>(%[[VALUE_g]]);
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
