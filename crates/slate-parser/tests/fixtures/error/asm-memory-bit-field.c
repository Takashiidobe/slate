// SLATE-FILECHECK-ARGS --dump-ir
struct Pair { int b : 3; int w; };

void f(struct Pair *s) {
    asm("# %0" : : "m"(s->b));
}

// SLATE-FILECHECK-ERROR SEMANTIC

// SLATE-FILECHECK-BEGIN SEMANTIC
// SEMANTIC: Error:   × semantic analysis failed
// SEMANTIC: Error:
// SEMANTIC: × invalid in this context: address of a bit-field
// SEMANTIC: ╭─[tests/fixtures/error/asm-memory-bit-field.c:4:5]
// SEMANTIC: 3 │ void f(struct Pair *s) {
// SEMANTIC: 4 │     asm("# %0" : : "m"(s->b));
// SEMANTIC: ·     ──────────────────────────
// SEMANTIC: 5 │ }
// SEMANTIC: ╰────
// SLATE-FILECHECK-END SEMANTIC
