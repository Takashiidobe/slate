// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir
// SLATE-FILECHECK-STD IR gnu23
void general(void) {
    asm volatile("" ::: "x0", "w1", "r9", "x30", "lr", "fp", "sp");
}

void vector(void) {
    asm volatile("" ::: "v2", "d3", "s4");
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
// IR-NEXT:     fn %[[VALUE_general:[0-9]+]] @general() -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         asm volatile "" [options=nostack,preserves_flags] {
// IR-NEXT:             clobbers: "x0" as x0, "w1" as x1, "r9" as x9, "x30" as x30, "lr" as x30, "fp" as x29, "sp" as sp;
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_vector:[0-9]+]] @vector() -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         asm volatile "" [options=nostack,preserves_flags] {
// IR-NEXT:             clobbers: "v2" as v2, "d3" as v3, "s4" as v4;
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
