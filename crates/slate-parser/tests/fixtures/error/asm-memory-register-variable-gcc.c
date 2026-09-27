// SLATE-FILECHECK-ARGS --dump-ir
// SLATE-FILECHECK-FLAVOR gcc
void f(void) {
    register int r = 1;
    asm("# %0" : "=m"(r));
}

// SLATE-FILECHECK-ERROR SEMANTIC

// SLATE-FILECHECK-BEGIN SEMANTIC
// SEMANTIC: Error:   × invalid in this context: address of register variable requested
// SLATE-FILECHECK-END SEMANTIC
