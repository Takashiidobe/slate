// SLATE-FILECHECK-ARGS --dump-ir
void f(int x) {
    asm("# %0" : : "m"(x + 1));
}

// SLATE-FILECHECK-ERROR SEMANTIC

// SLATE-FILECHECK-BEGIN SEMANTIC
// SEMANTIC: Error:   × invalid in this context: asm input with a memory-only constraint is not an
// SLATE-FILECHECK-END SEMANTIC
