// SLATE-FILECHECK-ARGS --dump-ir
void f(void) {
    register int r = 1;
    asm("# %0" : "=m"(r));
}

// SLATE-FILECHECK-ERROR SEMANTIC

// SLATE-FILECHECK-BEGIN SEMANTIC
// SEMANTIC: Error:   × semantic analysis failed
// SEMANTIC: Error:
// SEMANTIC: × invalid in this context: address of register variable requested
// SEMANTIC: ╭─[tests/fixtures/error/gcc/linux/x86_64/asm-memory-register-variable.c:3:5]
// SEMANTIC: 2 │     register int r = 1;
// SEMANTIC: 3 │     asm("# %0" : "=m"(r));
// SEMANTIC: ·     ──────────────────────
// SEMANTIC: 4 │ }
// SEMANTIC: ╰────
// SLATE-FILECHECK-END SEMANTIC
