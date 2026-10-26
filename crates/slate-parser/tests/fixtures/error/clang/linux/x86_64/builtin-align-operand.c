// SLATE-FILECHECK-ERROR SEMA

double up(double value) { return __builtin_align_up(value, 8); }

// SLATE-FILECHECK-BEGIN SEMA
// SEMA: Error:   × semantic analysis failed
// SEMA: Error:
// SEMA:   × alignment builtin operand type
// SEMA:    ╭─[tests/fixtures/error/clang/linux/x86_64/builtin-align-operand.c:2:34]
// SEMA:  1 │
// SEMA:  2 │ double up(double value) { return __builtin_align_up(value, 8); }
// SEMA:    ·                                  ────────────────────────────
// SEMA:    ╰────
// SLATE-FILECHECK-END SEMA
