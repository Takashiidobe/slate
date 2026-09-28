// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-FLAVOR gcc

// SLATE-FILECHECK-IR-ERROR DEFAULT

enum { OVERFLOWED = (int)1e10 };

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × semantic analysis failed
// DEFAULT: Error:
// DEFAULT: × unsupported in numeric IR lowering: nonconstant or undefined integer
// DEFAULT: ╭─[tests/fixtures/sema/ir_float_constant_folding_gcc.c:3:1]
// DEFAULT: 2 │
// DEFAULT: 3 │ enum { OVERFLOWED = (int)1e10 };
// DEFAULT: · ────────────────────────────────
// DEFAULT: 4 │
// DEFAULT: ╰────
// SLATE-FILECHECK-END DEFAULT
