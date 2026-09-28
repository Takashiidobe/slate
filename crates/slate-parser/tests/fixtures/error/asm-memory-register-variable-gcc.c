// SLATE-FILECHECK-ARGS --dump-ir
// SLATE-FILECHECK-FLAVOR gcc
void f(void) {
    register int r = 1;
    asm("# %0" : "=m"(r));
}

// SLATE-FILECHECK-ERROR SEMANTIC

// SLATE-FILECHECK-BEGIN SEMANTIC
// SEMANTIC: Error:   × semantic analysis failed
// SEMANTIC: Error:
// SEMANTIC: × invalid in this context: address of register variable requested
// SEMANTIC: ╭─[tests/fixtures/error/asm-memory-register-variable-gcc.c:1:1]
// SEMANTIC: 1 │ ╭─▶ void f(void) {
// SEMANTIC: 2 │ │       register int r = 1;
// SEMANTIC: 3 │ │       asm("# %0" : "=m"(r));
// SEMANTIC: 4 │ ╰─▶ }
// SEMANTIC: 5 │
// SEMANTIC: ╰────
// SLATE-FILECHECK-END SEMANTIC
