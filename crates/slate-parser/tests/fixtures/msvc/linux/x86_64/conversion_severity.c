// SLATE-FILECHECK-DEFINES WARN
// SLATE-FILECHECK-WARNING WARN
// SLATE-FILECHECK-ARGS --dump-ir

// MSVC 19.51 reports C4047/C4133 and keeps compiling at every standard, so
// the msvc flavor keeps both as warnings. Measured 2026-09-21.
int *to_pointer(int value) { return value; }

// SLATE-FILECHECK-BEGIN WARN
// WARN: -Wint-conversion
// WARN: ⚠ incompatible integer to pointer conversion
// WARN: ╭─[tests/fixtures/msvc/linux/x86_64/conversion_severity.c:4:37]
// WARN: 3 │ // the msvc flavor keeps both as warnings. Measured 2026-09-21.
// WARN: 4 │ int *to_pointer(int value) { return value; }
// WARN: ·                                     ─────
// WARN: 5 │
// WARN: ╰────
// SLATE-FILECHECK-END WARN
// SLATE-FILECHECK-BEGIN IR-WARN
// IR-WARN: module {
// IR-WARN-NEXT:     target "x86_64-unknown-linux-gnu" {
// IR-WARN-NEXT:         endian = little;
// IR-WARN-NEXT:         pointer [size=8, align=8];
// IR-WARN-NEXT:         stack_alignment = 16;
// IR-WARN-NEXT:         long_double = f80;
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
// IR-WARN-NEXT:         storage f80 [size=16, align=16];
// IR-WARN-NEXT:         storage f128 [size=16, align=16];
// IR-WARN-NEXT:         storage d32 [size=4, align=4];
// IR-WARN-NEXT:         storage d64 [size=8, align=8];
// IR-WARN-NEXT:         storage d128 [size=16, align=16];
// IR-WARN-NEXT:     }
// IR-WARN-NEXT:     fn %0 @to_pointer(%1 value: i32) -> ptr<i32> [linkage=external] [fallthrough=ub_if_used] {
// IR-WARN-NEXT:         return int_to_ptr<ptr<i32>, reason=return>(read<i32>(%1));
// IR-WARN-NEXT:     }
// IR-WARN-NEXT: }
// SLATE-FILECHECK-END IR-WARN
