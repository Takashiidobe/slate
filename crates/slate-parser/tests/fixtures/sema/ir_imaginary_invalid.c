// SLATE-FILECHECK-DEFINES INTEGER INTEGER
// SLATE-FILECHECK-DEFINES DECIMAL DECIMAL
// SLATE-FILECHECK-DEFINES RELATIONAL RELATIONAL
// SLATE-FILECHECK-DEFINES REMAINDER REMAINDER
// SLATE-FILECHECK-DEFINES COMPLEMENT COMPLEMENT
// SLATE-FILECHECK-DEFINES PREPROCESSOR PREPROCESSOR
// SLATE-FILECHECK-ERROR INTEGER
// SLATE-FILECHECK-ERROR DECIMAL
// SLATE-FILECHECK-ERROR RELATIONAL
// SLATE-FILECHECK-ERROR REMAINDER
// SLATE-FILECHECK-ERROR COMPLEMENT
// SLATE-FILECHECK-ERROR PREPROCESSOR
// SLATE-FILECHECK-ARGS --dump-ir

#ifdef INTEGER
int _Imaginary integer;
#endif
#ifdef DECIMAL
_Decimal64 _Imaginary decimal;
#endif
#ifdef PREPROCESSOR
#if 3i
#endif
#endif

int invalid(double _Imaginary y, double x) {
#ifdef RELATIONAL
    return y < x;
#endif
#ifdef REMAINDER
    return y % 2;
#endif
#ifdef COMPLEMENT
    return ~y;
#endif
    return 0;
}

// SLATE-FILECHECK-BEGIN INTEGER
// INTEGER: Error:   × expected declarator
// INTEGER: ╰─▶ expected declarator
// INTEGER: ╭─[tests/fixtures/sema/ir_imaginary_invalid.c:3:5]
// INTEGER: 2 │ #ifdef INTEGER
// INTEGER: 3 │ int _Imaginary integer;
// INTEGER: ·     ──────────
// INTEGER: 4 │ #endif
// INTEGER: ╰────
// SLATE-FILECHECK-END INTEGER
// SLATE-FILECHECK-BEGIN DECIMAL
// DECIMAL: Error:   × expected declarator
// DECIMAL: ╰─▶ expected declarator
// DECIMAL: ╭─[tests/fixtures/sema/ir_imaginary_invalid.c:6:12]
// DECIMAL: 5 │ #ifdef DECIMAL
// DECIMAL: 6 │ _Decimal64 _Imaginary decimal;
// DECIMAL: ·            ──────────
// DECIMAL: 7 │ #endif
// DECIMAL: ╰────
// SLATE-FILECHECK-END DECIMAL
// SLATE-FILECHECK-BEGIN RELATIONAL
// RELATIONAL: Error:   × invalid in this context: relational comparison requires real operands
// SLATE-FILECHECK-END RELATIONAL
// SLATE-FILECHECK-BEGIN REMAINDER
// REMAINDER: Error:   × invalid in this context: operator requires integer or real operands
// SLATE-FILECHECK-END REMAINDER
// SLATE-FILECHECK-BEGIN COMPLEMENT
// COMPLEMENT: Error:   × invalid in this context: bitwise complement of imaginary operand
// SLATE-FILECHECK-END COMPLEMENT
// SLATE-FILECHECK-BEGIN PREPROCESSOR
// PREPROCESSOR: Error:   × invalid #if expression: imaginary literal is not a constant expression
// PREPROCESSOR: ╰─▶ invalid #if expression: imaginary literal is not a constant expression
// PREPROCESSOR: ╭─[tests/fixtures/sema/ir_imaginary_invalid.c:9:5]
// PREPROCESSOR: 8 │ #ifdef PREPROCESSOR
// PREPROCESSOR: 9 │ #if 3i
// PREPROCESSOR: ·     ──
// PREPROCESSOR: 10 │ #endif
// PREPROCESSOR: ╰────
// SLATE-FILECHECK-END PREPROCESSOR
