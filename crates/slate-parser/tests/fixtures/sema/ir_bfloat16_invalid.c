// SLATE-FILECHECK-DEFINES DECIMAL DECIMAL
// SLATE-FILECHECK-DEFINES TRAILING TRAILING
// SLATE-FILECHECK-DEFINES TRUNCATED TRUNCATED
// SLATE-FILECHECK-DEFINES HEXNOEXP HEXNOEXP
// SLATE-FILECHECK-DEFINES MIXEDCASE MIXEDCASE
// SLATE-FILECHECK-ERROR DECIMAL
// SLATE-FILECHECK-ERROR TRAILING
// SLATE-FILECHECK-ERROR TRUNCATED
// SLATE-FILECHECK-ERROR HEXNOEXP
// SLATE-FILECHECK-ERROR MIXEDCASE
// SLATE-FILECHECK-ARGS --dump-ir

#ifdef DECIMAL
_Decimal64 mixed(__bf16 x, _Decimal64 y) { return x + y; }
#endif

#ifdef TRAILING
__bf16 trailing = 1.5bf16l;
#endif

#ifdef TRUNCATED
__bf16 truncated = 1.5bf1;
#endif

#ifdef HEXNOEXP
__bf16 hex_no_exponent = 0x1.8bf16;
#endif

#ifdef MIXEDCASE
double mixed_case = 1.5dF;
#endif

// SLATE-FILECHECK-BEGIN DECIMAL
// DECIMAL: Error:   × invalid operands to binary expression: bf16 + d64
// SLATE-FILECHECK-END DECIMAL
// SLATE-FILECHECK-BEGIN TRAILING
// TRAILING: Error:   × semantic analysis failed
// TRAILING: Error:
// TRAILING: × invalid floating literal `1.5bf16l`
// TRAILING: ╭─[tests/fixtures/sema/ir_bfloat16_invalid.c:7:19]
// TRAILING: 6 │ #ifdef TRAILING
// TRAILING: 7 │ __bf16 trailing = 1.5bf16l;
// TRAILING: ·                   ────────
// TRAILING: 8 │ #endif
// TRAILING: ╰────
// SLATE-FILECHECK-END TRAILING
// SLATE-FILECHECK-BEGIN TRUNCATED
// TRUNCATED: Error:   × semantic analysis failed
// TRUNCATED: Error:
// TRUNCATED: × invalid floating literal `1.5bf1`
// TRUNCATED: ╭─[tests/fixtures/sema/ir_bfloat16_invalid.c:11:20]
// TRUNCATED: 10 │ #ifdef TRUNCATED
// TRUNCATED: 11 │ __bf16 truncated = 1.5bf1;
// TRUNCATED: ·                    ──────
// TRUNCATED: 12 │ #endif
// TRUNCATED: ╰────
// SLATE-FILECHECK-END TRUNCATED
// SLATE-FILECHECK-BEGIN HEXNOEXP
// HEXNOEXP: Error:   × semantic analysis failed
// HEXNOEXP: Error:
// HEXNOEXP: × invalid floating literal `0x1.8bf16`
// HEXNOEXP: ╭─[tests/fixtures/sema/ir_bfloat16_invalid.c:15:26]
// HEXNOEXP: 14 │ #ifdef HEXNOEXP
// HEXNOEXP: 15 │ __bf16 hex_no_exponent = 0x1.8bf16;
// HEXNOEXP: ·                          ─────────
// HEXNOEXP: 16 │ #endif
// HEXNOEXP: ╰────
// SLATE-FILECHECK-END HEXNOEXP
// SLATE-FILECHECK-BEGIN MIXEDCASE
// MIXEDCASE: Error:   × semantic analysis failed
// MIXEDCASE: Error:
// MIXEDCASE: × invalid floating literal `1.5dF`
// MIXEDCASE: ╭─[tests/fixtures/sema/ir_bfloat16_invalid.c:19:21]
// MIXEDCASE: 18 │ #ifdef MIXEDCASE
// MIXEDCASE: 19 │ double mixed_case = 1.5dF;
// MIXEDCASE: ·                     ─────
// MIXEDCASE: 20 │ #endif
// MIXEDCASE: ╰────
// SLATE-FILECHECK-END MIXEDCASE
