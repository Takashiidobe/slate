// SLATE-FILECHECK-DEFINES WARN
// SLATE-FILECHECK-WARNING WARN
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir


int object;
long object;

int returns(int);
long returns(int);

// SLATE-FILECHECK-BEGIN WARN
// WARN: -Wconflicting-types
// WARN: ⚠ redeclaration with a different integer type of the same size
// WARN: ╭─[tests/fixtures/sema/x86_64-pc-windows-msvc/ir_redeclaration_layout.c:4:6]
// WARN: 3 │ int object;
// WARN: 4 │ long object;
// WARN: ·      ──────
// WARN: 5 │
// WARN: ╰────
// WARN: -Wconflicting-types
// WARN: ⚠ function redeclared with a different integer return type of the same size
// WARN: ╭─[tests/fixtures/sema/x86_64-pc-windows-msvc/ir_redeclaration_layout.c:7:6]
// WARN: 6 │ int returns(int);
// WARN: 7 │ long returns(int);
// WARN: ·      ────────────
// WARN: 8 │
// WARN: ╰────
// SLATE-FILECHECK-END WARN
// SLATE-FILECHECK-BEGIN IR-WARN
// IR-WARN: module {
// IR-WARN-NEXT:     target "x86_64-pc-windows-msvc" {
// IR-WARN-NEXT:         endian = little;
// IR-WARN-NEXT:         pointer [size=8, align=8];
// IR-WARN-NEXT:         stack_alignment = 16;
// IR-WARN-NEXT:         long_double = f64;
// IR-WARN-NEXT:         storage bool [size=1, align=1];
// IR-WARN-NEXT:         storage i8, u8 [size=1, align=1];
// IR-WARN-NEXT:         storage i16, u16 [size=2, align=2];
// IR-WARN-NEXT:         storage i32, u32 [size=4, align=4];
// IR-WARN-NEXT:         storage i64, u64 [size=8, align=8];
// IR-WARN-NEXT:         storage i128, u128 [size=16, align=16];
// IR-WARN-NEXT:         storage bf16 [size=2, align=2];
// IR-WARN-NEXT:         storage f16 [size=2, align=2];
// IR-WARN-NEXT:         storage f32 [size=4, align=4];
// IR-WARN-NEXT:         storage f64 [size=8, align=8];
// IR-WARN-NEXT:         storage f128 [size=16, align=16];
// IR-WARN-NEXT:         storage d32 [size=4, align=4];
// IR-WARN-NEXT:         storage d64 [size=8, align=8];
// IR-WARN-NEXT:         storage d128 [size=16, align=16];
// IR-WARN-NEXT:     }
// IR-WARN-NEXT:     global %0 object: i32 [storage=static] [linkage=external];
// IR-WARN-NEXT:     fn %1 @returns(%2 <unnamed>: i32) -> i32 [linkage=external];
// IR-WARN-NEXT: }
// SLATE-FILECHECK-END IR-WARN
