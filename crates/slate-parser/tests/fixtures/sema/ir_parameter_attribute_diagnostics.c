// SLATE-FILECHECK-DEFINES WARN_ALIGNED WARN_ALIGNED
// SLATE-FILECHECK-WARNING WARN_ALIGNED
// SLATE-FILECHECK-DEFINES WARN_ALIGNAS WARN_ALIGNAS
// SLATE-FILECHECK-WARNING WARN_ALIGNAS
// SLATE-FILECHECK-DEFINES ERR_SYMBOL ERR_SYMBOL
// SLATE-FILECHECK-ERROR ERR_SYMBOL
// SLATE-FILECHECK-DEFINES WARN_SYMBOL WARN_SYMBOL
// SLATE-FILECHECK-WARNING WARN_SYMBOL
// SLATE-FILECHECK-DEFINES WARN_LAYOUT WARN_LAYOUT
// SLATE-FILECHECK-WARNING WARN_LAYOUT
// SLATE-FILECHECK-DEFINES VECTOR VECTOR
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
#elif defined(WARN_SYMBOL)
void f(int p __attribute__((weak, visibility("hidden"))));
#elif defined(WARN_LAYOUT)
void f(int p __attribute__((nocommon)));
#elif defined(VECTOR)
void f(int p __attribute__((vector_size(16))));
#elif defined(WARN_IGNORED)
void f(int p __attribute__((packed)));
#elif defined(ERR_UNSUPPORTED)
void f(__declspec(code_seg("s")) int p);
#endif

