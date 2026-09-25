static x;
f() { return 1; }

// SLATE-FILECHECK-DEFINES C89
// SLATE-FILECHECK-STD C89 c89
// SLATE-FILECHECK-DEFINES C23
// SLATE-FILECHECK-STD C23 c23
// SLATE-FILECHECK-ERROR C23

// SLATE-FILECHECK-BEGIN C23
// C23: Error:   × a type specifier is required for all declarations
// C23: ╰─▶ a type specifier is required for all declarations
// C23: ╭─[tests/fixtures/implicit_int_by_standard.c:1:8]
// C23: 1 │ static x;
// C23: ·        ─
// C23: 2 │ f() { return 1; }
// C23: ╰────
// SLATE-FILECHECK-END C23
// SLATE-FILECHECK-BEGIN C89
// C89: module {
// C89-NEXT:     target "x86_64-unknown-linux-gnu" {
// C89-NEXT:         endian = little;
// C89-NEXT:         pointer [size=8, align=8];
// C89-NEXT:         stack_alignment = 16;
// C89-NEXT:         long_double = f80;
// C89-NEXT:         storage bool [size=1, align=1];
// C89-NEXT:         storage i8, u8 [size=1, align=1];
// C89-NEXT:         storage i16, u16 [size=2, align=2];
// C89-NEXT:         storage i32, u32 [size=4, align=4];
// C89-NEXT:         storage i64, u64 [size=8, align=8];
// C89-NEXT:         storage i128, u128 [size=16, align=16];
// C89-NEXT:         storage bf16 [size=2, align=2];
// C89-NEXT:         storage f16 [size=2, align=2];
// C89-NEXT:         storage f32 [size=4, align=4];
// C89-NEXT:         storage f64 [size=8, align=8];
// C89-NEXT:         storage f80 [size=16, align=16];
// C89-NEXT:         storage f128 [size=16, align=16];
// C89-NEXT:         storage d32 [size=4, align=4];
// C89-NEXT:         storage d64 [size=8, align=8];
// C89-NEXT:         storage d128 [size=16, align=16];
// C89-NEXT:     }
// C89-NEXT:     global %0 x: i32 [storage=static] [linkage=internal];
// C89-NEXT:     fn %1 @f(unprototyped) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// C89-NEXT:         return const<i32>(1);
// C89-NEXT:     }
// C89-NEXT: }
// SLATE-FILECHECK-END C89
