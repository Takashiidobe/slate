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
// REMAINDER: Error:   × invalid in this context: operator requires integer or real operands
// SLATE-FILECHECK-END REMAINDER
// SLATE-FILECHECK-BEGIN BITWISE
// BITWISE: Error:   × invalid in this context: operator requires integer or real operands
// SLATE-FILECHECK-END BITWISE
// SLATE-FILECHECK-BEGIN COMPLEMENT
// COMPLEMENT: Error:   × invalid in this context: bitwise complement of a fixed-point operand
// SLATE-FILECHECK-END COMPLEMENT
// SLATE-FILECHECK-BEGIN COMPLEX
// COMPLEX: Error:   × invalid in this context: complex or imaginary operand with a fixed-point
// SLATE-FILECHECK-END COMPLEX
// SLATE-FILECHECK-BEGIN SWITCH
// SWITCH: Error:   × unsupported in numeric IR lowering: noninteger switch discriminant
// SLATE-FILECHECK-END SWITCH
// SLATE-FILECHECK-BEGIN SPELLING
// SPELLING: Error:   × cannot combine `int` with previous declaration specifiers
// SPELLING: ╰─▶ cannot combine `int` with previous declaration specifiers
// SPELLING: ╭─[tests/fixtures/sema/ir_fixed_point_invalid.c:3:6]
// SPELLING: 2 │ #ifdef SPELLING
// SPELLING: 3 │ _Sat int saturating_integer;
// SPELLING: ·      ───
// SPELLING: 4 │ #endif
// SPELLING: ╰────
// SLATE-FILECHECK-END SPELLING
