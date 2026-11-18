// SLATE-FILECHECK-ARGS --dump-ir
// SLATE-FILECHECK-ERROR SEMANTIC

struct rq;
extern struct rq requested __attribute__((aligned(64)));
unsigned long requested_alignment(void) { return __alignof__(requested); }

// SLATE-FILECHECK-BEGIN SEMANTIC
// SEMANTIC: Error:   × semantic analysis failed
// SEMANTIC: Error:
// SEMANTIC:   × incomplete field type
// SEMANTIC:    ╭─[tests/fixtures/error/clang/linux/x86_64/alignof_incomplete_object.c:4:50]
// SEMANTIC:  3 │ extern struct rq requested __attribute__((aligned(64)));
// SEMANTIC:  4 │ unsigned long requested_alignment(void) { return __alignof__(requested); }
// SEMANTIC:    ·                                                  ──────────────────────
// SEMANTIC:    ╰────
// SLATE-FILECHECK-END SEMANTIC
