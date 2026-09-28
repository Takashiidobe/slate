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
// SEMANTIC: ╭─[tests/fixtures/error/asm-memory-bit-field.c:3:1]
// SEMANTIC: 2 │
// SEMANTIC: 3 │ ╭─▶ void f(struct Pair *s) {
// SEMANTIC: 4 │ │       asm("# %0" : : "m"(s->b));
// SEMANTIC: 5 │ ╰─▶ }
// SEMANTIC: 6 │
// SEMANTIC: ╰────
// SLATE-FILECHECK-END SEMANTIC
