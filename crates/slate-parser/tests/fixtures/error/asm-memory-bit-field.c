// SLATE-FILECHECK-ARGS --dump-ir
struct Pair { int b : 3; int w; };

void f(struct Pair *s) {
    asm("# %0" : : "m"(s->b));
}

// SLATE-FILECHECK-ERROR SEMANTIC

// SLATE-FILECHECK-BEGIN SEMANTIC
// SEMANTIC: Error:   × invalid in this context: address of a bit-field
// SLATE-FILECHECK-END SEMANTIC
