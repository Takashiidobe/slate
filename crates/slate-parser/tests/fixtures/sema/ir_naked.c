// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir
// SLATE-FILECHECK-STD IR c23
__attribute__((naked)) int identity(int x) {
    asm("mov %edi, %eax\n\tret");
}

__attribute__((naked)) void two(void) {
    asm("nop");
    asm("ret");
}

__attribute__((naked)) void constant(void) {
    asm volatile("mov %0, %%eax\n\tret" : : "i"(42));
}

__attribute__((naked)) void declared_naked(void);

void declared_naked(void) {
    asm("ret");
}

int normal(int x) {
    asm("nop");
    return x;
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
// IR-NEXT:     fn %0 @identity(%1 x: i32) -> i32 [linkage=external] [naked] [fallthrough=ub] {
// IR-NEXT:         asm "mov %edi, %eax\\n\\tret" [dialect=att];
// IR-NEXT:     }
// IR-NEXT:     fn %2 @two() -> void [linkage=external] [naked] [fallthrough=ub] {
// IR-NEXT:         asm "nop" [dialect=att];
// IR-NEXT:         asm "ret" [dialect=att];
// IR-NEXT:     }
// IR-NEXT:     fn %3 @constant() -> void [linkage=external] [naked] [fallthrough=ub] {
// IR-NEXT:         asm volatile "mov %0, %%eax\\n\\tret" [dialect=att] {
// IR-NEXT:             template: "mov " %0 ", " %% "eax\\n\\tret";
// IR-NEXT:             in 0 "i" [imm] width 32 const<i32>(42);
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT:     fn %4 @declared_naked() -> void [linkage=external] [naked] [fallthrough=ub] {
// IR-NEXT:         asm "ret" [dialect=att];
// IR-NEXT:     }
// IR-NEXT:     fn %5 @normal(%6 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         asm "nop" [dialect=att] [options=nostack];
// IR-NEXT:         return read<i32>(%6);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
