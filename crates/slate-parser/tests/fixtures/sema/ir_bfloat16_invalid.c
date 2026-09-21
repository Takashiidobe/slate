// SLATE-FILECHECK-DEFINES DECIMAL DECIMAL
// SLATE-FILECHECK-DEFINES COMPLEX COMPLEX
// SLATE-FILECHECK-DEFINES TRAILING TRAILING
// SLATE-FILECHECK-DEFINES TRUNCATED TRUNCATED
// SLATE-FILECHECK-DEFINES HEXNOEXP HEXNOEXP
// SLATE-FILECHECK-ERROR DECIMAL
// SLATE-FILECHECK-ERROR COMPLEX
// SLATE-FILECHECK-ERROR TRAILING
// SLATE-FILECHECK-ERROR TRUNCATED
// SLATE-FILECHECK-ERROR HEXNOEXP
// SLATE-FILECHECK-ARGS --dump-ir

#ifdef DECIMAL
_Decimal64 mixed(__bf16 x, _Decimal64 y) { return x + y; }
#endif

#ifdef COMPLEX
_Complex __bf16 complex_value;
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

// SLATE-FILECHECK-BEGIN DECIMAL
// DECIMAL: Error:   × invalid operands to binary expression: bf16 + d64
// SLATE-FILECHECK-END DECIMAL
// SLATE-FILECHECK-BEGIN COMPLEX
// COMPLEX: Error:   × expected declarator
// COMPLEX: ╰─▶ expected declarator
// COMPLEX: ╭─[tests/fixtures/sema/ir_bfloat16_invalid.c:7:10]
// COMPLEX: 6 │ #ifdef COMPLEX
// COMPLEX: 7 │ _Complex __bf16 complex_value;
// COMPLEX: ·          ──────
// COMPLEX: 8 │ #endif
// COMPLEX: ╰────
// SLATE-FILECHECK-END COMPLEX
// SLATE-FILECHECK-BEGIN TRAILING
// TRAILING: Error:   × semantic analysis failed
// TRAILING: Error:
// TRAILING: × invalid floating literal `1.5bf16l`
// TRAILING: ╭─[tests/fixtures/sema/ir_bfloat16_invalid.c:11:19]
// TRAILING: 10 │ #ifdef TRAILING
// TRAILING: 11 │ __bf16 trailing = 1.5bf16l;
// TRAILING: ·                   ────────
// TRAILING: 12 │ #endif
// TRAILING: ╰────
// SLATE-FILECHECK-END TRAILING
// SLATE-FILECHECK-BEGIN TRUNCATED
// TRUNCATED: Error:   × semantic analysis failed
// TRUNCATED: Error:
// TRUNCATED: × invalid floating literal `1.5bf1`
// TRUNCATED: ╭─[tests/fixtures/sema/ir_bfloat16_invalid.c:15:20]
// TRUNCATED: 14 │ #ifdef TRUNCATED
// TRUNCATED: 15 │ __bf16 truncated = 1.5bf1;
// TRUNCATED: ·                    ──────
// TRUNCATED: 16 │ #endif
// TRUNCATED: ╰────
// SLATE-FILECHECK-END TRUNCATED
// SLATE-FILECHECK-BEGIN HEXNOEXP
// HEXNOEXP: Error:   × semantic analysis failed
// HEXNOEXP: Error:
// HEXNOEXP: × invalid floating literal `0x1.8bf16`
// HEXNOEXP: ╭─[tests/fixtures/sema/ir_bfloat16_invalid.c:19:26]
// HEXNOEXP: 18 │ #ifdef HEXNOEXP
// HEXNOEXP: 19 │ __bf16 hex_no_exponent = 0x1.8bf16;
// HEXNOEXP: ·                          ─────────
// HEXNOEXP: 20 │ #endif
// HEXNOEXP: ╰────
// SLATE-FILECHECK-END HEXNOEXP
