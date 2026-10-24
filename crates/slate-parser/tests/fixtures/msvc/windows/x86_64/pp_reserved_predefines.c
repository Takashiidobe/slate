// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-STD DEFAULT c17
// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-WARNING DEFAULT

#define __STDC_VERSION__ 1L
#undef __STDC_HOSTED__
long version = __STDC_VERSION__;
long hosted = __STDC_HOSTED__;

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: -Wbuiltin-macro-redefined
// DEFAULT:   ⚠ macro name '__STDC_VERSION__' is reserved, '#define' ignored
// DEFAULT:    ╭─[tests/fixtures/msvc/windows/x86_64/pp_reserved_predefines.c:2:9]
// DEFAULT:  1 │
// DEFAULT:  2 │ #define __STDC_VERSION__ 1L
// DEFAULT:    ·         ────────────────
// DEFAULT:  3 │ #undef __STDC_HOSTED__
// DEFAULT:    ╰────
// DEFAULT: -Wbuiltin-macro-redefined
// DEFAULT:   ⚠ macro name '__STDC_HOSTED__' is reserved, '#undef' ignored
// DEFAULT:    ╭─[tests/fixtures/msvc/windows/x86_64/pp_reserved_predefines.c:3:8]
// DEFAULT:  2 │ #define __STDC_VERSION__ 1L
// DEFAULT:  3 │ #undef __STDC_HOSTED__
// DEFAULT:    ·        ───────────────
// DEFAULT:  4 │ long version = __STDC_VERSION__;
// DEFAULT:    ╰────
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN IR-DEFAULT
// IR-DEFAULT: module {
// IR-DEFAULT-NEXT:     target "x86_64-pc-windows-msvc" {
// IR-DEFAULT-NEXT:         endian = little;
// IR-DEFAULT-NEXT:         pointer [size=8, align=8];
// IR-DEFAULT-NEXT:         stack_alignment = 16;
// IR-DEFAULT-NEXT:         long_double = f64;
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
// IR-DEFAULT-NEXT:         storage f128 [size=16, align=16];
// IR-DEFAULT-NEXT:         storage d32 [size=4, align=4];
// IR-DEFAULT-NEXT:         storage d64 [size=8, align=8];
// IR-DEFAULT-NEXT:         storage d128 [size=16, align=16];
// IR-DEFAULT-NEXT:     }
// IR-DEFAULT-NEXT:     global %[[VALUE_version:[0-9]+]] version: i32 [storage=static] = const<i32>(201710) [linkage=external];
// IR-DEFAULT-NEXT:     global %[[VALUE_hosted:[0-9]+]] hosted: i32 [storage=static] = const<i32>(1) [linkage=external];
// IR-DEFAULT-NEXT: }
// SLATE-FILECHECK-END IR-DEFAULT
