// SLATE-FILECHECK-DEFINES NEGATIVE NEGATIVE
// SLATE-FILECHECK-DEFINES INITIALIZER INITIALIZER
// SLATE-FILECHECK-ERROR NEGATIVE
// SLATE-FILECHECK-ERROR INITIALIZER
// SLATE-FILECHECK-ARGS --dump-ir

#ifdef NEGATIVE
enum GccConstants { SHIFT_NEGATIVE = 1 << -1 };
#endif

#ifdef INITIALIZER
int negative_shift_initializer = 1 << -1;
#endif

// SLATE-FILECHECK-BEGIN NEGATIVE
// NEGATIVE: Error:   × semantic analysis failed
// NEGATIVE: Error:
// NEGATIVE: × nonconstant or undefined integer expression
// NEGATIVE: ╭─[tests/fixtures/error/gcc/linux/x86_64/constant_arithmetic_gcc_invalid.c:3:1]
// NEGATIVE: 2 │ #ifdef NEGATIVE
// NEGATIVE: 3 │ enum GccConstants { SHIFT_NEGATIVE = 1 << -1 };
// NEGATIVE: · ───────────────────────────────────────────────
// NEGATIVE: 4 │ #endif
// NEGATIVE: ╰────
// SLATE-FILECHECK-END NEGATIVE
// SLATE-FILECHECK-BEGIN INITIALIZER
// INITIALIZER: Error:   × semantic analysis failed
// INITIALIZER: Error:
// INITIALIZER: × initializer element is not a compile-time constant: negative shift count
// INITIALIZER: ╭─[tests/fixtures/error/gcc/linux/x86_64/constant_arithmetic_gcc_invalid.c:7:34]
// INITIALIZER: 6 │ #ifdef INITIALIZER
// INITIALIZER: 7 │ int negative_shift_initializer = 1 << -1;
// INITIALIZER: ·                                  ───────
// INITIALIZER: 8 │ #endif
// INITIALIZER: ╰────
// SLATE-FILECHECK-END INITIALIZER
