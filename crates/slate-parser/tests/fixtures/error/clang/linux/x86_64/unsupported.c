// SLATE-FILECHECK-DEFINES FLOAT_REM FLOAT_REM
// SLATE-FILECHECK-DEFINES FLOAT_BITAND FLOAT_BITAND
// SLATE-FILECHECK-DEFINES FLOAT_SHIFT FLOAT_SHIFT
// SLATE-FILECHECK-DEFINES COMPOUND_ASSIGN COMPOUND_ASSIGN
// SLATE-FILECHECK-DEFINES FLOAT_BITNOT FLOAT_BITNOT
// SLATE-FILECHECK-DEFINES PRE_INCREMENT PRE_INCREMENT
// SLATE-FILECHECK-DEFINES POST_DECREMENT POST_DECREMENT
// SLATE-FILECHECK-DEFINES BUILTIN BUILTIN
// SLATE-FILECHECK-ERROR FLOAT_REM
// SLATE-FILECHECK-ERROR FLOAT_BITAND
// SLATE-FILECHECK-ERROR FLOAT_SHIFT
// SLATE-FILECHECK-ERROR COMPOUND_ASSIGN
// SLATE-FILECHECK-ERROR FLOAT_BITNOT
// SLATE-FILECHECK-ERROR PRE_INCREMENT
// SLATE-FILECHECK-ERROR POST_DECREMENT
// SLATE-FILECHECK-ERROR BUILTIN
// SLATE-FILECHECK-ARGS --dump-ir-expressions

void unsupported(void) {
#ifdef BUILTIN
    __builtin_clz(1);
#endif
#ifdef FLOAT_REM
    1.0 % 2.0;
#endif
#ifdef FLOAT_BITAND
    1.0 & 2.0;
#endif
#ifdef FLOAT_SHIFT
    1 << 2.0;
#endif
#ifdef FLOAT_BITNOT
    ~1.0;
#endif
}

#ifdef COMPOUND_ASSIGN
void assign(int x) {
    x += 1;
}
#endif

#ifdef PRE_INCREMENT
void pre_increment(int x) {
    ++x;
}
#endif

#ifdef POST_DECREMENT
void post_decrement(int x) {
    x--;
}
#endif

// SLATE-FILECHECK-BEGIN FLOAT_REM
// FLOAT_REM: Error:   × semantic analysis failed
// FLOAT_REM: Error:
// FLOAT_REM: × invalid operands to binary expression: f64 % f64
// FLOAT_REM: ╭─[tests/fixtures/error/clang/linux/x86_64/unsupported.c:7:5]
// FLOAT_REM: 6 │ #ifdef FLOAT_REM
// FLOAT_REM: 7 │     1.0 % 2.0;
// FLOAT_REM: ·     ─────────
// FLOAT_REM: 8 │ #endif
// FLOAT_REM: ╰────
// SLATE-FILECHECK-END FLOAT_REM
// SLATE-FILECHECK-BEGIN FLOAT_BITAND
// FLOAT_BITAND: Error:   × semantic analysis failed
// FLOAT_BITAND: Error:
// FLOAT_BITAND: × invalid operands to binary expression: f64 & f64
// FLOAT_BITAND: ╭─[tests/fixtures/error/clang/linux/x86_64/unsupported.c:10:5]
// FLOAT_BITAND: 9 │ #ifdef FLOAT_BITAND
// FLOAT_BITAND: 10 │     1.0 & 2.0;
// FLOAT_BITAND: ·     ─────────
// FLOAT_BITAND: 11 │ #endif
// FLOAT_BITAND: ╰────
// SLATE-FILECHECK-END FLOAT_BITAND
// SLATE-FILECHECK-BEGIN FLOAT_SHIFT
// FLOAT_SHIFT: Error:   × semantic analysis failed
// FLOAT_SHIFT: Error:
// FLOAT_SHIFT: × invalid operands to binary expression: i32 << f64
// FLOAT_SHIFT: ╭─[tests/fixtures/error/clang/linux/x86_64/unsupported.c:13:5]
// FLOAT_SHIFT: 12 │ #ifdef FLOAT_SHIFT
// FLOAT_SHIFT: 13 │     1 << 2.0;
// FLOAT_SHIFT: ·     ────────
// FLOAT_SHIFT: 14 │ #endif
// FLOAT_SHIFT: ╰────
// SLATE-FILECHECK-END FLOAT_SHIFT
// SLATE-FILECHECK-BEGIN COMPOUND_ASSIGN
// COMPOUND_ASSIGN: Error:   × nonliteral numeric expression
// SLATE-FILECHECK-END COMPOUND_ASSIGN
// SLATE-FILECHECK-BEGIN FLOAT_BITNOT
// FLOAT_BITNOT: Error:   × semantic analysis failed
// FLOAT_BITNOT: Error:
// FLOAT_BITNOT: × invalid argument type to unary expression: ~f64
// FLOAT_BITNOT: ╭─[tests/fixtures/error/clang/linux/x86_64/unsupported.c:16:5]
// FLOAT_BITNOT: 15 │ #ifdef FLOAT_BITNOT
// FLOAT_BITNOT: 16 │     ~1.0;
// FLOAT_BITNOT: ·     ────
// FLOAT_BITNOT: 17 │ #endif
// FLOAT_BITNOT: ╰────
// SLATE-FILECHECK-END FLOAT_BITNOT
// SLATE-FILECHECK-BEGIN PRE_INCREMENT
// PRE_INCREMENT: Error:   × nonconstant or unknown identifier
// SLATE-FILECHECK-END PRE_INCREMENT
// SLATE-FILECHECK-BEGIN POST_DECREMENT
// POST_DECREMENT: Error:   × nonliteral numeric expression
// SLATE-FILECHECK-END POST_DECREMENT
// SLATE-FILECHECK-BEGIN BUILTIN
// BUILTIN: Error:   × nonliteral numeric expression
// SLATE-FILECHECK-END BUILTIN
