// { dg-do preprocess }
// { dg-options "-std=gnu99 -fdiagnostics-show-option -Wdeprecated" }

#assert x(x)  // { dg-warning "'#assert' is a deprecated GCC extension .-Wdeprecated." }

#if #x(x)     // { dg-warning "assertions are a deprecated extension .-Wdeprecated." }
#endif
// SLATE-FILECHECK-STD DEFAULT gnu99
// SLATE-FILECHECK-ARGS -Wdeprecated
// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-WARNING DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: -Wdeprecated
// DEFAULT: ⚠ '#assert' is a deprecated GCC extension
// DEFAULT: ╭─[tests/fixtures/suites/gcc-dg/gcc/linux/x86_64/cpp__warn-deprecated.c:4:2]
// DEFAULT: 3 │
// DEFAULT: 4 │ #assert x(x)  // { dg-warning "'#assert' is a deprecated GCC extension .-Wdeprecated." }
// DEFAULT: ·  ──────
// DEFAULT: 5 │
// DEFAULT: ╰────
// DEFAULT: -Wdeprecated
// DEFAULT: ⚠ assertions are a deprecated extension
// DEFAULT: ╭─[tests/fixtures/suites/gcc-dg/gcc/linux/x86_64/cpp__warn-deprecated.c:6:5]
// DEFAULT: 5 │
// DEFAULT: 6 │ #if #x(x)     // { dg-warning "assertions are a deprecated extension .-Wdeprecated." }
// DEFAULT: ·     ─
// DEFAULT: 7 │ #endif
// DEFAULT: ╰────
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN IR-DEFAULT
// IR-DEFAULT: module {
// IR-DEFAULT-NEXT:     target "x86_64-unknown-linux-gnu" {
// IR-DEFAULT-NEXT:         endian = little;
// IR-DEFAULT-NEXT:         pointer [size=8, align=8];
// IR-DEFAULT-NEXT:         stack_alignment = 16;
// IR-DEFAULT-NEXT:         long_double = f80;
// IR-DEFAULT-NEXT:         storage bool [size=1, align=1];
// IR-DEFAULT-NEXT:         storage i8, u8 [size=1, align=1];
// IR-DEFAULT-NEXT:         storage i16, u16 [size=2, align=2];
// IR-DEFAULT-NEXT:         storage i32, u32 [size=4, align=4];
// IR-DEFAULT-NEXT:         storage i64, u64 [size=8, align=8];
// IR-DEFAULT-NEXT:         storage i128, u128 [size=16, align=16];
// IR-DEFAULT-NEXT:         storage bf16 [size=2, align=2];
// IR-DEFAULT-NEXT:         storage f16 [size=2, align=2];
// IR-DEFAULT-NEXT:         storage f32 [size=4, align=4];
// IR-DEFAULT-NEXT:         storage f64 [size=8, align=8];
// IR-DEFAULT-NEXT:         storage f80 [size=16, align=16];
// IR-DEFAULT-NEXT:         storage f128 [size=16, align=16];
// IR-DEFAULT-NEXT:         storage d32 [size=4, align=4];
// IR-DEFAULT-NEXT:         storage d64 [size=8, align=8];
// IR-DEFAULT-NEXT:         storage d128 [size=16, align=16];
// IR-DEFAULT-NEXT:     }
// IR-DEFAULT-NEXT: }
// SLATE-FILECHECK-END IR-DEFAULT
