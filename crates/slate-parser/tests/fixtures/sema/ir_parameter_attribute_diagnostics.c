// SLATE-FILECHECK-DEFINES WARN_ALIGNED WARN_ALIGNED
// SLATE-FILECHECK-WARNING WARN_ALIGNED
// SLATE-FILECHECK-DEFINES WARN_ALIGNAS WARN_ALIGNAS
// SLATE-FILECHECK-WARNING WARN_ALIGNAS
// SLATE-FILECHECK-DEFINES ERR_SYMBOL ERR_SYMBOL
// SLATE-FILECHECK-ERROR ERR_SYMBOL
// SLATE-FILECHECK-DEFINES ERR_LAYOUT ERR_LAYOUT
// SLATE-FILECHECK-ERROR ERR_LAYOUT
// SLATE-FILECHECK-DEFINES WARN_IGNORED WARN_IGNORED
// SLATE-FILECHECK-WARNING WARN_IGNORED
// SLATE-FILECHECK-DEFINES ERR_UNSUPPORTED ERR_UNSUPPORTED
// SLATE-FILECHECK-ERROR ERR_UNSUPPORTED
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir

#if defined(WARN_ALIGNED)
void f(int q __attribute__((aligned(16))));
#elif defined(WARN_ALIGNAS)
void f(_Alignas(32) int p);
#elif defined(ERR_SYMBOL)
void f(int p __attribute__((section("s"))));
#elif defined(ERR_LAYOUT)
void f(int p __attribute__((nocommon)));
#elif defined(WARN_IGNORED)
void f(int p __attribute__((packed)));
#elif defined(ERR_UNSUPPORTED)
void f(int p __attribute__((code_seg("s"))));
#endif

