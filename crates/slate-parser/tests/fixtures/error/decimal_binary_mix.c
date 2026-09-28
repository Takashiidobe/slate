_Decimal64 mixed(_Decimal64 x, double d) { return x + d; }

// SLATE-FILECHECK-ERROR SEMA
// SLATE-FILECHECK-ARGS --dump-ir

// SLATE-FILECHECK-BEGIN SEMA
// SEMA: Error:   × semantic analysis failed
// SEMA: Error:
// SEMA: × invalid operands to binary expression: d64 + f64
// SEMA: ╭─[tests/fixtures/error/decimal_binary_mix.c:1:1]
// SEMA: 1 │ _Decimal64 mixed(_Decimal64 x, double d) { return x + d; }
// SEMA: · ──────────────────────────────────────────────────────────
// SEMA: 2 │
// SEMA: ╰────
// SLATE-FILECHECK-END SEMA
