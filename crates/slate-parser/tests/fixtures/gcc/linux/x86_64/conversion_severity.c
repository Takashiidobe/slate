// SLATE-FILECHECK-DEFINES C89 C89
// SLATE-FILECHECK-STD C89 c89
// SLATE-FILECHECK-WARNING C89
// SLATE-FILECHECK-DEFINES GNU89 GNU89
// SLATE-FILECHECK-STD GNU89 gnu89
// SLATE-FILECHECK-WARNING GNU89
// SLATE-FILECHECK-ARGS --dump-ir

// gcc 16.2.1 demotes both default errors to warnings in c89/gnu89 only; c99
// and later error. Measured 2026-09-21; the c99 half is in
// error/conversion_default_errors_gcc.c.
#if defined(C89) || defined(GNU89)
int *to_pointer(int value) { return value; }
#endif

// SLATE-FILECHECK-BEGIN C89
// C89: -Wint-conversion
// C89: ⚠ incompatible integer to pointer conversion
// C89: ╭─[tests/fixtures/gcc/linux/x86_64/conversion_severity.c:6:37]
// C89: 5 │ #if defined(C89) || defined(GNU89)
// C89: 6 │ int *to_pointer(int value) { return value; }
// C89: ·                                     ─────
// C89: 7 │ #endif
// C89: ╰────
// SLATE-FILECHECK-END C89
// SLATE-FILECHECK-BEGIN GNU89
// GNU89: -Wint-conversion
// GNU89: ⚠ incompatible integer to pointer conversion
// GNU89: ╭─[tests/fixtures/gcc/linux/x86_64/conversion_severity.c:6:37]
// GNU89: 5 │ #if defined(C89) || defined(GNU89)
// GNU89: 6 │ int *to_pointer(int value) { return value; }
// GNU89: ·                                     ─────
// GNU89: 7 │ #endif
// GNU89: ╰────
// SLATE-FILECHECK-END GNU89
// SLATE-FILECHECK-BEGIN IR-C89
// IR-C89: module {
// IR-C89-NEXT:     target "x86_64-unknown-linux-gnu" {
// IR-C89-NEXT:         endian = little;
// IR-C89-NEXT:         pointer [size=8, align=8];
// IR-C89-NEXT:         stack_alignment = 16;
// IR-C89-NEXT:         long_double = f80;
// IR-C89-NEXT:         storage bool [size=1, align=1];
// IR-C89-NEXT:         storage i8, u8 [size=1, align=1];
// IR-C89-NEXT:         storage i16, u16 [size=2, align=2];
// IR-C89-NEXT:         storage i32, u32 [size=4, align=4];
// IR-C89-NEXT:         storage i64, u64 [size=8, align=8];
// IR-C89-NEXT:         storage i128, u128 [size=16, align=16];
// IR-C89-NEXT:         storage bf16 [size=2, align=2];
// IR-C89-NEXT:         storage f16 [size=2, align=2];
// IR-C89-NEXT:         storage f32 [size=4, align=4];
// IR-C89-NEXT:         storage f64 [size=8, align=8];
// IR-C89-NEXT:         storage f80 [size=16, align=16];
// IR-C89-NEXT:         storage f128 [size=16, align=16];
// IR-C89-NEXT:         storage d32 [size=4, align=4];
// IR-C89-NEXT:         storage d64 [size=8, align=8];
// IR-C89-NEXT:         storage d128 [size=16, align=16];
// IR-C89-NEXT:     }
// IR-C89-NEXT:     fn %0 @to_pointer(%1 value: i32) -> ptr<i32> [linkage=external] [fallthrough=ub_if_used] {
// IR-C89-NEXT:         return int_to_ptr<ptr<i32>, reason=return>(read<i32>(%1));
// IR-C89-NEXT:     }
// IR-C89-NEXT: }
// SLATE-FILECHECK-END IR-C89
// SLATE-FILECHECK-BEGIN IR-GNU89
// IR-GNU89: module {
// IR-GNU89-NEXT:     target "x86_64-unknown-linux-gnu" {
// IR-GNU89-NEXT:         endian = little;
// IR-GNU89-NEXT:         pointer [size=8, align=8];
// IR-GNU89-NEXT:         stack_alignment = 16;
// IR-GNU89-NEXT:         long_double = f80;
// IR-GNU89-NEXT:         storage bool [size=1, align=1];
// IR-GNU89-NEXT:         storage i8, u8 [size=1, align=1];
// IR-GNU89-NEXT:         storage i16, u16 [size=2, align=2];
// IR-GNU89-NEXT:         storage i32, u32 [size=4, align=4];
// IR-GNU89-NEXT:         storage i64, u64 [size=8, align=8];
// IR-GNU89-NEXT:         storage i128, u128 [size=16, align=16];
// IR-GNU89-NEXT:         storage bf16 [size=2, align=2];
// IR-GNU89-NEXT:         storage f16 [size=2, align=2];
// IR-GNU89-NEXT:         storage f32 [size=4, align=4];
// IR-GNU89-NEXT:         storage f64 [size=8, align=8];
// IR-GNU89-NEXT:         storage f80 [size=16, align=16];
// IR-GNU89-NEXT:         storage f128 [size=16, align=16];
// IR-GNU89-NEXT:         storage d32 [size=4, align=4];
// IR-GNU89-NEXT:         storage d64 [size=8, align=8];
// IR-GNU89-NEXT:         storage d128 [size=16, align=16];
// IR-GNU89-NEXT:     }
// IR-GNU89-NEXT:     fn %0 @to_pointer(%1 value: i32) -> ptr<i32> [linkage=external] [fallthrough=ub_if_used] {
// IR-GNU89-NEXT:         return int_to_ptr<ptr<i32>, reason=return>(read<i32>(%1));
// IR-GNU89-NEXT:     }
// IR-GNU89-NEXT: }
// SLATE-FILECHECK-END IR-GNU89
