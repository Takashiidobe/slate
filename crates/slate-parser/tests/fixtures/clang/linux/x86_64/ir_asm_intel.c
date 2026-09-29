// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir -masm=intel
// SLATE-FILECHECK-STD IR c23

void dialects(int x) {
    asm("mov{l|} {%0, %%eax|eax, %0}" : : "r"(x));
    asm("a{b|c|d} {e} f|g} {h|i" : : "r"(x));
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
// IR-NEXT:     fn %[[VALUE_dialects:[0-9]+]] @dialects(%[[VALUE_x:[0-9]+]] x: i32) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         asm "mov{l|} {%0, %%eax|eax, %0}" [dialect=intel] [options=nostack] {
// IR-NEXT:             template: "mov eax, " %0;
// IR-NEXT:             in 0 "r" [reg] width 32 read<i32>(%[[VALUE_x]]);
// IR-NEXT:         }
// IR-NEXT:         asm "a{b|c|d} {e} f|g} {h|i" [dialect=intel] [options=nostack] {
// IR-NEXT:             template: "ac  f|g} i";
// IR-NEXT:             in 0 "r" [reg] width 32 read<i32>(%[[VALUE_x]]);
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
