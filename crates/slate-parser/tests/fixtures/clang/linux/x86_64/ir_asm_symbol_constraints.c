// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir
// SLATE-FILECHECK-STD IR c23
int values[4];
void function(void);
void symbols(void) {
  asm("# %0 %1 %2" : : "s"(&values[2]), "Ws"(function), "Ws"((char *)&values[2] - 4));
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
// IR-NEXT:     global %[[VALUE_values:[0-9]+]] values: array<i32, 4> [storage=static] [align=16] [linkage=external];
// IR-NEXT:     fn %[[VALUE_function:[0-9]+]] @function() -> void [linkage=external];
// IR-NEXT:     fn %[[VALUE_symbols:[0-9]+]] @symbols() -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         asm "# %0 %1 %2" [dialect=att] [options=nostack] {
// IR-NEXT:             template: "# " %0 " " %1 " " %2;
// IR-NEXT:             in 0 "s" [sym] width 64 sym<offset=8>(%[[VALUE_values]]);
// IR-NEXT:             in 1 "Ws" [sym] width 64 sym<offset=0>(%[[VALUE_function]]);
// IR-NEXT:             in 2 "Ws" [sym] width 64 sym<offset=4>(%[[VALUE_values]]);
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
