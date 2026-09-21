// SLATE-FILECHECK-DEFINES WARN_ALIGNED WARN_ALIGNED
// SLATE-FILECHECK-WARNING WARN_ALIGNED
// SLATE-FILECHECK-DEFINES WARN_ALIGNAS WARN_ALIGNAS
// SLATE-FILECHECK-WARNING WARN_ALIGNAS
// SLATE-FILECHECK-DEFINES ERR_SYMBOL ERR_SYMBOL
// SLATE-FILECHECK-ERROR ERR_SYMBOL
// SLATE-FILECHECK-DEFINES ERR_LAYOUT ERR_LAYOUT
// SLATE-FILECHECK-ERROR ERR_LAYOUT
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
void f(int p __attribute__((packed)));
#elif defined(ERR_UNSUPPORTED)
void f(int p __attribute__((cleanup(g))));
#endif

// SLATE-FILECHECK-BEGIN ERR_SYMBOL
// ERR_SYMBOL: Error:   × unsupported in numeric IR lowering: symbol attribute on a parameter
// SLATE-FILECHECK-END ERR_SYMBOL
// SLATE-FILECHECK-BEGIN ERR_LAYOUT
// ERR_LAYOUT: Error:   × unsupported in numeric IR lowering: layout attribute on a parameter
// SLATE-FILECHECK-END ERR_LAYOUT
// SLATE-FILECHECK-BEGIN ERR_UNSUPPORTED
// ERR_UNSUPPORTED: Error:   × unsupported in numeric IR lowering: cleanup attribute
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
