// SLATE-FILECHECK-DEFINES DECIMAL DECIMAL
// SLATE-FILECHECK-DEFINES COMPLEX COMPLEX
// SLATE-FILECHECK-ERROR DECIMAL
// SLATE-FILECHECK-ERROR COMPLEX
// SLATE-FILECHECK-ARGS --dump-ir

#ifdef DECIMAL
_Decimal64 mixed(__bf16 x, _Decimal64 y) { return x + y; }
#endif

#ifdef COMPLEX
_Complex __bf16 complex_value;
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
