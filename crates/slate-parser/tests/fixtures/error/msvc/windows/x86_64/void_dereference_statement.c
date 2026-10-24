// SLATE-FILECHECK-ARGS --dump-ir
// SLATE-FILECHECK-ERROR SEMANTIC

void f(void *p) { *p; }

// SLATE-FILECHECK-BEGIN SEMANTIC
// SEMANTIC: Error:   × semantic analysis failed
// SEMANTIC: Error:
// SEMANTIC:   × indirection of a void pointer
// SEMANTIC:    ╭─[tests/fixtures/error/msvc/windows/x86_64/void_dereference_statement.c:2:19]
// SEMANTIC:  1 │
// SEMANTIC:  2 │ void f(void *p) { *p; }
// SEMANTIC:    ·                   ──
// SEMANTIC:  3 │
// SEMANTIC:    ╰────
// SLATE-FILECHECK-END SEMANTIC
