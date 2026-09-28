// SLATE-FILECHECK-ARGS --dump-ir
void f(int x) {
    asm("# %0" : : "m"(x + 1));
}

// SLATE-FILECHECK-ERROR SEMANTIC

// SLATE-FILECHECK-BEGIN SEMANTIC
// SEMANTIC: Error:   × semantic analysis failed
// SEMANTIC: Error:
// SEMANTIC: × invalid in this context: asm input with a memory-only constraint is not an
// SEMANTIC: ╭─[tests/fixtures/error/clang/linux/x86_64/asm-memory-not-lvalue.c:2:5]
// SEMANTIC: 1 │ void f(int x) {
// SEMANTIC: 2 │     asm("# %0" : : "m"(x + 1));
// SEMANTIC: ·     ───────────────────────────
// SEMANTIC: 3 │ }
// SEMANTIC: ╰────
// SLATE-FILECHECK-END SEMANTIC
