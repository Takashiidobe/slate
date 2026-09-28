// SLATE-FILECHECK-DEFINES REMAINDER REMAINDER
// SLATE-FILECHECK-DEFINES BITWISE BITWISE
// SLATE-FILECHECK-DEFINES COMPLEMENT COMPLEMENT
// SLATE-FILECHECK-DEFINES COMPLEX COMPLEX
// SLATE-FILECHECK-DEFINES SWITCH SWITCH
// SLATE-FILECHECK-DEFINES SPELLING SPELLING
// SLATE-FILECHECK-ERROR REMAINDER
// SLATE-FILECHECK-ERROR BITWISE
// SLATE-FILECHECK-ERROR COMPLEMENT
// SLATE-FILECHECK-ERROR COMPLEX
// SLATE-FILECHECK-ERROR SWITCH
// SLATE-FILECHECK-ERROR SPELLING
// SLATE-FILECHECK-ARGS --dump-ir

#ifdef SPELLING
_Sat int saturating_integer;
#endif

int invalid(_Accum a, _Accum b, double _Complex z) {
#ifdef REMAINDER
    return a % b;
#endif
#ifdef BITWISE
    return a & b;
#endif
#ifdef COMPLEMENT
    return ~a;
#endif
#ifdef COMPLEX
    return a + z;
#endif
#ifdef SWITCH
    switch (a) {
    default:
        return 1;
    }
#endif
    return 0;
}

// SLATE-FILECHECK-BEGIN REMAINDER
// REMAINDER: Error:   × semantic analysis failed
// REMAINDER: Error:
// REMAINDER: × operator requires integer or real operands
// REMAINDER: ╭─[tests/fixtures/error/clang/linux/x86_64/ir_fixed_point_invalid.c:8:12]
// REMAINDER: 7 │ #ifdef REMAINDER
// REMAINDER: 8 │     return a % b;
// REMAINDER: ·            ─────
// REMAINDER: 9 │ #endif
// REMAINDER: ╰────
// SLATE-FILECHECK-END REMAINDER
// SLATE-FILECHECK-BEGIN BITWISE
// BITWISE: Error:   × semantic analysis failed
// BITWISE: Error:
// BITWISE: × operator requires integer or real operands
// BITWISE: ╭─[tests/fixtures/error/clang/linux/x86_64/ir_fixed_point_invalid.c:11:12]
// BITWISE: 10 │ #ifdef BITWISE
// BITWISE: 11 │     return a & b;
// BITWISE: ·            ─────
// BITWISE: 12 │ #endif
// BITWISE: ╰────
// SLATE-FILECHECK-END BITWISE
// SLATE-FILECHECK-BEGIN COMPLEMENT
// COMPLEMENT: Error:   × semantic analysis failed
// COMPLEMENT: Error:
// COMPLEMENT: × bitwise complement of a fixed-point operand
// COMPLEMENT: ╭─[tests/fixtures/error/clang/linux/x86_64/ir_fixed_point_invalid.c:14:12]
// COMPLEMENT: 13 │ #ifdef COMPLEMENT
// COMPLEMENT: 14 │     return ~a;
// COMPLEMENT: ·            ──
// COMPLEMENT: 15 │ #endif
// COMPLEMENT: ╰────
// SLATE-FILECHECK-END COMPLEMENT
// SLATE-FILECHECK-BEGIN COMPLEX
// COMPLEX: Error:   × semantic analysis failed
// COMPLEX: Error:
// COMPLEX: × complex or imaginary operand with a fixed-point operand
// COMPLEX: ╭─[tests/fixtures/error/clang/linux/x86_64/ir_fixed_point_invalid.c:17:12]
// COMPLEX: 16 │ #ifdef COMPLEX
// COMPLEX: 17 │     return a + z;
// COMPLEX: ·            ─────
// COMPLEX: 18 │ #endif
// COMPLEX: ╰────
// SLATE-FILECHECK-END COMPLEX
// SLATE-FILECHECK-BEGIN SWITCH
// SWITCH: Error:   × semantic analysis failed
// SWITCH: Error:
// SWITCH: × noninteger switch discriminant
// SWITCH: ╭─[tests/fixtures/error/clang/linux/x86_64/ir_fixed_point_invalid.c:20:5]
// SWITCH: 19 │     #ifdef SWITCH
// SWITCH: 20 │ ╭─▶     switch (a) {
// SWITCH: 21 │ │       default:
// SWITCH: 22 │ │           return 1;
// SWITCH: 23 │ ╰─▶     }
// SWITCH: 24 │     #endif
// SWITCH: ╰────
// SLATE-FILECHECK-END SWITCH
// SLATE-FILECHECK-BEGIN SPELLING
// SPELLING: Error:   × cannot combine `int` with previous declaration specifiers
// SPELLING: ╰─▶ cannot combine `int` with previous declaration specifiers
// SPELLING: ╭─[tests/fixtures/error/clang/linux/x86_64/ir_fixed_point_invalid.c:3:6]
// SPELLING: 2 │ #ifdef SPELLING
// SPELLING: 3 │ _Sat int saturating_integer;
// SPELLING: ·      ───
// SPELLING: 4 │ #endif
// SPELLING: ╰────
// SLATE-FILECHECK-END SPELLING
