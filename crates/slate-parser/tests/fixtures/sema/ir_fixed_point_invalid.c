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
// REMAINDER: × invalid in this context: operator requires integer or real operands
// REMAINDER: ╭─[tests/fixtures/sema/ir_fixed_point_invalid.c:6:1]
// REMAINDER: 5 │
// REMAINDER: 6 │ ╭─▶ int invalid(_Accum a, _Accum b, double _Complex z) {
// REMAINDER: 7 │ │   #ifdef REMAINDER
// REMAINDER: 8 │ │       return a % b;
// REMAINDER: 9 │ │   #endif
// REMAINDER: 10 │ │   #ifdef BITWISE
// REMAINDER: 11 │ │       return a & b;
// REMAINDER: 12 │ │   #endif
// REMAINDER: 13 │ │   #ifdef COMPLEMENT
// REMAINDER: 14 │ │       return ~a;
// REMAINDER: 15 │ │   #endif
// REMAINDER: 16 │ │   #ifdef COMPLEX
// REMAINDER: 17 │ │       return a + z;
// REMAINDER: 18 │ │   #endif
// REMAINDER: 19 │ │   #ifdef SWITCH
// REMAINDER: 20 │ │       switch (a) {
// REMAINDER: 21 │ │       default:
// REMAINDER: 22 │ │           return 1;
// REMAINDER: 23 │ │       }
// REMAINDER: 24 │ │   #endif
// REMAINDER: 25 │ │       return 0;
// REMAINDER: 26 │ ╰─▶ }
// REMAINDER: 27 │
// REMAINDER: ╰────
// SLATE-FILECHECK-END REMAINDER
// SLATE-FILECHECK-BEGIN BITWISE
// BITWISE: Error:   × semantic analysis failed
// BITWISE: Error:
// BITWISE: × invalid in this context: operator requires integer or real operands
// BITWISE: ╭─[tests/fixtures/sema/ir_fixed_point_invalid.c:6:1]
// BITWISE: 5 │
// BITWISE: 6 │ ╭─▶ int invalid(_Accum a, _Accum b, double _Complex z) {
// BITWISE: 7 │ │   #ifdef REMAINDER
// BITWISE: 8 │ │       return a % b;
// BITWISE: 9 │ │   #endif
// BITWISE: 10 │ │   #ifdef BITWISE
// BITWISE: 11 │ │       return a & b;
// BITWISE: 12 │ │   #endif
// BITWISE: 13 │ │   #ifdef COMPLEMENT
// BITWISE: 14 │ │       return ~a;
// BITWISE: 15 │ │   #endif
// BITWISE: 16 │ │   #ifdef COMPLEX
// BITWISE: 17 │ │       return a + z;
// BITWISE: 18 │ │   #endif
// BITWISE: 19 │ │   #ifdef SWITCH
// BITWISE: 20 │ │       switch (a) {
// BITWISE: 21 │ │       default:
// BITWISE: 22 │ │           return 1;
// BITWISE: 23 │ │       }
// BITWISE: 24 │ │   #endif
// BITWISE: 25 │ │       return 0;
// BITWISE: 26 │ ╰─▶ }
// BITWISE: 27 │
// BITWISE: ╰────
// SLATE-FILECHECK-END BITWISE
// SLATE-FILECHECK-BEGIN COMPLEMENT
// COMPLEMENT: Error:   × semantic analysis failed
// COMPLEMENT: Error:
// COMPLEMENT: × invalid in this context: bitwise complement of a fixed-point operand
// COMPLEMENT: ╭─[tests/fixtures/sema/ir_fixed_point_invalid.c:6:1]
// COMPLEMENT: 5 │
// COMPLEMENT: 6 │ ╭─▶ int invalid(_Accum a, _Accum b, double _Complex z) {
// COMPLEMENT: 7 │ │   #ifdef REMAINDER
// COMPLEMENT: 8 │ │       return a % b;
// COMPLEMENT: 9 │ │   #endif
// COMPLEMENT: 10 │ │   #ifdef BITWISE
// COMPLEMENT: 11 │ │       return a & b;
// COMPLEMENT: 12 │ │   #endif
// COMPLEMENT: 13 │ │   #ifdef COMPLEMENT
// COMPLEMENT: 14 │ │       return ~a;
// COMPLEMENT: 15 │ │   #endif
// COMPLEMENT: 16 │ │   #ifdef COMPLEX
// COMPLEMENT: 17 │ │       return a + z;
// COMPLEMENT: 18 │ │   #endif
// COMPLEMENT: 19 │ │   #ifdef SWITCH
// COMPLEMENT: 20 │ │       switch (a) {
// COMPLEMENT: 21 │ │       default:
// COMPLEMENT: 22 │ │           return 1;
// COMPLEMENT: 23 │ │       }
// COMPLEMENT: 24 │ │   #endif
// COMPLEMENT: 25 │ │       return 0;
// COMPLEMENT: 26 │ ╰─▶ }
// COMPLEMENT: 27 │
// COMPLEMENT: ╰────
// SLATE-FILECHECK-END COMPLEMENT
// SLATE-FILECHECK-BEGIN COMPLEX
// COMPLEX: Error:   × semantic analysis failed
// COMPLEX: Error:
// COMPLEX: × invalid in this context: complex or imaginary operand with a fixed-point
// COMPLEX: ╭─[tests/fixtures/sema/ir_fixed_point_invalid.c:6:1]
// COMPLEX: 5 │
// COMPLEX: 6 │ ╭─▶ int invalid(_Accum a, _Accum b, double _Complex z) {
// COMPLEX: 7 │ │   #ifdef REMAINDER
// COMPLEX: 8 │ │       return a % b;
// COMPLEX: 9 │ │   #endif
// COMPLEX: 10 │ │   #ifdef BITWISE
// COMPLEX: 11 │ │       return a & b;
// COMPLEX: 12 │ │   #endif
// COMPLEX: 13 │ │   #ifdef COMPLEMENT
// COMPLEX: 14 │ │       return ~a;
// COMPLEX: 15 │ │   #endif
// COMPLEX: 16 │ │   #ifdef COMPLEX
// COMPLEX: 17 │ │       return a + z;
// COMPLEX: 18 │ │   #endif
// COMPLEX: 19 │ │   #ifdef SWITCH
// COMPLEX: 20 │ │       switch (a) {
// COMPLEX: 21 │ │       default:
// COMPLEX: 22 │ │           return 1;
// COMPLEX: 23 │ │       }
// COMPLEX: 24 │ │   #endif
// COMPLEX: 25 │ │       return 0;
// COMPLEX: 26 │ ╰─▶ }
// COMPLEX: 27 │
// COMPLEX: ╰────
// SLATE-FILECHECK-END COMPLEX
// SLATE-FILECHECK-BEGIN SWITCH
// SWITCH: Error:   × semantic analysis failed
// SWITCH: Error:
// SWITCH: × unsupported in numeric IR lowering: noninteger switch discriminant
// SWITCH: ╭─[tests/fixtures/sema/ir_fixed_point_invalid.c:6:1]
// SWITCH: 5 │
// SWITCH: 6 │ ╭─▶ int invalid(_Accum a, _Accum b, double _Complex z) {
// SWITCH: 7 │ │   #ifdef REMAINDER
// SWITCH: 8 │ │       return a % b;
// SWITCH: 9 │ │   #endif
// SWITCH: 10 │ │   #ifdef BITWISE
// SWITCH: 11 │ │       return a & b;
// SWITCH: 12 │ │   #endif
// SWITCH: 13 │ │   #ifdef COMPLEMENT
// SWITCH: 14 │ │       return ~a;
// SWITCH: 15 │ │   #endif
// SWITCH: 16 │ │   #ifdef COMPLEX
// SWITCH: 17 │ │       return a + z;
// SWITCH: 18 │ │   #endif
// SWITCH: 19 │ │   #ifdef SWITCH
// SWITCH: 20 │ │       switch (a) {
// SWITCH: 21 │ │       default:
// SWITCH: 22 │ │           return 1;
// SWITCH: 23 │ │       }
// SWITCH: 24 │ │   #endif
// SWITCH: 25 │ │       return 0;
// SWITCH: 26 │ ╰─▶ }
// SWITCH: 27 │
// SWITCH: ╰────
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
