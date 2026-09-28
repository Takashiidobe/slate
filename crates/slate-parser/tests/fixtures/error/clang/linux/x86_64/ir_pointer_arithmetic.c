// SLATE-FILECHECK-DEFINES FLOAT -DFLOAT
// SLATE-FILECHECK-DEFINES INCOMPATIBLE -DINCOMPATIBLE
// SLATE-FILECHECK-DEFINES INCOMPLETE -DINCOMPLETE
// SLATE-FILECHECK-ERROR FLOAT
// SLATE-FILECHECK-ERROR INCOMPATIBLE
// SLATE-FILECHECK-ERROR INCOMPLETE
// SLATE-FILECHECK-ARGS --dump-ir

#ifdef FLOAT
void bad(int *p) { p + 1.0; }
#endif
#ifdef INCOMPATIBLE
void bad(int *p, long *q) { p - q; }
#endif
#ifdef INCOMPLETE
struct S;
void bad(struct S *p) { p++; }
#endif

// SLATE-FILECHECK-BEGIN FLOAT
// FLOAT: Error:   × semantic analysis failed
// FLOAT: Error:
// FLOAT: × noninteger pointer offset
// FLOAT: ╭─[tests/fixtures/error/clang/linux/x86_64/ir_pointer_arithmetic.c:3:20]
// FLOAT: 2 │ #ifdef FLOAT
// FLOAT: 3 │ void bad(int *p) { p + 1.0; }
// FLOAT: ·                    ───────
// FLOAT: 4 │ #endif
// FLOAT: ╰────
// SLATE-FILECHECK-END FLOAT
// SLATE-FILECHECK-BEGIN INCOMPATIBLE
// INCOMPATIBLE: Error:   × semantic analysis failed
// INCOMPATIBLE: Error:
// INCOMPATIBLE: × incompatible pointer subtraction
// INCOMPATIBLE: ╭─[tests/fixtures/error/clang/linux/x86_64/ir_pointer_arithmetic.c:6:29]
// INCOMPATIBLE: 5 │ #ifdef INCOMPATIBLE
// INCOMPATIBLE: 6 │ void bad(int *p, long *q) { p - q; }
// INCOMPATIBLE: ·                             ─────
// INCOMPATIBLE: 7 │ #endif
// INCOMPATIBLE: ╰────
// SLATE-FILECHECK-END INCOMPATIBLE
// SLATE-FILECHECK-BEGIN INCOMPLETE
// INCOMPLETE: Error:   × semantic analysis failed
// INCOMPLETE: Error:
// INCOMPLETE: × incomplete field type
// INCOMPLETE: ╭─[tests/fixtures/error/clang/linux/x86_64/ir_pointer_arithmetic.c:10:25]
// INCOMPLETE: 9 │ struct S;
// INCOMPLETE: 10 │ void bad(struct S *p) { p++; }
// INCOMPLETE: ·                         ───
// INCOMPLETE: 11 │ #endif
// INCOMPLETE: ╰────
// SLATE-FILECHECK-END INCOMPLETE