// SLATE-FILECHECK-BEGIN ERR_SYMBOL
// ERR_SYMBOL: Error:   × unsupported in numeric IR lowering: symbol attribute on a parameter
// SLATE-FILECHECK-END ERR_SYMBOL
// SLATE-FILECHECK-BEGIN ERR_LAYOUT
// ERR_LAYOUT: Error:   × unsupported in numeric IR lowering: layout attribute on a parameter
// SLATE-FILECHECK-END ERR_LAYOUT
// SLATE-FILECHECK-BEGIN ERR_UNSUPPORTED
// ERR_UNSUPPORTED: Error:   × unsupported in numeric IR lowering: code segment attribute
// SLATE-FILECHECK-END ERR_UNSUPPORTED
// SLATE-FILECHECK-BEGIN WARN_ALIGNED
// WARN_ALIGNED: -Wparameter-alignment
// WARN_ALIGNED: ⚠ alignment of 16 on a function parameter is rejected by gcc
// WARN_ALIGNED: ╭─[tests/fixtures/sema/ir_parameter_attribute_diagnostics.c:3:8]
// WARN_ALIGNED: 2 │ #if defined(WARN_ALIGNED)
// WARN_ALIGNED: 3 │ void f(int q __attribute__((aligned(16))));
// WARN_ALIGNED: ·        ──────────────────────────────────
// WARN_ALIGNED: 4 │ #elif defined(WARN_ALIGNAS)
// WARN_ALIGNED: ╰────
// SLATE-FILECHECK-END WARN_ALIGNED
// SLATE-FILECHECK-BEGIN WARN_ALIGNAS
// WARN_ALIGNAS: -Wparameter-alignment
// WARN_ALIGNAS: ⚠ alignment of 32 on a function parameter is rejected by clang and gcc
// WARN_ALIGNAS: ╭─[tests/fixtures/sema/ir_parameter_attribute_diagnostics.c:5:8]
// WARN_ALIGNAS: 4 │ #elif defined(WARN_ALIGNAS)
// WARN_ALIGNAS: 5 │ void f(_Alignas(32) int p);
// WARN_ALIGNAS: ·        ──────────────────
// WARN_ALIGNAS: 6 │ #elif defined(ERR_SYMBOL)
// WARN_ALIGNAS: ╰────
// SLATE-FILECHECK-END WARN_ALIGNAS
// SLATE-FILECHECK-BEGIN WARN_IGNORED
// WARN_IGNORED: -Wignored-attributes
// WARN_IGNORED: ⚠ 'packed' attribute ignored
// WARN_IGNORED: ╭─[tests/fixtures/sema/ir_parameter_attribute_diagnostics.c:11:29]
// WARN_IGNORED: 10 │ #elif defined(WARN_IGNORED)
// WARN_IGNORED: 11 │ void f(int p __attribute__((packed)));
// WARN_IGNORED: ·                             ──────
// WARN_IGNORED: 12 │ #elif defined(ERR_UNSUPPORTED)
// WARN_IGNORED: ╰────
// SLATE-FILECHECK-END WARN_IGNORED
// SLATE-FILECHECK-BEGIN IR-WARN_ALIGNED
// IR-WARN_ALIGNED: module {
// IR-WARN_ALIGNED-NEXT:     target "x86_64-unknown-linux-gnu" {
// IR-WARN_ALIGNED-NEXT:         endian = little;
// IR-WARN_ALIGNED-NEXT:         pointer [size=8, align=8];
// IR-WARN_ALIGNED-NEXT:         stack_alignment = 16;
// IR-WARN_ALIGNED-NEXT:         long_double = f80;
// IR-WARN_ALIGNED-NEXT:         storage bool [size=1, align=1];
// IR-WARN_ALIGNED-NEXT:         storage i8, u8 [size=1, align=1];
// IR-WARN_ALIGNED-NEXT:         storage i16, u16 [size=2, align=2];
// IR-WARN_ALIGNED-NEXT:         storage i32, u32 [size=4, align=4];
// IR-WARN_ALIGNED-NEXT:         storage i64, u64 [size=8, align=8];
// IR-WARN_ALIGNED-NEXT:         storage i128, u128 [size=16, align=16];
// IR-WARN_ALIGNED-NEXT:         storage bf16 [size=2, align=2];
// IR-WARN_ALIGNED-NEXT:         storage f16 [size=2, align=2];
// IR-WARN_ALIGNED-NEXT:         storage f32 [size=4, align=4];
// IR-WARN_ALIGNED-NEXT:         storage f64 [size=8, align=8];
// IR-WARN_ALIGNED-NEXT:         storage f80 [size=16, align=16];
// IR-WARN_ALIGNED-NEXT:         storage f128 [size=16, align=16];
// IR-WARN_ALIGNED-NEXT:         storage d32 [size=4, align=4];
// IR-WARN_ALIGNED-NEXT:         storage d64 [size=8, align=8];
// IR-WARN_ALIGNED-NEXT:         storage d128 [size=16, align=16];
// IR-WARN_ALIGNED-NEXT:     }
// IR-WARN_ALIGNED-NEXT:     fn %0 @f(%1 q: i32) -> void [linkage=external];
// IR-WARN_ALIGNED-NEXT: }
// SLATE-FILECHECK-END IR-WARN_ALIGNED
// SLATE-FILECHECK-BEGIN IR-WARN_ALIGNAS
// IR-WARN_ALIGNAS: module {
// IR-WARN_ALIGNAS-NEXT:     target "x86_64-unknown-linux-gnu" {
// IR-WARN_ALIGNAS-NEXT:         endian = little;
// IR-WARN_ALIGNAS-NEXT:         pointer [size=8, align=8];
// IR-WARN_ALIGNAS-NEXT:         stack_alignment = 16;
// IR-WARN_ALIGNAS-NEXT:         long_double = f80;
// IR-WARN_ALIGNAS-NEXT:         storage bool [size=1, align=1];
// IR-WARN_ALIGNAS-NEXT:         storage i8, u8 [size=1, align=1];
// IR-WARN_ALIGNAS-NEXT:         storage i16, u16 [size=2, align=2];
// IR-WARN_ALIGNAS-NEXT:         storage i32, u32 [size=4, align=4];
// IR-WARN_ALIGNAS-NEXT:         storage i64, u64 [size=8, align=8];
// IR-WARN_ALIGNAS-NEXT:         storage i128, u128 [size=16, align=16];
// IR-WARN_ALIGNAS-NEXT:         storage bf16 [size=2, align=2];
// IR-WARN_ALIGNAS-NEXT:         storage f16 [size=2, align=2];
// IR-WARN_ALIGNAS-NEXT:         storage f32 [size=4, align=4];
// IR-WARN_ALIGNAS-NEXT:         storage f64 [size=8, align=8];
// IR-WARN_ALIGNAS-NEXT:         storage f80 [size=16, align=16];
// IR-WARN_ALIGNAS-NEXT:         storage f128 [size=16, align=16];
// IR-WARN_ALIGNAS-NEXT:         storage d32 [size=4, align=4];
// IR-WARN_ALIGNAS-NEXT:         storage d64 [size=8, align=8];
// IR-WARN_ALIGNAS-NEXT:         storage d128 [size=16, align=16];
// IR-WARN_ALIGNAS-NEXT:     }
// IR-WARN_ALIGNAS-NEXT:     fn %0 @f(%1 p: i32) -> void [linkage=external];
// IR-WARN_ALIGNAS-NEXT: }
// SLATE-FILECHECK-END IR-WARN_ALIGNAS
// SLATE-FILECHECK-BEGIN IR-WARN_IGNORED
// IR-WARN_IGNORED: module {
// IR-WARN_IGNORED-NEXT:     target "x86_64-unknown-linux-gnu" {
// IR-WARN_IGNORED-NEXT:         endian = little;
// IR-WARN_IGNORED-NEXT:         pointer [size=8, align=8];
// IR-WARN_IGNORED-NEXT:         stack_alignment = 16;
// IR-WARN_IGNORED-NEXT:         long_double = f80;
// IR-WARN_IGNORED-NEXT:         storage bool [size=1, align=1];
// IR-WARN_IGNORED-NEXT:         storage i8, u8 [size=1, align=1];
// IR-WARN_IGNORED-NEXT:         storage i16, u16 [size=2, align=2];
// IR-WARN_IGNORED-NEXT:         storage i32, u32 [size=4, align=4];
// IR-WARN_IGNORED-NEXT:         storage i64, u64 [size=8, align=8];
// IR-WARN_IGNORED-NEXT:         storage i128, u128 [size=16, align=16];
// IR-WARN_IGNORED-NEXT:         storage bf16 [size=2, align=2];
// IR-WARN_IGNORED-NEXT:         storage f16 [size=2, align=2];
// IR-WARN_IGNORED-NEXT:         storage f32 [size=4, align=4];
// IR-WARN_IGNORED-NEXT:         storage f64 [size=8, align=8];
// IR-WARN_IGNORED-NEXT:         storage f80 [size=16, align=16];
// IR-WARN_IGNORED-NEXT:         storage f128 [size=16, align=16];
// IR-WARN_IGNORED-NEXT:         storage d32 [size=4, align=4];
// IR-WARN_IGNORED-NEXT:         storage d64 [size=8, align=8];
// IR-WARN_IGNORED-NEXT:         storage d128 [size=16, align=16];
// IR-WARN_IGNORED-NEXT:     }
// IR-WARN_IGNORED-NEXT:     fn %0 @f(%1 p: i32) -> void [linkage=external];
// IR-WARN_IGNORED-NEXT: }
// SLATE-FILECHECK-END IR-WARN_IGNORED
