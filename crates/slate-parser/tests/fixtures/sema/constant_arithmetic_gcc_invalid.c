// SLATE-FILECHECK-DEFINES NEGATIVE NEGATIVE
// SLATE-FILECHECK-DEFINES INITIALIZER INITIALIZER
// SLATE-FILECHECK-ERROR NEGATIVE
// SLATE-FILECHECK-ERROR INITIALIZER
// SLATE-FILECHECK-ARGS --dump-ir --flavor=gcc

#ifdef NEGATIVE
enum GccConstants { SHIFT_NEGATIVE = 1 << -1 };
#endif

#ifdef INITIALIZER
int negative_shift_initializer = 1 << -1;
#endif

// SLATE-FILECHECK-BEGIN NEGATIVE
// NEGATIVE: Error:   × unsupported in numeric IR lowering: nonconstant or undefined integer
// SLATE-FILECHECK-END NEGATIVE
// SLATE-FILECHECK-BEGIN INITIALIZER
// INITIALIZER: Error:   × semantic analysis failed
// INITIALIZER: Error:
// INITIALIZER: × initializer element is not a compile-time constant: negative shift count
// INITIALIZER: ╭─[tests/fixtures/sema/constant_arithmetic_gcc_invalid.c:7:34]
// INITIALIZER: 6 │ #ifdef INITIALIZER
// INITIALIZER: 7 │ int negative_shift_initializer = 1 << -1;
// INITIALIZER: ·                                  ───────
// INITIALIZER: 8 │ #endif
// INITIALIZER: ╰────
// SLATE-FILECHECK-END INITIALIZER
