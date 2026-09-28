// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir
// SLATE-FILECHECK-STD IR c23
static const int tentative;
const int external;
static const int redefined;
static const int redefined = 4;

void tentative_zero(void) {
    asm("# %0 %1 %2" : : "i"(tentative), "i"(external), "i"(redefined));
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
// IR-NEXT:     global %0 tentative: i32 [storage=static] [const] [linkage=internal];
// IR-NEXT:     global %1 external: i32 [storage=static] [const] [linkage=external];
// IR-NEXT:     global %2 redefined: i32 [storage=static] [const] = const<i32>(4) [linkage=internal];
// IR-NEXT:     fn %3 @tentative_zero() -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         asm "# %0 %1 %2" [dialect=att] [options=nomem,nostack] {
// IR-NEXT:             template: "# " %0 " " %1 " " %2;
// IR-NEXT:             in 0 "i" [imm | sym] -> imm width 32 const<i32>(0);
// IR-NEXT:             in 1 "i" [imm | sym] -> imm width 32 const<i32>(0);
// IR-NEXT:             in 2 "i" [imm | sym] -> imm width 32 const<i32>(4);
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