// SLATE-FILECHECK-BEGIN ERR_SYMBOL
// ERR_SYMBOL: Error:   × semantic analysis failed
// ERR_SYMBOL: Error:
// ERR_SYMBOL: × invalid in this context: 'section' attribute only applies to functions and
// ERR_SYMBOL: ╭─[tests/fixtures/sema/ir_parameter_attribute_diagnostics.c:7:1]
// ERR_SYMBOL: 6 │ #elif defined(ERR_SYMBOL)
// ERR_SYMBOL: 7 │ void f(int p __attribute__((section("s"))));
// ERR_SYMBOL: · ────────────────────────────────────────────
// ERR_SYMBOL: 8 │ #elif defined(WARN_SYMBOL)
// ERR_SYMBOL: ╰────
// SLATE-FILECHECK-END ERR_SYMBOL
// SLATE-FILECHECK-BEGIN ERR_UNSUPPORTED
// ERR_UNSUPPORTED: Error:   × semantic analysis failed
// ERR_UNSUPPORTED: Error:
// ERR_UNSUPPORTED: × unsupported in numeric IR lowering: code segment attribute
// ERR_UNSUPPORTED: ╭─[tests/fixtures/sema/ir_parameter_attribute_diagnostics.c:17:1]
// ERR_UNSUPPORTED: 16 │ #elif defined(ERR_UNSUPPORTED)
// ERR_UNSUPPORTED: 17 │ void f(__declspec(code_seg("s")) int p);
// ERR_UNSUPPORTED: · ────────────────────────────────────────
// ERR_UNSUPPORTED: 18 │ #endif
// ERR_UNSUPPORTED: ╰────
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
// SLATE-FILECHECK-BEGIN WARN_SYMBOL
// WARN_SYMBOL: -Wignored-attributes
// WARN_SYMBOL: ⚠ 'weak' attribute ignored
// WARN_SYMBOL: ╭─[tests/fixtures/sema/ir_parameter_attribute_diagnostics.c:9:29]
// WARN_SYMBOL: 8 │ #elif defined(WARN_SYMBOL)
// WARN_SYMBOL: 9 │ void f(int p __attribute__((weak, visibility("hidden"))));
// WARN_SYMBOL: ·                             ────
// WARN_SYMBOL: 10 │ #elif defined(WARN_LAYOUT)
// WARN_SYMBOL: ╰────
// WARN_SYMBOL: -Wignored-attributes
// WARN_SYMBOL: ⚠ 'visibility' attribute ignored
// WARN_SYMBOL: ╭─[tests/fixtures/sema/ir_parameter_attribute_diagnostics.c:9:35]
// WARN_SYMBOL: 8 │ #elif defined(WARN_SYMBOL)
// WARN_SYMBOL: 9 │ void f(int p __attribute__((weak, visibility("hidden"))));
// WARN_SYMBOL: ·                                   ──────────
// WARN_SYMBOL: 10 │ #elif defined(WARN_LAYOUT)
// WARN_SYMBOL: ╰────
// SLATE-FILECHECK-END WARN_SYMBOL
// SLATE-FILECHECK-BEGIN WARN_LAYOUT
// WARN_LAYOUT: -Wignored-attributes
// WARN_LAYOUT: ⚠ 'nocommon' attribute ignored
// WARN_LAYOUT: ╭─[tests/fixtures/sema/ir_parameter_attribute_diagnostics.c:11:29]
// WARN_LAYOUT: 10 │ #elif defined(WARN_LAYOUT)
// WARN_LAYOUT: 11 │ void f(int p __attribute__((nocommon)));
// WARN_LAYOUT: ·                             ────────
// WARN_LAYOUT: 12 │ #elif defined(VECTOR)
// WARN_LAYOUT: ╰────
// SLATE-FILECHECK-END WARN_LAYOUT
// SLATE-FILECHECK-BEGIN WARN_IGNORED
// WARN_IGNORED: -Wignored-attributes
// WARN_IGNORED: ⚠ 'packed' attribute ignored
// WARN_IGNORED: ╭─[tests/fixtures/sema/ir_parameter_attribute_diagnostics.c:15:29]
// WARN_IGNORED: 14 │ #elif defined(WARN_IGNORED)
// WARN_IGNORED: 15 │ void f(int p __attribute__((packed)));
// WARN_IGNORED: ·                             ──────
// WARN_IGNORED: 16 │ #elif defined(ERR_UNSUPPORTED)
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
// SLATE-FILECHECK-BEGIN IR-WARN_SYMBOL
// IR-WARN_SYMBOL: module {
// IR-WARN_SYMBOL-NEXT:     target "x86_64-unknown-linux-gnu" {
// IR-WARN_SYMBOL-NEXT:         endian = little;
// IR-WARN_SYMBOL-NEXT:         pointer [size=8, align=8];
// IR-WARN_SYMBOL-NEXT:         stack_alignment = 16;
// IR-WARN_SYMBOL-NEXT:         long_double = f80;
// IR-WARN_SYMBOL-NEXT:         storage bool [size=1, align=1];
// IR-WARN_SYMBOL-NEXT:         storage i8, u8 [size=1, align=1];
// IR-WARN_SYMBOL-NEXT:         storage i16, u16 [size=2, align=2];
// IR-WARN_SYMBOL-NEXT:         storage i32, u32 [size=4, align=4];
// IR-WARN_SYMBOL-NEXT:         storage i64, u64 [size=8, align=8];
// IR-WARN_SYMBOL-NEXT:         storage i128, u128 [size=16, align=16];
// IR-WARN_SYMBOL-NEXT:         storage bf16 [size=2, align=2];
// IR-WARN_SYMBOL-NEXT:         storage f16 [size=2, align=2];
// IR-WARN_SYMBOL-NEXT:         storage f32 [size=4, align=4];
// IR-WARN_SYMBOL-NEXT:         storage f64 [size=8, align=8];
// IR-WARN_SYMBOL-NEXT:         storage f80 [size=16, align=16];
// IR-WARN_SYMBOL-NEXT:         storage f128 [size=16, align=16];
// IR-WARN_SYMBOL-NEXT:         storage d32 [size=4, align=4];
// IR-WARN_SYMBOL-NEXT:         storage d64 [size=8, align=8];
// IR-WARN_SYMBOL-NEXT:         storage d128 [size=16, align=16];
// IR-WARN_SYMBOL-NEXT:     }
// IR-WARN_SYMBOL-NEXT:     fn %0 @f(%1 p: i32) -> void [linkage=external];
// IR-WARN_SYMBOL-NEXT: }
// SLATE-FILECHECK-END IR-WARN_SYMBOL
// SLATE-FILECHECK-BEGIN IR-WARN_LAYOUT
// IR-WARN_LAYOUT: module {
// IR-WARN_LAYOUT-NEXT:     target "x86_64-unknown-linux-gnu" {
// IR-WARN_LAYOUT-NEXT:         endian = little;
// IR-WARN_LAYOUT-NEXT:         pointer [size=8, align=8];
// IR-WARN_LAYOUT-NEXT:         stack_alignment = 16;
// IR-WARN_LAYOUT-NEXT:         long_double = f80;
// IR-WARN_LAYOUT-NEXT:         storage bool [size=1, align=1];
// IR-WARN_LAYOUT-NEXT:         storage i8, u8 [size=1, align=1];
// IR-WARN_LAYOUT-NEXT:         storage i16, u16 [size=2, align=2];
// IR-WARN_LAYOUT-NEXT:         storage i32, u32 [size=4, align=4];
// IR-WARN_LAYOUT-NEXT:         storage i64, u64 [size=8, align=8];
// IR-WARN_LAYOUT-NEXT:         storage i128, u128 [size=16, align=16];
// IR-WARN_LAYOUT-NEXT:         storage bf16 [size=2, align=2];
// IR-WARN_LAYOUT-NEXT:         storage f16 [size=2, align=2];
// IR-WARN_LAYOUT-NEXT:         storage f32 [size=4, align=4];
// IR-WARN_LAYOUT-NEXT:         storage f64 [size=8, align=8];
// IR-WARN_LAYOUT-NEXT:         storage f80 [size=16, align=16];
// IR-WARN_LAYOUT-NEXT:         storage f128 [size=16, align=16];
// IR-WARN_LAYOUT-NEXT:         storage d32 [size=4, align=4];
// IR-WARN_LAYOUT-NEXT:         storage d64 [size=8, align=8];
// IR-WARN_LAYOUT-NEXT:         storage d128 [size=16, align=16];
// IR-WARN_LAYOUT-NEXT:     }
// IR-WARN_LAYOUT-NEXT:     fn %0 @f(%1 p: i32) -> void [linkage=external];
// IR-WARN_LAYOUT-NEXT: }
// SLATE-FILECHECK-END IR-WARN_LAYOUT
// SLATE-FILECHECK-BEGIN VECTOR
// VECTOR: module {
// VECTOR-NEXT:     target "x86_64-unknown-linux-gnu" {
// VECTOR-NEXT:         endian = little;
// VECTOR-NEXT:         pointer [size=8, align=8];
// VECTOR-NEXT:         stack_alignment = 16;
// VECTOR-NEXT:         long_double = f80;
// VECTOR-NEXT:         storage bool [size=1, align=1];
// VECTOR-NEXT:         storage i8, u8 [size=1, align=1];
// VECTOR-NEXT:         storage i16, u16 [size=2, align=2];
// VECTOR-NEXT:         storage i32, u32 [size=4, align=4];
// VECTOR-NEXT:         storage i64, u64 [size=8, align=8];
// VECTOR-NEXT:         storage i128, u128 [size=16, align=16];
// VECTOR-NEXT:         storage bf16 [size=2, align=2];
// VECTOR-NEXT:         storage f16 [size=2, align=2];
// VECTOR-NEXT:         storage f32 [size=4, align=4];
// VECTOR-NEXT:         storage f64 [size=8, align=8];
// VECTOR-NEXT:         storage f80 [size=16, align=16];
// VECTOR-NEXT:         storage f128 [size=16, align=16];
// VECTOR-NEXT:         storage d32 [size=4, align=4];
// VECTOR-NEXT:         storage d64 [size=8, align=8];
// VECTOR-NEXT:         storage d128 [size=16, align=16];
// VECTOR-NEXT:     }
// VECTOR-NEXT:     fn %0 @f(%1 p: vector<i32, 4>) -> void [linkage=external] [abi=sysv64(direct) -> void];
// VECTOR-NEXT: }
// SLATE-FILECHECK-END VECTOR
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
