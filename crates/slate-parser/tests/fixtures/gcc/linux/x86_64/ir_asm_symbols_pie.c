// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-STD IR gnu23
extern int declared;
void fn(void);

void pie_symbols(void) {
    asm("# %0" : : "i"(&declared));
    asm("# %0" : : "i"(fn));
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
// IR-NEXT:     extern %[[VALUE_declared:[0-9]+]] declared: i32 [storage=static] [linkage=external];
// IR-NEXT:     fn %[[VALUE_fn:[0-9]+]] @fn() -> void [linkage=external];
// IR-NEXT:     fn %[[VALUE_pie_symbols:[0-9]+]] @pie_symbols() -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         asm "# %0" [dialect=att] [options=nomem,nostack] {
// IR-NEXT:             template: "# " %0;
// IR-NEXT:             in 0 "i" [imm | sym] -> sym width 64 sym<offset=0>(%[[VALUE_declared]]);
// IR-NEXT:         }
// IR-NEXT:         asm "# %0" [dialect=att] [options=nomem,nostack] [alternative=none] {
// IR-NEXT:             template: "# " %0;
// IR-NEXT:             in 0 "i" [imm | sym] width 64 function_decay<ptr<fn() -> void>>(%[[VALUE_fn]]);
// IR-NEXT:             rejected: 0 (operand 0: not-constant);
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
