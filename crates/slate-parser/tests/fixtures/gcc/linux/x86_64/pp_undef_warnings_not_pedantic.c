// SLATE-FILECHECK-ARGS -pedantic-errors
// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-WARNING DEFAULT

#undef __STDC_HOSTED__
#undef __DATE__
#undef __LINE__
int x;

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: -Wmacro-redefined
// DEFAULT: ⚠ undefining '__STDC_HOSTED__'
// DEFAULT: ╭─[tests/fixtures/gcc/linux/x86_64/pp_undef_warnings_not_pedantic.c:2:8]
// DEFAULT: 1 │
// DEFAULT: 2 │ #undef __STDC_HOSTED__
// DEFAULT: ·        ───────────────
// DEFAULT: 3 │ #undef __DATE__
// DEFAULT: ╰────
// DEFAULT: -Wbuiltin-macro-redefined
// DEFAULT: ⚠ undefining '__DATE__'
// DEFAULT: ╭─[tests/fixtures/gcc/linux/x86_64/pp_undef_warnings_not_pedantic.c:3:8]
// DEFAULT: 2 │ #undef __STDC_HOSTED__
// DEFAULT: 3 │ #undef __DATE__
// DEFAULT: ·        ────────
// DEFAULT: 4 │ #undef __LINE__
// DEFAULT: ╰────
// DEFAULT: -Wmacro-redefined
// DEFAULT: ⚠ undefining '__LINE__'
// DEFAULT: ╭─[tests/fixtures/gcc/linux/x86_64/pp_undef_warnings_not_pedantic.c:4:8]
// DEFAULT: 3 │ #undef __DATE__
// DEFAULT: 4 │ #undef __LINE__
// DEFAULT: ·        ────────
// DEFAULT: 5 │ int x;
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
// IR-DEFAULT-NEXT:     global %[[VALUE_x:[0-9]+]] x: i32 [storage=static] [linkage=external];
// IR-DEFAULT-NEXT: }
// SLATE-FILECHECK-END IR-DEFAULT
